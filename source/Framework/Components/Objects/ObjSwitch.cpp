#include "ObjSwitch.h"
#include <UnigineGame.h>
#include <UnigineEditor.h>

REGISTER_COMPONENT(ObjSwitch);

using namespace Unigine;
using namespace Math;

void ObjSwitch::init()
{
	// convert euler's vec3 to rotation quat
	vec3 dis_rot = disable_rotation_euler;
	vec3 en_rot = enable_rotation_euler;
	disable_rotation = quat(dis_rot.x, dis_rot.y, dis_rot.z);
	enable_rotation = quat(en_rot.x, en_rot.y, en_rot.z);

	const char* file_name = switch_sound_file.get();
	if (file_name[0] != '\0')
	{
		switch_sound = SoundSource::create(file_name);
		switch_sound->setLoop(0);
		switch_sound->setOcclusion(0);
		switch_sound->setMinDistance(2.0f);
		switch_sound->setMaxDistance(15.0f);
		switch_sound->setParent(node);
		switch_sound->setPosition(Vec3_zero);
	}

	// init value is "disabled" by default
	enabled = 0;
	if (change_position) node->setPosition(Vec3(disable_position));
	if (change_rotation) node->setRotation(disable_rotation);
	time = 1;
}

void ObjSwitch::update()
{
	// animation
	if (time < 1)
	{
		time = saturate(time + (Game::getIFps() / Game::getScale()) / animation_duration);

		if (play_pingpong)
		{
			float percent = 1.0f - Math::abs(time - 0.5f) * 2.0f; // 0 -> 1 -> 0

			if (change_position)
				node->setPosition(Vec3(lerp(time < 0.5f ? from_pos : Vec3(disable_position), Vec3(enable_position), percent)));
			if (change_rotation)
			{
				if (use_quaternions)
					node->setRotation(slerp(time < 0.5f ? from_rot : disable_rotation, enable_rotation, percent));
				else
				{
					vec3 cur_rot = lerp(time < 0.5f ? from_rot_euler : disable_rotation_euler, enable_rotation_euler, percent);
					node->setRotation(quat(cur_rot.x, cur_rot.y, cur_rot.z));
				}
			}
		}
		else
		{
			if (change_position)
				node->setPosition(Vec3(lerp(from_pos, to_pos, time)));
			if (change_rotation)
			{
				if (use_quaternions)
					node->setRotation(slerp(from_rot, to_rot, time));
				else
				{
					vec3 cur_rot = lerp(from_rot_euler, to_rot_euler, time);
					node->setRotation(quat(cur_rot.x, cur_rot.y, cur_rot.z));
				}
			}
		}
	}
}

void ObjSwitch::grabIt(VRPlayer* player, int hand_num)
{
	// rotate switcher
	enabled = 1 - enabled;
	from_pos = node->getPosition();
	from_rot = node->getRotation();
	from_rot_euler = decomposeRotationXYZ(mat3(node->getTransform()));
	to_pos = Vec3(enabled ? enable_position : disable_position);
	to_rot = enabled ? enable_rotation : disable_rotation;
	to_rot_euler = enabled ? enable_rotation_euler : disable_rotation_euler;
	time = 0;

	// play sound
	if (switch_sound)
		switch_sound->play();
}