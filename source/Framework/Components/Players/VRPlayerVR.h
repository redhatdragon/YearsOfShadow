#pragma once
#include <UnigineEngine.h>
#include <UnigineEditor.h>
#include <UniginePlayers.h>
#include <UnigineLogic.h>
#include <UnigineGame.h>
#include "VRPlayer.h"
#include "../../Triggers.h"

#define CONTROLLER_COUNT 2

class VRPlayerVR : public VRPlayer
{
public:
	COMPONENT(VRPlayerVR, VRPlayer);

	// parameters
	PROP_PARAM(Node, controller_0);					// reference to left controller
	PROP_PARAM(Node, controller_1);					// reference to right controller
	PROP_PARAM(Node, teleport_allowed_marker);		// reference to teleport marker
	PROP_PARAM(Node, teleport_forbidden_marker);	// reference to teleport marker
	PROP_PARAM(Material, teleport_allowed_mat);		// reference to it material
	PROP_PARAM(Material, teleport_forbidden_mat);	// reference to it material
	PROP_PARAM(Node, obstacles);					// container with triggers (look at Trigger.h file)
	PROP_PARAM(Float, hand_force_multiplier, 1.3f); // getHandLinearVelocity = velocity * hand_force_multiplier
	PROP_PARAM(Int, controller_buffer_count, 4);	// used in Linear Regression. Big value removes noise, but looks unnatural

	// teleportation
	void setRayIntersectionMask(int mask) override;
	void setTeleportationMask(int mask) override;

	// player
	Unigine::PlayerPtr getPlayer() override;
	
	// hands
	int getNumHands() override;										// get count of hands
	Unigine::NodePtr getHandNode(int num) override;					// get hand's node
	int getHandDegreesOfFreedom(int num) override;					// get hand's degrees of freedom
	int getHandState(int num) override;								// get hand state (grab, hold, throw)
	const Unigine::NodePtr &getGrabNode(int num) const override;
	const Unigine::Vector<VRInteractable*> &getGrabComponents(int num) const override;
	Unigine::Math::vec3 getHandLinearVelocity(int num) override;	// get speed of hand
	Unigine::Math::vec3 getHandAngularVelocity(int num) override;	// get angular speed of hand

	// hand's controllers
	int isControllerValid(int controller_num);
	virtual Unigine::Math::Mat4 getControllerTransform(int controller_num) = 0;
	int getControllerButtonDown(int controller_num, int button) override;
	int getControllerButtonUp(int controller_num, int button) override;

	// gui
	Unigine::GuiPtr getGui() override
	{
		if (object_gui)
			return object_gui->getGui();

		return nullptr;
	}
	Unigine::ObjectGuiPtr getObjectGui() { return object_gui; };

	// eyetracking
	bool isEyetrackingValid() const override;
	Unigine::Math::Vec3 getFocusWorldPosition() const override;

	virtual const Unigine::Math::Vec3& getHeadOffset() const { return Vec3_zero; }

protected:
	// methods
	void update_button_states() override;

	void init_player();
	void load_player_height(float default_value);
	void save_player_height();
	void put_head_to_position(const Unigine::Math::mat4 &hmd_transform, const Unigine::Math::Vec3 &pos);
	void land_player_to(const Unigine::Math::mat4 &hmd_transform, const Unigine::Math::Vec3 &position, const Unigine::Math::vec3 &direction); // teleport player to position, head to direction, foot to ground
	void move_update(const Unigine::Math::Mat4 &world_head_transform);
	void collisions_update(const Unigine::Math::Mat4 &head_transform, const Unigine::Math::Vec3 &offset);
	
	void controllers_init();
	void find_obj_in_children(const Unigine::NodePtr &node, Unigine::Vector<Unigine::ObjectPtr> *obj);
	void controller_update(
		int num,
		const Unigine::Math::Mat4 &player_transform,
		int is_device_connected,
		const Unigine::Math::mat4 &device_pose);
	
