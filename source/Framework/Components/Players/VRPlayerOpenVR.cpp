#include "VRPlayerOpenVR.h"
#include <UnigineVisualizer.h>
#include <UnigineGame.h>
#include "../../Utils.h"

#define HIDE_BASESTATIONS

REGISTER_COMPONENT(VRPlayerOpenVR);

using namespace Unigine;
using namespace Math;

void VRPlayerOpenVR::init()
{
	VRPlayer::init();
	openvr.init();
	find_devices();
	init_player();
	load_player_height(0);
	landPlayerTo(player->getWorldPosition(), player->getWorldDirection());
	controllers_init();
	basestation[0] = checked_ptr_cast<NodeReference>(basestation_0.get());
	basestation[1] = checked_ptr_cast<NodeReference>(basestation_1.get());
#ifdef HIDE_BASESTATIONS
	basestation[0]->setEnabled(0);
	basestation[1]->setEnabled(0);
#endif
	teleport_init();
	grab_init();
	eyetracking.init();
}

void VRPlayerOpenVR::update()
{
	find_devices();

	controller_valid[0] = CONTROLLER_DEVICE_0 != -1 && openvr.isDeviceConnected(CONTROLLER_DEVICE_0) && openvr.isPoseValid(CONTROLLER_DEVICE_0);
	controller_valid[1] = CONTROLLER_DEVICE_1 != -1 && openvr.isDeviceConnected(CONTROLLER_DEVICE_1) && openvr.isPoseValid(CONTROLLER_DEVICE_1);

	// if system menu is open
	if ((controller_valid[0] && openvr.getControllerButtonPressed(CONTROLLER_DEVICE_0, BUTTON_SYSTEM) < 0) ||
		(controller_valid[1] && openvr.getControllerButtonPressed(CONTROLLER_DEVICE_1, BUTTON_SYSTEM) < 0))
	{
		for (int i = 0; i < 2; i++)
		{
			controller_ref[i]->setEnabled(0);
			teleport_button_pressed[i] = 0;
		}
		teleport_allowed_marker->setEnabled(0);
		teleport_forbidden_marker->setEnabled(0);
		teleport_ray->setEnabled(0);

		return;
	}

	Mat4 player_transform = player->getWorldTransform();
	Mat4 hmd_transform = Mat4(openvr.getDevicePose(HMD_DEVICE_0));
	Mat4 hmd_transform_world = player_transform * hmd_transform;
	head_offset =
		(openvr.isDeviceConnected(HMD_DEVICE_0) && HMD_DEVICE_0 != -1) ?
		player_transform.getTranslate() - hmd_transform_world.getTranslate() :
		Vec3_zero;
	head_offset.z = 0;

	head->setWorldTransform(hmd_transform_world);

#ifndef HIDE_BASESTATIONS
	basestation_update(0, player_transform, BASESTATION_DEVICE_0);
	basestation_update(1, player_transform, BASESTATION_DEVICE_1);
#endif
	controller_update(0, player_transform, CONTROLLER_DEVICE_0);
	controller_update(1, player_transform, CONTROLLER_DEVICE_1);

	teleport_update(0, controller_valid[0] ? openvr.getControllerButtonPressed(CONTROLLER_DEVICE_0, BUTTON_STEAMVR_TOUCHPAD) : 0, head_offset);
	teleport_update(1, controller_valid[1] ? openvr.getControllerButtonPressed(CONTROLLER_DEVICE_1, BUTTON_STEAMVR_TOUCHPAD) : 0, head_offset);

	move_update(hmd_transform_world);
	collisions_update(hmd_transform_world, head_offset);

	grab_update(0, controller_valid[0], getControllerAxis(0, BUTTON::GRIP), getControllerButtonDown(0, BUTTON::TRIGGER));
	grab_update(1, controller_valid[1], getControllerAxis(1, BUTTON::GRIP), getControllerButtonDown(1, BUTTON::TRIGGER));

	quat rot = player->getRotation();
	if (CONTROLLER_DEVICE_0 != -1)
	{
		push_hand_linear_velocity(0, rot * openvr.getDeviceVelocity(CONTROLLER_DEVICE_0) * hand_force_multiplier);
		push_hand_angular_velocity(0, rot * openvr.getDeviceAngularVelocity(CONTROLLER_DEVICE_0));
	}
	if (CONTROLLER_DEVICE_1 != -1)
	{
		push_hand_linear_velocity(1, rot * openvr.getDeviceVelocity(CONTROLLER_DEVICE_1) * hand_force_multiplier);
		push_hand_angular_velocity(1, rot * openvr.getDeviceAngularVelocity(CONTROLLER_DEVICE_1));
	}

	update_gui();

	update_eyetracking(eyetracking.getFocusWorldPosition(), eyetracking.isEyetrackingAvailable() && eyetracking.isValid());
}

