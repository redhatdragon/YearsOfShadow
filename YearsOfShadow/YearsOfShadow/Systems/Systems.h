#pragma once
#include <thread>

#define ENGINE_PATH ../EchoEngine/

#include <EchoEngine/HAL/HAL_3D.h>

void* threadPool;
//#define REWIND_ENABLED
#define THREADING_ENABLED
#include <EchoEngine/PhysicsEngineAABB3D_MT.h>
#include <EchoEngine/DDECS.h>

//#include <memory>
//#include <stddef>

constexpr uint32_t chunk_width = 16, chunk_depth = 16, chunk_height = 256;
constexpr uint32_t world_size = 400 - (400 % chunk_width);  //block across
constexpr uint32_t max_npc = 10000;
//constexpr uint32_t hash_width = 2;
//constexpr uint32_t max_bodies_per_hash = 16;
constexpr uint32_t hash_width = 1;
constexpr uint32_t max_bodies_per_hash = 50;


typedef PhysicsEngineAABB3D <
	((world_size) / hash_width) + 2,
	(chunk_height / hash_width) + 2,
	((world_size) / hash_width) + 2,
	hash_width,
	max_bodies_per_hash
> PHYSICS_DEF;

PHYSICS_DEF physics;

constexpr uint64_t sizeofPhysics = sizeof(PHYSICS_DEF);

#include "SystemUtilities/Voxel.h"

typedef VoxelWorld<world_size / chunk_width> VOXEL_WORLD_DEF;

VOXEL_WORLD_DEF voxelWorld;

constexpr uint64_t sizeofVoxelWorld = sizeof(VOXEL_WORLD_DEF);

DDECS<32, 20000> ecs = DDECS<32, 20000>();
constexpr uint64_t sizeOfECS = sizeof(ecs);

#include "SystemUtilities.h"

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
void instancedMeshDestrutor(ComponentID id, u32 index) {
	u32* instancedMeshBuffer = (u32*)ecs.getComponentBuffer(id);
	u32 instancedMesh = instancedMeshBuffer[index];
	instancedMeshCodex.release(instancedMesh);
}
void registerDestructors() {
	ComponentID meshComponentID = ecs.registerComponent("mesh", sizeof(void*));
	ecs.registerDestructor(meshComponentID, meshDestructor);
	ComponentID instancedMeshComponentID = ecs.registerComponent("instancedMesh", sizeof(u32));
	ecs.registerDestructor(instancedMeshComponentID, instancedMeshDestrutor);
}

inline void initSystems() {
	ecs.init();

	u16 threadCount = EE_getHardwareThreadCount();
	threadCount = std::thread::hardware_concurrency() / 4;
	std::cout << "ThreadPool thread count: " << threadCount << std::endl;

	threadPool = EE_getNewThreadPool(threadCount);

	instancedMeshCodex.init();

	setupFixedPointTableFiles();

	registerDestructors();

	ecs.registerSystem<SystemDeath>();
	ecs.registerSystem<SystemPhysics>();
	ecs.registerSystem<SystemController>();
	ecs.registerSystem<SystemVoxel>();
	//ecs.registerSystem<SystemExplosionTest>();
	ecs.registerSystem<SystemEnemy>();
	ecs.registerSystem<SystemExplode>();

	ecs.registerSystem<SystemDisplay>();
}

#include "Systems_impl.h"