	void teleport_init();
	void teleport_update(int num, int button_pressed, const Unigine::Math::Vec3 &offset);

	void grab_init();
	void grab_update(int num, int pose_valid, float trigger_value, int button_touch, float pressed_pos = 0.3f, float release_pos = 0.8f);
	void set_outline(int num, int enable);
	void pointer_init();
	void pointer_update(int num, int find_mode);

	void push_hand_linear_velocity(int num, const Unigine::Math::vec3 &velocity);
	void push_hand_angular_velocity(int num, const Unigine::Math::vec3 &velocity);

	void update_gui();

	void update_eyetracking(Unigine::Math::Vec3 focus_world_position, bool valid);

	// common
	Unigine::PlayerDummyPtr player;
	Unigine::NodeReferencePtr controller[CONTROLLER_COUNT];
	Unigine::NodePtr controller_ref[CONTROLLER_COUNT];
	Unigine::Vector<Unigine::ObjectPtr> controller_obj[CONTROLLER_COUNT];
	int controller_valid[CONTROLLER_COUNT];

	Unigine::WorldIntersectionNormalPtr intersection = Unigine::WorldIntersectionNormal::create();
	Unigine::Vector<Unigine::ObjectPtr> intersections;

	// controllers buttons
	int buttons_prev_state[CONTROLLER_COUNT][BUTTON::COUNT];

	// height calibration
	float player_height = 1.7f;

	// gui
	Unigine::ObjectGuiPtr object_gui;

	// moving
	Unigine::ControlsGamepadPtr xpad360;
	float dead_zone = 0.2f;		// "dead zone" of gamepad
	float step = 0.2f;			// discrete rotation
	float discrete_timer = 0;
	bool is_moving = false;
	Unigine::Math::quat head_dir = Unigine::Math::quat_identity;

	// teleportation
	int ray_intersection_mask = 1;
	int teleportation_mask = 1;
	Unigine::ObjectMeshDynamicPtr teleport_ray;
	int teleport_button_pressed[CONTROLLER_COUNT];

	// grabbing
	Unigine::Vector<int> hand_state;		// status of hand: free, grab, hold, throw, etc.
	Unigine::Vector<int> node_grabbed;	// hand grab something?
	Unigine::Vector<int> node_selected;	// hand around grabable node?
	Unigine::Vector<Unigine::ObjectPtr> last_selected;
	Unigine::Vector<float> last_selected_timer;
	Unigine::Vector<Unigine::NodePtr> grab_node;	// what is it?
	Unigine::Vector<Unigine::Vector<VRInteractable*>> grab_components;
	Unigine::Vector<float> throw_trigger_value;	// for best experience (grab when 20%, throw when 80%)
	float ray_back_length = 0.03f;		// ray's length of controllers from pivot
	float ray_forward_length = 0.05f;

	enum GRAB_HANDY
	{
		NON_HANDY,
		GRAB,
		GRAB_RELEASED,
		GRAB_AGAIN,
	};
	Unigine::Vector<int> handy_pos_state;

	// pointer (part of grabbing)
	Unigine::Vector<Unigine::ObjectMeshDynamicPtr> ptr_mesh;
	Unigine::Vector<float> ptr_anim;
	float ptr_width = 0.0125f;

	// controller velocities
	Unigine::Vector<Unigine::Vector<Unigine::Math::vec3>> hand_linear_velocity;
	Unigine::Vector<Unigine::Vector<Unigine::Math::vec3>> hand_angular_velocity;

	// collision detection
	Triggers triggers;
	float triggers_scale = 0.03f;
	float trigger_timer = 0;
	float black_screen_max_sec = 3.0f;	// how long screen may be black (player goes out of room)
	float fade_alpha = 0;
	Unigine::Math::Vec3 before_collision_point = Unigine::Math::Vec3_zero;
	Unigine::Math::Vec3 before_collision_dir = Unigine::Math::Vec3_zero;

	// eye tracking
	Unigine::Math::Vec3 focus_position;
	bool eyetracking_valid = false;
};
