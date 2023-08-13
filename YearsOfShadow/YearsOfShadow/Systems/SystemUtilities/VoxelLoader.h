struct VoxelLoader {
    struct ChunkDataCompressed {
        uint32_t size;
        uint64_t chunkID;
        uint8_t blocks[];  //Cast to VoxelBlock
        ChunkDataCompressed* next() {
            return this+size;
        }
        void getAsUncompressed(uint32_t& blocksOut) {
            uint32_t outCount = 16*16*256*4;
            Crunch::decomp(blocks, size, blocksOut, &outCount);
        }
    };
    struct FileData {
        uint32_t count;
        uint8_t data[];
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