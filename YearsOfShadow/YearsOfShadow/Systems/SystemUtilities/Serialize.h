#pragma once
#include <fstream>
#include <unordered_map>

namespace SystemUtilities {
	//A purely blittable object
	//Freeing directly should thus be safe
	class SerialEntity {
		struct Component {
			Name name;
			uint32_t size;
			uint8_t data[1];  //Not allowed to do 0 length array gaaaaaaaaaaaay
			static constexpr uint32_t getHeaderSize() {
				return sizeof(name) + sizeof(size);
			}
		};
		uint8_t components[1];
		Component* getRootComponent() {
			return (Component*)components;
		}
	public:
		static SerialEntity* constructNew(EntityID entity) {
			static FlatBuffer<ComponentID, 32> idBuff;
			idBuff.clear();
			ecs.getEntityComponentIDs(entity, idBuff);
			uint32_t idCount = idBuff.count;
			uint32_t totalSize = 0;
			for (uint32_t i = 0; i < idCount; i++) {
				if (ecs.isComponentTypeDArray(idBuff[i])) {

					continue;
				}
				totalSize += ecs.getComponentSize(idBuff[i]);
			}
			totalSize += Component::getHeaderSize() * idCount;
			SerialEntity* retValue = (SerialEntity*)malloc(totalSize);
			if (retValue == nullptr)
				HAL_PANIC("SerialEntity::constructNew() failed to allocate new SerialEntity with size {}\n", totalSize);
			uint8_t* retValueOffset = (uint8_t*)retValue;
			for (uint32_t i = 0; i < idCount; i++) {
				if (ecs.isComponentTypeDArray(idBuff[i])) {

					continue;
				}
				uint32_t componentSize = ecs.getComponentSize(idBuff[i]);
				void* componentData = ecs.getEntityComponent(entity, idBuff[i]);
				memcpy(retValueOffset, componentData, componentSize);
				retValueOffset += componentSize;
			}
		}
	};

	struct Serialize {
		
	};
}