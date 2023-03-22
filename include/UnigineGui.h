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
#include "UnigineMathLib.h"
#include "UnigineCallback.h"
#include "UnigineInput.h"

namespace Unigine
{

class Widget;
class WidgetSprite;
class WidgetVBox;


class UNIGINE_API Gui : public APIInterface
{
public:

	enum CALLBACK_INDEX
	{
		SHOW = 0,
		HIDE,
		FOCUS_IN,
		FOCUS_OUT,
		CHANGED,
		CLICKED,
		DOUBLE_CLICKED,
		PRESSED,
		RELEASED,
		KEY_PRESSED,
		TEXT_PRESSED,
		ENTER,
		LEAVE,
		DRAG_MOVE,
		DRAG_DROP,
		REMOVE,
		NUM_CALLBACKS,
	};

	enum
	{
		VERTICAL = 0,
		HORIZONTAL,
	};

	enum
	{
		VALIDATOR_ANY = 0,
		VALIDATOR_INT,
		VALIDATOR_UINT,
		VALIDATOR_FLOAT,
		VALIDATOR_ASCII,
	};

	enum
	{
		ATTACH_LOG10 = 1 << 0,
		ATTACH_MIN_EXPAND = 1 << 1,
		ATTACH_MAX_EXPAND = 1 << 2,
	};

	enum
	{
		ALIGN_CENTER = 1 << 0,
		ALIGN_LEFT = 1 << 1,
		ALIGN_RIGHT = 1 << 2,
		ALIGN_TOP = 1 << 3,
		ALIGN_BOTTOM = 1 << 4,
		ALIGN_EXPAND = 1 << 5,
		ALIGN_OVERLAP = 1 << 6,
		ALIGN_BACKGROUND = 1 << 7,
		ALIGN_FIXED = 1 << 8,
	};

	enum
	{
		BUFFER_RED = 1 << 0,
		BUFFER_GREEN = 1 << 1,
		BUFFER_BLUE = 1 << 2,
		BUFFER_ALPHA = 1 << 3,
		BUFFER_ALL = (BUFFER_RED | BUFFER_GREEN | BUFFER_BLUE | BUFFER_ALPHA),
	};

	enum
	{
		BLEND_NONE = 0,
		BLEND_ZERO,
		BLEND_ONE,
		BLEND_SRC_COLOR,
		BLEND_ONE_MINUS_SRC_COLOR,
		BLEND_SRC_ALPHA,
		BLEND_ONE_MINUS_SRC_ALPHA,
		BLEND_DEST_COLOR,
		BLEND_ONE_MINUS_DEST_COLOR,
		BLEND_DEST_ALPHA,
		BLEND_ONE_MINUS_DEST_ALPHA,
	};

	enum
	{
		CURSOR_NONE = 0,
		CURSOR_ARROW,
		CURSOR_NA,
		CURSOR_DND,
		CURSOR_WIDTH,
		CURSOR_HEIGHT,
		CURSOR_NESW,
		CURSOR_NWSE,
		CURSOR_MOVE,
		CURSOR_ROTATE,
		CURSOR_SCALE,
		NUM_CURSORS,
	};

