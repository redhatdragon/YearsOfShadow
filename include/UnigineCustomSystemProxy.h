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


#pragma once

#include "UnigineBase.h"
#include "UnigineMathLib.h"
#include "UniginePtr.h"
#include "UnigineImage.h"
#include "UnigineWindowManager.h"
#include "UnigineInput.h"

namespace Unigine
{

union WIN_HANDLE
{
	uint64_t win_id;
	#ifdef _WIN32
		void *hwnd;
	#elif _LINUX
		unsigned long system_win;
	#endif

	WIN_HANDLE()
	{
		win_id = 0;
		#ifdef _WIN32
			hwnd = nullptr;
		#elif _LINUX
			system_win = 0;
		#endif
	}
};

static_assert(sizeof(WIN_HANDLE) == sizeof(uint64_t), "struct size must remain unchanged");

inline bool operator==(const WIN_HANDLE &m0, const WIN_HANDLE &m1)
{
	return m0.win_id == m1.win_id;
}

template<typename Type>
struct Hasher;

template<>
struct Hasher<WIN_HANDLE>
{
	using HashType = uint64_t;
	UNIGINE_INLINE static HashType create(const WIN_HANDLE &v) { return v.win_id; }
};

// friend
class SystemProxyWrapper;

class UNIGINE_API CustomSystemProxy
{
public:
	CustomSystemProxy(int features);
	virtual ~CustomSystemProxy();

	static CustomSystemProxy *get() { return instance; }

	enum
	{
		SYSTEM_PROXY_WINDOWS = 1 << 0,
		SYSTEM_PROXY_MOUSE = 1 << 1,
		SYSTEM_PROXY_KEYBOARD = 1 << 2,
		SYSTEM_PROXY_TOUCHES = 1 << 3,
		SYSTEM_PROXY_DISPLAYS = 1 << 4,
		SYSTEM_PROXY_JOYSTICKS = 1 << 5,
		SYSTEM_PROXY_GAMEPADS = 1 << 6,
	};

	int getFeatures() const;

	bool isWindowsSupported() const;
	bool isMouseSupported() const;
	bool isKeyboardSupported() const;
	bool isTouchesSupported() const;
	bool isDisplaysSupported() const;
	bool isJoysticksSupported() const;
	bool isGamepadsSupported() const;

	bool isFocus();
	void focusGained();
	void focusLost();

	void updateWindowOrders();
	int getWindowOrder(WIN_HANDLE win_handle);
	void windowToTop(WIN_HANDLE win_handle);

	// external windows
	bool initExternalWindowBuffers(WIN_HANDLE win_handle, const Math::ivec2 &size);
	bool resizeExternalWindowBuffers(WIN_HANDLE win_handle, const Math::ivec2 &size);
	bool shutdownExternalWindowBuffers(WIN_HANDLE win_handle);

	virtual bool needRenderExternalWindow(WIN_HANDLE win_handle) { return true; }
	virtual void onExternalWindowRender(WIN_HANDLE win_handle) {}

protected:

	bool isDragAreaIntersection(uint64_t win_handle, int global_pos_x, int global_pos_y);

	// invokers
	void invokeWindowEvent(WindowEventPtr e);
	void invokeInputEvent(InputEventPtr e);

	/////////////////////////////////////////
	// proxy functions
	/////////////////////////////////////////

	// main thread
	virtual void mainUpdate() = 0;

	// windows
	virtual WIN_HANDLE createWindow(int width, int height) = 0;
	virtual void removeWindow(WIN_HANDLE win_handle) = 0;

	virtual void setWindowTitle(WIN_HANDLE win_handle, const char *title) = 0;
	virtual void setWindowIcon(WIN_HANDLE win_handle, const ImagePtr &image) = 0;

	virtual void setWindowSize(WIN_HANDLE win_handle, const Math::ivec2 &size) = 0;
	virtual void setWindowMinSize(WIN_HANDLE win_handle, const Math::ivec2 &size) = 0;
	virtual void setWindowMaxSize(WIN_HANDLE win_handle, const Math::ivec2 &size) = 0;

	virtual void setWindowPosition(WIN_HANDLE win_handle, const Math::ivec2 &pos) = 0;

	virtual void showWindow(WIN_HANDLE win_handle) = 0;
	virtual void hideWindow(WIN_HANDLE win_handle) = 0;

	virtual void setWindowFocus(WIN_HANDLE win_handle) = 0;

	virtual void setWindowMouseGrab(WIN_HANDLE win_handle, bool state) = 0;

	virtual void setWindowResizable(WIN_HANDLE win_handle, bool state) = 0;
	virtual void setWindowBordered(WIN_HANDLE win_handle, bool state) = 0;

	virtual void disableWindowFullscreen(WIN_HANDLE win_handle) = 0;
	virtual bool enableWindowFullscreen(WIN_HANDLE win_handle, int display, int mode) = 0;

	virtual void minimizeWindow(WIN_HANDLE win_handle) = 0;
	virtual void maximizeWindow(WIN_HANDLE win_handle) = 0;
	virtual void restoreWindow(WIN_HANDLE win_handle) = 0;

	virtual void setWindowOpacity(WIN_HANDLE win_handle, float opacity) = 0;

	virtual int getWindowDisplayIndex(WIN_HANDLE win_handle) const = 0;

