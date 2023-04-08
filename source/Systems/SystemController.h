#pragma once
#include INC_UTILS_PATH(ENGINE_PATH, DDECS.h)
#include INC_UTILS_PATH(ENGINE_PATH, PhysicsEngineAABB3D.h)
//#include "../FP_FPSCamera.h"  //Postoned till fixed
#include "../FPSCamera.h"
#include <iostream>

struct Controller {
	FPSCamera cam;
	GameTick lastFired;
	GameTick fireRate;

	inline void init() {
		cam.init();
		lastFired = 0;
		fireRate = 20;
	}
	inline bool canFire() {
		if (lastFired + fireRate <= ecs.getTicksPassed())
			return true;
		return false;
	}
	inline void setHasFired() {
		lastFired = ecs.getTicksPassed();
	}
};
struct Explode {
	GameTick explodeTime;
	enum EXPLOSION_TRIGGERS : uint8_t {
		ON_CONTACT,
		ON_TIMER,
		ON_DAMAGE
	};
	uint8_t triggers;

	//Example: init(Explode::ON_CONTACT | EXPLODE | ON_TIMER, 5);
	//explodes the owning entity in 5 game ticks or on contact with another physics body
	void init(uint8_t _triggers, GameTick _explodeTimer = 0) {
		triggers = _triggers;
		explodeTime = ecs.getTicksPassed() + _explodeTimer;
	}
	bool canExplode() {
		if (explodeTime <= ecs.getTicksPassed())
			return true;
		return false;
	}
};

