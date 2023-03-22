#include "VRPlayerOculus.h"
#include <UnigineVisualizer.h>
#include <UnigineGame.h>
#include <UnigineRender.h>
#include "../../Utils.h"

REGISTER_COMPONENT(VRPlayerOculus);

using namespace Unigine;
using namespace Math;

void VRPlayerOculus::init()
{
	VRPlayerVR::init();
	oculus.init();
	init_player();
	load_player_height(player_height);
	landPlayerTo(player->getWorldPosition(), player->getWorldDirection());
	controllers_init();
	buttons_init();
	teleport_init();
	grab_init();
	height_calibration_init();
	gamepad_init();
}

void VRPlayerOculus::update()
{
	// get world position of the head
	Mat4 player_transform = player->getWorldTransform();
	Mat4 hmd_transform = Mat4(oculus.getDevicePose(OCULUS_DEVICE_HMD));
	Mat4 hmd_transform_world = player_transform * hmd_transform;

	// move and rotate head
	move_update_touch(oculus.getControllerAxis(OCULUS_DEVICE_CONTROLLER_LEFT, OCULUS_AXIS_THUMBSTICK), player_transform * Mat4(oculus.getDevicePose(OCULUS_DEVICE_CONTROLLER_LEFT)));
	turn_update_touch(oculus.getControllerAxis(OCULUS_DEVICE_CONTROLLER_RIGHT, OCULUS_AXIS_THUMBSTICK), hmd_transform_world);

	// refresh info about position of the head
	player_transform = player->getWorldTransform();
	hmd_transform_world = player_transform * hmd_transform;
	head_offset =
		oculus.isPoseValid(OCULUS_DEVICE_HMD) ?
		player_transform.getTranslate() - hmd_transform_world.getTranslate() :
		Vec3_zero;
	head_offset.z = player_height;

	head->setWorldTransform(hmd_transform_world);

	controller_valid[0] = oculus.isPoseValid(OCULUS_DEVICE_CONTROLLER_LEFT);
	controller_valid[1] = oculus.isPoseValid(OCULUS_DEVICE_CONTROLLER_RIGHT);

	controller_update(0, player_transform, controller_valid[0], oculus.getDevicePose(OCULUS_DEVICE_CONTROLLER_LEFT));
	controller_update(1, player_transform, controller_valid[1], oculus.getDevicePose(OCULUS_DEVICE_CONTROLLER_RIGHT));
	buttons_update();

	teleport_update(0, oculus.getControllerButtonPressed(OCULUS_BUTTON_X), head_offset);
	teleport_update(1, oculus.getControllerButtonPressed(OCULUS_BUTTON_A), head_offset);

	move_update(hmd_transform_world);
	collisions_update(hmd_transform_world, head_offset);

	grab_update(0, controller_valid[0], getControllerAxis(0, BUTTON::GRIP), getControllerButtonDown(0, BUTTON::TRIGGER), 0.5f, 0.9f);
	grab_update(1, controller_valid[1], getControllerAxis(1, BUTTON::GRIP), getControllerButtonDown(1, BUTTON::TRIGGER), 0.5f, 0.9f);

	height_calibration_update();

	gamepad_update(hmd_transform_world);

	quat rot = player->getRotation();
	vec3 vel0 = oculus.getDeviceVelocity(OCULUS_DEVICE_CONTROLLER_LEFT) * hand_force_multiplier;
	vec3 vel1 = oculus.getDeviceVelocity(OCULUS_DEVICE_CONTROLLER_RIGHT) * hand_force_multiplier;
	push_hand_linear_velocity(0, rot * vel0);
	push_hand_linear_velocity(1, rot * vel1);
	push_hand_angular_velocity(0, rot * oculus.getDeviceAngularVelocity(OCULUS_DEVICE_CONTROLLER_LEFT));
	push_hand_angular_velocity(1, rot * oculus.getDeviceAngularVelocity(OCULUS_DEVICE_CONTROLLER_RIGHT));

	update_gui();
}

void VRPlayerOculus::postUpdate()
{
	head->setWorldTransform(player->getWorldTransform() * Mat4(oculus.getDevicePose(OCULUS_DEVICE_HMD)));
	VRPlayer::postUpdate();
}

void VRPlayerOculus::setLock(int lock)
{
	oculus.setHeadPositionLock(lock);
	oculus.setHeadRotationLock(lock);
}

