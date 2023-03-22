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
#include "UnigineMathLibDVec2.h"

namespace Unigine
{
namespace Math
{

struct alignas(16) dvec3
{
	UNIGINE_INLINE constexpr dvec3(double v, ConstexprTag): x(v), y(v), z(v), align(0.0f) {}
	UNIGINE_INLINE constexpr dvec3(double x, double y, double z, ConstexprTag) : x(x), y(y), z(z), align(0.0f) {}

	UNIGINE_INLINE dvec3()
		: x(0.0)
		, y(0.0)
		, z(0.0)
		, align(0)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit dvec3(double v)
		: x(v)
		, y(v)
		, z(v)
		, align(0)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit dvec3(float v)
		: x(v)
		, y(v)
		, z(v)
		, align(0)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE dvec3(double x, double y, double z)
		: x(x)
		, y(y)
		, z(z)
		, align(0)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit dvec3(const double *v)
		: x(v[0])
		, y(v[1])
		, z(v[2])
		, align(0)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE dvec3(const dvec3 &v)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(v);
	}

	UNIGINE_INLINE dvec3(const dvec2 &v, double z)
		: x(v.x)
		, y(v.y)
		, z(z)
		, align(0)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit dvec3(const dvec2 &v)
		: x(v.x)
		, y(v.y)
		, z(0.0)
		, align(0)
	{}
	UNIGINE_INLINE explicit dvec3(const vec3 &v)
		: x(v.x)
		, y(v.y)
		, z(v.z)
		, align(0)
	{}
	
	#ifdef USE_SSE
		UNIGINE_INLINE dvec3(const __m128d &v0, const __m128d &v1)
		{
			UNIGINE_ASSERT_ALIGNED16(this);
			sse.v0 = v0;
			sse.v1 = v1;
		}
	#endif

	UNIGINE_INLINE explicit dvec3(const dvec4 &v);
	UNIGINE_INLINE explicit dvec3(const hvec3 &v);
	UNIGINE_INLINE explicit dvec3(const ivec3 &v);
	
	UNIGINE_INLINE void set(double val) { x = val; y = val; z = val; }
	UNIGINE_INLINE void set(double x_, double y_, double z_) { x = x_; y = y_; z = z_; }
	UNIGINE_INLINE void set(const dvec2 &val, double z_ = 0.0) { x = val.x; y = val.y; z = z_; }

	UNIGINE_INLINE void set(const dvec3 &val)
	{
		sse.v0 = val.sse.v0;
		sse.v1 = val.sse.v1;
	}
	UNIGINE_INLINE void set(const double *val)
	{
		x = val[0];
		y = val[1];
		z = val[2];
	}

	UNIGINE_INLINE void get(double *val) const
	{
		val[0] = x;
		val[1] = y;
		val[2] = z;
	}
	UNIGINE_INLINE double *get() { return v; }
	UNIGINE_INLINE const double *get() const { return v; }

	UNIGINE_INLINE operator double *() { return v; }
	UNIGINE_INLINE operator const double *() const { return v; }
	UNIGINE_INLINE operator void *() { return v; }
	UNIGINE_INLINE operator const void *() const { return v; }

	UNIGINE_INLINE double &operator[](int i)
	{
		assert((unsigned int)i < 3 && "dvec3::operator[](): bad index");
		return v[i];
	}
	UNIGINE_INLINE double operator[](int i) const
	{
		assert((unsigned int)i < 3 && "dvec3::operator[](): bad index");
		return v[i];
	}

	UNIGINE_INLINE dvec3 operator-() const { return dvec3(-x, -y, -z); }
	UNIGINE_INLINE dvec3 &operator=(const dvec3 &v) { set(v); return *this; }

