struct VoxelLoader {
    struct ChunkDataCompressed {
    private:
        uint32_t size;
        uint64_t chunkID;
        uint8_t blocks[1];  //Cast to VoxelBlock
    public:
        ChunkDataCompressed* next() {
            return this+4+8+size;
        }
        void getAsUncompressed(uint32_t& blocksOut) {
            uint32_t outCount = 16*16*256*4;
            Crunch::decomp(blocks, size, &blocksOut, &outCount);
        }
    };
    struct FileData {
    private:
        uint32_t count;
        uint8_t data[1];
    public:
        ChunkDataCompressed* root() {
            return (ChunkDataCompressed*)data;
        }
        static FileData* loadFrom(std::fstream& file) {

        }
        static void saveTo(std::fstream& file) {

        }
    };

    static void load(const char* path) {
        std::fstream file;
        file.open(path);
        if (file.is_open() == false) {
            std::string errStr = "Error: VoxelLoader::load()'s input file path isn't accessible!  Path: ";
            errStr += path;
            HAL::hal_error(errStr);
            throw;
        }
        FileData* fileData = FileData::loadFrom(file);
    }
    static void save(const char* path) {
        std::fstream file;
        file.open(path);
        if (file.is_open() == false) {
            std::string errStr = "Error: VoxelLoader::save()'s input file path isn't accessible!  Path: ";
            errStr += path;
            HAL::hal_error(errStr);
            throw;
        }
        FileData::saveTo(file);
    }
};