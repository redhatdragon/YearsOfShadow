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

#include "UnigineGui.h"
#include "UnigineCamera.h"
#include "UnigineWidgets.h"
#include "UnigineCallback.h"

namespace Unigine
{

class WindowEvent;

class UNIGINE_API EngineWindow : public APIInterface
{
public:
	static Ptr<EngineWindow> create(const Math::ivec2 &size, int flags = 0);
	static Ptr<EngineWindow> create(int width, int height, int flags = 0);
	static Ptr<EngineWindow> create(const char *title, int width, int height, int flags = 0);

	enum FLAGS
	{
		FLAGS_MAIN = 1 << 0,
		FLAGS_CONSOLE_USAGE = 1 << 1,
		FLAGS_PROFILER_USAGE = 1 << 2,
		FLAGS_VISUALIZER_USAGE = 1 << 3,
		FLAGS_SHOWN = 1 << 4,
		FLAGS_FIXED_SIZE = 1 << 5,
		FLAGS_HOLD_ENGINE = 1 << 6,
	};

	enum HITTEST
	{
		HITTEST_INVALID = -1,
		HITTEST_NORMAL = 0,
		HITTEST_DRAGGABLE,
		HITTEST_RESIZE_TOPLEFT,
		HITTEST_RESIZE_TOP,
		HITTEST_RESIZE_TOPRIGHT,
		HITTEST_RESIZE_RIGHT,
		HITTEST_RESIZE_BOTTOMRIGHT,
		HITTEST_RESIZE_BOTTOM,
		HITTEST_RESIZE_BOTTOMLEFT,
		HITTEST_RESIZE_LEFT,
	};

	enum STATE
	{
		STATE_SEPARATE_WINDOW = 0,
		STATE_SEPARATE_GROUP,
		STATE_NESTED_WINDOW,
		STATE_NESTED_GROUP,
	};

	enum GROUP_TYPE
	{
		GROUP_TYPE_NONE = 0,
		GROUP_TYPE_TAB,
		GROUP_TYPE_HORIZONTAL,
		GROUP_TYPE_VERTICAL,
	};

