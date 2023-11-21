#pragma once
#include "SystemUtilities/NetworkingTools.h"

using namespace NetworkingTools;

struct ReplicateEntity {
	Vec3D<FixedPoint<>> distToClient;
	EntityID serverEntityID;
	EntityHandle serverEntityHandle;
};

class SystemNetwork : public System {
	ComponentID bodyComponentID;
	ComponentID controllerComponentID;
	ComponentID replicateEntityComponentID;
	NetworkManager nm;

public:
	virtual void init() {
		OPTICK_THREAD("MainThread");
		OPTICK_EVENT();
		bodyComponentID = ecs.registerComponentAsBlittable("body", sizeof(BodyID));
		controllerComponentID = ecs.registerComponentAsBlittable("controller", sizeof(Controller));
		replicateEntityComponentID = ecs.registerComponentAsBlittable("replicateEntity", sizeof(ReplicateEntity));
	}
	virtual void run() {
		OPTICK_THREAD("MainThread");
		OPTICK_EVENT();
		#if GAME_TYPE != GAME_TYPE_SINGLE
		#if GAME_TYPE == GAME_TYPE_SERVER
			serverLogic();
		#else
			clientLogic();
		#endif
		#endif
	}
	virtual const char* getName() {
		return "SystemNetwork";
	}
private:
	#if GAME_TYPE == GAME_TYPE_SERVER
	inline void serverLogic() {
		uint32_t count = ecs.getComponentCount(replicateEntityComponentID);
		ReplicateEntity* replicateEntityBuff =
			(ReplicateEntity*)ecs.getComponentBuffer(replicateEntityComponentID);
		for (uint32_t i = 0; i < count; i++) {
			
		}
	}
	#endif
	#if GAME_TYPE == GAME_TYPE_CLIENT
	inline void clientLogic() {

	}
	#endif
};