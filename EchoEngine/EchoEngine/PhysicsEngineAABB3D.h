#pragma once

#include "Vec.h"
#include "FlatBuffer.h"
#include "DArray.h"
#ifdef THREADING_ENABLED
#include "HAL/HAL.h"
#endif
#ifdef REWIND_ENABLED
#include "../customDBG.h"
//#include <Windows.h>
#include "RingBuffer.h"
#endif
#include <time.h>


/*
ABOUT
This is a simple multithreaded fixed point deterministic AABB physics engine using a spatial hash table with
optional rewind capability.  It doesn't have friction/weight/bouncing and is intended to be fairly effecient
at the expense of functionality and perhaps ideal memory usage.

This is a somewhat specialized and yet modular physics engine.  Yet hopefully lean enough to rework as need be.
This does depend on HAL.h's API specifically to use a thread pool that's meant to be directly used with the
application as well.  Essentially this engine's designed to be synced/batch threaded and will intentionally
wait till the thread pool isn't at all working on anything before proceeding with it's own tasks.  Ideal
when most of your game logic uses a similar threading model which to be frank is more ideal for batched work.
*/

//#define ENABLE_REWIND  use this to enable rewind functionality, useful when you want this functionality on different executables

#ifndef physics_unit_size
#define physics_unit_size (256*256)  //used as a pretend "1" normalized value to emulate decimal
#else
// defined externally, reusing external define
#endif

#define physics_fp FixedPoint<physics_unit_size>

//TODO: Consider expanding for future utility.

#include "PhysicsEngineAABB3D_BodyAABB.h"

#include "PhysicsEngineAABB3D_SHT.h"



template<uint32_t width, uint32_t height, uint32_t depth, uint32_t hash_width, uint32_t max_bodies_per_hash = 64, uint32_t max_bodies_per_collision = 64>
class PhysicsEngineAABB3D {
	static constexpr uint32_t max_dynamic_bodies = 100000;  //100k
	static constexpr uint32_t max_static_bodies = 1000000;  //1mill
	static constexpr uint32_t max_bodies = max_dynamic_bodies + max_static_bodies;
	DArray<BodyAABB> bodies;
	uint32_t dynamicBodyCount;
	uint32_t staticBodyCount;  //mostly for debug purposes
	FlatFlagBuffer<max_bodies> isValid;
	DArray<void*> userData;
	DArray<FlatBuffer<BodyID, max_bodies_per_collision>> overlappingBodyIDs;
	SpatialHashTable<width, height, depth, hash_width* physics_unit_size, max_bodies_per_hash> spatialHashTable;
	float timeFromStepping;

	uint32_t threadsFinished;

