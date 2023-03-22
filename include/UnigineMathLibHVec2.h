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
#include "UnigineMathLibVec2.h"
#include "UnigineMathLibDVec2.h"

namespace Unigine
{
namespace Math
{

struct hvec2
{
	UNIGINE_INLINE constexpr hvec2(): x(0), y(0) {}
	UNIGINE_INLINE hvec2(const hvec2 &v) : x(v.x), y(v.y) {}
	UNIGINE_INLINE constexpr hvec2(half x, half y) : x(x), y(y) {}
	UNIGINE_INLINE constexpr explicit hvec2(half v): x(v), y(v) {}
	UNIGINE_INLINE explicit hvec2(float v): x(v), y(v) {}
	UNIGINE_INLINE explicit hvec2(const vec2 &v): x(v.x), y(v.y) {}
	UNIGINE_INLINE explicit hvec2(const dvec2 &v): x(toFloat(v.x)), y(toFloat(v.y)) {}

	UNIGINE_INLINE hvec2 &operator=(const hvec2 &v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

	UNIGINE_INLINE operator half *() { return &x; }
	UNIGINE_INLINE operator const half *() const { return &x; }
	UNIGINE_INLINE operator void *() { return &x; }
	UNIGINE_INLINE operator const void *() const { return &x; }

	UNIGINE_INLINE half &operator[](int i)
	{
		assert((unsigned int)i < 2 && "hvec2::operator[](): bad index");
		return (&x)[i];
	}
	UNIGINE_INLINE half operator[](int i) const
	{
		assert((unsigned int)i < 2 && "hvec2::operator[](): bad index");
		return (&x)[i];
	}

	half x, y;
};

constexpr hvec2 hvec2_zero(half(0x0000));
constexpr hvec2 hvec2_one(half(0x3c00));

}
}