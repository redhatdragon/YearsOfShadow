template<uint32_t width, uint32_t height, uint32_t depth, uint32_t hash_width, uint32_t max_bodies_per_hash = 64>
class SpatialHashTable {
	//FlatBuffer<BodyID, max_bodies_per_hash> hash[width * height * depth];
	// i'll miss you FlatBuffer ;-;
	//FlatBuffer<BodyID, max_bodies_per_hash> hash[width][height][depth];  //TODO: perf test this compared to manual flat variant..
	//std::vector<BodyID> hashf[width][height][depth];

	struct Bounds {
		Vec3D<uint32_t> pos, siz;
	};
	class SpatialHash {
		FlatBuffer<BodyID, max_bodies_per_hash> bodyIDs;
	public:
		inline void addBody(BodyID id) {
			if (bodyIDs.count == max_bodies_per_hash) {
				printf("SpatialHashTable::addBodyToHash - Error: too many bodies in hash!\n");
				//std::cout << "BodyID: " << id.id << " hash: " << x << ' ' << y << ' ' << z << '!' << std::endl;
				throw;
			}
			bodyIDs.push(id);
		}
		inline void removeBody(BodyID id) {
			uint32_t len = bodyIDs.count;
			for (uint32_t i = 0; i < len; i++) {
				if (bodyIDs[i].id == id.id) {
					bodyIDs[i] = bodyIDs[len - 1];
					bodyIDs.pop();
					return;
				}
			}
			std::cout << "SpatialHashTable::removeBodyFromHash - Error: body " << id.id << " not found in hash " << std::endl;
			//	<< x << ' ' << y << ' ' << z << '!' << std::endl;
			throw;
		}
		inline BodyID getBodyID(uint32_t index) {
			return bodyIDs[index];
		}
		inline uint32_t getCount() {
			return bodyIDs.count;
		}
	};

	SpatialHash(*hash)[width][height][depth];

	inline SpatialHash& getHash(uint32_t x, uint32_t y, uint32_t z) {
		return (*hash)[x][y][z];
	}

	inline void addBodyToHash(BodyID id, uint32_t x, uint32_t y, uint32_t z) {
		getHash(x, y, z).addBody(id);
	}
	void removeBodyFromHash(BodyID id, uint32_t x, uint32_t y, uint32_t z) {
		getHash(x, y, z).removeBody(id);
	}

