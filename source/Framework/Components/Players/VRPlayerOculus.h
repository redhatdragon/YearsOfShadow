#pragma once
#include <UnigineEngine.h>
#include <UnigineEditor.h>
#include <UniginePlayers.h>
#include <UnigineLogic.h>

#include "VRPlayerVR.h"
#include "OculusProxy.h"

class VRPlayerOculus : public VRPlayerVR
{
public:
	COMPONENT(VRPlayerOculus, VRPlayerVR);
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);
	COMPONENT_POST_UPDATE(postUpdate);

	// property
	PROP_NAME("player_oculus");
	PROP_AUTOSAVE(0);

	// parameters
	PROP_PARAM(Node, warning_message);
	PROP_PARAM(Node, success_message);
	
	// player
	void setLock(int lock) override;
	void setPlayerPosition(const Unigine::Math::Vec3 &pos) override;
	void landPlayerTo(const Unigine::Math::Vec3 &position, const Unigine::Math::vec3 &direction) override;

	// hands
	Unigine::Math::Mat4 getControllerTransform(int controller_num) override;
	int getControllerButton(int controller_num, int button) override;
	float getControllerAxis(int controller_num, int button) override;
	void vibrateController(int controller_num, float amplitude = 1.0f) override;

	UNIGINE_INLINE Unigine::Math::vec3 getHandyPos() override { return Unigine::Math::vec3(0.007f, -0.018f, 0.018f); }
	UNIGINE_INLINE Unigine::Math::quat getHandyRot() override { return Unigine::Math::quat(33.0f, 0, 0); }

	virtual const Unigine::Math::Vec3& getHeadOffset() const { return head_offset; }

protected:
	// main loop
	void init();
	void update();
	void postUpdate();

private:
	// height calibration
	float show_message_timer = 0;
	float show_message_duration = 2.0f;
	void show_object_in_front_hmd(const Unigine::NodePtr &obj);
	void height_calibration_init();
	void height_calibration_update();

	OculusProxy oculus;
	Unigine::Math::Vec3 head_offset;

	// controller's button animation
	Unigine::Vector<Unigine::NodePtr> buttons;
	void buttons_init();
	void buttons_update();

	// gamepad (teleportation and grab)
	Unigine::NodeDummyPtr xpad_head, xpad_hand;
	void gamepad_init();
	void gamepad_update(const Unigine::Math::Mat4 &hmd_transform);

	// temporary
	float odiscrete_timer = 0;

	void move_update_touch(const Unigine::Math::vec2 &axis, const Unigine::Math::Mat4 &controller_transform);
	void turn_update_touch(const Unigine::Math::vec2 &axis, const Unigine::Math::Mat4 &head_transform);
};