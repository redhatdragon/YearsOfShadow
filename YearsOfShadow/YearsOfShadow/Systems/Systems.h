#pragma once
#include <thread>
#include <HAL/HAL.h>

HAL::thread_pool_handle_t threadPool;

#define GAME_TYPE_SINGLE 0
#define GAME_TYPE_CLIENT 1
#define GAME_TYPE_SERVER 2
//#define GAME_TYPE GAME_TYPE_CLIENT

#if GAME_TYPE == GAME_TYPE_CLIENT
#define REWIND_ENABLED
#endif
#define THREADING_ENABLED
#define PROFILER_ENABLED
#include <EchoEngine/DDECS.h>

//#include <memory>
//#include <stddef>

constexpr uint32_t chunk_width = 16, chunk_depth = 16, chunk_height = 256;
constexpr uint32_t world_size = 460 - (460 % chunk_width);  //block across
constexpr uint32_t max_npc = 100;
//constexpr uint32_t hash_width = 2;
//constexpr uint32_t max_bodies_per_hash = 16;
constexpr uint32_t hash_width = 1;
constexpr uint32_t max_bodies_per_hash = 20;//12;


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

#include "Components.h"

#include "SystemDeath.h"
#include "SystemPhysics.h"
#include "SystemVoxel.h"
//#include "SystemExplosionTest.h"
#include "SystemController.h"
#include "SystemExplode.h"
#include "SystemEnemy.h"
#include "SystemNetwork.h"

#include "SystemDisplay.h"

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

	uint32_t threadCount = -1;
	{
		if (HAL::file_exists("Settings.ent") == false) {
			HAL::file_create("Settings.ent");
		}
		auto eo = EntityObjectLoader::createEntityObjectFromFile("Settings.ent");
		if (auto co = eo.getComponent("threadCount")) {
			if (co->type != ComponentObject::TYPE::TYPE_INT) {
				HAL_WARN("Settings.ent has component threadCount but isn't of type int, ignoring!");
				threadCount = ThreadPoolAdjuster::getIdealThreadCount();
			} else {
				threadCount = co->getInt();
				//EntityObjectLoader::writeComponentObjectToFile("Settings.ent", "threadCount", std::to_string(threadCount));
			}
		}
		else {
			threadCount = ThreadPoolAdjuster::getIdealThreadCount();
			EntityObjectLoader::writeComponentObjectToFile("Settings.ent", "threadCount", std::to_string(threadCount));
		}
	}
	#ifdef THREADING_ENABLED
	HAL_LOG("ThreadPool thread count: {}\n", threadCount);
	threadPool = HAL::get_new_thread_pool(threadCount);
	#endif

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
	ecs.registerSystem<SystemNetwork>();
	ecs.registerSystem<SystemDisplay>();
}

#include "Systems_impl.h"