	//NOTE: used for rewinding
	#ifdef REWIND_ENABLED
	#include "PhysicsFrame.h"
	static constexpr uint32_t max_frames = 60 * 5;  //5 seconds at 60 TPS
	RingBuffer<PhysicsFrame, max_frames> frames;
	#endif

public:
	void init() {
		dynamicBodyCount = staticBodyCount = 0;
		timeFromStepping = 0.f;

		bodies.init(max_bodies);
		overlappingBodyIDs.init(max_bodies);
		for (uint32_t i = 0; i < max_bodies; i++) {
			overlappingBodyIDs[i].clear();
		}
		userData.init(max_bodies);
		spatialHashTable.init();
		timeFromStepping = 0.f;
		threadsFinished = 0;
		#ifdef REWIND_ENABLED
		frames.init();
		frames.get().clear();
		#endif
	}
	BodyID addBodyBox(physics_fp x, physics_fp y, physics_fp z,
		physics_fp w, physics_fp h, physics_fp d, void* data, bool isSolid) {
		BodyID retValue;
		BodyAABB body = {
			{
				static_cast<uint32_t>(x.getRaw()), static_cast<uint32_t>(y.getRaw()), static_cast<uint32_t>(z.getRaw())
			}, { static_cast<uint32_t>(w.getRaw()), static_cast<uint32_t>(h.getRaw()), static_cast<uint32_t>(d.getRaw())},
			{ 0u,0u,0u }
		};
		//retValue.id = bodies.insert(body);
		retValue.id = isValid.toggleFirstInvalid();
		bodies[retValue.id] = body;
		setUserData(retValue, data);
		setIsSolid(retValue, isSolid);
		#ifdef REWIND_ENABLED
		frames.get().addBody(bodies[retValue.id], isSolid, data, retValue);
		#endif
		spatialHashTable.addBody(retValue, body.pos, body.siz);
		overlappingBodyIDs[retValue.id].clear();
		dynamicBodyCount++;
		return retValue;
	}
	BodyID addStaticBodyBox(physics_fp x, physics_fp y, physics_fp z,
		physics_fp w, physics_fp h, physics_fp d, void* data, bool isSolid) {
		BodyID retValue;
		BodyAABB body = { { (uint32_t)x.getRaw(), (uint32_t)y.getRaw(), (uint32_t)z.getRaw() },
			{ (uint32_t)w.getRaw(), (uint32_t)h.getRaw(), (uint32_t)d.getRaw() }, { 0,0,0 } };
		retValue.id = isValid.toggleFirstInvalid(max_dynamic_bodies);
		bodies[retValue.id] = body;
		setUserData(retValue, data);
		setIsSolid(retValue, isSolid);
		#ifdef REWIND_ENABLED
		frames.get().addBody(bodies[retValue.id], isSolid, data, retValue);
		#endif
		spatialHashTable.addBody(retValue, body.pos, body.siz);
		overlappingBodyIDs[retValue.id].clear();
		staticBodyCount++;
		return retValue;
	}
	void removeBody(BodyID bodyID) {
		const BodyAABB& body = bodies[bodyID.id];
		spatialHashTable.removeBody(bodyID, body.pos, body.siz);
		if (bodyID.id < max_dynamic_bodies)
			if (dynamicBodyCount)
				dynamicBodyCount--;
			else
				throw;
		else
			if (staticBodyCount)
				staticBodyCount--;
			else
				throw;
		isValid.setInvalid(bodyID.id);

		#ifdef REWIND_ENABLED
		bool _isSolid = getIsSolid(bodyID);
		void* _userData = getUserData(bodyID);
		frames.get().removeBody(body, _isSolid, _userData, bodyID);
		#endif
	}

	void* getUserData(BodyID id) {
		return userData[id.id];
	}

	FlatBuffer<BodyID, 100>& getOverlappingBodies(BodyID id) {
		return overlappingBodyIDs[id.id];
	}

	inline std::vector<BodyID> getBodiesInRectRough(Vec3D<physics_fp> pos, Vec3D<physics_fp> siz) {
		std::vector<BodyID> retValue;
		auto hashes = spatialHashTable.getHashes(*(Vec3D<uint32_t>*)&pos, *(Vec3D<uint32_t>*)&siz);
		retValue.reserve(hashes.size());
		uint32_t hashCount = (uint32_t)hashes.size();
		for (uint32_t i = 0; i < hashCount; i++) {
			auto* hashPtr = hashes[i];
			uint32_t hashSize = hashPtr->count;
			for (uint32_t j = 0; j < hashSize; j++) {
				BodyID bodyID = (*hashPtr)[j];
				uint32_t totalBodyCount = (uint32_t)retValue.size();
				for (uint32_t k = 0; k < totalBodyCount; k++) {
					if (retValue[k] == bodyID)
						goto skip;
				}
				retValue.push_back(bodyID);
			skip:
				continue;
			}
		}
		return retValue;
	}
	inline std::vector<BodyID> getBodiesInRectRough(Vec3D<uint32_t> pos, Vec3D<uint32_t> siz) {
		pos *= physics_unit_size; siz *= physics_unit_size;
		return getBodiesInRectRough(*(Vec3D<physics_fp>*)&pos, *(Vec3D<physics_fp>*)&siz);
	}
	inline bool pointTrace(const Vec3D<FixedPoint<256 * 256>>& pos, BodyID ignoredBody) {
		std::vector<BodyID> out = {};
		Vec3D<uint32_t> siz = { 1, 1, 1 };
		spatialHashTable.getIDs(*(Vec3D<uint32_t>*) & pos, siz, out);
		uint32_t size = (uint32_t)out.size();
		for (uint32_t i = 0; i < size; i++)
			if (out[i] != ignoredBody)
				return true;
		return false;
	}
	void setVelocity(BodyID id, physics_fp vx, physics_fp vy, physics_fp vz) {
		BodyAABB* body = &bodies[id.id];
		#ifdef REWIND_ENABLED
		Vec3D<uint32_t> offset = { (uint32_t)vx.getRaw(), (uint32_t)vy.getRaw(), (uint32_t)vz.getRaw() };
		offset -= body->vel;
		frames.get().addVelocity(offset, id);
		#endif
		body->vel.x = (uint32_t)vx.getRaw();
		body->vel.y = (uint32_t)vy.getRaw();
		body->vel.z = (uint32_t)vz.getRaw();
	}
	Vec3D<physics_fp> getVelocity(BodyID id) {
		BodyAABB* body = &bodies[id.id];
		return *(Vec3D<physics_fp>*) & body->vel;
	}