void VRPlayerOculus::setPlayerPosition(const Vec3 &pos)
{
	put_head_to_position(oculus.getDevicePose(OCULUS_DEVICE_HMD), pos);
}

void VRPlayerOculus::landPlayerTo(const Vec3 &position, const vec3 &direction)
{
	land_player_to(oculus.getDevicePose(OCULUS_DEVICE_HMD), position, direction);
}

void VRPlayerOculus::height_calibration_init()
{
	warning_message->setEnabled(0);
	success_message->setEnabled(0);
}

void VRPlayerOculus::show_object_in_front_hmd(const NodePtr &obj)
{
	obj->setEnabled(1);

	mat4 t = oculus.getDevicePose(OCULUS_DEVICE_HMD);
	obj->setWorldTransform(player->getWorldTransform() * Mat4(t));
	obj->setWorldPosition(obj->getWorldPosition() + Vec3(obj->getWorldDirection()) * 0.5f);
}

void VRPlayerOculus::height_calibration_update()
{
	if (show_message_timer > 0)
	{
		show_message_timer -= Game::getIFps() / Game::getScale();
		if (show_message_timer <= 0)
		{
			warning_message->setEnabled(0);
			success_message->setEnabled(0);
		}
	}

	if (oculus.getControllerButtonPressed(OCULUS_BUTTON_RTHUMB) &&
		oculus.getControllerAxis(OCULUS_DEVICE_CONTROLLER_RIGHT, OCULUS_AXIS_INDEX_TRIGGER).x > 0.9f &&
		oculus.getControllerAxis(OCULUS_DEVICE_CONTROLLER_RIGHT, OCULUS_AXIS_HAND_TRIGGER).x > 0.9f)
	{
		if (oculus.isPoseValid(OCULUS_DEVICE_CONTROLLER_RIGHT))
		{
			mat4 controller_transform = oculus.getDevicePose(OCULUS_DEVICE_CONTROLLER_RIGHT);
			player_height = -controller_transform.getTranslate().y + 0.035f;

			Vec3 pos = player->getPosition();
			pos.z = player_height;
			player->setPosition(pos);

			show_object_in_front_hmd(success_message);
			warning_message->setEnabled(0);

			oculus.setControllerVibration(OCULUS_DEVICE_CONTROLLER_RIGHT, 100, 0.3f);

			save_player_height();
		} else
		{
			show_object_in_front_hmd(warning_message);
			success_message->setEnabled(0);

			oculus.setControllerVibration(OCULUS_DEVICE_CONTROLLER_RIGHT, 100, 1.0f);
		}
		show_message_timer = show_message_duration;
	}
}

void VRPlayerOculus::buttons_init()
{
	buttons.clear();
	buttons.append(controller_ref[0]->getChild(controller_ref[0]->findChild("oculus_touch_left_thumbstick")));
	buttons.append(controller_ref[0]->getChild(controller_ref[0]->findChild("oculus_touch_left_button_x")));
	buttons.append(controller_ref[0]->getChild(controller_ref[0]->findChild("oculus_touch_left_button_y")));
	buttons.append(controller_ref[0]->getChild(controller_ref[0]->findChild("oculus_touch_left_trigger")));
	buttons.append(controller_ref[0]->getChild(controller_ref[0]->findChild("oculus_touch_left_grip")));
	buttons.append(controller_ref[1]->getChild(controller_ref[1]->findChild("oculus_touch_right_thumbstick")));
	buttons.append(controller_ref[1]->getChild(controller_ref[1]->findChild("oculus_touch_right_button_a")));
	buttons.append(controller_ref[1]->getChild(controller_ref[1]->findChild("oculus_touch_right_button_b")));
	buttons.append(controller_ref[1]->getChild(controller_ref[1]->findChild("oculus_touch_right_trigger")));
	buttons.append(controller_ref[1]->getChild(controller_ref[1]->findChild("oculus_touch_right_grip")));
}

