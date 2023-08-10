#pragma once

class SystemPhysics : public System {
	static void destructPhysicsBody(ComponentID componentID, uint32_t index) {
		EntityID entity = ecs.getOwner(componentID, index);
		BodyID bodyID = ecs.getEntityComponentAs<BodyID>(entity, componentID);
		physics.removeBody(bodyID);
	}
public:
	virtual void init() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
		ComponentID bodyComponentID = ecs.registerComponentAsBlittable("body", sizeof(BodyID));
		ecs.registerDestructor(bodyComponentID, destructPhysicsBody);
		physics.init();
		spawnBoundaries();
	}
	virtual void run() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
		physics.tick();
	}
	virtual const char* getName() {
		return "SystemPhysics";
	}
	void spawnBoundaries() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
		Vec3D<FixedPoint<256 * 256>> pos, siz;

		pos = { 0, 0, 0 };
		siz = { 1, 256, world_size };
		BodyID bodyID;
		bodyID = physics.addStaticBodyBox(pos.x, pos.y, pos.z, siz.x, siz.y, siz.z, (void*)-1, true);  //left

		pos = { 0, 0, 0 };
		siz = { world_size, 1, world_size };
		bodyID = physics.addStaticBodyBox(pos.x, pos.y, pos.z, siz.x, siz.y, siz.z, (void*)-1, true);  //top

		pos = { world_size-1, 0, 0 };
		siz = { 1, 256, world_size };
		bodyID = physics.addStaticBodyBox(pos.x, pos.y, pos.z, siz.x, siz.y, siz.z, (void*)-1, true);  //right

		pos = { 0, 256-1, 0 };
		siz = { world_size, 1, world_size };
		bodyID = physics.addStaticBodyBox(pos.x, pos.y, pos.z, siz.x, siz.y, siz.z, (void*)-1, true);  //down

		pos = { 0, 0, 0 };
		siz = { world_size, 256, 1 };
		bodyID = physics.addStaticBodyBox(pos.x, pos.y, pos.z, siz.x, siz.y, siz.z, (void*)-1, true);  //front

		pos = { 0, 0, 256-1 };
		siz = { world_size, 256, 1 };
		bodyID = physics.addStaticBodyBox(pos.x, pos.y, pos.z, siz.x, siz.y, siz.z, (void*)-1, true);  //back
	}
};