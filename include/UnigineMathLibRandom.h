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
#include "UnigineMathLibCommon.h"
#include "UnigineMathLibVec3.h"
#include "UnigineMathLibVec4.h"

namespace Unigine
{
namespace Math
{

struct Random
{
public:
	mutable unsigned int seed;

	enum
	{
		A = 1664525,
		C = 1013904223,
		MAX_RANDOM = 0x7fffffff,
	};

	UNIGINE_INLINE static Random &getRandom()
	{
		static thread_local Random random;
		return random;
	}

	UNIGINE_INLINE Random() { setSeed((unsigned int)time(NULL)); }
	UNIGINE_INLINE Random(unsigned int seed_) { setSeed(seed_); }

	UNIGINE_INLINE void setSeed(unsigned int seed_) const { seed = seed_; }
	UNIGINE_INLINE unsigned int getSeed() const { return seed; }

	UNIGINE_INLINE unsigned int get() const
	{
		seed = (unsigned int)((unsigned long long)seed * A + C) & MAX_RANDOM;
		return seed;
	}

	UNIGINE_INLINE int getInt() { return (long)(get() % INT_MAX); }

	UNIGINE_INLINE unsigned long long getULong() { return doubleIntToLong(get(), get()); }
	UNIGINE_INLINE long long getLong() { return (long)(getULong() % LLONG_MAX); }

	UNIGINE_INLINE float getFloat() { return intBitsToFloat(get()); }
	UNIGINE_INLINE double getDouble() { return longBitsToFloat(getLong()); }

	int getInt(int from, int to) const
	{
		unsigned int range = static_cast<unsigned int>(Math::max(to, from) - from);
		if (range <= 1)
			return from;

		unsigned int rand = get();

		if (range < (MAX_RANDOM >> 16))
		{
			rand >>= 16;
			unsigned int rand_max = ((MAX_RANDOM >> 16) / range) * range;
			while (rand > rand_max)
				rand = get() >> 16;
		} else
		{
			unsigned int rand_max = (MAX_RANDOM / range) * range;
			while (rand > rand_max)
				rand = get();
		}

		return from + rand % range;
	}
	UNIGINE_INLINE float getFloat(float from, float to) const
	{
		union
		{
			unsigned int i;
			float f;
		} rand = {(0x3f800000 | (get() & 0x007fffff))};

		return from + (rand.f - 1.0f) * (to - from);
	}
	UNIGINE_INLINE double getDouble(double from, double to) const
	{
		return from + (double)get() / MAX_RANDOM * (to - from);
	}

	UNIGINE_INLINE vec4 getColor() const
	{
		vec4 color;
		color.x = getFloat(0.0f, 1.0f);
		color.y = getFloat(0.0f, 1.0f);
		color.z = getFloat(0.0f, 1.0f);
		color /= max(color.xyz.max(), Consts::EPS);
		color.w = 1.0f;
		return color;
	}
	UNIGINE_INLINE vec3 getDirection() const
	{
		vec3 dir;
		dir.x = getFloat(0.0f, 1.0f);
		dir.y = getFloat(0.0f, 1.0f);
		dir.z = getFloat(0.0f, 1.0f);
		return dir.normalize();
	}
};

UNIGINE_INLINE auto randInt() { return Random::getRandom().get(); }
UNIGINE_INLINE auto randUInt() { return Random::getRandom().getInt(); }
UNIGINE_INLINE auto randULong() { return Random::getRandom().getULong(); }
UNIGINE_INLINE auto randLong() { return Random::getRandom().getLong(); }
UNIGINE_INLINE auto randFloat() { return Random::getRandom().getFloat(); }
UNIGINE_INLINE auto randDouble() { return Random::getRandom().getDouble(); }
UNIGINE_INLINE auto randColor() { return Random::getRandom().getColor(); }
UNIGINE_INLINE auto randDirection() { return Random::getRandom().getDirection(); }
UNIGINE_INLINE auto randInt(int from, int to) { return Random::getRandom().getInt(from, to); }
UNIGINE_INLINE auto randFloat(float from, float to) { return Random::getRandom().getFloat(from, to); }
UNIGINE_INLINE auto randDouble(double from, double to) { return Random::getRandom().getDouble(from, to); }
UNIGINE_INLINE auto randFixedFloat() { return Random::getRandom().getFloat(0.0f, 1.0f); }
UNIGINE_INLINE auto randFixedDouble() { return Random::getRandom().getDouble(0.0f, 1.0f); }

}
} // namespace Unigine
