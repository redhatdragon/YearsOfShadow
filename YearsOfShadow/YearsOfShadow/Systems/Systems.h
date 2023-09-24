#pragma once
#include <thread>
#include <HAL/HAL.h>

HAL::thread_pool_handle_t threadPool;

#define REWIND_ENABLED
#define THREADING_ENABLED
#define PROFILER_ENABLED
#include <EchoEngine/DDECS.h>

//#include <memory>
//#include <stddef>

constexpr uint32_t chunk_width = 16, chunk_depth = 16, chunk_height = 256;
constexpr uint32_t world_size = 480 - (480 % chunk_width);  //block across
constexpr uint32_t max_npc = 10000;
//constexpr uint32_t hash_width = 2;
//constexpr uint32_t max_bodies_per_hash = 16;
constexpr uint32_t hash_width = 1;
constexpr uint32_t max_bodies_per_hash = 24;


#define physics_width ((world_size) / hash_width) + 2
#define physics_height (chunk_height / hash_width) + 2
#define physics_depth ((world_size) / hash_width) + 2

#define physics_hash_width hash_width
#define physics_max_bodies_per_hash max_bodies_per_hash
#define physics_max_bodies_per_collision 64

#include <EchoEngine/PhysicsEngineAABB3D.h>

//typedef PhysicsEngineAABB3D <
//	((world_size) / hash_width) + 2,
//	(chunk_height / hash_width) + 2,
//	((world_size) / hash_width) + 2,
//	hash_width,
//	max_bodies_per_hash
//> PHYSICS_DEF;
typedef PhysicsEngineAABB3D PHYSICS_DEF;

PHYSICS_DEF physics;

constexpr uint64_t sizeofPhysics = sizeof(PHYSICS_DEF);

#include "SystemUtilities/Voxel.h"

typedef VoxelWorld VOXEL_WORLD_DEF;

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

#include "SystemUtilities/ThreadPoolAdjuster.h"

void meshDestructor(ComponentID id, uint32_t index) {
	const auto meshBuffer = reinterpret_cast<HAL::mesh_handle_t*>(ecs.getComponentBuffer(id));
	auto mesh = meshBuffer[index];
	HAL::release_mesh(mesh);
}
void instancedMeshDestrutor(ComponentID id, u32 index) {
	u32* instancedMeshBuffer = (u32*)ecs.getComponentBuffer(id);
	u32 instancedMesh = instancedMeshBuffer[index];
	instancedMeshCodex.release(instancedMesh);
}
void registerDestructors() {
	ComponentID meshComponentID = ecs.registerComponentAsBlittable("mesh", sizeof(void*));
	ecs.registerDestructor(meshComponentID, meshDestructor);
	ComponentID instancedMeshComponentID = ecs.registerComponentAsBlittable("instancedMesh", sizeof(u32));
	ecs.registerDestructor(instancedMeshComponentID, instancedMeshDestrutor);
}

inline void initSystems() {
	ecs.init();

	//auto threadCount = HAL::get_hardware_thread_count();
    ////threadCount = (size_t)((threadCount * 0.75f) - 1);
    //threadCount = 3;
    uint32_t threadCount = ThreadPoolAdjuster::getIdealThreadCount();
	std::cout << "ThreadPool thread count: " << threadCount << std::endl;

	threadPool = HAL::get_new_thread_pool(threadCount);

	instancedMeshCodex.init();

	setupFixedPointTableFiles();

	registerDestructors();

	ecs.registerSystem<SystemDeath>();
	ecs.registerSystem<SystemPhysics>();
	ecs.registerSystem<SystemController>();
	//ecs.registerSystem<SystemExplosionTest>();
	ecs.registerSystem<SystemEnemy>();
	ecs.registerSystem<SystemExplode>();

	ecs.registerSystem<SystemVoxel>();
	ecs.registerSystem<SystemDisplay>();
}

#include "Systems_impl.h"