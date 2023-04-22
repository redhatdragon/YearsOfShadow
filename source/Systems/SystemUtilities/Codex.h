#pragma once
#include <unordered_map>

class Codex {
	std::vector<void*> assets;
	std::unordered_map<std::string, u32> hashes;
	std::vector<u32> refCounts;
	u32 assetCount;
public:
	bool has(const std::string& path) {
		if (hashes.find(path) == hashes.end())
			return false;
		return true;
	}
	void* get(u32 id) {
		return assets[id];
	}
	void release(u32 id) {
		refCounts[id]--;
		if (refCounts[id] == 0) {
			releaseAsset(assets[id]);
		}
	}
protected:
	static constexpr u32 INITIAL_CAPACITY = 256;

	virtual void* getAsset(const std::string& path) = 0;
	virtual void releaseAsset(void* asset) = 0;

	void baseInit() {
		assets.reserve(INITIAL_CAPACITY);
		hashes.reserve(INITIAL_CAPACITY);
		refCounts.reserve(INITIAL_CAPACITY);
	}
	u32 baseAdd(const std::string& path) {
		const auto& element = hashes.find(path);
		if (element != hashes.end()) {
			refCounts[element->second]++;
			return element->second;
		}
		assetCount++;
		hashes[path] = assetCount;
		void* asset = getAsset(path);
		assets[assetCount] = asset;
		std::cout << "Caching new asset: " << assetCount << " " << path.c_str() << std::endl;
		return assetCount;
	}
};

class InstancedMeshCodex : public Codex {
	std::vector<Vec3D<float>> sizes;
public:

	void init() {
		baseInit();
		sizes.reserve(INITIAL_CAPACITY);
	}
	u32 add(const std::string& path) {
		if (has(path) == false) {
			u32 id = baseAdd(path);
			sizes[id] = {1, 1, 1};
			return id;
		}
		return baseAdd(path);
	}
	void setSize(u32 id, Vec3D<float> siz) {  //May be removed...
		sizes[id] = siz;
	}
	Vec3D<float> getSize(u32 id) {  //May be removed...
		return sizes[id];
	}
private:
	void* getAsset(const std::string& path) override {
		return EE_getNewInstancedMesh(path.c_str());
	}
	void releaseAsset(void* asset) override {
		EE_releaseInstancedMesh(asset);
	}
};
InstancedMeshCodex instancedMeshCodex;