	UNIGINE_INLINE void mul(double v)
	{
		#ifdef USE_SSE
			__m128d temp = _mm_set1_pd(v);
			sse.v0 = _mm_mul_pd(sse.v0, temp);
			sse.v1 = _mm_mul_pd(sse.v1, temp);
		#else
			x *= v;
			y *= v;
			z *= v;
		#endif
	}
	UNIGINE_INLINE void mul(const dvec3 &v)
	{
		#ifdef USE_SSE
			sse.v0 = _mm_mul_pd(sse.v0, v.sse.v0);
			sse.v1 = _mm_mul_pd(sse.v1, v.sse.v1);
		#else
			x *= v.x;
			y *= v.y;
			z *= v.z;
		#endif
	}
	UNIGINE_INLINE dvec3 &operator*=(double v) { mul(v); return *this; }
	UNIGINE_INLINE dvec3 &operator*=(const dvec3 &v) { mul(v); return *this; }
	
	UNIGINE_INLINE void div(double v) { mul(rcp(v)); }
	UNIGINE_INLINE void div(const dvec3 &v)
	{
		#ifdef USE_SSE
			sse.v0 = _mm_div_pd(sse.v0, v.sse.v0);
			sse.v1 = _mm_div_pd(sse.v1, v.sse.v1);
		#else
			x /= v.x;
			y /= v.y;
			z /= v.z;
		#endif
	}
	UNIGINE_INLINE dvec3 &operator/=(double v) { div(v); return *this; }
	UNIGINE_INLINE dvec3 &operator/=(const dvec3 &v) { div(v); return *this; }

	UNIGINE_INLINE void add(double v)
	{
		#ifdef USE_SSE
			__m128d temp = _mm_set1_pd(v);
			sse.v0 = _mm_add_pd(sse.v0, temp);
			sse.v1 = _mm_add_pd(sse.v1, temp);
		#else
			x += v;
			y += v;
			z += v;
		#endif
	}
	UNIGINE_INLINE void add(const dvec3 &v)
	{
		#ifdef USE_SSE
			sse.v0 = _mm_add_pd(sse.v0, v.sse.v0);
			sse.v1 = _mm_add_pd(sse.v1, v.sse.v1);
		#else
			x += v.x;
			y += v.y;
			z += v.z;
		#endif
	}
	UNIGINE_INLINE dvec3 &operator+=(double v) { add(v); return *this; }
	UNIGINE_INLINE dvec3 &operator+=(const dvec3 &v) { add(v); return *this; }

	UNIGINE_INLINE void sub(double v)
	{
		#ifdef USE_SSE
			__m128d temp = _mm_set1_pd(v);
			sse.v0 = _mm_sub_pd(sse.v0, temp);
			sse.v1 = _mm_sub_pd(sse.v1, temp);
		#else
			x -= v;
			y -= v;
			z -= v;
		#endif
	}
	UNIGINE_INLINE void sub(const dvec3 &v)
	{
		#ifdef USE_SSE
			sse.v0 = _mm_sub_pd(sse.v0, v.sse.v0);
			sse.v1 = _mm_sub_pd(sse.v1, v.sse.v1);
		#else
			x -= v.x;
			y -= v.y;
			z -= v.z;
		#endif
	}
	UNIGINE_INLINE dvec3 &operator-=(double v) { sub(v); return *this; }
	UNIGINE_INLINE dvec3 &operator-=(const dvec3 &v) { sub(v); return *this; }

	UNIGINE_INLINE double sum() const { return x + y + z; }
	UNIGINE_INLINE double length2() const { return x * x + y * y + z * z; }
	UNIGINE_INLINE double length() const { return sqrt(length2()); }
	UNIGINE_INLINE double iLength() const { return Math::rsqrt(length2()); }
	
	UNIGINE_INLINE dvec3 &normalize() { mul(iLength()); return *this; }
	UNIGINE_INLINE dvec3 &normalizeValid()
	{
		double length = length2();
		if (length != 0.0)
			mul(Math::rsqrt(length));
		return *this;
	}

