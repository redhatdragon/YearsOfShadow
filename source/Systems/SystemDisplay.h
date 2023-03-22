#pragma once
#include INC_UTILS_PATH(ENGINE_PATH, DDECS.h)
#include INC_UTILS_PATH(ENGINE_PATH, PhysicsEngineAABB3D.h)
#include INC_UTILS_PATH(ENGINE_PATH, HAL/HAL.h)
#include <iostream>

class SystemDisplay : public System {
	ComponentID bodyComponentID;
	ComponentID meshComponentID;
public:
	virtual void init() {
		bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		meshComponentID = ecs.registerComponent("mesh", sizeof(void*));
	}
	virtual void run() {
		register uint32_t meshCount = ecs.getComponentCount(meshComponentID);
		void** meshBuffer = (void**)ecs.getComponentBuffer(meshComponentID);
		for (uint32_t i = 0; i < meshCount; i++) {
			EntityID owner = ecs.getOwner(meshComponentID, i);
			BodyID* bodyIDPtr = (BodyID*)ecs.getEntityComponent(owner, bodyComponentID);
			if (bodyIDPtr == nullptr) continue;
			Vec3D<float> pos = physics.getPosF(*bodyIDPtr);
			EE_setPositionMesh(meshBuffer[i], pos.x, pos.y, pos.z);
			EE_drawMesh(meshBuffer[i]);
		}
	}
	virtual const char* getName() {
		return "SystemDisplay";
	}
};