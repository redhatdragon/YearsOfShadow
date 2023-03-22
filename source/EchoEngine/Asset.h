#pragma once
#include "HAL/HAL.h"
#include <unordered_map>
#include <iostream>

struct TextureID { uint32_t id; };
struct SoundID { uint32_t id; };
struct FileID { uint32_t id; };

namespace TextureCodex {
	static constexpr uint32_t max = 256;
	extern void* textures[max];
	extern std::unordered_map<std::string, uint32_t> hashes;
	extern uint32_t refCount[max];
	extern uint32_t textureCount;

	static TextureID add(const std::string& path) {
		const auto& element = hashes.find(path);
		if (element != hashes.end()) {
			refCount[element->second]++;
			return { element->second };
		}
		textureCount++;
		hashes[path] = textureCount;
		void* newTexture = EE_getNewTexture(path.c_str());
		textures[textureCount] = newTexture;
		std::cout << "Caching new texture: " << textureCount << " " << path.c_str() << std::endl;
		return { textureCount };
	}

	static void* get(TextureID id) {
		return textures[id.id];
	}
};

namespace FileCodex {
	static constexpr uint32_t max = 256;
	extern char* fileDatas[max];
	extern std::unordered_map<std::string, uint32_t> hashes;
	extern uint32_t refCount[max];
	extern uint32_t fileCount;

	static FileID add(const std::string& path) {
		const auto& element = hashes.find(path);
		if (element != hashes.end()) {
			refCount[element->second]++;
			return { element->second };
		}
		char* newFileData = (char*)malloc(EE_fileGetSize(path.c_str()));
		if (newFileData == NULL) return { 0 };
		fileCount++;
		hashes[path] = fileCount;
		EE_getFileText(path.c_str(), newFileData);
		fileDatas[fileCount] = newFileData;
		std::cout << "Caching new file: " << fileCount << " " << path.c_str() << std::endl;
		return { fileCount };
	}

	static char* get(FileID id) {
		return fileDatas[id.id];
	}
};