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

#include "UnigineImage.h"
#include "UnigineCallback.h"

namespace Unigine
{

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API LoadingScreen
{
protected:
	

public:
	static int isInitialized(); 
	static void setEnabled(bool enabled);
	static bool isEnabled();
	static void setThreshold(int threshold);
	static int getThreshold();
	static void setTexturePath(const char *path);
	static const char *getTexturePath();
	static void setImage(const Ptr<Image> &image);
	static void setTransform(const Math::vec4 &transform);
	static Math::vec4 getTransform();
	static void setBackgroundColor(const Math::vec4 &color);
	static Math::vec4 getBackgroundColor();
	static void setText(const char *text);
	static const char *getText();
	static void setFontPath(const char *path);
	static const char *getFontPath();
	static void setMessageLoadingWorld(const char *world);
	static const char *getMessageLoadingWorld();
	static void setMessageShadersCompilation(const char *compilation);
	static const char *getMessageShadersCompilation();
	static void renderInterface();
	static void render();
	static void render(int progress);
	static void render(int progress, const char *message);
	static void renderForce();
	static void renderForce(const char *message);
	static int getProgress();
	static const char *getMessage();
	static void *addRenderBeginCallback(CallbackBase *func);
	static bool removeRenderBeginCallback(void *id);
	static void clearRenderBeginCallbacks();
	static void *addRenderEndCallback(CallbackBase *func);
	static bool removeRenderEndCallback(void *id);
	static void clearRenderEndCallbacks();
};

} // namespace Unigine