void VRPlayerOculus::buttons_update()
{
	int button_a = oculus.getControllerButtonPressed(OCULUS_BUTTON_A);
	int button_b = oculus.getControllerButtonPressed(OCULUS_BUTTON_B);
	int button_x = oculus.getControllerButtonPressed(OCULUS_BUTTON_X);
	int button_y = oculus.getControllerButtonPressed(OCULUS_BUTTON_Y);
	vec2 lthumb = oculus.getControllerAxis(OCULUS_DEVICE_CONTROLLER_LEFT, OCULUS_AXIS_THUMBSTICK);
	vec2 rthumb = oculus.getControllerAxis(OCULUS_DEVICE_CONTROLLER_RIGHT, OCULUS_AXIS_THUMBSTICK);
	vec2 lindex = oculus.getControllerAxis(OCULUS_DEVICE_CONTROLLER_LEFT, OCULUS_AXIS_INDEX_TRIGGER);
	vec2 rindex = oculus.getControllerAxis(OCULUS_DEVICE_CONTROLLER_RIGHT, OCULUS_AXIS_INDEX_TRIGGER);
	vec2 lhand = oculus.getControllerAxis(OCULUS_DEVICE_CONTROLLER_LEFT, OCULUS_AXIS_HAND_TRIGGER);
	vec2 rhand = oculus.getControllerAxis(OCULUS_DEVICE_CONTROLLER_RIGHT, OCULUS_AXIS_HAND_TRIGGER);

	buttons[0]->setRotation(quat(-lthumb.y * 15.0f, lthumb.x * 15.0f, 0));												// thumbstick
	buttons[1]->setPosition(Vec3(buttons[1]->getPosition().x, buttons[1]->getPosition().y, button_x ? -0.001f : 0.0f));	// x
	buttons[2]->setPosition(Vec3(buttons[2]->getPosition().x, buttons[2]->getPosition().y, button_y ? -0.001f : 0.0f));	// y
	buttons[3]->setRotation(quat(-lindex.x * 20.0f, 0, 0));																// trigger
	buttons[4]->setRotation(quat(0, 0, -lhand.x * 6.0f));																// grip

	buttons[5]->setRotation(quat(-rthumb.y * 15.0f, rthumb.x * 15.0f, 0));												// thumbstick
	buttons[6]->setPosition(Vec3(buttons[6]->getPosition().x, buttons[6]->getPosition().y, button_a ? -0.001f : 0.0f));	// a
	buttons[7]->setPosition(Vec3(buttons[7]->getPosition().x, buttons[7]->getPosition().y, button_b ? -0.001f : 0.0f));	// b
	buttons[8]->setRotation(quat(-rindex.x * 20.0f, 0, 0));																// trigger
	buttons[9]->setRotation(quat(0, 0, rhand.x * 6.0f));																// grip
}

void VRPlayerOculus::vibrateController(int controller_num, float amplitude)
{
	if (xpad360->isAvailable())
	{
		if (controller_num == 0) xpad360->setVibration(amplitude * 0.2f, 0.0f);
		if (controller_num == 1) xpad360->setVibration(0.0f, amplitude * 0.2f);
	}

	if (amplitude > 0)
	{
		if (controller_num == 0 && oculus.isPoseValid(OCULUS_DEVICE_CONTROLLER_LEFT))
			oculus.setControllerVibration(OCULUS_DEVICE_CONTROLLER_LEFT, 100, amplitude * 0.25f);
		if (controller_num == 1 && oculus.isPoseValid(OCULUS_DEVICE_CONTROLLER_RIGHT))
			oculus.setControllerVibration(OCULUS_DEVICE_CONTROLLER_RIGHT, 100, amplitude * 0.25f);
	}
}

void VRPlayerOculus::move_update_touch(const vec2 &axis, const Mat4 &controller_transform)
{
	if (Math::abs(axis.x) > dead_zone || Math::abs(axis.y) > dead_zone)
	{
		Vec3 move_dir = controller_transform.getRotate() * Vec3(axis.x, 0, -axis.y);
		move_dir.z = 0;
		player->setPosition(player->getPosition() + move_dir * Game::getIFps() / Game::getScale());
	}
}

void VRPlayerOculus::turn_update_touch(const vec2 &axis, const Mat4 &head_transform)
{
	if (Math::abs(axis.x) > dead_zone)
	{
		odiscrete_timer -= Game::getIFps() / Game::getScale();
		if (odiscrete_timer < 0)
		{

			quat rot = quat(0, 0, -Math::sign(axis.x) * 75.0f * step);
			Vec3 offset0 = head_transform.getTranslate() - player->getWorldPosition();
			Vec3 offset1 = rot * offset0;
			player->setPosition(player->getPosition() + offset0 - offset1);
			player->setRotation(rot * player->getRotation());

			odiscrete_timer = step;
		}
	}
	else
		odiscrete_timer = 0;
}

