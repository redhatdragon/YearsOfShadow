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
#include "UniginePtr.h"
#include "UnigineMathLib.h"

namespace Unigine
{

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Displays
{
protected:
	

public:
	static int isInitialized(); 

	enum ORIENTATION
	{
		ORIENTATION_UNKNOWN = -1,
		ORIENTATION_LANDSCAPE = 0,
		ORIENTATION_LANDSCAPE_FLIPPED,
		ORIENTATION_PORTRAIT,
		ORIENTATION_PORTRAIT_FLIPPED,
	};
	static int getMain();
	static int getCurrent();
	static int getDefaultSystemDPI();
	static int getNum();
	static Math::ivec2 getPosition(int display_index);
	static Math::ivec2 getResolution(int display_index);
	static int getRefreshRate(int display_index);
	static int getCurrentMode(int display_index);
	static int getDesktopMode(int display_index);
	static Displays::ORIENTATION getOrientation(int display_index);
	static int getDPI(int display_index);
	static const char *getName(int display_index);
	static int getUniqueID(int display_index);
	static int findDisplay(const char *name);
	static int findDisplay(int unique_id);
	static int getNumModes(int display_index);
	static Math::ivec2 getModeResolution(int display_index, int mode_index);
	static int getModeRefreshRate(int display_index, int mode_index);
	static int findMode(int display_index, const Math::ivec2 &resolution);
	static int findMode(int display_index, const Math::ivec2 &resolution, int refresh_rate);
};

} // namespace Unigine
