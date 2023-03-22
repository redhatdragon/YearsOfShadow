#include "AppOculusProxy.h"

void AppOculusProxy::init()
{
	interpreter = (Interpreter *)Engine::get()->getSystemInterpreter();
	reflection = std::make_shared<Reflection>(interpreter, Variable("engine.oculus"));

	for (int i = 0; i < reflection->getNumFunctions(); i++)
	{
		if (reflection->getFunctionName(i) == "getDevicePose")
			getDevicePoseID = reflection->getFunctionID(i);
		if (reflection->getFunctionName(i) == "getDeviceVelocity")
			getDeviceVelocityID = reflection->getFunctionID(i);
		if (reflection->getFunctionName(i) == "getDeviceAngularVelocity")
			getDeviceAngularVelocityID = reflection->getFunctionID(i);
		if (reflection->getFunctionName(i) == "isDeviceConnected")
			isDeviceConnectedID = reflection->getFunctionID(i);
		if (reflection->getFunctionName(i) == "isPoseValid")
			isPoseValidID = reflection->getFunctionID(i);
		if (reflection->getFunctionName(i) == "getControllerButtonPressed")
			getControllerButtonPressedID = reflection->getFunctionID(i);
		if (reflection->getFunctionName(i) == "getControllerButtonTouched")
			getControllerButtonTouchedID = reflection->getFunctionID(i);
		if (reflection->getFunctionName(i) == "getControllerAxis")
			getControllerAxisID = reflection->getFunctionID(i);
		if (reflection->getFunctionName(i) == "setControllerVibration")
			setControllerVibrationID = reflection->getFunctionID(i);
		if (reflection->getFunctionName(i) == "setHeadPositionLock")
			setHeadPositionLockID = reflection->getFunctionID(i);
		if (reflection->getFunctionName(i) == "setHeadRotationLock")
			setHeadRotationLockID = reflection->getFunctionID(i);
		if (reflection->getFunctionName(i) == "isHeadPositionLocked")
			isHeadPositionLockedID = reflection->getFunctionID(i);
		if (reflection->getFunctionName(i) == "isHeadRotationLocked")
			isHeadRotationLockedID = reflection->getFunctionID(i);
	}
}

const mat4 &AppOculusProxy::getDevicePose(int device)
{
	Variable v;
	v = reflection->callExternFunction(getDevicePoseID, Variable(device));
	return v.getMat4();
}

const vec3 &AppOculusProxy::getDeviceVelocity(int device)
{
	Variable v;
	v = reflection->callExternFunction(getDeviceVelocityID, Variable(device));
	return v.getVec3();
}

const vec3 &AppOculusProxy::getDeviceAngularVelocity(int device)
{
	Variable v;
	v = reflection->callExternFunction(getDeviceAngularVelocityID, Variable(device));
	return v.getVec3();
}

int AppOculusProxy::isDeviceConnected(int device)
{
	Variable v;
	v = reflection->callExternFunction(isDeviceConnectedID, Variable(device));
	return v.getInt();
}

int AppOculusProxy::isPoseValid(int device)
{
	Variable v;
	v = reflection->callExternFunction(isPoseValidID, Variable(device));
	return v.getInt();
}

int AppOculusProxy::getControllerButtonPressed(int button)
{
	Variable v;
	v = reflection->callExternFunction(getControllerButtonPressedID, Variable(button));
	return v.getInt();
}

int AppOculusProxy::getControllerButtonTouched(int button)
{
	Variable v;
	v = reflection->callExternFunction(getControllerButtonTouchedID, Variable(button));
	return v.getInt();
}

const vec2 &AppOculusProxy::getControllerAxis(int device, int axis)
{
	Variable v;
	v = reflection->callExternFunction(getControllerAxisID, Variable(device), Variable(axis));
	return v.getVec2();
}

void AppOculusProxy::setControllerVibration(int device, unsigned short duration, float amplitude)
{
	reflection->callExternFunction(setControllerVibrationID, Variable(device), Variable(duration), Variable(amplitude));
}

void AppOculusProxy::setHeadPositionLock(int lock)
{
	reflection->callExternFunction(setHeadPositionLockID, Variable(lock));
}

void AppOculusProxy::setHeadRotationLock(int lock)
{
	reflection->callExternFunction(setHeadRotationLockID, Variable(lock));
}

int AppOculusProxy::isHeadPositionLocked()
{
	Variable v;
	v = reflection->callExternFunction(isHeadPositionLockedID);
	return v.getInt();
}

int AppOculusProxy::isHeadRotationLocked()
{
	Variable v;
	v = reflection->callExternFunction(isHeadRotationLockedID);
	return v.getInt();
}