#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include <concepts>
#include <type_traits>
#include <bit>
#include <iostream>

#include "FlatBuffer.h"
#ifdef THREADING_ENABLED
#include "HAL/HAL.h"
#endif
#include "DArray.h"
#include "RingBuffer.h"
#include <time.h>
typedef uint32_t EntityID;

template<std::integral T>
[[nodiscard]] void* to_void_ptr(T v) noexcept {
	if constexpr (std::is_unsigned_v<T>)
		return reinterpret_cast<void*>(static_cast<size_t>(v));
	else
		return reinterpret_cast<void*>(std::bit_cast<uint64_t>(static_cast<int64_t>(v)));
}

typedef uint32_t ComponentID;
class System;
typedef uint64_t EntityHandle;
typedef uint64_t GameTick;

template<uint32_t max_components, uint32_t max_entities>
class DDECS {
	struct Entity {
		FlatFlaggedBuffer<uint32_t, max_components> componentIndexes;
		void clear() {
			memset(this, 0, sizeof(*this));
		}
	};
	FlatFlaggedBuffer<Entity, max_entities> entities;
	class ComponentBuffer {
	public:
		uint8_t* data;
		uint32_t count = 0;
		EntityID owner[max_entities];
		uint32_t componentSize;
		std::string name;
		inline void init(const std::string& componentName, uint32_t size) {
			data = (uint8_t*)malloc((uint64_t)size * max_entities);
			componentSize = size;
			name = componentName;
		}
		inline uint32_t getCount() { return count; }
		inline EntityID getOwner(uint32_t index) { return owner[index]; }
		inline void* getData() { return (void*)data; }
		inline const std::string& getName() { return name; }
		inline uint32_t getComponentSize() { return componentSize; }
		inline void push(void* _data, EntityID entityID) {
			memcpy(&data[count * componentSize], _data, componentSize);
			owner[count] = entityID;
			count++;
		}
		inline EntityID remove(uint32_t index) {
			count--;
			memcpy(&data[index * componentSize], &data[count * componentSize], componentSize);
			owner[index] = owner[count];
			//TODO: This needs rework not sure how though.
			EntityID retValue = owner[count];
			owner[count] = -1;  //TODO: should be safe to remove without any consequence.
			//return owner[count];
			return retValue;
		}
	};
	FlatBuffer<ComponentBuffer, max_components> components;
	class BufferSerializer {
		void(*serializerFunction)(ComponentID id, void* outBuff);
		uint32_t sizePerDArrayElem;
	public:
		inline void defineAsDArray(uint32_t sizePerElem) {
            serializerFunction = (void(*)(ComponentID, void *)) - 1;
			sizePerDArrayElem = sizePerElem;
		}
		inline void defineAsCustom(void(*_serializerFunction)(ComponentID id, void* outBuff)) {
			serializerFunction = _serializerFunction;
		}
		inline void defineAsDefault() {
			serializerFunction = nullptr;
		}
		inline void serializeBuffer(ComponentID id, DDECS& ecs, void* outBuff) {
			if (isCustom())
				serializerFunction(id, outBuff);
			else if (isDArray()) {  //Hacks!
				uint32_t count = ecs.getComponentCount(id);
				DArray<uint8_t>* componentBuffer = (DArray<uint8_t>*)ecs.getComponentBuffer(id);
				DArray<uint8_t>* outBuffAsDArrays = (DArray<uint8_t>*)outBuff;
				for (uint32_t i = 0; i < count; i++) {
					DArray<uint8_t>* elem = &componentBuffer[i];
					DArray<uint8_t>* outElem = &outBuffAsDArrays[i];
					if (elem->size() != outElem->size()) {
						outElem->free();
						outElem->initCopy(elem->data(), elem->size() * sizePerDArrayElem);
						outElem->dataHeader->count = elem->size();
					} else {
						memcpy(outElem->data(), elem->data(), elem->size() * sizePerDArrayElem);
					}
				}
			} else {
				uint32_t count = ecs.getComponentCount(id);
				uint32_t size = ecs._getComponentSize(id);
				void* componentBuffer = ecs.getComponentBuffer(id);
				memcpy(outBuff, componentBuffer, count * size);
			}
		}
		inline bool isDArray() {
			if (serializerFunction == (void*)-1)
				return true;
			return false;
		}
		inline uint32_t getDArrayElemSize() {
			return sizePerDArrayElem;
		}
		inline bool isCustom() {
			if (serializerFunction != 0 && serializerFunction != (void*)-1)
				return true;
			return false;
		}
	};
	FlatBuffer<BufferSerializer, max_components> serializers;
	struct ECSFrame {
		FlatFlaggedBuffer<Entity, max_entities> entities;
		FlatBuffer<ComponentBuffer, max_components> components;
	public:
		void init() {
			memset(this, 0, sizeof(*this));
		}
		void update(DDECS& ecs) {
			memcpy(&entities, &ecs.entities, sizeof(entities));
			memcpy(&components, &ecs.components, sizeof(components));
			for (uint32_t i = 0; i < max_components; i++) {
				auto& comp = components[i];
				auto& ecsComp = ecs.components[i];
				comp.count = ecsComp.count;
				memcpy(&comp.owner[0], &ecsComp.owner[0], sizeof(comp.owner));
				comp.componentSize = ecsComp.componentSize;
				comp.name = ecsComp.name;
				if (comp.data == nullptr) {
					comp.data = (uint8_t*)malloc((uint64_t)ecsComp.componentSize * max_entities);
				}
				ecs.serializers[i].serializeBuffer(i, ecs, comp.data);
			}
		}
		void copyToECS(DDECS& ecs) {
			memcpy(&ecs.entities, &entities, sizeof(entities));
			memcpy(&ecs.components, &components, sizeof(components));
			for (uint32_t i = 0; i < max_components; i++) {
				auto& comp = components[i];
				auto& ecsComp = ecs.components[i];
				ecsComp.count = comp.count;
				memcpy(&ecsComp.owner[0], &comp.owner[0], sizeof(ecsComp.owner));
				ecsComp.componentSize = comp.componentSize;
				ecsComp.name = comp.name;
				if (ecsComp.data == nullptr) {
					ecsComp.data = (uint8_t*)malloc((uint64_t)comp.componentSize * max_entities);
				}
				ecs.serializers[i].serializeBuffer(i, ecs, ecsComp.data);
			}
		}
	};
	static constexpr uint32_t max_frames = 60 * 5;
	RingBuffer<ECSFrame*, max_frames> frames;

