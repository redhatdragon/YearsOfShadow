#include "UltraleapHandController.h"
#include <UnigineVisualizer.h>
#include <UnigineConsole.h>
#include <UnigineWidgets.h>
#include "../VRPlayerSpawner.h"
#include "../../Utils.h"

using namespace Unigine;
using namespace Unigine::Math;
using namespace Unigine::Plugins;

REGISTER_COMPONENT(UltraleapHandControllerVarjo);
REGISTER_COMPONENT(MeshSkinnedHandMapper);

UltraleapFinger *get_finger_by_id(const UltraleapHand *hand, int finger_type)
{
	switch (finger_type)
	{
		case 0: return hand->getFingerThumb();
		case 1: return hand->getFingerIndex();
		case 2: return hand->getFingerMiddle();
		case 3: return hand->getFingerRing();
		case 4: return hand->getFingerPinky();
	}

	return nullptr;
}

UltraleapBone *get_bone_by_id(const UltraleapFinger *finger, int bone_type)
{
	switch (bone_type)
	{
		case 0: return finger->getBoneMetacarpal();
		case 1: return finger->getBoneProximal();
		case 2: return finger->getBoneIntermediate();
		case 3: return finger->getBoneDistal();
	}

	return nullptr;
}

const int BONE_NUM_TYPES = 4;


void UltraleapHandControllerVarjo::post_init()
{
	ultraleap = Ultraleap::get();
	if (!ultraleap)
	{
		node->setEnabled(false);
		return;
	}

	ultraleap->setStreamImages(true);
	ultraleap->setBackgroundUpdate(true);
	ultraleap->setAllowPauseResume(true);

	VRPlayerSpawner *spawner = ComponentSystem::get()->getComponent<VRPlayerSpawner>(player_spawner_component.get());
	if (!spawner)
	{
		Log::error("can't find VRPlayerSpawnerComponent!\n");
		node->setEnabled(false);
		return;
	}
	vr_player_main = dynamic_cast<VRPlayerVR *>(spawner->getSpawnedPlayer());
	if (!vr_player_main)
	{
		Log::error("can't get spawned VRPlayerVR\n");
		node->setEnabled(false);
		return;
	}


	player = checked_ptr_cast<PlayerDummy>(vr_player_main->getPlayer());
	
	hands_init();

	teleport_init();
	grab_init();

	if (object_gui)
		object_gui.deleteLater();

	object_gui = vr_player_main->getObjectGui();
	if (object_gui)
	{
		if (gui_spawn_point.get())
		{
			object_gui->setParent(gui_spawn_point.get());
			object_gui->setTransform(Mat4_identity);
		}
		else
		{
			object_gui->setParent(controller_0);
			object_gui->setTransform(Mat4(rotateY(180.0f)) * object_gui->getTransform());
		}
	}

	if (visualize_hands)
		Visualizer::setEnabled(1);

	Log::warning("Ultraleap is ready!\n");

	if (show_help)
	{
		helper = ObjectGui::create(0.45, 0.25);
		helper->setScreenSize(900, 500);
		helper->setMouseMode(ObjectGui::MOUSE_VIRTUAL);

		WidgetSpritePtr icon = WidgetSprite::create(helper->getGui(), help_img.get());
		icon->setWidth(900);
		icon->setHeight(500);
		icon->setPosition(0, 0);
		helper->getGui()->addChild(icon, Gui::ALIGN_FIXED | Gui::ALIGN_BACKGROUND);

		WidgetButtonPtr ok = WidgetButton::create(helper->getGui(), "CLOSE");
		ok->addCallback(Gui::CLICKED, MakeCallback(this, &UltraleapHandControllerVarjo::close_helper));
		ok->setPosition(400, 400);
		ok->setWidth(100);
		ok->setFontSize(22);
		helper->getGui()->addChild(ok, Gui::ALIGN_OVERLAP | Gui::ALIGN_FIXED);
		helper->setEnabled(false);
	}
}

