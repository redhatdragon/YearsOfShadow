#pragma once
#include <fstream>
#include <unordered_map>

namespace SystemUtilities {
	struct SerialObject {
		uint32_t count;
		bool isFixedLength;
		uint32_t elementSize;  //ignored if isFixedLength == true
		char componentName[16];  //max of 16 bytes
		uint8_t data[0];

		const uint32_t* getSizeofBuffer() {
			if (isFixedLength == true)
				return nullptr;
			return (uint32_t*)data;
		}
		const void* getDataBuffer() {
			if (isFixedLength == true)
				return data;
			uint32_t* dataAs32 = (uint32_t*)data;
			return &dataAs32[count];
		}

		static SerialObject* init(const std::string& name, 
			void* buffer, uint32_t elemCount, uint32_t sizeofElement) 
		{
			if (name.size() > 15) {
				std::cout << "Error: SerialObject::init()'s name arg exceed's max length!"
					<< std::endl;
				return nullptr;
			}
			if (name.size() == 0) {
				std::cout << "Error: SerialObject::init()'s name arg is empty!"
					<< std::endl;
				return nullptr;
			}
			uint32_t size = sizeof(SerialObject) + (elemCount * sizeofElement);
			SerialObject* retValue = (SerialObject*)malloc(size);
			retValue->count = elemCount;
			retValue->isFixedLength = true;
			retValue->elementSize = sizeofElement;
			memcpy(retValue->componentName, &name[0], name.size());
			retValue->componentName[name.size()] = 0;
		}
		static SerialObject* init(const std::string& name, 
			void* buffer, uint32_t elemCount, uint32_t* elementSizes) 
		{
			if (name.size() > 15) {
				std::cout << "Error: SerialObject::init()'s name arg exceed's max length!"
					<< std::endl;
				return nullptr;
			}
			if (name.size() == 0) {
				std::cout << "Error: SerialObject::init()'s name arg is empty!"
					<< std::endl;
				return nullptr;
			}
			uint32_t bufferSize = 0;
			for (uint32_t i = 0; i < elemCount; i++)
				bufferSize += elementSizes[i];
			uint32_t size = sizeof(SerialObject) + sizeof(uint32_t) * elemCount + bufferSize;
			SerialObject* retValue = (SerialObject*)malloc(size);
			retValue->count = elemCount;
			retValue->isFixedLength = false;
			memcpy(retValue->componentName, &name[0], name.size());
			retValue->componentName[name.size()] = 0;
			uint32_t* sizeofBuffer = (uint32_t*)retValue->getSizeofBuffer();
			void* dataBuffer = (void*)retValue->getDataBuffer();
			memcpy(sizeofBuffer, elementSizes, retValue->count);
			memcpy(dataBuffer, buffer, bufferSize);
		}
	private:

	};

	struct Serialize {
		static std::vector<SerialObject*> serializeECS() {
			std::vector<SerialObject*> retValue;
			retValue.reserve(64);

			return retValue;
		}
		static void deserializeECS(const std::vector<SerialObject*>& input) {

		}
		static void destructSerialObject(std::vector<SerialObject*>& input) {

		}

		static void writeToFile(const std::string& filePath, const std::vector<SerialObject*>& input) {

		}
		static std::vector<SerialObject*> readFromFile(const std::string& filePath) {

		}
	};



	std::unordered_map<std::string, SerialObject* (*)()> serializeMethods;

	static SerialObject* serializeHealth() {
		ComponentID componentID = ecs.getComponentID("health");
		if (componentID == -1) return nullptr;
		uint32_t count = ecs.getComponentCount(componentID);
		if (count == 0) return nullptr;
		void* healthBuffer = ecs.getComponentBuffer(componentID);
		SerialObject* retValue = SerialObject::init("health", healthBuffer, count, sizeof(Health));
		return retValue;
	}


}