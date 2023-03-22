#pragma once
#include INC_UTILS_PATH(ENGINE_PATH, DDECS.h)
#include INC_UTILS_PATH(ENGINE_PATH, PhysicsEngineAABB3D.h)
#include "../Voxel.h"
#include <iostream>

class SystemVoxel : public System {
	
public:
	virtual void init() {
		voxelWorld.init(100, 0, 0, 0);
	}
	virtual void run() {
		voxelWorld.display();
	}
	virtual const char* getName() {
		return "SystemVoxel";
	}
};