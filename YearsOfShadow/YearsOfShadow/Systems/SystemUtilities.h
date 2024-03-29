#pragma once
#include <string>
#include <iostream>
// #include "INC_UTILS.h"
#include <HAL/HAL.h>
#include <EchoEngine/EntityObjectLoader.h>
#include <EchoEngine/FixedPoint.h>
#include <EchoEngine/Vec.h>
#include <EchoEngine/FlatBuffer.h>
#include <EchoEngine/Name.h>
#include <EchoEngine/EE_Types.h>

//#include INC_UTILS_PATH(ENGINE_PATH, PhysicsEngineAABB3D.h)
//#include INC_UTILS_PATH(ENGINE_PATH, PhysicsEngineAABB3D_MT.h)

//#include "SystemUtilities/Inventory.h"
//#include "SystemUtilities/PlantData.h"
//#include "SystemUtilities/ItemData.h"
#include "SystemUtilities/Codex.h"
#include "SystemUtilities/NetworkingTools.h"
#include "SystemUtilities/Serialize.h"

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
	//inline void setUserDataVoxel(BodyID bodyID) {
	//	physics.setUserData(bodyID, (void*)-1);
	//}

	inline EntityID spawnEntityAt(const std::string& entityPath, Vec3D<uint32_t> pos, bool isSolid) {
		std::string dataDir = HAL::get_dir_data();
		EntityObject entityObject = EntityObjectLoader::createEntityObjectFromFile(dataDir + entityPath);
		EntityID entity = ecs.getNewEntity();

		auto* size = entityObject.getComponent("size");
		if (size) {
			BodyID bodyID = physics.addBodyBox(pos.x, pos.y, pos.z, size->getArray()[0], size->getArray()[1], size->getArray()[2],
				reinterpret_cast<void*>(static_cast<size_t>(entity)), isSolid);
			ComponentID bodyComponentID = ecs.registerComponentAsBlittable("body", sizeof(BodyID));
			ecs.emplace(entity, bodyComponentID, &bodyID);
		}

		for (auto& c : entityObject.components) {
			if (c.name == "size") continue;
			if (c.type == ComponentObject::TYPE::TYPE_INT ||
				c.type == ComponentObject::TYPE::TYPE_FIXED_FLOAT ||
				c.type == ComponentObject::TYPE::TYPE_STRING) {
                ComponentID componentID = ecs.registerComponentAsBlittable(c.name, c.size);
                ecs.emplace(entity, componentID, &c.data);
			} else if (c.type == ComponentObject::TYPE::TYPE_DARRAY) {
                ComponentID componentID = ecs.registerComponentAsDArray(c.name, c.size);
                ecs.emplace(entity, componentID, &c.data);
			} else if (c.type == ComponentObject::TYPE::TYPE_ARRAY) {
                ComponentID componentID = ecs.registerComponentAsBlittable(c.name, c.size);
				ecs.emplace(entity, componentID, c.data);
			} else if (c.type == ComponentObject::TYPE::TYPE_NULL) {
                ComponentID componentID = ecs.registerComponentAsBlittable(c.name, c.size);
                ecs.emplace(entity, componentID, NULL);
			}
			else {
				std::cout << "error: spawnEntityAt()'s entity has component of invalid type..?  destroying entity" << std::endl;
				ecs.removeEntity(entity);
				return -1;
			}
		}
		return entity;
	}
	inline EntityID spawnEntityAtWithSize(const std::string& entityPath, Vec3D<uint32_t> pos, Vec3D<uint32_t> siz, bool isSolid) {
		EntityID retValue = spawnEntityAt(entityPath, pos, isSolid);
        ComponentID bodyComponentID = ecs.registerComponentAsBlittable("body", sizeof(BodyID));
		if (ecs.entityHasComponent(retValue, bodyComponentID)) {
			std::cout << "Error: spawnEntityAtWithSize()'s entity from entityPath had a size declared in file " <<
				entityPath << std::endl;
			return -1;
		}
		BodyID bodyID = physics.addBodyBox(pos.x, pos.y, pos.z, siz.x, siz.y, siz.z, 
			reinterpret_cast<void*>(static_cast<size_t>(retValue)), isSolid);
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