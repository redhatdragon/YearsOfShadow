#pragma once

#include "../../HAL_3D.h"

#include <UnigineObjects.h>
#include "../../Framework/Components/Players/VRPlayerVR.h"
#include <vector>



//HAL
const char* EE_getDirData() {
	return "./";
}



//HAL_3D

//const float visualScale = 0.01f;
const float visualScale = 1;
std::vector<Unigine::ObjectMeshDynamicPtr> meshPtrPool;  //buffer of ref counted smart pointers

inline void fixYZ(float& y, float& z) {
	float t = y;
	y = z;
	z = t;
	y = -y;  //TODO: needed?
	z = -z;
}
inline void fixYZ(double& y, double& z) {
	double t = y;
	y = z;
	z = t;
	y = -y;
	z = -z;
}

void* EE_getNewMesh(const char* filePath) {
	//Unigine::ObjectMeshStaticPtr meshPtr = Unigine::ObjectMeshStatic::create(filePath);
	Unigine::ObjectMeshDynamicPtr meshPtr = Unigine::ObjectMeshDynamic::create(filePath);
	//void* meshID = (void*)meshPtrPool.size();
	void* meshID;
	memcpy(&meshID, &meshPtr, sizeof(void*));
	meshPtrPool.push_back(meshPtr);
	EE_setScaleMesh(meshID, 1, 1, 1);
	return meshID;
}
void EE_releaseMesh(void* meshID) {
	uint32_t count = meshPtrPool.size();
	for (uint32_t i = 0; i < count; i++) {
		auto& meshPtr = meshPtrPool[i];
		if (meshPtr == meshID) {
			meshPtr->deleteLater();  //TODO: consider deleteForce
			meshPtr = meshPtrPool[meshPtrPool.size() - 1];
			meshPtrPool.pop_back();
		}
	}
}
void EE_drawMesh(void* meshID) {
	Unigine::ObjectMeshDynamicPtr meshPtr;
	memcpy(&meshPtr, &meshID, sizeof(void*));
	meshPtr->setEnabled(true);
}
//void translateMesh(void* meshID, float x, float y, float z) {
//	meshPtrPool[(size_t)meshID]->translate({ x * visualScale, y * visualScale, z * visualScale });
//}
void EE_setTextureSubmesh(void* meshID, uint8_t submeshIndex, const char* textureType, const char* path) {
	Unigine::ObjectMeshDynamicPtr meshPtr;
	memcpy(&meshPtr, &meshID, sizeof(void*));
	//mesh->setMaterialTexture("diffuse", path, submeshIndex);
	//mesh->setMaterialTexture("albedo", "core/textures/common/noise.dds", submeshIndex);
	meshPtr->setMaterialTexture(textureType, path, submeshIndex);

	//// update texture
	//MaterialPtr material = mesh->getMaterialInherit(0);
	//int num = material->findTexture("albedo");
	//if (num != -1)
	//	material->setTextureImage(num, image);
}
void EE_setPositionMesh(void* meshID, float x, float y, float z) {
	fixYZ(y, z);
	Unigine::ObjectMeshDynamicPtr meshPtr;
	memcpy(&meshPtr, &meshID, sizeof(void*));
	meshPtr->setWorldPosition({ x * visualScale, y * visualScale, z * visualScale });

	//Mat4 mat;
	//dvec3 pos = { x * visualScale, y * visualScale, z * visualScale };
	//dvec3 siz = { 50, 50, 50 };
	//mat = translate(pos) * scale(siz);
	//meshPtrPool[(size_t)meshID]->setTransform(mat);
	// 
	//meshPtrPool[(size_t)meshID]->translate({ x * visualScale, y * visualScale, z * visualScale });
}
void rotateMesh(void* meshID, float rx, float ry, float rz) {
	fixYZ(ry, rz);
	Unigine::ObjectMeshDynamicPtr meshPtr;
	memcpy(&meshPtr, &meshID, sizeof(void*));
	meshPtr->rotate(rx, ry, rz);
}
void EE_setRotationMesh(void* meshID, float rx, float ry, float rz) {
	fixYZ(ry, rz);
	//meshPtrPool[(size_t)meshID]->setRotation(rx, ry, rz);
	throw;
}
void EE_setScaleMesh(void* meshID, float sx, float sy, float sz) {
	fixYZ(sy, sz);
	//meshPtrPool[(size_t)meshID]->setScale({ sx*visualScale, sy*visualScale, sz*visualScale });
	//meshPtrPool[(size_t)meshID]->setScale({ sx*visualScale*100, sy*visualScale*100, sz*visualScale*100 });
	Unigine::ObjectMeshDynamicPtr meshPtr;
	memcpy(&meshPtr, &meshID, sizeof(void*));
	meshPtr->setScale({ sx, sy, sz });
}