	bool getIsSolid(BodyID id) {
		return bodies[id.id].isSolid;
	}

	Vec3D<FixedPoint<physics_unit_size>> getPos(BodyID id) {
		Vec3D<uint32_t>& bodyPos = bodies[id.id].pos;
		Vec3D<FixedPoint<physics_unit_size>> pos;
		pos.x.setRaw(bodyPos.x);
		pos.y.setRaw(bodyPos.y);
		pos.z.setRaw(bodyPos.z);
		return pos;
	}
	Vec3D<float> getPosF(BodyID id) {
		Vec3D<uint32_t>& bodyPos = bodies[id.id].pos;
		Vec3D<float> pos = { (float)bodyPos.x / physics_unit_size, (float)bodyPos.y / physics_unit_size, (float)bodyPos.z / physics_unit_size };
		return pos;
	}
	Vec3D<FixedPoint<physics_unit_size>> getSize(BodyID id) {
		Vec3D<uint32_t> bodySiz = bodies[id.id].siz;
		Vec3D<FixedPoint<physics_unit_size>> siz;
		siz.x.setRaw(bodySiz.x);
		siz.y.setRaw(bodySiz.y);
		siz.z.setRaw(bodySiz.z);
		return siz;
	}
	Vec3D<float> getSizeF(BodyID id) {
		Vec3D<uint32_t>& bodySiz = bodies[id.id].siz;
		Vec3D<float> siz = { (float)bodySiz.x / physics_unit_size, (float)bodySiz.y / physics_unit_size, (float)bodySiz.z / physics_unit_size };
		return siz;
	}

	float getTime() {
		return timeFromStepping;
	}

	static constexpr inline int64_t getSizeofBroad() {
		return sizeof(spatialHashTable);
	}
	static constexpr inline int64_t getSizeofBodies() {
		return sizeof(bodies);
	}

