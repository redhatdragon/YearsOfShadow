#pragma once

class PhysicsFrame {
	struct BodyData {
		Vec3D<physics_fp> pos, siz, vel;
		bool isSolid;
		void* userData;
		BodyID id;
	};
	struct VelocityData {
        Vec3D<physics_fp> vel;
		BodyID id;
	};
	struct PosAndVel {
        Vec3D<physics_fp> pos, vel;
		BodyID id;
	};
	static constexpr uint32_t max = 40000;
	FlatBuffer<BodyData, max> bodiesAdded;
	FlatBuffer<VelocityData, max> velocitesAdded;
	FlatBuffer<BodyData, max> bodiesRemoved;
	FlatBuffer<PosAndVel, max> dynamicBodyPosAndVelMirror;
public:
	void clear() {
		bodiesAdded.count = velocitesAdded.count = bodiesRemoved.count =
			dynamicBodyPosAndVelMirror.count = 0;
	}
	void addBody(const BodyAABB& body, bool isSolid, void* userData, BodyID id) {
		BodyData bodyData = { body.pos, body.siz, body.vel, isSolid, userData, id };
		if (bodiesAdded.count == max) {
			std::cout << "Error: PhysicsFrame::addBody() called more than " << max
				<< " iterations this frame" << std::endl;
			throw;
		}
		bodiesAdded.push(bodyData);
	}
	void removeBody(const BodyAABB& body, bool isSolid, void* userData, BodyID id) {
		BodyData bodyData = { body.pos, body.siz, body.vel, isSolid, userData, id };
		if (bodiesRemoved.count == max) {
			std::cout << "Error: PhysicsFrame::removeBody() called more than " << max
				<< " iterations this frame" << std::endl;
			throw;
		}
		bodiesRemoved.push(bodyData);
	}
	void addVelocity(const Vec3D<physics_fp>& vel, BodyID id) {
		VelocityData velocityData = { vel, id };
		if (velocitesAdded.count == max) {
			std::cout << "Error: PhysicsFrame::addVelocity() called more than " << max
				<< " iterations this frame" << std::endl;
			throw;
		}
		velocitesAdded.push(velocityData);
	}

	//TODO: rename
	void cpyDynamicBodyPosAndVel(BodyID id, const Vec3D<physics_fp>& pos, const Vec3D<physics_fp>& vel) {
		PosAndVel pav = { pos, vel, id };
		if (dynamicBodyPosAndVelMirror.count == max) {
			std::cout << "Error: PhysicsFrame::cpyDynamicBodyPosAndVel() called more than " << max
				<< " iterations this frame" << std::endl;
			throw;
		}
		dynamicBodyPosAndVelMirror.push(pav);
	}

	void rewind(PhysicsEngineAABB3D& physics) {
		uint32_t velCount = velocitesAdded.count;
		uint32_t bodyAddCount = bodiesAdded.count;
		uint32_t bodyRemovedCount = bodiesRemoved.count;
		uint32_t dbCount = dynamicBodyPosAndVelMirror.count;

		for (uint32_t i = 0; i < velCount; i++) {
			VelocityData& velData = velocitesAdded[i];
            Vec3D<physics_fp> vec = {velData.vel.x, velData.vel.y, velData.vel.z};
			vec = { -vec.x, -vec.y, -vec.z };
			
			BodyAABB* body = &physics.bodies[velData.id.id];
			body->vel = { vec.x, vec.y, vec.z };
		}
		for (uint32_t i = 0; i < bodyAddCount; i++) {
			BodyData& bodyData = bodiesAdded[i];
			//physics.bodies[bodyData.id] = {bodyData.pos, }
			physics.isValid.setInvalid(bodyData.id.id);
            physics.spatialHashTable.removeBody(bodyData.id, bodyData.pos, bodyData.siz);
		}
		for (uint32_t i = 0; i < bodyRemovedCount; i++) {
			BodyData& bodyData = bodiesRemoved[i];
			BodyAABB& body = physics.bodies[bodyData.id.id];
            physics.isValid.setValid(bodyData.id.id);
			body.pos = bodyData.pos;
			body.siz = bodyData.siz;
			body.vel = bodyData.vel;
			body.isSolid = bodyData.isSolid;
			physics.setUserData(bodyData.id, bodyData.userData);
            physics.spatialHashTable.addBody(bodyData.id, body.pos, body.siz);
		}

		for (uint32_t i = 0; i < dbCount; i++) {
			PosAndVel& pav = dynamicBodyPosAndVelMirror[i];
			BodyAABB& body = physics.bodies[pav.id.id];
			physics.spatialHashTable.removeBody(pav.id, body.pos, body.siz);
			body.pos = pav.pos;
			body.vel = pav.vel;
			physics.spatialHashTable.addBody(pav.id, body.pos, body.siz);
		}
	}
};

uint64_t sizeofPhysicsFrame = sizeof(PhysicsFrame);