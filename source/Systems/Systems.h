#pragma once
#include "INC_UTILS.h"

#define ENGINE_PATH ../EchoEngine/

#include "EchoEngine/HAL/HAL_3D.h"
#include "EchoEngine/PhysicsEngineAABB3D.h"
#include "../EchoEngine/DDECS.h"
//#include <memory>
//#include <stddef>

constexpr uint32_t chunk_width = 16, chunk_depth = 16, chunk_height = 256;
constexpr uint32_t world_size = 500 - (500 % chunk_width);  //block across
//constexpr uint32_t hash_width = 2;
//constexpr uint32_t max_bodies_per_hash = 16;
constexpr uint32_t hash_width = 1;
constexpr uint32_t max_bodies_per_hash = 10;

typedef PhysicsEngineAABB3D <
	((world_size) / hash_width) + 2,
	(chunk_height / hash_width) + 2,
	((world_size) / hash_width) + 2,
	hash_width,
	max_bodies_per_hash
> PHYSICS_DEF;

PHYSICS_DEF physics;

constexpr uint64_t sizeofPhysics = sizeof(PHYSICS_DEF);

#include "Voxel.h"

typedef VoxelWorld<world_size / chunk_width> VOXEL_WORLD_DEF;

VOXEL_WORLD_DEF voxelWorld;

constexpr uint64_t sizeofVoxelWorld = sizeof(VOXEL_WORLD_DEF);

DDECS<64, 500000> ecs = DDECS<64, 500000>();
constexpr uint64_t sizeOfECS = sizeof(ecs);

#include "SystemDeath.h"
#include "SystemPhysics.h"
#include "SystemVoxel.h"
//#include "SystemExplosionTest.h"
#include "SystemController.h"
#include "SystemExplode.h"
#include "SystemEnemy.h"

#include "SystemDisplay.h"

#include "SystemUtilities/Serialize.h"

void meshDestructor(ComponentID id, uint32_t index) {
	void** meshBuffer = (void**)ecs.getComponentBuffer(id);
	void* mesh = meshBuffer[index];
	EE_releaseMesh(mesh);
}

#include <UniginePlayers.h>

void initSystems() {
	//initFixedPointUtilities();
	setupFixedPointTableFiles();

	ComponentID meshComponentID = ecs.registerComponent("mesh", sizeof(void*));
	ecs.registerDestructor(meshComponentID, meshDestructor);

	ecs.registerSystem<SystemDeath>();
	ecs.registerSystem<SystemPhysics>();
	ecs.registerSystem<SystemVoxel>();
	//ecs.registerSystem<SystemExplosionTest>();
	ecs.registerSystem<SystemController>();
	ecs.registerSystem<SystemEnemy>();
	ecs.registerSystem<SystemExplode>();

	ecs.registerSystem<SystemDisplay>();
}