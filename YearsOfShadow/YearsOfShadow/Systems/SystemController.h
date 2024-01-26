#pragma once
#include <iostream>

class SystemController : public System {
	HAL::camera_handle_t EE_camera;

	std::vector<BodyID> castBuff;

public:
	virtual void init() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
		EE_camera = HAL::get_new_camera();
        HAL::use_camera(EE_camera);

		EntityID entity = ecs.getNewEntity();
		BodyID bodyID = physics.addBodyBox(30, 155-15, 30, "0.5f", 1, "0.5f", 
			to_void_ptr(entity), true);
		ecs.emplace(entity, bodyComponentID, &bodyID);
		Controller controller = {};
		controller.init();
		ecs.emplace(entity, controllerComponentID, &controller);
	}
	virtual void run() {
        OPTICK_THREAD("MainThread");
        OPTICK_EVENT();
		#if GAME_TYPE != GAME_TYPE_SERVER
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
		jumpPhase(controller, pos, bodyID);
		firePhase(controller, pos);
		#endif
	}
	virtual const char* getName() {
		return "SystemController";
	}
private:
	void turnPhase(Controller* controller, const Vec3D<FixedPoint<256 * 256>>& pos) {
		// EE_setMouseEnable(true);
		// EE_setMouseCursorHide(true); // Those were unimplemented
        Vec2D<int32_t> winDimensions;
        winDimensions.assign(HAL::get_canvas_size());

		Vec2D<int32_t> mousePos;
        mousePos.assign(HAL::get_mouse_canvas_pos());

		Vec2D<int32_t> mouseDelta = mousePos - (winDimensions / 2);
		Vec2D<float> mouseOffset = {
			(float)mouseDelta.x / ((float)winDimensions.x / 2),
			(float)mouseDelta.y / ((float)winDimensions.y / 2)
		};

		FPSCamera& cam = controller->cam;
		cam.setPosition({ pos.x.getAsFloat(), pos.y.getAsFloat(), pos.z.getAsFloat() });
		cam.ProcessMouseMovement(-mouseOffset.x, mouseOffset.y);
		auto lookAt = cam.getWorldLookAtPos();
        HAL::set_camera_position(EE_camera, {pos.x.getAsFloat(), pos.y.getAsFloat(), pos.z.getAsFloat()} );
        HAL::camera_look_at(EE_camera, {lookAt.x, lookAt.y, lookAt.z});

		HAL::set_mouse_canvas_pos({winDimensions.x / 2, winDimensions.y / 2});
	}
	void strafePhase(Controller* controller, const Vec3D<FixedPoint<256 * 256>>& pos, BodyID bodyID) {
		auto lookAtFloat = controller->cam.getLocalLookAtPos();
		Vec3D<FixedPoint<256 * 256>> dir = { lookAtFloat.x, lookAtFloat.y, lookAtFloat.z };
		dir.y = 0;
		dir *= 10;
		dir.normalize();
		Vec3D<FixedPoint<256 * 256>> right; right.cross(dir, {0, 1, 0});
		Vec3D<FixedPoint<256 * 256>> vel = {};
		if (HAL::get_key_state('W'))
			vel += dir;
        if (HAL::get_key_state('S'))
			vel -= dir;
        if (HAL::get_key_state('A'))
			vel += right;
        if (HAL::get_key_state('D'))
			vel -= right;
		vel *= "0.2f";
		Vec3D<FixedPoint<256 * 256>> originalVel = physics.getVelocity(bodyID);
		vel.y = originalVel.y;
		Vec3D<FixedPoint<256 * 256>> siz = physics.getSize(bodyID);
		Vec3D<FixedPoint<256 * 256>> pointPos = pos;
		pointPos += siz / 2;
		pointPos.y += siz.y / 2;
		pointPos.y += "0.01f";
		//if (controller->isFalling()) {
		//	if (physics.pointTrace(pointPos, bodyID)) {
		//		controller->setHasFloored();
		//		vel.y = 0;
		//	}
		//	else
		//		vel.y = originalVel.y;
		//} else if (controller->isFloored()) {
        //    if (HAL::get_key_state(HAL::key_code_t::key_space))
        //    {
		//		controller->setHasJumped();
		//		vel.y = -controller->getJumpDistPerTick();
		//	}
		//	else
		//		vel.y = 0;
		//} else if (controller->isJumping()) {
		//	vel.y = -controller->getJumpDistPerTick();
		//}
        auto belowPos = pos;
        belowPos += {siz.x / 2, siz.y, siz.z / 2};
        belowPos.y += "0.0001f";

		if (controller->state == Controller::IS_AIRBORNE)
        {
            bool canFall = !physics.pointTrace(belowPos, bodyID, castBuff);
            if (canFall == false)
            {
                controller->state = Controller::IS_FLOORED;
            }
        }
        if (controller->state == Controller::IS_FLOORED) {
            if (HAL::get_key_state(HAL::key_code_t::key_space))
            {
                controller->state = Controller::IS_AIRBORNE;
                vel.y = controller->getJumpVel();
            }
            else if (!physics.pointTrace(belowPos, bodyID, castBuff))
            {
                controller->state = Controller::IS_AIRBORNE;
			}
			else
				vel.y = 0;
        } else if (controller->state == Controller::IS_AIRBORNE) {  //Probably always true...
            vel.y = originalVel.y;
		}

		physics.setVelocity(bodyID, vel.x, vel.y, vel.z);
	}
	void jumpPhase(Controller* controller, const Vec3D<FixedPoint<256 * 256>>& pos, BodyID bodyID) {
        if (HAL::get_key_state(HAL::key_code_t::key_space))
        {

		}
	}
	void firePhase(Controller* controller, const Vec3D<FixedPoint<256 * 256>>& pos) {
		const auto leftMouse = HAL::get_mouse_left_state();
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
		BodyID bodyID = physics.addBodyBox(spawnAt.x, spawnAt.y, spawnAt.z, "0.1f", "0.1f", "0.1f",
			to_void_ptr(bomb), true);
		physics.setVelocity(bodyID, velocity.x, velocity.y, velocity.z);

		Explode explode;
		explode.init(Explode::ON_CONTACT | Explode::ON_TIMER, 60);

		std::string path = "./Data/Meshes/Props/Dynamite.obj";
		auto meshID = HAL::get_new_mesh(path.c_str());
		HAL::set_mesh_submesh_texture(meshID, 0, "diffuse", "./Data/Meshes/Props/D_Dynamite.png");
        HAL::set_mesh_scale(meshID, {0.1f, 0.1f, 0.1f});

		auto plHandle = HAL::get_new_light_point();
		HAL::set_light_point_dist(plHandle, 40);  //ignored in shader...
		HAL::set_light_point_intensity(plHandle, 0.5f);
		HAL::set_light_point_pos(plHandle,
			{ spawnAt.x.getAsFloat(), spawnAt.y.getAsFloat(), spawnAt.z.getAsFloat() });
		PointLight pointLight;
		pointLight.pointLightHandle = plHandle;

		ecs.emplace(bomb, bodyComponentID, &bodyID);
		ecs.emplace(bomb, explodeComponentID, &explode);
		ecs.emplace(bomb, meshComponentID, &meshID);
		ecs.emplace(bomb, pointLightComponentID, &pointLight);
	}

	bool isFloored() {
		return true;
	}
};