#pragma once
#include <iostream>

class SystemDisplay : public System {
	ComponentID bodyComponentID;
	ComponentID meshComponentID;
	ComponentID instancedMeshComponentID;
public:
	virtual void init() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
		bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		meshComponentID = ecs.registerComponent("mesh", sizeof(void*));
		instancedMeshComponentID = ecs.registerComponent("instancedMesh", sizeof(u32));
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