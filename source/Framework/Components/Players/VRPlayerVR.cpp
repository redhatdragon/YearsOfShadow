#include "VRPlayerVR.h"
#include <UnigineRender.h>
#include <UnigineConfig.h>
#include <UnigineSounds.h>
#include "../../Utils.h"
#include "../Objects/ObjMovable.h"

using namespace Unigine;
using namespace Math;

void VRPlayerVR::setRayIntersectionMask(int mask)
{
	ray_intersection_mask = mask;
}

void VRPlayerVR::setTeleportationMask(int mask)
{
	teleportation_mask = mask;
}

PlayerPtr VRPlayerVR::getPlayer()
{
	return player;
}

int VRPlayerVR::getNumHands()
{
	return CONTROLLER_COUNT;
}

NodePtr VRPlayerVR::getHandNode(int num)
{
	return controller[num];
}

int VRPlayerVR::getHandDegreesOfFreedom(int num)
{
	return 6; // x, y, z + pitch, yaw, roll
}

int VRPlayerVR::getHandState(int num)
{
	return hand_state[num];
}

const NodePtr &VRPlayerVR::getGrabNode(int num) const
{
	return grab_node[num];
}

const Vector<VRInteractable*> &VRPlayerVR::getGrabComponents(int num) const
{
	return grab_components[num];
}

vec3 VRPlayerVR::getHandLinearVelocity(int num)
{
	return linearRegression(hand_linear_velocity[num]);
}

vec3 VRPlayerVR::getHandAngularVelocity(int num)
{
	return linearRegression(hand_angular_velocity[num]);
}

int VRPlayerVR::isControllerValid(int controller_num)
{
	return controller_valid[controller_num];
}

int VRPlayerVR::getControllerButtonDown(int controller_num, int button)
{
	return getControllerButton(controller_num, button) && !buttons_prev_state[controller_num][button];
}

int VRPlayerVR::getControllerButtonUp(int controller_num, int button)
{
	return !getControllerButton(controller_num, button) && buttons_prev_state[controller_num][button];
}

bool VRPlayerVR::isEyetrackingValid() const
{
	return eyetracking_valid;
}

Unigine::Math::Vec3 VRPlayerVR::getFocusWorldPosition() const
{
	return focus_position;
}

void VRPlayerVR::update_button_states()
{
	for (int k = 0; k < CONTROLLER_COUNT; k++)
		for (int i = 0; i < BUTTON::COUNT; i++)
			buttons_prev_state[k][i] = getControllerButton(k, i);
}

void VRPlayerVR::init_player()
{
	player = checked_ptr_cast<PlayerDummy>(node);
	Game::setPlayer(player);

	head = NodeDummy::create();

	xpad360 = ControlsGamepad::create(0);

	triggers.init(obstacles, triggers_scale);

	Sound::setHRTF(1);
}

void VRPlayerVR::load_player_height(float default_value)
{
	if (Config::isExist("player_height"))
		player_height = Config::getFloat("player_height");
	else
		player_height = default_value;

	player_height = clamp(player_height, -5.0f, 5.0f);
}

void VRPlayerVR::save_player_height()
{
	Config::setFloat("player_height", player_height);
	Config::save();
}

void VRPlayerVR::put_head_to_position(const mat4 &hmd_transform, const Vec3 &pos)
{
	Mat4 player_transform = player->getWorldTransform();
	Mat4 hmd_transform_world = player_transform * Mat4(hmd_transform);
	Vec3 head_offset =
		player_transform.getTranslate() - hmd_transform_world.getTranslate();
	head_offset.z = player_height;

	player->setWorldPosition(pos + head_offset);
}

void VRPlayerVR::land_player_to(const mat4 &hmd_transform, const Vec3 &position, const vec3 &direction)
{
	// rotate player
	vec3 dir = direction;
	dir.z = 0;
	dir = normalize(dir);
	quat rot = conjugate(quat(lookAt(Vec3(0, 0, 0), Vec3(dir), vec3(0, 0, 1))));
	vec3 angles = decomposeRotationYXZ(mat3(hmd_transform));
	player->setRotation(rot * quat(0.0f, -angles.y, 0.0f));

	// move player
	Mat4 player_transform = player->getWorldTransform();
	Mat4 hmd_transform_world = player_transform * Mat4(hmd_transform);
	Vec3 head_offset = Vec3(0, 0, 0);
	head_offset = player_transform.getTranslate() - hmd_transform_world.getTranslate();
	head_offset.z = player_height;

	// ground player
	Vec3 pos1 = position;
	Vec3 pos2 = position + Vec3(0, 0, -1) * player->getZFar();
	ObjectPtr hitObj = World::getIntersection(pos1, pos2, teleportation_mask, intersection);
	if (hitObj)
		player->setWorldPosition(intersection->getPoint() + head_offset);
}

