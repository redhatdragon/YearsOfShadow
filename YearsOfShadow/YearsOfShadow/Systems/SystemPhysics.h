#pragma once

void serializeBody(EntityID entity, std::vector<uint8_t>& out) {
	void* data = nullptr;
	ComponentID componentID = ecs.registerComponentAsBlittable("body", sizeof(BodyID));
	BodyID bodyID = *(BodyID*)ecs.getEntityComponent(entity, componentID);
	BodyAABB body = physics._getBodyCpy(bodyID);
	out.resize(sizeof(body));
	memcpy(&out[0], &body, sizeof(body));
}
void deserializeBody(EntityID entity, const std::vector<uint8_t>& in) {
	ComponentID componentID = ecs.registerComponentAsBlittable("body", sizeof(BodyID));
	if (in.size() != sizeof(BodyAABB)) {
		HAL_ERROR("deserializeBody()'s in data's size: {} != sizeof BodyAABB: {}",
			in.size(), sizeof(BodyAABB));
		return;
	}
	if (ecs.entityHasComponent(entity, componentID) == false) {
		BodyAABB* body = (BodyAABB*)&in[0];
		BodyID bodyID = physics.addBodyBox(body->pos.x, body->pos.y, body->pos.z,
			body->siz.x, body->siz.y, body->siz.z, &entity, body->isSolid);
		ecs.emplaceOrCpy(entity, componentID, &bodyID);
		return;
	}
	BodyAABB* body = (BodyAABB*)&in[0];
	BodyID bodyID = *(BodyID*)ecs.getEntityComponent(entity, componentID);
	physics.setPosition(bodyID, body->pos.x, body->pos.y, body->pos.z);
}

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
		ecs.registerDestructor(bodyComponentID, destructPhysicsBody);
		SystemUtilities::SerialEntity::registerSerializeFunction(bodyComponentID, serializeBody);
		SystemUtilities::SerialEntity::registerDeSerializeFunction(bodyComponentID, deserializeBody);
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