	uint32_t lastBodyIndex;
	inline void simulate() {
		uint32_t bodyCount = dynamicBodyCount;
		if (bodyCount == 0) return;
		uint32_t validBodyCount = 0;
		for (uint32_t i = 0; true; i++) {
			if (isValid.getIsValid(i) == false)
				continue;
			validBodyCount++;
			spatialHashTable.removeBody({ i }, bodies[i].pos, bodies[i].siz);
			bodies[i].simulate();
			spatialHashTable.addBody({ i }, bodies[i].pos, bodies[i].siz);
			if (validBodyCount >= bodyCount) {
				lastBodyIndex = i;
				break;
			}
		}
	}
	inline void detectSingle() {
		uint32_t bodyCount = dynamicBodyCount;
		if (bodyCount == 0) return;
		for (uint32_t i = 0; i <= lastBodyIndex; i++) {
			if (isValid.getIsValid(i) == false)
				continue;
			overlappingBodyIDs[i].clear();
		}

		static std::vector<BodyID> IDs;
		static DetectThreadData lastDTD;
		u32 start = 0; u32 end = start + lastBodyIndex;
		lastDTD = { this, start, end };
		detectThreadBody(&lastDTD);
	}
	struct DetectThreadData {
		void* self;
		uint32_t start, end;
	};
	inline void detect() {
		uint32_t bodyCount = dynamicBodyCount;
		if (bodyCount == 0) return;
		for (uint32_t i = 0; i <= lastBodyIndex; i++) {
			if (isValid.getIsValid(i) == false)
				continue;
			overlappingBodyIDs[i].clear();
		}

		uint16_t threadCount = EE_getThreadPoolFreeThreadCount(threadPool);
		static std::vector<DetectThreadData> dtd;
		dtd.clear();
		dtd.reserve(threadCount);
		uint32_t totalWork = lastBodyIndex + 1;
		uint32_t workPerThread = totalWork / threadCount;
		uint32_t leftover = totalWork % threadCount;
		for (uint32_t i = 0; i < threadCount; i++) {
			u32 start = workPerThread * i; u32 end = start + workPerThread - 1;
			DetectThreadData currentDTD = { this, start, end };
			dtd.push_back(currentDTD);
		}
		for (uint32_t i = 0; i < threadCount; i++) {
			EE_sendThreadPoolTask(threadPool, detectThreadBody, &dtd[i]);
		}
		static DetectThreadData lastDTD;
		u32 start = workPerThread * threadCount; u32 end = start + leftover;
		lastDTD = { this, start, end };
		if (leftover)
			detectThreadBody(&lastDTD);
		while (EE_isThreadPoolFinished(threadPool) == false)
			continue;
	}
	static void detectThreadBody(void* _data) {
		DetectThreadData* data = (DetectThreadData*)_data;
		PhysicsEngineAABB3D<width, height, depth, hash_width, max_bodies_per_hash>* self =
			(PhysicsEngineAABB3D<width, height, depth, hash_width, max_bodies_per_hash>*)data->self;

		std::vector<BodyID> IDs;
		IDs.reserve(100);
		for (uint32_t i = data->start; i <= data->end; i++) {
			if (self->isValid.getIsValid(i) == false)
				continue;
			IDs.clear();
			const BodyAABB& b = self->bodies[i];
			self->spatialHashTable.getIDs(b.pos, b.siz, IDs);
			size_t IDCount = IDs.size();
			for (size_t j = 0; j < IDCount; j++) {
				if (i == IDs[j].id)
					continue;
				if (b.collidesWith(self->bodies[IDs[j].id])) {
					//self->overlappingBodyPushIfUnique(i, IDs[j]);  //Needed?
					self->overlappingBodyIDs[i].push(IDs[j]);
				}
			}
		}
	}
	inline void resolve() {
		uint32_t bodyCount = dynamicBodyCount;
		if (bodyCount == 0) return;
		for (uint32_t i = 0; i <= lastBodyIndex; i++) {
			if (isValid.getIsValid(i) == false)
				continue;
			bool isSolid = getIsSolid({ i });
			if (overlappingBodyIDs[i].count && bodies[i].vel.isZero() == false && getIsSolid({ i })) {
				static FlatBuffer<BodyAABB*, 100> overlappingSolidBodies = {};
				overlappingSolidBodies.clear();
				getOverlappingSolidBodies(i, overlappingSolidBodies);
				if (overlappingSolidBodies.count) {

					spatialHashTable.removeBody({ i }, bodies[i].pos, bodies[i].siz);
					bodies[i].reverseSimulate();
					auto newPos = bodies[i].resolveClosestSafePosition(&overlappingSolidBodies[0],
						overlappingSolidBodies.count);
					bodies[i].pos = newPos;
					*(Vec3D<int32_t>*)& bodies[i].vel /= 2;
					spatialHashTable.addBody({ i }, bodies[i].pos, bodies[i].siz);
				}
			}
			gravity({ i });
#ifdef REWIND_ENABLED
			frames.get().cpyDynamicBodyPosAndVel({i}, bodies[i].pos, bodies[i].vel);
#endif
		}
	}

	void tick() {
		clock_t c = clock();
		#ifdef REWIND_ENABLED
		frames.advance();
		frames.get().clear();  //TODO: 0th frame isn't used initially, figure out later
		#endif
		simulate();
		#ifdef THREADING_ENABLED
		detect();
		#else
		detectSingle();
		#endif
		resolve();
		timeFromStepping = (float)(clock() - c);
		//std::cout << "Time: " << timeFromStepping << std::endl;
	}

