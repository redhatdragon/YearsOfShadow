#include "AppSystemLogic.h"
#include <UnigineConsole.h>
#include <UnigineComponentSystem.h>

int AppSystemLogic::init()
{
	// run ComponentSystem
	Unigine::ComponentSystem::get()->initialize();

	return 1;
}

