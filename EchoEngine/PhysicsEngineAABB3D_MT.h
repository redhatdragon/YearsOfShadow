#pragma once

#include "Vec.h"
#include "FlatBuffer.h"
#include "DArray.h"
#include <time.h>

#ifndef physics_unit_size
#define physics_unit_size (256*256)  //used as a pretend "1" normalized value to emulate decimal
#else
// defined externally, reusing external define
#endif

#define physics_fp FixedPoint<physics_unit_size>

//TODO: Consider expanding for future utility.

struct BodyAABB {
	Vec3D<uint32_t> pos, siz, vel;
	uint8_t isSolid;

	__forceinline void simulate() {
		pos += vel;
	}
	__forceinline void reverseSimulate() {
		pos -= vel;
	}
	__forceinline bool collidesWith(const BodyAABB& other) const {
		if (pos.x + siz.x < other.pos.x || pos.x > other.pos.x + other.siz.x ||
			pos.y + siz.y < other.pos.y || pos.y > other.pos.y + other.siz.y ||
			pos.z + siz.z < other.pos.z || pos.z > other.pos.z + other.siz.z)
			return false;
		return true;
	}
	//Expects the input body to already be reverse simulated and was colliding with other before hand
	__forceinline Vec3D<uint32_t> resolveClosestSafePosition(const BodyAABB& other) {
		//const Vec3D<int32_t>& signedVel = *(Vec3D<int32_t>*)&vel;
		//Vec3D<uint32_t> futurePos = pos + vel;
		//Vec3D<uint32_t> closestPos = futurePos;
		//if (futurePos.x + siz.x > other.pos.x && futurePos.x < other.pos.x + other.siz.x) {
		//	if (signedVel.x < 0) {
		//		closestPos.x = other.pos.x + other.siz.x;
		//	} else if (signedVel.x > 0) {
		//		closestPos.x = other.pos.x - siz.x;
		//	}
		//}
		//if (futurePos.y + siz.y > other.pos.y && futurePos.y < other.pos.y + other.siz.y) {
		//	if (signedVel.y < 0) {
		//		closestPos.y = other.pos.y + other.siz.y;
		//	}
		//	else if (signedVel.y > 0) {
		//		closestPos.y = other.pos.y - siz.y;
		//	}
		//}
		//if (futurePos.z + siz.z > other.pos.z && futurePos.z < other.pos.z + other.siz.z) {
		//	if (signedVel.z < 0) {
		//		closestPos.z = other.pos.z + other.siz.z;
		//	}
		//	else if (signedVel.z > 0) {
		//		closestPos.z = other.pos.z - siz.z;
		//	}
		//}
		//return closestPos;

		Vec3D<uint32_t> savedPos = pos;
		constexpr uint32_t intervil = 100;
		Vec3D<int32_t> miniVel = *(Vec3D<int32_t>*) & vel / intervil;
		Vec3D<uint32_t> lastPos = pos;
		for (uint32_t i = 0; i < intervil; i++) {
			pos += *(Vec3D<uint32_t>*) & miniVel;
			if (collidesWith(other))
				break;
			lastPos = pos;
		}
		pos = savedPos;
		return lastPos;
	}
	__forceinline Vec3D<uint32_t> resolveClosestSafePosition(BodyAABB** others, uint32_t count) {
		Vec3D<uint32_t> closestPos = resolveClosestSafePosition(*others[0]);
		for (uint32_t i = 1; i < count; i++) {
			Vec3D<uint32_t> curPos = resolveClosestSafePosition(*others[i]);
			Vec3D<uint32_t> dist = curPos - pos;
			if (dist.getDistanceSquared() < (closestPos - pos).getDistanceSquared())
				closestPos = curPos;
		}
		return closestPos;
	}

	void printDebug() const {
		std::cout << ((physics_fp*)&pos.x)->getAsFloat() << ' '
			<< ((physics_fp*)&pos.y)->getAsFloat() << ' ' <<
			((physics_fp*)&pos.z)->getAsFloat() << std::endl;
		std::cout << ((physics_fp*)&siz.x)->getAsFloat() << ' '
			<< ((physics_fp*)&siz.y)->getAsFloat() << ' ' <<
			((physics_fp*)&siz.z)->getAsFloat() << std::endl;
	}
};
constexpr uint32_t sizeofBodyAABB = sizeof(BodyAABB);

