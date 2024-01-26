#pragma once
#include "../FPSCamera.h"

struct PointLight {
	void* pointLightHandle;
	//Vec3D<FixedPoint<>> pos;
	//FixedPoint<> dist, intensity;
	//uint8_t r, g, b, dummy;
};
struct Controller {
	FPSCamera cam;
	GameTick lastFired;
	GameTick fireRate;
	GameTick lastJumped;
	//uint16_t jumpDuration;
	//uint16_t jumpDistance;

	enum {
		//IS_JUMPING,
		//IS_FALLING,
		IS_AIRBORNE,
		IS_FLOORED
	} state;

	inline void init() {
		cam.init();
		lastFired = 0;
		fireRate = 20;
		lastJumped = 0;
		//jumpDuration = 60;
		//jumpDistance = 2;
		//state = IS_FALLING;
	}

	FixedPoint<256 * 256> getJumpVel() {
		//Based on...  v = sqrt(2g * h);
		//TODO: figure out how to make h runtime dynamic later.
		FixedPoint<256 * 256> g = "0.40f";
		g *= "1.414f";
		FixedPoint<256 * 256> h = "1.22f";
		FixedPoint<256 * 256> retValue = g * h;
		retValue /= 3;
		return -retValue;
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
struct EnemyAI {
	enum ACTIVE_STATE : u8 {
		ASLEEP,
		IDLE,
		WALKING,
		RUNNING,
		FERAL
	} active_state;
	enum JUMP_STATE : u8 {
		IS_JUMPING,
		IS_FALLING,
		IS_FLOORED
	} jump_state;
	u8 visualDistance;
	u8 auditorySensitivity;
	EntityHandle targetHandle;
	EntityID targetEntity;
	GameTick lastPositionChoice;
	Vec3D<FixedPoint<256 * 256>> gotoPos;
	GameTick lastJumped;
	u8 jumpDuration;
	u8 jumpDistance;
	void init() {
		active_state = IDLE;
		visualDistance = 16;
		targetHandle = -1;
		targetEntity = -1;
		lastPositionChoice = 0;
	}
};





//Initialized in Components.cpp
//All are given an initial value of -2 to force crashing if something didn't get registered.
extern ComponentID handleComponentID;
extern ComponentID replicateEntityComponentID;
extern ComponentID bodyComponentID;
extern ComponentID meshComponentID;
extern ComponentID instancedMeshComponentID;
extern ComponentID controllerComponentID;
extern ComponentID enemyAIComponentID;
extern ComponentID explodeComponentID;
extern ComponentID pointLightComponentID;
extern ComponentID deadComponentID;

ComponentID handleComponentID = -2;
ComponentID replicateEntityComponentID = -2;
ComponentID bodyComponentID = -2;
ComponentID meshComponentID = -2;
ComponentID instancedMeshComponentID = -2;
ComponentID controllerComponentID = -2;
ComponentID enemyAIComponentID = -2;
ComponentID explodeComponentID = -2;
ComponentID pointLightComponentID = -2;
ComponentID deadComponentID = -2;

void registerAllComponents() {
	replicateEntityComponentID = ecs.registerComponentAsBlittable("replicateEntity", sizeof(ReplicateEntity));
	bodyComponentID = ecs.registerComponentAsBlittable("body", sizeof(BodyID));
	meshComponentID = ecs.registerComponentAsBlittable("mesh", sizeof(void*));
	instancedMeshComponentID = ecs.registerComponentAsBlittable("instancedMesh", sizeof(u32));
	controllerComponentID = ecs.registerComponentAsBlittable("controller", sizeof(Controller));
	enemyAIComponentID = ecs.registerComponentAsBlittable("enemyAI", sizeof(EnemyAI));
	explodeComponentID = ecs.registerComponentAsBlittable("explode", sizeof(Explode));
	pointLightComponentID = ecs.registerComponentAsBlittable("pointLight", sizeof(PointLight));
	deadComponentID = ecs.registerComponentAsBlittable("dead", NULL);
}