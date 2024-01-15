#pragma once
#include "../FPSCamera.h"
#include <iostream>
#include <random>
#include <optick.h>

inline std::mt19937& getRandGenerator() {
	static thread_local std::mt19937 generator;
	return generator;
}
int getRandInt(std::mt19937& generator, int min, int max) {
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
}

struct EnemyAI {
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
    GameTick lastPositionChoice;
	Vec3D<FixedPoint<256 * 256>> gotoPos;
	GameTick lastJumped;
	u8 jumpDuration;
	u8 jumpDistance;
	void init() {
		active_state = IDLE;
		visualDistance = 16;
		targetHandle = -1;
        targetEntity = -1;
        lastPositionChoice = 0;
	}
};

class SystemEnemy : public System {
	ComponentID bodyComponentID;
	ComponentID meshComponentID;
	ComponentID instancedMeshComponentID;
	ComponentID enemyAIComponentID;
	ComponentID controllerComponentID;
	ComponentID replicateEntityComponentID;
public:
	virtual void init() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();

		bodyComponentID = ecs.registerComponentAsBlittable("body", sizeof(BodyID));
		meshComponentID = ecs.registerComponentAsBlittable("mesh", sizeof(void*));
        instancedMeshComponentID = ecs.registerComponentAsBlittable("instancedMesh", sizeof(u32));
        enemyAIComponentID = ecs.registerComponentAsBlittable("enemyAI", sizeof(EnemyAI));
        controllerComponentID = ecs.registerComponentAsBlittable("controller", sizeof(Controller));
		replicateEntityComponentID = ecs.registerComponentAsBlittable("replicateEntity", sizeof(ReplicateEntity));

		std::mt19937& generator = getRandGenerator();
		#if GAME_TYPE != GAME_TYPE_CLIENT
		createTest(generator);
		#endif

		#if GAME_TYPE == GAME_TYPE_CLIENT
		u32 count = 0;
		u32 padding = 20, border = world_size;
		while (true) {
			Vec3D<u32> pos = {};
			pos.x = getRandInt(generator, 0, border - (padding + padding));
			pos.z = getRandInt(generator, 0, border - (padding + padding));
			pos += padding;
			pos.y = 153;
			static std::vector<BodyID> castBuff;
			if (physics.getBodiesInBoxRough(pos, { 6, 1, 6 }, castBuff))
				continue;
			createEnemy(pos);
			break;
		}
		#endif

		//u32 padding = 20, border = world_size;
		//while (true) {
		//	Vec3D<u32> pos = {};
		//	pos.x = getRandInt(generator, 0, border - (padding + padding));
		//	pos.z = getRandInt(generator, 0, border - (padding + padding));
		//	pos += padding;
		//	pos.y = 153;
		//	static std::vector<BodyID> castBuff;
		//	if (physics.getBodiesInRectRough(pos, { 6, 1, 6 }, castBuff))
		//		continue;
		//	createEnemy(pos);
		//	break;
		//}

	}
	virtual void run() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
		u32 enemyCount = ecs.getComponentCount(enemyAIComponentID);
		#ifdef THREADING_ENABLED
		const auto threadCount = HAL::get_thread_pool_free_thread_count(threadPool);
		if (enemyCount / 3 > threadCount && threadCount > 1)
			runMulti();
		else
			runSingle();
		#else
		runSingle();
		#endif
	}
	virtual const char* getName() {
		return "SystemEnemy";
	}
