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

struct alignas(8) ivec2
{
	UNIGINE_INLINE constexpr ivec2(int v, ConstexprTag): x(v), y(v) {}

	UNIGINE_INLINE ivec2(): x(0), y(0)
	{
		UNIGINE_ASSERT_ALIGNED8(this);
	}
	UNIGINE_INLINE ivec2(int x, int y): x(x), y(y)
	{
		UNIGINE_ASSERT_ALIGNED8(this);
	}
	UNIGINE_INLINE explicit ivec2(int v): x(v), y(v)
	{
		UNIGINE_ASSERT_ALIGNED8(this);
	}
	UNIGINE_INLINE explicit ivec2(const int *v): x(v[0]), y(v[1])
	{
		UNIGINE_ASSERT_ALIGNED8(this);
	}
	UNIGINE_INLINE explicit ivec2(const dvec2 &v): x(toInt(v.x)), y(toInt(v.y))
	{
		UNIGINE_ASSERT_ALIGNED8(this);
	}
	UNIGINE_INLINE explicit ivec2(const vec2 &v): x(toInt(v.x)), y(toInt(v.y))
	{
		UNIGINE_ASSERT_ALIGNED8(this);
	}
	UNIGINE_INLINE ivec2(const ivec2 & v): long_v(v.long_v)
	{
		UNIGINE_ASSERT_ALIGNED8(this);
	}
	
	UNIGINE_INLINE void set(int val)
	{
		x = val;
		y = val;
	}
	UNIGINE_INLINE void set(int x_, int y_)
	{
		x = x_;
		y = y_;
	}
	UNIGINE_INLINE void set(const ivec2 &val)
	{
		long_v = val.long_v;
	}
	UNIGINE_INLINE void set(const int *val)
	{
		x = val[0];
		y = val[1];
	}

	UNIGINE_INLINE void get(int *val) const
	{
		val[0] = x;
		val[1] = y;
	}
	UNIGINE_INLINE int *get() { return v; }
	UNIGINE_INLINE const int *get() const { return v; }

	UNIGINE_INLINE operator int *() { return v; }
	UNIGINE_INLINE operator const int *() const { return v; }
	UNIGINE_INLINE operator void *() { return v; }
	UNIGINE_INLINE operator const void *() const { return v; }

	UNIGINE_INLINE int &operator[](int i)
	{
		assert((unsigned int)i < 2 && "ivec2::operator[](): bad index");
		return v[i];
	}
	UNIGINE_INLINE int operator[](int i) const
	{
		assert((unsigned int)i < 2 && "ivec2::operator[](): bad index");
		return v[i];
	}

	UNIGINE_INLINE ivec2 &operator=(const ivec2 &val) { long_v = val.long_v; return *this; }
	UNIGINE_INLINE ivec2 operator-() const { return ivec2(-x, -y); }

	UNIGINE_INLINE int mul() const { return x * y; }
	UNIGINE_INLINE void mul(int v) { x *= v; y *= v; }
	UNIGINE_INLINE void mul(const ivec2 &v) { x *= v.x; y *= v.y; }
	UNIGINE_INLINE ivec2 &operator*=(int v) { mul(v); return *this; }
	UNIGINE_INLINE ivec2 &operator*=(const ivec2 &v) { mul(v); return *this; }

	UNIGINE_INLINE void div(int v) { x /= v; y /= v; }
	UNIGINE_INLINE void div(const ivec2 &v) { x /= v.x; y /= v.y; }
	UNIGINE_INLINE ivec2 &operator/=(int v) { div(v); return *this; }
	UNIGINE_INLINE ivec2 &operator/=(const ivec2 &v) { div(v); return *this; }

	UNIGINE_INLINE void sub(int v) { x -= v; y -= v; }
	UNIGINE_INLINE void sub(const ivec2 &v) { x -= v.x; y -= v.y; }
	UNIGINE_INLINE ivec2 &operator-=(int v) { add(v); return *this; }
	UNIGINE_INLINE ivec2 &operator-=(const ivec2 &v) { sub(v); return *this; }

	UNIGINE_INLINE int sum() const { return x + y; }
	UNIGINE_INLINE void add(int v) { x += v; y += v; }
	UNIGINE_INLINE void add(const ivec2 &v) { x += v.x; y += v.y; }
	UNIGINE_INLINE ivec2 &operator+=(int v) { add(v); return *this; }
	UNIGINE_INLINE ivec2 &operator+=(const ivec2 &v) { add(v); return *this; }

