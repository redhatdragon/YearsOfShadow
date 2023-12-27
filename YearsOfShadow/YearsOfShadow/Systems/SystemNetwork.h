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
		#if GAME_TYPE == GAME_TYPE_SERVER || GAME_TYPE == GAME_TYPE_CLIENT
		bodyComponentID = ecs.registerComponentAsBlittable("body", sizeof(BodyID));
		controllerComponentID = ecs.registerComponentAsBlittable("controller", sizeof(Controller));
		replicateEntityComponentID = ecs.registerComponentAsBlittable("replicateEntity", sizeof(ReplicateEntity));

		auto conn = HAL::UDP_open("127.0.0.1", 8122, 8123);
		nm.init(conn);
		#endif
	}
	virtual void run() {
		OPTICK_THREAD("MainThread");
		OPTICK_EVENT();
		#if GAME_TYPE == GAME_TYPE_SERVER
			serverLogic();
			spawnTest();
		#elif GAME_TYPE == GAME_TYPE_CLIENT
		return;
			clientLogic();
		#endif
	}
	virtual const char* getName() {
		return "SystemNetwork";
	}
private:
	#if GAME_TYPE == GAME_TYPE_SERVER
	inline void serverLogic() {
		using namespace SystemUtilities;
		nm.update();
		uint32_t count = ecs.getComponentCount(replicateEntityComponentID);
		ReplicateEntity* replicateEntityBuff =
			(ReplicateEntity*)ecs.getComponentBuffer(replicateEntityComponentID);
		std::vector<EntityID> entities;
		entities.reserve(count);
		for (uint32_t i = 0; i < count; i++) {
			entities.push_back(ecs.getOwner(replicateEntityComponentID, i));
		}
		if (count == 0)
			return;
		uint32_t size;
		SerialEntity* seBuff = SerialEntity::constructSerialEntityBuffer(&entities[0], count, size);
		nm.trySendTo("127.0.0.1", (uint8_t*)seBuff, size);

		static std::vector<uint8_t> buff;
		while (true) {
			buff.clear();
			nm.tryPopNextMsg(buff);
			if (buff.size() == 0)
				break;
		}
	}
	#endif
	#if GAME_TYPE == GAME_TYPE_CLIENT
	inline void clientLogic() {
		using namespace SystemUtilities;
		nm.update();
		static std::vector<uint8_t> buff;
		while (true) {
			buff.clear();
			nm.tryPopNextMsg(buff);
			if (buff.size() == 0)
				break;
			HAL_LOG("HIT {}", buff.size());
			SerialEntity* se = (SerialEntity*)&buff[0];
			if (se->isEnd())
				break;
			while (true) {
				se->deserializeToDDECS();
				if ((se = se->next()) == nullptr)
					break;
			}
		}
	}
	#endif

	void spawnTest() {
		if (ecs.getTicksPassed() % 60)
			return;
		EntityID entity = ecs.getNewEntity();
		uint32_t x = rand() % 200 + 20;
		uint32_t y = rand() % 200 + 20;
		uint32_t z = rand() % 100;
		BodyID bodyID = physics.addBodyBox(x, y, z, 1, 1, 1, to_void_ptr(entity), true);
		ecs.emplace(entity, bodyComponentID, &bodyID);
		ReplicateEntity re;
		re.init(entity, ecs.entityGetHandle(entity));
		ecs.emplace(entity, replicateEntityComponentID, &re);
		ComponentID instancedMeshComponentID = ecs.registerComponentAsBlittable("instancedMesh", sizeof(u32));
		u32 imeshID = instancedMeshCodex.add("./Data/Meshes/Props/Dynamite.obj");
		instancedMeshCodex.setTexture(imeshID, "./Data/Meshes/Props/D_Dynamite.png");
		instancedMeshCodex.setSize(imeshID, {1, 1, 1});
		ecs.emplace(entity, instancedMeshComponentID, &imeshID);
	}
};