void VRPlayerOpenVR::setLock(int lock)
{
	openvr.setHeadPositionLock(lock);
	openvr.setHeadRotationLock(lock);
}

void VRPlayerOpenVR::setPlayerPosition(const Vec3 &pos)
{
	put_head_to_position(openvr.getDevicePose(HMD_DEVICE_0), pos);
}

void VRPlayerOpenVR::landPlayerTo(const Vec3 &position, const vec3 &direction)
{
	land_player_to(openvr.getDevicePose(HMD_DEVICE_0), position, direction);
}

void VRPlayerOpenVR::basestation_update(int num, const Mat4 &player_transform, int device_id)
{
	if (device_id != -1 && openvr.isDeviceConnected(device_id))
	{
		mat4 transform = openvr.getDevicePose(device_id);
		basestation[num]->setWorldTransform(player_transform * Mat4(transform));
		basestation[num]->setEnabled(1);
	}
	else
		basestation[num]->setEnabled(0);
}

void VRPlayerOpenVR::controller_update(int num, const Mat4 &player_transform, int device_id)
{
	if (device_id != -1 && openvr.isDeviceConnected(device_id))
	{
		mat4 transform = openvr.getDevicePose(device_id);
		controller[num]->setWorldTransform(player_transform * Mat4(transform) * Mat4(rotateX(-90.0f)));

		// update buttons
		float trigger_position = openvr.getControllerAxis(device_id, 1).x;
		set_controller_trigger_position(controller[num], trigger_position);

		if (controller[num]->isEnabled())
			controller_ref[num]->setEnabled(1);
	}
	else if (controller[num]->isEnabled())
		controller_ref[num]->setEnabled(0);
}

void VRPlayerOpenVR::set_controller_trigger_position(NodeReferencePtr &controller, float position)
{
	NodePtr root = controller->getReference();
	int pivot_id = root->findChild("vive_trigger");
	if (pivot_id != -1)
	{
		NodePtr pivot_node = root->getChild(pivot_id);
		pivot_node->setTransform(Mat4(translate(0.0f, -0.039f, -0.018f) * rotateX(-position * 20.0f)));
	}
}

void VRPlayerOpenVR::find_devices()
{
	int curTracking = 0;
	int curController = 0;
	for (int i = 0; i < MAX_TRACKED_DEVICE_COUNT; i++)
	{
		int deviceType = openvr.getDeviceType(i);
		switch (deviceType)
		{
		case TRACKED_DEVICE_TRACKING:
			if (curTracking == 0)
				BASESTATION_DEVICE_0 = i;
			else
				BASESTATION_DEVICE_1 = i;
			curTracking++;
			break;
		case TRACKED_DEVICE_CONTROLLER:
			if (curController == 0)
				CONTROLLER_DEVICE_0 = i;
			else
				CONTROLLER_DEVICE_1 = i;
			curController++;
			break;
		case TRACKED_DEVICE_HMD:
			HMD_DEVICE_0 = i;
			break;
		default:
			break;
		}
	}
}

