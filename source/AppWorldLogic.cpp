/* Copyright (C) 2005-2022, UNIGINE. All rights reserved.
 *
 * This file is a part of the UNIGINE 2 SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE. at http://unigine.com/
 */


//#define TESTING
#include "AppWorldLogic.h"
#include "Systems/Systems.h"

#include <vector>



void EE_appStart() {
	//testMesh = EE_getNewMesh("Meshes/Cube2.fbx/cube.001.mesh");
	//EE_setPositionMesh(testMesh, 100, 100, 100);
	//EE_setTextureSubmesh(testMesh, 0, 0, "Textures/photos_2017_10_6_fst_grass-texture-lawn.jpg");
	//EE_setScaleMesh(testMesh, 10, 10, 10);

	//uint32_t maxRoot = 100;
	//for (uint32_t i = 0; i < maxRoot; i++) {
	//	for (uint32_t j = 0; j < maxRoot; j++) {
	//		void* mesh = EE_getNewMesh("Meshes/Cube2.fbx/cube.001.mesh");
	//		EE_setPositionMesh(mesh, 100 + i * 100, 100 + j * 100, 200);
	//		EE_setTextureSubmesh(mesh, 0, 0, "Textures/photos_2017_10_6_fst_grass-texture-lawn.jpg");
	//		EE_setScaleMesh(mesh, 50, 50, 50);
	//	}
	//}

	//uint32_t maxRoot = 100;
	//std::vector<EE_Point3Df> positions;
	//positions.reserve(maxRoot * maxRoot);
	//void* instancedMesh = EE_getNewInstancedMesh("Meshes/Cube2.fbx/cube.001.mesh");
	//for (uint32_t i = 0; i < maxRoot; i++) {
	//	for (uint32_t j = 0; j < maxRoot; j++) {
	//		for (uint32_t k = 0; k < maxRoot; k++) {
	//			EE_Point3Df p = { 100 + i * 100, 100 + j * 100, 200 + k * 100 };
	//			positions.push_back(p);
	//		}
	//	}
	//}
	//EE_setInstancedMeshPos(instancedMesh, &positions[0], positions.size());

	//physics.init();
	//voxelWorld.init(100, 0, 0, 0);
	//testMesh = EE_getNewMesh("Meshes/Cube2.fbx/cube.001.mesh");
	////EE_setTextureSubmesh(testMesh, 0, 0, "Textures/photos_2017_10_6_fst_grass-texture-lawn.jpg");
	//EE_setScaleMesh(testMesh, 50*2, 50*2, 50*2);
	//testBody = physics.addBodyBox(10, 0, 10, 1, 1, 1);
	//physics.setSolid(testBody, true);
	//physics.setVelocity(testBody, 0, "0.1f", 0);
	////EE_setPositionMesh(testMesh, 10, 20, 10);
	//
	//testMesh2 = EE_getNewMesh("Meshes/Cube2.fbx/cube.001.mesh");
	////EE_setTextureSubmesh(testMesh, 0, 0, "Textures/photos_2017_10_6_fst_grass-texture-lawn.jpg");
	//EE_setScaleMesh(testMesh2, 50 * 2, 50 * 2, 50 * 2);
	////testBody2 = physics.addBodyBox(10, 150, 10, 1, 1, 1);
	//testBody2 = physics.addBodyBox(10, 25, 10, 1, 1, 1);
	//physics.setVelocity(testBody2, 0, "0.1f", 0);
	//physics.setSolid(testBody2, true);
	testFlatFlaggedBuffer();
	initSystems();
}
void EE_appLoop() {
	//voxelWorld.display();
	//auto pos = physics.getPos<uint32_t>(testBody);
	//EE_setPositionMesh(testMesh, pos.x, pos.y, pos.z);
	//pos = physics.getPos<uint32_t>(testBody2);
	//EE_setPositionMesh(testMesh2, pos.x, pos.y, pos.z);
	////EE_setPositionMesh(testMesh, 10, 20, 10);
	//EE_drawMesh(testMesh);
	//physics.tick();
	ecs.runSystems();
	//std::cout << physics.getBodyCount() << std::endl;
	//for (auto& str : physics.getDbgInfo())
	//	std::cout << str << std::endl;

	//std::vector<std::string> dbgInfo = ecs.getDebugInfoStr();
	//for (auto& str : dbgInfo) {
	//	std::cout << str << std::endl;
	//}
}
void EE_appEnd() {

}



// World logic, it takes effect only when the world is loaded.
// These methods are called right after corresponding world script's (UnigineScript) methods.

AppWorldLogic::AppWorldLogic()
{
}

AppWorldLogic::~AppWorldLogic()
{
}

int AppWorldLogic::init() {
	// Write here code to be called on world initialization: initialize resources for your world scene during the world start.
	EE_appStart();
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// start of the main loop
////////////////////////////////////////////////////////////////////////////////

int AppWorldLogic::update() {
	// Write here code to be called before updating each render frame: specify all graphics-related functions you want to be called every frame while your application executes.
	return 1;
}

int AppWorldLogic::postUpdate()
{
	// The engine calls this function after updating each render frame: correct behavior after the state of the node has been updated.
	return 1;
}

int AppWorldLogic::updatePhysics()
{
	// Write here code to be called before updating each physics frame: control physics in your application and put non-rendering calculations.
	// The engine calls updatePhysics() with the fixed rate (60 times per second by default) regardless of the FPS value.
	// WARNING: do not create, delete or change transformations of nodes here, because rendering is already in progress.
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// end of the main loop
////////////////////////////////////////////////////////////////////////////////

int AppWorldLogic::shutdown() {
	// Write here code to be called on world shutdown: delete resources that were created during world script execution to avoid memory leaks.
	EE_appEnd();
	return 1;
}

int AppWorldLogic::save(const Unigine::StreamPtr& stream)
{
	// Write here code to be called when the world is saving its state (i.e. state_save is called): save custom user data to a file.
	UNIGINE_UNUSED(stream);
	return 1;
}

int AppWorldLogic::restore(const Unigine::StreamPtr& stream)
{
	// Write here code to be called when the world is restoring its state (i.e. state_restore is called): restore custom user data to a file here.
	UNIGINE_UNUSED(stream);
	return 1;
}

int AppWorldLogic::swap() {
	EE_appLoop();

	return 1;
}