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

#include "UnigineBase.h"
#include "UnigineMathLib.h"
#include "UnigineString.h"

namespace Unigine
{

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Config
{
protected:
	

public:
	static int isInitialized(); 
	static void setPath(const char *path);
	static const char *getPath();
	static void setAutosave(bool autosave);
	static bool isAutosave();
	static bool load();
	static bool save();
	static bool isExist(const char *name);
	static void remove(const char *name);
	static void setBool(const char *name, bool value);
	static bool getBool(const char *name, bool value);
	static bool getBool(const char *name);
	static void setInt(const char *name, int value);
	static int getInt(const char *name, int value);
	static int getInt(const char *name);
	static void setFloat(const char *name, float value);
	static float getFloat(const char *name, float value);
	static float getFloat(const char *name);
	static void setString(const char *name, const char *value);
	static const char *getString(const char *name, const char *value);
	static const char *getString(const char *name);
};

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API BootConfig
{
protected:
	

public:
	static int isInitialized(); 
	static void setPath(const char *path);
	static const char *getPath();
	static void setAutosave(bool autosave);
	static bool isAutosave();
	static bool load();
	static bool save();
	static void reset();
	static void setGuiPath(const char *path);
	static const char *getGuiPath();
	static void setEngineLog(const char *log);
	static const char *getEngineLog();
	static void setSystemScript(const char *script);
	static const char *getSystemScript();
	static void setEditorScript(const char *script);
	static const char *getEditorScript();
	static void setCachePath(const char *path);
	static const char *getCachePath();
	static void setEditorCache(const char *cache);
	static const char *getEditorCache();
	static void setSystemCache(const char *cache);
	static const char *getSystemCache();
	static void setVideoApp(const char *app);
	static const char *getVideoApp();
	static void setSoundApp(const char *app);
	static const char *getSoundApp();
	static void setExternDefine(const char *define);
	static const char *getExternDefine();
	static void setConsoleCommand(const char *command);
	static const char *getConsoleCommand();
	static void setWindowTitle(const char *title);
	static const char *getWindowTitle();
	static void setWindowIconPath(const char *path);
	static const char *getWindowIconPath();
	static int getNumPluginPaths();
	static void removePluginPath(int num);
	static void swapPluginPath(int num0, int num1);
	static const char *getPluginPath(int num);
	static void setPluginPath(int num, const char *value);
	static int getNumExternPlugins();
	static void removeExternPlugin(int num);
	static void swapExternPlugin(int num0, int num1);
	static const char *getExternPlugin(int num);
	static void setExternPlugin(int num, const char *value);
	static void setScreenEnabled(bool enabled);
	static bool isScreenEnabled();
	static void setScreenWidth(int width);
	static int getScreenWidth();
	static void setScreenHeight(int height);
	static int getScreenHeight();
	static void setScreenBackgroundColor(const Math::vec4 &color);
	static Math::vec4 getScreenBackgroundColor();
	static void setScreenTransform(const Math::vec4 &transform);
	static Math::vec4 getScreenTransform();
	static void setScreenThreshold(int threshold);
	static int getScreenThreshold();
	static void setScreenTexture(const char *texture);
	static const char *getScreenTexture();
	static void setScreenFont(const char *font);
	static const char *getScreenFont();
	static void setScreenText(const char *text);
	static const char *getScreenText();
	static void setScreenMessageEngineInit(const char *init);
	static const char *getScreenMessageEngineInit();
	static void setScreenMessageFileSystemInit(const char *init);
	static const char *getScreenMessageFileSystemInit();
	static void setScreenMessageMaterialsInit(const char *init);
	static const char *getScreenMessageMaterialsInit();
	static void setScreenMessagePropertiesInit(const char *init);
	static const char *getScreenMessagePropertiesInit();
	static void setScreenMessageShadersCompilation(const char *compilation);
	static const char *getScreenMessageShadersCompilation();
};

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API UserConfig
{
protected:
	

public:
	static int isInitialized(); 
	static void setPath(const char *path);
	static const char *getPath();
	static void setAutosave(bool autosave);
	static bool isAutosave();
	static bool load();
	static bool save();
	static void reset();
};

} // namespace Unigine
