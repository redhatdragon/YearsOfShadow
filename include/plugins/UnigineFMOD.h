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

#include <UnigineEngine.h>
#include <UnigineMathLib.h>
#include <UnigineNode.h>
#include <UnigineVector.h>

namespace Unigine
{
namespace Plugins
{


namespace FMOD
{

class DSPType
{
protected:
	virtual ~DSPType(){}
public:

	enum TYPE
	{
		UNKNOWN,
		MIXER,
		OSCILLATOR,
		LOWPASS,
		ITLOWPASS,
		HIGHPASS,
		ECHO,
		FADER,
		FLANGE,
		DISTORTION,
		NORMALIZE,
		LIMITER,
		PARAMEQ,
		PITCHSHIFT,
		CHORUS,
		VSTPLUGIN,
		WINAMPPLUGIN,
		ITECHO,
		COMPRESSOR,
		SFXREVERB,
		LOWPASS_SIMPLE,
		DELAY,
		TREMOLO,
		LADSPAPLUGIN,
		SEND,
		RETURN,
		HIGHPASS_SIMPLE,
		PAN,
		THREE_EQ,
		FFT,
		LOUDNESS_METER,
		ENVELOPEFOLLOWER,
		CONVOLUTIONREVERB,
		CHANNELMIX,
		TRANSCEIVER,
		OBJECTPAN,
		MULTIBAND_EQ,
		MAX,
	};
};


class FMODEnums
{
protected:
	virtual ~FMODEnums(){}
public:

	enum FMOD_MODE
	{
		DEFAULT = 0x00000000,
		LOOP_OFF = 0x00000001,
		LOOP_NORMAL = 0x00000002,
		LOOP_BIDI = 0x00000004,
		_2D = 0x00000008,
		_3D = 0x00000010,
		CREATESTREAM = 0x00000080,
		CREATESAMPLE = 0x00000100,
		CREATECOMPRESSEDSAMPLE = 0x00000200,
		OPENUSER = 0x00000400,
		OPENMEMORY = 0x00000800,
		OPENMEMORY_POINT = 0x10000000,
		OPENRAW = 0x00001000,
		OPENONLY = 0x00002000,
		ACCURATETIME = 0x00004000,
		MPEGSEARCH = 0x00008000,
		NONBLOCKING = 0x00010000,
		UNIQUE = 0x00020000,
		_3D_HEADRELATIVE = 0x00040000,
		_3D_WORLDRELATIVE = 0x00080000,
		_3D_INVERSEROLLOFF = 0x00100000,
		_3D_LINEARROLLOFF = 0x00200000,
		_3D_LINEARSQUAREROLLOFF = 0x00400000,
		_3D_INVERSETAPEREDROLLOFF = 0x00800000,
		_3D_CUSTOMROLLOFF = 0x04000000,
		_3D_IGNOREGEOMETRY = 0x40000000,
		IGNORETAGS = 0x02000000,
		LOWMEM = 0x08000000,
	};

	enum INIT_FLAGS
	{
		NORMAL = 0x00000000,
		STREAM_FROM_UPDATE = 0x00000001,
		MIX_FROM_UPDATE = 0x00000002,
		_3D_RIGHTHANDED = 0x00000004,
		CHANNEL_LOWPASS = 0x00000100,
		CHANNEL_DISTANCEFILTER = 0x00000200,
		PROFILE_ENABLE = 0x00010000,
		VOL0_BECOMES_VIRTUAL = 0x00020000,
		GEOMETRY_USECLOSEST = 0x00040000,
		PREFER_DOLBY_DOWNMIX = 0x00080000,
		THREAD_UNSAFE = 0x00100000,
		PROFILE_METER_ALL = 0x00200000,
		MEMORY_TRACKING = 0x00400000,
	};

