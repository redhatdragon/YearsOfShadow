#pragma once
#include <UnigineWorld.h>
#include <UnigineSounds.h>
#include "../VRInteractable.h"

class ObjHandle : public VRInteractable
{
public:
	COMPONENT(ObjHandle, VRInteractable);
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);

	// property
	PROP_NAME("handle");
	PROP_AUTOSAVE(0);

	// parameters
	PROP_PARAM(File, sound_start);
	PROP_PARAM(File, sound_loop);
	PROP_PARAM(File, sound_stop);
	PROP_PARAM(Float, sound_min_dist, 0.2f);
	PROP_PARAM(Float, sound_max_dist, 1000.0f);

	PROP_PARAM(Toggle, change_pos, 1);
	PROP_PARAM(Toggle, change_rot, 1);
	PROP_PARAM(Vec3, handle_min_pos, Unigine::Math::vec3(0, 0, 0));
	PROP_PARAM(Vec3, handle_max_pos, Unigine::Math::vec3(0, 0, 0));
	PROP_PARAM(Vec3, handle_min_rot, Unigine::Math::vec3(-180, -180, -180));
	PROP_PARAM(Vec3, handle_max_rot, Unigine::Math::vec3(180, 180, 180));

	// interact methods
	void grabIt(VRPlayer* player, int hand_num) override;
	void holdIt(VRPlayer* player, int hand_num) override;
	void throwIt(VRPlayer* player, int hand_num) override;

protected:
	void init();
	void update();

private:
	Unigine::ObjectPtr obj;
	Unigine::BodyRigidPtr body;
	
	Unigine::SoundSourcePtr sound;

	float slide_sound_timer = 0;
	int border_reached = 0;
	Unigine::Math::vec3 last_cangles;
	Unigine::Math::Vec3 last_handle_pos;
	Unigine::Math::Vec3 grab_hand_pos; // position of the hand when it grab that object
	Unigine::Math::Vec3 grab_obj_pos; // position of the object when it was grabbed
	Unigine::Math::quat grab_rotate; //rotation of the node when it was grabbed
	Unigine::Math::Scalar length_hand_pc; 

	int is_grabbed;
	Unigine::Math::vec3 speed; // current moving speed of the handle

	Unigine::WorldIntersectionPtr intersection = Unigine::WorldIntersection::create();
};