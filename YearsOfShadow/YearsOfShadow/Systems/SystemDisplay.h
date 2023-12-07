#pragma once
#include <iostream>



auto* serializeInstancedMesh(EntityID entity, uint32_t& outSize) {
	void* data = nullptr;
	ComponentID componentID = ecs.registerComponentAsBlittable("instancedMesh", sizeof(u32));
	u32 iMeshID = *(u32*)ecs.getEntityComponent(entity, componentID);
	auto iMeshHandle = instancedMeshCodex.get(iMeshID);
	const char* meshPath = HAL::get_instanced_mesh_name(iMeshHandle);
	size_t strSize = strlen(meshPath);
	HAL_ALLOC_RAWBYTE(data, strSize + 1);
	memcpy(data, meshPath, strSize);
	((uint8_t*)data)[strSize] = 0;  //null terminator...
	outSize = (uint32_t)strSize + 1;
	return data;
}
void deserializeInstancedMesh(EntityID entity, void* data, uint32_t size) {
	const char* cstr = (const char*)data;
	if (strlen(cstr) != size)
		HAL_PANIC("deserializeInstancedMesh()  input data's strlen != size: {}", size);
	ComponentID componentID = ecs.registerComponentAsBlittable("instancedMesh", sizeof(u32));
	if (ecs.entityHasComponent(entity, componentID) == true)
		return;
	uint32_t id = instancedMeshCodex.add(cstr);
	ecs.emplace(entity, componentID, &id);
}
auto* serializeMesh(EntityID entity, uint32_t& outSize) {
	void* data = nullptr;
	ComponentID componentID = ecs.registerComponentAsBlittable("mesh", sizeof(void*));
	HAL::mesh_handle_t meshHandle = *(HAL::mesh_handle_t*)ecs.getEntityComponent(entity, componentID);
	const char* meshPath = HAL::get_mesh_name(meshHandle);
	size_t strSize = strlen(meshPath);
	HAL_ALLOC_RAWBYTE(data, strSize + 1);
	memcpy(data, meshPath, strSize);
	((uint8_t*)data)[strSize] = 0;  //null terminator...
	outSize = (uint32_t)strSize + 1;
	return data;
}
void deserializeMesh(EntityID entity, void* data, uint32_t size) {
	const char* cstr = (const char*)data;
	if (strlen(cstr) != size)
		HAL_PANIC("deserializeMesh()  input data's strlen != size: {}", size);
	ComponentID componentID = ecs.registerComponentAsBlittable("mesh", sizeof(void*));
	if (ecs.entityHasComponent(entity, componentID) == true)
		return;
	uint32_t id = instancedMeshCodex.add(cstr);
	ecs.emplace(entity, componentID, &id);
}



class SystemDisplay : public System {
	ComponentID bodyComponentID;
	ComponentID meshComponentID;
	ComponentID instancedMeshComponentID;
public:
	virtual void init() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
		bodyComponentID = ecs.registerComponentAsBlittable("body", sizeof(BodyID));
		meshComponentID = ecs.registerComponentAsBlittable("mesh", sizeof(void*));
		instancedMeshComponentID = ecs.registerComponentAsBlittable("instancedMesh", sizeof(u32));
		SystemUtilities::SerialEntity::registerSerializeFunction(instancedMeshComponentID, serializeInstancedMesh);
		SystemUtilities::SerialEntity::registerDeSerializeFunction(instancedMeshComponentID, deserializeInstancedMesh);
	}
	virtual void run() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
		drawMeshes();
		drawInstancedMeshes();
	}
	virtual const char* getName() {
		return "SystemDisplay";
	}
private:
	void drawMeshes() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
		uint32_t meshCount = ecs.getComponentCount(meshComponentID);
		HAL::mesh_handle_t* meshBuffer = reinterpret_cast<HAL::mesh_handle_t*>(ecs.getComponentBuffer(meshComponentID));
		for (uint32_t i = 0; i < meshCount; i++) {
			EntityID owner = ecs.getOwner(meshComponentID, i);
			BodyID* bodyIDPtr = (BodyID*)ecs.getEntityComponent(owner, bodyComponentID);
			if (bodyIDPtr == nullptr) continue;
			Vec3D<float> pos = physics.getPosF(*bodyIDPtr);
			Vec3D<float> siz = physics.getSizeF(*bodyIDPtr);
			pos += siz / 2;
			HAL::set_mesh_position(meshBuffer[i], {pos.x, pos.y, pos.z});
			HAL::draw_mesh(meshBuffer[i]);
		}
	}
	void drawInstancedMeshes() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
		uint32_t instancedMeshCount = ecs.getComponentCount(instancedMeshComponentID);
		u32* instancedMeshBuffer = (u32*)ecs.getComponentBuffer(instancedMeshComponentID);
		std::unordered_map<u32, std::vector<Vec3D<float>>> instancePositionsMap;
		for (uint32_t i = 0; i < instancedMeshCount; i++) {
			EntityID owner = ecs.getOwner(instancedMeshComponentID, i);
			BodyID* bodyIDPtr = (BodyID*)ecs.getEntityComponent(owner, bodyComponentID);
			if (bodyIDPtr == nullptr) continue;
			Vec3D<float> pos = physics.getPosF(*bodyIDPtr);
			Vec3D<float> siz = physics.getSizeF(*bodyIDPtr);
			if (instancePositionsMap.find(instancedMeshBuffer[i]) == instancePositionsMap.end())
				instancePositionsMap[instancedMeshBuffer[i]] = {};
			pos += siz / 2;
			instancePositionsMap[instancedMeshBuffer[i]].push_back(pos);
		}
		for (auto& mapElem : instancePositionsMap) {
			u32 count = (u32)mapElem.second.size();
			auto instanceTypeID = instancedMeshCodex.get(mapElem.first);
			//EE_Point3Df scale = *(EE_Point3Df*)&instancedMeshCodex.getSize(mapElem.first);
			//EE_Point3Df scale = {100, 100, 100};
			//EE_setInstancedMeshScale(instanceTypeID, scale);
			const char* texturePath = instancedMeshCodex.getTexture(mapElem.first);
			HAL::set_instanced_mesh_submesh_texture(instanceTypeID, 0, "diffuse", texturePath);
			HAL::set_instanced_mesh_positions(instanceTypeID, std::span(reinterpret_cast<glm::vec3*>(std::data(mapElem.second)), count));
			HAL::draw_instanced_mesh(instanceTypeID);
		}
	}
};