struct BodyID {
	uint32_t id;
	bool operator==(BodyID other) {
		if (id == other.id)
			return true;
		return false;
	}
	bool operator!=(BodyID other) {
		if (id != other.id)
			return true;
		return false;
	}
};

template<uint32_t width, uint32_t height, uint32_t depth, uint32_t hash_width, uint32_t max_bodies_per_hash = 64>
class SpatialHashTable {
	//FlatBuffer<BodyID, max_bodies_per_hash> hash[width * height * depth];
	// i'll miss you FlatBuffer ;-;
	//FlatBuffer<BodyID, max_bodies_per_hash> hash[width][height][depth];  //TODO: perf test this compared to manual flat variant..
	//std::vector<BodyID> hashf[width][height][depth];

	struct Bounds {
		Vec3D<uint32_t> pos, siz;
	};

	FlatBuffer<BodyID, max_bodies_per_hash>(*hash)[width][height][depth];

	inline FlatBuffer<BodyID, max_bodies_per_hash>& getHash(uint32_t x, uint32_t y, uint32_t z) {
		return (*hash)[x][y][z];
	}

	inline void addBodyToHash(BodyID id, uint32_t x, uint32_t y, uint32_t z) {
		//if (x == 1 && y == 157 && z == 1)
		//	throw;
		if (getHash(x, y, z).count == max_bodies_per_hash) {
			printf("SpatialHashTable::addBodyToHash - Error: too many bodies in hash!\n");
			throw;
		}
		getHash(x, y, z).push(id);
	}
	void removeBodyFromHash(BodyID id, uint32_t x, uint32_t y, uint32_t z) {
		uint32_t len = getHash(x, y, z).count;
		for (uint32_t i = 0; i < len; i++) {
			if (getHash(x, y, z)[i].id == id.id) {
				getHash(x, y, z)[i] = getHash(x, y, z)[len - 1];
				getHash(x, y, z).pop();
				return;
			}
		}
		printf("SpatialHashTable::removeBodyFromHash - Error: body not found in hash!\n");
	}

