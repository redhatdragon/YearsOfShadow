#pragma once
#include <iostream>

class SystemExplode : public System {
	ComponentID explodeComponentID;
	ComponentID bodyComponentID;
	ComponentID meshComponentID;
	ComponentID deadComponentID;
	static constexpr Vec3D<uint32_t> siz = { 1, 1, 1 };
public:
	virtual void init() {
		explodeComponentID = ecs.registerComponent("explode", sizeof(Explode));
		bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		meshComponentID = ecs.registerComponent("mesh", sizeof(void*));
		deadComponentID = ecs.registerComponent("dead", NULL);
	}
	virtual void run() {
		register uint32_t explodeCount = ecs.getComponentCount(explodeComponentID);
		Explode* explodeComponentBuffer = (Explode*)ecs.getComponentBuffer(explodeComponentID);
		for (uint32_t i = 0; i < explodeCount; i++) {
			Explode& explode = explodeComponentBuffer[i];
			if (explode.canExplode()) {
				EntityID owner = ecs.getOwner(explodeComponentID, i);
				if (ecs.entityHasComponent(owner, deadComponentID) == false)
					ecs.emplace(owner, deadComponentID, NULL);
				BodyID bodyID = *(BodyID*)ecs.getEntityComponent(owner, bodyComponentID);
				Vec3D<FixedPoint<256*256>> rootPosFixed = physics.getPos(bodyID);
				Vec3D<uint32_t> rootPos = {
					(uint32_t)rootPosFixed.x.getAsInt(),
					(uint32_t)rootPosFixed.y.getAsInt(),
					(uint32_t)rootPosFixed.z.getAsInt()
				};
				Vec3D<uint32_t> totalSiz = {1, 1, 1};
				totalSiz *= 3;
				rootPos -= (totalSiz / 2);
				auto solidBlockPositions = voxelWorld.getValidSolidBlockPositions(rootPos, totalSiz);
				for (uint32_t j = 0; j < solidBlockPositions.size(); j++) {
					auto& pos = solidBlockPositions[j];
					voxelWorld.destroyBlock(pos.x, pos.y, pos.z);
				}
				continue;
				std::vector<BodyID> bodies = physics.getBodiesInRectRough(rootPos, totalSiz);
				uint32_t bodyCount = (uint32_t)bodies.size();
				for (uint32_t j = 0; j < bodyCount; j++) {
					BodyID nextBody = bodies[j];
					void* userData = physics.getUserData(nextBody);
					if (userData == (void*)-1)
						continue;
					EntityID nextOwner = (EntityID)userData;
					if (ecs.entityHasComponent(nextOwner, deadComponentID) == true)
						continue;
					ecs.emplace(nextOwner, deadComponentID, NULL);
				}
			}
		}
	}
	virtual const char* getName() {
		return "SystemExplode";
	}
};