	enum TIME_UNIT
	{
		MS = 0x00000001,
		PCM = 0x00000002,
		PCMBYTES = 0x00000004,
		RAWBYTES = 0x00000008,
		PCMFRACTION = 0x00000010,
		MODORDER = 0x00000100,
		MODROW = 0x00000200,
		MODPATTERN = 0x00000400,
	};

	enum ERROR_TYPE
	{
		OK,
		ERR_BADCOMMAND,
		ERR_CHANNEL_ALLOC,
		ERR_CHANNEL_STOLEN,
		ERR_DMA,
		ERR_DSP_CONNECTION,
		ERR_DSP_DONTPROCESS,
		ERR_DSP_FORMAT,
		ERR_DSP_INUSE,
		ERR_DSP_NOTFOUND,
		ERR_DSP_RESERVED,
		ERR_DSP_SILENCE,
		ERR_DSP_TYPE,
		ERR_FILE_BAD,
		ERR_FILE_COULDNOTSEEK,
		ERR_FILE_DISKEJECTED,
		ERR_FILE_EOF,
		ERR_FILE_ENDOFDATA,
		ERR_FILE_NOTFOUND,
		ERR_FORMAT,
		ERR_HEADER_MISMATCH,
		ERR_HTTP,
		ERR_HTTP_ACCESS,
		ERR_HTTP_PROXY_AUTH,
		ERR_HTTP_SERVER_ERROR,
		ERR_HTTP_TIMEOUT,
		ERR_INITIALIZATION,
		ERR_INITIALIZED,
		ERR_INTERNAL,
		ERR_INVALID_FLOAT,
		ERR_INVALID_HANDLE,
		ERR_INVALID_PARAM,
		ERR_INVALID_POSITION,
		ERR_INVALID_SPEAKER,
		ERR_INVALID_SYNCPOINT,
		ERR_INVALID_THREAD,
		ERR_INVALID_VECTOR,
		ERR_MAXAUDIBLE,
		ERR_MEMORY,
		ERR_MEMORY_CANTPOINT,
		ERR_NEEDS3D,
		ERR_NEEDSHARDWARE,
		ERR_NET_CONNECT,
		ERR_NET_SOCKET_ERROR,
		ERR_NET_URL,
		ERR_NET_WOULD_BLOCK,
		ERR_NOTREADY,
		ERR_OUTPUT_ALLOCATED,
		ERR_OUTPUT_CREATEBUFFER,
		ERR_OUTPUT_DRIVERCALL,
		ERR_OUTPUT_FORMAT,
		ERR_OUTPUT_INIT,
		ERR_OUTPUT_NODRIVERS,
		ERR_PLUGIN,
		ERR_PLUGIN_MISSING,
		ERR_PLUGIN_RESOURCE,
		ERR_PLUGIN_VERSION,
		ERR_RECORD,
		ERR_REVERB_CHANNELGROUP,
		ERR_REVERB_INSTANCE,
		ERR_SUBSOUNDS,
		ERR_SUBSOUND_ALLOCATED,
		ERR_SUBSOUND_CANTMOVE,
		ERR_TAGNOTFOUND,
		ERR_TOOMANYCHANNELS,
		ERR_TRUNCATED,
		ERR_UNIMPLEMENTED,
		ERR_UNINITIALIZED,
		ERR_UNSUPPORTED,
		ERR_VERSION,
		ERR_EVENT_ALREADY_LOADED,
		ERR_EVENT_LIVEUPDATE_BUSY,
		ERR_EVENT_LIVEUPDATE_MISMATCH,
		ERR_EVENT_LIVEUPDATE_TIMEOUT,
		ERR_EVENT_NOTFOUND,
		ERR_STUDIO_UNINITIALIZED,
		ERR_STUDIO_NOT_LOADED,
		ERR_INVALID_STRING,
		ERR_ALREADY_LOCKED,
		ERR_NOT_LOCKED,
		ERR_RECORD_DISCONNECTED,
		ERR_TOOMANYSAMPLES,
		RESULT_FORCEINT = 65536,
	};

