#include "Asset.h"

void* TextureCodex::textures[max];
std::unordered_map<std::string, uint32_t> TextureCodex::hashes;
uint32_t TextureCodex::refCount[max] = { 0 };
uint32_t TextureCodex::textureCount = 0;

char* FileCodex::fileDatas[max];
std::unordered_map<std::string, uint32_t> FileCodex::hashes;
uint32_t FileCodex::refCount[max];
uint32_t FileCodex::fileCount;