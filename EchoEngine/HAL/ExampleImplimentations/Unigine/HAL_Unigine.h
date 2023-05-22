#pragma once

#include "../../HAL_3D.h"
#include "CustomUnigineThreadPool.h"
//#include "CustomWindowsThreadPool.h"

#include <UnigineObjects.h>
//#include "../../Framework/Components/Players/VRPlayerVR.h"
#include "../../../../Framework/Components/Players/VRPlayerVR.h"
#include <vector>
#include <iostream>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#include <windows.h>
#else
#include <X11/Xlib.h>
#endif

#include "../../../cxxpool/src/cxxpool.h"



//HAL
uint8_t keyMap[256];
void setupKeyMap() {
	using namespace Unigine;
	keyMap['W'] = Input::KEY::KEY_W;
	keyMap['A'] = Input::KEY::KEY_A;
	keyMap['S'] = Input::KEY::KEY_S;
	keyMap['D'] = Input::KEY::KEY_D;
	keyMap[32] = Input::KEY::KEY_SPACE;
}
uint8_t getKeyMapToUnigineInputKey(uint8_t k) {
	return keyMap[k];
}
bool EE_getKeyState(char k) {
	using namespace Unigine;
	return Input::isKeyPressed((Input::KEY)getKeyMapToUnigineInputKey(k));
}
void EE_getMouseState(uint8_t* leftButton, uint8_t* middleButton, uint8_t* rightButton) {
	using namespace Unigine;
	if(leftButton)
		*leftButton = Input::isMouseButtonDown(Input::MOUSE_BUTTON_LEFT);
	if(middleButton)
		*middleButton = Input::isMouseButtonDown(Input::MOUSE_BUTTON_MIDDLE);
	if(rightButton)
		*rightButton = Input::isMouseButtonDown(Input::MOUSE_BUTTON_RIGHT);
}
void EE_getMouseCanvasPos(int* x, int* y) {
	auto pos = Unigine::Input::getMousePosition();
	*x = pos.x; *y = pos.y;
}
void EE_setMouseCanvasPos(int x, int y) {
	Unigine::Input::setMousePosition({ x, y });
	Unigine::Input::setMouseCursorNeedUpdate(true);
	Unigine::Input::updateMouseCursor();
}
void EE_setMouseCursorHide(uint8_t isTrue) {
	Unigine::Input::setMouseCursorHide(true);
}
void EE_setMouseEnable(uint8_t isTrue) {
	Unigine::ControlsApp::setMouseEnabled(isTrue);
}

void EE_getCanvasSize(unsigned int* width, unsigned int* height) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
	*width = GetSystemMetrics(SM_CXSCREEN);
	*height = GetSystemMetrics(SM_CYSCREEN);
#else
	Display* disp = XOpenDisplay(NULL);
	Screen* scrn = DefaultScreenOfDisplay(disp);
	*width = scrn->width;
	*height = scrn->height;
#endif
}

const char* EE_getDirData() {
	return "./";
}

//void* EE_getNewThreadPool(uint16_t maxThreadCount) {
//	cxxpool::thread_pool* tp = new cxxpool::thread_pool(maxThreadCount);
//	//maxThreads = maxThreadCount;
//	return tp;
//}
//void EE_releaseThreadPool(void* self) {
//	delete (cxxpool::thread_pool*)self;
//}
//uint16_t EE_getThreadPoolFreeThreadCount(void* self) {
//	uint32_t threadCount = ((cxxpool::thread_pool*)self)->n_threads();
//	uint32_t activeThreadCount = threadCount - ((cxxpool::thread_pool*)self)->n_tasks();
//	if (activeThreadCount <= 0) return 0;
//	return (uint16_t)activeThreadCount;
//}
//bool EE_sendThreadPoolTask(void* self, void(*func)(void*), void* param) {
//	using namespace cxxpool;
//	thread_pool* tp = (thread_pool*)self;
//	auto future = tp->push(func, param);
//	//future._Abandon();
//	return true;
//}
//bool EE_isThreadPoolFinished(void* self) {
//	using namespace cxxpool;
//	thread_pool* tp = (thread_pool*)self;
//	if (tp->n_tasks())
//		return false;
//	return true;
//}

void* EE_getNewThreadPool(uint16_t maxThreadCount) {
	CustomUnigineThreadPool* tp = (CustomUnigineThreadPool*)malloc(sizeof(CustomUnigineThreadPool));
	tp->init(maxThreadCount);
	return tp;
}
void EE_releaseThreadPool(void* self) {
	CustomUnigineThreadPool* tp = (CustomUnigineThreadPool*)self;
	while (tp->allTasksFinished() == false)
		continue;
	free(tp);
}
uint16_t EE_getThreadPoolFreeThreadCount(void* self) {  //TODO: fix the logic
	CustomUnigineThreadPool* tp = (CustomUnigineThreadPool*)self;
	return tp->getFreeThreadCount();
}
bool EE_sendThreadPoolTask(void* self, void(*func)(void*), void* param) {
	CustomUnigineThreadPool* tp = (CustomUnigineThreadPool*)self;
	tp->addTask(func, param);
	return true;
}
bool EE_isThreadPoolFinished(void* self) {
	CustomUnigineThreadPool* tp = (CustomUnigineThreadPool*)self;
	return tp->allTasksFinished();
}