	UNIGINE_INLINE double maxXY() const { return Math::max(x, y); }
	UNIGINE_INLINE double max() const { return Math::max(maxXY(), z); }

	UNIGINE_INLINE double minXY() { return Math::min(x, y); }
	UNIGINE_INLINE double min() { return Math::min(minXY(), z); }

	UNIGINE_INLINE dvec3 &abs()
	{
		x = Math::abs(x); y = Math::abs(y); z = Math::abs(z);
		return *this;
	}
	UNIGINE_INLINE dvec3 sign() const { return dvec3(Math::sign(x), Math::sign(y), Math::sign(z)); }

	UNIGINE_INLINE unsigned int hash() const { return hashCast64To32(hashCombine(hashCombine(hashInteger(x), y), z)); }

	#ifdef USE_SSE
		struct sse_data
		{
			__m128d v0;
			__m128d v1;
		}; 
	#endif

	union
	{
		struct
		{
			double x, y, z, align;
		};
		double v[3];
		dvec2 xy;

		#ifdef USE_SSE
			sse_data sse; // DO NOT TOUCH UNLESS ABSOLUTELY SURE
		#endif
	};
};

constexpr dvec3 dvec3_zero(0.0, ConstexprTag{});
constexpr dvec3 dvec3_one(1.0, ConstexprTag{});
constexpr dvec3 dvec3_eps(Consts::EPS, ConstexprTag{});
constexpr dvec3 dvec3_inf(Consts::INF, ConstexprTag{});
constexpr dvec3 dvec3_up(0.0f, 0.0f, 1.0f, ConstexprTag{});
constexpr dvec3 dvec3_down(0.0f, 0.0f, -1.0f, ConstexprTag{});
constexpr dvec3 dvec3_forward(0.0f, 1.0f, 0.0f, ConstexprTag{});
constexpr dvec3 dvec3_back(0.0f, -1.0f, 0.0f, ConstexprTag{});
constexpr dvec3 dvec3_right(1.0f, 0.0f, 0.0f, ConstexprTag{});
constexpr dvec3 dvec3_left(-1.0f, 0.0f, 0.0f, ConstexprTag{});

UNIGINE_INLINE double length(const dvec3 &v) { return v.length(); }
UNIGINE_INLINE double length2(const dvec3 &v) { return v.length2(); }

UNIGINE_INLINE dvec3 normalize(const dvec3 &v)
{
	dvec3 ret = v;
	return ret.normalize();
}
UNIGINE_INLINE dvec3 normalizeValid(const dvec3 &v)
{
	dvec3 ret = v;
	return ret.normalizeValid();
}

UNIGINE_INLINE int compare(const dvec3 &v0, const dvec3 &v1)
{
	return (compare(v0.x, v1.x) && compare(v0.y, v1.y) && compare(v0.z, v1.z));
}
UNIGINE_INLINE int compare(const dvec3 &v0, const dvec3 &v1, double eps)
{
	return (compare(v0.x, v1.x, eps) && compare(v0.y, v1.y, eps) && compare(v0.z, v1.z, eps));
}
UNIGINE_INLINE int operator==(const dvec3 &v0, const dvec3 &v1) { return compare(v0, v1); }
UNIGINE_INLINE int operator!=(const dvec3 &v0, const dvec3 &v1) { return !compare(v0, v1); }

UNIGINE_INLINE int operator<(const dvec3 &v0, const dvec3 &v1) { return v0.x < v1.x &&v0.y < v1.y &&v0.z < v1.z; }
UNIGINE_INLINE int operator>(const dvec3 &v0, const dvec3 &v1) { return v0.x > v1.x && v0.y > v1.y && v0.z > v1.z; }

UNIGINE_INLINE int operator<=(const dvec3 &v0, const dvec3 &v1) { return v0.x <= v1.x && v0.y <= v1.y && v0.z <= v1.z; }
UNIGINE_INLINE int operator>=(const dvec3 &v0, const dvec3 &v1) { return v0.x >= v1.x && v0.y >= v1.y && v0.z >= v1.z; }

UNIGINE_INLINE dvec3 &mul(dvec3 &ret, const dvec3 &v0, double v1)
{
	#ifdef USE_SSE
		__m128d temp = _mm_set1_pd(v1);
		ret.sse.v0 = _mm_mul_pd(v0.sse.v0, temp);
		ret.sse.v1 = _mm_mul_pd(v0.sse.v1, temp);
	#else
		ret.x = v0.x * v1;
		ret.y = v0.y * v1;
		ret.z = v0.z * v1;
	#endif
	return ret;
}
UNIGINE_INLINE dvec3 &mul(dvec3 &ret, const dvec3 &v0, const dvec3 &v1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_mul_pd(v0.sse.v0, v1.sse.v0);
		ret.sse.v1 = _mm_mul_pd(v0.sse.v1, v1.sse.v1);
	#else
		ret.x = v0.x * v1.x;
		ret.y = v0.y * v1.y;
		ret.z = v0.z * v1.z;
	#endif
	return ret;
}
UNIGINE_INLINE dvec3 mul(const dvec3 &v0, double v1)
{
	dvec3 ret;
	return mul(ret, v0, v1);
}
UNIGINE_INLINE dvec3 mul(const dvec3 &v0, const dvec3 &v1)
{
	dvec3 ret;
	return mul(ret, v0, v1);
}
UNIGINE_INLINE dvec3 operator*(const dvec3 &v0, double v1) { return mul(v0, v1); }
UNIGINE_INLINE dvec3 operator*(const dvec3 &v0, const dvec3 &v1) { return mul(v0, v1); }

UNIGINE_INLINE dvec3 &div(dvec3 &ret, const dvec3 &v0, double v1) { return mul(ret, v0, rcp(v1)); }
UNIGINE_INLINE dvec3 &div(dvec3 &ret, const dvec3 &v0, const dvec3 &v1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_div_pd(v0.sse.v0, v1.sse.v0);
		ret.sse.v1 = _mm_div_pd(v0.sse.v1, v1.sse.v1);
	#else
		ret.x = v0.x / v1.x;
		ret.y = v0.y / v1.y;
		ret.z = v0.z / v1.z;
	#endif
	return ret;
}
UNIGINE_INLINE dvec3 div(const dvec3 &v0, double v1)
{
	dvec3 ret;
	return div(ret, v0, v1);
}
UNIGINE_INLINE dvec3 div(const dvec3 &v0, const dvec3 &v1)
{
	dvec3 ret;
	return div(ret, v0, v1);
}
UNIGINE_INLINE dvec3 operator/(const dvec3 &v0, double v1) { return div(v0, v1); }
UNIGINE_INLINE dvec3 operator/(const dvec3 &v0, const dvec3 &v1) { return div(v0, v1); }

UNIGINE_INLINE dvec3 &add(dvec3 &ret, const dvec3 &v0, double v1)
{
	#ifdef USE_SSE
		__m128d temp = _mm_set1_pd(v1);
		ret.sse.v0 = _mm_add_pd(v0.sse.v0, temp);
		ret.sse.v1 = _mm_add_pd(v0.sse.v1, temp);
	#else
		ret.x = v0.x + v1;
		ret.y = v0.y + v1;
		ret.z = v0.z + v1;
	#endif
	return ret;
}
UNIGINE_INLINE dvec3 &add(dvec3 &ret, const dvec3 &v0, const dvec3 &v1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_add_pd(v0.sse.v0, v1.sse.v0);
		ret.sse.v1 = _mm_add_pd(v0.sse.v1, v1.sse.v1);
	#else
		ret.x = v0.x + v1.x;
		ret.y = v0.y + v1.y;
		ret.z = v0.z + v1.z;
	#endif
	return ret;
}
UNIGINE_INLINE vec3 &add(vec3 &ret, const dvec3 &v0, const dvec3 &v1)
{
	ret.x = toFloat(v0.x + v1.x);
	ret.y = toFloat(v0.y + v1.y);
	ret.z = toFloat(v0.z + v1.z);
	return ret;
}
UNIGINE_INLINE dvec3 add(const dvec3 &v0, double v1)
{
	dvec3 ret;
	return add(ret, v0, v1);
}
UNIGINE_INLINE dvec3 add(const dvec3 &v0, const dvec3 &v1)
{
	dvec3 ret;
	return add(ret, v0, v1);
}
UNIGINE_INLINE dvec3 operator+(const dvec3 &v0, double v1) { return add(v0, v1); }
UNIGINE_INLINE dvec3 operator+(const dvec3 &v0, const dvec3 &v1) { return add(v0, v1); }

UNIGINE_INLINE dvec3 &sub(dvec3 &ret, const dvec3 &v0, double v1)
{
	#ifdef USE_SSE
		__m128d temp = _mm_set1_pd(v1);
		ret.sse.v0 = _mm_sub_pd(v0.sse.v0, temp);
		ret.sse.v1 = _mm_sub_pd(v0.sse.v1, temp);
	#else
		ret.x = v0.x - v1;
		ret.y = v0.y - v1;
		ret.z = v0.z - v1;
	#endif
	return ret;
}
UNIGINE_INLINE dvec3 &sub(dvec3 &ret, const dvec3 &v0, const dvec3 &v1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_sub_pd(v0.sse.v0, v1.sse.v0);
		ret.sse.v1 = _mm_sub_pd(v0.sse.v1, v1.sse.v1);
	#else
		ret.x = v0.x - v1.x;
		ret.y = v0.y - v1.y;
		ret.z = v0.z - v1.z;
	#endif
	return ret;
}
UNIGINE_INLINE vec3 &sub(vec3 &ret, const dvec3 &v0, const dvec3 &v1)
{
	ret.x = toFloat(v0.x - v1.x);
	ret.y = toFloat(v0.y - v1.y);
	ret.z = toFloat(v0.z - v1.z);
	return ret;
}
UNIGINE_INLINE dvec3 sub(const dvec3 &v0, double v1)
{
	dvec3 ret;
	return sub(ret, v0, v1);
}
UNIGINE_INLINE dvec3 sub(const dvec3 &v0, const dvec3 &v1)
{
	dvec3 ret;
	return sub(ret, v0, v1);
}
UNIGINE_INLINE dvec3 operator-(const dvec3 &v0, double v1) { return sub(v0, v1); }
UNIGINE_INLINE dvec3 operator-(const dvec3 &v0, const dvec3 &v1) { return sub(v0, v1); }

UNIGINE_INLINE dvec3 &mad(dvec3 &ret, const dvec3 &v0, double v1, const dvec3 &v2)
{
	#ifdef USE_SSE
		__m128d temp = _mm_set1_pd(v1);
		ret.sse.v0 = _mm_add_pd(_mm_mul_pd(v0.sse.v0, temp), v2.sse.v0);
		ret.sse.v1 = _mm_add_pd(_mm_mul_pd(v0.sse.v1, temp), v2.sse.v1);
	#else
		ret.x = v0.x * v1 + v2.x;
		ret.y = v0.y * v1 + v2.y;
		ret.z = v0.z * v1 + v2.z;
	#endif
	return ret;
}
UNIGINE_INLINE dvec3 &mad(dvec3 &ret, const dvec3 &v0, const dvec3 &v1, const dvec3 &v2)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_add_pd(_mm_mul_pd(v0.sse.v0, v1.sse.v0), v2.sse.v0);
		ret.sse.v1 = _mm_add_pd(_mm_mul_pd(v0.sse.v1, v1.sse.v1), v2.sse.v1);
	#else
		ret.x = v0.x * v1.x + v2.x;
		ret.y = v0.y * v1.y + v2.y;
		ret.z = v0.z * v1.z + v2.z;
	#endif
	return ret;
}
UNIGINE_INLINE dvec3 mad(const dvec3 &v0, double v1, const dvec3 &v2)
{
	dvec3 ret;
	return mad(ret, v0, v1, v2);
}
UNIGINE_INLINE dvec3 mad(const dvec3 &v0, const dvec3 &v1, const dvec3 &v2)
{
	dvec3 ret;
	return mad(ret, v0, v1, v2);
}

