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

struct bvec4
{
	UNIGINE_INLINE constexpr bvec4(): x(0), y(0), z(0), w(0) {}
	UNIGINE_INLINE constexpr explicit bvec4(unsigned char v): x(v), y(v), z(v), w(v) {}
	UNIGINE_INLINE constexpr bvec4(unsigned char x, unsigned char y, unsigned char z, unsigned char w) : x(x), y(y), z(z), w(w) {}
	UNIGINE_INLINE constexpr explicit bvec4(const unsigned char *v): x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}
	UNIGINE_INLINE constexpr bvec4(const bvec4 &v) : vec(v.vec) {}

	UNIGINE_INLINE constexpr explicit bvec4(const vec4 &v)
		: x(toUChar(v.x))
		, y(toUChar(v.y))
		, z(toUChar(v.z))
		, w(toUChar(v.w))
	{}
	UNIGINE_INLINE constexpr explicit bvec4(const dvec4 &v)
		: x(toUChar(v.x))
		, y(toUChar(v.y))
		, z(toUChar(v.z))
		, w(toUChar(v.w))
	{}
	UNIGINE_INLINE constexpr explicit bvec4(const ivec4 &v)
		: x(toUChar(v.x))
		, y(toUChar(v.y))
		, z(toUChar(v.z))
		, w(toUChar(v.w))
	{}

	UNIGINE_INLINE bvec4(const vec4 &v, float scale) { set(v, scale); }

	UNIGINE_INLINE void set(unsigned char val)
	{
		x = val;
		y = val;
		z = val;
		w = val;
	}
	UNIGINE_INLINE void set(unsigned char x_, unsigned char y_, unsigned char z_, unsigned char w_)
	{
		x = x_;
		y = y_;
		z = z_;
		w = w_;
	}
	UNIGINE_INLINE void set(const bvec4 &val)
	{
		x = val.x;
		y = val.y;
		z = val.z;
		w = val.w;
	}
	UNIGINE_INLINE void set(const unsigned char *val)
	{
		x = val[0];
		y = val[1];
		z = val[2];
		w = val[3];
	}
	UNIGINE_INLINE void set(const vec4 &val, float scale)
	{
		x = toUChar(clamp(ftoi(val.x * scale), 0, 255));
		y = toUChar(clamp(ftoi(val.y * scale), 0, 255));
		z = toUChar(clamp(ftoi(val.z * scale), 0, 255));
		w = toUChar(clamp(ftoi(val.w * scale), 0, 255));
	}

	UNIGINE_INLINE void get(unsigned char *val) const
	{
		val[0] = x;
		val[1] = y;
		val[2] = z;
		val[3] = w;
	}
	UNIGINE_INLINE unsigned char *get() { return v; }
	UNIGINE_INLINE const unsigned char *get() const { return v; }

	UNIGINE_INLINE operator unsigned char *() { return v; }
	UNIGINE_INLINE operator const unsigned char *() const { return v; }
	UNIGINE_INLINE operator void *() { return v; }
	UNIGINE_INLINE operator const void *() const { return v; }

	UNIGINE_INLINE unsigned char &operator[](int i)
	{
		assert((unsigned int)i < 4 && "bvec4::operator[](): bad index");
		return v[i];
	}
	UNIGINE_INLINE unsigned char operator[](int i) const
	{
		assert((unsigned int)i < 4 && "bvec4::operator[](): bad index");
		return v[i];
	}

	UNIGINE_INLINE bvec4 &operator=(const bvec4 &val) { vec = val.vec; return *this; }

	union
	{
		struct
		{
			unsigned char x, y, z, w;
		};
		unsigned char v[4];
		unsigned int vec;
	};
};

constexpr bvec4 bvec4_zero((unsigned char)0);
constexpr bvec4 bvec4_one((unsigned char)255);

UNIGINE_INLINE int compare(const bvec4 &v0, const bvec4 &v1) { return v0.vec == v1.vec; }
UNIGINE_INLINE int operator==(const bvec4 &v0, const bvec4 &v1) { return v0.vec == v1.vec; }
UNIGINE_INLINE int operator!=(const bvec4 &v0, const bvec4 &v1) { return v0.vec != v1.vec; }

UNIGINE_INLINE bvec4 min(const bvec4 &v0, const bvec4 &v1)
{
	bvec4 ret;
	ret.x = toUChar(min(v0.x, v1.x));
	ret.y = toUChar(min(v0.y, v1.y));
	ret.z = toUChar(min(v0.z, v1.z));
	ret.w = toUChar(min(v0.w, v1.w));
	return ret;
}
UNIGINE_INLINE bvec4 max(const bvec4 &v0, const bvec4 &v1)
{
	bvec4 ret;
	ret.x = toUChar(max(v0.x, v1.x));
	ret.y = toUChar(max(v0.y, v1.y));
	ret.z = toUChar(max(v0.z, v1.z));
	ret.w = toUChar(max(v0.w, v1.w));
	return ret;
}
UNIGINE_INLINE bvec4 clamp(const bvec4 &v, const bvec4 &v0, const bvec4 &v1)
{
	bvec4 ret;
	ret.x = toUChar(clamp(v.x, v0.x, v1.x));
	ret.y = toUChar(clamp(v.y, v0.y, v1.y));
	ret.z = toUChar(clamp(v.z, v0.z, v1.z));
	ret.w = toUChar(clamp(v.w, v0.w, v1.w));
	return ret;
}

}
}