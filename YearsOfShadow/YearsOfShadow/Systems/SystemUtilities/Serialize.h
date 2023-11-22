#pragma once
#include <fstream>
#include <unordered_map>

//Attach this to all entities that should be replicated across the network or saved to file
//Internal data used to help avoid accidentally duplicating entities client side
struct ReplicateEntity {
	EntityID serverEntityID;
	EntityHandle serverEntityHandle;
	void init(EntityID entityID, EntityHandle entityHandle) {
		serverEntityID = entityID;
		serverEntityHandle = entityHandle;
	}
	bool match(ReplicateEntity& other) {
		if (serverEntityID == other.serverEntityID &&
			serverEntityHandle == other.serverEntityHandle)
			return true;
		return false;
	}
};

namespace SystemUtilities {
	//A purely blittable object
	//Freeing directly should thus be safe
	//Expects that entity has a ReplicateEntity component, else will crash
	class SerialEntity {
		struct Component {
			//If == -1 we have reached the very end of our SerialEntity object
			ComponentID componentID;
			uint32_t size;
			uint8_t data[1];  //Not allowed to do 0 length array gaaaaaaaaaaaay
			static constexpr uint32_t getHeaderSize() {
				return sizeof(componentID) + sizeof(size);
			}
		};
		uint8_t components[1];
		Component* getRootComponent() {
			return (Component*)components;
		}
		Component* getNextComponent(Component* component) {
			uint8_t* ptr = (uint8_t*)component;
			ptr += Component::getHeaderSize() + component->size;
			return (Component*)ptr;
		}
		Component* findComponent(ComponentID componentID) {
			Component* component = getRootComponent();
			loop:
			if (component->componentID == -1) {
				HAL_WARN("SerialEntity::findComponent()\n");
				HAL_WARN("Couldn't find component with id: {}\n", ecs.getComponentName(componentID));
				return nullptr;
			}
			if (component->componentID == componentID)
				return component;
			goto loop;
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
					totalSize += ecs.getDArrayElementSize(idBuff[i]);
					continue;
				}
				totalSize += ecs.getComponentSize(idBuff[i]);
			}
			totalSize += Component::getHeaderSize() * idCount;
			totalSize += sizeof(ComponentID);  //-1 end of obj marker
			SerialEntity* retValue = (SerialEntity*)malloc(totalSize);
			if (retValue == nullptr)
				HAL_PANIC("SerialEntity::constructNew() failed to allocate new SerialEntity with size {}\n", totalSize);
			uint8_t* retValueOffset = (uint8_t*)retValue;
			for (uint32_t i = 0; i < idCount; i++) {
				if (ecs.isComponentTypeDArray(idBuff[i])) {
					uint32_t DArrayElementSize = ecs.getDArrayElementSize(idBuff[i]);
					DArray<uint8_t>* componentDArray = (DArray<uint8_t>*)ecs.getEntityComponent(entity, idBuff[i]);
					uint32_t componentSize = DArrayElementSize * (uint32_t)componentDArray->size();
					void* componentData = componentDArray->data();
					memcpy(retValueOffset, &idBuff[i], sizeof(ComponentID));
					retValueOffset += sizeof(ComponentID);
					memcpy(retValueOffset, &componentSize, sizeof(uint32_t));
					retValueOffset += sizeof(uint32_t);
					memcpy(retValueOffset, componentData, componentSize);
					retValueOffset += componentSize;
					continue;
				}
				uint32_t componentSize = ecs.getComponentSize(idBuff[i]);
				void* componentData = ecs.getEntityComponent(entity, idBuff[i]);
				memcpy(retValueOffset, &idBuff[i], sizeof(ComponentID));
				retValueOffset += sizeof(ComponentID);
				memcpy(retValueOffset, &componentSize, sizeof(uint32_t));
				retValueOffset += sizeof(uint32_t);
				memcpy(retValueOffset, componentData, componentSize);
				retValueOffset += componentSize;
			}
			ComponentID EOO = -1;  //End Of Object marker
			memcpy(retValueOffset, &EOO, sizeof(ComponentID));
		}
		void deserializeToDDECS() {
			ComponentID replicateEntityComponentID = ecs.getComponentID("replicateEntity");
			Component* replicateEntityComponent = findComponent(replicateEntityComponentID);
			if (replicateEntityComponent == nullptr) {
				HAL_ERROR("SerialEntity::deserializeToDDECS()\n");
				HAL_PANIC("Couldn't find replicateEntityComponent (this shouldn't ever be possible), aboring process...\n");
			}
			if (replicateEntityComponent->size != sizeof(ReplicateEntity)) {
				HAL_ERROR("SerialEntity::deserializeToDDECS()\n");
				HAL_PANIC("replicateEntityComponent size: {} != sizeof(ReplicateEntity): {}, Aboring...\n",
					replicateEntityComponent->size, sizeof(ReplicateEntity));
			}
			ReplicateEntity* replicateEntity = (ReplicateEntity*)replicateEntityComponent->data;
			uint32_t count = ecs.getComponentCount(replicateEntityComponentID);
			ReplicateEntity* replicateEntityBuff = (ReplicateEntity*)ecs.getComponentBuffer(replicateEntityComponentID);
			for (uint32_t i = 0; i < count; i++) {
				EntityID entity = ecs.getOwner(replicateEntityComponentID, i);
				ReplicateEntity& re = replicateEntityBuff[i];
				if (replicateEntity->match(re)) {
					//TODO: What to do, ignore, copy everything??!
					return;
				}
			}
			//create entity and add all components...
		}
	};
}