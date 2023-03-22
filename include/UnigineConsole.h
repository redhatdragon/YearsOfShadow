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
#include "UnigineCallback.h"
#include "UnigineString.h"
#include "UniginePalette.h"

namespace Unigine
{

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Console
{
protected:
	

public:
	static int isInitialized(); 

	enum LEVEL
	{
		LEVEL_NORMAL = 0,
		LEVEL_WARNING,
		LEVEL_ERROR,
	};
	static void setActive(bool active);
	static bool isActive();
	static void setLock(int lock);
	static int getLock();
	static void setBackgroundColor(const Math::vec4 &color);
	static Math::vec4 getBackgroundColor();
	static void setMessageColor(const Math::vec4 &color);
	static Math::vec4 getMessageColor();
	static void setWarningColor(const Math::vec4 &color);
	static Math::vec4 getWarningColor();
	static void setErrorColor(const Math::vec4 &color);
	static Math::vec4 getErrorColor();
	static void setTextColor(const Math::vec4 &color);
	static Math::vec4 getTextColor();
	static void setPrompt(const char *prompt);
	static const char *getPrompt();
	static void setLimit(int limit);
	static int getLimit();
	static void setHeight(int height);
	static int getHeight();
	static void setWrapping(bool wrapping);
	static bool isWrapping();
	static void setFontSize(int size);
	static int getFontSize();
	static void setOnscreen(bool onscreen);
	static bool isOnscreen();
	static void setOnscreenFontSize(int size);
	static int getOnscreenFontSize();
	static void setOnscreenHeight(int height);
	static int getOnscreenHeight();
	static void setOnscreenTime(float time);
	static float getOnscreenTime();
	static const char *getLastMessage();
	static const char *getLastWarning();
	static const char *getLastError();
	static void getMessages(Vector<String> &messages, Vector<int> &levels, int limit);
	static void getMessages(Vector<String> &messages, int limit);
	static void getWarnings(Vector<String> &messages, int limit);
	static void getErrors(Vector<String> &messages, int limit);
	static void write(const char *text);
	static void write(const Math::vec4 &color, const char *text);
	static void write(Console::LEVEL level, const char *text);
	static void writeLine(const char *text);
	static void writeLine(const Math::vec4 &color, const char *text);
	static void writeLine(Console::LEVEL level, const char *text);
	static void onscreenWrite(const char *text);
	static void onscreenWrite(const Math::vec4 &color, const char *text);
	static void onscreenWrite(Console::LEVEL level, const char *text);
	static void onscreenWriteLine(const char *text);
	static void onscreenWriteLine(const Math::vec4 &color, const char *text);
	static void onscreenWriteLine(Console::LEVEL level, const char *text);

	static void message(const Unigine::Math::vec4 &color, const char *format, ...);
	static void message(const char *format, ...);
	static void warning(const char *format, ...);
	static void error(const char *format, ...);

	static void messageLine(const Unigine::Math::vec4 &color, const char *format, ...);
	static void messageLine(const char *format, ...);
	static void warningLine(const char *format, ...);
	static void errorLine(const char *format, ...);

	static void onscreenMessage(const Unigine::Math::vec4 &color, const char *format, ...);
	static void onscreenMessage(const char *format, ...);
	static void onscreenWarning(const char *format, ...);
	static void onscreenError(const char *format, ...);

	static void onscreenMessageLine(const Unigine::Math::vec4 &color, const char *format, ...);
	static void onscreenMessageLine(const char *format, ...);
	static void onscreenWarningLine(const char *format, ...);
	static void onscreenErrorLine(const char *format, ...);

	static void *addOutputCallback(CallbackBase2<const char *, Console::LEVEL> *func);
	static bool removeOutputCallback(void *id);
	static void clearOutputCallbacks();
	static int getNumVariables();
	static const char *getVariableName(int num);
	static const char *getVariableDescription(const char *name);
	static int getNumCommands();
	static const char *getCommandName(int num);
	static const char *getCommandDescription(const char *name);
	static bool isCommand(const char *name);
	static bool isVariable(const char *name);
	static bool isInt(const char *name);
	static void setInt(const char *name, int value);
	static int getInt(const char *name);
	static int getIntMin(const char *name);
	static int getIntMax(const char *name);
	static bool isFloat(const char *name);
	static void setFloat(const char *name, float value);
	static float getFloat(const char *name);
	static float getFloatMin(const char *name);
	static float getFloatMax(const char *name);
	static bool isVec4(const char *name);
	static void setVec4(const char *name, const Math::vec4 &value);
	static Math::vec4 getVec4(const char *name);
	static bool isVec3(const char *name);
	static void setVec3(const char *name, const Math::vec3 &value);
	static Math::vec3 getVec3(const char *name);
	static bool isVec2(const char *name);
	static void setVec2(const char *name, const Math::vec2 &value);
	static Math::vec2 getVec2(const char *name);
	static bool isString(const char *name);
	static void setString(const char *name, const char *value);
	static const char *getString(const char *name);
	static bool isPalette(const char *name);
	static void setPalette(const char *name, const Palette &value);
	static void setPalette(const char *name, int index, float value);
	static void setPalette(const char *name, const char *color, float value);
	static Palette getPalette(const char *name);
	static void run(const char *command);
	static void flush();
	static int addCommand(const char *name, const char *desc, CallbackBase2<int, char **> *func);
	static bool removeCommand(const char *name);
};

class UNIGINE_API ConsoleVariableInt
{
	public:
		ConsoleVariableInt(const char *name, const char *desc, int save, int value, int min, int max);
		~ConsoleVariableInt();
		ConsoleVariableInt(const ConsoleVariableInt &) = delete;
		ConsoleVariableInt &operator=(ConsoleVariableInt&) = delete;
		UNIGINE_INLINE int operator=(int value) { set(value); return value; }
		UNIGINE_INLINE operator int() const { return get(); }
		void set(int value) const;
		int get() const;