UNIGINE_INLINE double dot(const dvec2 &v0, const dvec3 &v1)
{
	return v0.x * v1.x + v0.y * v1.y + v1.z;
}
UNIGINE_INLINE double dot(const dvec3 &v0, const dvec2 &v1)
{
	return v0.x * v1.x + v0.y * v1.y + v0.z;
}
UNIGINE_INLINE double dot(const dvec3 &v0, const dvec3 &v1)
{
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

UNIGINE_INLINE dvec3 &min(dvec3 &ret, const dvec3 &v0, const dvec3 &v1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_min_pd(v0.sse.v0, v1.sse.v0);
		ret.sse.v1 = _mm_min_sd(v0.sse.v1, v1.sse.v1);
	#else
		ret.x = min(v0.x, v1.x);
		ret.y = min(v0.y, v1.y);
		ret.z = min(v0.z, v1.z);
	#endif
	return ret;
}
UNIGINE_INLINE dvec3 min(const dvec3 &v0, const dvec3 &v1)
{
	dvec3 ret;
	return min(ret, v0, v1);
}

UNIGINE_INLINE dvec3 &max(dvec3 &ret, const dvec3 &v0, const dvec3 &v1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_max_pd(v0.sse.v0, v1.sse.v0);
		ret.sse.v1 = _mm_max_sd(v0.sse.v1, v1.sse.v1);
	#else
		ret.x = max(v0.x, v1.x);
		ret.y = max(v0.y, v1.y);
		ret.z = max(v0.z, v1.z);
	#endif
	return ret;
}
UNIGINE_INLINE dvec3 max(const dvec3 &v0, const dvec3 &v1)
{
	dvec3 ret;
	return max(ret, v0, v1);
}

UNIGINE_INLINE dvec3 &clamp(dvec3 &ret, const dvec3 &v, const dvec3 &v0, const dvec3 &v1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_min_pd(_mm_max_pd(v.sse.v0, v0.sse.v0), v1.sse.v0);
		ret.sse.v1 = _mm_min_sd(_mm_max_sd(v.sse.v1, v0.sse.v1), v1.sse.v1);
	#else
		ret.x = clamp(v.x, v0.x, v1.x);
		ret.y = clamp(v.y, v0.y, v1.y);
		ret.z = clamp(v.z, v0.z, v1.z);
	#endif
	return ret;
}
UNIGINE_INLINE dvec3 clamp(const dvec3 &v, const dvec3 &v0, const dvec3 &v1)
{
	dvec3 ret;
	return clamp(ret, v, v0, v1);
}

UNIGINE_INLINE dvec3 &saturate(dvec3 &ret, const dvec3 &v)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_min_pd(_mm_max_pd(v.sse.v0, dvec2_zero.vec), dvec2_one.vec);
		ret.sse.v1 = _mm_min_sd(_mm_max_sd(v.sse.v1, dvec2_zero.vec), dvec2_one.vec);
	#else
		ret.x = saturate(v.x);
		ret.y = saturate(v.y);
		ret.z = saturate(v.z);
	#endif
	return ret;
}
UNIGINE_INLINE dvec3 saturate(const dvec3 &v)
{
	dvec3 ret;
	return saturate(ret, v);
}

