class VoxelFrame {
	static constexpr uint32_t max_chunks_modifiable = 50;
	FlatBuffer<Vec2D<uint16_t>, max_chunks_modifiable> chunksModifiedPos;
	//DArray<VoxelChunk> chunkCopies;
	DArray<uint32_t[chunk_width][chunk_height][chunk_depth]> chunkBlockCopies;
	DArray<FlatBuffer<BodyID, chunk_width * chunk_height * chunk_depth>> chunkActiveBodies;
public:
	void init() {
		chunkBlockCopies.init(max_chunks_modifiable);
		chunkActiveBodies.init(max_chunks_modifiable);
        //for (uint32_t i = 0; i < max_chunks_modifiable; i++)
            //chunkCopies[i].init(0, 0, 0);
		memset(&chunkBlockCopies[0], 0,
			sizeof(uint32_t[chunk_width][chunk_height][chunk_depth]) * max_chunks_modifiable);
		memset(&chunkActiveBodies[0], 0,
			sizeof(FlatBuffer<BodyID, chunk_width * chunk_height * chunk_depth>) * max_chunks_modifiable);
        reset();
	}
	void reset() {
		chunksModifiedPos.count = 0;
	}
	void destruct() {
		chunkActiveBodies.free();
        chunkBlockCopies.free();
		chunksModifiedPos.count = 0;
	}
	void addChunkIfUnique(const VoxelChunk& chunk, Vec2D<uint16_t> pos) {
        uint32_t count = getChunkCount();
		for (uint32_t i = 0; i < count; i++)
            if (chunksModifiedPos[i].isEqual(pos))
                return;
        chunksModifiedPos.push(pos);
        //chunk.copyTo(chunkCopies[count]);
		memcpy(&chunkBlockCopies[count], &chunk.blockBuffer(), VoxelChunk::sizeOfBlockBuffer());
		memcpy(&chunkActiveBodies[count], &chunk.bodyBuffer(), VoxelChunk::sizeOfBodyBuffer());
	}
	inline bool hasChunk(Vec2D<uint16_t> pos) {
        uint32_t count = getChunkCount();
		for (uint32_t i = 0; i < count; i++)
            if (chunksModifiedPos[i].isEqual(pos))
                return true;
        return false;
	}
	inline auto* chunkBlockData(uint32_t i) {
		return &chunkBlockCopies[i];
	}
	inline auto* chunkBodyData(uint32_t i) {
		return &chunkActiveBodies[i];
	}
	inline auto getChunkPos(uint32_t i) {
		return chunksModifiedPos[i];
	}
	uint32_t getChunkCount() {
		return chunksModifiedPos.count;
	}
};