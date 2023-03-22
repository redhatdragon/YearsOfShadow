#include "VRPlayerSpawner.h"

#include "Players/VRPlayerPC.h"
#include "Players/VRPlayerOpenVR.h"
#include "Players/VRPlayerOculus.h"

REGISTER_COMPONENT(VRPlayerSpawner);

using namespace Unigine;
using namespace Math;

void VRPlayerSpawner::init()
{
	Engine *engine = Engine::get();

	bool has_openvr = engine->findPlugin("OpenVR") != -1;
	bool has_varjo = engine->findPlugin("Varjo") != -1;
	bool has_oculus = engine->findPlugin("Oculus") != -1;

	// check OpenVR headset
	if (has_openvr || has_varjo)
	{
		// show openvr controls
		openvr_basestation_0->setEnabled(1);
		openvr_basestation_1->setEnabled(1);
		openvr_controller_0->setEnabled(1);
		openvr_controller_1->setEnabled(1);
		teleport_allowed_marker->setEnabled(1);
		teleport_forbidden_marker->setEnabled(1);
		openvr_obstacles->setEnabled(0);

		// add node
		PlayerDummyPtr player = PlayerDummy::create();
		init_player(player);

		// add component
		VRPlayerOpenVR *vr_player_openvr = ComponentSystem::get()->addComponent<VRPlayerOpenVR>(player);
		vr_player_openvr->basestation_0 = openvr_basestation_0;
		vr_player_openvr->basestation_1 = openvr_basestation_1;
		vr_player_openvr->controller_0 = openvr_controller_0;
		vr_player_openvr->controller_1 = openvr_controller_1;
		vr_player_openvr->teleport_allowed_marker = teleport_allowed_marker;
		vr_player_openvr->teleport_forbidden_marker = teleport_forbidden_marker;
		vr_player_openvr->teleport_allowed_mat = teleport_allowed_mat;
		vr_player_openvr->teleport_forbidden_mat = teleport_forbidden_mat;
		vr_player_openvr->obstacles = openvr_obstacles;
		vr_player_openvr->hand_force_multiplier = openvr_hand_force_multiplier;
		vr_player_openvr->controller_buffer_count = openvr_controller_buffer_count;
		vr_player_openvr->setRayIntersectionMask(ray_intersection_mask.get());
		vr_player_openvr->setTeleportationMask(teleportation_mask.get());
		vr_player = vr_player_openvr;
	} else
	{
		// hide openvr controls
		openvr_basestation_0->setEnabled(0);
		openvr_basestation_1->setEnabled(0);
		openvr_controller_0->setEnabled(0);
		openvr_controller_1->setEnabled(0);
		teleport_allowed_marker->setEnabled(0);
		teleport_forbidden_marker->setEnabled(0);
		openvr_obstacles->setEnabled(0);
	}

	// check Oculus VR headset
	if (has_oculus)
	{
		// show oculus controls
		oculus_warning_message->setEnabled(1);
		oculus_success_message->setEnabled(1);
		oculus_controller_0->setEnabled(1);
		oculus_controller_1->setEnabled(1);
		teleport_allowed_marker->setEnabled(1);
		teleport_forbidden_marker->setEnabled(1);
		oculus_obstacles->setEnabled(0);

		// add node
		PlayerDummyPtr player = PlayerDummy::create();
		init_player(player);

		// add component
		VRPlayerOculus* vr_player_oculus = ComponentSystem::get()->addComponent<VRPlayerOculus>(player);
		vr_player_oculus->warning_message = oculus_warning_message;
		vr_player_oculus->success_message = oculus_success_message;
		vr_player_oculus->controller_0 = oculus_controller_0;
		vr_player_oculus->controller_1 = oculus_controller_1;
		vr_player_oculus->teleport_allowed_marker = teleport_allowed_marker;
		vr_player_oculus->teleport_forbidden_marker = teleport_forbidden_marker;
		vr_player_oculus->teleport_allowed_mat = teleport_allowed_mat;
		vr_player_oculus->teleport_forbidden_mat = teleport_forbidden_mat;
		vr_player_oculus->obstacles = oculus_obstacles;
		vr_player_oculus->hand_force_multiplier = oculus_hand_force_multiplier;
		vr_player_oculus->controller_buffer_count = oculus_controller_buffer_count;
		vr_player_oculus->setRayIntersectionMask(ray_intersection_mask.get());
		vr_player_oculus->setTeleportationMask(teleportation_mask.get());
		vr_player = vr_player_oculus;
	} else
	{
		// hide oculus controls
		oculus_warning_message->setEnabled(0);
		oculus_success_message->setEnabled(0);
		oculus_controller_0->setEnabled(0);
		oculus_controller_1->setEnabled(0);
		teleport_allowed_marker->setEnabled(0);
		teleport_forbidden_marker->setEnabled(0);
		oculus_obstacles->setEnabled(0);
	}

	// if VR is not loaded
	if (has_openvr == false && has_varjo == false && has_oculus == false)
	{
		// add node
		PlayerActorPtr player = PlayerActor::create();
		init_player(player);

		// add component
		VRPlayerPC *vr_player_pc = ComponentSystem::get()->addComponent<VRPlayerPC>(player);
		vr_player_pc->move_speed = move_speed;
		vr_player_pc->acceleration = acceleration;
		vr_player_pc->damping = damping;
		vr_player_pc->hand_length = hand_length;
		vr_player_pc->throw_force = throw_force;
		vr_player = vr_player_pc;
	}
}

void VRPlayerSpawner::init_player(const PlayerPtr &player)
{
	// copy transform
	player->setWorldTransform(node->getWorldTransform());

	if (node->getType() >= Node::PLAYER_BEGIN &&
		node->getType() <= Node::PLAYER_END)
	{
		PlayerPtr player_ref = checked_ptr_cast<Player>(node);
		
		// copy basic parameters
		player->setFov(player_ref->getFov());
		player->setZNear(player_ref->getZNear());
		player->setZFar(player_ref->getZFar());
		for (int i = 0 ; i < player_ref->getNumScriptableMaterials(); i++)
			player->addScriptableMaterial(player_ref->getScriptableMaterial(i));
	}

	// unparent current node 
	// (cause vr_group can be our parent and we don't want to turn off
	// components that are children to us)
	node->setWorldParent(NodePtr());
}