UNIGINE_INLINE dvec3 &lerp(dvec3 &ret, const dvec3 &v0, const dvec3 &v1, double k)
{
	#ifdef USE_SSE
		__m128d temp = _mm_set1_pd(k);
		ret.sse.v0 = _mm_add_pd(v0.sse.v0, _mm_mul_pd(_mm_sub_pd(v1.sse.v0, v0.sse.v0), temp));
		ret.sse.v1 = _mm_add_pd(v0.sse.v1, _mm_mul_pd(_mm_sub_pd(v1.sse.v1, v0.sse.v1), temp));
	#else
		ret.x = lerp(v0.x, v1.x, k);
		ret.y = lerp(v0.y, v1.y, k);
		ret.z = lerp(v0.z, v1.z, k);
	#endif
	return ret;
}
UNIGINE_INLINE dvec3 lerp(const dvec3 &v0, const dvec3 &v1, double k)
{
	dvec3 ret;
	return lerp(ret, v0, v1, k);
}

UNIGINE_INLINE dvec3 &cross(dvec3 &ret, const dvec3 &v0, const dvec3 &v1)
{
	ret.x = v0.y * v1.z - v0.z * v1.y;
	ret.y = v0.z * v1.x - v0.x * v1.z;
	ret.z = v0.x * v1.y - v0.y * v1.x;
	return ret;
}
UNIGINE_INLINE dvec3 cross(const dvec3 &v0, const dvec3 &v1)
{
	dvec3 ret;
	return cross(ret, v0, v1);
}

