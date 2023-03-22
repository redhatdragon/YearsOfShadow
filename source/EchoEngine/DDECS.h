#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include "FlatBuffer.h"
#include <time.h>

typedef uint32_t EntityID;
typedef uint32_t ComponentID;
class System;
typedef uint64_t EntityHandle;

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
	//std::unordered_map<std::vector<ComponentID>,std::vector<EntityID>> entityLists;
	std::vector<System*> systems;
	ComponentID handleComponentID = -1;
	uint64_t ticksPassed;
public:
	DDECS() {
		for (unsigned int i = 0; i < max_entities; i++) {
			entities[i].clear();
		}
		handleComponentID = registerComponent("handle", sizeof(EntityHandle));
		for (unsigned int i = 0; i < max_components; i++) {
			destructors[i] = nullptr;
		}
	}
	~DDECS() {
		size_t count = systems.size();
		for (size_t i = 0; i < count; i++) {
			delete systems[i];
		}
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
	ComponentID registerComponentUnsafe(const std::string& componentName, uint32_t size) {
		ComponentID retValue = { components.count };
		ComponentBuffer *buffer = &components[components.count];
		buffer->init(componentName, size);
		components.count++;
		return retValue;
	}
	ComponentID registerComponent(const std::string& componentName, uint32_t size) {
		ComponentID retValue = getComponentID(componentName);
		if (retValue == -1)
			return registerComponentUnsafe(componentName, size);
		//TODO: need to check for valid size, if false return -1
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
	void emplace(EntityID entity, ComponentID componentID, void* data) {
		if (entityHasComponent(entity, componentID)) {
			std::cout << "warning: entity already had component " << components[componentID].name
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

	uint64_t getTicksPassed() {
		return ticksPassed;
	}

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
		for (auto sys : systems) {
			clock_t start = clock();
			sys->run();
			sys->ms = clock() - start;
		}
		ticksPassed++;
	}
	void runSystems(std::vector<std::string>& blackList) {
		for (auto sys : systems) {
			clock_t start = clock();
			for (auto& str : blackList)
				if (str == sys->getName())
					goto skip;
			sys->run();
			skip:
			sys->ms = clock() - start;
		}
		ticksPassed++;
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
};



class System {
	template<uint32_t max_components, uint32_t max_entities> friend class DDECS;
protected:
	clock_t ms = 0;
public:
	virtual void init() {};
	virtual void run() = 0;
	virtual const char* getName() = 0;
	inline clock_t getTimeMS() const {
		return ms;
	}
	std::string getTimeMSStr() const {
		return std::to_string((int)ms);
	}
};