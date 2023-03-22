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
#include "UnigineMathLibCommon.h"

#include "UnigineMathLibVec4.h"
#include "UnigineMathLibDVec4.h"
#include "UnigineMathLibIVec4.h"

namespace Unigine
{
namespace Math
{

struct svec4
{
	UNIGINE_INLINE constexpr svec4(): x(0), y(0), z(0), w(0) {}
	UNIGINE_INLINE constexpr explicit svec4(unsigned short v): x(v), y(v), z(v), w(v) {}
	UNIGINE_INLINE constexpr svec4(unsigned short x, unsigned short y, unsigned short z, unsigned short w): x(x), y(y), z(z), w(w) {}
	UNIGINE_INLINE constexpr explicit svec4(const unsigned short *v): x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}
	UNIGINE_INLINE constexpr svec4(const svec4 &v) : long_v(v.long_v) {}

	UNIGINE_INLINE constexpr explicit svec4(const vec4 &v)
		: x(toUShort(v.x))
		, y(toUShort(v.y))
		, z(toUShort(v.z))
		, w(toUShort(v.w))
	{}
	UNIGINE_INLINE constexpr explicit svec4(const dvec4 &v)
		: x(toUShort(v.x))
		, y(toUShort(v.y))
		, z(toUShort(v.z))
		, w(toUShort(v.w))
	{}
	UNIGINE_INLINE constexpr explicit svec4(const ivec4 &v)
		: x(toUShort(v.x))
		, y(toUShort(v.y))
		, z(toUShort(v.z))
		, w(toUShort(v.w))
	{}

	UNIGINE_INLINE svec4(const vec4 &v, float scale) { set(v, scale); }

	UNIGINE_INLINE void set(unsigned short val)
	{
		x = val;
		y = val;
		z = val;
		w = val;
	}
	UNIGINE_INLINE void set(unsigned short x_, unsigned short y_, unsigned short z_, unsigned short w_)
	{
		x = x_;
		y = y_;
		z = z_;
		w = w_;
	}
	UNIGINE_INLINE void set(const svec4 &val)
	{
		long_v = val.long_v;
	}
	UNIGINE_INLINE void set(const unsigned short *val)
	{
		x = val[0];
		y = val[1];
		z = val[2];
		w = val[3];
	}
	UNIGINE_INLINE void set(const vec4 &val, float scale)
	{
		x = toUShort(clamp(toInt(val.x * scale), 0, 65535));
		y = toUShort(clamp(toInt(val.y * scale), 0, 65535));
		z = toUShort(clamp(toInt(val.z * scale), 0, 65535));
		w = toUShort(clamp(toInt(val.w * scale), 0, 65535));
	}

	UNIGINE_INLINE void get(unsigned short *val) const
	{
		val[0] = x;
		val[1] = y;
		val[2] = z;
		val[3] = w;
	}
	UNIGINE_INLINE unsigned short *get() { return v; }
	UNIGINE_INLINE const unsigned short *get() const { return v; }

	UNIGINE_INLINE operator unsigned short *() { return v; }
	UNIGINE_INLINE operator const unsigned short *() const { return v; }
	UNIGINE_INLINE operator void *() { return v; }
	UNIGINE_INLINE operator const void *() const { return v; }

	UNIGINE_INLINE unsigned short &operator[](int i)
	{
		assert((unsigned int)i < 4 && "svec4::operator[](): bad index");
		return v[i];
	}
	UNIGINE_INLINE unsigned short operator[](int i) const
	{
		assert((unsigned int)i < 4 && "svec4::operator[](): bad index");
		return v[i];
	}

	UNIGINE_INLINE svec4 &operator=(const svec4 &val) { set(val); return *this; }

	union
	{
		struct
		{
			unsigned short x, y, z, w;
		};
		unsigned short v[4];
		unsigned long long long_v;
	};
};

constexpr svec4 svec4_zero((unsigned short)0);
constexpr svec4 svec4_one((unsigned short)65535);

UNIGINE_INLINE int compare(const svec4 &v0, const svec4 &v1) { return v0.long_v == v1.long_v; }
UNIGINE_INLINE int operator==(const svec4 &v0, const svec4 &v1) { return v0.long_v == v1.long_v; }
UNIGINE_INLINE int operator!=(const svec4 &v0, const svec4 &v1) { return v0.long_v != v1.long_v; }

}
}