UNIGINE_INLINE dvec3 &reflect(dvec3 &ret, const dvec3 &v0, const dvec3 &v1)
{
	double k = 2.0 * (v0.x * v1.x + v0.y * v1.y + v0.z * v1.z);
	#ifdef USE_SSE
		__m128d temp = _mm_set1_pd(k);
		ret.sse.v0 = _mm_sub_pd(v0.sse.v0, _mm_mul_pd(v1.sse.v0, temp));
		ret.sse.v1 = _mm_sub_pd(v0.sse.v1, _mm_mul_pd(v1.sse.v1, temp));
	#else
		ret.x = v0.x - v1.x * k;
		ret.y = v0.y - v1.y * k;
		ret.z = v0.z - v1.z * k;
	#endif
	return ret;
}
UNIGINE_INLINE dvec3 reflect(const dvec3 &v0, const dvec3 &v1)
{
	dvec3 ret;
	return reflect(ret, v0, v1);
}

UNIGINE_INLINE dvec3 round(const dvec3 &v)
{
	return {round(v.x), round(v.y), round(v.z)};
}
UNIGINE_INLINE dvec3 floor(const dvec3 &v)
{
	#ifdef USE_SSE
		return dvec3(_mm_floor_pd(v.sse.v0), _mm_floor_pd(v.sse.v1));
	#else
		return {floor(v.x), floor(v.y), floor(v.z)};
	#endif
}
UNIGINE_INLINE dvec3 ceil(const dvec3 &v)
{
	#ifdef USE_SSE
		return dvec3(_mm_ceil_pd(v.sse.v0), _mm_ceil_pd(v.sse.v1));
	#else
		return {ceil(v.x), ceil(v.y), ceil(v.z)};
	#endif
}

UNIGINE_INLINE dvec3 frac(const dvec3 &v) { return {frac(v.x), frac(v.y), frac(v.z)}; }
UNIGINE_INLINE dvec3 abs(const dvec3 &v) { return {abs(v.x), abs(v.y), abs(v.z)}; }

UNIGINE_INLINE dvec3 inverseLerp(const dvec3 &v0, const dvec3 &v1, const dvec3 &v)
{
	return saturate((v - v0) / (v1 - v0));
}

UNIGINE_INLINE bool areCollinear(const dvec3 &v0, const dvec3 &v1) { return length(cross(v0, v1)) < 1e-6f; }

}
}
