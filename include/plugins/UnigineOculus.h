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
#include <UniginePlayers.h>

namespace Unigine
{
namespace Plugins
{


class Oculus
{
protected:
	virtual ~Oculus(){}
public:
	UNIGINE_INLINE static Oculus *get() { return Unigine::Engine::get()->getPlugin<Oculus>("Oculus"); }

	enum VIEWPORT
	{
		VIEWPORT_BLACK_SCREEN,
		VIEWPORT_MONO,
		VIEWPORT_STEREO,
		VIEWPORT_STEREO_FINAL,
	};

	enum DEVICE
	{
		DEVICE_HMD,
		DEVICE_CONTROLLER_LEFT,
		DEVICE_CONTROLLER_RIGHT,
	};

	enum BUTTON
	{
		BUTTON_A = 1,
		BUTTON_B = 2,
		BUTTON_RTHUMB = 4,
		BUTTON_RSHOULDER = 8,
		BUTTON_X = 256,
		BUTTON_Y = 512,
		BUTTON_LTHUMB = 1024,
		BUTTON_LSHOULDER = 2048,
		BUTTON_UP = 65536,
		BUTTON_DOWN = 131072,
		BUTTON_LEFT = 262144,
		BUTTON_RIGHT = 524288,
		BUTTON_ENTER = 1048576,
		BUTTON_BACK = 2097152,
		BUTTON_VOLUP = 4194304,
		BUTTON_VOLDOWN = 8388608,
		BUTTON_HOME = 16777216,
	};

	enum TOUCH
	{
		TOUCH_A = 1,
		TOUCH_B = 2,
		TOUCH_RTHUMB = 4,
		TOUCH_RTHUMB_REST = 8,
		TOUCH_RINDEX_TRIGGER = 16,
		TOUCH_RINDEX_POINTING = 32,
		TOUCH_RTHUMB_UP = 64,
		TOUCH_X = 256,
		TOUCH_Y = 512,
		TOUCH_LTHUMB = 1024,
		TOUCH_LTHUMB_REST = 2048,
		TOUCH_LINDEX_TRIGGER = 4096,
		TOUCH_LINDEX_POINTING = 8192,
		TOUCH_LTHUMB_UP = 16384,
	};

	enum AXIS
	{
		AXIS_INDEX_TRIGGER,
		AXIS_HAND_TRIGGER,
		AXIS_THUMBSTICK,
		AXIS_INDEX_TRIGGER_NO_DEADZONE,
		AXIS_HAND_TRIGGER_NO_DEADZONE,
		AXIS_THUMBSTICK_NO_DEADZONE,
	};
	virtual void setEnabled(bool enabled) = 0;
	virtual bool isEnabled() const = 0;
	virtual void setViewportMode(Oculus::VIEWPORT mode) = 0;
	virtual Oculus::VIEWPORT getViewportMode() const = 0;
	virtual const char *getVersion() const = 0;
	virtual int getHmdType() const = 0;
	virtual const char *getHmdProductName() const = 0;
	virtual const char *getHmdManufacturer() const = 0;
	virtual int getHmdVendorID() const = 0;
	virtual int getHmdProductID() const = 0;
	virtual int getHmdFirmwareMajor() const = 0;
	virtual int getHmdFirmwareMinor() const = 0;
	virtual float getHmdTrackerHFov() const = 0;
	virtual float getHmdTrackerVFov() const = 0;
	virtual float getHmdTrackerZNear() const = 0;
	virtual float getHmdTrackerZFar() const = 0;
	virtual int getDefaultHmdCaps() const = 0;
	virtual int getDefaultTrackingCaps() const = 0;
	virtual int getAvailableHmdCaps() const = 0;
	virtual int getAvailableTrackingCaps() const = 0;
	virtual Math::vec4 getHmdDefaultFov(int num) const = 0;
	virtual Math::vec4 getHmdMaxFov(int num) const = 0;
	virtual int getHmdWidth() const = 0;
	virtual int getHmdHeight() const = 0;
	virtual const char *getHmdSerialNumber() const = 0;
	virtual void setPositionScale(float scale) = 0;
	virtual float getPositionScale() const = 0;
	virtual void resetTracking() = 0;
	virtual Math::vec4 getEyeFov(int num) const = 0;
	virtual Math::vec3 getEyePosition(int num) const = 0;
	virtual Math::quat getEyeRotation(int num) const = 0;
	virtual Math::vec3 getHeadPosition() const = 0;
	virtual Math::quat getHeadRotation() const = 0;
	virtual Math::vec3 getHeadLinearVelocity() const = 0;
	virtual Math::vec3 getHeadAngularVelocity() const = 0;
	virtual Math::vec3 getHeadLinearAcceleration() const = 0;
	virtual Math::vec3 getHeadAngularAcceleration() const = 0;
	virtual Math::vec3 getHandPosition(int num) const = 0;
	virtual Math::quat getHandRotation(int num) const = 0;
	virtual Math::vec3 getTrackerPosition() const = 0;
	virtual Math::quat getTrackerRotation() const = 0;
	virtual Math::vec3 getLeveledTrackerPosition() const = 0;
	virtual Math::quat getLeveledTrackerRotation() const = 0;
	virtual unsigned int getTrackingStatus() const = 0;
	virtual bool isVisible() const = 0;
	virtual void setHeadPositionLock(bool lock) = 0;
	virtual bool isHeadPositionLock() const = 0;
	virtual void setHeadRotationLock(bool lock) = 0;
	virtual bool isHeadRotationLock() const = 0;
	virtual Math::mat4 getDevicePose(Oculus::DEVICE device) = 0;
	virtual Math::vec3 getDeviceVelocity(Oculus::DEVICE device) = 0;
	virtual Math::vec3 getDeviceAngularVelocity(Oculus::DEVICE device) = 0;
	virtual bool isDeviceConnected(Oculus::DEVICE device) = 0;
	virtual bool isPoseValid(Oculus::DEVICE device) = 0;
	virtual bool getControllerButtonPressed(Oculus::BUTTON button) = 0;
	virtual bool getControllerButtonTouched(Oculus::TOUCH button) = 0;
	virtual Math::vec2 getControllerAxis(Oculus::DEVICE device, Oculus::AXIS axis) = 0;
	virtual void setControllerVibration(Oculus::DEVICE device, unsigned short duration, float amplitude) = 0;
	virtual void render(const Ptr<Player> &player, const Math::ivec2 &size, bool render_window) = 0;
};

} // namespace Plugins
} // namespace Unigine