	enum SOUND_TYPE
	{
		UNKNOWN,
		AIFF,
		ASF,
		DLS,
		FLAC,
		FSB,
		IT,
		MIDI,
		MOD,
		MPEG,
		OGGVORBIS,
		PLAYLIST,
		RAW,
		S3M,
		USER,
		WAV,
		XM,
		XMA,
		AUDIOQUEUE,
		AT9,
		VORBIS,
		MEDIA_FOUNDATION,
		MEDIACODEC,
		FADPCM,
		OPUS,
		MAX,
	};

	enum SOUND_FORMAT
	{
		NONE,
		PCM8,
		PCM16,
		PCM24,
		PCM32,
		PCMFLOAT,
		BITSTREAM,
		FORMAT_MAX,
	};

	enum DSP_CONNECTION_TYPE
	{
		STANDARD,
		CONNECTION_TYPE_SIDECHAIN,
		SEND,
		SEND_SIDECHAIN,
		CONNECTION_TYPE_MAX,
	};

	enum PARAMETER_DATA_TYPE
	{
		DATA_TYPE_USER = 0,
		OVERALLGAIN = -1,
		_3DATTRIBUTES = -2,
		SIDECHAIN = -3,
		FFT = -4,
		_3DATTRIBUTES_MULTI = -5,
		ATTENUATION_RANGE = -6,
	};
};


class SoundGroup
{
protected:
	virtual ~SoundGroup(){}
public:
	virtual void* getFMOD() = 0;
};


class Geometry
{
protected:
	virtual ~Geometry(){}
public:
	virtual void* getFMOD() = 0;
};


class Reverb3D
{
protected:
	virtual ~Reverb3D(){}
public:
	virtual void* getFMOD() = 0;
};


class ChannelControl
{
protected:
	virtual ~ChannelControl(){}
public:
	virtual void* getFMOD() = 0;
};


class DSP
{
protected:
	virtual ~DSP(){}
public:
	virtual void setParameterFloat(int index, float value) = 0;
	virtual void setParameterInt(int index, int value) = 0;
	virtual void release() = 0;
	virtual void releaseFromChannel() = 0;
	virtual void* getFMOD() = 0;
};


class Channel
{
protected:
	virtual ~Channel(){}
public:
	virtual void unregisterDSP(void *fmod_dsp) = 0;
	virtual bool containsDSP(void *fmod_dsp) const = 0;
	virtual Unigine::Plugins::FMOD::DSP* getDSP(void *fmod_dsp) = 0;
	virtual void addDSP(Unigine::Plugins::FMOD::DSP* fmod_dsp) = 0;
	virtual void setPaused(bool paused) = 0;
	virtual bool isPaused() const = 0;
	virtual void setVolume(float volume) = 0;
	virtual float getVolume() const = 0;
	virtual void setVolumeRamp(bool ramp) = 0;
	virtual bool isVolumeRamp() const = 0;
	virtual void setMute(bool mute) = 0;
	virtual bool isMute() const = 0;
	virtual bool isPlaying() const = 0;
	virtual void stop() = 0;
	virtual void setMinMaxDistance(float min, float max) = 0;
	virtual void setPosition(const Math::Vec3 &position) = 0;
	virtual void setPositionTimeLine(unsigned int position, FMODEnums::TIME_UNIT time_unit) = 0;
	virtual void getPositionTimeLine(unsigned int &position, FMODEnums::TIME_UNIT time_unit) = 0;
	virtual void release() = 0;
	virtual void setVelocity(const Math::Vec3 &velocity) = 0;
	virtual DSP * addDSP(int index, DSPType::TYPE dsp_type) = 0;
	virtual void removeDSP(int index) = 0;
	virtual DSP * getDSP(int index) = 0;
	virtual void setFrequency(float frequency) = 0;
	virtual float getFrequency() const = 0;
	virtual void setPitch(float pitch) = 0;
	virtual float getPitch() const = 0;
	virtual void setPriority(int priority) = 0;
	virtual int getPriority() const = 0;
	virtual void setLoopCount(int count) = 0;
	virtual int getLoopCount() const = 0;
	virtual float getAudibility() const = 0;
	virtual void setPan(float pan) = 0;
	virtual bool isSoundOnChannel(void *sound) const = 0;
	virtual bool isVirtual() const = 0;
	virtual void* getFMOD() = 0;
};


class ChannelGroup
{
protected:
	virtual ~ChannelGroup(){}
public:
	virtual Channel * getChannel(int id) = 0;
	virtual int getChannelCount() = 0;
	virtual void setVolume(float volume) = 0;
	virtual void addChannel(Channel *channel) = 0;
	virtual void* getFMOD() = 0;
};


class Sound
{
protected:
	virtual ~Sound(){}
public:
	virtual Channel * play() = 0;
	virtual void release() = 0;
	virtual unsigned int getLength(FMODEnums::TIME_UNIT time_unit) = 0;
	virtual void* getFMOD() = 0;
};


class FMODCore
{
protected:
	virtual ~FMODCore(){}
public:
	virtual void close() = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void initCore(int max_channels, FMODEnums::INIT_FLAGS flags) = 0;
	virtual Sound * createSound(const char *name_of_data, FMODEnums::FMOD_MODE mode) = 0;
	virtual Sound * createStream(const char *url) = 0;
	virtual Channel * playSound(Sound *sound, ChannelGroup *channel_group, bool paused) = 0;
	virtual void setListener3DAttributes(int listener, const Math::Vec3 &position, const Math::Vec3 &up, const Math::Vec3 &forward, const Math::Vec3 &velocity) = 0;
	virtual void getListener3DAttributes(int listener, Math::Vec3 &position, Math::Vec3 &up, Math::Vec3 &forward, Math::Vec3 &velocity) = 0;
	virtual Channel * getChannel(int index) = 0;
	virtual void* getFMOD() = 0;
	virtual Unigine::Plugins::FMOD::Sound* getSound(void* fmod_sound) = 0;
};


class EventInstance
{
protected:
	virtual ~EventInstance(){}
public:
	virtual void setPaused(bool paused) = 0;
	virtual bool isPaused() const = 0;
	virtual void setPitch(float pitch) = 0;
	virtual float getPitch() const = 0;
	virtual void setGain(float gain) = 0;
	virtual float getGain() const = 0;
	virtual void setMaxDistance(float distance) = 0;
	virtual float getMaxDistance() const = 0;
	virtual void setMinDistance(float distance) = 0;
	virtual float getMinDistance() const = 0;
	virtual void setPriority(float priority) = 0;
	virtual float getPriority() const = 0;
	virtual void setTimeLinePosition(int position) = 0;
	virtual int getTimeLinePosition() const = 0;
	virtual void set3DAttributes(const Math::Vec3 &position, const Math::Vec3 &up, const Math::Vec3 &forward, const Math::Vec3 &velocity) = 0;
	virtual void get3DAttributes(Math::Vec3 &position, Math::Vec3 &up, Math::Vec3 &forward, Math::Vec3 &velocity) = 0;
	virtual void setVelocity(const Math::Vec3 &velocity) = 0;
	virtual bool isVirtual() const = 0;
	virtual bool isStarting() const = 0;
	virtual bool isStopping() const = 0;
	virtual bool isStopped() const = 0;
	virtual bool isSustaining() const = 0;
	virtual bool isPlaying() const = 0;
	virtual bool isValid() const = 0;
	virtual void play() = 0;
	virtual void stop() = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void setParameter(const char *name, float value) = 0;
	virtual float getParameter(const char *name) = 0;
	virtual void setTransform(const Math::Mat4 &transform) = 0;
	virtual void setParent(const Ptr<Node> &parent) = 0;
	virtual void releaseFromDefinition() = 0;
	virtual String getDescriptionPath() const = 0;
	virtual void* getFMOD() = 0;
};


class EventDescription
{
protected:
	virtual ~EventDescription(){}
public:
	virtual bool isSampleUnloading() const = 0;
	virtual bool isSampleUnloaded() const = 0;
	virtual bool isSampleLoading() const = 0;
	virtual bool isSampleLoaded() const = 0;
	virtual bool isSampleError() const = 0;
	virtual bool isValid() const = 0;
	virtual int getLength() const = 0;
	virtual bool isDopplerEnabled() const = 0;
	virtual bool isOneShot() const = 0;
	virtual EventInstance * createEvent() = 0;
	virtual void releaseAllEvents() = 0;
	virtual void loadSampleData() = 0;
	virtual void unloadSampleData() = 0;
	virtual void release() = 0;
	virtual void releaseFromStudio() = 0;
	virtual void update() = 0;
	virtual String getPath() const = 0;
	virtual void* getFMOD() = 0;
};


class Bus
{
protected:
	virtual ~Bus(){}
public:
	virtual void setPaused(bool paused) = 0;
	virtual bool isPaused() const = 0;
	virtual void setVolume(float volume) = 0;
	virtual float getVolume() const = 0;
	virtual void setMuted(bool muted) = 0;
	virtual bool isMuted() const = 0;
	virtual bool isValid() const = 0;
	virtual void stopAllEvents() = 0;
	virtual void release() = 0;
	virtual String getPath() const = 0;
	virtual void* getFMOD() = 0;
};


class VCA
{
protected:
	virtual ~VCA(){}
public:
	virtual void setVolume(float volume) = 0;
	virtual float getVolume() const = 0;
	virtual void release() = 0;
	virtual void releaseFromStudio() = 0;
	virtual bool isValid() const = 0;
	virtual String getPath() const = 0;
	virtual void* getFMOD() = 0;
};


class Bank
{
protected:
	virtual ~Bank(){}
public:
	virtual bool isUnloading() const = 0;
	virtual bool isUnloaded() const = 0;
	virtual bool isLoading() const = 0;
	virtual bool isLoaded() const = 0;
	virtual bool isError() const = 0;
	virtual bool isValid() const = 0;
	virtual EventDescription * getEventDescription(int index) = 0;
	virtual Bus * getBus(int index) = 0;
	virtual VCA * getVCA(int index) = 0;
	virtual int getEventDescriptionCount() = 0;
	virtual int getBusCount() = 0;
	virtual int getVCACount() = 0;
	virtual void loadSampleData() = 0;
	virtual void unloadSampleData() = 0;
	virtual void release() = 0;
	virtual String getPath() const = 0;
	virtual void* getFMOD() = 0;
};


class FMODStudio
{
protected:
	virtual ~FMODStudio(){}
public:
	struct BufferInfo
	{
	 int current_usage;
	 int peak_usage;
	 int capacity;
	 int stall_count;
	 float stall_time;


	};