void VRPlayerVR::move_update(const Mat4 &world_head_transform)
{
	float ifps = Game::getIFps() / Game::getScale();

	xpad360->updateEvents();
	if (xpad360->isAvailable())
	{
		// moving
		float lx = xpad360->getLeftX();
		float ly = xpad360->getLeftY();
		if (Math::abs(lx) < dead_zone) lx = 0;
		if (Math::abs(ly) < dead_zone) ly = 0;

		if (lx != 0 || ly != 0)
		{
			if (!is_moving)
			{
				head_dir = world_head_transform.getRotate();
				is_moving = true;
			}
			vec3 move_dir = head_dir * vec3(lx, 0, -ly);
			move_dir.z = 0;
			player->setWorldPosition(player->getWorldPosition() + Vec3(move_dir) * ifps);
		}
		else
			is_moving = false;


		// rotation
		float rx = xpad360->getRightX();
		if (Math::abs(rx) > dead_zone)
		{
			discrete_timer -= Game::getIFps() / Game::getScale();
			if (discrete_timer < 0)
			{
				/*
				// slow motion on small values and fast motion on high values
				rx = UNIGINE_PI * Math::sign(rx) * (Math::sin(rx * UNIGINE_PI - UNIGINE_PI * 0.5f) + 1.0f) * 0.5f;
				quat rot = quat(0, 0, -rx * 45.0f * step);
				*/

				quat rot = quat(0, 0, -Math::sign(rx) * 75.0f * step);
				Vec3 offset0 = world_head_transform.getTranslate() - player->getWorldPosition();
				Vec3 offset1 = rot * offset0;
				player->setWorldPosition(player->getWorldPosition() + offset0 - offset1);
				player->setWorldRotation(rot * player->getWorldRotation());

				// apply rotation to moving too
				head_dir = rot * head_dir;

				discrete_timer = step;
			}
		}
		else
			discrete_timer = 0;
	}
}

void VRPlayerVR::collisions_update(const Mat4 &head_transform, const Vec3 &offset)
{
	// triggers
	if (triggers.isInside(head_transform.getTranslate()))
	{
		float depth = triggers.getDepth();
		depth = clamp(depth / triggers_scale, 0.0f, 1.0f);
		fade_alpha = depth;
		vibrateController(0, depth);
		vibrateController(1, depth);

		if (trigger_timer <= 0)
		{
			before_collision_dir = player->getPosition() - offset - before_collision_point;
			before_collision_dir.z = 0;
			before_collision_dir = normalize(before_collision_dir);
		}

		trigger_timer += Game::getIFps() / Game::getScale();
		if (trigger_timer > black_screen_max_sec)
		{
			// teleport to last point, where was no collisions
			player->setWorldPosition(before_collision_point - before_collision_dir + offset);
		}
	}
	else
	{
		fade_alpha = moveTowards(fade_alpha, 0, Game::getIFps() * 2.0f / Game::getScale());
		vibrateController(0, 0);
		vibrateController(1, 0);
		trigger_timer = 0;
		before_collision_point = player->getPosition() - offset;
	}

	Render::setFadeColor(vec4(0, 0, 0, fade_alpha));
}

void VRPlayerVR::controllers_init()
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

	// find gui on left controller
	int gui_index = controller[0]->findChild("vr_gui");
	if (gui_index == -1)
		Log::error("VRPlayerVR::controllers_init(): Node \"vr_gui\" does not exist in the left controller!\n");

	object_gui = checked_ptr_cast<ObjectGui>(controller[0]->getChild(gui_index));
	object_gui->setBackground(0);
	object_gui->setMouseMode(ObjectGui::MOUSE_VIRTUAL);
	object_gui->setEnabled(0);
}

