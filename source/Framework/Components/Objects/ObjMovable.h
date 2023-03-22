#pragma once
#include <UnigineNode.h>
#include <UniginePhysics.h>
#include <UnigineVector.h>
#include <UnigineMap.h>
#include "../VRInteractable.h"
#include "../../Utils.h"

class ObjMovable: public VRInteractable
{
public:
	COMPONENT(ObjMovable, VRInteractable);
	COMPONENT_INIT(init);
	COMPONENT_SHUTDOWN(shutdown);

	// property
	PROP_NAME("movable");
	PROP_AUTOSAVE(0);

	// parameters
	PROP_PARAM(Toggle, use_physics, 1);
	PROP_PARAM(Switch, mass_list, 0);
	
	PROP_PARAM(Toggle, use_handy_pos, 0);
	PROP_PARAM(Vec3, handy_pos);
	PROP_PARAM(Vec3, handy_rot_euler);
	PROP_PARAM(Vec3, handy_pos_vr);
	PROP_PARAM(Vec3, handy_rot_euler_vr);
	
	PROP_PARAM(Toggle, can_attach_to_head, 0);
	PROP_PARAM(Vec3, hat_def_pos);
	PROP_PARAM(Vec3, hat_def_rot_euler);
	
	PROP_PARAM(Toggle, use_sounds);
	PROP_PARAM(File, impact_sound_file);
	PROP_PARAM(Float, impact_min_dist, 0.2f);
	PROP_PARAM(Float, impact_max_dist, 1000.0f);
	PROP_PARAM(File, slide_sound_file);
	PROP_PARAM(Float, slide_min_dist, 0.2f);
	PROP_PARAM(Float, slide_max_dist, 1000.0f);

	// interact methods
	void grabIt(VRPlayer* player, int hand_num) override;
	void holdIt(VRPlayer* player, int hand_num) override;
	void throwIt(VRPlayer* player, int hand_num) override;

	UNIGINE_INLINE Unigine::BodyRigid* getBody() { return body.get(); }

	// events
	Action<void(Unigine::BodyPtr)> onContacts;
	Action<void(Unigine::BodyPtr)> onFrozen;
	
protected:
	void init();
	void shutdown();

private:
	Unigine::BodyRigidPtr body;
	Unigine::Math::Mat4 transform;			// local transform (when holding) of the node
	Unigine::NodePtr null_node;
	void *position_callback_id {nullptr};
	void *contact_callback_id {nullptr};
	void *frozen_callback_id {nullptr};

	int hold = 0;			// this node is holding now?

	// saveState / restoreState variables
	Unigine::Vector<int> s_immovable;
	Unigine::Vector<int> s_frozen;

	float handy_pos_factor = 7.5f; // speed of changing pos/rot to handy position for hand

	float max_attach_distance = 0.3f;
	Unigine::NodePtr attached_to = null_node;
	Unigine::Math::quat handy_rot, handy_rot_vr;
	Unigine::Math::quat hat_def_rot;

	int mass_index = 0;		   // 0 - auto, 1 - lightest, 2 - light...
	Unigine::Vector<float> shapes_mass; // mass of each shape in body

	// buffer of last two positions for true velocity detection
	Unigine::Math::Vec3 last_pos[2];
	int last_pos_index;

	void unfreeze(const Unigine::BodyPtr &body);
	void play_sound(const Unigine::BodyPtr &body, const Unigine::Math::Vec3 &pos, float volume);
	void play_loop_sound(const Unigine::BodyPtr &body, float volume, float pitch);

	void activate_body(const Unigine::BodyPtr &body);

	// callbacks and actions
	UNIGINE_INLINE void position_callback(Unigine::BodyPtr body) { last_pos_index = 1 - last_pos_index; last_pos[last_pos_index] = body->getPosition(); }
	UNIGINE_INLINE void contact_callback(Unigine::BodyPtr body) { onContacts(body); }
	UNIGINE_INLINE void frozen_callback(Unigine::BodyPtr body) { onFrozen(body); }

	int unfreeze_action_id, play_sound_action_id, mute_action_id;
	void unfreeze_action(const Unigine::BodyPtr &body);
	void play_sound_action(const Unigine::BodyPtr &body);
	void mute_action(const Unigine::BodyPtr &body);
};
