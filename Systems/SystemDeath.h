#include INC_UTILS_PATH(ENGINE_PATH, DDECS.h)
#include <iostream>

class SystemDeath : public System {
	ComponentID deadComponentID = -1;
public:
	virtual void init() {
		deadComponentID = ecs.registerComponent("dead", NULL);
	}
	virtual void run() {
		uint32_t deadCount = ecs.getComponentCount(deadComponentID);
		std::vector<EntityID> entities;
		for (uint32_t i = 0; i < deadCount; i++) {
			EntityID entity = ecs.getOwner(deadComponentID, i);
			entities.push_back(entity);
		}
		for (uint32_t i = 0; i < deadCount; i++) {
			EntityID e = entities[i];
			ecs.removeEntity(e);
		}
	}
	virtual const char* getName() {
		return "SystemDeath";
	}
};