	virtual EngineWindow::HITTEST getWindowHitTestResult(WIN_HANDLE win_handle) = 0;

	virtual Math::ivec4 getWindowSystemBorderSize(WIN_HANDLE win_handle) = 0;
	virtual void setWindowResizeBorderSize(WIN_HANDLE win_handle, int size) = 0;

	virtual void setWindowModal(WIN_HANDLE win_handle, WIN_HANDLE parent_handle) = 0;

	virtual void setWindowAlwaysOnTop(WIN_HANDLE win_handle, bool state) = 0;

	virtual WIN_HANDLE getWindowIntersection(const Math::ivec2 &global_pos, const Vector<WIN_HANDLE> &excludes) = 0;

	virtual WIN_HANDLE getDraggableWindow() const = 0;
	virtual WIN_HANDLE getResizableWindow() const = 0;

	// mouse
	virtual void setGlobalMousePosition(const Math::ivec2 &pos) = 0;
	virtual Math::ivec2 getGlobalMousePosition() const = 0;

	virtual void showCursor(bool state) = 0;

	virtual void showMouseCursorSystemArrow() = 0;

	virtual void setMouseCursorSkinCustom(const ImagePtr &mouse_skin) = 0;
	virtual void setMouseCursorSkinSystem() = 0;

	virtual void setMouseCursorCustom(const ImagePtr &image, int x, int y) = 0;
	virtual void clearMouseCursorCustom() = 0;

	virtual void changeMouseCursorSkinNumber(int number) = 0;

	// keyboard
	virtual bool isKeyboardModifierEnabled(int modifier) const = 0;

	virtual unsigned int keyToUnicode(unsigned int key) const = 0;
	virtual unsigned int unicodeToKey(unsigned int unicode) const = 0;

	// touches

	// displays
	virtual int getDisplayDefaultSystemDPI() const = 0;
	virtual int getNumDisplays() const = 0;
	virtual Math::ivec2 getDisplayPosition(int display_index) const = 0;
	virtual Math::ivec2 getDisplayResolution(int display_index) const = 0;
	virtual int getDisplayRefreshRate(int display_index) const = 0;
	virtual int getDisplayCurrentMode(int display_index) const = 0;
	virtual int getDisplayDesktopMode(int display_index) const = 0;
	virtual int getDisplayOrientation(int display_index) const = 0;
	virtual int getDisplayDPI(int display_index) const = 0;

	virtual int getMainDisplay() const = 0;

	virtual int getDisplayNumModes(int display_index) const = 0;
	virtual Math::ivec2 getDisplayModeResolution(int display_index, int mode_index) const = 0;
	virtual int getDisplayModeRefreshRate(int display_index, int mode_index) const = 0;

	virtual const char *getDisplayName(int display_index) const = 0;

	// joysticks
	virtual void getConnectedJoysticks(Unigine::Vector<int32_t> &connected_ids) = 0;
	virtual int getJoystickPlayerIndex(int32_t joy_id) const = 0;
	virtual int getJoystickDeviceType(int32_t joy_id) const = 0;
	virtual int getJoystickVendor(int32_t joy_id) const = 0;
	virtual int getJoystickProduct(int32_t joy_id) const = 0;
	virtual int getJoystickProductVersion(int32_t joy_id) const = 0;
	virtual const char *getJoystickName(int32_t joy_id) const = 0;
	virtual const char *getJoystickModelGUID(int32_t joy_id) const = 0;
	virtual int getJoystickNumButtons(int32_t joy_id) const = 0;
	virtual int getJoystickNumAxes(int32_t joy_id) const = 0;
	virtual int getJoystickNumPovs(int32_t joy_id) const = 0;
	virtual float getJoystickAxisInitValue(int32_t joy_id, int axis) const = 0;
	virtual const char *getJoystickButtonName(int32_t joy_id, int button) = 0;
	virtual const char *getJoystickAxisName(int32_t joy_id, int axis) = 0;
	virtual const char *getJoystickPovName(int32_t joy_id, int pov) = 0;

	// gamepads
	virtual void getConnectedGamepads(Unigine::Vector<int32_t> &connected_ids) = 0;
	virtual int getGamepadPlayerIndex(int32_t pad_id) const = 0;
	virtual int getGamepadDeviceType(int32_t pad_id) const = 0;
	virtual int getGamepadModelType(int32_t pad_id) const = 0;
	virtual const char *getGamepadName(int32_t pad_id) const = 0;
	virtual const char *getGamepadModelGUID(int32_t pad_id) const = 0;
	virtual void setGamepadVibration(int32_t pad_id, float low_frequency, float high_frequency, float duration_ms) = 0;

	// other
	virtual bool hasClipboardText() const = 0;
	virtual void setClipboardText(const char *str) = 0;
	virtual const char *getClipboardText() = 0;

	virtual bool showDialogMessage(const char *title, const char *message, WIN_HANDLE parent_window_handle) = 0;
	virtual bool showDialogWarning(const char *title, const char *warning, WIN_HANDLE parent_window_handle) = 0;
	virtual bool showDialogError(const char *title, const char *error, WIN_HANDLE parent_window_handle) = 0;

private:
	friend SystemProxyWrapper;

	static CustomSystemProxy *instance;
};

} // namespace Unigine
