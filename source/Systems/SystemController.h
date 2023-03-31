#pragma once
#include INC_UTILS_PATH(ENGINE_PATH, DDECS.h)
#include INC_UTILS_PATH(ENGINE_PATH, PhysicsEngineAABB3D.h)
//#include "../FP_FPSCamera.h"
#include "../FPSCamera.h"
#include <iostream>

struct Controller {
	//Vec3D<FixedPoint<256 * 256>> pos, rot;
	//Vec3D<FixedPoint<256 * 256>> rot;
	FPSCamera cam;
};

class SystemController : public System {
	ComponentID bodyComponentID;
	ComponentID controllerComponentID;
	void* EE_camera;
	float x;

public:
	virtual void init() {
		bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		controllerComponentID = ecs.registerComponent("controller", sizeof(Controller));
		Unigine::Input::getMouseDeltaPosition();
		EE_camera = EE_getNewCamera();
		x = 0;

		EntityID entity = ecs.getNewEntity();
		BodyID bodyID = physics.addStaticBodyBox(30, 155, 30, 1, 1, 1);
		ecs.emplace(entity, bodyComponentID, &bodyID);
		Controller controller = {};
		//controller.cam.init({ 30, 155, 30 });  //useful?
		controller.cam.init();
		ecs.emplace(entity, controllerComponentID, &controller);

		EE_setMouseEnable(true);
		EE_setMouseCursorHide(true);
	}
	virtual void run() {
		clock_t end;
		float time = (float)(clock() - end) / (float)CLOCKS_PER_SEC;
		//std::cout << time << std::endl;

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
		//std::cout << winDimensions.getDebugStr<int32_t>() << std::endl;

		FPSCamera& cam = controller->cam;
		float mouseX = 1.0f, mouseY = 1.0f;
		cam.setPosition(pos.x.getAsFloat(), pos.y.getAsFloat(), pos.z.getAsFloat());
		//cam.ProcessMouseMovement(mouseX, mouseY);
		cam.ProcessMouseMovement(mouseOffset.x, mouseOffset.y);
		Vec3D<float> newRotF;
		cam.getWorldLookAtPos(newRotF.x, newRotF.y, newRotF.z);
		EE_setCameraPos(EE_camera, pos.x.getAsFloat(), pos.y.getAsFloat(), pos.z.getAsFloat());
		EE_cameraLookAt(EE_camera, newRotF.x, newRotF.y, newRotF.z);

		EE_setMouseCanvasPos(winDimensions.x / 2, winDimensions.y / 2);

		end = clock();
	}
	virtual const char* getName() {
		return "SystemController";
	}
private:
	void loopBetween(float& input, float max) {
		if (input > max) {
			//input = -(input - (input / max) * max);
			input = -max;
			return;
		}
		if (input < -max) {
			//input = -(input + (-input / max) * max);
			input = max;
		}
	}
	void radianToDegree(float& input) {
		input = input * ((float)180 / 3.141592653589793238463);
	}
	void degreeToRadian(float& input) {
		input = input * ((float)3.141592653589793238463 / 180);
	}
};