void UltraleapHandControllerVarjo::update()
{
	if (!ultraleap)
		return;

	if (ultraleap->getConnectionStatus() != Ultraleap::CONNECTION_STATUS_CONNECTED) // what about handshake incompleate status?
		return;

	ul_device = ultraleap->getDevice(0);
	if (!ul_device)
		return;

	// for static ultraleap - you need to track transform of device...
	//ul_device->setTransformMode(UltraleapDevice::TRANSFORM_MODE_MANUAL);
	//auto ppp = Game::getPlayer();
	//ul_device->setTransform(ppp->getCamera()->getIModelview());

	update_hand(ul_device->getLeftHand());
	update_hand(ul_device->getRightHand());


	teleport_update();
	menu_update();
	gui_update(ul_device->getLeftHand());
	gui_update(ul_device->getRightHand());

	if (mapper)
		mapper->update(ul_device->getLeftHand(), ul_device->getRightHand());

	if (helper)
	{
		auto head_node = vr_player_main->getHead();
		helper->setEnabled(true);
		helper->setWorldTransform(setTo(head_node->getWorldTransform() * Vec3(head_node->getWorldDirection(AXIS_NY) * 0.4f), head_node->getWorldPosition(), vec3_up, AXIS_Z));
	}
}

void UltraleapHandControllerVarjo::update_hand(Plugins::UltraleapHand *hand)
{
	if (!hand)
		return;

	int num = hand->getType();

	auto new_time = hand->getVisibleTime();
	bool the_same_time = hand_visible_time[num] == new_time;
	if (the_same_time)
	{
		invisible_time[num] += Game::getIFps();
		if (invisible_time[num] > max_invisible_time)
			controller_valid[num] = false;
	} else
	{
		controller_valid[num] = true;
		invisible_time[num] = 0.0f;
		hand_visible_time[num] = new_time;
	}


	Vec3 normal = Vec3(-hand->getPalmNormal());
	Vec3 dir = Vec3(hand->getDirection());
	Vec3 right = cross(dir, normal).normalize();
	Mat4 basis;
	basis.setColumn3(0, right);
	basis.setColumn3(1, dir);
	basis.setColumn3(2, normal);
	basis.setColumn3(3, hand->getPalmPosition());
	controller[num]->setWorldTransform(basis);

	push_hand_linear_velocity(num, vec3(hand->getPalmVelocity() * hand_force_multiplier));
	//push_hand_angular_velocity(num, );

	grab_update(num, controller_valid[num], getControllerAxis(num, BUTTON::GRIP), getControllerButtonDown(num, BUTTON::TRIGGER));

	if (visualize_hands)
		draw_debug(hand);
}

void UltraleapHandControllerVarjo::shutdown()
{
}

void UltraleapHandControllerVarjo::hands_init()
{
	controller[0] = checked_ptr_cast<NodeReference>(controller_0.get());
	controller[1] = checked_ptr_cast<NodeReference>(controller_1.get());

	hand_linear_velocity.clear();
	hand_angular_velocity.clear();

	for (int i = 0; i < CONTROLLER_COUNT; i++)
	{
		controller_ref[i] = controller[i]->getReference();

		controller_obj[i].clear();
		find_obj_in_children(controller_ref[i], &controller_obj[i]);

		Vector<vec3> lv;
		hand_linear_velocity.append(lv);

		Vector<vec3> av;
		hand_angular_velocity.append(av);
	}

	if (noderef_hands_skinned_mesh.get())
	{
		hands_skinned = checked_ptr_cast <ObjectMeshSkinned>(noderef_hands_skinned_mesh.get());
		if (!hands_skinned)
		{
			auto nr = checked_ptr_cast<NodeReference>(noderef_hands_skinned_mesh.get());
			if (nr)
				hands_skinned = checked_ptr_cast <ObjectMeshSkinned>(nr->getReference());
		}

		if (hands_skinned)
		{
			mapper = ComponentSystem::get()->getComponent<MeshSkinnedHandMapper>(hands_skinned);
		}

	}
}