	inline void getIterationBounds(const Vec3D<uint32_t>& pos, const Vec3D<uint32_t>& siz, Bounds& bounds) {
		//uint32_t startRight = pos.x / hash_width, startDown = pos.y / hash_width;
		//uint32_t endRight = pos.x / hash_width + siz.x / hash_width, endDown = pos.y / hash_width + siz.y / hash_width;
		//uint32_t startForward = pos.z / hash_width, endForward = pos.z / hash_width + siz.z / hash_width;
		bounds = {
			{
				pos.x / hash_width, pos.y / hash_width, pos.z / hash_width
			},
			{
				(pos.x + siz.x) / hash_width,
				(pos.y + siz.y) / hash_width,
				(pos.z + siz.z) / hash_width
			}
		};
		if (bounds.siz.x % hash_width == 0)
			bounds.siz.x--;
		if (bounds.siz.y % hash_width == 0)
			bounds.siz.y--;
		if (bounds.siz.z % hash_width == 0)
			bounds.siz.z--;

		//return { {startRight, startDown, startForward}, {endRight, endDown, endForward} };
	}

public:
	void init() {
		//memset(this, 0, sizeof(*this));
		uint64_t sizeOfHash = sizeof(FlatBuffer<BodyID, max_bodies_per_hash>);
		sizeOfHash *= width;
		sizeOfHash *= height;
		sizeOfHash *= depth;
		hash = (FlatBuffer<BodyID, max_bodies_per_hash>(*)[width][height][depth])malloc(sizeOfHash);
		//hash = new FlatBuffer<BodyID, max_bodies_per_hash>[width][height][depth];
		//for(uint32_t z = 0; z < depth; z++)
		//	for(uint32_t y = 0; y < height; y++)
		//		for (uint32_t x = 0; x < width; x++) {
		//			//hash[x][y][z].reserve(max_bodies_per_hash);
		//			//hash[x][y][z].clear();
		//			getHash(x, y, z).clear();
		//		}
		memset(hash, 0, sizeOfHash);
	}
	void addBody(BodyID id, const Vec3D<uint32_t>& pos, const Vec3D<uint32_t>& siz) {
		Bounds bounds;
		getIterationBounds(pos, siz, bounds);
		for (uint32_t z = bounds.pos.z; z <= bounds.siz.z; z++)
			for (uint32_t y = bounds.pos.y; y <= bounds.siz.y; y++)
				for (uint32_t x = bounds.pos.x; x <= bounds.siz.x; x++)
					addBodyToHash(id, x, y, z);
	}
	void removeBody(BodyID id, const Vec3D<uint32_t>& pos, const Vec3D<uint32_t>& siz) {
		Bounds bounds;
		getIterationBounds(pos, siz, bounds);
		for (uint32_t z = bounds.pos.z; z <= bounds.siz.z; z++)
			for (uint32_t y = bounds.pos.y; y <= bounds.siz.y; y++)
				for (uint32_t x = bounds.pos.x; x <= bounds.siz.x; x++)
					removeBodyFromHash(id, x, y, z);
	}
	//TODO: consider making this return through param as to potentially allow .reserve() before calling.
	__forceinline std::vector<FlatBuffer<BodyID, max_bodies_per_hash>*> getHashes(const Vec3D<uint32_t>& pos, const Vec3D<uint32_t>& siz) {
		std::vector<FlatBuffer<BodyID, max_bodies_per_hash>*> returnValue = {};
		//std::vector<std::vector<BodyID>*> returnValue = {};
		Bounds bounds;
		getIterationBounds(pos, siz, bounds);
		for (uint32_t z = bounds.pos.z; z <= bounds.siz.z; z++)
			for (uint32_t y = bounds.pos.y; y <= bounds.siz.y; y++)
				for (uint32_t x = bounds.pos.x; x <= bounds.siz.x; x++)
					returnValue.push_back(&getHash(x, y, z));
		return returnValue;
	}
	// This was complicated and may need another look over
	inline void getIDs(const Vec3D<uint32_t>& pos, const Vec3D<uint32_t>& siz, std::vector<BodyID>& returnValue) {
		Bounds bounds;
		getIterationBounds(pos, siz, bounds);
		for (uint32_t z = bounds.pos.z; z <= bounds.siz.z; z++)
			for (uint32_t y = bounds.pos.y; y <= bounds.siz.y; y++)
				for (uint32_t x = bounds.pos.x; x <= bounds.siz.x; x++) {
					auto len = getHash(x, y, z).count;
					//if (len > 100) {
					//	std::cout << hash_width << std::endl;
					//	throw;
					//}
					for (uint32_t i = 0; i < len; i++) {
						auto retValueLen = returnValue.size();
						bool hasID = false;
						for (size_t j = 0; j < retValueLen; j++) {
							if (returnValue[j].id == getHash(x, y, z)[i].id) {
								hasID = true;
								break;
							}
						}
						if (hasID == false) {
							returnValue.push_back(getHash(x, y, z)[i]);
						}

					}
				}
		//if (clock() - c > 2)  //Why?
		//	throw;
	}
	std::string getDbgStr() {
		uint64_t sizeOfHash = sizeof(FlatBuffer<BodyID, max_bodies_per_hash>);
		sizeOfHash *= width;
		sizeOfHash *= height;
		sizeOfHash *= depth;
		return std::to_string(sizeOfHash);
	}
};

template<uint32_t width, uint32_t height, uint32_t depth, uint32_t hash_width, uint32_t max_bodies_per_hash = 64>
class PhysicsEngineAABB3D {
public:
	static constexpr uint32_t max_dynamic_bodies = 100000;  //100k
	static constexpr uint32_t max_static_bodies = 1000000;  //1mill
	static constexpr uint32_t max_bodies = max_dynamic_bodies + max_static_bodies;
	//FlatFlaggedBuffer<BodyAABB, max_bodies> bodies = FlatFlaggedBuffer<BodyAABB, max_bodies>();
	DArray<BodyAABB> bodies;
	uint32_t dynamicBodyCount;
	uint32_t staticBodyCount;  //mostly for debug purposes
	//uint32_t earliestEmptyStaticIndex = max_dynamic_bodies;
	FlatFlagBuffer<max_bodies> isValid;
	//FlatBuffer<void*, max_bodies> userData;
	DArray<void*> userData;
	//FlatBuffer<FlatBuffer<BodyID, 100>, max_bodies> overlappingBodyIDs;
	DArray<FlatBuffer<BodyID, 10>> overlappingBodyIDs;
	SpatialHashTable<width, height, depth, hash_width* physics_unit_size, max_bodies_per_hash> spatialHashTable;
	float timeFromStepping = 0;

