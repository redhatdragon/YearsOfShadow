#pragma once
#include <memory.h>



struct VoxelBlockMetaData {
	uint16_t maxHealth;
};
VoxelBlockMetaData voxelBlockMetaData[256];

//NOTE: Excactly 4 bytes large in 'usage'.
//WARNING: STORING A DIRECT BUFFER OF THIS TYPE MAY CAUSE UB!!!
//Store a buffer of uint32_t instead.
struct VoxelBlock {
	uint8_t typeID;  //Used to index into a buffer of VoxelBlockMetaData
	uint8_t  padding;  //Reserved for later...
	uint16_t damage;

	inline uint8_t isAir() const { return !typeID; }
	inline uint8_t isNotAir() const { return typeID; }
	inline const VoxelBlockMetaData& getMetaData() const {
		return voxelBlockMetaData[typeID];
	}
};

template<uint32_t width = 16, uint32_t height = 256, uint32_t depth = 16>
class VoxelChunk {
	struct BlockPos {
		uint8_t x, y, z;
	};
	struct BlockBody {
		uint8_t x, y, z;
		uint16_t w, h, d;
	};
	uint32_t worldOffsetX, worldOffsetY, worldOffsetZ;
	//NOTE: Each element is meant to be cast to VoxelBlock.
	//This just ensures across all compilers that struct padding not intefere.
	uint32_t blocks[width][height][depth];
	FlatBuffer<BlockPos, width * height * depth> drawableBlocks;
	bool hasBody[width][height][depth];
	FlatBuffer<BodyID, width* height* depth> activeBodies;

	void* blockMesh;
	bool modified;
public:
	//static constexpr uint32_t width = 16, height = 256, depth = 16;

	void init(uint32_t _x, uint32_t _y, uint32_t _z) {
		memset(this, 0, sizeof(*this));
		modified = true;
		worldOffsetX = _x; worldOffsetY = _y; worldOffsetZ = _z;
		//std::string path = EE_getDirData();
		std::string path = "";
		//path += "Meshes/Cube.obj";
		//path += "Meshes/Cube2.fbx/cube.001.mesh";
		//path += "./Data/Meshes/Props/Dynamite.obj";
		//blockMesh = EE_getNewInstancedMesh(path.c_str());
		//EE_setInstancedMeshScale(blockMesh, { 50, 50, 50 });
		//EE_setInstancedSubmeshTexture(blockMesh, 0, "albedo", "Textures/Grass1.png");
		//EE_setInstancedSubmeshTexture(blockMesh, 0, "normal", "Textures/Grass1_n.png");

		//for (uint32_t z = 0; z < depth; z++)
		//	for (uint32_t y = 0; y < height; y++)
		//		for (uint32_t x = 0; x < width; x++) {
		//			VoxelBlock block; block.typeID = 0; block.damage = 0;
		//			setBlock(block, x, y, z);
		//		}
		//activeBodies.clear();
	}

	void gen(uint32_t genHeight) {
		modified = true;
		for (uint32_t z = 0; z < depth; z++)
			for (uint32_t y = 0; y < genHeight; y++)
				for (uint32_t x = 0; x < width; x++) {
					VoxelBlock block; block.typeID = 1; block.damage = 0;
					setBlock(block, x, (height-1)-y, z);
				}

		rebuild();
	}

	inline const VoxelBlock& getBlock(uint8_t x, uint8_t y, uint8_t z) const {
		return *(VoxelBlock*)&blocks[x][y][z];
	}
	inline void setBlock(const VoxelBlock& _block, uint8_t x, uint8_t y, uint8_t z) {
		VoxelBlock& block = (VoxelBlock&)getBlock(x, y, z);
		block = _block;
		modified = true;
	}
	inline void destroyBlock(uint8_t x, uint8_t y, uint8_t z) {
		VoxelBlock& block = (VoxelBlock&)getBlock(x, y, z);
		block.typeID = 0;
		modified = true;
	}

	void display() {
		if (modified == false)
			return;
		rebuild();
		modified = false;
	}