std::vector<Unigine::ObjectMeshClusterPtr> meshClusterPtrPool;  //buffer of ref counted smart pointers

void* EE_getNewInstancedMesh(const char* filePath) {
	ObjectMeshClusterPtr instancedMeshPtr = ObjectMeshCluster::create(filePath);
	//instancedMeshPtr->setWorldScale({ 50, 50, 50 });
	//instancedMeshPtr->setMaterialTexture("albedo", "Textures/photos_2017_10_6_fst_grass-texture-lawn.jpg", 0);
	void* meshID = (void*)meshClusterPtrPool.size();
	meshClusterPtrPool.push_back(instancedMeshPtr);
	instancedMeshPtr->setMaxVisibleDistance((16*12) / 2, 0);
	return meshID;
}
void EE_releaseInstancedMesh(void* meshID) {
	meshClusterPtrPool[(size_t)meshID]->deleteLater();  //TODO: consider deleteForce
	meshClusterPtrPool[(size_t)meshID] = meshClusterPtrPool[meshClusterPtrPool.size() - 1];
	meshClusterPtrPool.pop_back();
}
void EE_setInstancedMeshPositions(void* meshID, const EE_Point3Df* posBuffer, uint32_t count) {
	auto& cluster = meshClusterPtrPool[(size_t)meshID];
	Vector<Mat4> transforms;
	auto clusterWorldScale = cluster->getWorldScale();
	transforms.reserve(count);
	for (uint32_t i = 0; i < count; i++) {
		const EE_Point3Df* posPtr = &posBuffer[i];
		dvec3 pos = dvec3(posPtr->x * visualScale, posPtr->y * visualScale, posPtr->z * visualScale);
		fixYZ(pos.y, pos.z);
		Mat4 transform = Mat4(1);
		dvec3 rot = dvec3(1, 1, 1);
		dvec3 siz = dvec3(clusterWorldScale);
		transform = translate(pos) * scale(siz / 2);
		transforms.push_back(transform);
	}
	//cluster->appendMeshes(transforms);
	cluster->clearMeshes();
	cluster->createMeshes(transforms);
	cluster->setEnabled(true);
}
void EE_setInstancedMeshScale(void* meshID, EE_Point3Df scale) {
	auto& cluster = meshClusterPtrPool[(size_t)meshID];
	cluster->setWorldScale({ scale.x, scale.y, scale.z });
}
void EE_setInstancedSubmeshTexture(void* meshID, uint8_t submeshIndex, const char* textureType, const char* path) {
	auto& cluster = meshClusterPtrPool[(size_t)meshID];
	cluster->setMaterialTexture(textureType, path, 0);
}

void EE_setCameraRotation(float rx, float ry, float rz) {
	fixYZ(ry, rz);
	/*VRPlayer* p = VRPlayerVR::get();
	CameraPtr cam = p->getPlayer()->getCamera();
	NodePtr head = p->getHead();
	Unigine::Math::Vec3 headPos = head->getPosition();

	Unigine::Math::Vec3 camPos = cam->getPosition();*/
	PlayerPtr p = Unigine::Game::getPlayer();
	CameraPtr cam = p->getCamera();
	auto pos = p->getPosition();
	p->setViewDirection({ rx, ry, rz });
	p->setPosition(pos);  //TODO: verify if requierd
}
//void rotateCamera(float rx, float ry, float rz) {
//
//}
void EE_setCameraPos(void* self, float x, float y, float z) {
	fixYZ(y, z);
	//VRPlayer* p = VRPlayerVR::get();
	PlayerPtr p = Unigine::Game::getPlayer();

	//CameraPtr cam = p->getPlayer()->getCamera();
	CameraPtr cam = p->getCamera();
	auto rot = p->getViewDirection();
	Unigine::Math::Vec3 pos = { x, y, z };
	pos *= visualScale;
	//p->setPosition(pos);
	//p->setWorldPosition(p->getWorldPosition()-pos);
	p->setWorldPosition(pos);
	p->setViewDirection(rot);  //seems required for some reason..

	//cam->setPosition({ x, y, z });
}
//void getRotationCamera(void* self, float* rx, float* ry, float* rz) {
//
//}