void UltraleapHandControllerVarjo::draw_debug(UltraleapHand *hand) const
{
	if (!hand)
		return;

	int num = hand->getType();

	vec4 hand_color(num, teleport_button_pressed[num], num-1, 1);

	String formated_text = String::format("%s: '%s' (%ld) \n", num ? "right" : "left", hand->isVisible() ? "visible" : "invisible", hand->getVisibleTime());

	formated_text += String::format("Pinch distance: %f\nPinch strength: %f\nGrab Angle: %f\nGrab Strength: %f\n",
		hand->getPinchDistance(), hand->getPinchStrength(), hand->getGrabAngle(), hand->getGrabStrength());
	formated_text += String::format("fingers %d %d %d %d %d\n",
		hand->getFingerThumb()->isExtended(), hand->getFingerIndex()->isExtended(), hand->getFingerMiddle()->isExtended(), hand->getFingerRing()->isExtended(), hand->getFingerPinky()->isExtended());

	auto palm_pos = hand->getPalmPosition();
	Mat4 transform{};
	transform.setTranslate(palm_pos);
	Visualizer::renderSphere(0.005f, transform, hand_color);

	auto wrist_pos = hand->getArm()->getPositionWrist();
	transform.setTranslate(wrist_pos);
	Visualizer::renderSphere(0.003f, transform, hand_color);

	auto arm_pos = hand->getArm()->getPositionElbow();
	transform.setTranslate(arm_pos);
	Visualizer::renderSphere(0.007f, transform, hand_color);

	Visualizer::renderLine3D(palm_pos, wrist_pos, hand_color);
	Visualizer::renderLine3D(arm_pos, wrist_pos, hand_color);

	formated_text += String::format("palm_pos: %f %f %f\n", palm_pos.x, palm_pos.y, palm_pos.z);
	Console::onscreenMessageLine(formated_text);

	for (int i = 0; i < UltraleapFinger::NUM_TYPES; i++)
	{
		const auto &finger = get_finger_by_id(hand, i);

		Visualizer::renderLine3D(wrist_pos, finger->getBoneMetacarpal()->getJointBeginPosition(), hand_color);

		float start_radius = 0.003f;
		const float radius_step = -0.0005f;
		for (auto j = 0; j < UltraleapBone::NUM_TYPES; ++j, start_radius += radius_step)
		{
			const auto &bone = get_bone_by_id(finger, j);

			transform.setTranslate(bone->getJointBeginPosition());
			Visualizer::renderSphere(start_radius, transform, hand_color);
			Visualizer::renderLine3D(bone->getJointBeginPosition(), bone->getJointEndPosition(), hand_color);
		}

		transform.setTranslate(finger->getBoneDistal()->getJointEndPosition()); // distal bone - its end of finger
		Visualizer::renderSphere(start_radius, transform, hand_color);
	}
}

int UltraleapHandControllerVarjo::getControllerButton(int controller_num, int button)
{
	if (!ul_device)
		return 0;
	UltraleapHand *hand = controller_num == 0 ? ul_device->getLeftHand() : ul_device->getRightHand();
	if (!hand || !controller_valid[controller_num])
		return 0;

	switch (button)
	{
		case TRIGGER: return hand->getPinchStrength() > 0.65;
		case GRIP: return hand->getGrabStrength() > 0.65;
	}
	return 0;
}

float UltraleapHandControllerVarjo::getControllerAxis(int controller_num, int button)
{
	if (!ul_device)
		return 0.0f;
	UltraleapHand *hand = controller_num == 0 ? ul_device->getLeftHand() : ul_device->getRightHand();
	if (!hand || !controller_valid[controller_num])
		return 0.0f;

	switch (button)
	{
		case TRIGGER: return hand->getPinchStrength();
		case GRIP: return hand->getGrabStrength();
	}

	return 0.0f;
}

