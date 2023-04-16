#pragma once
#include INC_UTILS_PATH(ENGINE_PATH, DDECS.h)
#include INC_UTILS_PATH(ENGINE_PATH, PhysicsEngineAABB3D.h)
//#include "../FP_FPSCamera.h"  //Postoned till fixed
#include "../FPSCamera.h"
#include <iostream>

class SystemEnemy : public System {
	ComponentID bodyComponentID;
	ComponentID controllerComponentID;
	ComponentID meshComponentID;

public:
	virtual void init() {
		bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		controllerComponentID = ecs.registerComponent("controller", sizeof(Controller));
		meshComponentID = ecs.registerComponent("mesh", sizeof(void*));
	}
	virtual void run() {

	}
	virtual const char* getName() {
		return "SystemController";
	}
};