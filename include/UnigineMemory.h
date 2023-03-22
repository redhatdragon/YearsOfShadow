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

namespace Unigine
{

/// allocator statistics
struct MemoryStats
{
	long long	total_bytes;		// total allocated bytes, ie. reserved by Unigine allocator plus system allocator
	long long	heap_used_bytes;	// bytes currently used within Unigine allocator (not to confuse with reserved!)
	long long	system_bytes;		// bytes currently used by means of system allocator
	int			total_allocs;		// total existing allocs
	int			system_allocs;		// total existing system allocs
	int			frame_allocs;		// allocs made during the last frame
};

/// public interface to Unigine allocator
class Memory
{
public:
	static UNIGINE_API int init();
	static UNIGINE_API int shutdown();
	static UNIGINE_API int isInitialized();

	static UNIGINE_API void *allocate(size_t size);
	static UNIGINE_API void deallocate(void *ptr);
	static UNIGINE_API void deallocate(void *ptr, size_t size);

	static UNIGINE_API MemoryStats getStats();

	static UNIGINE_API size_t getHeapUsage();
	static UNIGINE_API size_t getMemoryUsage();
	static UNIGINE_API size_t getSystemUsage();
	static UNIGINE_API int getNumAllocations();
	static UNIGINE_API int getNumFrameAllocations();
};

/// Unigine Base class; managed using Unigine allocator by default
class Base
{
public:
	UNIGINE_DECLARE_USE_MEMORY
};

} // namespace

