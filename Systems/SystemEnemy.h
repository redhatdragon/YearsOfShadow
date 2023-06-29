#pragma once
#include "../FPSCamera.h"
#include <iostream>
#include <random>

inline std::mt19937& getRandGenerator() {
	static thread_local std::mt19937 generator;
	return generator;
}
int getRandInt(std::mt19937& generator, int min, int max) {
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
}

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
	Vec3D<FixedPoint<256 * 256>> gotoPos;
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
	ComponentID instancedMeshComponentID;
	ComponentID enemyAIComponentID;
	ComponentID controllerComponentID;

	int activeThreadCount;
public:
	virtual void init() {
		bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		meshComponentID = ecs.registerComponent("mesh", sizeof(void*));
		instancedMeshComponentID = ecs.registerComponent("instancedMesh", sizeof(u32));
		enemyAIComponentID = ecs.registerComponent("enemyAI", sizeof(EnemyAI));
		controllerComponentID = ecs.registerComponent("controller", sizeof(Controller));

		std::mt19937& generator = getRandGenerator();
		createTest(generator);
		activeThreadCount = 0;
	}
	virtual void run() {
		u32 enemyCount = ecs.getComponentCount(enemyAIComponentID);
		#ifdef THREADING_ENABLED
		u32 threadCount = EE_getThreadPoolFreeThreadCount(threadPool);
		if (threadCount < enemyCount + 10 && threadCount > 1) {
			runMulti();
			return;
		}
		#endif
		runSingle();
	}
	virtual const char* getName() {
		return "SystemEnemy";
	}
