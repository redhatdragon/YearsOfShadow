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

/**
 * Unigine namespace.
 */
namespace Unigine
{

#ifdef _LINUX
	#ifdef Time
		#undef Time
	#endif
#endif

class Time
{
public:
	enum
	{
		CLOCKS_PER_SECOND = 1000000,
	};
	UNIGINE_INLINE static double MicrosecondsToSeconds(long long microseconds) { return (double)microseconds / CLOCKS_PER_SECOND; }
	UNIGINE_INLINE static double MicrosecondsToMilliseconds(long long microseconds) { return (double)(microseconds * 1000) / CLOCKS_PER_SECOND; }

	/**
	 * Returns CPU clocks counter.
	 *
	 * @return CPU clocks counter.
	 */
	UNIGINE_API static long long getClock();

	/**
	 * Returns time in microseconds.
	 *
	 * @return Time in microseconds.
	 */
	UNIGINE_API static long long get();

	/**
	 * Returns time in seconds.
	 *
	 * @return Time in seconds.
	 */
	UNIGINE_INLINE static double getSeconds() { return MicrosecondsToSeconds(get()); }

	/**
	 * Returns time in milliseconds.
	 *
	 * @return Time in milliseconds.
	 */
	UNIGINE_INLINE static double getMilliseconds() { return MicrosecondsToMilliseconds(get()); }
};

struct Timer
{
	long long begin_time{0};

	// return elapsed time
	UNIGINE_INLINE void begin() { begin_time = Time::get(); }
	UNIGINE_INLINE long long end() const { return Time::get() - begin_time; }
	UNIGINE_INLINE double endSeconds() const { return Time::MicrosecondsToSeconds(end()); }
	UNIGINE_INLINE double endMilliseconds() const { return Time::MicrosecondsToMilliseconds(end()); }

	UNIGINE_INLINE long long getBeginTime() const { return begin_time; }
};

} /* namespace Unigine */
