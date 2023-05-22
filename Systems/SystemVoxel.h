#pragma once
#include "SystemUtilities/Voxel.h"
#include "SystemController.h"
#include <iostream>
#include <memory>

//std::vector<int> testVec;
//FlatBuffer<int, 100000000> testBuffer;
//std::unique_ptr<int[]> testBuffer = std::make_unique<int[]>(100000000);

class SystemVoxel : public System {
	ComponentID controllerComponentID;
public:
	virtual void init() {
		voxelWorld.init(100, 0, 0, 0);
		//testBuffer.init(100000000);
		//for (uint32_t i = 0; i < 100000000; i+=4) {
		//	testBuffer[i] = i;
		//	testBuffer[i+1] = i+1;
		//	testBuffer[i+2] = i+2;
		//	testBuffer[i+3] = i+3;
		//}
		controllerComponentID = ecs.registerComponent("controller", sizeof(Controller));
	}
	virtual void run() {
		Controller* controller = (Controller*)ecs.getComponentBuffer(controllerComponentID);
		auto pos = controller->cam.getPosition();
		voxelWorld.display(pos.x, pos.z);
	}
	virtual const char* getName() {
		return "SystemVoxel";
	}
};