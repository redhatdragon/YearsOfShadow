#pragma once

/* Description: converter from UnigineScript API to C++ API */

#include <UnigineEngine.h>
#include <UnigineInterpreter.h>
#include <UnigineInterface.h>
#include <memory>

#define MAX_TRACKED_DEVICE_COUNT 16
#define TRACKED_DEVICE_INVALID 0
#define TRACKED_DEVICE_HMD 1
#define TRACKED_DEVICE_CONTROLLER 2
#define TRACKED_DEVICE_TRACKING 4
#define TRACKED_DEVICE_OTHER 1000
#define CONTROLLER_AXIS_NONE 0
#define CONTROLLER_AXIS_TRACKPAD 1
#define CONTROLLER_AXIS_JOYSTICK 2
#define CONTROLLER_AXIS_TRIGGER 3
#define BUTTON_SYSTEM 0
#define BUTTON_APPLICATIONMENU 1
#define BUTTON_GRIP 2
#define BUTTON_DPAD_LEFT 3
#define BUTTON_DPAD_UP 4
#define BUTTON_DPAD_RIGHT 5
#define BUTTON_DPAD_DOWN 6
#define BUTTON_A 7
#define BUTTON_AXIS0 32
#define BUTTON_AXIS1 33
#define BUTTON_AXIS2 34
#define BUTTON_AXIS3 35
#define BUTTON_AXIS4 36
#define BUTTON_STEAMVR_TOUCHPAD 32
#define BUTTON_STEAMVR_TRIGGER 33
#define BUTTON_DASHBOARD_BACK 2
#define BUTTON_MAX 64
#define CONTROLLER_STATE_AXIS_COUNT 5

using namespace Unigine;
using namespace Math;

class OpenVRProxy
{
	Interpreter *interpreter;
	std::shared_ptr<Reflection> reflection;
	
	int getDevicePoseID = -1,
		getDeviceVelocityID = -1,
		getDeviceAngularVelocityID = -1,
		isDeviceConnectedID = -1,
		isPoseValidID = -1,
		getTrackingResultID = -1,
		getDeviceTypeID = -1,
		getControllerPacketNumID = -1,
		getControllerButtonPressedID = -1,
		getControllerButtonTouchedID = -1,
		getControllerAxisID = -1,
		setControllerVibrationID = -1,
		setHeadPositionLockID = -1,
		setHeadRotationLockID = -1,
		isHeadPositionLockID = -1,
		isHeadRotationLockID = -1;

public:
	void init();

	const mat4 &getDevicePose(int device);
	const vec3 &getDeviceVelocity(int device);
	const vec3 &getDeviceAngularVelocity(int device);
	int isDeviceConnected(int device);
	int isPoseValid(int device);
	int getTrackingResult(int device);
	int getDeviceType(int device);
	int getControllerPacketNum(int device);
	int getControllerButtonPressed(int device, int button);
	int getControllerButtonTouched(int device, int button);
	const vec2 &getControllerAxis(int device, int axis);
	void setControllerVibration(int device, unsigned short duration);
	void setHeadPositionLock(int lock);
	void setHeadRotationLock(int lock);
	int isHeadPositionLock();
	int isHeadRotationLock();
};