	void init() {
		timeFromStepping = dynamicBodyCount = staticBodyCount = 0;
		bodies.init(max_bodies);
		//overlappingBodyIDs.reserve(max_bodies);
		//for (uint64_t i = 0; i < max_bodies; i++) {
		//	//overlappingBodyIDs[i].init(100);
		//	overlappingBodyIDs.push_back({});
		//}
		overlappingBodyIDs.init(max_bodies);
		for (uint32_t i = 0; i < max_bodies; i++) {
			overlappingBodyIDs[i].clear();
		}
		userData.init(max_bodies);
		spatialHashTable.init();
	}
	BodyID addBodyBox(physics_fp x, physics_fp y, physics_fp z,
		physics_fp w, physics_fp h, physics_fp d) {
		//x *= physics_unit_size; y *= physics_unit_size; z *= physics_unit_size;
		//w *= physics_unit_size; h *= physics_unit_size; d *= physics_unit_size;
		BodyID retValue;
		//BodyAABB body = { { x,y,z }, { w,h,d }, { 0,0,0 } };
		BodyAABB body = { { x.getRaw(),y.getRaw(),z.getRaw() }, { w.getRaw(),h.getRaw(),d.getRaw() }, { 0,0,0 } };
		//retValue.id = bodies.insert(body);
		retValue.id = isValid.toggleFirstInvalid();
		bodies[retValue.id] = body;
		spatialHashTable.addBody(retValue, body.pos, body.siz);
		overlappingBodyIDs[retValue.id].clear();
		dynamicBodyCount++;
		return retValue;
	}
	BodyID addStaticBodyBox(physics_fp x, physics_fp y, physics_fp z,
		physics_fp w, physics_fp h, physics_fp d) {
		BodyID retValue;
		BodyAABB body = { { x.getRaw(),y.getRaw(),z.getRaw() }, { w.getRaw(),h.getRaw(),d.getRaw() }, { 0,0,0 } };
		retValue.id = isValid.toggleFirstInvalid(max_dynamic_bodies);
		bodies[retValue.id] = body;
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
		//bodies.decrementCount();
		//bodies.setInvalid(bodyID.id);
		isValid.setInvalid(bodyID.id);
	}

	void* getUserData(BodyID id) {
		return userData[id.id];
	}
	void setUserData(BodyID id, void* data) {
		userData[id.id] = data;
	}

	FlatBuffer<BodyID, 100>& getOverlappingBodies(BodyID id) {
		return overlappingBodyIDs[id.id];
	}

	inline std::vector<BodyID> getBodiesInRectRough(Vec3D<physics_fp> pos, Vec3D<physics_fp> siz) {
		std::vector<BodyID> retValue;
		auto hashes = spatialHashTable.getHashes(*(Vec3D<uint32_t>*)&pos, *(Vec3D<uint32_t>*)&siz);
		retValue.reserve(hashes.size());
		uint32_t hashCount = hashes.size();
		for (uint32_t i = 0; i < hashCount; i++) {
			auto* hashPtr = hashes[i];
			uint32_t hashSize = hashPtr->count;
			for (uint32_t j = 0; j < hashSize; j++) {
				BodyID bodyID = (*hashPtr)[j];
				uint32_t totalBodyCount = retValue.size();
				for (uint32_t k = 0; k < totalBodyCount; k++) {
					if (retValue[k] == bodyID)
						goto skip;
				}
				retValue.push_back(bodyID);
			skip:
				continue;
			}
		}

		//for (auto* hash : hashes) {
		//	uint32_t hashSize = hash->count;
		//	for (uint32_t i = 0; i < hashSize; i++) {
		//		BodyID bodyID = (*hash)[i];
		//		register uint32_t count = retValue.size();
		//		for (uint32_t j = 0; j < count; j++)
		//			if (retValue[j] == bodyID) goto skip;
		//		retValue.push_back(bodyID);
		//	skip:
		//		continue;
		//	}
		//}

		return retValue;
	}
	inline std::vector<BodyID> getBodiesInRectRough(Vec3D<uint32_t> pos, Vec3D<uint32_t> siz) {
		pos *= physics_unit_size; siz *= physics_unit_size;
		return getBodiesInRectRough(*(Vec3D<physics_fp>*)&pos, *(Vec3D<physics_fp>*)&siz);
	}
	inline FlatBuffer<BodyID, 10> getBodiesInPoint(Vec3D<FixedPoint<256 * 256>> pos, BodyID ignoredBody) {
		FlatBuffer<BodyID, 10> retValue = {};
		std::vector<BodyID> out = {};
		//pos *= physics_unit_size;
		Vec3D<uint32_t> siz = { 1, 1, 1 };
		//siz *= physics_unit_size;
		spatialHashTable.getIDs(*(Vec3D<uint32_t>*) & pos, siz, out);
		for (uint32_t i = 0; i < out.size(); i++)
			if (out[i] != ignoredBody)
				retValue.push(out[i]);
		return retValue;
	}
	inline bool pointTrace(const Vec3D<FixedPoint<256 * 256>>& pos, BodyID ignoredBody) {
		std::vector<BodyID> out = {};
		//pos *= physics_unit_size;
		Vec3D<uint32_t> siz = { 1, 1, 1 };
		//siz *= physics_unit_size;
		spatialHashTable.getIDs(*(Vec3D<uint32_t>*) & pos, siz, out);
		uint32_t size = out.size();
		for (uint32_t i = 0; i < size; i++)
			if (out[i] != ignoredBody)
				return true;
		return false;
	}

