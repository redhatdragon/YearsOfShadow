#include <IO_API/IO_API.h>
#include <DDECS.h>
#include <PhysicsEngineAABB2.h>
//#include "PhysicsEngineConvex.h"
#include "Asset.h"
#include <iostream>
#include <time.h>

//#include "Systems/systems.h"

struct ThreadPool* threadPool;

/*

void foo(void*) {

}

void appStart() {
	auto c = clock();

	std::cout << "Pre initialization finished in " << c << " miliseconds." << std::endl;
	std::cout << "Initializing app...    ";

	physics.init();

	setFPS(600);

	threadPool = ThreadPoolCreate(12);
	//ThreadPoolGiveTask(threadPool, foo, NULL);

	registerComponents();

	
	EntityID player = ecs.getNewEntity();
	BodyID bodyID = physics.addBodyRect(128, 128, 16, 16);
	physics.setUserData(bodyID, (void*)player);
	std::string dataDir = getDirData();
	TextureID koiTexID = TextureCodex::add(dataDir+"Textures/Koishi.png");
	ecs.emplace(player, bodyComponentID, &bodyID);
	ecs.emplace(player, playerControllerComponentID, NULL);
	ecs.emplace(player, textureComponentID, &koiTexID);
	ecs.emplace(player, detectOverlapComponentID, NULL);
	uint32_t health = 100;
	ecs.emplace(player, healthComponentID, &health);
	//ecs.emplace(player, suicideOnCollisionComponentID, NULL);

	for(auto i = 0; i < 400; i++) 
		for (auto j = 0; j < 1; j++) {
			EntityID npc = ecs.getNewEntity();
			BodyID bodyID = physics.addBodyRect(i*96, j*96, 64, 64);
			physics.addVelocity(bodyID, 2*i%3, 2*i%5);
			physics.setUserData(bodyID, (void*)npc);
			ecs.emplace(npc, bodyComponentID, &bodyID);
			ecs.emplace(npc, textureComponentID, &koiTexID);
			ecs.emplace(npc, healthComponentID, &health);
			ecs.emplace(npc, detectOverlapComponentID, NULL);
		}
	c = clock()-c;
	std::cout << "Done!  \nInitialized in " << c << " miliseconds." << std::endl;
}
void appLoop() {
	physics.tick();
	//systemAI.run();
	systemDisplay.run();
	systemPlayer.run();
	systemDamage.run();
	systemDead.run();
	int x = 32, y = 16, s = 16, d = 8;
	std::string str = "";
	str = "FPS: "; str += std::to_string(getFPS());
	drawText(str.c_str(), x, y, s); y += s + d;
	str = "EntityCount: "; str += std::to_string(ecs.getEntityCount());
	drawText(str.c_str(), x, y, s); y += s + d;
	str = "Physics: "; str += std::to_string((int)physics.getTime());
	drawText(str.c_str(), x, y, s); y += s + d;
	str = "systemAI: "; str += systemAI.getTimeMSStr();
	drawText(str.c_str(), x, y, s); y += s + d;
	str = "systemDisplay: "; str += systemDisplay.getTimeMSStr();
	drawText(str.c_str(), x, y, s); y += s + d;
	str = "systemPlayer: "; str += systemPlayer.getTimeMSStr();
	drawText(str.c_str(), x, y, s); y += s + d;
	str = "systemDamage: "; str += systemDamage.getTimeMSStr();
	drawText(str.c_str(), x, y, s); y += s + d;
	str = "systemDead: "; str += systemDead.getTimeMSStr();
	drawText(str.c_str(), x, y, s); y += s + d;
}
void appEnd() {
	ThreadPoolDestroy(threadPool);
}

*/