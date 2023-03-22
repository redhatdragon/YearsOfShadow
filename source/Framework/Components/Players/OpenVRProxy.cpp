#include "OpenVRProxy.h"

void OpenVRProxy::init()
{
	interpreter = (Interpreter *)Engine::get()->getSystemInterpreter();
	reflection = std::make_shared<Reflection>(interpreter, Variable("engine.openvr"));

	for (int i = 0; i < reflection->getNumFunctions(); i++)
	{
		String func_name = reflection->getFunctionName(i);
		if (func_name == "getDevicePose")
			getDevicePoseID = reflection->getFunctionID(i);
		else if (func_name == "getDeviceVelocity")
			getDeviceVelocityID = reflection->getFunctionID(i);
		else if (func_name == "getDeviceAngularVelocity")
			getDeviceAngularVelocityID = reflection->getFunctionID(i);
		else if (func_name == "isDeviceConnected")
			isDeviceConnectedID = reflection->getFunctionID(i);
		else if (func_name == "isPoseValid")
			isPoseValidID = reflection->getFunctionID(i);
		else if (func_name == "getTrackingResult")
			getTrackingResultID = reflection->getFunctionID(i);
		else if (func_name == "getDeviceType")
			getDeviceTypeID = reflection->getFunctionID(i);
		else if (func_name == "getControllerPacketNum")
			getControllerPacketNumID = reflection->getFunctionID(i);
		else if (func_name == "getControllerButtonPressed")
			getControllerButtonPressedID = reflection->getFunctionID(i);
		else if (func_name == "getControllerButtonTouched")
			getControllerButtonTouchedID = reflection->getFunctionID(i);
		else if (func_name == "getControllerAxis")
			getControllerAxisID = reflection->getFunctionID(i);
		else if (func_name == "setControllerVibration")
			setControllerVibrationID = reflection->getFunctionID(i);
		else if (func_name == "setHeadPositionLock")
			setHeadPositionLockID = reflection->getFunctionID(i);
		else if (func_name == "setHeadRotationLock")
			setHeadRotationLockID = reflection->getFunctionID(i);
		else if (func_name == "isHeadPositionLock")
			isHeadPositionLockID = reflection->getFunctionID(i);
		else if (func_name == "isHeadRotationLock")
			isHeadRotationLockID = reflection->getFunctionID(i);
	}
}

const mat4 &OpenVRProxy::getDevicePose(int device)
{
	Variable v;
	v = reflection->callExternFunction(getDevicePoseID, Variable(device));
	return v.getMat4();
}

const vec3 &OpenVRProxy::getDeviceVelocity(int device)
{
	Variable v;
	v = reflection->callExternFunction(getDeviceVelocityID, Variable(device));
	return v.getVec3();
}

const vec3 &OpenVRProxy::getDeviceAngularVelocity(int device)
{
	Variable v;
	v = reflection->callExternFunction(getDeviceAngularVelocityID, Variable(device));
	return v.getVec3();
}

int OpenVRProxy::isDeviceConnected(int device)
{
	Variable v;
	v = reflection->callExternFunction(isDeviceConnectedID, Variable(device));
	return v.getInt();
}

int OpenVRProxy::isPoseValid(int device)
{
	Variable v;
	v = reflection->callExternFunction(isPoseValidID, Variable(device));
	return v.getInt();
}

int OpenVRProxy::getTrackingResult(int device)
{
	Variable v;
	v = reflection->callExternFunction(getTrackingResultID, Variable(device));
	return v.getInt();
}

int OpenVRProxy::getDeviceType(int device)
{
	Variable v;
	v = reflection->callExternFunction(getDeviceTypeID, Variable(device));
	return v.getInt();
}

int OpenVRProxy::getControllerPacketNum(int device)
{
	Variable v;
	v = reflection->callExternFunction(getControllerPacketNumID, Variable(device));
	return v.getInt();
}

int OpenVRProxy::getControllerButtonPressed(int device, int button)
{
	Variable v;
	v = reflection->callExternFunction(getControllerButtonPressedID, Variable(device), Variable(button));
	return v.getInt();
}

int OpenVRProxy::getControllerButtonTouched(int device, int button)
{
	Variable v;
	v = reflection->callExternFunction(getControllerButtonTouchedID, Variable(device), Variable(button));
	return v.getInt();
}

const vec2 &OpenVRProxy::getControllerAxis(int device, int axis)
{
	Variable v;
	v = reflection->callExternFunction(getControllerAxisID, Variable(device), Variable(axis));
	return v.getVec2();
}

void OpenVRProxy::setControllerVibration(int device, unsigned short duration)
{
	reflection->callExternFunction(setControllerVibrationID, Variable(device), Variable(duration));
}

void OpenVRProxy::setHeadPositionLock(int lock)
{
	reflection->callExternFunction(setHeadPositionLockID, Variable(lock));
}

void OpenVRProxy::setHeadRotationLock(int lock)
{
	reflection->callExternFunction(setHeadRotationLockID, Variable(lock));
}

int OpenVRProxy::isHeadPositionLock()
{
	Variable v;
	v = reflection->callExternFunction(isHeadPositionLockID);
	return v.getInt();
}

int OpenVRProxy::isHeadRotationLock()
{
	Variable v;
	v = reflection->callExternFunction(isHeadRotationLockID);
	return v.getInt();
}