	enum CALLBACK_INDEX
	{
		CALLBACK_WINDOW_EVENT = 0,
		CALLBACK_FUNC_UPDATE,
		CALLBACK_FUNC_BEGIN_RENDER,
		CALLBACK_FUNC_RENDER,
		CALLBACK_FUNC_BEGIN_RENDER_GUI,
		CALLBACK_FUNC_END_RENDER_GUI,
		CALLBACK_FUNC_END_RENDER,
		CALLBACK_FUNC_SWAP,
		NUM_CALLBACKS,
	};
	unsigned long long getID() const;
	void setMain(bool main);
	bool isMain() const;
	void setCamera(const Ptr<Camera> &camera);
	Ptr<Camera> getCamera() const;
	Ptr<Gui> getGui() const;
	int getDisplayIndex() const;
	const char *getVideoModeName() const;
	void screenshot(const char *path);
	bool isSeparateWindow() const;
	bool isSeparateGroup() const;
	bool isNestedWindow() const;
	bool isNestedGroup() const;
	bool isWindow() const;
	bool isGroup() const;
	bool isNested() const;
	bool isSeparate() const;
	EngineWindow::STATE getState() const;
	EngineWindow::GROUP_TYPE getGroupType() const;
	Ptr<Gui> getSelfGui() const;
	void setPosition(const Math::ivec2 &position);
	Math::ivec2 getPosition() const;
	Math::ivec2 getClientPosition() const;
	Math::ivec2 getClientLocalPosition() const;
	void moveToCenter();
	void setSize(const Math::ivec2 &size);
	Math::ivec2 getSize() const;
	Math::ivec2 getClientSize() const;
	void setMinSize(const Math::ivec2 &size);
	Math::ivec2 getMinSize() const;
	void setMaxSize(const Math::ivec2 &size);
	Math::ivec2 getMaxSize() const;
	void setTitle(const char *title);
	const char *getTitle() const;
	bool setIcon(const Ptr<Image> &icon);
	Ptr<Image> getIcon();
	void setOpacity(float opacity);
	float getOpacity() const;
	void setBordersEnabled(bool enabled);
	bool isBordersEnabled() const;
	void setBorderSize(int size);
	int getBorderSize() const;
	void setHoldEngine(bool engine);
	bool isHoldEngine() const;
	void setIgnoreSystemClose(bool close);
	bool isIgnoreSystemClose() const;
	void setConsoleUsage(bool usage);
	bool isConsoleUsage() const;
	void setProfilerUsage(bool usage);
	bool isProfilerUsage() const;
	void setVisualizerUsage(bool usage);
	bool isVisualizerUsage() const;
	void setSkipRenderEngine(bool engine);
	bool isSkipRenderEngine() const;
	void setResizable(bool resizable);
	bool isResizable() const;
	void setResizeBorderSize(int size);
	int getResizeBorderSize() const;
	bool isFullscreen() const;
	void disableFullscreen();
	bool enableFullscreen(int display = -1, int mode = -1);
	void show();
	bool isShown() const;
	void hide();
	bool isHidden() const;
	void setFocus();
	bool isFocused() const;
	void minimize();
	bool isMinimized() const;
	void maximize();
	bool isMaximized() const;
	void restore();
	void setMouseGrab(bool value);
	EngineWindow::HITTEST getHitTestResult() const;
	void setModal(const Ptr<EngineWindow> &parent_window);
	bool isModal() const;
	bool isModalParent() const;
	Ptr<EngineWindow> getModalParent() const;
	void addModalWindow(const Ptr<EngineWindow> &window);
	void removeModalWindow(const Ptr<EngineWindow> &window);
	int getNumModalWindows() const;
	Ptr<EngineWindow> getModalWindow(int index) const;
	int getOrder() const;
	void toTop();
	void setAlwaysOnTop(bool top);
	bool isAlwaysOnTop() const;
	void addChild(const Ptr<Widget> &widget, int flags = -1);
	void removeChild(const Ptr<Widget> &widget);
	bool isChild(const Ptr<Widget> &widget) const;
	Ptr<Widget> getChild(int index) const;
	int getNumChildren() const;
	void arrange();
	void expand();
	void setGroupUsage(bool usage);
	bool isGroupUsage() const;
	void setNestedUsage(bool usage);
	bool isNestedUsage() const;
	void unstack();
	int getNumNestedWindows() const;
	Ptr<EngineWindow> getNestedWindow(int index) const;
	int getNestedWindowIndex(const Ptr<EngineWindow> &window);
	bool containsNestedWindow(const Ptr<EngineWindow> &window) const;
	bool containsNestedWindowGlobal(const Ptr<EngineWindow> &window) const;
	Ptr<EngineWindow> getParentGroup() const;
	Ptr<EngineWindow> getGlobalParentGroup() const;
	bool isGlobalChildOf(const Ptr<EngineWindow> &window);
	int getCurrentTab() const;
	int getTabWidth(int index) const;
	int getTabHeight(int index) const;
	int getTabBarWidth(int index) const;
	int getTabBarHeight(int index) const;
	Math::ivec2 getTabLocalPosition(int index) const;
	Math::ivec2 getTabBarLocalPosition(int index) const;
	void setHorizontalTabWidth(int index, int width);
	void setVerticalTabHeight(int index, int height);
	void setSeparatorPosition(int index, int pos);
	int getSeparatorPosition(int index) const;
	void setSeparatorValue(int index, float value);
	float getSeparatorValue(int index) const;
	void swapTabs(int first, int second);
	bool isHover(const Math::ivec2 &global_mouse_pos) const;
	bool isClientHover(const Math::ivec2 &global_mouse_pos) const;
	int getHoverTabBar(const Math::ivec2 &global_mouse_pos, Math::ivec2 &out_bar_pos, Math::ivec2 &out_bar_size) const;
	int getHoverTabBarArea(const Math::ivec2 &global_mouse_pos, Math::ivec2 &out_area_pos, Math::ivec2 &out_area_size) const;
	void *addCallback(EngineWindow::CALLBACK_INDEX callback, CallbackBase *func);
	void *addCallback(EngineWindow::CALLBACK_INDEX callback, CallbackBase1<float> *func);
	void *addCallback(EngineWindow::CALLBACK_INDEX callback, CallbackBase1<Ptr<WindowEvent>> *func);
	bool removeCallback(EngineWindow::CALLBACK_INDEX callback, void *id);
	void clearCallbacks(EngineWindow::CALLBACK_INDEX callback);
	int getNumDroppedItems() const;
	const char *getDroppedItem(int index);
	void *addCustomRenderCallback(CallbackBase1<Ptr<EngineWindow>> *func);
	bool removeCustomRenderCallback(void *id);
	void clearCustomRenderCallbacks();
	void setDeprecatedCustomPresentCallback(CallbackBase *callback);
	CallbackBase *getDeprecatedCustomPresentCallback() const;
};
typedef Ptr<EngineWindow> EngineWindowPtr;

class WindowEventGeneric;
class WindowEventDrop;

class UNIGINE_API WindowEvent : public APIInterface
{
public:
	static bool convertible(WindowEvent *obj) { return obj != nullptr; }

