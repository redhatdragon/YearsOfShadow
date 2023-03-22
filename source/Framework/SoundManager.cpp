#include "SoundManager.h"
#include <UnigineEditor.h>
#include <UnigineEngine.h>
#include <UnigineNodes.h>
#include <UnigineGame.h>
#include <UnigineDir.h>
#include <UnigineFileSystem.h>
#include <UnigineInterpreter.h>

#define SIMILAR_SOUND_LIMIT 10 // playback limit of similar sounds at the same time
#define MIN_DELAY_BETWEEN_SAME_SOUNDS 0.3f // minimum delay between playing of two similar sound (when it plays)
#define SIMILAR_LOOP_SOUND_LIMIT 10 // playback limit of similar loop sounds at the same time

SoundManager::SoundManager()
{
	Engine::get()->addWorldLogic(this);
	init();
}

SoundManager::~SoundManager()
{

}

int SoundManager::init()
{
	queue.clear();
	sounds.clear();
	groups.clear();

	Sound::setDoppler(0); // otherwise you can hear "beep" when moving in slowmo

	return 1;
}

int SoundManager::update()
{
	updateSounds();
	updateLoopSounds();
	updateSoundVolume();

	return 1;
}

void SoundManager::updateSounds()
{
	// add new sounds from queue
	for (int i = 0; i < queue.size(); i++)
	{
		bool need_new_sound = true;
		for (int k = 0; k < sounds.size(); k++)
		{
			if (!sounds[k].source->isPlaying())
			{
				need_new_sound = false;

				// reuse old SoundSource
				Sound::renderWorld(1);
				sounds[k].source->setSampleName(queue[i].fileName.get());
				sounds[k].source->setPosition(queue[i].position);
				sounds[k].source->setGain(queue[i].volume);
				sounds[k].source->setPitch(queue[i].pitch * Game::getScale());
				sounds[k].source->setMinDistance(queue[i].minDistance);
				sounds[k].source->setMaxDistance(queue[i].maxDistance);
				sounds[k].source->setTime(0);
				Sound::renderWorld(1);
				sounds[k].source->play();

				break;
			}
		}

		if (need_new_sound)
		{
			// create new SoundSource
			SoundSourcePtr sound = SoundSource::create(queue[i].fileName.get());
			sound->setPosition(queue[i].position);
			sound->setMinDistance(queue[i].minDistance);
			sound->setMaxDistance(queue[i].maxDistance);
			sound->setGain(queue[i].volume);
			sound->setPitch(queue[i].pitch * Game::getScale());
			sound->setLoop(0);
			sound->setOcclusion(0);
			sound->setSourceMask(SOUND_MASK);
			sound->play();

			SoundData data;
			data.name = queue[i].name;
			data.source = sound;

			sounds.append(data);
		}
	}
	queue.clear();
}

void SoundManager::updateLoopSounds()
{
	// update all loop sounds
	bool need_render_world = false;
	float ifps = Game::getIFps();
	for (int i = 0; i < soundsLoop.size(); i++)
		if (soundsLoop[i].timeRemaining > 0)
		{
			// update pitch
			soundsLoop[i].source->setPitch(lerp(
				soundsLoop[i].source->getPitch(), 
				soundsLoop[i].targetPitch * Game::getScale(), 
				5.0f * Game::getIFps() / Game::getScale()));

			// update gain
			soundsLoop[i].source->setGain(lerp(
				soundsLoop[i].source->getGain(),
				soundsLoop[i].targetGain,
				5.0f * Game::getIFps() / Game::getScale()));

			need_render_world = true;

			// update timer
			soundsLoop[i].timeRemaining -= ifps;
			if (soundsLoop[i].timeRemaining <= 0)
				soundsLoop[i].source->stop();
		}

	if (need_render_world)
		Sound::renderWorld(1);

	// add new loop sounds from queue
	for (int i = 0; i < queueLoop.size(); i++)
	{
		bool need_new_sound = true;
		for (int k = 0; k < soundsLoop.size(); k++)
		{
			if (soundsLoop[k].timeRemaining <= 0)
			{
				need_new_sound = false;

				// reuse old SoundSource
				Sound::renderWorld(1);
				soundsLoop[k].source->setSampleName(queueLoop[i].fileName.get());
				soundsLoop[k].source->setParent(queueLoop[i].parent);
				soundsLoop[k].source->setPosition(Vec3_zero);
				soundsLoop[k].source->setGain(queueLoop[i].volume);
				soundsLoop[k].source->setPitch(queueLoop[i].pitch * Game::getScale());
				soundsLoop[k].source->setMinDistance(queueLoop[i].minDistance);
				soundsLoop[k].source->setMaxDistance(queueLoop[i].maxDistance);
				soundsLoop[k].source->setTime(0);
				Sound::renderWorld(1);
				soundsLoop[k].source->play();

				soundsLoop[k].timeRemaining = queueLoop[i].duration;
				soundsLoop[k].targetPitch = queueLoop[i].pitch;
				soundsLoop[k].targetGain = queueLoop[i].volume;

				break;
			}
		}

		if (need_new_sound)
		{
			// create new SoundSource
			SoundSourcePtr sound = SoundSource::create(queueLoop[i].fileName.get());
			sound->setParent(queueLoop[i].parent);
			sound->setPosition(Vec3_zero);
			sound->setMinDistance(queueLoop[i].minDistance);
			sound->setMaxDistance(queueLoop[i].maxDistance);
			sound->setGain(queueLoop[i].volume);
			sound->setPitch(queueLoop[i].pitch * Game::getScale());
			sound->setLoop(1);
			sound->setOcclusion(0);
			sound->setSourceMask(SOUND_MASK);
			sound->play();

			SoundLoopData data;
			data.name = queueLoop[i].name;
			data.timeRemaining = queueLoop[i].duration;
			data.targetPitch = queueLoop[i].pitch;
			data.targetGain = queueLoop[i].volume;
			data.source = sound;

			soundsLoop.append(data);
		}
	}
	queueLoop.clear();
}

