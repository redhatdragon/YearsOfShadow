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
			ComponentID componentID;
			uint32_t size;
			uint8_t data[1];  //Not allowed to do 0 length array gaaaaaaaaaaaay
			static constexpr uint32_t getHeaderSize() {
				return sizeof(componentID) + sizeof(size);
			}
			void log() const {
				std::string type = "Blittable";
				if (serializeFunctions.find(componentID) != serializeFunctions.end())
					type = "Custom";
				else if (ecs.isComponentTypeDArray(componentID))
					type = "DArray";
				HAL_LOG("Component data at addr: {}\n", (void*)this);
				HAL_LOG("componentID: {}, name: {}\n", componentID, ecs.getComponentName(componentID));
				HAL_LOG("size: {}, type: {}\n", size, type.c_str());
			}
		};
		uint32_t componentCount;
		Component components[1];  //Used for debugging pruposes
		Component* getRootComponent() {
			return (Component*)components;
		}
		static Component* getNextComponent(Component* component) {
			uint8_t* ptr = (uint8_t*)component;
			ptr += Component::getHeaderSize() + component->size;
			Component* nextComponent = (Component*)ptr;
			return nextComponent;
		}
		Component* findComponent(ComponentID componentID) {
			Component* component = getRootComponent();
			for (uint32_t i = 0; i < componentCount; i++) {
				if (component->componentID == componentID)
					return component;
				component = getNextComponent(component);
			}
			HAL_WARN("SerialEntity::findComponent()\n");
			HAL_WARN("Couldn't find component with id: {}\n", ecs.getComponentName(componentID));
			return nullptr;
		}
	public:
		//Thread safe!
		//static SerialEntity* constructNew(EntityID entity) {
		static void constructNew(EntityID entity, std::vector<uint8_t>& out) {
			static thread_local FlatBuffer<ComponentID, 32> idBuff;
			idBuff.clear();
			ecs.getEntityComponentIDs(entity, idBuff);
			uint32_t idCount = idBuff.count;
			uint32_t totalSize = sizeof(componentCount);
			static thread_local FlatBuffer<Component*, 32> customSerializedComponents;
			customSerializedComponents.clear();
			for (uint32_t i = 0; i < idCount; i++) {
				if (serializeFunctions.find(idBuff[i]) != serializeFunctions.end()) {
					std::vector<uint8_t> out;
					serializeFunctions[idBuff[i]](entity, out);
					uint32_t size = (uint32_t)out.size();
					Component* component = constructComponent(out.data(), size, idBuff[i]);
					totalSize += size;
					totalSize += Component::getHeaderSize();
					customSerializedComponents.push(component);
					continue;
				}
				if (ecs.isComponentTypeDArray(idBuff[i])) {
					uint32_t DArrayElementSize = ecs.getDArrayElementSize(idBuff[i]);
					DArray<uint8_t>* componentDArray = (DArray<uint8_t>*)ecs.getEntityComponent(entity, idBuff[i]);
					totalSize += DArrayElementSize * (uint32_t)componentDArray->size();
					totalSize += Component::getHeaderSize();
					continue;
				}
				totalSize += ecs.getComponentSize(idBuff[i]);
				totalSize += Component::getHeaderSize();
			}
			//totalSize += Component::getHeaderSize() * idCount;
			out.resize(totalSize);
			SerialEntity* retValue = (SerialEntity*)&out[0];
			//HAL_ALLOC_RAWBYTE(retValue, totalSize);
			retValue->componentCount = idCount;
			uint8_t* retValueOffset = (uint8_t*)retValue;
			retValueOffset += sizeof(componentCount);
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
					uint32_t componentDataSize = DArrayElementSize * (uint32_t)componentDArray->size();
					void* componentData = componentDArray->data();
					memcpy(retValueOffset, &idBuff[i], sizeof(ComponentID));
					retValueOffset += sizeof(ComponentID);
					memcpy(retValueOffset, &componentDataSize, sizeof(uint32_t));
					retValueOffset += sizeof(uint32_t);
					memcpy(retValueOffset, componentData, componentDataSize);
					retValueOffset += componentDataSize;
					continue;
				}
				//ComponentID bodyComponentID = ecs.getComponentID("body");
				//if (idBuff[i] == bodyComponentID) {
				//	uint32_t componentSize = sizeof(BodyAABB);
				//	BodyID* bodyIDPtr = (BodyID*)ecs.getEntityComponent(entity, bodyComponentID);
				//	BodyAABB body = physics._getBodyCpy(*bodyIDPtr);
				//	memcpy(retValueOffset, &idBuff[i], sizeof(ComponentID));
				//	retValueOffset += sizeof(ComponentID);
				//	memcpy(retValueOffset, &componentSize, sizeof(uint32_t));
				//	retValueOffset += sizeof(uint32_t);
				//	memcpy(retValueOffset, &body, componentSize);
				//	retValueOffset += componentSize;
				//	continue;
				//}
				uint32_t componentSize = ecs.getComponentSize(idBuff[i]);
				void* componentData = ecs.getEntityComponent(entity, idBuff[i]);
				memcpy(retValueOffset, &idBuff[i], sizeof(ComponentID));
				retValueOffset += sizeof(ComponentID);
				memcpy(retValueOffset, &componentSize, sizeof(uint32_t));
				retValueOffset += sizeof(uint32_t);
				memcpy(retValueOffset, componentData, componentSize);
				retValueOffset += componentSize;
			}
			//for (uint32_t i = 0; i < customSerializedComponents.count; i++)
				//free(customSerializedComponents[i]);
			//return retValue;
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
			for (uint32_t i = 0; i < componentCount; i++) {
				ComponentID componentID = component->componentID;
				void* data = component->data;
				uint32_t size = component->size;
				if (deserializeFunctions.find(component->componentID) != deserializeFunctions.end()) {
					std::vector<uint8_t> in;
					in.resize(size);
					memcpy(&in[0], data, size);
					deserializeFunctions[component->componentID](entity, in);
					component = getNextComponent(component);
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
					ecs.emplaceOrCpy(entity, componentID, &darray);
					component = getNextComponent(component);
					continue;
				}
				//if (componentID == bodyComponentID) {
				//	BodyAABB* body = (BodyAABB*)component->data;
				//	if (component->size != sizeof(BodyAABB)) {
				//		HAL_ERROR("SerialEntity::deserializeToDDECS()\n");
				//		HAL_ERROR("THIS SHOULD NOT BE HAPPENING!\n");
				//		HAL_PANIC("serial entity component: {} size: {} != expected size: {}, Aboring...\n",
				//			ecs.getComponentName(componentID), size, sizeof(BodyAABB));
				//	}
				//	BodyID bodyID = physics.addBodyBox(body->pos.x, body->pos.y, body->pos.z,
				//		body->siz.x, body->siz.y, body->siz.z, &entity, body->isSolid);
				//	ecs.emplaceOrCpy(entity, componentID, &bodyID);
				//	component = getNextComponent(component);
				//	continue;
				//}
				if (size != ecs.getComponentSize(componentID)) {
					HAL_ERROR("SerialEntity::deserializeToDDECS()\n");
					HAL_ERROR("THIS SHOULD NOT BE HAPPENING!\n");
					HAL_PANIC("serial entity component: {} size: {} != expected size: {}, Aboring...\n",
						ecs.getComponentName(componentID), size, ecs.getComponentSize(componentID));
				}
				ecs.emplaceOrCpy(entity, componentID, data);
				component = getNextComponent(component);
			}
		}

		//Thread safe!
		static SerialEntity* getNextSerialEntity(SerialEntity* serialEntity) {
			Component* component = serialEntity->getRootComponent();
			uint32_t count = serialEntity->componentCount;
			for (uint32_t i = 0; i < count; i++) {
				component = getNextComponent(component);
			}
			SerialEntity* retValue = (SerialEntity*)component;
			return retValue;
		}
		uint32_t getSize() {
			uint32_t size = sizeof(componentCount);  //Account sizeof componentCount
			Component* root = getRootComponent();
			Component* c = root;
			size += Component::getHeaderSize() + c->size;
			uint32_t count = componentCount;
			for (uint32_t i = 1; i < count; i++) {
				c = getNextComponent(c);
				size += Component::getHeaderSize() + c->size;
			}
			return size;
		}
		void log() {
			HAL_LOG("SerialEntity addr: {}\n", (void*)this);
			Component* component = getRootComponent();
			component->log();
			uint32_t count = componentCount;
			for (uint32_t i = 1; i < count; i++) {
				component = getNextComponent(component);
				component->log();
			}
			HAL_LOG("\n");
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

		static std::unordered_map<ComponentID, void(*)(EntityID entity, std::vector<uint8_t>& out)> serializeFunctions;
		static std::unordered_map<ComponentID, void(*)(EntityID entity, const std::vector<uint8_t>& in)> deserializeFunctions;

		static void registerSerializeFunction(ComponentID componentID, void(*func)(EntityID entity, std::vector<uint8_t>& out)) {
			if (serializeFunctions.find(componentID) != serializeFunctions.end()) {
				HAL_WARN("registerSerializeFunction()'s input func already used, ignoring...\n");
				return;
			}
			serializeFunctions[componentID] = func;
		}
		static void registerDeSerializeFunction(ComponentID componentID, void(*func)(EntityID entity, const std::vector<uint8_t>& in)) {
			if (deserializeFunctions.find(componentID) != deserializeFunctions.end()) {
				HAL_WARN("registerDeSerializeFunction()'s input func already used, ignoring...\n");
				return;
			}
			deserializeFunctions[componentID] = func;
		}
	};
	std::unordered_map<ComponentID, void(*)(EntityID entity, std::vector<uint8_t>& out)> SerialEntity::serializeFunctions = {};
	std::unordered_map<ComponentID, void(*)(EntityID entity, const std::vector<uint8_t>& in)> SerialEntity::deserializeFunctions = {};



	struct SerialEntityBuffer {
		uint32_t serialEntityCount;
		SerialEntity serialEntities[1];
		inline static uint32_t getHeaderSize() {
			return sizeof(serialEntityCount);
		}

		//Returns buffer of SerialEntity + additional u32 EndOfBuffer value of -1
		//Meant to be sent over the network and saved to disk
		//Thread safe!
		static void constructNew(EntityID* ids, uint32_t count,
			std::vector<uint8_t>& out) {
			static thread_local std::vector<std::vector<uint8_t>> ses = {};
			ses.resize(count);
			uint32_t totalSize = getHeaderSize();
			for (uint32_t i = 0; i < count; i++) {
				ses[i] = {};
				SerialEntity::constructNew(ids[i], ses[i]);
				totalSize += (uint32_t)ses[i].size();
			}
			if (totalSize <= getHeaderSize()) {
				HAL_PANIC("SerialEntityBuffer::constructNew() trying to construct entities without components?!");
				return;
			}
			out.resize(totalSize);
			SerialEntityBuffer* serialEntityBuffer = (SerialEntityBuffer*)&out[0];
			serialEntityBuffer->serialEntityCount = count;
			uint8_t* offset = (uint8_t*)serialEntityBuffer;
			offset += getHeaderSize();
			for (uint32_t i = 0; i < count; i++) {
				uint32_t size = (uint32_t)ses[i].size();
				if (size == 0)
					continue;
				SerialEntity* se = (SerialEntity*)&ses[i][0];
				if (size != se->getSize()) {
					HAL_PANIC("SerialEntityBuffer::constructNew() size: {} != getSize: {}", size, se->getSize());
				}
				memcpy(offset, se, size);
				offset += size;
			}
		}

		static void log(SerialEntityBuffer* serialEntityBuffer) {
			uint32_t count = serialEntityBuffer->serialEntityCount;
			uint8_t* ptr = (uint8_t*)serialEntityBuffer;
			ptr += getHeaderSize();
			SerialEntity* se = (SerialEntity*)ptr;
			HAL_LOG("SerialEntityBuffer addr: {}\n", (void*)serialEntityBuffer);
			HAL_LOG("SerialEntity count: {}\n", count);
			se->log();
			for (uint32_t i = 1; i < count; i++) {
				se = SerialEntity::getNextSerialEntity(se);
				se->log();
			}
		}

		inline void deserializeToDDECS() {
			uint32_t count = serialEntityCount;
			uint8_t* ptr = (uint8_t*)this;
			ptr += getHeaderSize();
			SerialEntity* se = (SerialEntity*)ptr;
			se->deserializeToDDECS();
			for (uint32_t i = 1; i < count; i++) {
				se = SerialEntity::getNextSerialEntity(se);
				se->deserializeToDDECS();
			}
		}
	};
}