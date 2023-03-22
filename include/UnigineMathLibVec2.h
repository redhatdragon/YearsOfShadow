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

namespace Unigine
{
namespace Math
{

struct alignas(8) vec2
{
	UNIGINE_INLINE constexpr vec2(float v, ConstexprTag): x(v), y(v) {}
	UNIGINE_INLINE constexpr vec2(float x, float y, ConstexprTag) : x(x), y(y) {}

	UNIGINE_INLINE vec2()
		: x(0.0f)
		, y(0.0f)
	{
		UNIGINE_ASSERT_ALIGNED8(this);
	}
	UNIGINE_INLINE explicit vec2(float v)
		: x(v)
		, y(v)
	{
		UNIGINE_ASSERT_ALIGNED8(this);
	}
	UNIGINE_INLINE vec2(float x, float y)
		: x(x)
		, y(y)
	{
		UNIGINE_ASSERT_ALIGNED8(this);
	}
	UNIGINE_INLINE vec2(const vec2 &v)
		: x(v.x)
		, y(v.y)
	{
		UNIGINE_ASSERT_ALIGNED8(this);
	}
	UNIGINE_INLINE explicit vec2(const float *v)
		: x(v[0])
		, y(v[1])
	{
		UNIGINE_ASSERT_ALIGNED8(this);
	}

	UNIGINE_INLINE explicit vec2(const vec3 &v);
	UNIGINE_INLINE explicit vec2(const vec4 &v);
	UNIGINE_INLINE explicit vec2(const dvec2 &v);
	UNIGINE_INLINE explicit vec2(const hvec2 &v);
	UNIGINE_INLINE explicit vec2(const ivec2 &v);

	UNIGINE_INLINE void set(float val) { x = val; y = val; }
	UNIGINE_INLINE void set(float x_, float y_) { x = x_; y = y_; }
	UNIGINE_INLINE void set(const vec2 &val)
	{
		#ifdef USE_SSE
			vec = val.vec;
		#else
			x = val.x;
			y = val.y;
		#endif
	}
	UNIGINE_INLINE void set(const float *val) { x = val[0]; y = val[1]; }

	UNIGINE_INLINE void get(float *val) const { val[0] = x; val[1] = y; }
	UNIGINE_INLINE float *get() { return v; }
	UNIGINE_INLINE const float *get() const { return v; }

	UNIGINE_INLINE operator float *() { return v; }
	UNIGINE_INLINE operator const float *() const { return v; }
	UNIGINE_INLINE operator void *() { return v; }
	UNIGINE_INLINE operator const void *() const { return v; }

	UNIGINE_INLINE float &operator[](int i)
	{
		assert((unsigned int)i < 2 && "vec2::operator[](): bad index");
		return v[i];
	}
	UNIGINE_INLINE float operator[](int i) const
	{
		assert((unsigned int)i < 2 && "vec2::operator[](): bad index");
		return v[i];
	}

	UNIGINE_INLINE vec2 &operator=(const vec2 &v) { set(v); return *this; }
	UNIGINE_INLINE vec2 operator-() const { return vec2(-x, -y); }

	UNIGINE_INLINE void mul(float v)
	{
		x *= v;
		y *= v;
	}
	UNIGINE_INLINE void mul(const vec2 &v)
	{
		x *= v.x;
		y *= v.y;
	}
	UNIGINE_INLINE vec2 &operator*=(float v) { mul(v); return *this; }
	UNIGINE_INLINE vec2 &operator*=(const vec2 &v) { mul(v); return *this; }

	UNIGINE_INLINE void div(float v) { mul(Math::rcp(v)); }
	UNIGINE_INLINE void div(const vec2 &v)
	{
		x /= v.x;
		y /= v.y;
	}
	UNIGINE_INLINE vec2 &operator/=(float v) { div(v); return *this; }
	UNIGINE_INLINE vec2 &operator/=(const vec2 &v) { div(v); return *this; }
	UNIGINE_INLINE vec2 &operator/=(const ivec2 &v);

