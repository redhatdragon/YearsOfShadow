#pragma once
#include INC_UTILS_PATH(ENGINE_PATH, DDECS.h)
#include INC_UTILS_PATH(ENGINE_PATH, PhysicsEngineAABB3D.h)
#include <iostream>

struct Explode {
	uint32_t timer;
	uint16_t area;
};

class SystemExplosionTest : public System {
	ComponentID explodeComponentID;
	ComponentID bodyComponentID;
	ComponentID meshComponentID;
	ComponentID deadComponentID;
	static constexpr Vec3D<uint32_t> siz = { 1, 1, 1 };
public:
	virtual void init() {
		explodeComponentID = ecs.registerComponent("explode", sizeof(Explode));
		bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		meshComponentID = ecs.registerComponent("mesh", sizeof(void*));
		deadComponentID = ecs.registerComponent("dead", NULL);

		constexpr uint32_t maxExplodiesSqrt = 10*1;
		for (uint32_t i = 0; i < maxExplodiesSqrt; i++) {
			for (uint32_t j = 0; j < maxExplodiesSqrt; j++) {
				spawnRandom();
			}
		}
	}
	virtual void run() {
		for(uint32_t i = 0; i < 1 * 4; i++)
			spawnRandom();

		register uint32_t explodeCount = ecs.getComponentCount(explodeComponentID);
		Explode* explodeComponentBuffer = (Explode*)ecs.getComponentBuffer(explodeComponentID);
		for (uint32_t i = 0; i < explodeCount; i++) {
			Explode& explode = explodeComponentBuffer[i];
			explode.timer--;
			if (explode.timer == 0) {
				EntityID owner = ecs.getOwner(explodeComponentID, i);
				if (ecs.entityHasComponent(owner, deadComponentID) == false)
					ecs.emplace(owner, deadComponentID, NULL);
				BodyID bodyID = *(BodyID*)ecs.getEntityComponent(owner, bodyComponentID);
				Vec3D<uint32_t> rootPos = physics.getPos<uint32_t>(bodyID);
				Vec3D<uint32_t> totalSiz = physics.getSize<uint32_t>(bodyID);
				totalSiz *= 12;
				rootPos -= (totalSiz / 2);
				//for (uint32_t z = rootPos.z; z < totalSiz.z; z++) {
				//	for (uint32_t y = rootPos.y; y < totalSiz.y; y++) {
				//		for (uint32_t x = rootPos.x; x < totalSiz.x; x++) {
				//				
				//		}
				//	}
				//}
				auto solidBlockPositions = voxelWorld.getValidSolidBlockPositions(rootPos, totalSiz);
				for (uint32_t j = 0; j < solidBlockPositions.size(); j++) {
					auto& pos = solidBlockPositions[j];
					voxelWorld.destroyBlock(pos.x, pos.y, pos.z);
				}
				std::vector<BodyID> bodies = physics.getBodiesInRectRough(rootPos, totalSiz);
				uint32_t bodyCount = bodies.size();
				for (uint32_t j = 0; j < bodyCount; j++) {
					BodyID nextBody = bodies[j];
					void* userData = physics.getUserData(nextBody);
					if (userData == (void*)-1)
						continue;
					EntityID nextOwner = (EntityID)userData;
					if (ecs.entityHasComponent(nextOwner, deadComponentID) == true)
						continue;
					ecs.emplace(nextOwner, deadComponentID, NULL);
				}
			}
		}
	}
	virtual const char* getName() {
		return "SystemExplosionTest";
	}
private:
	void spawnBomb(Vec3D<uint32_t> pos) {
		EntityID entity = ecs.getNewEntity();
		Explode explode = { 60 * 3, 20 };
		BodyID bodyID = physics.addBodyBox(pos.x, pos.y, pos.z, siz.x, siz.y, siz.z);
		physics.setSolid(bodyID, true);
		physics.setUserData(bodyID, (void*)entity);
		physics.setVelocity(bodyID, 0, "1.5f", 0);
		void* mesh = EE_getNewMesh("Meshes/Cube2.fbx/cube.001.mesh");
		EE_setPositionMesh(mesh, pos.x, pos.y, pos.z);
		EE_setScaleMesh(mesh, 100, 100, 100);
		ecs.emplace(entity, meshComponentID, &mesh);
		ecs.emplace(entity, bodyComponentID, &bodyID);
		ecs.emplace(entity, explodeComponentID, &explode);
	}
	void spawnRandom() {
		std::vector<BodyID> overlappingBodies = {};

		redo:
		overlappingBodies.clear();
		Vec3D<uint32_t> pos = { rand() % (70 * 2), 40, rand() % (70 * 2) };
		pos.x += 30; pos.z += 30;
		overlappingBodies = physics.getBodiesInRectRough(pos, siz);
		if (overlappingBodies.size())
			goto redo;
		spawnBomb(pos);
	}
};