	/*struct Bucket {
		std::vector<ComponentID> componentTypes;
		std::vector<EntityID> entities;
		void sort() {

		}
		bool operator==(std::vector<ComponentID>& other) {
			for (size_t i = 0; i < componentTypes.size(); i++)
				if (componentTypes[i] != other[i])
					return false;
			return true;
		}
	};*/

	void(*destructors[max_components])(ComponentID id, uint32_t index);
	//void(*serializers[max_components])(ComponentID id);
	//uint32_t DArrayElemSize[max_components];
	//std::unordered_map<std::vector<ComponentID>,std::vector<EntityID>> entityLists;
	std::vector<System*> systems;
	ComponentID handleComponentID = -1;
	GameTick ticksPassed;
	clock_t ms;
public:
	void init() {
		for (unsigned int i = 0; i < max_entities; i++) {
			entities[i].clear();
		}
		handleComponentID = registerComponentAsBlittable("handle", sizeof(EntityHandle));
		for (unsigned int i = 0; i < max_components; i++) {
			destructors[i] = nullptr;
		}
#ifdef REWIND_ENABLED
		for (unsigned int i = 0; i < max_components; i++) {
			serializers[i].defineAsDefault();
		}
		frames.init();
		for (unsigned int i = 0; i < max_frames; i++) {
			ECSFrame* frame = frames.get();
			frame = new ECSFrame();
			frame->init();
			frames.set(frame);
			frames.advance();
		}
#endif
	}
	void destruct() {
		size_t count = systems.size();
		for (size_t i = 0; i < count; i++) {
			systems[i]->destroy();
			delete systems[i];
		}
		systems.resize(0);
	}
	EntityID getNewEntity() {
		static EntityHandle uid = 1;
		entities.incrementCount();
		EntityID retValue = entities.toggleFirstInvalid();
		emplace(retValue, handleComponentID, &uid);
		uid++;
		return retValue;
	}
	void removeEntity(EntityID entity) {
		auto &compIndexes = entities[entity].componentIndexes;
		uint32_t compCount = compIndexes.getCount();
		uint32_t i = 0, foundCount = 0;
		while (foundCount < compCount) {
			if (compIndexes.getIsValid(i)) {
				removeComponent(entity, i);
				foundCount++;
			}
			i++;
		}
		entities[entity].clear();
		entities.decrementCount();
		entities.setInvalid(entity);
	}
	ComponentID _registerComponentUnsafe(const std::string& componentName, uint32_t size) {
		ComponentID retValue = { components.count };
		ComponentBuffer *buffer = &components[components.count];
		buffer->init(componentName, size);
		components.count++;
		return retValue;
	}
	ComponentID registerComponentAsBlittable(const std::string& componentName, uint32_t size) {
		ComponentID retValue = getComponentID(componentName);
		if (retValue == -1)
			retValue = _registerComponentUnsafe(componentName, size);
		//TODO: need to check for valid size, if false return -1
		serializers[retValue].defineAsDefault();
		return retValue;
	}
	ComponentID registerComponentAsDArray(const std::string& componentName, uint32_t sizePerElem) {
		ComponentID retValue = getComponentID(componentName);
		if (retValue == -1)
			retValue = _registerComponentUnsafe(componentName, sizeof(DArray<uint8_t>));
		//TODO: need to check for valid size, if false return -1
		serializers[retValue].defineAsDArray(sizePerElem);
		return retValue;
	}
	ComponentID getComponentID(const std::string& componentName) {
		auto count = components.count;
		for (uint32_t i = 0; i < count; i++) {
			if (components[i].getName() == componentName)
				return { i };
		}
		return { (uint32_t)-1 };
	}
#ifdef REWIND_ENABLED
	void setComponentSerializerAsCustom(ComponentID componentID, void(*serializerFunction)(ComponentID id, void* outBuff)) {
		serializers[componentID].defineAsCustom(componentID, serializerFunction);
	}
	void setComponentSerializerAsDArray(ComponentID componentID, uint32_t dArrayElemSize) {
		serializers[componentID].defineAsDArray(componentID, dArrayElemSize);
	}
#endif
	void emplace(EntityID entity, ComponentID componentID, void* data) {
		if (entityHasComponent(entity, componentID)) {
			std::cout << "Warning: DDECS::emplace()'s entity arg already had component " << components[componentID].name
				<< " " << "canceling emplace()" << std::endl;
			return;
		}
		ComponentBuffer* buffer = &components[componentID];
		buffer->push(data, entity);
		auto componentSize = buffer->getComponentSize();
		entities[entity].componentIndexes[componentID] = buffer->getCount()-1;
		entities[entity].componentIndexes.setValid(componentID);
		entities[entity].componentIndexes.incrementCount();
	}
	void removeComponent(EntityID entity, ComponentID componentID) {
		auto index = entities[entity].componentIndexes[componentID];
		if (componentID >= max_components) {
			std::vector<std::string> debugInfo = getEntityDebugInfo(entity);
			size_t len = debugInfo.size();
			for (uint32_t i = 0; i < len; i++)
				std::cout << debugInfo[i] << std::endl;
			throw;
		}
		if (destructors[componentID])
			destructors[componentID](componentID, index);
		EntityID otherEntity = components[componentID].remove(index);
		entities[entity].componentIndexes.setInvalid(componentID);
		entities[entity].componentIndexes.decrementCount();
		entities[otherEntity].componentIndexes[componentID] = index;
	}
	void removeComponentsOfType(ComponentID componentID) {
		static FlatBuffer<EntityID, max_entities> entitiesToRemoveComponent;
		entitiesToRemoveComponent.count = 0;
		uint32_t count = getComponentCount(componentID);
		for (uint32_t i = 0; i < count; i++)
			entitiesToRemoveComponent.push(getOwner(componentID, i));
		for (uint32_t i = 0; i < count; i++)
			removeComponent(entitiesToRemoveComponent[i], componentID);
	}
	void* getComponentBuffer(ComponentID componentID) {
		return components[componentID].getData();
	}
	uint32_t getComponentCount(ComponentID componentID) {
		return components[componentID].getCount();
	}
	EntityID getOwner(ComponentID id, uint32_t index) {
		return components[id].getOwner(index);
	}
	void* getEntityComponent(EntityID entity, ComponentID componentID) {
		uint32_t componentIndex = entities[entity].componentIndexes[componentID];
		if (entities[entity].componentIndexes.getIsValid(componentID) == false)
			return nullptr;
		ComponentBuffer* buffer = &components[componentID];
		uint8_t* data = (uint8_t*)buffer->getData();
		data += componentIndex * buffer->getComponentSize();
		return (void*)data;
	}
	template<typename T>
	T getEntityComponentAs(EntityID entity, ComponentID componentID) {
		void* retValue = getEntityComponent(entity, componentID);
		return *(T*)retValue;
	}
	inline uint32_t getComponentSize(ComponentID componentID) {
		//TODO: add error checking here later to note if componentID is valid
		return components[componentID].getComponentSize();
	}
	inline void getEntityComponentIDs(EntityID entity, FlatBuffer<ComponentID, max_components>& outBuff) {
		//TODO: optimize using findFirstValid calls mid index
		for (uint32_t i = 0; i < max_components; i++)
			if (entities[entity].componentIndexes.getIsValid(i) == true)
				outBuff.push(i);
	}
	bool isComponentTypeDArray(ComponentID componentID) {
		return serializers[componentID].isDArray();
	}
	bool entityHasComponent(EntityID entity, ComponentID componentID) {
		if (getEntityComponent(entity, componentID)) return true;
		return false;
	}
	bool entityHasComponents(EntityID entity, const std::vector<ComponentID>& components) {
		for (auto c : components) {
			if (entityHasComponent(entity, c) == false)
				return false;
		}
		return true;
	}
	uint32_t getEntityCount() {
		return entities.getCount();
	}

