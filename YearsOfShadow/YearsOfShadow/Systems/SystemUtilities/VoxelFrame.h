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