	UNIGINE_INLINE void shiftLeft(int v) { x <<= v; y <<= v; }
	UNIGINE_INLINE void shiftLeft(const ivec2 &v) { x <<= v.x; y <<= v.y; }
	UNIGINE_INLINE ivec2 &operator<<=(int v) { shiftLeft(v); return *this; }
	UNIGINE_INLINE ivec2 &operator<<=(const ivec2 &v) { shiftLeft(v); return *this; }
	
	UNIGINE_INLINE void shiftRight(int v) { x >>= v; y >>= v; }
	UNIGINE_INLINE void shiftRight(const ivec2 &v) { x >>= v.x; y >>= v.y; }
	UNIGINE_INLINE ivec2 &operator>>=(int v) { shiftRight(v); return *this; }
	UNIGINE_INLINE ivec2 &operator>>=(const ivec2 &v) { shiftRight(v); return *this; }

	UNIGINE_INLINE int length2() const { return x * x + y * y; }

	UNIGINE_INLINE ivec2 &abs()
	{
		x = Math::abs(x);
		y = Math::abs(y);
		return *this;
	}

	UNIGINE_INLINE int max() const { return Math::max(x, y); }
	UNIGINE_INLINE int min() const { return Math::min(x, y); }

	UNIGINE_INLINE unsigned int hash() const { return hashCombine(x, y); }

	union
	{
		struct
		{
			int x, y;
		};
		int v[2];
		long long long_v;

		#ifdef USE_SSE
			__m64 vec;
		#endif
	};
};

constexpr ivec2 ivec2_zero(0, ConstexprTag{});
constexpr ivec2 ivec2_one(1, ConstexprTag{});

UNIGINE_INLINE int length2(const ivec2 &v) { return v.length2(); }

UNIGINE_INLINE bool compare(const ivec2 &v0, const ivec2 &v1) { return v0.long_v == v1.long_v; }
UNIGINE_INLINE bool operator==(const ivec2 &v0, const ivec2 &v1) { return v0.long_v == v1.long_v; }
UNIGINE_INLINE bool operator!=(const ivec2 &v0, const ivec2 &v1) { return v0.long_v != v1.long_v; }

UNIGINE_INLINE bool operator<(const ivec2 &v0, const ivec2 &v1) { return v0.long_v < v1.long_v; }
UNIGINE_INLINE bool operator>(const ivec2 &v0, const ivec2 &v1) { return v0.long_v > v1.long_v; }

UNIGINE_INLINE bool operator>=(const ivec2 &v0, const ivec2 &v1) { return v0.long_v >= v1.long_v; }
UNIGINE_INLINE bool operator<=(const ivec2 &v0, const ivec2 &v1) { return v0.long_v <= v1.long_v; }

UNIGINE_INLINE ivec2 &mul(ivec2 &ret, const ivec2 &v0, int v1)
{
	ret.x = v0.x * v1;
	ret.y = v0.y * v1;
	return ret;
}
UNIGINE_INLINE ivec2 &mul(ivec2 &ret, const ivec2 &v0, const ivec2 &v1)
{
	ret.x = v0.x * v1.x;
	ret.y = v0.y * v1.y;
	return ret;
}
UNIGINE_INLINE ivec2 mul(const ivec2 &v0, int v1)
{
	ivec2 ret;
	return mul(ret, v0, v1);
}
UNIGINE_INLINE ivec2 mul(const ivec2 &v0, const ivec2 &v1)
{
	ivec2 ret;
	return mul(ret, v0, v1);
}
UNIGINE_INLINE ivec2 operator*(const ivec2 &v0, int v1) { return mul(v0, v1); }
UNIGINE_INLINE ivec2 operator*(const ivec2 &v0, const ivec2 &v1) { return mul(v0, v1); }

UNIGINE_INLINE ivec2 &div(ivec2 &ret, const ivec2 &v0, int v1)
{
	ret.x = v0.x / v1;
	ret.y = v0.y / v1;
	return ret;
}
UNIGINE_INLINE ivec2 &div(ivec2 &ret, const ivec2 &v0, const ivec2 &v1)
{
	ret.x = v0.x / v1.x;
	ret.y = v0.y / v1.y;
	return ret;
}
UNIGINE_INLINE ivec2 div(const ivec2 &v0, int v1)
{
	ivec2 ret;
	return div(ret, v0, v1);
}
UNIGINE_INLINE ivec2 div(const ivec2 &v0, const ivec2 &v1)
{
	ivec2 ret;
	return div(ret, v0, v1);
}
UNIGINE_INLINE ivec2 operator/(const ivec2 &v0, int v1) { return div(v0, v1); }
UNIGINE_INLINE ivec2 operator/(const ivec2 &v0, const ivec2 &v1) { return div(v0, v1); }

