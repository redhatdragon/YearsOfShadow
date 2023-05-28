#include "EchoEngine/HAL/HAL_3D.h"
#include "Systems/Systems.h"

void* exampleMesh;
void* exampleInstancedMeshTest;
void EE_appStart() {
	initSystems();
	//mesh = EE_getNewMesh("./Data/Meshes/Props/Dynamite.obj");
	//EE_setTextureSubmesh(mesh, 0, "diffuse", "./Data/Meshes/Props/D_Dynamite.png");
	////EE_setPositionMesh(mesh, .5, .5, 2);
	//ComponentID meshComponentID = ecs.registerComponent("mesh", sizeof(void*));
	//ComponentID bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
	//EntityID entity = ecs.getNewEntity();
	//ecs.emplace(entity, meshComponentID, &mesh);
	//BodyID body = physics.addStaticBodyBox(30, 155 - 15, 32, "1.0f", "1.0f", "1.0f");
	//ecs.emplace(entity, bodyComponentID, &body);

	//EE_Point3Df positions[3] = { {30+0,140+0,30+0}, {30+1,140+1,30+1}, {30+3,140+3,30+3} };
	//exampleInstancedMeshTest = EE_getNewInstancedMesh("./Data/Meshes/Props/Dynamite.obj");
	//EE_setInstancedSubmeshTexture(exampleInstancedMeshTest, 0, "diffuse", "./Data/Meshes/Props/D_Dynamite.png");
	//EE_setInstancedMeshPositions(exampleInstancedMeshTest, positions, 3);

}
void EE_appLoop() {
	clock_t c = clock();
	EE_drawBackground(0, 0, 0, 255);
	ecs.runSystems();
	//auto dbgInfo = ecs.getDebugInfoStr();
	//for (auto& i : dbgInfo)
	//	std::cout << i << std::endl;
	//EE_drawMesh(mesh);
	//EE_drawText("Test", 20, 20, 12);
	//EE_drawInstancedMesh(exampleInstancedMeshTest);
	c = clock() - c;
	//std::cout << "Total frame: " << c << std::endl;
}
void EE_appEnd() {

}