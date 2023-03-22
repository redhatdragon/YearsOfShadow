#include "ObjMovable.h"
#include <UnigineGame.h>
#include <UnigineRender.h>
#include <UnigineWorlds.h>
#include "../../SoundManager.h"

#define SOUND_THRESHOLD 0.1f
#define MAX_ATTACH_HAND_VELOCITY 2.0f
#define IMPACT_SOUND_GAIN 1.0f
#define SLIDE_SOUND_GAIN 0.1f

REGISTER_COMPONENT(ObjMovable);

using namespace Unigine;
using namespace Math;

void ObjMovable::init()
{
	// generate physic shapes
	if (use_physics.get())
	{
		// create shapes
		body = node->getObjectBodyRigid();
		if (!body)
		{
			ObjectPtr obj = checked_ptr_cast<Object>(node);
			if (obj)
			{
				body = BodyRigid::create(checked_ptr_cast<Object>(node));
				body->createShapes(0, 0.2f, 0.2f);
			}
			else
			{
				Log::error("ObjMovable::init(): can't create physic shapes for \"%s\" node\n", node->getName());
				return;
			}
		}
		
		// calculate mass and damping
		float mass = 1;
		float damping = 0;
		if (mass_list.get() <= 0)
		{
			// auto generate
			BoundBox bb = node->getBoundBox();
			vec3 bb_size = bb.maximum - bb.minimum;
			mass = bb_size.x * bb_size.y * bb_size.z * 10.0f;
			damping = lerp_clamped(0.0f, 1.0f, 0.5f - mass * 10.0f);
		}
		else
		{
			// manual
			mass = powf(itof(mass_list.get()), 3) * 0.1f;
			damping = lerp_clamped(1.0f, 0.0f, (mass - 1.0f) / 4.0f);
		}

		// apply mass and damping
		float mass_piece = mass / itof(body->getNumShapes());
		for (int k = 0; k < body->getNumShapes(); k++)
			body->getShape(k)->setMass(mass_piece);

		body->setLinearDamping(damping);
		body->setAngularDamping(damping);

		body->setFrozen(1);

		shapes_mass.clear();
		for (int i = 0; i < body->getNumShapes(); i++)
			shapes_mass.append(body->getShape(i)->getMass());
	}

	// prepare quats
	handy_rot = quat(handy_rot_euler.get().x, handy_rot_euler.get().y, handy_rot_euler.get().z);
	handy_rot_vr = quat(handy_rot_euler_vr.get().x, handy_rot_euler_vr.get().y, handy_rot_euler_vr.get().z);
	hat_def_rot = quat(hat_def_rot_euler.get().x, hat_def_rot_euler.get().y, hat_def_rot_euler.get().z);
	
	// callbacks
	if (body)
	{
		position_callback_id = body->addPositionCallback(MakeCallback(this, &ObjMovable::position_callback));
		contact_callback_id = body->addContactsCallback(MakeCallback(this, &ObjMovable::contact_callback));
		frozen_callback_id = body->addFrozenCallback(MakeCallback(this, &ObjMovable::frozen_callback));
		play_sound_action_id = onContacts.reserveId();
		unfreeze_action_id = onContacts.reserveId();
		mute_action_id = onFrozen.reserveId();
	}

	last_pos_index = 0;
	last_pos[0] = last_pos[1] = node->getWorldPosition();
}

void ObjMovable::shutdown()
{
	if (body)
	{
		body->removePositionCallback(position_callback_id);
		body->removeContactsCallback(contact_callback_id);
		body->removeFrozenCallback(frozen_callback_id);
	}
}

void ObjMovable::grabIt(VRPlayer* player, int hand_num)
{
	NodePtr hand = player->getHandNode(hand_num);

	// get local transform of the node to the hand transform
	transform = hand->getIWorldTransform() * node->getWorldTransform();

	if (use_handy_pos)
		hand->setEnabled(0);

	if (body)
	{
		// unfreeze and unimmovable (for contact callbacks)
		activate_body(body);

		// set mass to zero (objs will no longer jump on top)
		for (int i = 0; i < body->getNumShapes(); i++)
			body->getShape(i)->setMass(0);

		// for moving other objects by this
		if (mass_index != 1 && mass_index != 2) // if not lightest and light
			onContacts.addListener(unfreeze_action_id, std::bind(&ObjMovable::unfreeze_action, this, std::placeholders::_1));
		onContacts.addListener(play_sound_action_id, std::bind(&ObjMovable::play_sound_action, this, std::placeholders::_1));
	}

	hold = 1;
}

void ObjMovable::holdIt(VRPlayer* player, int hand_num)
{
	// update local position and rotation
	if (use_handy_pos)
	{
		float ifps = Game::getIFps() / Game::getScale();
		int dof = player->getHandDegreesOfFreedom(hand_num);
		setTransformPosition(transform, lerp(transform.getTranslate(), Vec3(player->getHandyPos() + (dof < 6 ? handy_pos : handy_pos_vr)), handy_pos_factor * ifps));
		setTransformRotation(transform, slerp(transform.getRotate(), player->getHandyRot() * (dof < 6 ? handy_rot : handy_rot_vr), handy_pos_factor * ifps));
	}

	// update world transform of node relative hand
	node->setWorldTransform(player->getHandNode(hand_num)->getWorldTransform() * transform);

	// update physics
	if (body)
	{
		// (obj may be frozen on the air, if you didn't move your hand a long time)
		activate_body(body);

		// impulse for other bodies
		body->setLinearVelocity(player->getHandLinearVelocity(hand_num));
		body->setAngularVelocity(player->getHandAngularVelocity(hand_num));
	}
}