		void setGetFunc(int(*func)());
		void setSetFunc(void(*func)(int));
	private:
		void *data;
};

class UNIGINE_API ConsoleVariableFloat
{
	public:
		ConsoleVariableFloat(const char *name, const char *desc, int save, float value, float min, float max);
		~ConsoleVariableFloat();
		ConsoleVariableFloat(const ConsoleVariableFloat &) = delete;
		ConsoleVariableFloat &operator=(ConsoleVariableFloat&) = delete;
		UNIGINE_INLINE float operator=(float value) { set(value); return value; }
		UNIGINE_INLINE operator float() const { return get(); }
		void set(float value) const;
		float get() const;

		void setGetFunc(float(*func)());
		void setSetFunc(void(*func)(float));
	private:
		void *data;
};

class UNIGINE_API ConsoleVariableVec4
{
	public:
		ConsoleVariableVec4(const char *name, const char *desc, int save, const Math::vec4 &value);
		~ConsoleVariableVec4();
		ConsoleVariableVec4(const ConsoleVariableVec4 &) = delete;
		ConsoleVariableVec4 &operator=(ConsoleVariableVec4&) = delete;
		UNIGINE_INLINE Math::vec4 operator=(const Math::vec4 &value) { set(value); return value; }
		UNIGINE_INLINE operator Math::vec4() const { return get(); }
		void set(const Math::vec4 &value) const;
		Math::vec4 get() const;

		void setGetFunc(Math::vec4(*func)());
		void setSetFunc(void(*func)(Math::vec4));
	private:
		void *data;
};

class UNIGINE_API ConsoleVariableVec3
{
	public:
		ConsoleVariableVec3(const char *name, const char *desc, int save, const Math::vec3 &value);
		~ConsoleVariableVec3();
		ConsoleVariableVec3(const ConsoleVariableVec3 &) = delete;
		ConsoleVariableVec3 &operator=(ConsoleVariableVec3&) = delete;
		UNIGINE_INLINE Math::vec3 operator=(const Math::vec3 &value) { set(value); return value; }
		UNIGINE_INLINE operator Math::vec3() const { return get(); }
		void set(const Math::vec3 &value) const;
		Math::vec3 get() const;

		void setGetFunc(Math::vec3(*func)());
		void setSetFunc(void(*func)(Math::vec3));
	private:
		void *data;
};

class UNIGINE_API ConsoleVariableVec2
{
	public:
		ConsoleVariableVec2(const char *name, const char *desc, int save, const Math::vec2 &value);
		~ConsoleVariableVec2();
		ConsoleVariableVec2(const ConsoleVariableVec2 &) = delete;
		ConsoleVariableVec2 &operator=(ConsoleVariableVec2&) = delete;
		UNIGINE_INLINE Math::vec2 operator=(const Math::vec2 &value) { set(value); return value; }
		UNIGINE_INLINE operator Math::vec2() const { return get(); }
		void set(const Math::vec2 &value) const;
		Math::vec2 get() const;

		void setGetFunc(Math::vec2(*func)());
		void setSetFunc(void(*func)(Math::vec2));
	private:
		void *data;
};

class UNIGINE_API ConsoleVariableString
{
	public:
		ConsoleVariableString(const char *name, const char *desc, int save, const char *value);
		~ConsoleVariableString();
		ConsoleVariableString(const ConsoleVariableString &) = delete;
		ConsoleVariableString &operator=(ConsoleVariableString&) = delete;
		UNIGINE_INLINE const char *operator=(const char *value) { set(value); return value; }
		UNIGINE_INLINE operator const char *() const { return get(); }
		void set(const char *value) const;
		const char *get() const;

		void setGetFunc(String(*func)());
		void setSetFunc(void(*func)(String));
	private:
		void *data;
};
} // namespace Unigine
