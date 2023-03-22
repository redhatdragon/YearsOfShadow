/* Copyright (C) 2005-2022, UNIGINE. All rights reserved.
 *
 * This file is a part of the UNIGINE 2 SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE. at http://unigine.com/
 */

// DO NOT EDIT DIRECTLY. This is an auto-generated file. Your changes will be lost.

#pragma once

#include "UnigineNode.h"
#include "UnigineXml.h"

namespace Unigine
{

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Sound
{
protected:
	

public:
	static int isInitialized(); 

	enum
	{
		REVERB_DISABLED = 0,
		REVERB_SINGLE,
		REVERB_MULTIPLE,
	};

	enum
	{
		NUM_SOURCES = 32,
	};

	enum
	{
		ATTENUATION_INVERSE = 0,
		ATTENUATION_INVERSE_CLAMPED,
		ATTENUATION_LINEAR,
		ATTENUATION_LINEAR_CLAMPED,
		ATTENUATION_EXPONENT,
		ATTENUATION_EXPONENT_CLAMPED,
	};
	static void setEnabled(bool enabled);
	static bool isEnabled();
	static void setData(const char *data);
	static const char *getData();
	static float getTotalTime();
	static void setVolume(float volume);
	static float getVolume();
	static void setVelocity(float velocity);
	static float getVelocity();
	static void setAdaptation(float adaptation);
	static float getAdaptation();
	static void setDoppler(float doppler);
	static float getDoppler();
	static void setScale(float scale);
	static float getScale();
	static void setAttenuation(int attenuation);
	static int getAttenuation();
	static void setHRTF(bool hrtf);
	static bool isHRTF();
	static void setSourceReverbMode(int mode);
	static int getSourceReverbMode();
	static void setSourceOcclusion(bool occlusion);
	static bool isSourceOcclusion();
	static void setSourceVolume(int source, float volume);
	static float getSourceVolume(int source);
	static void setSourceLimit(int source, int limit);
	static int getSourceLimit(int source);
	static void renderWorld(int force);
	static bool loadSettings(const char *name, bool clear = false);
	static bool saveSettings(const char *name, int force = 0);
	static bool loadWorld(const Ptr<Xml> &xml);
	static bool saveWorld(const Ptr<Xml> &xml, int force = 0);
	static bool restoreState(const Ptr<Stream> &stream);
	static bool saveState(const Ptr<Stream> &stream);
};

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Sounds
{
protected:
	

public:
	static int isInitialized(); 
	static bool isDeviceConnected();
	static bool isDeviceEnumerationSupported();
	static int getNumDevices();
	static const char *getDeviceName(int num);
	static const char *getDefaultDeviceName();
	static void setCurrentDeviceName(const char *name);
	static const char *getCurrentDeviceName();
	static void updateDeviceList();
};


class UNIGINE_API AmbientSource : public APIInterface
{
public:
	static Ptr<AmbientSource> create(const char *name, int stream = 0);
	void setGain(float gain);
	float getGain() const;
	float getLength() const;
	void setLoop(int loop);
	int getLoop() const;
	void setPitch(float pitch);
	float getPitch() const;
	void setSampleName(const char *name);
	const char *getSampleName() const;
	void setSourceMask(int mask);
	int getSourceMask() const;
	void setTime(float time);
	float getTime() const;
	bool isPlaying() const;
	bool isStopped() const;
	void play();
	void stop();
};
typedef Ptr<AmbientSource> AmbientSourcePtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API SoundSource : public Node
{
public:
	static int type() { return Node::SOUND_SOURCE; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<SoundSource> create(const char *name, int stream = 0);
	void setGain(float gain);
	float getGain() const;
	float getLength() const;
	void setLoop(int loop);
	int getLoop() const;
	void setPlayOnEnable(bool enable);
	bool isPlayOnEnable() const;
	void setRestartOnEnable(bool enable);
	bool isRestartOnEnable() const;
	void setStream(bool stream);
	bool isStream() const;
	void setPitch(float pitch);
	float getPitch() const;
	void setSampleName(const char *name);
	const char *getSampleName() const;
	void setSourceMask(int mask);
	int getSourceMask() const;
	void setReverbMask(int mask);
	int getReverbMask() const;
	void setTime(float time);
	float getTime() const;
	void setAirAbsorption(float absorption);
	float getAirAbsorption() const;
	void setConeInnerAngle(float angle);
	float getConeInnerAngle() const;
	void setConeOuterAngle(float angle);
	float getConeOuterAngle() const;
	void setConeOuterGainHF(float coneoutergainhf);
	float getConeOuterGainHF() const;
	void setConeOuterGain(float gain);
	float getConeOuterGain() const;
	void setMaxDistance(float distance);
	float getMaxDistance() const;
	void setMinDistance(float distance);
	float getMinDistance() const;
	void setOcclusionMask(int mask);
	int getOcclusionMask() const;
	void setOcclusion(int occlusion);
	int getOcclusion() const;
	void setAdaptation(float adaptation);
	float getAdaptation() const;
	void setRoomRolloff(float rolloff);
	float getRoomRolloff() const;
	bool isPlaying() const;
	bool isStopped() const;
	void play();
	void stop();
};
typedef Ptr<SoundSource> SoundSourcePtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API SoundReverb : public Node
{
public:
	static int type() { return Node::SOUND_REVERB; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<SoundReverb> create(const Math::vec3 &arg1);
	void setReverbMask(int mask);
	int getReverbMask() const;
	void setRoomRolloff(float rolloff);
	float getRoomRolloff() const;
	void setAirAbsorption(float absorption);
	float getAirAbsorption() const;
	void setDecayHFRatio(float ratio);
	float getDecayHFRatio() const;
	void setDecayLFRatio(float ratio);
	float getDecayLFRatio() const;
	void setDecayTime(float time);
	float getDecayTime() const;
	void setDensity(float density);
	float getDensity() const;
	void setDiffusion(float diffusion);
	float getDiffusion() const;
	void setEchoDepth(float depth);
	float getEchoDepth() const;
	void setEchoTime(float time);
	float getEchoTime() const;
	void setGain(float gain);
	float getGain() const;
	void setGainHF(float gainhf);
	float getGainHF() const;
	void setGainLF(float gainlf);
	float getGainLF() const;
	void setLateReverbDelay(float delay);
	float getLateReverbDelay() const;
	void setLateReverbGain(float gain);
	float getLateReverbGain() const;
	void setModulationDepth(float depth);
	float getModulationDepth() const;
	void setModulationTime(float time);
	float getModulationTime() const;
	void setReflectionDelay(float delay);
	float getReflectionDelay() const;
	void setReflectionGain(float gain);
	float getReflectionGain() const;
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
	void setThreshold(const Math::vec3 &threshold);
	Math::vec3 getThreshold() const;
};
typedef Ptr<SoundReverb> SoundReverbPtr;

} // namespace Unigine
