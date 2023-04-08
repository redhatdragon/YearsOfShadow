#include "EchoEngine/HAL/HAL_3D.h"
#include "Systems/Systems.h"

void EE_appStart() {
	testFlatFlaggedBuffer();
	initSystems();
}
void EE_appLoop() {
	ecs.runSystems();
}
void EE_appEnd() {

}