	/*void addVelocity(BodyID id, float vx, float vy, float vz) {
		BodyAABB* body = &bodies[id.id];
		body->vel.x += vx * physics_unit_size;
		body->vel.y += vy * physics_unit_size;
		body->vel.z += vz * physics_unit_size;
	}
	void setVelocity(BodyID id, float vx, float vy, float vz) {
		BodyAABB* body = &bodies[id.id];
		body->vel.x = vx * physics_unit_size;
		body->vel.y = vy * physics_unit_size;
		body->vel.z = vz * physics_unit_size;
	}*/
	void addVelocity(BodyID id, physics_fp vx, physics_fp vy, physics_fp vz) {
		BodyAABB* body = &bodies[id.id];
		body->vel.x += vx;
		body->vel.y += vy;
		body->vel.z += vz;
	}
	void setVelocity(BodyID id, physics_fp vx, physics_fp vy, physics_fp vz) {
		BodyAABB* body = &bodies[id.id];
		body->vel.x = vx.getRaw();
		body->vel.y = vy.getRaw();
		body->vel.z = vz.getRaw();
	}
	Vec3D<physics_fp> getVelocity(BodyID id) {
		BodyAABB* body = &bodies[id.id];
		return *(Vec3D<physics_fp>*) & body->vel;
	}

