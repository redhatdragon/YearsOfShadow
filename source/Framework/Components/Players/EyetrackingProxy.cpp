#include "EyetrackingProxy.h"

using namespace Unigine;
using namespace Math;

void EyetrackingProxy::init()
{
	if (!isEyetrackingAvailable())
		return;

	interpreter = (Interpreter *)Engine::get()->getSystemInterpreter();
	reflection.reset(new Reflection(interpreter, Variable("engine.varjo.eyetracking")));

	for (int i = 0; i < reflection->getNumFunctions(); i++)
	{
		if (reflection->getFunctionName(i) == "getFocusWorldPosition")
			getFocusWorldPositionID = reflection->getFunctionID(i);
		if (reflection->getFunctionName(i) == "isValid")
			isValidID = reflection->getFunctionID(i);
	}
}

bool EyetrackingProxy::isEyetrackingAvailable() const
{
	return Engine::get()->findPlugin("Varjo") != -1;
}

Vec3 EyetrackingProxy::getFocusWorldPosition()
{
	if (getFocusWorldPositionID == -1)
		return Vec3_zero;

	Variable v;
	v = reflection->callExternFunction(getFocusWorldPositionID);
	if (v.isVec3())
		return Vec3{v.getVec3()};
	return Vec3{v.getDVec3()};
}

bool EyetrackingProxy::isValid() const
{
	if (getFocusWorldPositionID == -1)
		return false;

	Variable v;
	v = reflection->callExternFunction(isValidID);
	return v.getInt() != 0;
}