	void freshBuildPhysics() {
		uint32_t count = drawableBlocks.count;
		for (uint32_t i = 0; i < count; i++) {
			BlockPos rootBlockPos = drawableBlocks[i];
			if (hasBody[rootBlockPos.x][rootBlockPos.y][rootBlockPos.z])
				continue;
			uint32_t xplus = 0;
			for (uint32_t i = rootBlockPos.x; i < width; i++) {
				//if (isInBounds(i, rootBlockPos.y, rootBlockPos.z) == false)
				if(isVisible(i, rootBlockPos.y, rootBlockPos.z) == false)
					break;
				if (hasBody[i][rootBlockPos.y][rootBlockPos.z])
					break;
				xplus++;
			}
			uint32_t xminus = 0;
			for (uint32_t i = rootBlockPos.x; i >= 0; i--) {
				//if (i == -1) break;
				if (i > 15) break;
				//if (isInBounds(i, rootBlockPos.y, rootBlockPos.z) == false)
				if (isVisible(i, rootBlockPos.y, rootBlockPos.z) == false)
					break;
				if (hasBody[i][rootBlockPos.y][rootBlockPos.z])
					break;
				xminus++;
			}
			uint32_t yplus = 0;
			for (uint32_t i = rootBlockPos.y; i < height; i++) {
				//if (isInBounds(rootBlockPos.x, i, rootBlockPos.z) == false)
				if (isVisible(rootBlockPos.x, i, rootBlockPos.z) == false)
					break;
				if (hasBody[rootBlockPos.x][i][rootBlockPos.z])
					break;
				yplus++;
			}
			uint32_t yminus = 0;
			for (uint32_t i = rootBlockPos.y; i >= 0; i--) {
				//if (i == -1) break;
				if (i > 255) break;
				//if (isInBounds(rootBlockPos.x, i, rootBlockPos.z) == false)
				if (isVisible(rootBlockPos.x, i, rootBlockPos.z) == false)
					break;
				if (hasBody[rootBlockPos.x][i][rootBlockPos.z])
					break;
				yminus++;
			}

			if (xplus >= xminus && xplus >= yplus && xplus >= yminus) {
				BlockBody body = { rootBlockPos.x, rootBlockPos.y, rootBlockPos.z,
				xplus, 1, 1};
				spawnBody(body);
				continue;
			}
			if (xminus >= xplus && xminus >= yplus && xminus >= yminus) {
				BlockBody body = { rootBlockPos.x - xminus, rootBlockPos.y, rootBlockPos.z,
				xminus, 1, 1 };
				spawnBody(body);
				continue;
			}
			if (yplus >= xplus && yplus >= xminus && yplus >= yminus) {
				BlockBody body = { rootBlockPos.x, rootBlockPos.y, rootBlockPos.z,
				1, yplus, 1 };
				spawnBody(body);
				continue;
			}
			if (yminus >= xplus && yminus >= xminus && yminus >= yplus) {
				BlockBody body = { rootBlockPos.x, rootBlockPos.y - yminus, rootBlockPos.z,
				1, yminus, 1 };
				spawnBody(body);
				continue;
			}
		}
		for (uint32_t i = 0; i < count; i++) {
			BlockPos pos = drawableBlocks[i];
			if (hasBody[pos.x][pos.y][pos.z])
				continue;
			BlockBody body = { pos.x, pos.y, pos.z, 1, 1, 1 };
			spawnBody(body);
		}
	}

	void spawnPhysics() {
		freshBuildPhysics();

		//for (uint32_t i = 0; i < count; i++) {
		//	BlockPos pos = drawableBlocks[i];
		//	spawnBody({pos.x, pos.y, pos.z, 1, 1, 1});
		//}
	}