	inline void getIterationBounds(const Vec3D<physics_fp>& pos, const Vec3D<physics_fp>& _siz, Bounds& bounds) {
		//uint32_t startRight = pos.x / hash_width, startDown = pos.y / hash_width;
		//uint32_t endRight = pos.x / hash_width + siz.x / hash_width, endDown = pos.y / hash_width + siz.y / hash_width;
		//uint32_t startForward = pos.z / hash_width, endForward = pos.z / hash_width + siz.z / hash_width;
		
		//bounds = {
		//	{
		//		pos.x / hash_width, pos.y / hash_width, pos.z / hash_width
		//	},
		//	{
		//		(pos.x + siz.x) / hash_width,
		//		(pos.y + siz.y) / hash_width,
		//		(pos.z + siz.z) / hash_width
		//	}
		//};
		//if (bounds.siz.x % hash_width == 0)
		//	bounds.siz.x--;
		//if (bounds.siz.y % hash_width == 0)
		//	bounds.siz.y--;
		//if (bounds.siz.z % hash_width == 0)
		//	bounds.siz.z--;

		Vec3D<physics_fp> siz = _siz;
		//if (siz.x.getRaw() > 0)
		//	siz.x.setRaw(siz.x.getRaw() - 1);
		//if (siz.y.getRaw() > 0)
		//	siz.y.setRaw(siz.y.getRaw() - 1);
		//if (siz.z.getRaw() > 0)
		//	siz.z.setRaw(siz.z.getRaw() - 1);

		bounds = {
			{(uint32_t)pos.x.getAsInt(), (uint32_t)pos.y.getAsInt(), (uint32_t)pos.z.getAsInt()}, 
			{
				(uint32_t)pos.x.getAsInt() + (uint32_t)siz.x.getAsInt(),
				(uint32_t)pos.y.getAsInt() + (uint32_t)siz.y.getAsInt(),
				(uint32_t)pos.z.getAsInt() + (uint32_t)siz.z.getAsInt()
			}
		};
        //if (bounds.siz.x.hasDecimal() == false)
        //    bounds.siz.x--;
        //if (bounds.siz.y.hasDecimal() == false)
        //    bounds.siz.y--;
        //if (bounds.siz.z.hasDecimal() == false)
        //    bounds.siz.z--;

		//return { {startRight, startDown, startForward}, {endRight, endDown, endForward} };
	}

public:
	void init() {
		//memset(this, 0, sizeof(*this));
		//uint64_t sizeOfHash = sizeof(FlatBuffer<BodyID, max_bodies_per_hash>);
		uint64_t sizeOfHash = sizeof(SpatialHash);
		sizeOfHash *= width;
		sizeOfHash *= height;
		sizeOfHash *= depth;
		//hash = (FlatBuffer<BodyID, max_bodies_per_hash>(*)[width][height][depth])malloc(sizeOfHash);
		hash = (SpatialHash(*)[width][height][depth])malloc(sizeOfHash);

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
	void addBody(BodyID id, const Vec3D<physics_fp>& pos, const Vec3D<physics_fp>& siz) {
		Bounds bounds;
		getIterationBounds(pos, siz, bounds);	
		for (uint32_t z = bounds.pos.z; z <= bounds.siz.z; z++)
			for (uint32_t y = bounds.pos.y; y <= bounds.siz.y; y++)
				for (uint32_t x = bounds.pos.x; x <= bounds.siz.x; x++)
					addBodyToHash(id, x, y, z);
	}
	void removeBody(BodyID id, const Vec3D<physics_fp>& pos, const Vec3D<physics_fp>& siz) {
		Bounds bounds;
		getIterationBounds(pos, siz, bounds);
		for (uint32_t z = bounds.pos.z; z <= bounds.siz.z; z++)
			for (uint32_t y = bounds.pos.y; y <= bounds.siz.y; y++)
				for (uint32_t x = bounds.pos.x; x <= bounds.siz.x; x++)
					removeBodyFromHash(id, x, y, z);
	}
	//__forceinline void getHashes(const Vec3D<physics_fp>& pos, const Vec3D<physics_fp>& siz, std::vector<SpatialHash*>& out) {
	//	//std::vector<std::vector<BodyID>*> returnValue = {};
    //    out.clear();
	//	Bounds bounds;
	//	getIterationBounds(pos, siz, bounds);
	//	for (uint32_t z = bounds.pos.z; z <= bounds.siz.z; z++)
	//		for (uint32_t y = bounds.pos.y; y <= bounds.siz.y; y++)
	//			for (uint32_t x = bounds.pos.x; x <= bounds.siz.x; x++)
	//				out.push_back(&getHash(x, y, z));
	//}
	// This was complicated and may need another look over
	inline void getBodyIDsInBox(const Vec3D<physics_fp>& pos, const Vec3D<physics_fp>& siz, std::vector<BodyID>& retValue) {
		Bounds bounds;
		getIterationBounds(pos, siz, bounds);
		//std::cout << bounds.siz.x - bounds.pos.x << ' ' <<
		//	bounds.siz.y - bounds.pos.y << ' ' <<
		//	bounds.siz.z - bounds.pos.z << std::endl;
		thread_local std::unordered_set<uint32_t> idMap;
		for (uint32_t z = bounds.pos.z; z <= bounds.siz.z; z++)
			for (uint32_t y = bounds.pos.y; y <= bounds.siz.y; y++)
				for (uint32_t x = bounds.pos.x; x <= bounds.siz.x; x++) {
					auto len = getHash(x, y, z).getCount();
					//if (len > 100) {
					//	std::cout << hash_width << std::endl;
					//	throw;
					//}
					//for (uint32_t i = 0; i < len; i++) {
					//	auto retValueLen = retValue.size();
					//	bool hasID = false;
					//	BodyID id = getHash(x,y,z).getBodyID(i);
					//	for (size_t j = 0; j < retValueLen; j++) {
					//		if (retValue[j].id == id.id) {
					//			hasID = true;
					//			break;
					//		}
					//	}
					//	if (hasID == false) {
					//		retValue.push_back(id);
					//	}
					//}
					//for (uint32_t i = 0; i < len; i++) {
					//	BodyID id = getHash(x, y, z).getBodyID(i);
					//	if (idMap.contains(id.id) == false) {
					//		idMap.insert(id.id);
					//		retValue.push_back(id);
					//	}
					//}
					for (uint32_t i = 0; i < len; i++) {
						BodyID id = getHash(x, y, z).getBodyID(i);
						std::vector<uint32_t>* retValueAs32 = reinterpret_cast<std::vector<uint32_t>*>(&retValue);
						auto it = std::find(retValueAs32->begin(), retValueAs32->end(), id.id);
						if (it == retValueAs32->end()) {
							retValue.push_back(id);
						}
					}
				}
		idMap.clear();
	}
	inline void getDynamicBodyIDsInBox(const Vec3D<physics_fp>& pos, const Vec3D<physics_fp>& siz, uint32_t dynamicBodyIndex, std::vector<BodyID>& retValue) {
		Bounds bounds;
		getIterationBounds(pos, siz, bounds);
		//std::cout << bounds.siz.x - bounds.pos.x << ' ' <<
		//	bounds.siz.y - bounds.pos.y << ' ' <<
		//	bounds.siz.z - bounds.pos.z << std::endl;
		thread_local std::unordered_set<uint32_t> idMap;
		for (uint32_t z = bounds.pos.z; z <= bounds.siz.z; z++)
			for (uint32_t y = bounds.pos.y; y <= bounds.siz.y; y++)
				for (uint32_t x = bounds.pos.x; x <= bounds.siz.x; x++) {
					auto len = getHash(x, y, z).getCount();
					for (uint32_t i = 0; i < len; i++) {
						BodyID id = getHash(x, y, z).getBodyID(i);
						if (id.id < dynamicBodyIndex)
							continue;
						std::vector<uint32_t>* retValueAs32 = reinterpret_cast<std::vector<uint32_t>*>(&retValue);
						auto it = std::find(retValueAs32->begin(), retValueAs32->end(), id.id);
						if (it == retValueAs32->end()) {
							retValue.push_back(id);
						}
					}
				}
		idMap.clear();
	}
	std::string getDbgStr() {
		uint64_t sizeOfHash = sizeof(FlatBuffer<BodyID, max_bodies_per_hash>);
		sizeOfHash *= width;
		sizeOfHash *= height;
		sizeOfHash *= depth;
		return std::to_string(sizeOfHash);
	}
};