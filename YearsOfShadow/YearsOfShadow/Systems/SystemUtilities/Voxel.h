#pragma once
#include <memory.h>
#include <array>
#include <tuple>
#include <fstream>
#include <EchoEngine/Crunch.h>


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

#include "VoxelChunk.h"

//worldSize is the number or chunks wide for the x as well as z axis.
template<uint32_t worldSize = 1, uint32_t width = 16, uint32_t height = 256, uint32_t depth = 16>
class VoxelWorld {
	#include "VoxelFrame.h"
	#include "VoxelLoader.h"
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