private:
	void runSingle() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
        static std::vector<BodyID> castBuff;
		std::mt19937& generator = getRandGenerator();
		u32 enemyCount = ecs.getComponentCount(enemyAIComponentID);
		if (enemyCount == 0)
			return;
		EnemyAI* enemyAIs = (EnemyAI*)ecs.getComponentBuffer(enemyAIComponentID);
		for (u32 i = 0; i < enemyCount; i++) {
			//EnemyAI* enemyAI = &enemyAIs[i];
			//EntityID owner = ecs.getOwner(enemyAIComponentID, i);
			//BodyID bodyID = *(BodyID*)ecs.getEntityComponent(owner, bodyComponentID);
			//findTarget(enemyAI, bodyID);
			//if (enemyAI->targetEntity == -1) {
			//	auto pos = physics.getPos(bodyID);
			//	if (enemyAI->gotoPos.isZero()
			//		|| (enemyAI->gotoPos - pos).getDistance() <= 2)
			//		findRandomTargetPos(enemyAI, bodyID, generator);
			//}
			//movement(enemyAI, bodyID, generator, castBuff);
		}
		ThreadData td;
		td.init(0, enemyCount - 1, this);
		runThreadedBody(&td);
	}
	struct ThreadData {
		u32 startingIndex, endingIndex;
		SystemEnemy* self;
		void init(u32 _startingIndex, u32 _endingIndex, SystemEnemy* _self) {
			startingIndex = _startingIndex;
			endingIndex = _endingIndex;
			self = _self;
		}
	};
	void runMulti() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
		const auto threadCount = HAL::get_thread_pool_free_thread_count(threadPool);
		u32 enemyCount = ecs.getComponentCount(enemyAIComponentID);
		u32 totalWork = enemyCount;
		const auto workPerThread = totalWork / threadCount;
		u32 leftover = totalWork % threadCount;
		static ThreadData tds[256];
		for (u32 i = 0; i < threadCount; i++) {
			u32 start = static_cast<u32>(workPerThread * i); u32 end = static_cast<u32>(start + workPerThread - 1);
            tds[i].init(start, end, this);
		}
		for (u32 i = 0; i < threadCount; i++) {
			HAL::submit_thread_pool_task(threadPool, runThreadedBody, (void*)&tds[i]);
		}
		if (leftover) {
			u32 start = static_cast<u32>(workPerThread * threadCount); u32 end = static_cast<u32>(start + leftover - 1);
            tds[threadCount].init(start, end, this);
            runThreadedBody(&tds[threadCount]);
		}
		while (HAL::is_thread_pool_finished(threadPool) == false)
			continue;
	}
	static void runThreadedBody(void* data) {
        std::ostringstream ss;
        ss << std::this_thread::get_id();
        OPTICK_THREAD(ss.str().c_str());
        OPTICK_EVENT();
		ThreadData* td = (ThreadData*)data;
		SystemEnemy* self = td->self;

		auto generator = getRandGenerator();

		thread_local std::vector<BodyID> castBuff;
		u32 enemyCount = ecs.getComponentCount(self->enemyAIComponentID);
		EnemyAI* enemyAIs = (EnemyAI*)ecs.getComponentBuffer(self->enemyAIComponentID);
        u32 start = td->startingIndex, end = td->endingIndex;
		for (u32 i = start; i <= end; i++) {
			EnemyAI* enemyAI = &enemyAIs[i];
			EntityID owner = ecs.getOwner(self->enemyAIComponentID, i);
			BodyID bodyID = *(BodyID*)ecs.getEntityComponent(owner, self->bodyComponentID);

            self->verifyTarget(enemyAI);
            if (enemyAI->targetEntity == -1 || getRandInt(generator, 0, 10) == 0)
				self->findTarget(enemyAI, bodyID);
			if (enemyAI->targetEntity == -1) {
				auto pos = physics.getPos(bodyID);
                //if (ecs.getTicksPassed() - enemyAI->lastPositionChoice >= 60
                if (getRandInt(generator, 0, 60) == 0
					|| enemyAI->gotoPos.isZero()
					|| (enemyAI->gotoPos - pos).getDistance() <= 2) {
					self->findRandomTargetPos(enemyAI, bodyID, generator);
                    //int rng = getRandInt(generator, 0, 60);
                    //rng -= 30;
                    //enemyAI->lastPositionChoice = ecs.getTicksPassed() + rng;
				}
			}
			self->movement(enemyAI, bodyID, generator, castBuff);
		}
	}

	void verifyTarget(EnemyAI* enemyAI) {
		if (enemyAI->targetEntity == -1
			|| ecs.entityHandleValid(enemyAI->targetEntity, enemyAI->targetHandle) == true) {
            enemyAI->targetEntity = -1;
            enemyAI->targetHandle = -1;
		}
	}
	void findTarget(EnemyAI* enemyAI, BodyID bodyID) {
		u32 controllerCount = ecs.getComponentCount(controllerComponentID);
		EntityID closestEntity = -1;
		Vec3D<FixedPoint<256 * 256>> closestPos;
		FixedPoint<256 * 256> closestDist;
		u32 visualDistance = enemyAI->visualDistance;
		Vec3D<FixedPoint<256 * 256>> pos = physics.getPos(bodyID);
		for (u32 i = 0; i < controllerCount; i++) {
			EntityID otherEntityID = ecs.getOwner(controllerComponentID, i);
			if (ecs.entityHasComponent(otherEntityID, bodyComponentID) == false) {
				HAL_WARN("Error: SystemEnemyAI::perception(), an entity with a controller component lacks a body component")
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
					//NOTE: this shouldn't logically ever be equal.
					HAL_ASSERT_REL(otherPos.isEqual(closestPos) == false, "otherPos SOMHOW is == closestPos!?")
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
		u32 padding = 20, border = world_size;
		Vec3D<FixedPoint<256 * 256>> gotoPos = { 
			getRandInt(generator, 0, border - (padding + padding)),
			0,
			getRandInt(generator, 0, border - (padding + padding))
		};
		gotoPos += padding;
		auto pos = physics.getPos(bodyID);
		gotoPos.y = pos.y;
		enemyAI->gotoPos = gotoPos;
        enemyAI->targetEntity = -1;
        enemyAI->targetHandle = -1;
	}
    void movement(EnemyAI *enemyAI, BodyID bodyID, std::mt19937 &generator, std::vector<BodyID>& castBuff)
    {
		if (enemyAI->gotoPos.isZero())
			return;

		auto pos = physics.getPos(bodyID);

		int rng = getRandInt(generator, 0, 10);
		if (rng == 0) {
			Vec3D<FixedPoint<256 * 256>> avoidanceDir = getAvoidanceDir(enemyAI, bodyID, castBuff);
			if (avoidanceDir.isZero() == false) {
				auto gotoPos = pos + avoidanceDir * 3;
				Vec3D<FixedPoint<256 * 256>> min = { 0 + 20, 20, 0 + 20 },
					max = {world_size - 20, 256-20, world_size - 20};
				if (gotoPos.isBetween(min, max))
					enemyAI->gotoPos = gotoPos;
			}
		}

        auto vel = enemyAI->gotoPos;

		vel -= pos;
		//vel *= 30;
		vel.normalize();
		vel /= 30;
		vel.y = 0;
		auto pointPos = pos;
		auto siz = physics.getSize(bodyID);
		pointPos += siz / 2;
		pointPos.y += siz.y / 2;
		pointPos.y += "0.1f";
		if (physics.pointTrace(pointPos, bodyID, castBuff) == false) {
			vel.y = physics.getVelocity(bodyID).y;
		}
		physics.setVelocity(bodyID, vel.x, vel.y, vel.z);
	}



	inline Vec3D<FixedPoint<256 * 256>> getAvoidanceDir(EnemyAI* enemyAI, BodyID bodyID, std::vector<BodyID>& castBuff) {
		Vec3D<FixedPoint<256 * 256>> retValue = {};
		Vec3D<FixedPoint<256 * 256>> pos = physics.getPos(bodyID);
		Vec3D<FixedPoint<256 * 256>> offset = {1, 0, 1};
		Vec3D<FixedPoint<256 * 256>> siz = { 2, 1, 2 };
		std::vector<BodyID>& bodies = castBuff;
		physics.getDynamicBodiesInBoxRough(pos-offset, siz, bodies);
		thread_local std::vector<BodyID> bodiesWithEntities;
        bodiesWithEntities.clear();
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
		//Can't be 1.0f or larger on any axis, 
		//without harming runtime via broad phase physics putting this in more cells.
		BodyID bodyID = physics.addBodyBox(pos.x, pos.y, pos.z, "0.25f", "0.8f", "0.25f",
			to_void_ptr(entityID), true);
		ecs.emplace(entityID, bodyComponentID, &bodyID);

		u32 instanceMeshTypeID = instancedMeshCodex.add("./Data/Meshes/Props/Dynamite.obj");
		instancedMeshCodex.setTexture(instanceMeshTypeID, "./Data/Meshes/Props/D_Dynamite.png");
		ecs.emplace(entityID, instancedMeshComponentID, &instanceMeshTypeID);

		ReplicateEntity re;
		re.init(entityID, ecs.entityGetHandle(entityID));
		ecs.emplace(entityID, replicateEntityComponentID, &re);


		EnemyAI enemyAI;
		enemyAI.init();
		ecs.emplace(entityID, enemyAIComponentID, &enemyAI);
	}
	void createTest(std::mt19937& generator) {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
		u32 count = 0;
		u32 padding = 20, border = world_size;
		while (true) {
			Vec3D<u32> pos = {};
			pos.x = getRandInt(generator, 0, border-(padding+padding));
			pos.z = getRandInt(generator, 0, border-(padding+padding));
			pos += padding;
			pos.y = 153;
            static std::vector<BodyID> castBuff;
			if (physics.getBodiesInBoxRough(pos, { 6, 1, 6 }, castBuff))
				continue;
			createEnemy(pos);
			count++;
			if (count == max_npc)
				break;
		}
	}
};