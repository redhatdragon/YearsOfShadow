#pragma once
#include INC_UTILS_PATH(ENGINE_PATH, DDECS.h)
#include INC_UTILS_PATH(ENGINE_PATH, PhysicsEngineAABB3D.h)
#include INC_UTILS_PATH(ENGINE_PATH, EE_Types.h)
//#include "../FP_FPSCamera.h"  //Postoned till fixed
#include "../FPSCamera.h"
#include <iostream>

struct EnemyAI {
	//Controller controller;  //Might not be needed
	enum ACTIVE_STATE : u8 {
		ASLEEP,
		IDLE,
		WALKING,
		RUNNING,
		FERAL
	} active_state;
	enum JUMP_STATE : u8 {
		IS_JUMPING,
		IS_FALLING,
		IS_FLOORED
	} jump_state;
	u8 visualDistance;
	u8 auditorySensitivity;
	EntityHandle targetHandle;
	EntityID targetEntity;
	Vec3D<FixedPoint<256*256>> expectedTargetPosition;
	GameTick lastJumped;
	u8 jumpDuration;
	u8 jumpDistance;
	void init() {
		active_state = IDLE;
		visualDistance = 16;
		targetHandle = -1;
	}
};

class SystemEnemy : public System {
	ComponentID bodyComponentID;
	ComponentID meshComponentID;
	ComponentID enemyAIComponentID;
	ComponentID controllerComponentID;
public:
	virtual void init() {
		bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		meshComponentID = ecs.registerComponent("mesh", sizeof(void*));
		enemyAIComponentID = ecs.registerComponent("enemyAI", sizeof(EnemyAI));
		controllerComponentID = ecs.registerComponent("controller", sizeof(Controller));

		createTest();
	}
	virtual void run() {
		u32 enemyCount = ecs.getComponentCount(enemyAIComponentID);
		EnemyAI* enemyAIs = (EnemyAI*)ecs.getComponentBuffer(enemyAIComponentID);
		for (u32 i = 0; i < enemyCount; i++) {
			EnemyAI* enemyAI = &enemyAIs[i];
			EntityID owner = ecs.getOwner(enemyAIComponentID, i);
			BodyID bodyID = *(BodyID*)ecs.getEntityComponent(owner, bodyComponentID);
			perception(enemyAI, bodyID);
			movement(enemyAI, bodyID);
		}
	}
	virtual const char* getName() {
		return "SystemController";
	}
private:
	void perception(EnemyAI* enemyAI, BodyID bodyID) {
		int stopEarly = rand() % 10;
		if (stopEarly)
			return;
		u32 controllerCount = ecs.getComponentCount(controllerComponentID);
		EntityID closestEntity = -1;
		Vec3D<FixedPoint<256 * 256>> closestPos;
		FixedPoint<256 * 256> closestDist;
		u32 visualDistance = enemyAI->visualDistance;
		Vec3D<FixedPoint<256 * 256>> pos = physics.getPos(bodyID);
		for (u32 i = 0; i < controllerCount; i++) {
			EntityID otherEntityID = ecs.getOwner(controllerComponentID, i);
			if (ecs.entityHasComponent(otherEntityID, bodyComponentID) == false) {
				std::cout << "Error: SystemEnemyAI::perception(), an entity with a controller component lacks a body component" << std::endl;
				continue;
			}
			BodyID otherBodyID = *(BodyID*)ecs.getEntityComponent(otherEntityID, bodyComponentID);
			Vec3D<FixedPoint<256 * 256>> otherPos = physics.getPos(otherBodyID);
			auto distFrom = pos.getDistanceFrom(otherPos);
			if (distFrom <= visualDistance) {
				if (closestEntity == -1) {
					goto setNewTarget;
				}
				if (distFrom == closestDist) {
					if (otherPos < closestPos) {
						goto setNewTarget;
					}
					if (otherPos == closestPos) {  //NOTE: this shouldn't logically ever happen.
						std::cout << "Error: SystemEnemyAI::perception() otherPos SOMHOW is == closestPos!?" << std::endl;
						throw;
					}
				}
				if (distFrom < closestDist) {
					goto setNewTarget;
				}
				continue;
				setNewTarget:
				closestEntity = otherEntityID;
				closestDist = distFrom;
				closestPos = otherPos;
			}
			if (closestEntity == -1) {
				enemyAI->targetHandle = -1;
				enemyAI->targetEntity = -1;
				return;
			}
			enemyAI->targetHandle = ecs.entityGetHandle(closestEntity);
			enemyAI->targetEntity = closestEntity;
			enemyAI->expectedTargetPosition = closestPos;
		}
	}
	void movement(EnemyAI* enemyAI, BodyID bodyID) {
		if (enemyAI->targetHandle == -1)
			return;
		if (ecs.entityHandleValid(enemyAI->targetEntity, enemyAI->targetHandle) == false) {
			enemyAI->targetEntity = enemyAI->targetHandle = -1;
			return;
		}
		auto targetPos = enemyAI->expectedTargetPosition;
		auto vel = targetPos;
		vel -= physics.getPos(bodyID);
		vel *= 10;
		vel.normalize();
		vel /= 30;
		vel.y = 0;
		physics.setVelocity(bodyID, vel.x, vel.y, vel.z);
	}

	void createEnemy(Vec3D<uint32_t> pos) {
		BodyID bodyID = physics.addBodyBox(pos.x, pos.y, pos.z, "0.2f", "0.8f", "0.2f");
		physics.setSolid(bodyID, true);
		EntityID entityID = ecs.getNewEntity();
		physics.setUserData(bodyID, (void*)entityID);
		ecs.emplace(entityID, bodyComponentID, &bodyID);
		void* mesh = EE_getNewMesh("Meshes/Cube2.fbx/cube.001.mesh");
		EE_setScaleMesh(mesh, 50*0.2f, 50*0.8f, 50*0.2f);
		ecs.emplace(entityID, meshComponentID, &mesh);
		EnemyAI enemyAI;
		enemyAI.init();
		ecs.emplace(entityID, enemyAIComponentID, &enemyAI);
	}
	void createTest() {
		uint32_t count = 0;
		while (true) {
			Vec3D<uint32_t> pos;
			pos.x = rand() % 120;
			pos.z = rand() % 120;
			pos += 40;
			pos.y = 155;
			if (physics.getBodiesInRectRough(pos, { 1, 1, 1 }).size())
				continue;
			createEnemy(pos);
			count++;
			if (count == 100)
				break;
		}
	}
};