	void registerDestructor(ComponentID componentID, void(*destructor)(ComponentID componentID, uint32_t index)) {
		destructors[componentID] = destructor;
	}

	std::vector<void*>* getEntitiesFromList(uint32_t listID) {
		return nullptr;
	}

	GameTick getTicksPassed() {
		return ticksPassed;
	}
	#ifdef REWIND_ENABLED
	void rewind(uint32_t amount) {
		if (amount >= max_frames) {
			std::cout << "Error: DDECS:rewind()'s amount param is bigger than max frames, exiting!" << std::endl;
			std::cout << "Amount: " << amount << " max_frames: " << max_frames << std::endl;
			throw;
		}
		frames.rewindFor(amount);
		ECSFrame* frame = frames.get();
		frame->update(*this);
	}
	void rewindTo(GameTick _gameTick) {
		GameTick rewindAmount = getTicksPassed() - _gameTick;
		rewind(rewindAmount);
	}
	#endif

	template<typename T>
	void registerSystem() {
		T* newSys = new T;
		std::cout << "Initializing system: " << newSys->getName() << "...  ";
		clock_t c = clock();
		newSys->init();
		c = clock() - c;
		std::cout << "Complete!  " << "(" << (float)c << " ms)" << std::endl;
		systems.push_back(newSys);
	}
	void runSystems() {
		ms = clock();
		for (auto sys : systems) {
			clock_t start = clock();
			sys->run();
			sys->ms = clock() - start;
		}
#ifdef REWIND_ENABLED
		auto* frame = frames.get();
		frame->update(*this);
		frames.advance();
#endif
		ticksPassed++;
		ms = clock() - ms;
	}