void ObjMovable::throwIt(VRPlayer* player, int hand_num)
{
	hold = 0;
	vec3 linear_velocity = player->getHandLinearVelocity(hand_num);
	vec3 angular_velocity = player->getHandAngularVelocity(hand_num);

	// restore mass
	if (body)
	{
		for (int i = 0; i < body->getNumShapes(); i++)
			body->getShape(i)->setMass(shapes_mass[i]);
	}

	if (use_handy_pos)
		player->getHandNode(hand_num)->setEnabled(1);

	if (can_attach_to_head)
	{
		NodePtr head = player->getHead();
		float len = (float)length(head->getWorldPosition() - node->getWorldPosition());
		if (len <= max_attach_distance && length(linear_velocity) < MAX_ATTACH_HAND_VELOCITY)
		{
			// attach to head
			body->setImmovable(1);
			for (int i = 0; i < body->getNumShapes(); i++)
				body->getShape(i)->setMass(0);

			setParentKeepPosition(node, head);
			attached_to = head;

			// set default position
			node->setPosition(Vec3(hat_def_pos));
			node->setRotation(hat_def_rot);
		}
		else
		{
			// deattach
			if (attached_to != null_node)
			{
				for (int i = 0; i < body->getNumShapes(); i++)
					body->getShape(i)->setMass(shapes_mass[i]);

				unParentKeepPosition(node);
				attached_to = null_node;
			}
		}
	}

	if (body)
	{
		// set movable and unfrozen
		activate_body(body);

		// add impulse
		body->setLinearVelocity(linear_velocity);
		body->setAngularVelocity(angular_velocity);

		// add callbacks (if mass more than "lightest")
		if (mass_index != 1)
			onContacts.addListener(unfreeze_action_id, std::bind(&ObjMovable::unfreeze_action, this, std::placeholders::_1));
		onContacts.addListener(play_sound_action_id, std::bind(&ObjMovable::play_sound_action, this, std::placeholders::_1));
		onFrozen.addListener(mute_action_id, std::bind(&ObjMovable::mute_action, this, std::placeholders::_1));
	}
}

void ObjMovable::unfreeze(const BodyPtr &body)
{
	if (!body) return;

	if (getComponent<ObjMovable>(body->getObject()))
		activate_body(body);
}

void ObjMovable::play_sound(const BodyPtr &body, const Vec3 &pos, float volume)
{
	if (!body) return;
	if (volume <= 0) return;
	
	ObjMovable* obj = getComponent<ObjMovable>(body->getObject());
	if (!obj || !obj->use_sounds || obj->impact_sound_file.get()[0] == '\0')
		return;
	
	SoundManager::get()->playSound(
		obj->impact_sound_file,
		pos, 
		volume,
		Game::getRandomFloat(0.8f, 1.2f), // pitch
		obj->impact_min_dist,
		obj->impact_max_dist);
}

void ObjMovable::play_loop_sound(const BodyPtr &body, float volume, float pitch)
{
	if (!body) return;

	ObjMovable* obj = getComponent<ObjMovable>(body->getObject());
	if (!obj || !obj->use_sounds || obj->slide_sound_file.get()[0] == '\0')
		return;

	SoundManager::get()->playLoopSound(
		obj->slide_sound_file,
		body->getObject(),
		0.3f, // duration
		volume,
		pitch,
		obj->slide_min_dist,
		obj->slide_max_dist);
}

void ObjMovable::activate_body(const BodyPtr &body)
{
	body->setImmovable(0);
	body->setFrozen(0);
}

void ObjMovable::unfreeze_action(const BodyPtr &body)
{
	for (int num = 0, max = body->getNumContacts(); num < max; ++num)
	{
		unfreeze(body->getContactBody0(num));
		unfreeze(body->getContactBody1(num));
	}
}

void ObjMovable::play_sound_action(const BodyPtr &body)
{
	for (int num = 0, max = body->getNumContacts(); num < max; ++num)
	{
		vec3 velocity = vec3(last_pos[last_pos_index] - last_pos[1 - last_pos_index]) / Physics::getIFps();
		double velocity_len = velocity.length();
		float strength = (float)saturate(velocity_len * IMPACT_SOUND_GAIN);
	
		if (strength > SOUND_THRESHOLD)
		{
			bool impact_collision = Math::abs(dot(velocity, body->getContactNormal(num))) >= 0.2f;
			if (impact_collision)
			{
				// impact
				play_sound(body, body->getContactPoint(num), strength);
				play_sound(body->getContactBody1(num), body->getContactPoint(num), strength);
			}
			else
			{
				// sliding
				float vol = (float)saturate(velocity_len * SLIDE_SOUND_GAIN);
				if (vol > SOUND_THRESHOLD)
				{
					float pitch = clamp(vol * 8.0f, 0.8f, 2.0f);
					play_loop_sound(body, vol, pitch);
					play_loop_sound(body->getContactBody1(num), vol, pitch);
				}
			}
		}
	}
}

void ObjMovable::mute_action(const BodyPtr &body)
{
	// unsubscribe
	onContacts.removeListener(unfreeze_action_id);
	onContacts.removeListener(play_sound_action_id);
	onFrozen.removeListener(mute_action_id);
}