	struct BufferUsage
	{
	 BufferInfo studio_command_queue;
	 BufferInfo studio_handle;
	};

	struct CPUUsage
	{
	 float dsp;
	 float stream;
	 float geometry;
	 float update;
	 float convolution1;
	 float convolution2;
	};

	struct CPUUsageTotal
	{
	 float studio_cpu_usage_update;
	 CPUUsage usage_core;
	};

	virtual Bank * loadBank(const char *path) = 0;
	virtual bool unloadBank(Bank *bank) = 0;
	virtual bool unloadBank(const char *path) = 0;
	virtual VCA * getVCA(const char *path) = 0;
	virtual Bus * getBus(const char *path) = 0;
	virtual EventInstance * getEvent(const char *path) = 0;
	virtual EventDescription * getEventDescription(const char *path) = 0;
	virtual void setListener3DAttributes(int listener, const Math::Vec3 &position, const Math::Vec3 &up, const Math::Vec3 &forward, const Math::Vec3 &velocity) = 0;
	virtual void getListener3DAttributes(int listener, Math::Vec3 &position, Math::Vec3 &up, Math::Vec3 &forward, Math::Vec3 &velocity) = 0;
	virtual void setListenerVelocity(const Math::Vec3 &velocity) = 0;
	virtual void setListenerTransform(const Math::Mat4 &transform) = 0;
	virtual void setParentForListener(const Ptr<Node> &parent) = 0;
	virtual void useStudioLiveUpdateFlag() = 0;
	virtual void useStudioSyncUpdateFlag() = 0;
	virtual void useStudioLoadFromUpdateFlag() = 0;
	virtual void useCoreStreamFromUpdateFlag() = 0;
	virtual void useChannelsCount(int count) = 0;
	virtual void update() = 0;
	virtual void initStudio() = 0;
	virtual Bank * getBank(int id) = 0;
	virtual EventDescription * getEventDescription(int id) = 0;
	virtual Bus * getBus(int id) = 0;
	virtual VCA * getVCA(int id) = 0;
	virtual int getBankCount() = 0;
	virtual int getEventDescriptionCount() = 0;
	virtual int getBusCount() = 0;
	virtual int getVCACount() = 0;
	virtual FMODStudio::BufferUsage getBufferUsage() = 0;
	virtual void resetBufferUsage() = 0;
	virtual FMODStudio::CPUUsageTotal getCPUUsageTotal() = 0;
	virtual void getMemoryUsage(int &exclusive, int &inclusive, int &sample_data) = 0;
	virtual void unregisterDescription(void *fmod_description) = 0;
	virtual void unregisterBus(void *fmod_bus) = 0;
	virtual void unregisterVCA(void *fmod_vca) = 0;
	virtual bool containsBank(void *fmod_bank) = 0;
	virtual bool containsBus(void *fmod_bus) = 0;
	virtual bool containsVCA(void *fmod_vca) = 0;
	virtual bool containsEventDescription(void *fmod_event_description) = 0;
	virtual Unigine::Plugins::FMOD::Bank* getBank(void *fmod_bank) = 0;
	virtual Unigine::Plugins::FMOD::Bus* getBus(void *fmod_bus) = 0;
	virtual Unigine::Plugins::FMOD::VCA* getVCA(void *fmod_vca) = 0;
	virtual Unigine::Plugins::FMOD::EventDescription* getEventDescription(void *fmod_event_description) = 0;
	virtual void *getFMOD() = 0;
	virtual void release() = 0;
	virtual void releaseBuses() = 0;
	virtual void releaseVCAs() = 0;
	virtual void releaseEventDescriptions() = 0;
	virtual void releaseBanks() = 0;
	virtual FMODCore *getCore() = 0;
};


class FMOD
{
protected:
	virtual ~FMOD(){}
public:
	UNIGINE_INLINE static FMOD *get() { return Unigine::Engine::get()->getPlugin<FMOD>("FMOD"); }
	virtual FMODCore * getCore() const = 0;
	virtual FMODStudio * getStudio() const = 0;
	virtual void update() = 0;
	virtual bool hasErrors(int &error_type) = 0;
public:
	static void checkPlugin(FMOD* plugin)
	{
	if (!plugin)
	{
		String name = "FMOD";
	#if defined(USE_DOUBLE) || defined(UNIGINE_DOUBLE)
			name += "_double";
	#endif
			name += "_x64";
	#ifndef NDEBUG
			name += "d";
	#endif
			name += ".dll";
			Log::fatal("Can not find FMOD plugin. Please check %s and fmod.dll, fmodL.dll, fmodstudio.dll, fmodstudioL.dll(You can download this files from official site) in bin directory", name.getRaw());
	}
	}
};

} // namespace FMOD
} // namespace Plugins
} // namespace Unigine
