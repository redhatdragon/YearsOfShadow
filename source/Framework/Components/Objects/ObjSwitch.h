#pragma once
#include "../VRInteractable.h"
#include <UnigineSounds.h>

class ObjSwitch : public VRInteractable
{
public:
	COMPONENT(ObjSwitch, VRInteractable);
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);

	// property
	PROP_NAME("switch");
	PROP_AUTOSAVE(0);

	// parameters
	PROP_PARAM(Toggle, play_pingpong, 0);	// play_pingpong - play "ping-pong" animation (off_pos -> on_pos -> off_pos)
	PROP_PARAM(Toggle, use_quaternions, 0);	// use_quaterions - use quaternions instead of euler angles in on/off_rotation updates
	PROP_PARAM(Float, animation_duration, 0.5f);	// animation_duration - duration of animation between "off" to "on" switch in secs
	PROP_PARAM(Toggle, change_position, 1);
	PROP_PARAM(Vec3, disable_position);
	PROP_PARAM(Vec3, enable_position);
	PROP_PARAM(Toggle, change_rotation, 1);
	PROP_PARAM(Vec3, disable_rotation_euler, Unigine::Math::vec3(0, 0, 270));
	PROP_PARAM(Vec3, enable_rotation_euler, Unigine::Math::vec3(0, 0, 360));
	PROP_PARAM(File, switch_sound_file);

	// interact methods
	void grabIt(VRPlayer* player, int hand_num) override;

protected:
	void init();
	void update();

private:
	int enabled;
	Unigine::Math::quat disable_rotation, enable_rotation;

	float time;
	Unigine::Math::Vec3 from_pos, to_pos;
	Unigine::Math::quat from_rot, to_rot;
	Unigine::Math::vec3 from_rot_euler, to_rot_euler;
	
	Unigine::SoundSourcePtr switch_sound;
};