	enum TYPE
	{
		WINDOW_EVENT = 0,
		WINDOW_EVENT_GENERIC,
		WINDOW_EVENT_DROP,
		NUM_WINDOW_EVENTS,
	};
	WindowEvent::TYPE getType() const;
	const char *getTypeName() const;
	void setTimestamp(unsigned long long timestamp);
	unsigned long long getTimestamp() const;
	void setWinID(unsigned long long winid);
	unsigned long long getWinID() const;
	void setMousePosition(const Math::ivec2 &position);
	Math::ivec2 getMousePosition() const;
	void setPosition(const Math::ivec2 &position);
	Math::ivec2 getPosition() const;
	void setSize(const Math::ivec2 &size);
	Math::ivec2 getSize() const;
	void setWindow(const Ptr<EngineWindow> &window);
	Ptr<EngineWindow> getWindow() const;
	Ptr<WindowEventGeneric> getGeneric() const;
	Ptr<WindowEventDrop> getDrop() const;
};
typedef Ptr<WindowEvent> WindowEventPtr;


class UNIGINE_API WindowEventGeneric : public WindowEvent
{
public:
	static bool convertible(WindowEvent *obj) { return obj && obj->getType() == WindowEvent::WINDOW_EVENT_GENERIC; }

	enum ACTION
	{
		ACTION_RESIZED = 0,
		ACTION_MOVED,
		ACTION_SIZE_CHANGED,
		ACTION_FOCUS_GAINED,
		ACTION_FOCUS_LOST,
		ACTION_MOUSE_ENTER,
		ACTION_MOUSE_LEAVE,
		ACTION_SHOWN,
		ACTION_HIDDEN,
		ACTION_MINIMIZED,
		ACTION_MAXIMIZED,
		ACTION_RESTORED,
		ACTION_CLOSE,
		ACTION_UNSTACK_MOVE,
	};
	static Ptr<WindowEventGeneric> create();
	static Ptr<WindowEventGeneric> create(unsigned long long timestamp, unsigned long long win_id);
	static Ptr<WindowEventGeneric> create(unsigned long long timestamp, unsigned long long win_id, const Math::ivec2 &mouse_pos);
	static Ptr<WindowEventGeneric> create(unsigned long long timestamp, unsigned long long win_id, const Math::ivec2 &mouse_pos, const Math::ivec2 &position, const Math::ivec2 &size);
	static Ptr<WindowEventGeneric> create(unsigned long long timestamp, unsigned long long win_id, const Math::ivec2 &mouse_pos, const Math::ivec2 &position, const Math::ivec2 &size, WindowEventGeneric::ACTION action);
	void setAction(WindowEventGeneric::ACTION action);
	WindowEventGeneric::ACTION getAction() const;
};
typedef Ptr<WindowEventGeneric> WindowEventGenericPtr;


class UNIGINE_API WindowEventDrop : public WindowEvent
{
public:
	static bool convertible(WindowEvent *obj) { return obj && obj->getType() == WindowEvent::WINDOW_EVENT_DROP; }

	enum ACTION
	{
		ACTION_ITEMS_DROP_BEGIN = 0,
		ACTION_ITEM_DROP,
		ACTION_ITEMS_DROP_END,
	};
	static Ptr<WindowEventDrop> create();
	static Ptr<WindowEventDrop> create(unsigned long long timestamp, unsigned long long win_id);
	static Ptr<WindowEventDrop> create(unsigned long long timestamp, unsigned long long win_id, const Math::ivec2 &mouse_pos);
	static Ptr<WindowEventDrop> create(unsigned long long timestamp, unsigned long long win_id, const Math::ivec2 &mouse_pos, const Math::ivec2 &position, const Math::ivec2 &size);
	static Ptr<WindowEventDrop> create(unsigned long long timestamp, unsigned long long win_id, const Math::ivec2 &mouse_pos, const Math::ivec2 &position, const Math::ivec2 &size, WindowEventDrop::ACTION action, const char *item_path);
	void setAction(WindowEventDrop::ACTION action);
	WindowEventDrop::ACTION getAction() const;
	void setPath(const char *path);
	const char *getPath() const;
};
typedef Ptr<WindowEventDrop> WindowEventDropPtr;

int windowEventsFilterFunc(Ptr<WindowEvent> &e);

class UNIGINE_API SystemDialog : public APIInterface
{
public:
	static Ptr<SystemDialog> create();