void VRPlayerVR::find_obj_in_children(const NodePtr &node, Vector<ObjectPtr> *obj)
{
	if (!node) return;

	ObjectPtr o = checked_ptr_cast<Object>(node);
	if (o) obj->append(o);

	for (int i = 0; i < node->getNumChildren(); i++)
	{
		NodeReferencePtr nodeRef = checked_ptr_cast<NodeReference>(node->getChild(i));
		if (nodeRef)
			find_obj_in_children(nodeRef->getReference(), obj);
		else
			find_obj_in_children(node->getChild(i), obj);
	}
}

void VRPlayerVR::controller_update(
	int num,
	const Mat4 &player_transform,
	int is_device_connected,
	const mat4 &device_pose)
{
	if (is_device_connected)
	{
		controller[num]->setWorldTransform(player_transform * Mat4(device_pose) * Mat4(rotateX(-90.0f)));

		if (controller[num]->isEnabled())
			controller_ref[num]->setEnabled(1);
	}
	else if (controller[num]->isEnabled())
		controller_ref[num]->setEnabled(0);
}

void VRPlayerVR::teleport_init()
{
	// marker init
	teleport_allowed_marker->setEnabled(0);
	teleport_forbidden_marker->setEnabled(0);

	// ray init
	teleport_ray = ObjectMeshDynamic::create();
	for (int i = 0; i < teleport_ray->getNumSurfaces(); i++)
	{
		teleport_ray->setMaterial(teleport_allowed_mat.get(), i);
		teleport_ray->setSurfaceProperty("surface_base", i);
		teleport_ray->setCastShadow(0, i);
		teleport_ray->setCastWorldShadow(0, i);
		teleport_ray->setIntersection(0, i);
		teleport_ray->setCollision(0, i);
	}
	teleport_ray->setPosition(Vec3_zero);
	teleport_ray->setRotation(quat_identity);

	// clear array
	for (int i = 0; i < CONTROLLER_COUNT; i++)
		teleport_button_pressed[i] = 0;
}

