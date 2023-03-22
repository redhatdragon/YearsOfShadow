#pragma once
#include INC_UTILS_PATH(ENGINE_PATH, DDECS.h)
#include INC_UTILS_PATH(ENGINE_PATH, PhysicsEngineAABB3D.h)
#include <iostream>

class SystemPhysics : public System {
	static void destructPhysicsBody(ComponentID componentID, uint32_t index) {
		EntityID entity = ecs.getOwner(componentID, index);
		BodyID bodyID = ecs.getEntityComponentAs<BodyID>(entity, componentID);
		physics.removeBody(bodyID);
	}
public:
	virtual void init() {
		ComponentID bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		ecs.registerDestructor(bodyComponentID, destructPhysicsBody);
		physics.init();
	}
	virtual void run() {
		clock_t startTime = clock();

		physics.tick();

		ms = clock() - startTime;
	}
	virtual const char* getName() {
		return "SystemPhysics";
	}
};