UNIGINE_INLINE ivec2 &mod(ivec2 &ret, const ivec2 &v0, int v1)
{
	ret.x = v0.x % v1;
	ret.y = v0.y % v1;
	return ret;
}
UNIGINE_INLINE ivec2 &mod(ivec2 &ret, const ivec2 &v0, const ivec2 &v1)
{
	ret.x = v0.x % v1.x;
	ret.y = v0.y % v1.y;
	return ret;
}
UNIGINE_INLINE ivec2 mod(const ivec2 &v0, int v1)
{
	ivec2 ret;
	return mod(ret, v0, v1);
}
UNIGINE_INLINE ivec2 mod(const ivec2 &v0, const ivec2 &v1)
{
	ivec2 ret;
	return mod(ret, v0, v1);
}
UNIGINE_INLINE ivec2 operator%(const ivec2 &v0, int v1) { return mod(v0, v1); }
UNIGINE_INLINE ivec2 operator%(const ivec2 &v0, const ivec2 &v1) { return mod(v0, v1); }

UNIGINE_INLINE ivec2 &add(ivec2 &ret, const ivec2 &v0, int v1)
{
	ret.x = v0.x + v1;
	ret.y = v0.y + v1;
	return ret;
}
UNIGINE_INLINE ivec2 &add(ivec2 &ret, const ivec2 &v0, const ivec2 &v1)
{
	ret.x = v0.x + v1.x;
	ret.y = v0.y + v1.y;
	return ret;
}
UNIGINE_INLINE ivec2 add(const ivec2 &v0, int v1)
{
	ivec2 ret;
	return add(ret, v0, v1);
}
UNIGINE_INLINE ivec2 add(const ivec2 &v0, const ivec2 &v1)
{
	ivec2 ret;
	return add(ret, v0, v1);
}
UNIGINE_INLINE ivec2 operator+(const ivec2 &v0, int v1) { return add(v0, v1); }
UNIGINE_INLINE ivec2 operator+(const ivec2 &v0, const ivec2 &v1) { return add(v0, v1); }

UNIGINE_INLINE ivec2 &sub(ivec2 &ret, const ivec2 &v0, int v1)
{
	ret.x = v0.x - v1;
	ret.y = v0.y - v1;
	return ret;
}
UNIGINE_INLINE ivec2 &sub(ivec2 &ret, const ivec2 &v0, const ivec2 &v1)
{
	ret.x = v0.x - v1.x;
	ret.y = v0.y - v1.y;
	return ret;
}
UNIGINE_INLINE ivec2 sub(const ivec2 &v0, int v1)
{
	ivec2 ret;
	return sub(ret, v0, v1);
}
UNIGINE_INLINE ivec2 sub(const ivec2 &v0, const ivec2 &v1)
{
	ivec2 ret;
	return sub(ret, v0, v1);
}
UNIGINE_INLINE ivec2 operator-(const ivec2 &v0, int v1) { return sub(v0, v1); }
UNIGINE_INLINE ivec2 operator-(const ivec2 &v0, const ivec2 &v1) { return sub(v0, v1); }

UNIGINE_INLINE ivec2 &shiftLeft(ivec2 &ret, const ivec2 &v0, int v1)
{
	ret.x = v0.x << v1;
	ret.y = v0.y << v1;
	return ret;
}
UNIGINE_INLINE ivec2 &shiftLeft(ivec2 &ret, const ivec2 &v0, const ivec2 &v1)
{
	ret.x = v0.x << v1.x;
	ret.y = v0.y << v1.y;
	return ret;
}
UNIGINE_INLINE ivec2 shiftLeft(const ivec2 &v0, int v1)
{
	ivec2 ret;
	return shiftLeft(ret, v0, v1);
}
UNIGINE_INLINE ivec2 shiftLeft(const ivec2 &v0, const ivec2 &v1)
{
	ivec2 ret;
	return shiftLeft(ret, v0, v1);
}
UNIGINE_INLINE ivec2 operator<<(const ivec2 &v0, int v1) { return shiftLeft(v0, v1); }
UNIGINE_INLINE ivec2 operator<<(const ivec2 &v0, const ivec2 &v1) { return shiftLeft(v0, v1); }

