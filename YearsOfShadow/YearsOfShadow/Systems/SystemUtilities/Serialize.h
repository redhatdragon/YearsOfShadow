#pragma once
#include <fstream>
#include <unordered_map>
#include "EchoEngine//PhysicsEngineAABB3D.h"  //Help to serialize actual physics bodies

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
		//Thread safe!
		static SerialEntity* constructNew(EntityID entity) {
			static thread_local FlatBuffer<ComponentID, 32> idBuff;
			idBuff.clear();
			ecs.getEntityComponentIDs(entity, idBuff);
			uint32_t idCount = idBuff.count;
			uint32_t totalSize = 0;
			static thread_local FlatBuffer<Component*, 32> customSerializedComponents;
			customSerializedComponents.clear();
			for (uint32_t i = 0; i < idCount; i++) {
				if (serializeFunctions.find(idBuff[i]) != serializeFunctions.end()) {
					uint32_t size = 0;  //screw you compilation errors.
					void* data = serializeFunctions[idBuff[i]](entity, size);
					Component* component = constructComponent(data, size, idBuff[i]);
					free(data);
					totalSize += size;
					customSerializedComponents.push(component);
					continue;
				}
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
				if (serializeFunctions.find(idBuff[i]) != serializeFunctions.end()) {
					Component* component = nullptr;
					for (uint32_t j = 0; j < customSerializedComponents.count; j++) {
						if (idBuff[i] == customSerializedComponents[j]->componentID) {
							component = customSerializedComponents[j];
							break;
						}
					}
					uint32_t size = component->size + Component::getHeaderSize();
					memcpy(retValueOffset, component, size);
					retValueOffset += size;
					continue;
				}
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
				ComponentID bodyComponentID = ecs.getComponentID("body");
				if (idBuff[i] == bodyComponentID) {
					uint32_t componentSize = sizeof(BodyAABB);
					BodyID* bodyIDPtr = (BodyID*)ecs.getEntityComponent(entity, bodyComponentID);
					BodyAABB body = physics._getBodyCpy(*bodyIDPtr);
					memcpy(retValueOffset, &idBuff[i], sizeof(ComponentID));
					retValueOffset += sizeof(ComponentID);
					memcpy(retValueOffset, &componentSize, sizeof(uint32_t));
					retValueOffset += sizeof(uint32_t);
					memcpy(retValueOffset, &body, componentSize);
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
			for (uint32_t i = 0; i < customSerializedComponents.count; i++)
				free(customSerializedComponents[i]);
			ComponentID EOO = -1;  //End Of Object marker
			memcpy(retValueOffset, &EOO, sizeof(ComponentID));
			return retValue;
		}
		//NOT THREAD SAFE!
		void deserializeToDDECS() {
			ComponentID replicateEntityComponentID = ecs.getComponentID("replicateEntity");
			Component* replicateEntityComponent = findComponent(replicateEntityComponentID);
			if (replicateEntityComponent == nullptr) {
				HAL_ERROR("SerialEntity::deserializeToDDECS()\n");
				HAL_ERROR("THIS SHOULD NOT BE HAPPENING!\n");
				HAL_PANIC("Couldn't find replicateEntityComponent, aboring process...\n");
			}
			if (replicateEntityComponent->size != sizeof(ReplicateEntity)) {
				HAL_ERROR("SerialEntity::deserializeToDDECS()\n");
				HAL_ERROR("THIS SHOULD NOT BE HAPPENING!\n");
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
			Component* component = getRootComponent();
			EntityID entity = ecs.getNewEntity();
			ComponentID bodyComponentID = ecs.getComponentID("body");
			while (true) {
				ComponentID componentID = component->componentID;
				//Run first to avoid accidently reading past object
				if (componentID == -1)
					break;
				void* data = component->data;
				uint32_t size = component->size;
				if (deserializeFunctions.find(component->componentID) != deserializeFunctions.end()) {
					deserializeFunctions[component->componentID](entity, component->data, size);
					continue;
				}
				if (ecs.isComponentTypeDArray(componentID)) {
					uint32_t sizePerElem = ecs.getDArrayElementSize(componentID);
					if (size % sizePerElem) {
						HAL_ERROR("SerialEntity::deserializeToDDECS()\n");
						HAL_ERROR("THIS SHOULD NOT BE HAPPENING!\n");
						HAL_PANIC("serial entity component: {} size: {} % expected size: {} != 0, result: {}, Aboring...\n",
							size, sizePerElem, size % sizePerElem);
					}
					DArray<uint8_t> darray;
					darray.initCopy(component->data, component->size);
					ecs.emplace(entity, componentID, &darray);
					continue;
				}
				if (componentID == bodyComponentID) {
					BodyAABB* body = (BodyAABB*)component->data;
					if (component->size != sizeof(BodyAABB)) {
						HAL_ERROR("SerialEntity::deserializeToDDECS()\n");
						HAL_ERROR("THIS SHOULD NOT BE HAPPENING!\n");
						HAL_PANIC("serial entity component: {} size: {} != expected size: {}, Aboring...\n",
							ecs.getComponentName(componentID), size, sizeof(BodyAABB));
					}
					BodyID bodyID = physics.addBodyBox(body->pos.x, body->pos.y, body->pos.z,
						body->siz.x, body->siz.y, body->siz.z, &entity, body->isSolid);
					ecs.emplace(entity, componentID, &bodyID);
					continue;
				}
				if (size != ecs.getComponentSize(componentID)) {
					HAL_ERROR("SerialEntity::deserializeToDDECS()\n");
					HAL_ERROR("THIS SHOULD NOT BE HAPPENING!\n");
					HAL_PANIC("serial entity component: {} size: {} != expected size: {}, Aboring...\n",
						ecs.getComponentName(componentID), size, ecs.getComponentSize(componentID));
				}
				ecs.emplace(entity, componentID, data);
			}
		}

		//Thread safe!
		//Returns nullptr on if last object in buffer
		SerialEntity* next() {
			Component* component = getRootComponent();
			if (*(uint32_t*)this == -1)  //Check if end of SerialEntity* buffer
				return nullptr;
			while (true) {
				if (component->componentID == -1) {
					uint8_t* ptr = (uint8_t*)component;
					ptr++;
					//End of SerialEntity buffer
					if (*(uint32_t*)ptr == -1) {
						return nullptr;
					}
					return (SerialEntity*)ptr;
				}
				component = getNextComponent(component);
			}
		}
		uint32_t getSize() {
			uint32_t size = 0;
			Component* c = getRootComponent();
			if (c != nullptr) {
				while (true) {
					c = getNextComponent(c);
					if (c == nullptr)
						break;
					size += Component::getHeaderSize() + c->size;
				}
			}
			size+=sizeof(uint32_t);  //Account for the uint32_t -1 value for EndOfObject
			return size;
		}
		bool isEnd() {
			if (*(uint32_t*)this == -1)  //Check if end of SerialEntity* buffer
				return true;
			return false;
		}

		//Blittable, can use free on result
		static Component* constructComponent(void* data, uint32_t size, ComponentID componentID) {
			uint32_t totalSize = size + Component::getHeaderSize();
			Component* component;
			HAL_ALLOC_RAWBYTE(component, totalSize);
			component->componentID = componentID;
			component->size = size;
			memcpy(component->data, data, size);
			return component;
		}

		//Returns buffer of SerialEntity + additional u32 EndOfBuffer value of -1
		//Perfectly blittable, safe to call free manually or memcpy
		//Meant to be sent over the network and saved to disk
		//Thread safe!
		static SerialEntity* constructSerialEntityBuffer(EntityID* ids, uint32_t count, uint32_t& outSize) {
			static thread_local std::vector<SerialEntity*> ses;
			ses.clear();
			uint32_t totalSize = 0;
			for (uint32_t i = 0; i < count; i++) {
				ses.push_back(SerialEntity::constructNew(ids[i]));
			}
			for (uint32_t i = 0; i < count; i++) {
				totalSize += ses[i]->getSize();
			}
			if (totalSize == 0) {
				for (uint32_t i = 0; i < count; i++)
					free(ses[i]);
				outSize = 0;
				return nullptr;
			}
			totalSize += sizeof(uint32_t);  //Account for EndOfBuffer value
			SerialEntity* serialEntityBuffer;
			HAL_ALLOC_RAWBYTE(serialEntityBuffer, totalSize);
			uint8_t* offset = (uint8_t*)serialEntityBuffer;
			for (uint32_t i = 0; i < count; i++) {
				uint32_t size = ses[i]->getSize();
				memcpy(offset, ses[i], size);
				offset += size;
			}
			uint32_t buffTerminator = -1;
			memcpy(offset, &buffTerminator, sizeof(buffTerminator));
			for (uint32_t i = 0; i < count; i++)
				free(ses[i]);
			outSize = totalSize;
			return serialEntityBuffer;
		}

		static std::unordered_map<ComponentID, void* (*)(EntityID entity, uint32_t& outSize)> serializeFunctions;
		static std::unordered_map<ComponentID, void(*)(EntityID entity, void* data, uint32_t size)> deserializeFunctions;

		static void registerSerializeFunction(ComponentID componentID, void* (*func)(EntityID entity, uint32_t& outSize)) {
			if (serializeFunctions.find(componentID) == serializeFunctions.end()) {
				HAL_WARN("registerSerializeFunction()'s input func already used, ignoring...\n");
				return;
			}
			serializeFunctions[componentID] = func;
		}
		static void registerDeSerializeFunction(ComponentID componentID, void(*func)(EntityID entity, void* data, uint32_t size)) {
			if (deserializeFunctions.find(componentID) == deserializeFunctions.end()) {
				HAL_WARN("registerDeSerializeFunction()'s input func already used, ignoring...\n");
				return;
			}
			deserializeFunctions[componentID] = func;
		}
	};
	std::unordered_map<ComponentID, void* (*)(EntityID entity, uint32_t& outSize)> SerialEntity::serializeFunctions = {};
	std::unordered_map<ComponentID, void(*)(EntityID entity, void* data, uint32_t size)> SerialEntity::deserializeFunctions = {};
}