#pragma once
#include <iostream>

class SystemDisplay : public System {
	ComponentID bodyComponentID;
	ComponentID meshComponentID;
	ComponentID instancedMeshComponentID;
public:
	virtual void init() {
		bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		meshComponentID = ecs.registerComponent("mesh", sizeof(void*));
		instancedMeshComponentID = ecs.registerComponent("instancedMesh", sizeof(u32));
	}
	virtual void run() {
		drawMeshes();
		drawInstancedMeshes();
	}
	virtual const char* getName() {
		return "SystemDisplay";
	}
private:
	void drawMeshes() {
		uint32_t meshCount = ecs.getComponentCount(meshComponentID);
		void** meshBuffer = (void**)ecs.getComponentBuffer(meshComponentID);
		for (uint32_t i = 0; i < meshCount; i++) {
			EntityID owner = ecs.getOwner(meshComponentID, i);
			BodyID* bodyIDPtr = (BodyID*)ecs.getEntityComponent(owner, bodyComponentID);
			if (bodyIDPtr == nullptr) continue;
			Vec3D<float> pos = physics.getPosF(*bodyIDPtr);
			Vec3D<float> siz = physics.getSizeF(*bodyIDPtr);
			pos += siz / 2;
			EE_setPositionMesh(meshBuffer[i], pos.x, pos.y, pos.z);
			EE_drawMesh(meshBuffer[i]);
		}
	}
	void drawInstancedMeshes() {
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
			void* instanceTypeID = instancedMeshCodex.get(mapElem.first);
			//EE_Point3Df scale = *(EE_Point3Df*)&instancedMeshCodex.getSize(mapElem.first);
			//EE_Point3Df scale = {100, 100, 100};
			//EE_setInstancedMeshScale(instanceTypeID, scale);
			const char* texturePath = instancedMeshCodex.getTexture(mapElem.first);
			EE_setInstancedSubmeshTexture(instanceTypeID, 0, "diffuse", texturePath);
			EE_setInstancedMeshPositions(instanceTypeID, (EE_Point3Df*)&mapElem.second[0], count);
			EE_drawInstancedMesh(instanceTypeID);
		}
	}
};