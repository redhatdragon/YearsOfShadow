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

namespace Unigine
{

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API SystemInfo
{
protected:
	

public:
	static int isInitialized(); 

	enum
	{
		GPU_UNKNOWN = 0,
		GPU_AMD,
		GPU_NVIDIA,
		GPU_INTEL,
		GPU_ARM,
		GPU_APPLE,
		GPU_IMGTEC,
		GPU_QUALCOMM,
	};
	static const char *getBinaryInfo();
	static const char *getRevisionInfo();
	static const char *getSystemInfo();
	static const char *getCPUInfo();
	static int getSystemMemory();
	static int getCPUFrequency();
	static int getCPUCount();
	static int getCPUThreads();
	static bool hasMMX();
	static bool hasSSE();
	static bool hasSSE2();
	static bool hasSSE3();
	static bool hasSSE4();
	static bool hasSSE5();
	static bool hasAVX();
	static bool hasAVX2();
	static bool hasNeon();
	static bool hasHyperThreading();
	static int getGPUCount();
	static const char *getGPUInfo(int video_adapter_num = 0);
	static int getGPUID(int video_adapter_num = 0);
	static int getGPUMemory(int video_adapter_num = 0);
	static int getGPUName(const char *str);
	static int getGPUName(int video_adapter_num = 0);
};

} // namespace Unigine