class SystemController : public System {
	ComponentID bodyComponentID;
	ComponentID controllerComponentID;
	ComponentID explodeComponentID;
	ComponentID meshComponentID;
	void* EE_camera;

public:
	virtual void init() {
		bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		controllerComponentID = ecs.registerComponent("controller", sizeof(Controller));
		explodeComponentID = ecs.registerComponent("explode", sizeof(Explode));
		meshComponentID = ecs.registerComponent("mesh", sizeof(void*));
		EE_camera = EE_getNewCamera();

		EntityID entity = ecs.getNewEntity();
		//BodyID bodyID = physics.addBodyBox(30, 155-15, 30, 1, 1, 1);
		BodyID bodyID = physics.addBodyBox(30, 155-15, 30, "0.5f", 1, "0.5f");
		physics.setSolid(bodyID, true);
		ecs.emplace(entity, bodyComponentID, &bodyID);
		Controller controller = {};
		controller.init();
		ecs.emplace(entity, controllerComponentID, &controller);
	}
	virtual void run() {
		if (ecs.getComponentCount(controllerComponentID) == 0)
			return;
		Controller* controller = (Controller*)ecs.getComponentBuffer(controllerComponentID);
		EntityID owner = ecs.getOwner(controllerComponentID, 0);
		if (ecs.entityHasComponent(owner, bodyComponentID) == 0) {
			std::cout << "Error: SystemController::run(), entity with controller lacks a body!" << std::endl;
			return;
		}
		BodyID bodyID = *(BodyID*)ecs.getEntityComponent(owner, bodyComponentID);
		Vec3D<FixedPoint<256*256>> pos = physics.getPos(bodyID);
		auto siz = physics.getSize(bodyID);
		pos += siz / 2;

		turnPhase(controller, pos);
		strafePhase(controller, pos, bodyID);
		jumpPhase(controller, pos);
		firePhase(controller, pos);
	}
	virtual const char* getName() {
		return "SystemController";
	}
private:
	void turnPhase(Controller* controller, const Vec3D<FixedPoint<256 * 256>>& pos) {
		EE_setMouseEnable(true);
		EE_setMouseCursorHide(true);
		Vec2D<int32_t> winDimensions;
		EE_getCanvasSize((uint32_t*)&winDimensions.x, (uint32_t*)&winDimensions.y);
		Vec2D<int32_t> mousePos;
		EE_getMouseCanvasPos(&mousePos.x, &mousePos.y);
		Vec2D<int32_t> mouseDelta = mousePos - (winDimensions / 2);
		Vec2D<float> mouseOffset = {
			(float)mouseDelta.x / ((float)winDimensions.x / 2),
			(float)mouseDelta.y / ((float)winDimensions.y / 2)
		};

		FPSCamera& cam = controller->cam;
		cam.setPosition({ pos.x.getAsFloat(), pos.y.getAsFloat(), pos.z.getAsFloat() });
		//cam.setPosition(pos);
		cam.ProcessMouseMovement(mouseOffset.x, mouseOffset.y);
		auto lookAt = cam.getWorldLookAtPos();
		EE_setCameraPos(EE_camera, pos.x.getAsFloat(), pos.y.getAsFloat(), pos.z.getAsFloat());
		EE_cameraLookAt(EE_camera, lookAt.x, lookAt.y, lookAt.z);
		//EE_cameraLookAt(EE_camera, lookAt.x.getAsFloat(), lookAt.y.getAsFloat(), lookAt.z.getAsFloat());

		EE_setMouseCanvasPos(winDimensions.x / 2, winDimensions.y / 2);
	}
	void strafePhase(Controller* controller, const Vec3D<FixedPoint<256 * 256>>& pos, BodyID bodyID) {
		auto lookAtFloat = controller->cam.getLocalLookAtPos();
		Vec3D<FixedPoint<256 * 256>> dir = { lookAtFloat.x, lookAtFloat.y, lookAtFloat.z };
		dir.y = 0;
		dir *= 10;
		dir.normalize();
		Vec3D<FixedPoint<256 * 256>> right; right.cross(dir, {0, 1, 0});
		Vec3D<FixedPoint<256 * 256>> vel = {};
		if (EE_getKeyState('W'))
			vel += dir;
		if (EE_getKeyState('S'))
			vel -= dir;
		if (EE_getKeyState('A'))
			vel -= right;
		if (EE_getKeyState('D'))
			vel += right;
		vel *= "0.1f";
		//if (isFloored())
		//	vel.y = 0;
		if (vel.isZero() == false) {
			vel.y = 0;
		}
		else {
			Vec3D<FixedPoint<256 * 256>> originalVel = physics.getVelocity(bodyID);
			vel.y = originalVel.y;
		}
		physics.setVelocity(bodyID, vel.x, vel.y, vel.z);
	}
	void jumpPhase(Controller* controller, const Vec3D<FixedPoint<256 * 256>>& pos) {

	}
	void firePhase(Controller* controller, const Vec3D<FixedPoint<256 * 256>>& pos) {
		uint8_t leftMouse;
		EE_getMouseState(&leftMouse, nullptr, nullptr);
		if (leftMouse == false)
			return;
		if (controller->canFire() == false)
			return;
		controller->setHasFired();
		auto lookAtFloat = controller->cam.getLocalLookAtPos();
		Vec3D<FixedPoint<256 * 256>> spawnAt = { lookAtFloat.x, lookAtFloat.y, lookAtFloat.z };
		Vec3D<FixedPoint<256 * 256>> direction = spawnAt;
		FixedPoint<256 * 256> spawnDistance = "0.5f";
		spawnAt *= spawnDistance;
		spawnAt += pos;
		spawnBomb(spawnAt, direction);
	}

	void spawnBomb(const Vec3D<FixedPoint<256 * 256>>& spawnAt, const Vec3D<FixedPoint<256 * 256>>& direction) {
		EntityID bomb = ecs.getNewEntity();
		Vec3D<FixedPoint<256 * 256>> velocity = direction;
		velocity *= "0.25f";
		BodyID bodyID = physics.addBodyBox(spawnAt.x, spawnAt.y, spawnAt.z, "0.1f", "0.1f", "0.1f");
		//BodyID bodyID = physics.addBodyBox(spawnAt.x, spawnAt.y, spawnAt.z, 1, 1, 1);
		physics.setSolid(bodyID, true);
		physics.setUserData(bodyID, (void*)bomb);
		physics.setVelocity(bodyID, velocity.x, velocity.y, velocity.z);

		Explode explode;
		explode.init(Explode::ON_CONTACT | Explode::ON_TIMER, 60);
		//std::string meshPath = EE_getDirData();
		std::string meshPath = "";
		meshPath += "Meshes/Cube2.fbx/cube.001.mesh";
		void* meshID = EE_getNewMesh(meshPath.c_str());
		EE_setScaleMesh(meshID, 50 / 10, 50 / 10, 50 / 10);
		ecs.emplace(bomb, bodyComponentID, &bodyID);
		ecs.emplace(bomb, explodeComponentID, &explode);
		ecs.emplace(bomb, meshComponentID, &meshID);
	}

	bool isFloored() {
		return true;
	}
};