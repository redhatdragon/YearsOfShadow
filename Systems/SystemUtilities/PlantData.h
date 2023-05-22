#pragma once

namespace SystemUtilities {
	struct PlantMetaData {
		struct HarvestCountData {
			uint32_t min, max;
			Name algorithm;
		};
		//struct GrowthTextureData {
		//	uint32_t time;  //days
		//	Name textureName;
		//	TextureID texture;
		//};

		Name filePath;

		uint32_t harvestTime;  //seconds
		uint32_t minGrowthTime, maxGrowthTime;  //days
		uint32_t lifespan;  //days

		Name requiredTool;

		DArray<Name> harvestType;
		DArray<HarvestCountData> harvestCount;

		Name seedlingTexturePath;
		Name minGrowthTexturePath;
		Name maxGrowthTexturePath;
		TextureID seedlingTexture;
		//DArray<GrowthTextureData> growthTextureData;
		TextureID minGrowthTexture;
		TextureID maxGrowthTexture;

		bool init(const EntityObject& obj, const char* inputFilePath);
	private:
		const ComponentObject* getComponent(const EntityObject& obj,
			const char* componentName, ComponentObject::TYPE expectedType,
			const char* objName);
		bool initAssets(const char* objName);
	};

	FlatBuffer<PlantMetaData, 256> plantMetaData;

	struct PlantData {
		uint64_t plantedDate;
		TextureID texture;
		uint8_t metaDataIndex;
		uint8_t growthStage;

		const PlantMetaData& getMetaData() {
			return plantMetaData[metaDataIndex];
		}
	};

	struct PlantSpawn {
		Vec2D<uint32_t> pos;
		uint32_t plantType;
		bool init(uint32_t x, uint32_t y, Name plantFilePath) {
			uint32_t count = plantMetaData.count;
			for (uint32_t i = 0; i < count; i++) {
				if (plantMetaData[i].filePath == plantFilePath) {
					plantType = i;
					pos = { x, y };
					return true;
				}
			}
			return false;
		}
	};
}

#include "PlantData_impl.h"