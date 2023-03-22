#pragma once

/* Description: converter from UnigineScript API to C++ API */

#include <UnigineEngine.h>
#include <UnigineInterpreter.h>
#include <UnigineInterface.h>
#include <memory>

// devices
#define	OCULUS_DEVICE_HMD 0
#define	OCULUS_DEVICE_CONTROLLER_LEFT 1
#define	OCULUS_DEVICE_CONTROLLER_RIGHT 2


// buttons
// - right touch controller
#define	OCULUS_BUTTON_A 1
#define	OCULUS_BUTTON_B 2
#define	OCULUS_BUTTON_RTHUMB 4
#define	OCULUS_BUTTON_RSHOULDER 8

// - left touch controller
#define	OCULUS_BUTTON_X 256
#define	OCULUS_BUTTON_Y 512
#define	OCULUS_BUTTON_LTHUMB 1024
#define	OCULUS_BUTTON_LSHOULDER 2048

// - remote controller
#define	OCULUS_BUTTON_UP 65536
#define	OCULUS_BUTTON_DOWN 131072
#define	OCULUS_BUTTON_LEFT 262144
#define	OCULUS_BUTTON_RIGHT 524288
#define	OCULUS_BUTTON_ENTER 1048576
#define	OCULUS_BUTTON_BACK 2097152
#define	OCULUS_BUTTON_VOLUP 4194304
#define	OCULUS_BUTTON_VOLDOWN 8388608
#define	OCULUS_BUTTON_HOME 16777216


// touches
// - right touch controller
#define	OCULUS_TOUCH_A 1
#define	OCULUS_TOUCH_B 2
#define	OCULUS_TOUCH_RTHUMB 4
#define	OCULUS_TOUCH_RTHUMB_REST 8
#define	OCULUS_TOUCH_RINDEX_TRIGGER 16
#define	OCULUS_TOUCH_RINDEX_POINTING 32
#define	OCULUS_TOUCH_RTHUMB_UP 64

// - left touch controller
#define	OCULUS_TOUCH_X 256
#define	OCULUS_TOUCH_Y 512
#define	OCULUS_TOUCH_LTHUMB 1024
#define	OCULUS_TOUCH_LTHUMB_REST 2048
#define	OCULUS_TOUCH_LINDEX_TRIGGER 4096
#define	OCULUS_TOUCH_LINDEX_POINTING 8192
#define	OCULUS_TOUCH_LTHUMB_UP 16384


// axis
#define OCULUS_AXIS_INDEX_TRIGGER 0
#define OCULUS_AXIS_HAND_TRIGGER 1
#define OCULUS_AXIS_THUMBSTICK 2
#define OCULUS_AXIS_INDEX_TRIGGER_NO_DEADZONE 3
#define OCULUS_AXIS_HAND_TRIGGER_NO_DEADZONE 4
#define OCULUS_AXIS_THUMBSTICK_NO_DEADZONE 5

using namespace Unigine;
using namespace Math;

class OculusProxy
{
	Interpreter *interpreter;
	std::shared_ptr<Reflection> reflection;

	int getDevicePoseID = -1,
		getDeviceVelocityID = -1,
		getDeviceAngularVelocityID = -1,
		isDeviceConnectedID = -1,
		isPoseValidID = -1,
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
	int getControllerButtonPressed(int button);
	int getControllerButtonTouched(int button);
	const vec2 &getControllerAxis(int device, int axis);
	void setControllerVibration(int device, unsigned short duration, float amplitude);
	void setHeadPositionLock(int lock);
	void setHeadRotationLock(int lock);
	int isHeadPositionLock();
	int isHeadRotationLock();
};
