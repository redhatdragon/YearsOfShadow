#pragma once
#include <UnigineEngine.h>
#include <UnigineEditor.h>
#include <UniginePlayers.h>
#include <UnigineLogic.h>

#include "VRPlayerVR.h"
#include "AppViveProxy.h"
#include "EyetrackingProxy.h"

class VRPlayerVive : public VRPlayerVR
{
public:
	COMPONENT(VRPlayerVive, VRPlayerVR);
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);

	// property
	PROP_NAME("player_vive");
	PROP_AUTOSAVE(0);

	// parameters
	PROP_PARAM(Node, basestation_0);
	PROP_PARAM(Node, basestation_1);
	
	// player
	void setLock(int lock) override;
	void setPlayerPosition(const Unigine::Math::Vec3 &pos) override;
	void landPlayerTo(const Unigine::Math::Vec3 &position, const Unigine::Math::vec3 &direction) override;

	// hands
	Unigine::Math::Mat4 getControllerTransform(int controller_num) override;
	int getControllerButton(int controller_num, int button) override;
	float getControllerAxis(int controller_num, int button) override;
	void vibrateController(int controller_num, float amplitude = 1.0f) override;

	virtual const Unigine::Math::Vec3& getHeadOffset() const { return head_offset; }

protected:
	// main loop
	void init();
	void update();

private:
	void set_controller_trigger_position(Unigine::NodeReferencePtr &controller, float position);
	void find_devices();

	void basestation_update(int num, const Unigine::Math::Mat4 &player_transform, int device_id);
	void controller_update(int num, const Unigine::Math::Mat4 &player_transform, int device_id);

	int vive_getControllerDPadPressed(int device, int button);

	AppViveProxy vive;
	EyetrackingProxy eyetracking;
	Unigine::NodeReferencePtr basestation[CONTROLLER_COUNT];

	Unigine::Math::Vec3 head_offset;

	int BASESTATION_DEVICE_0 = -1,
		BASESTATION_DEVICE_1 = -1,
		CONTROLLER_DEVICE_0 = -1,
		CONTROLLER_DEVICE_1 = -1,
		HMD_DEVICE_0 = -1;
};