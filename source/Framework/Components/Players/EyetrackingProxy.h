#pragma once

/* Description: converter from UnigineScript API to C++ API */

#include <UnigineEngine.h>
#include <UnigineInterpreter.h>
#include <UnigineInterface.h>
#include <memory>

class EyetrackingProxy
{
	Unigine::Interpreter *interpreter;
	std::unique_ptr<Unigine::Reflection> reflection;

	int getFocusWorldPositionID = -1,
		isValidID = -1;

public:
	void init();
	
	bool isEyetrackingAvailable() const;
	Unigine::Math::Vec3 getFocusWorldPosition();
	bool isValid() const;
};