	void rebuild() {
		drawableBlocks.count = 0;
		modified = true;

		memset(hasBody, 0, sizeof(hasBody));
		//for (uint32_t z = 0; z < depth; z++)
		//	for (uint32_t y = 0; y < height; y++)
		//		for (uint32_t x = 0; x < width; x++)
		//			hasBody[x][y][z] = false;
		//memset(requiresBody, 0, sizeof(requiresBody));
		{
			uint32_t bodyCount = activeBodies.count;
			for (uint32_t i = 0; i < bodyCount; i++)
				physics.removeBody(activeBodies[i]);
			activeBodies.clear();
		}

		for (uint32_t z = 0; z < depth; z++)
			for (uint32_t y = 0; y < height; y++)
				for (uint32_t x = 0; x < width; x++) {
					if (isVisible(x, y, z)) {
						drawableBlocks.push({ (uint8_t)x, (uint8_t)y, (uint8_t)z });
						//requiresBody[x][y][z] = true;
					}
				}
		spawnPhysics();

		uint32_t count = drawableBlocks.count;
		std::vector<EE_Point3Df> positions;
		positions.reserve(count);
		for (uint32_t i = 0; i < count; i++) {
			BlockPos& blockPos = drawableBlocks[i];
			EE_Point3Df pos = {
				(float)blockPos.x + worldOffsetX,
				(float)blockPos.y + worldOffsetY,
				(float)blockPos.z + worldOffsetZ
			};
			pos.x += 0.5f; pos.y += 0.5f; pos.z += 0.5f;
			positions.push_back(pos);
		}
		//EE_setInstancedMeshPositions(blockMesh, &positions[0], positions.size());
	}
	void unload() {
		if (drawableBlocks.count == 0)
			return;
		drawableBlocks.count = 0;
		modified = true;

		memset(hasBody, 0, sizeof(hasBody));
		//memset(requiresBody, 0, sizeof(requiresBody));
		{
			//uint32_t bodyCount = activeBodies.count;
			//for (uint32_t i = 0; i < bodyCount; i++)
			//	physics.removeBody(activeBodies[i]);
			//activeBodies.clear();
		}
		//EE_setInstancedMeshPositions(blockMesh, NULL, NULL);
	}
private:
	inline bool isVisible(uint8_t x, uint8_t y, uint8_t z) {
		if (isAir(x, y, z) == false && hasNeighboringAir(x, y, z) == true)
			return true;
		return false;
	}
	inline void spawnBody(const BlockBody& blockBody) {
		//hasBody[blockBody.x][blockBody.y][blockBody.z] = true;

		for(uint32_t i = blockBody.z; i < blockBody.z + blockBody.d; i++)
			for (uint32_t j = blockBody.y; j < blockBody.y + blockBody.h; j++)
				for (uint32_t k = blockBody.x; k < blockBody.x + blockBody.w; k++) {
					hasBody[k][j][i] = true;
				}
		auto bodyID = physics.addStaticBodyBox(
			(uint32_t)blockBody.x + worldOffsetX,
			//(uint32_t)worldOffsetY + (height * 1 - (blockBody.y * 1)),
			(uint32_t)blockBody.y + worldOffsetY,
			//(uint32_t)blockBody.y + 1 + worldOffsetY,
			(uint32_t)blockBody.z + worldOffsetZ,
			//(uint32_t)blockBody.z + worldOffsetZ - 1,
			blockBody.w, blockBody.h, blockBody.d
		);
		physics.setSolid(bodyID, true);
		physics.setUserData(bodyID, (void*)-1);
		if (bodyID.id > 10000000)
			throw;
		activeBodies.push(bodyID);
	}
	inline bool hasNeighboringAir(uint32_t x, uint32_t y, uint32_t z) const {
		if (x == 0)
			return true;
		if (isAir(x - 1, y, z))
			return true;
		if (x == width - 1)
			return true;
		if (isAir(x + 1, y, z))
			return true;

		if (y == 0)
			return true;
		if (isAir(x, y - 1, z))
			return true;
		if (y == height - 1)
			return true;
		if (isAir(x, y + 1, z))
			return true;

		if (z == 0)
			return true;
		if (isAir(x, y, z - 1))
			return true;
		if (z == depth - 1)
			return true;
		if (isAir(x, y, z + 1))
			return true;

		return false;
	}
	inline bool isAir(uint8_t x, uint8_t y, uint8_t z) const {
		return getBlock(x, y, z).isAir();
	}
	inline bool isInBounds(uint32_t x, uint32_t y, uint32_t z) {
		if (x > 15 || y > 255 || z > 15)
			return false;
		return true;
	}
};

//worldSize is the number or chunks wide for the x as well as z axis.
template<uint32_t worldSize = 1, uint32_t width = 16, uint32_t height = 256, uint32_t depth = 16>
class VoxelWorld {
	VoxelChunk<width, height, depth> chunks[worldSize][worldSize];
	//bool needsRebuilding[worldSize][worldSize];
public:
	void init(uint32_t genHeight, uint32_t x, uint32_t y, uint32_t z) {
		memset(this, 0, sizeof(*this));
		for (uint32_t i = 0; i < worldSize; i++)
			for (uint32_t j = 0; j < worldSize; j++) {
				//float dx = x + i * width, dy = y, dz = z + j * depth;
				float dx = x + i * width, dy = 0, dz = z + j * depth;
				chunks[i][j].init(dx, dy, dz);
				chunks[i][j].gen(genHeight);
			}
		//rebuild();
	}

