#pragma once

#include <UnigineGame.h>
#include <UnigineComponentSystem.h>

#include <plugins/UnigineVarjo.h>
#include <plugins/UnigineUltraleap.h>
#include "VRPlayerVR.h"

class MeshSkinnedHandMapper;

class UltraleapHandControllerVarjo : public VRPlayerVR
{
public:
	COMPONENT_DEFINE(UltraleapHandControllerVarjo, VRPlayerVR);

	COMPONENT_INIT(post_init, 100);
	COMPONENT_UPDATE(update);
	COMPONENT_SHUTDOWN(shutdown);

	PROP_PARAM(Node, player_spawner_component);
	PROP_PARAM(Toggle, visualize_hands);
	PROP_PARAM(Node, gui_spawn_point);
	PROP_PARAM(Node, noderef_hands_skinned_mesh);
	PROP_PARAM(Toggle, show_help, true);
	PROP_PARAM(File, help_img, "vr_sample/props/hands/helper.png");
	virtual int getControllerButton(int controller_num, int button) override;
	virtual float getControllerAxis(int controller_num, int button) override;
	virtual void vibrateController(int controller_num, float amplitude) override {}
	virtual Mat4 getControllerTransform(int controller_num) override;

protected:
	void post_init();
	void update();
	void shutdown();
	void hands_init();

	void draw_debug(Unigine::Plugins::UltraleapHand* hand) const;

	void update_hand(Unigine::Plugins::UltraleapHand* hand);
	ObjectMeshSkinnedPtr hands_skinned;
	MeshSkinnedHandMapper* mapper = nullptr;

	Unigine::Plugins::Ultraleap * ultraleap = nullptr;
	Unigine::Plugins::UltraleapDevice * ul_device = nullptr;
	VRPlayerVR * vr_player_main = nullptr;

	float invisible_time[2] = { 0 };
	float max_invisible_time = 1.5f;
	long long hand_visible_time[2]{ 0 };

	void teleport_update();
	bool teleport_active = false;
	float teleport_delay_active_current = 0.0f;
	float teleport_delay_max = 1.0f;
	float teleport_delay = 0.0f;

	void menu_update();
	float menu_delay_current {0.0f};
	float menu_delay_max = 0.8f;
	float button_hand_radius = 0.08f;
	Unigine::ObjectGuiPtr helper;
	void gui_update(Unigine::Plugins::UltraleapHand* hand);
	void close_helper();
};


class MeshSkinnedHandMapper : public Unigine::ComponentBase
{
public:
	COMPONENT_DEFINE(MeshSkinnedHandMapper, Unigine::ComponentBase);
	COMPONENT_INIT(init);

	struct finger : ComponentStruct
	{
		PROP_PARAM(Int, last, -1);
		PROP_PARAM(Int, middle, -1);
		PROP_PARAM(Int, first, -1);
		PROP_PARAM(Int, base, -1);
	};

	struct hand : ComponentStruct
	{
		PROP_STRUCT(finger, thumb);
		PROP_STRUCT(finger, index);
		PROP_STRUCT(finger, middle);
		PROP_STRUCT(finger, ring);
		PROP_STRUCT(finger, pinky);

		PROP_PARAM(Int, wrist, -1);
		PROP_PARAM(Int, arm, -1);
		PROP_PARAM(Int, elbow, -1);

		PROP_PARAM(Toggle, invert_direction, false);
	};

	PROP_STRUCT(hand, left_hand);
	PROP_STRUCT(hand, right_hand);

	void update(Unigine::Plugins::UltraleapHand* left, Unigine::Plugins::UltraleapHand* right);

private:
	Unigine::ObjectMeshSkinnedPtr skinned;
	void init() { skinned = checked_ptr_cast<ObjectMeshSkinned>(node); }


};