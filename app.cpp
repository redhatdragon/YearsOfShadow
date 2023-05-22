#include "EchoEngine/HAL/HAL_3D.h"
#include "Systems/Systems.h"

void* mesh;
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
}
void EE_appLoop() {
	EE_drawBackground(0, 0, 0, 255);
	ecs.runSystems();
	//EE_drawMesh(mesh);
	//EE_drawText("Test", 20, 20, 12);
}
void EE_appEnd() {

}