#include "OculusProxy.h"

void OculusProxy::init()
{
	interpreter = (Interpreter *)Engine::get()->getSystemInterpreter();
	reflection = std::make_shared<Reflection>(interpreter, Variable("engine.oculus"));

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

const mat4 &OculusProxy::getDevicePose(int device)
{
	Variable v;
	v = reflection->callExternFunction(getDevicePoseID, Variable(device));
	return v.getMat4();
}

const vec3 &OculusProxy::getDeviceVelocity(int device)
{
	Variable v;
	v = reflection->callExternFunction(getDeviceVelocityID, Variable(device));
	return v.getVec3();
}

const vec3 &OculusProxy::getDeviceAngularVelocity(int device)
{
	Variable v;
	v = reflection->callExternFunction(getDeviceAngularVelocityID, Variable(device));
	return v.getVec3();
}

int OculusProxy::isDeviceConnected(int device)
{
	Variable v;
	v = reflection->callExternFunction(isDeviceConnectedID, Variable(device));
	return v.getInt();
}

int OculusProxy::isPoseValid(int device)
{
	Variable v;
	v = reflection->callExternFunction(isPoseValidID, Variable(device));
	return v.getInt();
}

int OculusProxy::getControllerButtonPressed(int button)
{
	Variable v;
	v = reflection->callExternFunction(getControllerButtonPressedID, Variable(button));
	return v.getInt();
}

int OculusProxy::getControllerButtonTouched(int button)
{
	Variable v;
	v = reflection->callExternFunction(getControllerButtonTouchedID, Variable(button));
	return v.getInt();
}

const vec2 &OculusProxy::getControllerAxis(int device, int axis)
{
	Variable v;
	v = reflection->callExternFunction(getControllerAxisID, Variable(device), Variable(axis));
	return v.getVec2();
}

void OculusProxy::setControllerVibration(int device, unsigned short duration, float amplitude)
{
	reflection->callExternFunction(setControllerVibrationID, Variable(device), Variable(duration), Variable(amplitude));
}

void OculusProxy::setHeadPositionLock(int lock)
{
	reflection->callExternFunction(setHeadPositionLockID, Variable(lock));
}

void OculusProxy::setHeadRotationLock(int lock)
{
	reflection->callExternFunction(setHeadRotationLockID, Variable(lock));
}

int OculusProxy::isHeadPositionLock()
{
	Variable v;
	v = reflection->callExternFunction(isHeadPositionLockID);
	return v.getInt();
}

int OculusProxy::isHeadRotationLock()
{
	Variable v;
	v = reflection->callExternFunction(isHeadRotationLockID);
	return v.getInt();
}