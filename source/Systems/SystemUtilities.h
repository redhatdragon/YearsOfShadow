#pragma once
#include <string>
#include <iostream>
#include INC_UTILS_PATH(ENGINE_PATH, EntityObjectLoader.h)
#include INC_UTILS_PATH(ENGINE_PATH, FixedPoint.h)
#include INC_UTILS_PATH(ENGINE_PATH, FlatBuffer.h)

#include "SystemUtilities/Inventory.h"
#include "SystemUtilities/PlantData.h"
#include "SystemUtilities/ItemData.h"

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
		FixedPoint<> base;
		FixedPoint<> modifiers[DAMAGE_TYPES::MAX];
	};
	struct MoveToLocation {
		Vec2D<uint32_t> pos;
		Vec2D<uint32_t> siz;
		FixedPoint<> speed;
	};
	struct UnitAI {
		EntityID target = -1;
		EntityHandle targetHandle;
		uint8_t fireRate = 20, ticksTillCanFire = 20;
		MoveToLocation moveTo;
		inline bool hasValidTarget() const {
			if (target == -1) return false;
			return ecs.entityHandleValid(target, targetHandle);
		}
		inline void setTarget(EntityID entity) {
			target = entity;
			targetHandle = ecs.entityGetHandle(entity);
		}
		bool canFire() {
			return !ticksTillCanFire;
		}
		//returns true if can fire
		bool tickFireRate(uint8_t ticks = 1) {
			if (ticks >= ticksTillCanFire) {
				ticksTillCanFire = 0;
				return true;
			}
			ticksTillCanFire -= ticks;
			return false;
		}
		void resetFireTimer() {
			ticksTillCanFire = fireRate;
		}
	};

	//Related to physics.getUserData()
	bool isUserDataVoxel(BodyID bodyID) {
		if (physics.getUserData(bodyID) == -1)
			return true;
		return false;
	}
	//Related to physics.getUserData()
	void setUserDataVoxel(BodyID bodyID) {
		physics.setUserData((void*)-1);
	}

	EntityID spawnEntityAt(const std::string& entityPath, Vec2D<uint32_t> pos) {
		std::string dataDir = EE_getDirData();
		EntityObject entityObject = EntityObjectLoader::createEntityObjectFromFile(dataDir + entityPath);
		EntityID entity = ecs.getNewEntity();

		auto* size = entityObject.getComponent("size");
		if (size) {
			BodyID bodyID = physics.addBodyRect(pos.x, pos.y, size->getArray()[0], size->getArray()[1]);
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
	EntityID spawnEntityAtWithSize(const std::string& entityPath, Vec2D<uint32_t> pos, Vec2D<uint32_t> siz) {
		EntityID retValue = spawnEntityAt(entityPath, pos);
		ComponentID bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		if (ecs.entityHasComponent(retValue, bodyComponentID)) {
			std::cout << "Error: spawnEntityAtWithSize()'s entity from entityPath had a size declared in file " <<
				entityPath << std::endl;
			return -1;
		}
		BodyID bodyID = physics.addBodyRect(pos.x, pos.y, siz.x, siz.y);
		physics.setUserData(bodyID, (void*)retValue);
		ecs.emplace(retValue, bodyComponentID, &bodyID);
		return retValue;
	}

	inline uint64_t getEntityDistanceSquared(EntityID e1, EntityID e2) {
		ComponentID bodyComponentID = ecs.getComponentID("body");
		if (e1 == e2) {
			printDebugError("getEntityDistance()'s e1 and e2 entities are equal.");
			return 0;
		}
		if (bodyComponentID == -1) {
			printDebugError("getEntityDistance() body component does not exist.");
			return 0;
		}
		BodyID* bodyID1 = (BodyID*)ecs.getEntityComponent(e1, bodyComponentID);
		BodyID* bodyID2 = (BodyID*)ecs.getEntityComponent(e2, bodyComponentID);
		if (bodyID1 == nullptr) {
			printDebugError("getEntityDistance()'s e1 entity does not have a bodyID component.");
			return 0;
		}
		if (bodyID2 == nullptr) {
			printDebugError("getEntityDistance()'s e2 entity does not have a bodyID component.");
			return 0;
		}
		Vec2D<uint32_t> p1 = physics.getPos<uint32_t>(*bodyID1);
		Vec2D<uint32_t> p2 = physics.getPos<uint32_t>(*bodyID2);
		return p1.getDistanceFromSquared(p2);
	}
};

#include "SystemUtilities/EnemyAI.h"