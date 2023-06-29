#pragma once
//#define WIN32_LEAN_AND_MEAN
//#define NOMINMAX
//#define _AMD64_
////#include <debugapi.h>
//#include <Windows.h>
//#include "../EchoEngine/HAL/ExampleImplimentations/GLFW/CustomWindowsThreadPool.h"

//struct PhysicsFrame {
//	Vec3D<uint32_t> dynamicBodyPositions[max_dynamic_bodies];
//	Vec3D<uint32_t> dynamicBodyVelocities[max_dynamic_bodies];
//	void* dynamicBodyDatas[max_dynamic_bodies];
//	uint32_t dynamicBodyIDs[max_dynamic_bodies];
//	uint32_t dynamicBodyCount;
//
//	Vec3D<uint32_t> staticBodyPositions[max_dynamic_bodies];
//	Vec3D<uint32_t> staticBodyVelocities[max_dynamic_bodies];
//	void* staticBodyDatas[max_dynamic_bodies];
//	uint32_t staticBodyIDs[max_dynamic_bodies];
//	uint32_t staticBodyCount;
//
//	struct _CommandFrame {
//		enum {
//			ADD_BODY,
//			REMOVE_BODY
//		}type;
//		BodyAABB bodyData;
//		uint32_t id;
//		inline void setAsAddBody(const BodyAABB& body, uint32_t _id) {
//			type = ADD_BODY;
//			bodyData = body;
//			id = _id;
//		}
//		inline void setAsRemoveBody(uint32_t _id) {
//			type = REMOVE_BODY;
//			id = _id;
//		}
//	};
//
//	FlatBuffer<_CommandFrame, 10000> commandFrames;
//
//	inline void clear() {
//		dynamicBodyCount = 0;
//		commandFrames.count = 0;
//	}
//
//	inline void addBody(const BodyAABB& body, uint32_t id) {
//		commandFrames[commandFrames.count].setAsAddBody(body, id);
//		commandFrames.count++;
//	}
//	inline void removeBody(uint32_t id) {
//		commandFrames[commandFrames.count].setAsRemoveBody(id);
//		commandFrames.count++;
//	}
//	inline void addNextDynamicBodyPositionAndVelocity(const Vec3D<uint32_t>& pos, const Vec3D<uint32_t>& vel) {
//		dynamicBodyPositions[dynamicBodyCount] = pos;
//		dynamicBodyVelocities[dynamicBodyCount] = vel;
//	}
//};

class PhysicsFrame {
	struct BodyData {
		Vec3D<uint32_t> pos, siz, vel;
		bool isSolid;
		void* userData;
		BodyID id;
	};
	struct VelocityData {
		Vec3D<uint32_t> vel;
		BodyID id;
	};
	struct PosAndVel {
		Vec3D<uint32_t> pos, vel;
		BodyID id;
	};
	static constexpr uint32_t max = 20000;
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
	void addVelocity(const Vec3D<uint32_t>& vel, BodyID id) {
		VelocityData velocityData = { vel, id };
		if (velocitesAdded.count == max) {
			std::cout << "Error: PhysicsFrame::addVelocity() called more than " << max
				<< " iterations this frame" << std::endl;
			throw;
		}
		velocitesAdded.push(velocityData);
	}

	//TODO: rename
	void cpyDynamicBodyPosAndVel(BodyID id, const Vec3D<uint32_t>& pos, const Vec3D<uint32_t>& vel) {
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

		//for (uint32_t i = 0; i < velCount; i++) {
		//	VelocityData& velData = velocitesAdded[i];
		//	Vec3D<int32_t> vec = { velData.vel.x, velData.vel.y, velData.vel.z };
		//	vec = { -vec.x, -vec.y, -vec.z };
		//	
		//	BodyAABB* body = &physics.bodies[velData.id.id];
		//	body->vel = { (uint32_t)vec.x, (uint32_t)vec.y, (uint32_t)vec.z };
		//}
		//for (uint32_t i = 0; i < bodyAddCount; i++) {
		//	BodyData& bodyData = bodiesAdded[i];
		//	//physics.bodies[bodyData.id] = {bodyData.pos, }
		//	physics.isValid.setInvalid(bodyData.id.id);
		//}
		//for (uint32_t i = 0; i < bodyRemovedCount; i++) {
		//	BodyData& bodyData = bodiesAdded[i];
		//	BodyAABB& body = physics.bodies[bodyData.id.id];
		//	body.pos = bodyData.pos;
		//	body.siz = bodyData.siz;
		//	body.vel = bodyData.vel;
		//	body.isSolid = bodyData.isSolid;
		//	physics.setUserData(bodyData.id, bodyData.userData);
		//}

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