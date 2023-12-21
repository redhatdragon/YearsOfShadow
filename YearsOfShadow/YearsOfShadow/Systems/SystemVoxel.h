#pragma once
#include "SystemUtilities/Voxel.h"
#include "SystemController.h"
#include <iostream>
#include <memory>

class SystemVoxel : public System {
	ComponentID controllerComponentID;
public:
	virtual void init() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
		voxelWorld.init(100, 0, 0, 0);
        controllerComponentID = ecs.registerComponentAsBlittable("controller", sizeof(Controller));
	}
	virtual void run() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
		Controller* controller = (Controller*)ecs.getComponentBuffer(controllerComponentID);
		auto pos = controller->cam.getPosition();
		voxelWorld.update((uint32_t)pos.x, (uint32_t)pos.z);
	}
	virtual void destruct() {
		voxelWorld.destruct();
	}
	virtual const char* getName() {
		return "SystemVoxel";
	}
};