	void runSystems(std::vector<std::string>& blackList) {
		ms = clock();
		for (auto sys : systems) {
			clock_t start = clock();
			while (HAL::is_thread_pool_finished(threadPool) == false)
				continue;
			for (auto& str : blackList)
				if (str == sys->getName())
					goto skip;
			sys->run();
			skip:
			sys->ms = clock() - start;
		}
		#ifdef REWIND_ENABLED
		auto* frame = frames.get();
		frame->update(*this);
		frames.advance();
		#endif
		ticksPassed++;
		ms = clock() - ms;
	}

	// entity must be valid
	bool entityHandleValid(EntityID entity, EntityHandle handle) {
		EntityHandle* trueHandle = (EntityHandle*)getEntityComponent(entity, handleComponentID);
		if (trueHandle == nullptr || *trueHandle != handle)
			return false;
		return true;
	}
	EntityHandle entityGetHandle(EntityID entity) {
		return *(EntityHandle*)getEntityComponent(entity, handleComponentID);
	}
	std::vector<std::string> getEntityDebugInfo(EntityID entity) {
		std::vector<std::string> retValue;
		retValue.push_back("EntityInfo");
		if(entities.getIsValid(entity)) {
			retValue.push_back("isAlive");
		} else {
			retValue.push_back("isNotAlive");
		}
		retValue.push_back("Entity has..");
		for (uint32_t i = 0; i < max_components; i++) {
			if (entityHasComponent(entity, i)) {
				std::string& componentName = components[i].name;
				std::string line = "";
				line += std::to_string(i);
				line += ": ";
				line += componentName;
				retValue.push_back(line);
			}
		}
		return retValue;
	}
	std::vector<std::string> getDebugInfoStr() {
		std::vector<std::string> retValue;
		for (System* sys : systems) {
			std::string str = sys->getName();
			str += ": ";
			str += sys->getTimeMSStr();
			retValue.push_back(str);
		}
		std::string str = "Total: ";
		str += std::to_string(ms);
		retValue.push_back(str);
		return retValue;
	}
	std::vector<float> getDebugInfoMS() {
		std::vector<float> retValue;
		for (System* sys : systems) {
			float e = sys->getTimeMS();
			retValue.push_back(e);
		}
		return retValue;
	}
	std::vector<std::string> getSystemNames() {
		std::vector<std::string> retValue;
		for (System* sys : systems) {
			std::string str = sys->getName();
			retValue.push_back(str);
		}
		return retValue;
	}
private:
	inline uint32_t _getComponentSize(ComponentID componentID) {
		return components[componentID].getComponentSize();
	}
};



class System {
	template<uint32_t max_components, uint32_t max_entities> friend class DDECS;
protected:
	clock_t ms = 0;
public:
	virtual void init() = 0;
	virtual void run() = 0;
	virtual void destroy() {};
	virtual const char* getName() = 0;
	inline clock_t getTimeMS() const {
		return ms;
	}
	std::string getTimeMSStr() const {
		return std::to_string((int)ms);
	}
};