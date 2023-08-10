#pragma once
#include <memory.h>
#include <array>
#include <tuple>


struct VoxelBlockMetaData {
	uint16_t maxHealth;
};
VoxelBlockMetaData voxelBlockMetaData[256];
HAL::mesh_handle_t grassTexture;

//NOTE: Excactly 4 bytes large in 'usage'.
//WARNING: STORING A DIRECT BUFFER OF THIS TYPE MAY CAUSE UB!!!
//Store a buffer of uint32_t instead.
struct VoxelBlock {
	uint8_t typeID;  //Used to index into a buffer of VoxelBlockMetaData
	uint8_t  padding;  //Reserved for later...
	uint16_t damage;

	inline bool isAir() const { return !typeID; }
	inline bool isNotAir() const { return typeID; }
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
	FlatBuffer<BodyID, width * height * depth> activeBodies;

	HAL::instanced_mesh_handle_t blockMesh;
	bool modified;
public:
	//static constexpr uint32_t width = 16, height = 256, depth = 16;

	void init(uint32_t _x, uint32_t _y, uint32_t _z) {
		memset(this, 0, sizeof(*this));
		modified = true;
		worldOffsetX = _x; worldOffsetY = _y; worldOffsetZ = _z;
		//std::string path = EE_getDirData();
        blockMesh = HAL::invalid_instanced_mesh_handle;
        //rebuildMesh();


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
		if (modified == false) {
            HAL::draw_instanced_mesh(blockMesh);
			return;
		}
		rebuild();
        HAL::draw_instanced_mesh(blockMesh);
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
				static_cast<uint16_t>(xplus), static_cast<uint16_t>(1), static_cast<uint16_t>(1)};
				spawnBody(body);
				continue;
			}
			if (xminus >= xplus && xminus >= yplus && xminus >= yminus) {
				BlockBody body = { static_cast<uint8_t>(rootBlockPos.x - xminus), static_cast<uint8_t>(rootBlockPos.y), static_cast<uint8_t>(rootBlockPos.z),
				static_cast<uint16_t>(xminus), static_cast<uint16_t>(1), static_cast<uint16_t>(1) };
				spawnBody(body);
				continue;
			}
			if (yplus >= xplus && yplus >= xminus && yplus >= yminus) {
				BlockBody body = { rootBlockPos.x, rootBlockPos.y, rootBlockPos.z,
				static_cast<uint16_t>(1), static_cast<uint16_t>(yplus), static_cast<uint16_t>(1) };
				spawnBody(body);
				continue;
			}
			if (yminus >= xplus && yminus >= xminus && yminus >= yplus) {
				BlockBody body = { static_cast<uint8_t>(rootBlockPos.x), static_cast<uint8_t>(rootBlockPos.y - yminus), static_cast<uint8_t>(rootBlockPos.z),
				static_cast<uint16_t>(1), static_cast<uint16_t>(yminus), static_cast<uint16_t>(1) };
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
        rebuildMesh();
        modified = false;
	}
	void rebuildMesh() {
		if (blockMesh == HAL::invalid_instanced_mesh_handle) {
            std::string path = "./Data/Meshes/Cube.obj";
            blockMesh = HAL::get_new_instanced_mesh(path);
            HAL::set_instanced_mesh_submesh_texture(blockMesh, 0, "diffuse", grassTexture);
		}

        uint32_t count = drawableBlocks.count;
        std::vector<glm::vec3> positions;
        positions.reserve(count);
        for (uint32_t i = 0; i < count; i++)
        {
            BlockPos &blockPos = drawableBlocks[i];
            glm::vec3 pos = {static_cast<float>(blockPos.x) + worldOffsetX,
                             static_cast<float>(blockPos.y) + worldOffsetY,
                             static_cast<float>(blockPos.z) + worldOffsetZ};
            pos.x += 0.5f;
            pos.y += 0.5f;
            pos.z += 0.5f;
            positions.push_back(pos);
        }
        HAL::set_instanced_mesh_positions(blockMesh, positions);
        HAL::set_instanced_mesh_scale(blockMesh, {0.5f, 0.5f, 0.5f});
	}
	void unload() {
		if (drawableBlocks.count == 0)
			return;
		drawableBlocks.count = 0;
		modified = true;

		memset(hasBody, 0, sizeof(hasBody));
		//memset(requiresBody, 0, sizeof(requiresBody));
		//{
		//	uint32_t bodyCount = activeBodies.count;
		//	for (uint32_t i = 0; i < bodyCount; i++)
		//		physics.removeBody(activeBodies[i]);
		//	activeBodies.clear();
		//}
        // assert(false); // TODO: What does OG unload do??
		//std::array<glm::vec3, 0> positions;
        //HAL::set_instanced_mesh_positions(blockMesh, positions);
        HAL::release_instanced_mesh(blockMesh);
        blockMesh = HAL::invalid_instanced_mesh_handle;
	}

	inline void copyTo(VoxelChunk& other) const {
		other.worldOffsetX = worldOffsetX;
        other.worldOffsetY = worldOffsetY;
        other.worldOffsetZ = worldOffsetZ;
        //other.blocks = blocks;
        memcpy(other.blocks, blocks, sizeof(blocks));
        other.drawableBlocks = drawableBlocks;
        //other.hasBody = hasBody;
        memcpy(other.hasBody, hasBody, sizeof(hasBody));
        other.activeBodies = activeBodies;
        //other.activeBodies.initCopy(activeBodies);
		//if (other.blockMesh != HAL::invalid_instanced_mesh_handle) {
        //    HAL::release_instanced_mesh(other.blockMesh);
		//}
        //std::string path = "./Data/Meshes/Cube.obj";
        //other.blockMesh = HAL::get_new_instanced_mesh(path);
        //HAL::set_instanced_mesh_submesh_texture(other.blockMesh, 0, "diffuse", grassTexture);
        //other.rebuildMesh();
		if (other.blockMesh != HAL::invalid_instanced_mesh_handle) {
		    HAL::release_instanced_mesh(other.blockMesh);
            other.blockMesh = HAL::invalid_instanced_mesh_handle;
		}
        //other.rebuild();
        //other.rebuildMesh();
        other.modified = true;
	}