	std::vector<std::string> getDbgInfo() {
		std::vector<std::string> retValue;
		uint64_t total = 0;
		std::string str = "sizeof overlappingBodyIDs: ";
		total += overlappingBodyIDs.size() * sizeof(overlappingBodyIDs[0]);
		str += std::to_string(overlappingBodyIDs.size() * sizeof(overlappingBodyIDs[0]));
		retValue.push_back(str);
		str = "sizeof bodies: ";
		total += bodies.size() * sizeof(bodies[0]);
		str += std::to_string(bodies.size() * sizeof(bodies[0]));
		retValue.push_back(str);
		str = "sizeof userData: ";
		total += userData.size() * sizeof(userData[0]);
		str += std::to_string(userData.size() * sizeof(userData[0]));
		retValue.push_back(str);

		str = "sizeof total^: ";
		str += std::to_string(total);
		retValue.push_back(str);

		str = "sizeof spatialHashTable: ";
		str += spatialHashTable.getDbgStr();
		retValue.push_back(str);
		str = "sizeof isValid ";
		total += sizeof(isValid);
		str += std::to_string(sizeof(isValid));
		retValue.push_back(str);
		return retValue;
	}

	uint64_t getBodyCount() {
		return dynamicBodyCount + staticBodyCount;
	}

	constexpr int32_t getGravityAcceleration() {
		return ((0163 * physics_unit_size) / 10000) / 2;
	}
	constexpr int32_t getGravityMax() {
		return getGravityAcceleration() * 2;
	}

#ifdef REWIND_ENABLED
	void rewind(uint32_t ticksPassed) {
		if (ticksPassed > max_frames) {
			std::cout << "Error: PhysicsEngineAABB_MT.rewind()'s ticksPassed arg is higher than " << max_frames
				<< std::endl << "ticksPassed == " << ticksPassed << std::endl;
			std::cout << "Aborting..." << std::endl;
			throw;
		}
		for (uint32_t i = 0; i < ticksPassed; i++) {
			frames.get().rewind(*this);
			//bodies[0].printDebug();
			frames.rewind();
		}
	}
#endif

private:
	void setUserData(BodyID id, void* data) {
		userData[id.id] = data;
	}
	void setIsSolid(BodyID id, bool isTrue) {
		bodies[id.id].isSolid = isTrue;
	}

	void overlappingBodyPushIfUnique(uint32_t index, BodyID id) {
		for (uint32_t i = 0; i < overlappingBodyIDs[index].count; i++)
			if (overlappingBodyIDs[index][i].id == id.id)
				return;
		overlappingBodyIDs[index].push(id);
	}
	bool areAnyOverlappingBodiesSolid(uint32_t bodyIndex) {
		uint32_t overlappingCount = overlappingBodyIDs[bodyIndex].count;
		for (uint32_t j = 0; j < overlappingCount; j++) {
			BodyID otherBody = overlappingBodyIDs[bodyIndex][j];
			if (getIsSolid(otherBody) == true)
				return true;
		}
		return false;
	}
	void getOverlappingSolidBodies(uint32_t bodyIndex, FlatBuffer<BodyAABB*, 100>& out) {
		uint32_t overlappingCount = overlappingBodyIDs[bodyIndex].count;
		for (uint32_t j = 0; j < overlappingCount; j++) {
			BodyID otherBody = overlappingBodyIDs[bodyIndex][j];
			if (getIsSolid(otherBody) == true)
				out.push(&bodies[otherBody.id]);
		}
	}
	bool isBodyDynamic(BodyID bodyID) {
		if (bodyID.id >= max_dynamic_bodies)
			return false;
		return true;
	}
	bool isBodyStatic(BodyID bodyID) {
		if (bodyID.id >= max_dynamic_bodies)
			return true;
		return false;
	}
	__forceinline void gravity(BodyID bodyID) {
		//FixedPoint<physics_unit_size> acceleration = "0.0163f";
		int32_t acceleration = getGravityAcceleration();
		BodyAABB& body = bodies[bodyID.id];
		if ((int32_t)body.vel.y < getGravityMax() * 2)
			body.vel.y += acceleration * 2;
	}
};