private:
	void runSingle() {
		std::mt19937& generator = getRandGenerator();
		u32 enemyCount = ecs.getComponentCount(enemyAIComponentID);
		EnemyAI* enemyAIs = (EnemyAI*)ecs.getComponentBuffer(enemyAIComponentID);
		for (u32 i = 0; i < enemyCount; i++) {
			EnemyAI* enemyAI = &enemyAIs[i];
			EntityID owner = ecs.getOwner(enemyAIComponentID, i);
			BodyID bodyID = *(BodyID*)ecs.getEntityComponent(owner, bodyComponentID);
			findTarget(enemyAI, bodyID, generator);
			if (enemyAI->targetEntity == -1) {
				auto pos = physics.getPos(bodyID);
				if (enemyAI->gotoPos.isZero()
					|| (enemyAI->gotoPos - pos).getDistance() <= 2)
					findRandomTargetPos(enemyAI, bodyID, generator);
			}
			movement(enemyAI, bodyID, generator);
		}
	}
	struct ThreadData {
		u32 startingIndex, endingIndex;
		SystemEnemy* self;

	};
	void runMulti() {
		u16 threadCount = EE_getThreadPoolFreeThreadCount(threadPool);
		u32 enemyCount = ecs.getComponentCount(enemyAIComponentID);
		u32 totalWork = enemyCount;
		u32 workPerThread = totalWork / threadCount;
		u32 leftover = totalWork % threadCount;
		static std::vector<ThreadData> tds;
		tds.resize(threadCount);
		for (u32 i = 0; i < threadCount; i++) {
			u32 start = workPerThread * i; u32 end = start + workPerThread - 1;
			ThreadData td = { start, end, this };
			//std::cout << start << ' ' << end << std::endl;
			tds[i] = td;
		}
		for (u32 i = 0; i < threadCount; i++) {
			EE_sendThreadPoolTask(threadPool, runThreadedBody, (void*)&tds[i]);
		}
		//NOTE: Needs to happen after all other threads finished, reasons beyond me
		if (leftover) {
			u32 start = workPerThread * threadCount; u32 end = start + leftover - 1;
			ThreadData td = { start, end, this };
			//std::cout << start << ' ' << end << std::endl;
			runThreadedBody(&td);
		}
		while (EE_isThreadPoolFinished(threadPool) == false)
			continue;
	}
	static void runThreadedBody(void* data) {
		ThreadData* td = (ThreadData*)data;
		SystemEnemy* self = td->self;

		auto generator = getRandGenerator();

		u32 enemyCount = ecs.getComponentCount(self->enemyAIComponentID);
		EnemyAI* enemyAIs = (EnemyAI*)ecs.getComponentBuffer(self->enemyAIComponentID);
		for (u32 i = td->startingIndex; i <= td->endingIndex; i++) {
			EnemyAI* enemyAI = &enemyAIs[i];
			EntityID owner = ecs.getOwner(self->enemyAIComponentID, i);
			BodyID bodyID = *(BodyID*)ecs.getEntityComponent(owner, self->bodyComponentID);
			self->findTarget(enemyAI, bodyID, generator);
			if (enemyAI->targetEntity == -1) {
				auto pos = physics.getPos(bodyID);
				if (enemyAI->gotoPos.isZero()
					|| (enemyAI->gotoPos - pos).getDistance() <= 2)
					self->findRandomTargetPos(enemyAI, bodyID, generator);
			}
			self->movement(enemyAI, bodyID, generator);
		}
	}

	void findTarget(EnemyAI* enemyAI, BodyID bodyID, std::mt19937& generator) {
		//int stopEarly = rand() % 10;
		//int stopEarly = getRandInt(generator, 0, 10);
		//if (stopEarly)
		//	return;
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
			enemyAI->gotoPos = closestPos;
		}
		if (enemyAI->targetHandle == -1)
			return;
		if (ecs.entityHandleValid(enemyAI->targetEntity, enemyAI->targetHandle) == false) {
			enemyAI->targetEntity = -1; enemyAI->targetHandle = -1;
		}
	}
	void findRandomTargetPos(EnemyAI* enemyAI, BodyID bodyID, std::mt19937& generator) {
		u32 padding = 40, border = world_size;
		Vec3D<FixedPoint<256 * 256>> gotoPos = { 
			//rand() % (border - (padding + padding)),
			getRandInt(generator, 0, border - (padding + padding)),
			0,
			//rand() % (border - (padding + padding))
			getRandInt(generator, 0, border - (padding + padding))
		};
		//gotoPos += 5;
		gotoPos += padding;
		//gotoPos.y = 0;
		auto pos = physics.getPos(bodyID);
		gotoPos.y = pos.y;
		enemyAI->gotoPos = gotoPos;
	}
	void movement(EnemyAI* enemyAI, BodyID bodyID, std::mt19937& generator) {
		if (enemyAI->gotoPos.isZero())
			return;

		auto targetPos = enemyAI->gotoPos;
		auto vel = targetPos;
		auto pos = physics.getPos(bodyID);

		//int rng = rand() % 10;
		int rng = getRandInt(generator, 0, 10);
		if (rng == 0) {
			Vec3D<FixedPoint<256 * 256>> avoidanceDir = getAvoidanceDir(enemyAI, bodyID);
			if (avoidanceDir.isZero() == false) {
				auto gotoPos = pos + avoidanceDir * 3;
				Vec3D<FixedPoint<256 * 256>> min = { 0 + 20, 0 + 20 }, max = {world_size - 20, world_size - 20};
				if (gotoPos.isBetween(min, max))
					enemyAI->gotoPos = gotoPos;
			}
		}

		vel -= pos;
		vel *= 10;
		vel.normalize();
		vel /= 30;
		vel.y = 0;
		auto pointPos = pos;
		auto siz = physics.getSize(bodyID);
		pointPos += siz / 2;
		pointPos.y += siz.y / 2;
		//pointPos.y += "0.01f";
		pointPos.y += "0.1f";
		//if (physics.getBodiesInPoint(pointPos, bodyID).count == 0) {
		if (physics.pointTrace(pointPos, bodyID) == false) {
			vel.y = physics.getVelocity(bodyID).y;
		}
		physics.setVelocity(bodyID, vel.x, vel.y, vel.z);
	}



	inline Vec3D<FixedPoint<256 * 256>> getAvoidanceDir(EnemyAI* enemyAI, BodyID bodyID) {
		Vec3D<FixedPoint<256 * 256>> retValue = {};
		Vec3D<FixedPoint<256 * 256>> pos = physics.getPos(bodyID);
		Vec3D<FixedPoint<256 * 256>> offset = {1, 0, 1};
		Vec3D<FixedPoint<256 * 256>> siz = { 2, 1, 2 };
		std::vector<BodyID> bodies = physics.getBodiesInRectRough(pos-offset, siz);
		std::vector<BodyID> bodiesWithEntities;
		bodiesWithEntities.reserve(bodies.size());
		u32 count = (u32)bodies.size();
		for (u32 i = 0; i < count; i++) {
			if (physics.getUserData(bodies[i]) == (void*)-1)
				continue;
			bodiesWithEntities.push_back(bodies[i]);
		}
		count = (u32)bodiesWithEntities.size();
		if (count == 0) {
			return {};
		}
		Vec3D<FixedPoint<256 * 256>> pressureDirection = {};
		for (u32 i = 0; i < count; i++) {
			Vec3D<FixedPoint<256 * 256>> otherPos = physics.getPos(bodiesWithEntities[i]);
			otherPos = otherPos-pos;
			otherPos.normalize();
			pressureDirection += otherPos;
		}
		pressureDirection.normalize();
		return {-pressureDirection.x, 0, -pressureDirection.y};
	}



	void createEnemy(Vec3D<uint32_t> pos) {
		EntityID entityID = ecs.getNewEntity();
		//BodyID bodyID = physics.addBodyBox(pos.x, pos.y, pos.z, "0.2f", "0.8f", "0.2f",
		//BodyID bodyID = physics.addBodyBox(pos.x, pos.y, pos.z, "0.5f", "1.0f", "0.5f",
		BodyID bodyID = physics.addBodyBox(pos.x, pos.y, pos.z, "0.1f", "1.0f", "0.1f",
			(void*)entityID, true);
		ecs.emplace(entityID, bodyComponentID, &bodyID);
		//std::string path = "Meshes/Cube2.fbx/cube.001.mesh";
		//std::string path = "./Data/Meshes/Props/Dynamite.obj";
		//void* mesh = EE_getNewMesh(path.c_str());
		//EE_setTextureSubmesh(mesh, 0, "diffuse", "./Data/Meshes/Props/D_Dynamite.png");
		//ecs.emplace(entityID, meshComponentID, &mesh);

		//u32 instanceMeshTypeID = instancedMeshCodex.add("Meshes/Cube2.fbx/cube.001.mesh");
		//instancedMeshCodex.setSize(instanceMeshTypeID, {50*0.2f, 50*0.8f, 50*0.2f});
		//ecs.emplace(entityID, instancedMeshComponentID, &instanceMeshTypeID);

		u32 instanceMeshTypeID = instancedMeshCodex.add("./Data/Meshes/Props/Dynamite.obj");
		instancedMeshCodex.setTexture(instanceMeshTypeID, "./Data/Meshes/Props/D_Dynamite.png");
		ecs.emplace(entityID, instancedMeshComponentID, &instanceMeshTypeID);


		EnemyAI enemyAI;
		enemyAI.init();
		ecs.emplace(entityID, enemyAIComponentID, &enemyAI);
	}
	void createTest(std::mt19937& generator) {
		u32 count = 0;
		u32 padding = 40, border = world_size;
		while (true) {
			Vec3D<u32> pos = {};
			//pos.x = rand() % (border-(padding+padding));
			pos.x = getRandInt(generator, 0, border-(padding+padding));
			//pos.z = rand() % (border-(padding+padding));
			pos.z = getRandInt(generator, 0, border-(padding+padding));
			pos += padding;
			pos.y = 153;
			if (physics.getBodiesInRectRough(pos, { 4, 1, 4 }).size())
				continue;
			createEnemy(pos);
			count++;
			if (count == max_npc)
				break;
		}
	}
};