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


class OpenVR
{
protected:
	virtual ~OpenVR(){}
public:
	UNIGINE_INLINE static OpenVR *get() { return Unigine::Engine::get()->getPlugin<OpenVR>("OpenVR"); }

	enum VIEWPORT
	{
		VIEWPORT_BLACK_SCREEN,
		VIEWPORT_MONO,
		VIEWPORT_STEREO,
	};

	enum DEVICE
	{
		DEVICE_INVALID,
		DEVICE_HMD,
		DEVICE_CONTROLLER,
		DEVICE_GENERIC_TRACKER,
		DEVICE_TRACKING,
	};

	enum AXIS
	{
		AXIS_NONE,
		AXIS_TRACKPAD,
		AXIS_JOYSTICK,
		AXIS_TRIGGER,
	};

	enum BUTTON
	{
		BUTTON_SYSTEM = 0,
		BUTTON_APPLICATIONMENU = 1,
		BUTTON_GRIP = 2,
		BUTTON_DPAD_LEFT = 3,
		BUTTON_DPAD_UP = 4,
		BUTTON_DPAD_RIGHT = 5,
		BUTTON_DPAD_DOWN = 6,
		BUTTON_A = 7,
		BUTTON_AXIS0 = 32,
		BUTTON_AXIS1 = 33,
		BUTTON_AXIS2 = 34,
		BUTTON_AXIS3 = 35,
		BUTTON_AXIS4 = 36,
		BUTTON_STEAMVR_TOUCHPAD = 32,
		BUTTON_STEAMVR_TRIGGER = 33,
		BUTTON_DASHBOARD_BACK = 2,
		BUTTON_MAX = 64,
	};

	enum CONTROLLER_ROLE
	{
		CONTROLLER_ROLE_INVALID = 0,
		CONTROLLER_ROLE_LEFT_HAND = 1,
		CONTROLLER_ROLE_RIGHT_HAND = 2,
		CONTROLLER_ROLE_OPT_OUT = 3,
		CONTROLLER_ROLE_TREADMILL = 4,
		CONTROLLER_ROLE_MAX = 5,
	};
	virtual void setViewportMode(OpenVR::VIEWPORT mode) = 0;
	virtual OpenVR::VIEWPORT getViewportMode() const = 0;
	virtual void setHeadPositionLock(bool lock) = 0;
	virtual bool isHeadPositionLock() const = 0;
	virtual void setHeadRotationLock(bool lock) = 0;
	virtual bool isHeadRotationLock() const = 0;
	virtual int getMaxTrackedDeviceCount() const = 0;
	virtual int getControllerStateAxisCount() const = 0;
	virtual UNIGINE_DEPRECATED(Math::mat4 getDevicePose(int device_num)) = 0;
	virtual Math::mat4 getDeviceTransform(int device_num) = 0;
	virtual Math::vec3 getDeviceVelocity(int device_num) = 0;
	virtual Math::vec3 getDeviceAngularVelocity(int device_num) = 0;
	virtual bool isDeviceConnected(int device_num) = 0;
	virtual bool isPoseValid(int device_num) = 0;
	virtual int getTrackingResult(int device_num) = 0;
	virtual OpenVR::DEVICE getDeviceType(int device_num) = 0;
	virtual OpenVR::AXIS getDeviceAxisType(int device_num, int axis_num) = 0;
	virtual String getDeviceManufacturerName(int device) = 0;
	virtual String getDeviceModelNumber(int device) = 0;
	virtual OpenVR::CONTROLLER_ROLE getControllerRole(int device_num) = 0;
	virtual int getControllerPacketNum(int device_num) = 0;
	virtual bool getControllerButtonPressed(int device_num, OpenVR::BUTTON button) = 0;
	virtual bool getControllerButtonTouched(int device_num, OpenVR::BUTTON button) = 0;
	virtual Math::vec2 getControllerAxis(int device_num, int axis_num) = 0;
	virtual void setControllerVibration(int device_num, unsigned short duration) = 0;
	virtual void render(const Ptr<Player> &player, const Math::ivec2 &size, bool render_window) = 0;
};

} // namespace Plugins
} // namespace Unigine
