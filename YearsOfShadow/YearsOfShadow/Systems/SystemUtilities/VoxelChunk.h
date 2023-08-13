#pragma once

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
	uint32_t blocks[chunk_width][chunk_height][chunk_depth];
	FlatBuffer<BlockPos, chunk_width * chunk_height * chunk_depth> drawableBlocks;
	bool hasBody[chunk_width][chunk_height][chunk_depth];
	FlatBuffer<BodyID, chunk_width * chunk_height * chunk_depth> activeBodies;

	GameTick lastModified;  //Last modification timestamp for IO syncing reasons.

	HAL::instanced_mesh_handle_t blockMesh;
	bool modified;
public:
	void init(uint32_t _x, uint32_t _y, uint32_t _z) {
		memset(this, 0, sizeof(*this));
		modified = true;
		worldOffsetX = _x; worldOffsetY = _y; worldOffsetZ = _z;
        blockMesh = HAL::invalid_instanced_mesh_handle;
	}

	void gen(uint32_t genHeight) {
		modified = true;
		for (uint32_t z = 0; z < chunk_depth; z++)
			for (uint32_t y = 0; y < genHeight; y++)
				for (uint32_t x = 0; x < chunk_width; x++) {
					VoxelBlock block; block.typeID = 1; block.damage = 0;
					setBlock(block, x, (chunk_height-1)-y, z);
				}
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
			for (uint32_t i = rootBlockPos.x; i < chunk_width; i++) {
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
			for (uint32_t i = rootBlockPos.y; i < chunk_height; i++) {
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

		for (uint32_t z = 0; z < chunk_depth; z++)
			for (uint32_t y = 0; y < chunk_height; y++)
				for (uint32_t x = 0; x < chunk_width; x++) {
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
        for (uint32_t i = 0; i < count; i++) {
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
		if (x == chunk_width - 1)
			return true;
		if (isAir(x + 1, y, z))
			return true;

		if (y == 0)
			return true;
		if (isAir(x, y - 1, z))
			return true;
		if (y == chunk_height - 1)
			return true;
		if (isAir(x, y + 1, z))
			return true;

		if (z == 0)
			return true;
		if (isAir(x, y, z - 1))
			return true;
		if (z == chunk_depth - 1)
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