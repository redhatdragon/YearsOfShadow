#include "ObjHandle.h"
#include <UnigineGame.h>
#include "../../Utils.h"
#include "../../SoundManager.h"

#define SPEED_FACTOR 15.0f
#define ACCELERATION_FACTOR 30.0f
#define MIN_SLIDE_SOUND_TIME 0.2f

REGISTER_COMPONENT(ObjHandle);

using namespace Unigine;
using namespace Math;

void ObjHandle::init()
{
	obj = checked_ptr_cast<Object>(node);

	body = node->getObjectBodyRigid();
	if (body)
		body->setFreezable(0);

	sound = SoundSource::create(sound_loop);
	sound->setPosition(node->getWorldPosition());
	sound->setMinDistance(sound_min_dist);
	sound->setMaxDistance(sound_max_dist);
	sound->setGain(1.0f);
	sound->setLoop(1);
	sound->setOcclusion(0);
	sound->stop();
	slide_sound_timer = 0;
	
	speed = vec3_zero;
	is_grabbed = 0;
}

void ObjHandle::update()
{
	if (!is_grabbed && change_pos && length2(speed) > Consts::EPS)
	{
		Vec3 pos = node->getPosition();
		Vec3 clamped_pos = clamp(pos, Vec3(handle_min_pos), Vec3(handle_max_pos));
		if (length2(clamped_pos - pos) > Consts::EPS)
		{
			// stop it! we touch some border!
			speed = vec3_zero;
			node->setPosition(clamped_pos);
		}
		else
		{
			// smooth slow down the handle if user dont touch it
			speed = lerp(speed, vec3_zero, saturate(ACCELERATION_FACTOR * Game::getIFps()));
		}
		if (body)
			body->setLinearVelocity(speed);
	}

	// change pitch of sound if user uses "Slow Motion" effect
	sound->setPitch(Game::getScale());
}

void ObjHandle::grabIt(VRPlayer* player, int hand_num)
{
	SoundManager::get()->playSound(
		sound_start,
		node->getWorldPosition(),
		1.0f,
		Game::getRandomFloat(0.8f, 1.2f),
		sound_min_dist,
		sound_max_dist);

	last_cangles = decomposeRotationXYZ(node->getRotation().getMat3());
	last_handle_pos = node->getWorldPosition();
	if (player->getHandDegreesOfFreedom(hand_num) < 6) // PC
	{
		PlayerPtr p = Game::getPlayer();
		Vec3 p1 = p->getCamera()->getPosition();
		Vec3 p2 = p1 + Vec3(p->getViewDirection()) * 100.0f;
		ObjectPtr hit_obj = World::getIntersection(p1, p2, 1, intersection);
		if (hit_obj){
			grab_hand_pos = intersection->getPoint();
		}
		length_hand_pc = length(grab_hand_pos - player->getHead()->getWorldPosition());
	} else { //VR
		grab_hand_pos = player->getHandNode(hand_num)->getWorldPosition();
	}
	
	grab_obj_pos = node->getWorldPosition();
	grab_rotate = node->getWorldRotation();

	// set ignoring of the handle from player's raycast (intersections)
	for (int i = 0; i < obj->getNumSurfaces(); i++)
		obj->setIntersection(0, i);

	is_grabbed = 1;
}

void ObjHandle::holdIt(VRPlayer* player, int hand_num)
{
	NodePtr hand = player->getHandNode(hand_num);
	Vec3 handle_pos = last_handle_pos;

	// change and clamp position
	if (change_pos) 
	{
		Vec3 hand_offset = hand->getWorldPosition() - grab_hand_pos;
		Vec3 prev_pos = node->getWorldPosition();
		Vec3 target_pos = grab_obj_pos + hand_offset;										// world coordinates
		NodePtr parent = node->getParent();
		if (parent)
		{
			target_pos = parent->getIWorldTransform() * target_pos;							// local coordinates
			target_pos = clamp(target_pos, Vec3(handle_min_pos), Vec3(handle_max_pos));		// clamp local coords
			target_pos = parent->getWorldTransform() * target_pos;							// return to world
		}
		else
			target_pos = clamp(target_pos, Vec3(handle_min_pos), Vec3(handle_max_pos));	

		handle_pos = target_pos;

		// calculate velocity
		vec3 target_speed = vec3(target_pos - prev_pos) * SPEED_FACTOR;
		speed = lerp(speed, target_speed, saturate(ACCELERATION_FACTOR * Game::getIFps() / (length(target_speed) + 0.01f)));
		if (body)
			body->setLinearVelocity(speed);
		else
			node->setPosition(target_pos);

		// slide sound
		if (length(handle_pos - last_handle_pos) / Game::getIFps() >= 0.3f)
			slide_sound_timer = MIN_SLIDE_SOUND_TIME;

		last_handle_pos = handle_pos;
	}

	// change rotation
	if (change_rot)
	{
		Vec3 new_rotate_vec;
		if (player->getHandDegreesOfFreedom(hand_num) < 6) // PC
		{
			PlayerPtr p = Game::getPlayer();
			Vec3 p1 = p->getCamera()->getPosition();
			Vec3 p2 = p1 + Vec3(p->getViewDirection()) * length_hand_pc;
			new_rotate_vec = normalize(p2 - node->getWorldPosition());
		}
		else { // VR
			new_rotate_vec = normalize(hand->getWorldPosition() - node->getWorldPosition());
		}

		Vec3 old_rotate_vec = normalize(grab_hand_pos - node->getWorldPosition());
		Vec3 up = cross(new_rotate_vec, old_rotate_vec);
		float angle = getAngle(vec3(old_rotate_vec), vec3(new_rotate_vec), vec3(up));
		node->setWorldRotation(quat(vec3(up), angle) * grab_rotate);

		//Visualizer::renderVector(node->getWorldPosition(), node->getWorldPosition() + old_rotate_vec, vec4_red);
		//Visualizer::renderVector(node->getWorldPosition(), node->getWorldPosition() + new_rotate_vec, vec4_green);
		//Visualizer::renderVector(node->getWorldPosition(), node->getWorldPosition() + up, vec4_blue);

		// clamp rotation
		vec3 angles = decomposeRotationXYZ(node->getRotation().getMat3());
		vec3 cangles = clamp(angles, handle_min_rot, handle_max_rot); // cangles = clamped angles
		if (angles.y != cangles.y) // TO DO: move axis selection to property
		{
			// if player reaches the border play impact sound
			if (!border_reached)
				SoundManager::get()->playSound(
				sound_stop,
				node->getWorldPosition(),
				Game::getScale(), // volume
				Game::getRandomFloat(0.9f, 1.1f),
				sound_min_dist,
				sound_max_dist);

			border_reached = 1;
		}
		else
			border_reached = 0;

		// apply rotation
		node->setRotation(quat(cangles.x, cangles.y, cangles.z));

		// slide sound
		if (Math::abs(getAngle(cangles.y, last_cangles.y)) >= 1.0f)
			slide_sound_timer = MIN_SLIDE_SOUND_TIME;

		last_cangles = cangles;
	}

	// play slide sound
	if (slide_sound_timer > 0)
	{
		sound->play();
		player->vibrateController(hand_num, 1.0f);
		slide_sound_timer = saturate(slide_sound_timer - Game::getIFps());
	}
	else
	{
		sound->stop();
		player->vibrateController(hand_num, 0);
	}
}

void ObjHandle::throwIt(VRPlayer* player, int hand_num)
{
	sound->stop();

	// revert changes
	for (int i = 0; i < obj->getNumSurfaces(); i++)
		obj->setIntersection(1, i);

	is_grabbed = 0;
}