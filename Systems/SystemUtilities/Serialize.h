#pragma once
#include <fstream>
#include <unordered_map>

namespace SystemUtilities {

	struct SerialComponent {
		void* data;
		uint32_t size;
		virtual void init() = 0;
		virtual void destruct() = 0;
		virtual void serialize(void* outData, uint32_t* outSize) = 0;
		virtual void deserialize(const void* inData, uint32_t inSize) = 0;
	};

	struct SerialHealthComponent : public SerialComponent {
		virtual void serialize(void* outData, uint32_t* outSize) {
			outData = &data;
		}
		virtual void deserialize(const void* inData, uint32_t inSize) {
			uint32_t* healthValue = (uint32_t*)inData;
			data = (void*)*healthValue;
		}
	};



	struct SerialEntity {
		std::vector<uint32_t> componentIDs;
		std::vector<SerialComponent> components;
	};

	struct SerialECS {
		std::vector<std::string> componentNames;  //needed?
		std::vector<SerialEntity> entities;
	};

	struct Serialize {
		//std::unordered_map<std::string, SerialComponent(*)()> serialMethods;

		static SerialECS serializeECS() {
			SerialECS retValue;

			return retValue;
		}
		static void deserializeECS(const SerialECS& input) {
			uint32_t entityCount = (uint32_t)input.entities.size();
			for (uint32_t i = 0; i < entityCount; i++) {
				const SerialEntity& serialEntity = input.entities[i];
				EntityID entityID = ecs.getNewEntity();
				uint32_t componentCount = (uint32_t)serialEntity.componentIDs.size();
				for (uint32_t j = 0; j < componentCount; j++) {
					//ecs.emplace(entityID, serialEntity.componentIDs[j], serialEntity.components[j]);

				}
			}
		}
		static void writeToFile(const std::string& filePath, const SerialECS& input) {

		}
		static SerialECS readFromFile(const std::string& filePath) {

		}
	};
}