	void display(uint32_t x, uint32_t z) {
		uint32_t chunksWide = 9;
		Vec2D<int32_t> start, end;
		start = { (int32_t)x, (int32_t)z };
		end = start;
		start -= (chunksWide / 2) * width;
		end += (chunksWide / 2) * width;
		for (uint32_t i = 0; i < worldSize; i++) {
			for (uint32_t j = 0; j < worldSize; j++) {
				Vec2D<int32_t> curPos = {j * width, i * width};
				if (curPos.isBetween(start, end))
					chunks[j][i].display();
				else
					chunks[j][i].unload();
			}
		}
	}

	//Checks within rectangle, top left corner is origin
	std::vector<Vec3D<uint32_t>> getValidBlockPositions(Vec3D<uint32_t> pos, Vec3D<uint32_t> siz) {
		std::vector<Vec3D<uint32_t>> retValue = {};
		retValue.reserve(siz.x * siz.y * siz.z);
		for (uint32_t z = pos.z; z < siz.z; z++) {
			for (uint32_t y = pos.y; y < siz.y; y++) {
				for (uint32_t x = pos.x; x < siz.x; x++) {
					if (posInBounds(x, y, z))
						retValue.push_back({ x, y, z });
				}
			}
		}
		return retValue;
	}
	//Checks within rectangle, top left corner is origin
	std::vector<Vec3D<uint32_t>> getValidSolidBlockPositions(Vec3D<uint32_t> pos, Vec3D<uint32_t> siz) {
		std::vector<Vec3D<uint32_t>> retValue = {};
		retValue.reserve(siz.x * siz.y * siz.z);
		Vec3D<uint32_t> end = pos + siz;
		for (uint32_t z = pos.z; z < end.z; z++) {
			for (uint32_t y = pos.y; y < end.y; y++) {
				for (uint32_t x = pos.x; x < end.x; x++) {
					if (posInBounds(x, y, z))
						if (getBlock(x, y, z).isNotAir())
							retValue.push_back({ x, y, z });
				}
			}
		}
		return retValue;
	}

	inline const VoxelBlock& getBlock(uint32_t x, uint32_t y, uint32_t z) {
		if (posInBounds(x, y, z) == false) {
			std::cout << "Error: VoxelWorld::getBlock's input position isn't in bounds!" << std::endl;
			std::cout << "Input: " << x << ", " << y << ", " << z << std::endl;
			throw;
		}
		uint32_t cx = x / width;
		uint32_t cz = z / depth;
		return chunks[cx][cz].getBlock(x % width, y, z % depth);
	}
	inline void setBlock(const VoxelBlock& _block, uint32_t x, uint32_t y, uint32_t z) {
		if (posInBounds(x, y, z) == false) {
			std::cout << "Error: VoxelWorld::setBlock's input position isn't in bounds!" << std::endl;
			std::cout << "Input: " << x << ", " << y << ", " << z << std::endl;
			throw;
		}
		uint32_t cx = x / width;
		uint32_t cz = z / depth;
		chunks[cx][cz].setBlock(_block, x % width, y, z % depth);
	}
	inline void destroyBlock(uint32_t x, uint32_t y, uint32_t z) {
		if (posInBounds(x, y, z) == false) {
			std::cout << "Error: VoxelWorld::destroyBlock's input position isn't in bounds!" << std::endl;
			std::cout << "Input: " << x << ", " << y << ", " << z << std::endl;
			throw;
		}
		uint32_t cx = x / width;
		uint32_t cz = z / depth;
		chunks[cx][cz].destroyBlock(x % width, y, z % depth);
		//needsRebuilding[x % width][z % depth] = true;
	}

	//void rebuild() {
	//	for (uint32_t i = 0; i < worldSize; i++)
	//		for (uint32_t j = 0; j < worldSize; j++)
	//			//if (needsRebuilding[i][j])
	//				chunks[j][i].rebuild();
	//}

	inline uint32_t getHeight() {
		return height;
	}
private:
	bool posInBounds(int32_t x, int32_t y, int32_t z) {
		if (x < 0 || y < 0 || z < 0)
			return false;
		if (x >= worldSize * width)
			return false;
		if (y >= height)
			return false;
		if (z >= worldSize * depth)
			return false;
		return true;
	}
};