void VRPlayerVR::teleport_update(int num, int button_pressed, const Vec3 &offset)
{
	// check if nobody using our teleport
	if (button_pressed &&
		!teleport_button_pressed[num] &&
		teleport_button_pressed[1 - num]) // if controllers > 2 it doesn't work!
		return;

	if (!button_pressed && !teleport_button_pressed[num])
		return;

	int last_button_state = teleport_button_pressed[num];
	teleport_button_pressed[num] = button_pressed;

	Vec3 pos1 = controller[num]->getWorldPosition();
	Vec3 pos2 = controller[num]->getWorldPosition() + Vec3(controller[num]->getWorldDirection(Math::AXIS_Y)) * player->getZFar();

	if (button_pressed == 1 || (button_pressed == 0 && last_button_state == 1))
	{
		ObjectPtr hitObj = World::getIntersection(pos1, pos2, ray_intersection_mask, intersection);
		if (hitObj)
		{
			int i = 0;
			for (; i < hitObj->getNumSurfaces(); i++)
				if (hitObj->getIntersectionMask(i) & teleportation_mask)
				{
					// show marker
					if (button_pressed == 1)
					{
						pos2 = intersection->getPoint() + Vec3(0, 0, 0.1f);
						teleport_allowed_marker->setWorldPosition(pos2);
						teleport_allowed_marker->setEnabled(1);

						teleport_ray->setMaterial(teleport_allowed_mat.get(), "*");

						teleport_forbidden_marker->setEnabled(0);
					}

					// teleport!
					if (button_pressed == 0 && last_button_state == 1)
					{
						player->setWorldPosition(intersection->getPoint() + offset);
						teleport_allowed_marker->setEnabled(0);
						teleport_forbidden_marker->setEnabled(0);
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
		}
		else
		{
			teleport_allowed_marker->setEnabled(0);
			teleport_forbidden_marker->setEnabled(0);
			teleport_ray->setMaterial(teleport_forbidden_mat.get(), "*");
		}
	}

	// show ray
	if (button_pressed)
	{
		teleport_ray->clearVertex();
		teleport_ray->clearIndices();

		int num = 30;	// num of quads
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
	else
	{
		teleport_ray->setEnabled(0);
		teleport_forbidden_marker->setEnabled(0);
		teleport_allowed_marker->setEnabled(0);
	}
}

void VRPlayerVR::grab_init()
{
	hand_state.clear();
	node_grabbed.clear();
	node_selected.clear();
	last_selected.clear();
	last_selected_timer.clear();
	grab_node.clear();
	grab_components.clear();
	throw_trigger_value.clear();
	handy_pos_state.clear();

	NodePtr node_null;
	ObjectPtr obj_null;
	for (int i = 0; i < CONTROLLER_COUNT; i++)
	{
		grab_node.append(node_null);
		grab_components.append();
		hand_state.append(0);
		node_grabbed.append(0);
		last_selected.append(obj_null);
		last_selected_timer.append(0);
		node_selected.append(0);
		throw_trigger_value.append(0);
		handy_pos_state.append(0);
	}

	//pointer_init();
}

void VRPlayerVR::grab_update(int num, int pose_valid, float trigger_value, int button_touch, float pressed_pos, float release_pos)
{
	// calculate button_pressed (user experience improvement)
	int button_pressed = 0;
	if (hand_state[num] == HAND_FREE)
	{
		throw_trigger_value[num] = min(throw_trigger_value[num], trigger_value);
		button_pressed = trigger_value > pressed_pos + throw_trigger_value[num]; // around +30% from minimum pressure
	}
	else
	{
		throw_trigger_value[num] = max(throw_trigger_value[num], trigger_value);
		button_pressed = trigger_value > release_pos * throw_trigger_value[num]; // around 80% from maximum pressure
	}

	// you can't grab anything if controller doesn't have valid pose
	if (!pose_valid)
	{
		set_outline(num, 0);
		return;
	}

	// hide outline
	if (last_selected_timer[num] >= 0)
	{
		last_selected_timer[num] -= Game::getIFps() / Game::getScale();
		if (last_selected_timer[num] < 0)
			set_outline(num, 0);
	}

	//pointer_update(num, node_grabbed[num] == 0);

	if (node_grabbed[num] == 0)
	{
		hand_state[num] = HAND_FREE;

		ObjectPtr hitObj = last_selected[num];

		// get intersections, find new hitObj
		if (last_selected_timer[num] < 0)
		{
			if (grab_mode == GRAB_MODE::BOUNDBOX)
			{
				hitObj = ObjectPtr(); // null pointer

				intersections.clear();
				// use getReference because NodeReference has no bounds
				if (World::getIntersection(controller[num]->getReference()->getWorldBoundBox(), intersections))
				{
					// find nearest interactive object (can i grab it?)
					int nearest = -1;
					Scalar min_dist = Consts::INF;
					for (int k = 0; k < intersections.size(); k++)
						if (can_i_grab_it(intersections[k]))
						{
							Scalar dist = length2(intersections[k]->getWorldPosition() - controller[num]->getWorldPosition());
							if (min_dist > dist)
							{
								min_dist = dist;
								nearest = k;
							}
						}

					// select nearest object
					if (nearest >= 0)
						hitObj = intersections[nearest];
				}
			}
			else if (grab_mode == GRAB_MODE::INTERSECTIONS)
			{
				Vec3 pos1 = controller[num]->getWorldPosition() - Vec3(controller[num]->getWorldDirection(Math::AXIS_NZ)) * ray_back_length;
				Vec3 pos2 = controller[num]->getWorldPosition() + Vec3(controller[num]->getWorldDirection(Math::AXIS_NZ)) * ray_forward_length;

				hitObj = World::getIntersection(pos1, pos2, 1, intersection);
				if (!hitObj) // second intersection (for best player's experience)
					hitObj = World::getIntersection(pos1, pos2 + Vec3(controller[num]->getWorldDirection(Math::AXIS_X)) * ptr_width * 0.5f, 1, intersection);
				if (!hitObj) // third intersection (for best player's experience)
					hitObj = World::getIntersection(pos1, pos2 + Vec3(controller[num]->getWorldDirection(Math::AXIS_NX)) * ptr_width * 0.5f, 1, intersection);
			}
		}

		if (hitObj)
		{
			// check if this object grabbed by another controller
			for (int z = 0; z < grab_node.size(); z++)
				if (z != num && node_grabbed[z] && hitObj == grab_node[z])
					return;

			// all right. It's movable or switcher?
			if (can_i_grab_it(hitObj))
			{
				vibrateController(num, 0.5f);

				// show controller's outline
				last_selected[num] = hitObj;
				if (last_selected_timer[num] < 0)
					last_selected_timer[num] = 0.1f;
				set_outline(num, 1);

				// ... and grab it (or touch)
				if (button_pressed || (button_touch && is_it_switcher(hitObj)))
				{
					last_selected_timer[num] = 0;
					grab_node[num] = hitObj;
					getComponents<VRInteractable>(grab_node[num], grab_components[num]);
					node_grabbed[num] = 1;
					hand_state[num] = HAND_GRAB;

					// don't need to holding the objects if they are change our controller
					ObjMovable* obj_movable = getComponent<ObjMovable>(grab_node[num]);
					if (obj_movable && obj_movable->use_handy_pos)
						handy_pos_state[num] = GRAB_HANDY::GRAB;
					else
						handy_pos_state[num] = GRAB_HANDY::NON_HANDY;
				}
			}
		}
	}
	else
	{
		if (button_pressed)
		{
			// hold
			hand_state[num] = HAND_HOLD;

			if (handy_pos_state[num] == GRAB_HANDY::GRAB_RELEASED)
				handy_pos_state[num] = GRAB_HANDY::GRAB_AGAIN;
		}
		else
		{
			if (handy_pos_state[num] == GRAB_HANDY::NON_HANDY || handy_pos_state[num] == GRAB_HANDY::GRAB_AGAIN)
			{
				// throw!
				node_grabbed[num] = 0;
				hand_state[num] = HAND_THROW;
				handy_pos_state[num] = GRAB_HANDY::NON_HANDY;
			}
			else
				handy_pos_state[num] = GRAB_HANDY::GRAB_RELEASED;
		}
	}
}

void VRPlayerVR::set_outline(int num, int enable)
{
	if (node_selected[num] == enable)
		return;

	node_selected[num] = enable;

	// set material state (to controller)
	for (int i = 0; i < controller_obj[num].size(); i++)
		for (int k = 0; k < controller_obj[num][i]->getNumSurfaces(); k++)
			controller_obj[num][i]->setMaterialState("auxiliary", enable, k);

	// set material state (to object)
	if (last_selected[num])
		for (int i = 0; i < last_selected[num]->getNumSurfaces(); i++)
			last_selected[num]->setMaterialState("auxiliary", enable, i);

	// set post materials
	if (enable)
	{
		auto post_mat = Materials::findManualMaterial("vr_post_filter_selection");
		if (post_mat)
		{
			if (player->findScriptableMaterial(post_mat) == -1)
				player->addScriptableMaterial(post_mat);
		} else
		{
			Log::warning("VRPlayerVR::set_outline(): can not find manual material \"vr_post_filter_selection\"\n");
		}
	}
	else
	{
		// if other hand use post materials, then return
		for (int i = 0; i < node_selected.size(); i++)
			if (node_selected[i] == 1)
				return;

		player->clearScriptableMaterials();
	}
}

void VRPlayerVR::pointer_init()
{
	ptr_mesh.clear();
	ptr_anim.clear();
	for (int i = 0; i < CONTROLLER_COUNT; i++)
	{
		ObjectMeshDynamicPtr m = ObjectMeshDynamic::create();
		for (int k = 0; k < m->getNumSurfaces(); k++)
		{
			m->setMaterial(Materials::findManualMaterial("Unigine::mesh_base"), k);
			m->setSurfaceProperty("surface_base", k);
			m->setCastShadow(0, k);
			m->setCastWorldShadow(0, k);
			m->setIntersection(0, k);
			m->setCollision(0, k);
		}
		m->setPosition(Vec3_zero);
		m->setRotation(quat_identity);

		m->clearVertex();
		m->clearIndices();
		m->addTriangleQuads(1);
		m->addVertex(vec3_zero);	m->addTexCoord(vec4(0, 0, 0, 0));
		m->addVertex(vec3_zero);	m->addTexCoord(vec4(1, 0, 0, 0));
		m->addVertex(vec3_zero);	m->addTexCoord(vec4(1, 1, 0, 0));
		m->addVertex(vec3_zero);	m->addTexCoord(vec4(0, 1, 0, 0));
		m->updateBounds();
		m->updateTangents();
		m->flushVertex();
		m->flushIndices();

		ptr_mesh.append(m);
		ptr_anim.append(0);
	}
}

void VRPlayerVR::pointer_update(int num, int find_mode)
{
	// find objects around hand
	float n = -1;
	if (find_mode)
	{
		Math::WorldBoundSphere s;
		Vector<ObjectPtr> objs;

		s.set(controller[num]->getWorldPosition(), ray_forward_length);
		if (World::getIntersection(s, objs))
			for (int i = 0; i < objs.size(); i++)
				if (can_i_grab_it(objs[i]))
				{
					n = 1;
					break;
				}
	}

	// animation
	ptr_anim[num] = saturate(ptr_anim[num] + n * 7.5f * Game::getIFps() / Game::getScale());

	// update mesh
	vec3 view_dir = head->getWorldDirection();
	vec3 pb = vec3(controller[num]->getWorldPosition());
	vec3 pe = pb + controller[num]->getWorldDirection(Math::AXIS_NZ) * ray_forward_length * ptr_anim[num];
	vec3 dir = normalize(pe - pb);
	vec3 right = cross(view_dir, dir);

	vec3 p0 = pb - right * ptr_width * 0.5f;	// 0, 0
	vec3 p1 = pb + right * ptr_width * 0.5f;	// 1, 0
	vec3 p2 = pe + right * ptr_width * 0.5f;	// 1, 1
	vec3 p3 = pe - right * ptr_width * 0.5f;	// 0, 1

	ptr_mesh[num]->setVertex(0, p0);
	ptr_mesh[num]->setVertex(1, p1);
	ptr_mesh[num]->setVertex(2, p2);
	ptr_mesh[num]->setVertex(3, p3);

	ptr_mesh[num]->updateBounds();
	//ptr_mesh[num]->updateTangents();
	ptr_mesh[num]->flushVertex();
	//ptr_mesh[num]->flushIndices();
}

void VRPlayerVR::push_hand_linear_velocity(int num, const vec3 &velocity)
{
	Vector<vec3> *buffer = &hand_linear_velocity[num];

	if (buffer->size() < controller_buffer_count)
		buffer->append(velocity);
	else
	{
		for (int i = 0; i < buffer->size() - 1; i++)
			buffer->get(i) = buffer->get(i + 1);

		buffer->get(buffer->size() - 1) = velocity;
	}
}

void VRPlayerVR::push_hand_angular_velocity(int num, const vec3 &velocity)
{
	Vector<vec3> *buffer = &hand_angular_velocity[num];

	if (buffer->size() < controller_buffer_count)
		buffer->append(velocity);
	else
	{
		for (int i = 0; i < buffer->size() - 1; i++)
			buffer->get(i) = buffer->get(i + 1);

		buffer->get(buffer->size() - 1) = velocity;
	}
}

void VRPlayerVR::update_gui()
{
	if (!controller_valid[0] || !controller_valid[1])
		return;
	if (getControllerButtonDown(0, BUTTON::MENU) || getControllerButtonDown(1, BUTTON::MENU))
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

	if (!object_gui->isEnabled())
		return;

	int hit_gui = 0;

	// update visuals (we are using teleport ray)
	Vec3 p0 = controller[1]->getWorldPosition();
	mat4 m = mat4(controller[1]->getWorldTransform());
	Vec3 p1 = p0 + Vec3(normalize(getDirectionNZ(m) + getDirectionY(m))) * 10.0f;
	Vec3 p1_end = p1;
	ObjectPtr hitObj = World::getIntersection(p0, p1, 1, intersection);
	if (hitObj)
	{
		p1_end = intersection->getPoint();
		if (hitObj->getID() == object_gui->getID())
			hit_gui = 1;
	}

	teleport_ray->clearVertex();
	teleport_ray->clearIndices();
	vec3 from = vec3(p0);
	vec3 to = vec3(p1_end);
	vec3 from_right = cross(normalize(to - from), vec3(normalize(head->getWorldPosition() - p0)));
	vec3 to_right = cross(normalize(to - from), vec3(normalize(head->getWorldPosition() - p1_end)));
	addLineSegment(teleport_ray, from, to, from_right, to_right, 0.01f);
	teleport_ray->updateBounds();
	teleport_ray->updateTangents();
	teleport_ray->flushVertex();
	teleport_ray->flushIndices();
	teleport_ray->setEnabled(1);

	// update gui
	object_gui->setMouse(p0, p1, hit_gui * getControllerButton(1, BUTTON::TRIGGER), 0);
}

void VRPlayerVR::update_eyetracking(Unigine::Math::Vec3 focus_world_position, bool valid)
{
	focus_position = focus_world_position;
	eyetracking_valid = valid;
}