UNIGINE_INLINE ivec2 &shiftRight(ivec2 &ret, const ivec2 &v0, int v1)
{
	ret.x = v0.x >> v1;
	ret.y = v0.y >> v1;
	return ret;
}
UNIGINE_INLINE ivec2 &shiftRight(ivec2 &ret, const ivec2 &v0, const ivec2 &v1)
{
	ret.x = v0.x >> v1.x;
	ret.y = v0.y >> v1.y;
	return ret;
}
UNIGINE_INLINE ivec2 shiftRight(const ivec2 &v0, int v1)
{
	ivec2 ret;
	return shiftRight(ret, v0, v1);
}
UNIGINE_INLINE ivec2 shiftRight(const ivec2 &v0, const ivec2 &v1)
{
	ivec2 ret;
	return shiftRight(ret, v0, v1);
}
UNIGINE_INLINE ivec2 operator>>(const ivec2 &v0, int v1) { return shiftRight(v0, v1); }
UNIGINE_INLINE ivec2 operator>>(const ivec2 &v0, const ivec2 &v1) { return shiftRight(v0, v1); }

UNIGINE_INLINE ivec2 &mad(ivec2 &ret, const ivec2 &v0, int v1, const ivec2 &v2)
{
	ret.x = v0.x * v1 + v2.x;
	ret.y = v0.y * v1 + v2.y;
	return ret;
}
UNIGINE_INLINE ivec2 &mad(ivec2 &ret, const ivec2 &v0, const ivec2 &v1, const ivec2 &v2)
{
	ret.x = v0.x * v1.x + v2.x;
	ret.y = v0.y * v1.y + v2.y;
	return ret;
}
UNIGINE_INLINE ivec2 mad(const ivec2 &v0, int v1, const ivec2 &v2)
{
	ivec2 ret;
	return mad(ret, v0, v1, v2);
}
UNIGINE_INLINE ivec2 mad(const ivec2 &v0, const ivec2 &v1, const ivec2 &v2)
{
	ivec2 ret;
	return mad(ret, v0, v1, v2);
}

UNIGINE_INLINE ivec2 &lerp(ivec2 &ret, const ivec2 &v0, const ivec2 &v1, int k)
{
	ret.x = lerp(v0.x, v1.x, k);
	ret.y = lerp(v0.y, v1.y, k);
	return ret;
}
UNIGINE_INLINE ivec2 &lerp(ivec2 &ret, const ivec2 &v0, const ivec2 &v1, const ivec2 &k)
{
	ret.x = lerp(v0.x, v1.x, k.x);
	ret.y = lerp(v0.y, v1.y, k.y);
	return ret;
}
UNIGINE_INLINE ivec2 lerp(const ivec2 &v0, const ivec2 &v1, const ivec2 &k)
{
	ivec2 ret;
	return lerp(ret, v0, v1, k);
}
UNIGINE_INLINE ivec2 lerp(const ivec2 &v0, const ivec2 &v1, int k)
{
	ivec2 ret;
	return lerp(ret, v0, v1, k);
}

UNIGINE_INLINE int dot(const ivec2 &v0, const ivec2 &v1)
{
	return v0.x * v1.x + v0.y * v1.y;
}

UNIGINE_INLINE ivec2 min(const ivec2 &v0, const ivec2 &v1)
{
	ivec2 ret;
	ret.x = min(v0.x, v1.x);
	ret.y = min(v0.y, v1.y);
	return ret;
}
UNIGINE_INLINE ivec2 max(const ivec2 &v0, const ivec2 &v1)
{
	ivec2 ret;
	ret.x = max(v0.x, v1.x);
	ret.y = max(v0.y, v1.y);
	return ret;
}
UNIGINE_INLINE ivec2 clamp(const ivec2 &v, const ivec2 &v0, const ivec2 &v1)
{
	ivec2 ret;
	ret.x = clamp(v.x, v0.x, v1.x);
	ret.y = clamp(v.y, v0.y, v1.y);
	return ret;
}
UNIGINE_INLINE ivec2 abs(const ivec2 &v)
{
	return {abs(v.x), abs(v.y)};
}

}
}