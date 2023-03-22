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

#include "UniginePtr.h"
#include "UnigineStreams.h"
#include "UnigineInput.h"

namespace Unigine
{

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ControlsApp
{
protected:
	

public:
	static int isInitialized(); 
	static void setPath(const char *path);
	static const char *getPath();
	static void setAutosave(bool autosave);
	static bool isAutosave();
	static int load();
	static int save();
	static void setEnabled(bool enabled);
	static bool isEnabled();
	static void setAlwaysRun(int run);
	static int getAlwaysRun();
	static void setMouseHandle(Input::MOUSE_HANDLE handle);
	static Input::MOUSE_HANDLE getMouseHandle();
	static void setMouseEnabled(bool enabled);
	static bool isMouseEnabled();
	static void setMouseInverse(bool inverse);
	static bool isMouseInverse();
	static void setMouseRawInput(bool input);
	static bool isMouseRawInput();
	static void setMouseSensitivity(float sensitivity);
	static float getMouseSensitivity();
	static void setMouseDX(float mousedx);
	static float getMouseDX();
	static void setMouseDY(float mousedy);
	static float getMouseDY();
	static void setState(int state, int value);
	static int getState(int state);
	static int clearState(int state);
	static void setRemoveGrabKey(Input::KEY key);
	static Input::KEY getRemoveGrabKey();
	static int isStateKey(Input::KEY key);
	static void setStateKey(int state, Input::KEY key);
	static Input::KEY getStateKey(int state);
	static int isStateMouseButton(Input::MOUSE_BUTTON button);
	static void setStateMouseButton(int state, Input::MOUSE_BUTTON button);
	static Input::MOUSE_BUTTON getStateMouseButton(int state);
	static String getStateName(int state);
	static String getStateInfo(int state);
	static void getStateEvent(int state);
	static int isStateEvent();
};


class UNIGINE_API Controls : public APIInterface
{
public:
	static bool convertible(Controls *obj) { return obj != nullptr; }

	enum TYPE
	{
		CONTROLS_APP,
		CONTROLS_DUMMY,
		CONTROLS_GAMEPAD,
		CONTROLS_JOYSTICK,
	};

	enum
	{
		STATE_FORWARD = 0,
		STATE_BACKWARD,
		STATE_MOVE_LEFT,
		STATE_MOVE_RIGHT,
		STATE_TURN_UP,
		STATE_TURN_DOWN,
		STATE_TURN_LEFT,
		STATE_TURN_RIGHT,
		STATE_CROUCH,
		STATE_JUMP,
		STATE_RUN,
		STATE_USE,
		STATE_FIRE,
		STATE_SAVE,
		STATE_RESTORE,
		STATE_SCREENSHOT,
		STATE_AUX_0,
		STATE_AUX_1,
		STATE_AUX_2,
		STATE_AUX_3,
		STATE_AUX_4,
		STATE_AUX_5,
		STATE_AUX_6,
		STATE_AUX_7,
		STATE_AUX_8,
		STATE_AUX_9,
		STATE_AUX_A,
		STATE_AUX_B,
		STATE_AUX_C,
		STATE_AUX_D,
		STATE_AUX_E,
		STATE_AUX_F,
		NUM_STATES,
	};
	static Ptr<Controls> create(int type);
	Controls::TYPE getType() const;
	const char *getTypeName() const;
	void setMouseDX(float mousedx);
	float getMouseDX() const;
	void setMouseDY(float mousedy);
	float getMouseDY() const;
	void setState(int state, int value);
	int getState(int state) const;
	int clearState(int state);
	int saveState(const Ptr<Stream> &stream) const;
	int restoreState(const Ptr<Stream> &stream);
	const char *getStateName(int state) const;
	int getStateByName(const char *name) const;
};
typedef Ptr<Controls> ControlsPtr;


class UNIGINE_API ControlsDummy : public Controls
{
public:
	static bool convertible(Controls *obj) { return obj && obj->getType() == Controls::CONTROLS_DUMMY; }
	static Ptr<ControlsDummy> create();
};
typedef Ptr<ControlsDummy> ControlsDummyPtr;


class UNIGINE_API ControlsJoystick : public Controls
{
public:
	static bool convertible(Controls *obj) { return obj && obj->getType() == Controls::CONTROLS_JOYSTICK; }

	enum POV
	{
		POV_NOT_PRESSED = -1,
		POV_UP = 0,
		POV_UP_RIGHT = 4500,
		POV_RIGHT = 9000,
		POV_DOWN_RIGHT = 13500,
		POV_DOWN = 18000,
		POV_DOWN_LEFT = 22500,
		POV_LEFT = 27000,
		POV_UP_LEFT = 31500,
	};
	static Ptr<ControlsJoystick> create(int num);
	int getNumber() const;
	int getPlayerIndex() const;
	bool isAvailable() const;
	const char *getName() const;
	Input::DEVICE_TYPE getDeviceType() const;
	void setFilter(float filter);
	float getFilter() const;
	int updateEvents();
	int getNumAxes() const;
	float getAxis(int axis) const;
	float getAxisInitialValue(int axis) const;
	const char *getAxisName(int axis) const;
	int getNumPovs() const;
	ControlsJoystick::POV getPov(int pov) const;
	const char *getPovName(int pov) const;
	int getNumButtons() const;
	int getButton(int button) const;
	int clearButton(int button);
	const char *getButtonName(int button) const;
	const char *getGuid() const;
	int getVendor() const;
	int getProduct() const;
	int getProductVersion() const;
	void setStateButton(int state, int button);
	int getStateButton(int state) const;
	String getStateName(int state) const;
	int saveState(const Ptr<Stream> &stream) const;
	int restoreState(const Ptr<Stream> &stream);
	void getStateEvent(int state);
	int isStateEvent() const;
};
typedef Ptr<ControlsJoystick> ControlsJoystickPtr;


class UNIGINE_API ControlsGamepad : public Controls
{
public:
	static bool convertible(Controls *obj) { return obj && obj->getType() == Controls::CONTROLS_GAMEPAD; }
	static Ptr<ControlsGamepad> create(int num);
	int getNumber() const;
	int getPlayerIndex() const;
	bool isAvailable() const;
	const char *getName() const;
	Input::DEVICE_TYPE getDeviceType() const;
	InputGamePad::MODEL_TYPE getModelType() const;
	void setFilter(float filter);
	float getFilter() const;
	int updateEvents();
	float getLeftX() const;
	float getLeftY() const;
	float getRightX() const;
	float getRightY() const;
	float getLeftTrigger() const;
	float getRightTrigger() const;
	void setVibration(float low_frequency, float high_frequency, float duration_ms = 100.0f);
	int getButton(InputGamePad::BUTTON button) const;
	int clearButton(InputGamePad::BUTTON button);
	void setStateButton(int state, int button);
	int getStateButton(int state) const;
	String getStateName(int state) const;
	int saveState(const Ptr<Stream> &stream) const;
	int restoreState(const Ptr<Stream> &stream);
	void getStateEvent(int state);
	bool isStateEvent() const;
};
typedef Ptr<ControlsGamepad> ControlsGamepadPtr;

} // namespace Unigine