void VRPlayerOculus::gamepad_init()
{
	xpad_head = NodeDummy::create();
	xpad_hand = NodeDummy::create();
	xpad_hand->setParent(xpad_head);
	xpad_hand->setRotation(quat(80, 0, 0));
}

void VRPlayerOculus::gamepad_update(const Mat4 &hmd_transform)
{
	if (!xpad360->isAvailable())
		return;

	// update head position
	xpad_head->setPosition(hmd_transform.getTranslate());
	vec3 dir = vec3(-hmd_transform.getColumn3(2)); // -Z
	xpad_head->setDirection(dir, vec3(0, 0, 1));

	// update hand position

	// update grabbing
	// grab_update(2, xpad360->getRightTrigger() > dead_zone);
}

Mat4 VRPlayerOculus::getControllerTransform(int controller_num)
{
	return Mat4(controller_num == 0 ? oculus.getDevicePose(OCULUS_DEVICE_CONTROLLER_LEFT) : oculus.getDevicePose(OCULUS_DEVICE_CONTROLLER_RIGHT));
}

int VRPlayerOculus::getControllerButton(int controller_num, int button)
{
	if (!controller_valid[controller_num])
		return 0;

	int device = controller_num > 0 ? OCULUS_DEVICE_CONTROLLER_RIGHT : OCULUS_DEVICE_CONTROLLER_LEFT;

	switch (button)
	{
		case STICK_X: return Math::abs(oculus.getControllerAxis(device, OCULUS_AXIS_THUMBSTICK).x) > 0.5f;
		case STICK_Y: return Math::abs(oculus.getControllerAxis(device, OCULUS_AXIS_THUMBSTICK).y) > 0.5f;
		case TRIGGER: return oculus.getControllerAxis(device, OCULUS_AXIS_INDEX_TRIGGER).x > 0.5f;
		case GRIP: return oculus.getControllerAxis(device, OCULUS_AXIS_HAND_TRIGGER).x > 0.5f;
		case XA: return device == OCULUS_DEVICE_CONTROLLER_LEFT ? oculus.getControllerButtonPressed(OCULUS_BUTTON_X) : oculus.getControllerButtonPressed(OCULUS_BUTTON_A);
		case YB: return device == OCULUS_DEVICE_CONTROLLER_LEFT ? oculus.getControllerButtonPressed(OCULUS_BUTTON_Y) : oculus.getControllerButtonPressed(OCULUS_BUTTON_B);
		case MENU: return oculus.getControllerButtonPressed(OCULUS_BUTTON_ENTER);
	}

	return 0;
}

float VRPlayerOculus::getControllerAxis(int controller_num, int button)
{
	if (!controller_valid[controller_num])
		return 0;

	int device = controller_num > 0 ? OCULUS_DEVICE_CONTROLLER_RIGHT : OCULUS_DEVICE_CONTROLLER_LEFT;

	switch (button)
	{
		case STICK_X: return oculus.getControllerAxis(device, OCULUS_AXIS_THUMBSTICK).x;
		case STICK_Y: return oculus.getControllerAxis(device, OCULUS_AXIS_THUMBSTICK).y;
		case TRIGGER: return oculus.getControllerAxis(device, OCULUS_AXIS_INDEX_TRIGGER).x;
		case GRIP: return oculus.getControllerAxis(device, OCULUS_AXIS_HAND_TRIGGER).x;
		case XA: return itof(device == OCULUS_DEVICE_CONTROLLER_LEFT ? oculus.getControllerButtonPressed(OCULUS_BUTTON_X) : oculus.getControllerButtonPressed(OCULUS_BUTTON_A));
		case YB: return itof(device == OCULUS_DEVICE_CONTROLLER_LEFT ? oculus.getControllerButtonPressed(OCULUS_BUTTON_Y) : oculus.getControllerButtonPressed(OCULUS_BUTTON_B));
		case MENU: return itof(oculus.getControllerButtonPressed(OCULUS_BUTTON_ENTER));
	}

	return 0;
}