void VRPlayerOpenVR::vibrateController(int controller_num, float amplitude)
{
	if (xpad360->isAvailable())
	{
		if (controller_num == 0) xpad360->setVibration(amplitude * 0.2f, 0.0f);
		if (controller_num == 1) xpad360->setVibration(0.0f, amplitude * 0.2f);
	}

	if (amplitude > 0)
	{
		if (controller_num == 0) openvr.setControllerVibration(CONTROLLER_DEVICE_0, 100);
		if (controller_num == 1) openvr.setControllerVibration(CONTROLLER_DEVICE_1, 100);
	}
}

Mat4 VRPlayerOpenVR::getControllerTransform(int controller_num)
{
	return Mat4(controller_num == 0 ? openvr.getDevicePose(CONTROLLER_DEVICE_0) : openvr.getDevicePose(CONTROLLER_DEVICE_1));
}

int VRPlayerOpenVR::getControllerButton(int controller_num, int button)
{
	if (!controller_valid[controller_num])
		return 0;

	int device = (controller_num == 0 ? CONTROLLER_DEVICE_0 : CONTROLLER_DEVICE_1);

	switch (button)
	{
	case STICK_X: return Math::abs(openvr.getControllerAxis(device, 0).x) > 0.5f;
	case STICK_Y: return Math::abs(openvr.getControllerAxis(device, 0).y) > 0.5f;
	case TRIGGER: return openvr.getControllerAxis(device, 1).x > 0.5f;
	case GRIP: return openvr.getControllerButtonPressed(device, BUTTON_GRIP);
	case XA: return openvr_getControllerDPadPressed(device, BUTTON_DPAD_DOWN);
	case YB: return openvr_getControllerDPadPressed(device, BUTTON_DPAD_UP);
	case MENU: return openvr.getControllerButtonPressed(device, BUTTON_APPLICATIONMENU);
	}

	return 0;
}

float VRPlayerOpenVR::getControllerAxis(int controller_num, int button)
{
	if (!controller_valid[controller_num])
		return 0;

	int device = (controller_num == 0 ? CONTROLLER_DEVICE_0 : CONTROLLER_DEVICE_1);

	switch (button)
	{
	case STICK_X: return openvr.getControllerAxis(device, 0).x;
	case STICK_Y: return openvr.getControllerAxis(device, 0).y;
	case TRIGGER: return openvr.getControllerAxis(device, 1).x;
	case GRIP: return itof(openvr.getControllerButtonPressed(device, BUTTON_GRIP));
	case XA: return itof(openvr_getControllerDPadPressed(device, BUTTON_DPAD_DOWN));
	case YB: return itof(openvr_getControllerDPadPressed(device, BUTTON_DPAD_UP));
	case MENU: return itof(openvr.getControllerButtonPressed(device, BUTTON_APPLICATIONMENU));
	}

	return 0;
}

int VRPlayerOpenVR::openvr_getControllerDPadPressed(int device, int button)
{
	// You might expect that pressing one of the edges of the SteamVR controller touchpad could
	// be detected with a call to k_EButton_DPad_* (BUTTON_DPAD_*), but currently this always returns false.
	// Not sure whether this is SteamVR's design intent, not yet implemented, or a bug.
	// The expected behaviour can be achieved by detecting overall Touchpad press, with Touch-Axis comparison to an edge threshold.

	if (openvr.getControllerButtonPressed(device, BUTTON_STEAMVR_TOUCHPAD))
	{
		vec2 axis = openvr.getControllerAxis(device, 0);
		if ((axis.y > 0.3f && button == BUTTON_DPAD_UP) ||
			(axis.y < -0.3f && button == BUTTON_DPAD_DOWN) ||
			(axis.x > 0.6f && button == BUTTON_DPAD_RIGHT) ||
			(axis.x < -0.6f && button == BUTTON_DPAD_LEFT))
			return 1;
	}

	return 0;
}