	bool getSolid(BodyID id) {
		return bodies[id.id].isSolid;
	}
	void setSolid(BodyID id, bool isTrue) {
		bodies[id.id].isSolid = isTrue;
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
		//uint32_t bodyCount = bodies.getCount();
		uint32_t bodyCount = dynamicBodyCount;
		if (bodyCount == 0) return;
		uint32_t validBodyCount = 0;
		for (uint32_t i = 0; true; i++) {
			//if (bodies.getIsValid(i) == false)
			if (isValid.getIsValid(i) == false)
				continue;
			validBodyCount++;
			//if (bodies[i].vel.isZero()) {
			//	if (validBodyCount >= bodyCount) {
			//		lastBodyIndex = i;
			//		break;
			//	}
			//	continue;
			//}
			spatialHashTable.removeBody({ i }, bodies[i].pos, bodies[i].siz);
			bodies[i].simulate();
			spatialHashTable.addBody({ i }, bodies[i].pos, bodies[i].siz);
			if (validBodyCount >= bodyCount) {
				lastBodyIndex = i;
				break;
			}
		}
	}
	struct DetectThreadData {
		void* self;
		uint32_t start, end;
		std::vector<BodyID>* IDs;
	};
	inline void detect() {
		uint32_t bodyCount = dynamicBodyCount;
		if (bodyCount == 0) return;
		for (uint32_t i = 0; i <= lastBodyIndex; i++) {
			if (isValid.getIsValid(i) == false)
				continue;
			overlappingBodyIDs[i].clear();
		}

		static std::vector<BodyID> IDses[256];

		uint16_t threadCount = EE_getThreadPoolFreeThreadCount(threadPool);
		//threadCount--;
		//threadCount = 1;
		static std::vector<DetectThreadData> dtd;
		dtd.clear();
		dtd.reserve(threadCount);
		//uint32_t totalWork = lastBodyIndex - 1;
		uint32_t totalWork = lastBodyIndex + 1;
		uint32_t workPerThread = totalWork / threadCount;
		uint32_t leftover = totalWork % threadCount;
		for (uint32_t i = 0; i < threadCount; i++) {
			u32 start = workPerThread * i; u32 end = start + workPerThread - 1;
			//u32 start = workPerThread * i; u32 end = start + workPerThread;
			DetectThreadData currentDTD = { this, start, end, &IDses[i]};
			dtd.push_back(currentDTD);
			//std::cout << start << ' ' << end << std::endl;
		}
		for (uint32_t i = 0; i < threadCount; i++) {
			EE_sendThreadPoolTask(threadPool, detectThreadBody, &dtd[i]);
			//detectThreadBody(&dtd[i]);
		}
		static DetectThreadData lastDTD;
		u32 start = workPerThread * threadCount; u32 end = start + leftover;
		lastDTD = { this, start, end, &IDses[threadCount]};
		//std::cout << start << ' ' << end << std::endl;
		if (leftover)
			detectThreadBody(&lastDTD);
		while (EE_isThreadPoolFinished(threadPool) == false)
			continue;
		//_sleep(1);
	}
	static void detectThreadBody(void* _data) {
		DetectThreadData* data = (DetectThreadData*)_data;
		PhysicsEngineAABB3D<width, height, depth, hash_width, max_bodies_per_hash>* self =
			(PhysicsEngineAABB3D<width, height, depth, hash_width, max_bodies_per_hash>*)data->self;

		std::vector<BodyID>& IDs = *data->IDs;
		//IDs.reserve(2000);
		//for (u32 i = data->start; i <= data->end; i++) {
		//std::cout << data->start << "  " << data->end << std::endl;
		for (uint32_t i = data->start; i <= data->end; i++) {
			if (self->isValid.getIsValid(i) == false)
				continue;
			IDs.clear();
			const BodyAABB& b = self->bodies[i];
			self->spatialHashTable.getIDs(b.pos, b.siz, IDs);
			size_t IDCount = IDs.size();
			//b.printDebug();
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
			bool isSolid = getSolid({ i });
			if (overlappingBodyIDs[i].count && bodies[i].vel.isZero() == false && getSolid({ i })
				//&& areAnyOverlappingBodiesSolid(i) == true) {
				) {

				//spatialHashTable.removeBody({ i }, bodies[i].pos, bodies[i].siz);
				//bodies[i].reverseSimulate();
				//spatialHashTable.addBody({ i }, bodies[i].pos, bodies[i].siz);

				static FlatBuffer<BodyAABB*, 100> overlappingSolidBodies = {};
				overlappingSolidBodies.clear();
				getOverlappingSolidBodies(i, overlappingSolidBodies);
				if (overlappingSolidBodies.count) {

					spatialHashTable.removeBody({ i }, bodies[i].pos, bodies[i].siz);
					bodies[i].reverseSimulate();
					auto newPos = bodies[i].resolveClosestSafePosition(&overlappingSolidBodies[0],
						overlappingSolidBodies.count);
					bodies[i].pos = newPos;
					//bodies[i].vel = { 0 };
					*(Vec3D<int32_t>*)& bodies[i].vel /= 2;
					spatialHashTable.addBody({ i }, bodies[i].pos, bodies[i].siz);
				}
			}
			gravity({ i });
		}
	}

	void tick() {
		clock_t c = clock();
		simulate();
		detect();
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

	//private:
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
			if (getSolid(otherBody) == true)
				return true;
		}
		return false;
	}
	void getOverlappingSolidBodies(uint32_t bodyIndex, FlatBuffer<BodyAABB*, 100>& out) {
		uint32_t overlappingCount = overlappingBodyIDs[bodyIndex].count;
		for (uint32_t j = 0; j < overlappingCount; j++) {
			BodyID otherBody = overlappingBodyIDs[bodyIndex][j];
			if (getSolid(otherBody) == true)
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