	UNIGINE_INLINE void sub(float v)
	{
		x -= v;
		y -= v;
	}
	UNIGINE_INLINE void sub(const vec2 &v)
	{
		x -= v.x;
		y -= v.y;
	}
	UNIGINE_INLINE vec2 &operator-=(float v) { sub(v); return *this; }
	UNIGINE_INLINE vec2 &operator-=(const vec2 &v) { sub(v); return *this; }

	UNIGINE_INLINE float sum() const { return x + y; }
	UNIGINE_INLINE void add(float v)
	{
		x += v;
		y += v;
	}
	UNIGINE_INLINE void add(const vec2 &v)
	{
		x += v.x;
		y += v.y;
	}
	UNIGINE_INLINE vec2 &operator+=(float v) { add(v); return *this; }
	UNIGINE_INLINE vec2 &operator+=(const vec2 &v) { add(v); return *this; }

	UNIGINE_INLINE float length2() const { return x * x + y * y; }
	UNIGINE_INLINE float length() const { return toFloat(sqrt(length2())); }
	UNIGINE_INLINE float iLength() const { return Math::rsqrt(length2()); }
	UNIGINE_INLINE float iLengthFast() const { return Math::rsqrtFast(length2()); }

	UNIGINE_INLINE vec2 &normalize()
	{
		mul(iLength());
		return *this;
	}
	UNIGINE_INLINE vec2 &normalizeValid()
	{
		float length = length2();
		if (length != 0.0f)
			mul(Math::rsqrt(length));
		return *this;
	}
	UNIGINE_INLINE vec2 &normalizeFast()
	{
		mul(iLengthFast());
		return *this;
	}
	UNIGINE_INLINE vec2 &normalizeValidFast()
	{
		float length = length2();
		if (length != 0.0f)
			mul(Math::rsqrtFast(length));
		return *this;
	}

	UNIGINE_INLINE vec2 frac() const { return vec2(Math::frac(x), Math::frac(y)); }
	UNIGINE_INLINE vec2 &abs()
	{
		x = Math::abs(x);
		y = Math::abs(y);
		return *this;
	}

	UNIGINE_INLINE float max() const { return Math::max(x, y); }
	UNIGINE_INLINE float min() const { return Math::min(x, y); }

	UNIGINE_INLINE unsigned int hash() const { return hashCombine(hashInteger(x), y); }

	UNIGINE_INLINE ivec2 floor() const;
	UNIGINE_INLINE ivec2 ceil() const;
	
public:
	union
	{
		struct
		{
			float x, y;
		};
		float v[2];