Mat4 UltraleapHandControllerVarjo::getControllerTransform(int controller_num)
{
	return controller[controller_num]->getWorldTransform();
}

void UltraleapHandControllerVarjo::teleport_update()
{
	if (!controller_valid[1])
		return;

	UltraleapHand* left_hand = ul_device->getLeftHand();
	UltraleapHand* right_hand = ul_device->getRightHand();

	WorldBoundSphere bs = WorldBoundSphere(right_hand->getArm()->getCenter(), button_hand_radius);
	bool gesture_activate = bs.inside(left_hand->getFingerIndex()->getBoneDistal()->getJointEndPosition());
	if (gesture_activate && controller_valid[0])
	{
		teleport_delay_active_current += Game::getIFps();

		if (teleport_delay_active_current > 0)
		{
			Visualizer::renderSolidSphere(button_hand_radius, translate(right_hand->getArm()->getCenter()), lerp(vec4(0,1,0,0), vec4(0,1,0,1), teleport_delay_active_current / teleport_delay_max));
			Visualizer::renderSolidSphere(0.005, translate(left_hand->getFingerIndex()->getBoneDistal()->getJointEndPosition()), vec4_blue);
		}
		
		if (teleport_delay_active_current > teleport_delay_max)
		{
			teleport_delay_active_current = -teleport_delay_max;
			teleport_delay = 0.0f;
			teleport_active = !teleport_active;
			if (!teleport_active)
			{
				teleport_ray->setEnabled(0);
				teleport_forbidden_marker->setEnabled(0);
				teleport_allowed_marker->setEnabled(0);
			}
		}
	}
	else
	{
		teleport_delay_active_current = 0.0f;
	}

	teleport_delay += Game::getIFps();

	if (!teleport_active || teleport_delay < teleport_delay_max)
		return;

	bool button_pressed = right_hand->getGrabAngle() < 0.001f;


	vec3 dir = right_hand->getPalmNormal();

	Vec3 pos1 = controller[1]->getWorldPosition();
	Vec3 pos2 = controller[1]->getWorldPosition() + Vec3(dir) * player->getZFar();

		ObjectPtr hitObj = World::getIntersection(pos1, pos2, ray_intersection_mask, intersection);
		if (hitObj)
		{
			int i = 0;
			for (; i < hitObj->getNumSurfaces(); i++)
				if (hitObj->getIntersectionMask(i) & teleportation_mask)
				{
					// show marker
						pos2 = intersection->getPoint() + Vec3(0, 0, 0.1f);
						teleport_allowed_marker->setWorldPosition(pos2);
						teleport_allowed_marker->setEnabled(1);

						teleport_ray->setMaterial(teleport_allowed_mat.get(), "*");

						teleport_forbidden_marker->setEnabled(0);

					// teleport!
					if (button_pressed == 1)
					{
						player->setWorldPosition(intersection->getPoint() + vr_player_main->getHeadOffset());
						//teleport_allowed_marker->setEnabled(0);
						//teleport_forbidden_marker->setEnabled(0);
						teleport_delay = false;
					}

					break;
				}

			if (i == hitObj->getNumSurfaces())
			{
				teleport_allowed_marker->setEnabled(0);

				pos2 = intersection->getPoint() + Vec3(0, 0, 0.1f);
				teleport_forbidden_marker->setWorldPosition(pos2);
				teleport_forbidden_marker->setEnabled(1);

				teleport_ray->setMaterial(teleport_forbidden_mat.get(), "*");
			}
		} else
		{
			teleport_allowed_marker->setEnabled(0);
			teleport_forbidden_marker->setEnabled(0);
			teleport_ray->setMaterial(teleport_forbidden_mat.get(), "*");
		}

	// show ray
		teleport_ray->clearVertex();
		teleport_ray->clearIndices();

		int num = 30; // num of quads
		float inum = 1.0f / num;

		Vec3 last_p = pos1;
		for (int i = 1; i <= num; i++)
		{
			Vec3 p = getHermiteSpline(pos1, pos1, pos2, pos2 + Vec3(0, 0, -3), inum * i);
			addLineSegment(teleport_ray, vec3(last_p), vec3(p), 0.025f);
			last_p = p;
		}

		teleport_ray->updateBounds();
		teleport_ray->updateTangents();
		teleport_ray->flushVertex();
		teleport_ray->flushIndices();

		teleport_ray->setEnabled(1);
}

