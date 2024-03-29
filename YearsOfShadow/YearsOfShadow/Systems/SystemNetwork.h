#pragma once
#include "SystemUtilities/NetworkingTools.h"
#include "SystemUtilities/Serialize.h"

using namespace NetworkingTools;

class SystemNetwork : public System {
	NetworkManager nm;

public:
	virtual void init() {
		OPTICK_THREAD("MainThread");
		OPTICK_EVENT();
		#if GAME_TYPE == GAME_TYPE_SERVER || GAME_TYPE == GAME_TYPE_CLIENT
		#if GAME_TYPE == GAME_TYPE_SERVER
		auto conn = HAL::UDP_open("127.0.0.1", 8122, 8123);
		#elif GAME_TYPE == GAME_TYPE_CLIENT
		auto conn = HAL::UDP_open("127.0.0.1", 8123, 8122);
		#endif
		nm.init(conn);
		#endif
	}
	virtual void run() {
		OPTICK_THREAD("MainThread");
		OPTICK_EVENT();
		#if GAME_TYPE == GAME_TYPE_SERVER
			serverLogic();
			//spawnTest();
		#elif GAME_TYPE == GAME_TYPE_CLIENT
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
		entities.resize(count);
		for (uint32_t i = 0; i < count; i++) {
			entities[i] = ecs.getOwner(replicateEntityComponentID, i);
		}
		if (count == 0)
			return;
		SerialEntityBuffer* seBuff;
		std::vector<uint8_t> seVec = {};
		SerialEntityBuffer::constructNew(&entities[0], count, seVec);

		uint32_t size = (uint32_t)seVec.size();
		if (size != 0) {
			seBuff = (SerialEntityBuffer*)&seVec[0];
			//HAL_LOG("HIT {}, {}\n", count, size);
			//SerialEntityBuffer::log(seBuff);
			nm.trySendTo("127.0.0.1", (uint8_t*)seBuff, size);
		}

		std::vector<uint8_t> buff = {};
		while (true) {
			buff.resize(0);
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
			//HAL_LOG("HIT {}", buff.size());
			SerialEntityBuffer* se = (SerialEntityBuffer*)&buff[0];
			//SerialEntityBuffer::log(se);
			//while (true) {
			//	se->log();
			//	se->deserializeToDDECS();
			//	if ((se = SerialEntity::getNextSerialEntity(se)) == nullptr)
			//		break;
			//}
			se->deserializeToDDECS();
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