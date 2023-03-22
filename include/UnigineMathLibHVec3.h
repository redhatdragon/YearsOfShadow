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
#include "UnigineMathLibVec3.h"
#include "UnigineMathLibDVec3.h"

namespace Unigine
{
namespace Math
{

struct hvec3
{
	UNIGINE_INLINE constexpr hvec3(): x(0), y(0), z(0) {}
	UNIGINE_INLINE hvec3(const hvec3 &v) : x(v.x), y(v.y), z(v.z) {}
	UNIGINE_INLINE constexpr hvec3(half x, half y, half z) : x(x), y(y), z(z) {}
	UNIGINE_INLINE constexpr explicit hvec3(half v): x(v), y(v), z(v) {}
	UNIGINE_INLINE explicit hvec3(float v): x(v), y(v), z(v) {}
	UNIGINE_INLINE explicit hvec3(const vec3 &v): x(v.x), y(v.y), z(v.z) {}
	UNIGINE_INLINE explicit hvec3(const dvec3 &v)
		: x(toFloat(v.x))
		, y(toFloat(v.y))
		, z(toFloat(v.z))
	{}

	UNIGINE_INLINE hvec3 &operator=(const hvec3 &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	UNIGINE_INLINE operator half *() { return &x; }
	UNIGINE_INLINE operator const half *() const { return &x; }
	UNIGINE_INLINE operator void *() { return &x; }
	UNIGINE_INLINE operator const void *() const { return &x; }
	UNIGINE_INLINE half &operator[](int i)
	{
		assert((unsigned int)i < 3 && "hvec3::operator[](): bad index");
		return (&x)[i];
	}
	UNIGINE_INLINE half operator[](int i) const
	{
		assert((unsigned int)i < 3 && "hvec3::operator[](): bad index");
		return (&x)[i];
	}

	half x, y, z, align;
};

constexpr hvec3 hvec3_zero(half(0x0000));
constexpr hvec3 hvec3_one(half(0x3c00));

}
}