		#ifdef USE_SSE
			__m64 vec; // DO NOT TOUCH UNLESS ABSOLUTELY SURE
		#endif
	};

}; // end class vec2

constexpr vec2 vec2_zero(0.0f, ConstexprTag{});
constexpr vec2 vec2_one(1.0f, ConstexprTag{});
constexpr vec2 vec2_half(0.5f, ConstexprTag{});
constexpr vec2 vec2_eps(Consts::EPS, ConstexprTag{});
constexpr vec2 vec2_inf(Consts::INF, ConstexprTag{});


UNIGINE_INLINE float length(const vec2 &v) { return v.length(); }
UNIGINE_INLINE float length2(const vec2 &v) { return v.length2(); }

UNIGINE_INLINE vec2 normalize(const vec2 &v)
{
	vec2 ret = v;
	return ret.normalize();
}
UNIGINE_INLINE vec2 normalizeValid(const vec2 &v)
{
	vec2 ret = v;
	return ret.normalizeValid();
}


UNIGINE_INLINE int compare(const vec2 &v0, const vec2 &v1)
{
	return compare(v0.x, v1.x) && compare(v0.y, v1.y);
}
UNIGINE_INLINE int compare(const vec2 &v0, const vec2 &v1, float epsilon)
{
	return compare(v0.x, v1.x, epsilon) && compare(v0.y, v1.y, epsilon);
}
UNIGINE_INLINE int operator==(const vec2 &v0, const vec2 &v1) { return compare(v0, v1); }
UNIGINE_INLINE int operator!=(const vec2 &v0, const vec2 &v1) { return !compare(v0, v1); }

UNIGINE_INLINE int operator<(const vec2 &v0, const vec2 &v1) { return v0.x < v1.x && v0.y < v1.y; }
UNIGINE_INLINE int operator>(const vec2 &v0, const vec2 &v1) { return v0.x > v1.x && v0.y > v1.y; }

UNIGINE_INLINE int operator>=(const vec2 &v0, const vec2 &v1) { return v0.x >= v1.x && v0.y >= v1.y; }
UNIGINE_INLINE int operator<=(const vec2 &v0, const vec2 &v1) { return v0.x <= v1.x && v0.y <= v1.y; }

UNIGINE_INLINE vec2 &mul(vec2 &ret, const vec2 &v0, float v1)
{
	ret.x = v0.x * v1;
	ret.y = v0.y * v1;
	return ret;
}
UNIGINE_INLINE vec2 &mul(vec2 &ret, const vec2 &v0, const vec2 &v1)
{
	ret.x = v0.x * v1.x;
	ret.y = v0.y * v1.y;
	return ret;
}
UNIGINE_INLINE vec2 mul(const vec2 &v0, float v1)
{
	vec2 ret;
	mul(ret, v0, v1);
	return ret;
}
UNIGINE_INLINE vec2 mul(const vec2 &v0, const vec2 &v1)
{
	vec2 ret;
	mul(ret, v0, v1);
	return ret;
}
UNIGINE_INLINE vec2 operator*(const vec2 &v0, float v1) { return mul(v0, v1); }
UNIGINE_INLINE vec2 operator*(const vec2 &v0, const vec2 &v1) { return mul(v0, v1); }

UNIGINE_INLINE vec2 &div(vec2 &ret, const vec2 &v0, float v1) { mul(ret, v0, rcp(v1)); return ret; }
UNIGINE_INLINE vec2 &div(vec2 &ret, const vec2 &v0, const vec2 &v1)
{
	ret.x = v0.x / v1.x;
	ret.y = v0.y / v1.y;
	return ret;
}
UNIGINE_INLINE vec2 div(const vec2 &v0, float v1)
{
	vec2 ret;
	div(ret, v0, v1);
	return ret;
}
UNIGINE_INLINE vec2 div(const vec2 &v0, const vec2 &v1)
{
	vec2 ret;
	div(ret, v0, v1);
	return ret;
}
UNIGINE_INLINE vec2 operator/(const vec2 &v0, float v1) { return div(v0, v1); }
UNIGINE_INLINE vec2 operator/(const vec2 &v0, const vec2 &v1) { return div(v0, v1); }

UNIGINE_INLINE vec2 &add(vec2 &ret, const vec2 &v0, float v1)
{
	ret.x = v0.x + v1;
	ret.y = v0.y + v1;
	return ret;
}
UNIGINE_INLINE vec2 &add(vec2 &ret, const vec2 &v0, const vec2 &v1)
{
	ret.x = v0.x + v1.x;
	ret.y = v0.y + v1.y;
	return ret;
}
UNIGINE_INLINE vec2 add(const vec2 &v0, float v1)
{
	vec2 ret;
	add(ret, v0, v1);
	return ret;
}
UNIGINE_INLINE vec2 add(const vec2 &v0, const vec2 &v1)
{
	vec2 ret;
	add(ret, v0, v1);
	return ret;
}
UNIGINE_INLINE vec2 operator+(const vec2 &v0, float v1) { return add(v0, v1); }
UNIGINE_INLINE vec2 operator+(const vec2 &v0, const vec2 &v1) { return add(v0, v1); }

UNIGINE_INLINE vec2 &sub(vec2 &ret, const vec2 &v0, float v1)
{
	ret.x = v0.x - v1;
	ret.y = v0.y - v1;
	return ret;
}
UNIGINE_INLINE vec2 &sub(vec2 &ret, const vec2 &v0, const vec2 &v1)
{
	ret.x = v0.x - v1.x;
	ret.y = v0.y - v1.y;
	return ret;
}
UNIGINE_INLINE vec2 sub(const vec2 &v0, float v1)
{
	vec2 ret;
	sub(ret, v0, v1);
	return ret;
}
UNIGINE_INLINE vec2 sub(const vec2 &v0, const vec2 &v1)
{
	vec2 ret;
	sub(ret, v0, v1);
	return ret;
}
UNIGINE_INLINE vec2 operator-(const vec2 &v0, float v1) { return sub(v0, v1); }
UNIGINE_INLINE vec2 operator-(const vec2 &v0, const vec2 &v1) { return sub(v0, v1); }

UNIGINE_INLINE vec2 &mad(vec2 &ret, const vec2 &v0, float v1, const vec2 &v2)
{
	ret.x = v0.x * v1 + v2.x;
	ret.y = v0.y * v1 + v2.y;
	return ret;
}
UNIGINE_INLINE vec2 &mad(vec2 &ret, const vec2 &v0, const vec2 &v1, const vec2 &v2)
{
	ret.x = v0.x * v1.x + v2.x;
	ret.y = v0.y * v1.y + v2.y;
	return ret;
}
UNIGINE_INLINE vec2 mad(const vec2 &v0, float v1, const vec2 &v2)
{
	vec2 ret;
	mad(ret, v0, v1, v2);
	return ret;
}
UNIGINE_INLINE vec2 mad(const vec2 &v0, const vec2 &v1, const vec2 &v2)
{
	vec2 ret;
	mad(ret, v0, v1, v2);
	return ret;
}

UNIGINE_INLINE vec2 &lerp(vec2 &ret, const vec2 &v0, float v1, float k)
{
	ret.x = lerp(v0.x, v1, k);
	ret.y = lerp(v0.y, v1, k);
	return ret;
}
UNIGINE_INLINE vec2 &lerp(vec2 &ret, float v0, const vec2 &v1, float k)
{
	ret.x = lerp(v0, v1.x, k);
	ret.y = lerp(v0, v1.y, k);
	return ret;
}
UNIGINE_INLINE vec2 &lerp(vec2 &ret, const vec2 &v0, const vec2 &v1, float k)
{
	ret.x = lerp(v0.x, v1.x, k);
	ret.y = lerp(v0.y, v1.y, k);
	return ret;
}
UNIGINE_INLINE vec2 &lerp(vec2 &ret, const vec2 &v0, const vec2 &v1, const vec2 &k)
{
	ret.x = lerp(v0.x, v1.x, k.x);
	ret.y = lerp(v0.y, v1.y, k.y);
	return ret;
}
UNIGINE_INLINE vec2 lerp(const vec2 &v0, const vec2 &v1, float k)
{
	vec2 ret;
	lerp(ret, v0, v1, k);
	return ret;
}
UNIGINE_INLINE vec2 lerp(const vec2 &v0, const vec2 &v1, const vec2 &k)
{
	vec2 ret;
	lerp(ret, v0, v1, k);
	return ret;
}

UNIGINE_INLINE vec2 &inverseLerp(vec2 &ret, const vec2 &v0, const vec2 &v1, const vec2 &k)
{
	ret.x = inverseLerp(v0.x, v1.x, k.x);
	ret.y = inverseLerp(v0.y, v1.y, k.y);
	return ret;
}
UNIGINE_INLINE vec2 inverseLerp(const vec2 &v0, const vec2 &v1, const vec2 &k)
{
	vec2 ret;
	lerp(ret, v0, v1, k);
	return ret;
}

UNIGINE_INLINE float dot(const vec2 &v0, const vec2 &v1) { return v0.x * v1.x + v0.y * v1.y; }
UNIGINE_INLINE float cross(const vec2 &v0, const vec2 &v1) { return v0.x * v1.y - v0.y * v1.x; }

UNIGINE_INLINE vec2 round(const vec2 &v) { return {round(v.x), round(v.y)}; }
UNIGINE_INLINE vec2 floor(const vec2 &v) { return {floor(v.x), floor(v.y)}; }
UNIGINE_INLINE vec2 ceil(const vec2 &v) { return {ceil(v.x), ceil(v.y)}; }
UNIGINE_INLINE vec2 abs(const vec2 &v) { return {abs(v.x), abs(v.y)}; }
UNIGINE_INLINE vec2 saturate(const vec2 &v) { return {saturate(v.x), saturate(v.y)}; }

UNIGINE_INLINE vec2 min(const vec2 &v0, const vec2 &v1)
{
	vec2 ret;
	ret.x = min(v0.x, v1.x);
	ret.y = min(v0.y, v1.y);
	return ret;
}
UNIGINE_INLINE vec2 max(const vec2 &v0, const vec2 &v1)
{
	vec2 ret;
	ret.x = max(v0.x, v1.x);
	ret.y = max(v0.y, v1.y);
	return ret;
}
UNIGINE_INLINE vec2 min(const vec2 &v0, float v1)
{
	vec2 ret;
	ret.x = min(v0.x, v1);
	ret.y = min(v0.y, v1);
	return ret;
}
UNIGINE_INLINE vec2 max(const vec2 &v0, float v1)
{
	vec2 ret;
	ret.x = max(v0.x, v1);
	ret.y = max(v0.y, v1);
	return ret;
}
UNIGINE_INLINE vec2 clamp(const vec2 &v, const vec2 &v0, const vec2 &v1)
{
	vec2 ret;
	ret.x = clamp(v.x, v0.x, v1.x);
	ret.y = clamp(v.y, v0.y, v1.y);
	return ret;
}

UNIGINE_INLINE vec2 srgb(const vec2 &color) { return vec2(srgb(color.x), srgb(color.y)); }
UNIGINE_INLINE vec2 isrgb(const vec2 &color) { return vec2(isrgb(color.x), isrgb(color.y)); }

UNIGINE_INLINE vec2 contrastLerp(const vec2 &point_a, const vec2 &point_b, float coef_min_bound, float coef_max_bound, float coef)
{
	return lerp(point_a, point_b, smoothstep(coef_min_bound, coef_max_bound, coef));
}

UNIGINE_INLINE vec2 step(const vec2 &a, const vec2 &b)
{
	return {toFloat(a.x > b.x), toFloat(a.y > b.y)};
}
UNIGINE_INLINE vec2 step(const vec2 &a, float b)
{
	return {toFloat(a.x > b), toFloat(a.y > b)};
}

UNIGINE_INLINE vec2 overlay(const vec2 &a, const vec2 &b, float x)
{
	return max(lerp(a, lerp(vec2_one - (vec2_one - a) * (vec2_one - b) * 2.0f, a * b * 2.0f, step(a, vec2_half)), x), vec2_zero);
}
UNIGINE_INLINE vec2 overlay(const vec2 &a, const vec2 &b, const vec2 &x)
{
	return max(lerp(a, lerp(vec2_one - (vec2_one - a) * (vec2_one - b) * 2.0f, a * b * 2.0f, step(a, vec2_half)), x), vec2_zero);
}

UNIGINE_INLINE float distance2(const vec2 &v0, const vec2 &v1) { return length2(v0 - v1); }
UNIGINE_INLINE float distance(const vec2 &v0, const vec2 &v1) { return length(v0 - v1); }

}
}