private:
	inline bool isVisible(uint8_t x, uint8_t y, uint8_t z) {
		if (isAir(x, y, z) == false && hasNeighboringAir(x, y, z) == true)
			return true;
		return false;
	}
	inline void spawnBody(const BlockBody& blockBody) {
		//hasBody[blockBody.x][blockBody.y][blockBody.z] = true;

		for(uint32_t i = blockBody.z; i < (uint32_t)blockBody.z + blockBody.d; i++)
			for (uint32_t j = blockBody.y; j < (uint32_t)blockBody.y + blockBody.h; j++)
				for (uint32_t k = blockBody.x; k < (uint32_t)blockBody.x + blockBody.w; k++) {
					hasBody[k][j][i] = true;
				}
		auto bodyID = physics.addStaticBodyBox(
			(uint32_t)blockBody.x + worldOffsetX,
			//(uint32_t)worldOffsetY + (height * 1 - (blockBody.y * 1)),
			(uint32_t)blockBody.y + worldOffsetY,
			//(uint32_t)blockBody.y + 1 + worldOffsetY,
			(uint32_t)blockBody.z + worldOffsetZ,
			//(uint32_t)blockBody.z + worldOffsetZ - 1,
			blockBody.w, blockBody.h, blockBody.d,
			(void*)-1, true
		);
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
	struct VoxelFrame {
        static constexpr uint32_t max_chunks_modifiable = 50;
        FlatBuffer<Vec2D<uint16_t>, max_chunks_modifiable> chunksModifiedPos;
        DArray<VoxelChunk<width, height, depth>> chunkCopies;
		void init() {
            chunkCopies.init(max_chunks_modifiable);
            for (uint32_t i = 0; i < max_chunks_modifiable; i++)
                chunkCopies[i].init(0, 0, 0);
            reset();
		}
		void reset() {
			chunksModifiedPos.count = 0;
		}
		void destruct() {
            chunkCopies.free();
			chunksModifiedPos.count = 0;
		}
		void addChunkIfUnique(const VoxelChunk<width, height, depth>& chunk, Vec2D<uint16_t> pos) {
            uint32_t count = getChunkCount();
			for (uint32_t i = 0; i < count; i++)
                if (chunksModifiedPos[i].isEqual(pos))
                    return;
            chunksModifiedPos.push(pos);
            chunk.copyTo(chunkCopies[count]);
		}
		inline bool hasChunk(Vec2D<uint16_t> pos) {
            uint32_t count = getChunkCount();
			for (uint32_t i = 0; i < count; i++)
                if (chunksModifiedPos[i] == pos)
                    return true;
            return false;
		}
		uint32_t getChunkCount() {
			return chunksModifiedPos.count;
		}
	};
	VoxelChunk<width, height, depth> chunks[worldSize][worldSize];
	//bool needsRebuilding[worldSize][worldSize];
    static constexpr uint32_t max_frames = 60*2;
	RingBuffer<VoxelFrame, max_frames> frames;
public:
	void init(uint32_t genHeight, uint32_t x, uint32_t y, uint32_t z) {
		memset(this, 0, sizeof(*this));

		grassTexture = HAL::get_new_texture("./Data/Textures/grass1.png");

		for (uint32_t i = 0; i < worldSize; i++)
			for (uint32_t j = 0; j < worldSize; j++) {
				//float dx = x + i * width, dy = y, dz = z + j * depth;
				uint32_t dx = x + i * width, dy = 0, dz = z + j * depth;
				chunks[i][j].init(dx, dy, dz);
				chunks[i][j].gen(genHeight);
			}
		//rebuild();
		for (uint32_t i = 0; i < max_frames; i++) {
            VoxelFrame* frame = &frames.get();
            frame->init();
            frames.advance();
		}
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
				Vec2D<int32_t> curPos = {static_cast<int32_t>(j * width), static_cast<int32_t>(i * width)};
				if (curPos.isBetween(start, end))
					chunks[j][i].display();
				else
					chunks[j][i].unload();
			}
		}
        frames.advance();
        VoxelFrame *frame = &frames.get();
        frame->reset();
	}

	void rewind(uint16_t ticks) {
        std::vector<std::tuple<VoxelChunk<width, height, depth> *, uint16_t>> chunksToModify;
        std::vector<Vec2D<uint16_t>> chunkPositions;
		if (ticks >= max_frames) {
            HAL_ERROR("Voxel::rewind()'s ticks ${ticks} arg is larger than max_frames ${max_frames}!  Throwing...");
            throw;
		}
        frames.rewindFor(ticks);
		for (uint32_t i = 0; i < ticks; i++) {
			//frames.rewind();
            //frames.advance();
            VoxelFrame *frame = &frames.get();
            uint32_t count = frame->getChunkCount();
			for (uint32_t j = 0; j < count; j++) {
                VoxelChunk<width, height, depth>* chunk = &frame->chunkCopies[j];
				for (uint32_t k = 0; k < chunksToModify.size(); k++) {
					if (std::get<0>(chunksToModify[k]) == chunk) {
                        //std::get<1>(chunksToModify[k]) = i;
                        goto end;
					}
				}
                chunksToModify.push_back({chunk, i});
                chunkPositions.push_back(frame->chunksModifiedPos[j]);
                end:
                continue;
			}
            frames.advance();
		}
        frames.rewindFor(ticks);
        // TODO: Multithread this!
        uint32_t count = (uint32_t)(uint64_t)chunksToModify.size();
		for (uint32_t i = 0; i < count; i++) {
            auto &pos = chunkPositions[i];
            VoxelChunk<width, height, depth> &chunk = chunks[pos.x][pos.y];
            std::get<0>(chunksToModify[i])->copyTo(chunk);
            chunk.rebuild();
            //throw;
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
        HAL_ASSERT_REL(posInBounds(x, y, z) == true, "Error: VoxelWorld::getBlock's input position isn't in bounds!\n" 
			"Input: {}, {}, {}\n"
			, x, y, z);
		uint32_t cx = x / width;
		uint32_t cz = z / depth;
		return chunks[cx][cz].getBlock(x % width, y, z % depth);
	}
	inline void setBlock(const VoxelBlock& _block, uint32_t x, uint32_t y, uint32_t z) {
        HAL_ASSERT_REL(posInBounds(x, y, z) == true,
			"Error: VoxelWorld::setBlock's input position isn't in bounds!\n"
			"Input: {}, {}, {}\n",
			x, y, z);
		uint16_t cx = x / width;
		uint16_t cz = z / depth;
        VoxelFrame *frame = &frames.get();
        frame->markChunkIfUnique(chunks[cx][cz], {cx, cz});
		chunks[cx][cz].setBlock(_block, x % width, y, z % depth);
	}
	inline const VoxelChunk<width, height, depth>& getChunk(uint16_t x, uint16_t z) const {
		return chunks[x][z];
	}
	inline void destroyBlock(uint32_t x, uint32_t y, uint32_t z) {
		HAL_ASSERT_REL(posInBounds(x, y, z) == true,
			"Error: VoxelWorld::destroyBlock's input position isn't in bounds!\n"
			"Input: {}, {}, {}\n",
			x, y, z);
		uint16_t cx = x / width;
		uint16_t cz = z / depth;
        VoxelFrame *frame = &frames.get();
        frame->addChunkIfUnique(chunks[cx][cz], {cx, cz});
		chunks[cx][cz].destroyBlock(x % width, y, z % depth);
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