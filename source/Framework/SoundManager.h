#pragma once
#include <UnigineNode.h>
#include <UnigineMap.h>
#include <UnigineSounds.h>
#include <UnigineLogic.h>

using namespace Unigine;
using namespace Math;

#define SOUND_CHANNEL 0
#define MUSIC_CHANNEL 1
#define SOUND_MASK 1
#define MUSIC_MASK 2

class SoundManager : public Unigine::WorldLogic
{
public:
	static SoundManager *get()
	{
		static SoundManager instance;
		return &instance;
	}

	int init() override;
	int update() override;
	int shutdown() override;

	// use it before calling playSound/playLoopSound if necessary
	void addSoundGroup(const NodePtr &node, const char *parameter_name); // add sound group via getParameterString() from node
	void addSoundGroup(const char *file_name); // find group of sounds and add it to SoundManager
	Vector<String> getSoundGroup(const char *file_name); // find group of sounds with name like sound_01.wav, sound_02.wav, sound_03.wav...

	// warming
	void warmAllSounds();

	// use it for "impact" sounds
	void playSound(const char *file_name, const Vec3 &pos, float volume = 1.0f, float pitch = 1.0f, float min_distance = 10.0f, float max_distance = 100.0f);

	// use it for "slide" sounds
	void playLoopSound(const char *file_name, const NodePtr &parent, float duration = 0.3f, float volume = 1.0f, float pitch = 1.0f, float min_distance = 10.0f, float max_distance = 100.0f);

	void setVolume(float volume, int delay = 0);
	float getVolume();

	void setSoundsVolume(float volume);
	float getSoundVolume();

	void setMusicVolume(float volume);
	float getMusicVolume();

private:
	struct SoundQueue
	{
		String name;		// name of the pattern (reference to sound group)
		String fileName;	// name of the file
		float volume = 1.0f;
		float pitch = 1.0f;
		float minDistance = 2.0f;
		float maxDistance = 15.0f;
		Vec3 position = Vec3_zero;
	};

	struct SoundLoopQueue
	{
		String name;		// name of the pattern (reference to sound group)
		String fileName;	// name of the file
		float volume = 1.0f;
		float pitch = 1.0f;
		float minDistance = 2.0f;
		float maxDistance = 15.0f;
		NodePtr parent;
		float duration = 0.3f;
	};

	struct SoundData
	{
		String name;		// name of the pattern (reference to sound group)
		SoundSourcePtr source;
	};

	struct SoundLoopData
	{
		String name;		// name of the pattern (reference to sound group)
		SoundSourcePtr source;
		float timeRemaining;
		float targetGain;
		float targetPitch;
	};

	Vector<SoundQueue> queue;			// queue of new sounds that SoundManager must play
	Vector<SoundLoopQueue> queueLoop;
	Vector<SoundData> sounds;			// current playing sounds
	Vector<SoundLoopData> soundsLoop;
	Map<String, Vector<String>> groups;	// groups of similar sounds

	int delay_volume = 0;	// TO DO: Rewrite it on threads (or same things)
	float target_volume = 1;

	SoundManager();
	virtual ~SoundManager();
	SoundManager(SoundManager const&) = delete;
	SoundManager& operator= (SoundManager const&) = delete;

	int is_same(const char* pattern, const char* path);
	void updateSounds();
	void updateLoopSounds();
	void updateSoundVolume();
};