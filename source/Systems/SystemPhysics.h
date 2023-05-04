#pragma once

class SystemPhysics : public System {
	static void destructPhysicsBody(ComponentID componentID, uint32_t index) {
		EntityID entity = ecs.getOwner(componentID, index);
		BodyID bodyID = ecs.getEntityComponentAs<BodyID>(entity, componentID);
		physics.removeBody(bodyID);
	}
public:
	virtual void init() {
		ComponentID bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		ecs.registerDestructor(bodyComponentID, destructPhysicsBody);
		physics.init();
		spawnBoundaries();
	}
	virtual void run() {
		physics.tick();
	}
	virtual const char* getName() {
		return "SystemPhysics";
	}
	void spawnBoundaries() {
		Vec3D<FixedPoint<256 * 256>> pos, siz;

		pos = { 0, 0, 0 };
		siz = { 1, 256, world_size };
		BodyID bodyID;
		bodyID = physics.addStaticBodyBox(pos.x, pos.y, pos.z, siz.x, siz.y, siz.z);  //left
		physics.setSolid(bodyID, true);
		physics.setUserData(bodyID, (void*)-1);

		pos = { 0, 0, 0 };
		siz = { world_size, 1, world_size };
		bodyID = physics.addStaticBodyBox(pos.x, pos.y, pos.z, siz.x, siz.y, siz.z);  //top
		physics.setSolid(bodyID, true);
		physics.setUserData(bodyID, (void*)-1);

		pos = { world_size-1, 0, 0 };
		siz = { 1, 256, world_size };
		bodyID = physics.addStaticBodyBox(pos.x, pos.y, pos.z, siz.x, siz.y, siz.z);  //right
		physics.setSolid(bodyID, true);
		physics.setUserData(bodyID, (void*)-1);

		pos = { 0, 256-1, 0 };
		siz = { world_size, 1, world_size };
		bodyID = physics.addStaticBodyBox(pos.x, pos.y, pos.z, siz.x, siz.y, siz.z);  //down
		physics.setSolid(bodyID, true);
		physics.setUserData(bodyID, (void*)-1);

		pos = { 0, 0, 0 };
		siz = { world_size, 256, 1 };
		bodyID = physics.addStaticBodyBox(pos.x, pos.y, pos.z, siz.x, siz.y, siz.z);  //front
		physics.setSolid(bodyID, true);
		physics.setUserData(bodyID, (void*)-1);

		pos = { 0, 0, 256-1 };
		siz = { world_size, 256, 1 };
		bodyID = physics.addStaticBodyBox(pos.x, pos.y, pos.z, siz.x, siz.y, siz.z);  //back
		physics.setSolid(bodyID, true);
		physics.setUserData(bodyID, (void*)-1);
	}
};