	enum
	{
		TYPE_ERROR = 0,
		TYPE_WARNING,
		TYPE_INFORMATION,
	};
	void setType(int type);
	int getType() const;
	void setTitle(const char *title);
	const char *getTitle() const;
	void setMessage(const char *message);
	const char *getMessage() const;
	void setDefaultButtonReturn(int val);
	int getDefaultButtonReturn() const;
	void setDefaultButtonEscape(int escape);
	int getDefaultButtonEscape() const;
	int getNumButtons() const;
	int addButton();
	int addButton(const char *name);
	void removeButton(int num);
	void swapButtons(int num_0, int num_1);
	const char *getButtonName(int num) const;
	void setButtonName(int num, const char *name);
};
typedef Ptr<SystemDialog> SystemDialogPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API WindowManager
{
protected:
	

public:
	static int isInitialized(); 

	enum CALLBACKS
	{
		CALLBACKS_WINDOW_CREATED = 0,
		CALLBACKS_WINDOW_REMOVED,
		CALLBACKS_WINDOWS_STACKED,
		CALLBACKS_WINDOW_UNSTACKED,
		CALLBACKS_NUM,
	};
	static bool isMultipleWindowsSupported();
	static bool isFullscreenMode();
	static Ptr<EngineWindow> getFullscreenWindow();
	static Ptr<EngineWindow> getMainWindow();
	static Ptr<EngineWindow> getUnderCursorWindow();
	static Ptr<EngineWindow> getFocusedWindow();
	static Ptr<EngineWindow> getGuiFocusedWindow();
	static int getNumWindows();
	static Ptr<EngineWindow> getWindow(int index);
	static int getWindowIndex(const Ptr<EngineWindow> &window);
	static Ptr<EngineWindow> getWindowByID(unsigned long long win_id);
	static Ptr<EngineWindow> stack(const Ptr<EngineWindow> &first, const Ptr<EngineWindow> &second, int index = -1, EngineWindow::GROUP_TYPE group_type = EngineWindow::GROUP_TYPE_TAB, bool decompose_second = false);
	static Ptr<EngineWindow> stackToParentGroup(const Ptr<EngineWindow> &destination, const Ptr<EngineWindow> &window, int index = -1, bool decompose_second = false);
	static Ptr<EngineWindow> stackWindows(const Ptr<EngineWindow> &first, const Ptr<EngineWindow> &second, EngineWindow::GROUP_TYPE group_type = EngineWindow::GROUP_TYPE_TAB);
	static Ptr<EngineWindow> stackGroups(const Ptr<EngineWindow> &first, const Ptr<EngineWindow> &second, EngineWindow::GROUP_TYPE group_type = EngineWindow::GROUP_TYPE_TAB, bool decompose_second = false);
	static Ptr<EngineWindow> stackToWindow(const Ptr<EngineWindow> &destination_window, const Ptr<EngineWindow> &window, EngineWindow::GROUP_TYPE group_type = EngineWindow::GROUP_TYPE_TAB, bool decompose_second = false);
	static Ptr<EngineWindow> stackToGroup(const Ptr<EngineWindow> &destination_group, const Ptr<EngineWindow> &group, int index = -1, bool decompose_second = false);
	static Ptr<EngineWindow> getIntersection(const Math::ivec2 &global_pos, const Vector<Ptr<EngineWindow>> &excludes);
	static Ptr<EngineWindow> getIntersection(const Math::ivec2 &global_pos);
	static void unstack(const Ptr<EngineWindow> &unstacked);
	static void forceUpdateWindowOrders();
	static bool dialogMessage(const char *title, const char *message);
	static bool dialogWarning(const char *title, const char *warning);
	static bool dialogError(const char *title, const char *error);
	static int showSystemDialog(const Ptr<SystemDialog> &dialog);
	static const char *dialogOpenFolder(const char *path);
	static const char *dialogOpenFolder();
	static Vector<String> dialogOpenFiles(const char *path, const char *filter);
	static Vector<String> dialogOpenFiles(const char *path);
	static Vector<String> dialogOpenFiles();
	static const char *dialogOpenFile(const char *path, const char *filter);
	static const char *dialogOpenFile(const char *path);
	static const char *dialogOpenFile();
	static const char *dialogSaveFile(const char *path, const char *filter);
	static const char *dialogSaveFile(const char *path);
	static const char *dialogSaveFile();
	static void *addCallback(WindowManager::CALLBACKS callback, CallbackBase *func);
	static void *addCallback(WindowManager::CALLBACKS callback, CallbackBase1<Ptr<EngineWindow>> *func);
	static void *addCallback(WindowManager::CALLBACKS callback, CallbackBase2<Ptr<EngineWindow>, Ptr<EngineWindow>> *func);
	static bool removeCallback(WindowManager::CALLBACKS callback, void *id);
	static void clearCallbacks(WindowManager::CALLBACKS callback);
	static void setEventsFilter(int (*func)(Ptr<WindowEvent> &e));
};

} // namespace Unigine
