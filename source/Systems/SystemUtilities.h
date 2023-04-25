#pragma once
#include <string>
#include <iostream>
#include "INC_UTILS.h"
#define ENGINE_PATH ../EchoEngine/
#include INC_UTILS_PATH(ENGINE_PATH, HAL/HAL_3D.h)
#include INC_UTILS_PATH(ENGINE_PATH, EntityObjectLoader.h)
#include INC_UTILS_PATH(ENGINE_PATH, FixedPoint.h)
#include INC_UTILS_PATH(ENGINE_PATH, Vec.h)
#include INC_UTILS_PATH(ENGINE_PATH, FlatBuffer.h)
#include INC_UTILS_PATH(ENGINE_PATH, Asset.h)
#include INC_UTILS_PATH(ENGINE_PATH, Name.h)
#include INC_UTILS_PATH(ENGINE_PATH, EE_Types.h)
//#include INC_UTILS_PATH(ENGINE_PATH, PhysicsEngineAABB3D.h)
//#include INC_UTILS_PATH(ENGINE_PATH, PhysicsEngineAABB3D_MT.h)

//#include "SystemUtilities/Inventory.h"
//#include "SystemUtilities/PlantData.h"
//#include "SystemUtilities/ItemData.h"
#include "SystemUtilities/Codex.h"

namespace SystemUtilities {
	inline void printDebugError(const std::string& str) {
		#ifdef DEBUG
		std::cout << "Error: " << str << std::endl;
		#endif
	}

	namespace DAMAGE_TYPES {
		enum {
			TRUE,
			AP,
			EXPLOSIVE,
			BALLISTIC,
		};
		constexpr uint32_t MAX = 4;
	};

	struct Health {
		uint32_t team;
		FixedPoint<> base;
		FixedPoint<> modifiers[DAMAGE_TYPES::MAX];
	};
	struct DamageOnCollision {
		uint32_t team;
		FixedPoint<256*256> base;
		FixedPoint<256*256> modifiers[DAMAGE_TYPES::MAX];
	};
	struct MoveToLocation {
		Vec3D<uint32_t> pos;
		Vec3D<uint32_t> siz;
		FixedPoint<256*256> speed;
	};

	//Related to physics.getUserData()
	inline bool isUserDataVoxel(BodyID bodyID) {
		if (physics.getUserData(bodyID) == (void*)-1)
			return true;
		return false;
	}
	//Related to physics.getUserData()
	inline void setUserDataVoxel(BodyID bodyID) {
		physics.setUserData(bodyID, (void*)-1);
	}

	inline EntityID spawnEntityAt(const std::string& entityPath, Vec3D<uint32_t> pos) {
		std::string dataDir = EE_getDirData();
		EntityObject entityObject = EntityObjectLoader::createEntityObjectFromFile(dataDir + entityPath);
		EntityID entity = ecs.getNewEntity();

		auto* size = entityObject.getComponent("size");
		if (size) {
			BodyID bodyID = physics.addBodyBox(pos.x, pos.y, pos.z, size->getArray()[0], size->getArray()[1], size->getArray()[2]);
			ComponentID bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
			physics.setUserData(bodyID, (void*)entity);
			ecs.emplace(entity, bodyComponentID, &bodyID);
		}

		for (auto& c : entityObject.components) {
			if (c.name == "size") continue;
			ComponentID componentID = ecs.registerComponent(c.name, c.size);
			if (c.type == ComponentObject::TYPE::TYPE_INT ||
				c.type == ComponentObject::TYPE::TYPE_FIXED_FLOAT ||
				c.type == ComponentObject::TYPE::TYPE_STRING ||
				c.type == ComponentObject::TYPE::TYPE_DARRAY)
				ecs.emplace(entity, componentID, &c.data);
			else if (c.type == ComponentObject::TYPE::TYPE_ARRAY)
				ecs.emplace(entity, componentID, c.data);
			else if (c.type == ComponentObject::TYPE::TYPE_NULL)
				ecs.emplace(entity, componentID, NULL);
			else {
				std::cout << "error: spawnEntityAt()'s entity has component of invalid type..?  destroying entity" << std::endl;
				ecs.removeEntity(entity);
				return -1;
			}
		}
		return entity;
	}
	inline EntityID spawnEntityAtWithSize(const std::string& entityPath, Vec3D<uint32_t> pos, Vec3D<uint32_t> siz) {
		EntityID retValue = spawnEntityAt(entityPath, pos);
		ComponentID bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		if (ecs.entityHasComponent(retValue, bodyComponentID)) {
			std::cout << "Error: spawnEntityAtWithSize()'s entity from entityPath had a size declared in file " <<
				entityPath << std::endl;
			return -1;
		}
		BodyID bodyID = physics.addBodyBox(pos.x, pos.y, pos.z, siz.x, siz.y, siz.z);
		physics.setUserData(bodyID, (void*)retValue);
		ecs.emplace(retValue, bodyComponentID, &bodyID);
		return retValue;
	}

	//inline uint64_t getEntityDistanceSquared(EntityID e1, EntityID e2) {
	//	ComponentID bodyComponentID = ecs.getComponentID("body");
	//	if (e1 == e2) {
	//		printDebugError("getEntityDistance()'s e1 and e2 entities are equal.");
	//		return 0;
	//	}
	//	if (bodyComponentID == -1) {
	//		printDebugError("getEntityDistance() body component does not exist.");
	//		return 0;
	//	}
	//	BodyID* bodyID1 = (BodyID*)ecs.getEntityComponent(e1, bodyComponentID);
	//	BodyID* bodyID2 = (BodyID*)ecs.getEntityComponent(e2, bodyComponentID);
	//	if (bodyID1 == nullptr) {
	//		printDebugError("getEntityDistance()'s e1 entity does not have a bodyID component.");
	//		return 0;
	//	}
	//	if (bodyID2 == nullptr) {
	//		printDebugError("getEntityDistance()'s e2 entity does not have a bodyID component.");
	//		return 0;
	//	}
	//	auto p1 = physics.getPos(*bodyID1);
	//	auto p2 = physics.getPos(*bodyID2);
	//	return p1.getDistanceFromSquared(p2);
	//}
};

#include "SystemUtilities/EnemyAI.h"