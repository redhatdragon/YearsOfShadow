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
			std::cout << "BodyID: " << id.id << " hash: " << x << ' ' << y << ' ' << z << '!' << std::endl;
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
		std::cout << "SpatialHashTable::removeBodyFromHash - Error: body " << id.id << " not found in hash " 
			<< x << ' ' << y << ' ' << z << '!' << std::endl;
		throw;
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