	enum
	{
		MOUSE_MASK_LEFT = 1 << 0,
		MOUSE_MASK_MIDDLE = 1 << 1,
		MOUSE_MASK_RIGHT = 1 << 2,
		MOUSE_MASK_DCLICK = 1 << 3,
		MOUSE_BUTTON_MASK = MOUSE_MASK_LEFT | MOUSE_MASK_MIDDLE | MOUSE_MASK_RIGHT,
	};
	static Ptr<Gui> getCurrent();
	static Ptr<Gui> create(const char *name = nullptr);
	void enable();
	void disable();
	void destroy();
	void update();
	void render(int custom_mouse_buttons);
	void render();
	void focusGained();
	void focusLost();
	bool isHover(int global_pos_x, int global_pos_y) const;
	Ptr<Widget> getWidgetIntersection(int global_pos_x, int global_pos_y);
	Ptr<Widget> getUnderCursorWidget();
	static Ptr<Gui> getFocusGui();
	static Ptr<Gui> getGuiIntersection(int global_pos_x, int global_pos_y);
	static Ptr<Gui> getUnderCursorGui();
	static void getWorldGuiInstances(Vector<Ptr<Gui>> &ret_instances);
	void setEnabled(bool enabled);
	bool isEnabled() const;
	void setHidden(bool hidden);
	bool isHidden() const;
	void setSize(const Math::ivec2 &size);
	Math::ivec2 getSize() const;
	void setPosition(const Math::ivec2 &position);
	Math::ivec2 getPosition() const;
	int getWidth() const;
	int getHeight() const;
	void setWinHandle(unsigned long long handle);
	unsigned long long getWinHandle() const;
	void setWorldObject(bool val);
	bool isWorldObject() const;
	void setColor(const Math::vec4 &color);
	Math::vec4 getColor() const;
	void setTransform(const Math::mat4 &transform);
	Math::mat4 getTransform() const;
	void setExposeSpeed(float speed);
	float getExposeSpeed() const;
	void setFadeInSpeed(float speed);
	float getFadeInSpeed() const;
	void setFadeOutSpeed(float speed);
	float getFadeOutSpeed() const;
	void setDefaultSize(int size);
	int getDefaultSize() const;
	void setDefaultColor(const Math::vec4 &color);
	Math::vec4 getDefaultColor() const;
	void setDefaultAlpha(float alpha);
	float getDefaultAlpha() const;
	void setFocusedEnabled(bool enabled);
	bool isFocusedEnabled() const;
	void setFocusedPermanent(bool permanent);
	bool isFocusedPermanent() const;
	void setFocusedColor(const Math::vec4 &color);
	Math::vec4 getFocusedColor() const;
	void setFocusedAlpha(float alpha);
	float getFocusedAlpha() const;
	void setDisabledEnabled(bool enabled);
	bool isDisabledEnabled() const;
	void setDisabledColor(const Math::vec4 &color);
	Math::vec4 getDisabledColor() const;
	void setDisabledAlpha(float alpha);
	float getDisabledAlpha() const;
	void setTransparentEnabled(bool enabled);
	bool isTransparentEnabled() const;
	void setTransparentColor(const Math::vec4 &color);
	Math::vec4 getTransparentColor() const;
	void setTransparentAlpha(float alpha);
	float getTransparentAlpha() const;
	void setToolTipEnabled(bool enabled);
	bool isToolTipEnabled() const;
	void setToolTipSize(int size);
	int getToolTipSize() const;
	void setToolTipWidth(int width);
	int getToolTipWidth() const;
	void setToolTipColor(const Math::vec4 &color);
	Math::vec4 getToolTipColor() const;
	void setToolTipAlpha(float alpha);
	float getToolTipAlpha() const;
	void setToolTipTime(float time);
	float getToolTipTime() const;
	void setToolTipText(const char *text);
	const char *getToolTipText() const;
	void setToolTipX(int tooltipx);
	int getToolTipX() const;
	void setToolTipY(int tooltipy);
	int getToolTipY() const;
	void setToolTip(int x, int y, const char *str);
	int getToolTipWidth(const char *str) const;
	int getToolTipHeight(const char *str) const;
	void setMouseEnabled(bool enabled);
	bool isMouseEnabled() const;
	void setMouseSprite(const Ptr<WidgetSprite> &sprite);
	Ptr<WidgetSprite> getMouseSprite() const;
	void setMouseCursor(int cursor);
	int getMouseCursor() const;
	void setMouseButtons(int buttons);
	int getMouseButtons() const;
	void setMouseGrab(int grab);
	int getMouseGrab() const;
	void setMouseShow(bool show);
	bool isMouseShow() const;
	int getMouseX() const;
	int getMouseY() const;
	int getMouseDX() const;
	int getMouseDY() const;
	int getMouseWheel() const;
	int getAndClearMouseWheel();
	void forceSetMouseWheel(int value);
	int getMouseWheelHorizontal() const;
	int getAndClearMouseWheelHorizontal();
	void forceSetMouseWheelHorizontal(int value);
	bool isUnderCursor() const;
	bool getKey(Input::KEY key);
	bool getAndClearKey(Input::KEY key);
	Ptr<Widget> getFocus() const;
	Ptr<Widget> getOverlappedFocus() const;
	Ptr<Widget> getPermanentFocus() const;
	Ptr<WidgetVBox> getVBox() const;
	void removeFocus();
	void addChild(const Ptr<Widget> &widget, int flags = -1);
	void removeChild(const Ptr<Widget> &widget);
	void replaceChild(const Ptr<Widget> &widget, const Ptr<Widget> &old_widget, int flags = -1);
	int isChild(const Ptr<Widget> &widget) const;
	int getNumChildren() const;
	Ptr<Widget> getChild(int num) const;
	bool isActive() const;
	int getKeyActivity(unsigned int key) const;
	int setFont(const char *name);
	int setSkin(const char *name);
	int setResource(const char *name);
	int clearTexture(const char *name);
	Math::vec4 parseColor(const char *str) const;
	void clearDictionaries();
	int addDictionary(const char *name, const char *language = 0);
	int saveDictionary(const char *name, const char *language = 0);
	int hasTranslation(const char *arg1) const;
	const char *translate(const char *str);
	static bool isRenderingBootScreen();
	static bool isRenderingSplashScreen();
	static bool isRenderingLoadingScreen();
	void *addUpdateCallback(CallbackBase *func);
	bool removeUpdateCallback(void *id);
	void clearUpdateCallbacks();
};
typedef Ptr<Gui> GuiPtr;

} // namespace Unigine