//void* EE_getNewThreadPool(uint16_t maxThreadCount) {
//	CustomWindowsThreadPool* tp = (CustomWindowsThreadPool*)malloc(sizeof(CustomWindowsThreadPool));
//	tp->init(maxThreadCount);
//	return tp;
//}
//void EE_releaseThreadPool(void* self) {
//	CustomWindowsThreadPool* tp = (CustomWindowsThreadPool*)self;
//	while (tp->allTasksFinished() == false)
//		continue;
//	free(tp);
//}
//uint16_t EE_getThreadPoolFreeThreadCount(void* self) {  //TODO: fix the logic
//	CustomWindowsThreadPool* tp = (CustomWindowsThreadPool*)self;
//	return tp->getFreeThreadCount();
//}
//bool EE_sendThreadPoolTask(void* self, void(*func)(void*), void* param) {
//	CustomWindowsThreadPool* tp = (CustomWindowsThreadPool*)self;
//	tp->addTask(func, param);
//	return true;
//}
//bool EE_isThreadPoolFinished(void* self) {
//	CustomWindowsThreadPool* tp = (CustomWindowsThreadPool*)self;
//	return tp->allTasksFinished();
//}



void EE_init() {
	setupKeyMap();
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
	//meshPtr->setScale({ sx, sy, sz });
	meshPtr->setWorldScale({ sx, sy, sz });
}

std::vector<Unigine::ObjectMeshClusterPtr> meshClusterPtrPool;  //buffer of ref counted smart pointers

void* EE_getNewInstancedMesh(const char* filePath) {
	ObjectMeshClusterPtr instancedMeshPtr = ObjectMeshCluster::create(filePath);
	//instancedMeshPtr->setWorldScale({ 50, 50, 50 });
	//instancedMeshPtr->setMaterialTexture("albedo", "Textures/photos_2017_10_6_fst_grass-texture-lawn.jpg", 0);
	void* meshID = (void*)meshClusterPtrPool.size();
	meshClusterPtrPool.push_back(instancedMeshPtr);
	instancedMeshPtr->setMaxVisibleDistance((16*3) / 2, 0);  //TODO: May need tuning.
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
		//transform = translate(pos) * scale(siz / 2);
		transform = translate(pos) * scale(siz);
		transforms.push_back(transform);
	}
	//cluster->appendMeshes(transforms);
	cluster->clearMeshes();
	cluster->createMeshes(transforms);
	cluster->setEnabled(true);
}
void EE_setInstancedMeshScale(void* meshID, EE_Point3Df scale) {
	auto& cluster = meshClusterPtrPool[(size_t)meshID];
	fixYZ(scale.y, scale.z);
	cluster->setWorldScale({ scale.x, scale.y, scale.z });
}
void EE_setInstancedSubmeshTexture(void* meshID, uint8_t submeshIndex, const char* textureType, const char* path) {
	auto& cluster = meshClusterPtrPool[(size_t)meshID];
	cluster->setMaterialTexture(textureType, path, 0);
}

void* EE_getNewCamera() {
	PlayerDummyPtr player = PlayerDummy::create();
	player->setPosition(Vec3(0, 0, -145));
	//player->setDirection(vec3(0.0f, -1.0f, -0.5f), vec3_up);
	player->setMainPlayer(true);
	void* retValue;
	memcpy(&retValue, &player, sizeof(player));
	return retValue;
}
void EE_cameraLookAt(void* self, float x, float y, float z) {
	PlayerDummyPtr player;
	memcpy(&player, &self, sizeof(player));
	fixYZ(y, z);

	//float t = x;
	//x = z;
	//z = -t;
	////y = -30;

	Vec3 atPos = { x, y, z };
	player->worldLookAt({x, y, z});

	//auto pos = player->getWorldPosition();
	//std::cout << "StartPos: " << pos.x << ", " << pos.y << ", " << pos.z
	//	<< "; AtPos: " << atPos.x << ", " << atPos.y << ", " << atPos.z << std::endl;
}
void EE_setCameraRotation(void* self, float rx, float ry, float rz) {
	PlayerDummyPtr player;
	memcpy(&player, &self, sizeof(player));
	fixYZ(ry, rz);
	//constexpr float C_PI = 3.14159265358979323846f;
	//rx = rx * ((float)C_PI / 180);
	//ry = ry * ((float)C_PI / 180);
	//rz = rz * ((float)C_PI / 180);
	//rx = rx * ((float)180 / C_PI);
	//ry = ry * ((float)180 / C_PI);
	//rz = rz * ((float)180 / C_PI);
	Unigine::Math::quat q = {};
	q.set(rx, ry, rz);
	vec3 vec;
	vec = q.getNormal();
	//player->setWorldRotation({ rx, ry, rz });
	//player->setWorldRotation(q);

	//player->setPosition(player->getPosition());
	//player->setDirection({ rx, ry, rz }, vec3_up);
	//player->setDirection(vec, vec3_up);

	//auto pos = player->getPosition();
	//Vec3 atPos = pos;
	//atPos.x *= rx; atPos.y *= ry; atPos.z *= rz;
	//player->worldLookAt(atPos);

	//player->setViewDirection({ rx, ry, rz });
	
	//CameraPtr cam = player->getCamera();
	//auto pos = player->getPosition();
	//player->setViewDirection({ rx, ry, rz });
	//player->setPosition(pos);  //TODO: verify if requierd
}
void EE_setCameraPos(void* self, float x, float y, float z) {
	PlayerDummyPtr player;
	memcpy(&player, &self, sizeof(player));
	fixYZ(y, z);
	CameraPtr cam = player->getCamera();
	//auto rot = player->getViewDirection();
	Unigine::Math::Vec3 pos = { x, y, z };
	pos *= visualScale;
	player->setWorldPosition(pos);
	//player->setViewDirection(rot);  //seems required for some reason..
}