void UltraleapHandControllerVarjo::menu_update()
{
	if (!controller_valid[0])
		return;

	UltraleapHand* left_hand = ul_device->getLeftHand();
	UltraleapHand* right_hand = ul_device->getRightHand();

	bool button_down = false;

	WorldBoundSphere bs = WorldBoundSphere(left_hand->getArm()->getCenter(), button_hand_radius);
	bool gesture_activate = bs.inside(right_hand->getFingerIndex()->getBoneDistal()->getJointEndPosition());
	if (gesture_activate && controller_valid[1])
	{
		menu_delay_current += Game::getIFps();

		if (menu_delay_current > 0)
		{
			Visualizer::renderSolidSphere(button_hand_radius, translate(left_hand->getArm()->getCenter()), lerp(vec4(0, 1, 0, 0), vec4(0, 1, 0, 1), menu_delay_current / menu_delay_max));
			Visualizer::renderSolidSphere(0.005, translate(right_hand->getFingerIndex()->getBoneDistal()->getJointEndPosition()), vec4_blue);

		}
		if (menu_delay_current > menu_delay_max)
		{
			button_down = true;
			menu_delay_current = -menu_delay_max;
		}
	}
	else
	{
		menu_delay_current = 0.0f;
	}

	if (button_down)
	{
		object_gui->setEnabled(!object_gui->isEnabled());

		if (!object_gui->isEnabled())
		{
			int pressed = 0;
			for (int i = 0; i < CONTROLLER_COUNT; i++)
				pressed |= teleport_button_pressed[i];


			teleport_ray->setEnabled(pressed > 0);
		}
	}
}

void UltraleapHandControllerVarjo::gui_update(UltraleapHand * hand)
{
	int hit_gui = 0;

	const float distance_to_check = 5.0f;

	// update visuals (we are using teleport ray)
	UltraleapBone * distal_index_bone = hand->getFingerIndex()->getBoneDistal();
	Vec3 p0 = distal_index_bone->getJointBeginPosition();
	Vec3 p1 = p0 + Vec3(distal_index_bone->getDirection() * distance_to_check * distal_index_bone->getLength());
	Vec3 p1_end = p1;
	ObjectPtr hitObj = World::getIntersection(p0, p1, 1, intersection);
	if (hitObj)
	{
		ObjectGuiPtr gui = checked_ptr_cast<ObjectGui>(hitObj);
		if (gui)
		{

			p1_end = intersection->getPoint();
			Visualizer::renderSolidSphere(0.005, translate(p1_end), vec4_green);
			if (length(p1_end - p0) < distal_index_bone->getLength())
			{
				Visualizer::renderCircle(0.0175, setTo(p1_end, p1_end + Vec3(intersection->getNormal()), vec3_up), vec4_red);
				Visualizer::renderCircle(0.0075, setTo(p1_end, p1_end + Vec3(intersection->getNormal()), vec3_up), vec4_red);
				Visualizer::renderCircle(0.0125, setTo(p1_end, p1_end + Vec3(intersection->getNormal()), vec3_up), vec4_red);
			}
		
			gui->setMouse(p0, p1, length(p1_end - p0) < distal_index_bone->getLength(), 0);
		}
	}


}