void SoundManager::updateSoundVolume()
{
	// set volume delayed
	if (delay_volume > 0)
	{
		delay_volume--;
		if (delay_volume <= 0)
			setVolume(target_volume);
	}
}

int SoundManager::shutdown()
{
	queue.clear();
	queueLoop.clear();
	sounds.clear();
	soundsLoop.clear();
	groups.clear();

	return 1;
}

void SoundManager::addSoundGroup(const NodePtr &node, const char *parameter_name)
{
	if (!node) 
		return;

	PropertyPtr prop = node->getProperty();
	if (!prop)
		return;

	String file_name = prop->getParameterPtr(parameter_name)->getValueString();
	if (!file_name.size())
		return;

	addSoundGroup(file_name);
}

void SoundManager::addSoundGroup(const char *file_name)
{
	// sound group is exist?
	if (groups.find(String(file_name)) != groups.end())
		return;

	groups[String(file_name)] = getSoundGroup(file_name);
}

Vector<String> SoundManager::getSoundGroup(const char *file_name)
{
	Vector<String> result;

	// make pattern (example: rename file_name "sound_1.oga" to "sound_?.oga")
	String original(file_name);
	String extension = "." + original.extension();

	// 1) find underline symbol (from end to start)
	bool has_underline = false;
	int underline_pos = original.size() - 1;
	for (int i = original.size() - 1; i >= 0; i--)
	{
		if (original[i] == '_')
		{
			has_underline = true;
			i = 0;
		}
		else if (original[i] == '/')
			i = 0;
		else
			underline_pos--;
	}
	if (!has_underline)
		underline_pos = original.size() - 1 - extension.size();

	// 2) calc size of postfix
	int num_size = original.size() - extension.size() - underline_pos - 1;

	// 3) make pattern!
	String pattern("");
	for (int i = 0; i <= underline_pos; i++)
		pattern.append(original[i]);
	for (int i = 0; i < num_size; i++)
		pattern.append('?');
	pattern.append(extension);

	// get all files in file_name directory
	Vector<String> files;
	FileSystem::getVirtualFiles(files);

	for (const String &file : files)
		if (is_same(pattern.get(), file))
			result.append(FileSystem::getAbsolutePath(file));

	return result;
}

int SoundManager::is_same(const char* pattern, const char* path)
{
	size_t len_pattern = strlen(pattern);
	size_t len_path = strlen(path);

	if (len_pattern > len_path || len_path <= 0 || len_pattern <= 0)
		return 0;

	size_t d = len_path - len_pattern;
	for (unsigned int i = 0; i < len_pattern; i++)
		if (pattern[i] != '?' && pattern[i] != path[i + d])
			return 0;

	return 1;
}

void SoundManager::warmAllSounds()
{
	SoundSourcePtr source = SoundSource::create("");

	for (auto i = groups.begin(); i != groups.end(); ++i)
		for (int k = 0; k < i->data.size(); k++)
			source->setSampleName(i->data[k].get());

	source.deleteLater();
}

