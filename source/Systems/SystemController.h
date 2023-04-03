#pragma once
#include INC_UTILS_PATH(ENGINE_PATH, DDECS.h)
#include INC_UTILS_PATH(ENGINE_PATH, PhysicsEngineAABB3D.h)
//#include "../FP_FPSCamera.h"  //Postoned till fixed
#include "../FPSCamera.h"
#include <iostream>

struct Controller {
	FPSCamera cam;
};

class SystemController : public System {
	ComponentID bodyComponentID;
	ComponentID controllerComponentID;
	void* EE_camera;

public:
	virtual void init() {
		bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		controllerComponentID = ecs.registerComponent("controller", sizeof(Controller));
		Unigine::Input::getMouseDeltaPosition();
		EE_camera = EE_getNewCamera();

		EntityID entity = ecs.getNewEntity();
		BodyID bodyID = physics.addStaticBodyBox(30, 155, 30, 1, 1, 1);
		ecs.emplace(entity, bodyComponentID, &bodyID);
		Controller controller = {};
		controller.cam.init();
		ecs.emplace(entity, controllerComponentID, &controller);
	}
	virtual void run() {
		EE_setMouseEnable(true);
		EE_setMouseCursorHide(true);
		if (ecs.getComponentCount(controllerComponentID) == 0)
			return;
		Controller* controller = (Controller*)ecs.getComponentBuffer(controllerComponentID);
		EntityID owner = ecs.getOwner(controllerComponentID, 0);
		if (ecs.entityHasComponent(owner, bodyComponentID) == 0) {
			std::cout << "Error: SystemController::run(), entity with controller lacks a body!" << std::endl;
			return;
		}
		BodyID bodyID = *(BodyID*)ecs.getEntityComponent(owner, bodyComponentID);
		Vec3D<FixedPoint<256*256>> pos = physics.getPos<FixedPoint<256*256>>(bodyID);

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
	virtual const char* getName() {
		return "SystemController";
	}
private:
};