void UltraleapHandControllerVarjo::close_helper()
{
	helper->setEnabled(false);
	helper.deleteLater();
	helper = ObjectGuiPtr();
}

vec3 getUP(const Vec3& start, const Vec3& end, const vec3& right)
{
	vec3 dir = vec3(end - start);
	return cross(right, dir);
}

void MeshSkinnedHandMapper::update(Unigine::Plugins::UltraleapHand* left, Unigine::Plugins::UltraleapHand* right)
{
	if (!skinned)
		return;

	auto process_hand = [this](Unigine::Plugins::UltraleapHand* u_hand, hand* c_hand)
	{
		int axis = c_hand->invert_direction.get() ? AXIS_NY : AXIS_Y;
		int invert_dir = c_hand->invert_direction.get() ? -1 : 1;

		auto right_dir = cross(u_hand->getDirection(), -u_hand->getPalmNormal());

		auto arm = u_hand->getArm();
		auto elbow = arm->getPositionElbow();
		auto wrist = arm->getPositionWrist();
		auto hand = u_hand->getPalmPosition();

		Mat4 T;
		vec3 up = vec3_up;
		Vec3 pos;


		if (c_hand->elbow > 0)
		{
			pos = lerp(elbow, wrist, -0.5);
			up = cross(right_dir, arm->getDirection()) * invert_dir;
			T = setTo(pos, wrist, up, axis);
			skinned->setBoneWorldTransformWithChildren(c_hand->elbow, T);
		}

		if (c_hand->arm > 0)
		{
			pos = lerp(elbow, wrist, 0.8);
			T = setTo(pos, wrist, up, axis);
			skinned->setBoneWorldTransformWithChildren(c_hand->arm, T);
		}

		if (c_hand->wrist > 0)
		{
			T = setTo(wrist, u_hand->getPalmPosition(), c_hand->invert_direction.get() ? u_hand->getPalmNormal() : -u_hand->getPalmNormal(), axis);
			skinned->setBoneWorldTransformWithChildren(c_hand->wrist, T);
		}
		auto process_finger = [&](Unigine::Plugins::UltraleapFinger * u_f, finger * c_f, vec3 right_dir)
		{

			auto process_bone = [&](int bone, Unigine::Plugins::UltraleapBone::TYPE bone_type)
			{
				Unigine::Plugins::UltraleapBone *u_b = get_bone_by_id(u_f, int(bone_type));
				
				if (bone > 0)
				{
					vec3 up = getUP(u_b->getJointBeginPosition(), u_b->getJointEndPosition(), right_dir) * invert_dir;
					Mat4 tt = setTo(u_b->getJointBeginPosition(), u_b->getJointEndPosition(), up, axis);
					skinned->setBoneWorldTransformWithChildren(bone, tt);
				}
			};

			process_bone(c_f->base, Unigine::Plugins::UltraleapBone::TYPE_METACARPAL);
			process_bone(c_f->first, Unigine::Plugins::UltraleapBone::TYPE_PROXIMAL);
			process_bone(c_f->middle, Unigine::Plugins::UltraleapBone::TYPE_INTERMEDIATE);
			process_bone(c_f->last, Unigine::Plugins::UltraleapBone::TYPE_DISTAL);
		};

		process_finger(u_hand->getFingerThumb(), &(c_hand->thumb.get()), c_hand->invert_direction.get() ? u_hand->getPalmNormal() : -u_hand->getPalmNormal());
		process_finger(u_hand->getFingerIndex(), &(c_hand->index.get()), right_dir);
		process_finger(u_hand->getFingerMiddle(), &(c_hand->middle.get()), right_dir);
		process_finger(u_hand->getFingerRing(), &(c_hand->ring.get()), right_dir);
		process_finger(u_hand->getFingerPinky(), &(c_hand->pinky.get()), right_dir);
	};

	process_hand(left, &(left_hand.get()));
	process_hand(right, &(right_hand.get()));
}
