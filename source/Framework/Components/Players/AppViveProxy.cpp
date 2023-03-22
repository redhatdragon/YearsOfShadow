#include "AppViveProxy.h"

void AppViveProxy::init()
{
	interpreter = (Interpreter *)Engine::get()->getSystemInterpreter();
	reflection = std::make_shared<Reflection>(interpreter, Variable("engine.vive"));

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
		if (reflection->getFunctionName(i) == "getTrackingResult")
			getTrackingResultID = reflection->getFunctionID(i);
		if (reflection->getFunctionName(i) == "getDeviceType")
			getDeviceTypeID = reflection->getFunctionID(i);
		if (reflection->getFunctionName(i) == "getControllerPacketNum")
			getControllerPacketNumID = reflection->getFunctionID(i);
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

const mat4 &AppViveProxy::getDevicePose(int device)
{
	Variable v;
	v = reflection->callExternFunction(getDevicePoseID, Variable(device));
	return v.getMat4();
}

const vec3 &AppViveProxy::getDeviceVelocity(int device)
{
	Variable v;
	v = reflection->callExternFunction(getDeviceVelocityID, Variable(device));
	return v.getVec3();
}

const vec3 &AppViveProxy::getDeviceAngularVelocity(int device)
{
	Variable v;
	v = reflection->callExternFunction(getDeviceAngularVelocityID, Variable(device));
	return v.getVec3();
}

int AppViveProxy::isDeviceConnected(int device)
{
	Variable v;
	v = reflection->callExternFunction(isDeviceConnectedID, Variable(device));
	return v.getInt();
}

int AppViveProxy::isPoseValid(int device)
{
	Variable v;
	v = reflection->callExternFunction(isPoseValidID, Variable(device));
	return v.getInt();
}

int AppViveProxy::getTrackingResult(int device)
{
	Variable v;
	v = reflection->callExternFunction(getTrackingResultID, Variable(device));
	return v.getInt();
}

int AppViveProxy::getDeviceType(int device)
{
	Variable v;
	v = reflection->callExternFunction(getDeviceTypeID, Variable(device));
	return v.getInt();
}

int AppViveProxy::getControllerPacketNum(int device)
{
	Variable v;
	v = reflection->callExternFunction(getControllerPacketNumID, Variable(device));
	return v.getInt();
}

int AppViveProxy::getControllerButtonPressed(int device, int button)
{
	Variable v;
	v = reflection->callExternFunction(getControllerButtonPressedID, Variable(device), Variable(button));
	return v.getInt();
}

int AppViveProxy::getControllerButtonTouched(int device, int button)
{
	Variable v;
	v = reflection->callExternFunction(getControllerButtonTouchedID, Variable(device), Variable(button));
	return v.getInt();
}

const vec2 &AppViveProxy::getControllerAxis(int device, int axis)
{
	Variable v;
	v = reflection->callExternFunction(getControllerAxisID, Variable(device), Variable(axis));
	return v.getVec2();
}

void AppViveProxy::setControllerVibration(int device, unsigned short duration)
{
	reflection->callExternFunction(setControllerVibrationID, Variable(device), Variable(duration));
}

void AppViveProxy::setHeadPositionLock(int lock)
{
	reflection->callExternFunction(setHeadPositionLockID, Variable(lock));
}

void AppViveProxy::setHeadRotationLock(int lock)
{
	reflection->callExternFunction(setHeadRotationLockID, Variable(lock));
}

int AppViveProxy::isHeadPositionLocked()
{
	Variable v;
	v = reflection->callExternFunction(isHeadPositionLockedID);
	return v.getInt();
}

int AppViveProxy::isHeadRotationLocked()
{
	Variable v;
	v = reflection->callExternFunction(isHeadRotationLockedID);
	return v.getInt();
}