#pragma once
#include "SystemUtilities/NetworkingTools.h"
#include "SystemUtilities/Serialize.h"

using namespace NetworkingTools;

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

		auto conn = HAL::UDP_open("127.0.0.1", 8122, 8123);
		nm.init(conn);
	}
	virtual void run() {
		OPTICK_THREAD("MainThread");
		OPTICK_EVENT();
		#if GAME_TYPE == GAME_TYPE_SERVER
			serverLogic();
		#else
			clientLogic();
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
		nm.update();
		static std::vector<uint8_t> buff;
		while (true) {
			buff.clear();
			nm.tryPopNextMsg(buff);
			if (buff.size() == 0)
				break;

		}
	}
	#endif
};