void SoundManager::playSound(const char *file_name, const Vec3 &pos, float volume, float pitch, float min_distance, float max_distance)
{
	// check empty string
	if (file_name[0] == '\0')
		return;

	// this sound added to queue already?
	for (int i = 0; i < queue.size(); i++)
		if (strcmp(queue[i].name.get(), file_name) == 0)
			return;

	// can we reuse currently playing sound with the same sample name?
	int similar_sound_count = 0;
	for (int i = 0; i < sounds.size(); i++)
		if (sounds[i].source->isPlaying() && strcmp(sounds[i].name.get(), file_name) == 0)
		{
			// don't play this, if sound quieter than now
			//if (volume < 0.3f && sounds[i].source->getGain() > volume)
			//	return;

			// don't play this sound very often
			if (sounds[i].source->getTime() < MIN_DELAY_BETWEEN_SAME_SOUNDS * Game::getScale())
				return;

			similar_sound_count++;
			if (similar_sound_count >= SIMILAR_SOUND_LIMIT)
			{
				// all right. replay (reuse) it now
				String pattern(file_name);
				auto group = groups.find(pattern);
				sounds[i].source->stop();
				Sound::renderWorld(1);
				sounds[i].source->setSampleName(group == groups.end() ? pattern.get() : group->data[Game::getRandomInt(0, group->data.size())].get());
				sounds[i].source->setPosition(pos);
				sounds[i].source->setGain(volume);
				sounds[i].source->setPitch(pitch * Game::getScale());
				sounds[i].source->setMinDistance(min_distance);
				sounds[i].source->setMaxDistance(max_distance);
				sounds[i].source->setTime(0);
				sounds[i].source->play();
				return;
			}
		}

	// it's a new sound!
	// add it to queue (in C++ i can't create SoundSource in updatePhysics() stage)
	SoundQueue q;
	String pattern(file_name);
	auto group = groups.find(pattern);
	q.name = pattern;
	if (group == groups.end() || group->data.size() == 0)
		q.fileName = pattern;
	else 
		q.fileName = group->data[Game::getRandomInt(0, group->data.size())];
	q.position = pos;
	q.volume = volume;
	q.pitch = pitch;
	q.minDistance = min_distance;
	q.maxDistance = max_distance;
	queue.append(q);
}

void SoundManager::playLoopSound(const char *file_name, const NodePtr &parent, float duration, float volume, float pitch, float min_distance, float max_distance)
{
	// check empty string
	if (file_name[0] == '\0')
		return;

	// this sound added to queue already?
	for (int i = 0; i < queueLoop.size(); i++)
		if (strcmp(queueLoop[i].name.get(), file_name) == 0)
			return;

	// if this sound playing already, simply modifing timeRemaining field
	for (int i = 0; i < soundsLoop.size(); i++)
		if (soundsLoop[i].timeRemaining > 0 && 
			strcmp(soundsLoop[i].name.get(), file_name) == 0 &&
			soundsLoop[i].source->getParent() == parent)
		{
			soundsLoop[i].timeRemaining = duration;
			soundsLoop[i].targetGain = volume;
			soundsLoop[i].targetPitch = pitch;
			return;
		}

	// limit check
	int similar_sound_count = 0;
	for (int i = 0; i < soundsLoop.size(); i++)
		if (soundsLoop[i].timeRemaining > 0 &&
			strcmp(soundsLoop[i].name.get(), file_name) == 0)
		{
			similar_sound_count++;
			if (similar_sound_count >= SIMILAR_LOOP_SOUND_LIMIT)
				return;
		}

	// it's a new loop sound!
	// add it to queue (in C++ i can't create SoundSource in updatePhysics() stage)
	SoundLoopQueue q;
	String pattern(file_name);
	auto group = groups.find(pattern);
	q.name = pattern;
	q.fileName = (group == groups.end() ? pattern : group->data[Game::getRandomInt(0, group->data.size())]);

	if (group == groups.end() || group->data.size() == 0)
		q.fileName = pattern;
	else
		q.fileName = group->data[Game::getRandomInt(0, group->data.size())];
	q.parent = parent;
	q.duration = duration;
	q.volume = volume;
	q.pitch = pitch;
	q.minDistance = min_distance;
	q.maxDistance = max_distance;
	queueLoop.append(q);
}

void SoundManager::setVolume(float volume, int delay)
{
	if (delay == 0)
	{
		Sound::setVolume(volume);
		Sound::renderWorld(1);
	}

	delay_volume = delay;
	target_volume = volume;
}

float SoundManager::getVolume()
{
	return Sound::getVolume();
}

void SoundManager::setSoundsVolume(float volume)
{
	Sound::setSourceVolume(SOUND_CHANNEL, volume);
}

float SoundManager::getSoundVolume()
{
	return Sound::getSourceVolume(SOUND_CHANNEL);
}

void SoundManager::setMusicVolume(float volume)
{
	Sound::setSourceVolume(MUSIC_CHANNEL, volume);
}

float SoundManager::getMusicVolume()
{
	return Sound::getSourceVolume(MUSIC_CHANNEL);
}


