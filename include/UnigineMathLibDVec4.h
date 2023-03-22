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
#include "UnigineMathLibDVec2.h"
#include "UnigineMathLibDVec3.h"

namespace Unigine
{
namespace Math
{

struct alignas(16) dvec4
{
	UNIGINE_INLINE constexpr dvec4(double v, ConstexprTag): x(v), y(v), z(v), w(v) {}
	UNIGINE_INLINE constexpr dvec4(double x, double y, double z, double w, ConstexprTag) : x(x), y(y), z(z), w(w) {}

	UNIGINE_INLINE dvec4()
		: x(0.0)
		, y(0.0)
		, z(0.0)
		, w(0.0)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit dvec4(double v)
		: x(v)
		, y(v)
		, z(v)
		, w(v)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE dvec4(double x, double y, double z = 0.0, double w = 0.0)
		: x(x)
		, y(y)
		, z(z)
		, w(w)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit dvec4(const double *v)
		: x(v[0])
		, y(v[1])
		, z(v[2])
		, w(v[3])
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE dvec4(const dvec4 &v)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(v);
	}

	UNIGINE_INLINE explicit dvec4(const dvec2 &v)
		: x(v.x)
		, y(v.y)
		, z(0.0)
		, w(1.0)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE dvec4(const dvec2 &v, double z, double w)
		: x(v.x)
		, y(v.y)
		, z(z)
		, w(w)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE dvec4(const dvec2 &v0, const dvec2 &v1)
		: x(v0.x)
		, y(v0.y)
		, z(v1.x)
		, w(v1.y)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}

	UNIGINE_INLINE explicit dvec4(const dvec3 &v)
		: x(v.x)
		, y(v.y)
		, z(v.z)
		, w(1.0)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE dvec4(const dvec3 &v, double w)
		: x(v.x)
		, y(v.y)
		, z(v.z)
		, w(w)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}

	UNIGINE_INLINE explicit dvec4(const vec4 &v)
		: x(v.x)
		, y(v.y)
		, z(v.z)
		, w(v.w)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	
	#ifdef USE_SSE
		UNIGINE_INLINE dvec4(const __m128d &v0, const __m128d &v1)
		{
			UNIGINE_ASSERT_ALIGNED16(this);
			sse.v0 = v0;
			sse.v1 = v1;
		}
	#endif

	UNIGINE_INLINE explicit dvec4(const hvec4 &v);
	UNIGINE_INLINE explicit dvec4(const ivec4 &v);
	UNIGINE_INLINE explicit dvec4(const bvec4 &v);
	UNIGINE_INLINE explicit dvec4(const svec4 &v);


	UNIGINE_INLINE void set(double v) { x = v; y = v; z = v; w = v; }
	UNIGINE_INLINE void set(double x_, double y_, double z_, double w_) { x = x_; y = y_; z = z_; w = w_; }
	UNIGINE_INLINE void set(const dvec2 &v, double z_ = 0.0, double w_ = 1.0)
	{
		x = v.x;
		y = v.y;
		z = z_;
		w = w_;
	}
	UNIGINE_INLINE void set(const dvec3 &v, double w_ = 1.0)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = w_;
	}
	UNIGINE_INLINE void set(const dvec4 &v)
	{
		sse.v0 = v.sse.v0;
		sse.v1 = v.sse.v1;
	}
	UNIGINE_INLINE void set(const double *v)
	{
		x = v[0];
		y = v[1];
		z = v[2];
		w = v[3];
	}

	UNIGINE_INLINE void get(double *v) const
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
		v[3] = w;
	}
	UNIGINE_INLINE double *get() { return v; }
	UNIGINE_INLINE const double *get() const { return v; }

	UNIGINE_INLINE operator double *() { return v; }
	UNIGINE_INLINE operator const double *() const { return v; }
	UNIGINE_INLINE operator void *() { return v; }
	UNIGINE_INLINE operator const void *() const { return v; }

	UNIGINE_INLINE double &operator[](int i)
	{
		assert((unsigned int)i < 4 && "dvec4::operator[](): bad index");
		return v[i];
	}
	UNIGINE_INLINE double operator[](int i) const
	{
		assert((unsigned int)i < 4 && "dvec4::operator[](): bad index");
		return v[i];
	}

	
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
			w *= v;
		#endif
	}
	UNIGINE_INLINE void mul(const dvec4 &v)
	{
		#ifdef USE_SSE
			sse.v0 = _mm_mul_pd(sse.v0, v.sse.v0);
			sse.v1 = _mm_mul_pd(sse.v1, v.sse.v1);
		#else
			x *= v.x;
			y *= v.y;
			z *= v.z;
			w *= v.w;
		#endif
	}
	UNIGINE_INLINE void mul3(double v)
	{
		x *= v;
		y *= v;
		z *= v;
	}
	UNIGINE_INLINE dvec4 &operator*=(double v) { mul(v); return *this; }
	UNIGINE_INLINE dvec4 &operator*=(const dvec4 &v) { mul(v); return *this; }

	UNIGINE_INLINE void div(double v) { mul(rcp(v)); }
	UNIGINE_INLINE void div(const dvec4 &v)
	{
		#ifdef USE_SSE
			sse.v0 = _mm_div_pd(sse.v0, v.sse.v0);
			sse.v1 = _mm_div_pd(sse.v1, v.sse.v1);
		#else
			x /= v.x;
			y /= v.y;
			z /= v.z;
			w /= v.w;
		#endif
	}
	UNIGINE_INLINE dvec4 &operator/=(double v) { div(v); return *this; }
	UNIGINE_INLINE dvec4 &operator/=(const dvec4 &v) { div(v); return *this; }

	UNIGINE_INLINE double sum() const { return x + y + z + w; }
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
			w += v;
		#endif
	}
	UNIGINE_INLINE void add(const dvec4 &v)
	{
		#ifdef USE_SSE
			sse.v0 = _mm_add_pd(sse.v0, v.sse.v0);
			sse.v1 = _mm_add_pd(sse.v1, v.sse.v1);
		#else
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
		#endif
	}
	UNIGINE_INLINE dvec4 &operator+=(double v) { add(v); return *this; }
	UNIGINE_INLINE dvec4 &operator+=(const dvec4 &v) { add(v); return *this; }

	UNIGINE_INLINE void sub(double v)
	{
		#ifdef USE_SSE
			__m128d temp = _mm_set1_pd(v);
			sse.v0 = _mm_sub_pd(sse.v0, temp);
			sse.v1 = _mm_sub_pd(sse.v1, temp);
		#else
			x += v;
			y += v;
			z += v;
			w += v;
		#endif
	}
	UNIGINE_INLINE void sub(const dvec4 &v)
	{
		#ifdef USE_SSE
			sse.v0 = _mm_sub_pd(sse.v0, v.sse.v0);
			sse.v1 = _mm_sub_pd(sse.v1, v.sse.v1);
		#else
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
		#endif
	}
	UNIGINE_INLINE dvec4 &operator-=(double v) { sub(v); return *this; }
	UNIGINE_INLINE dvec4 &operator-=(const dvec4 &v) { sub(v); return *this; }
	
	UNIGINE_INLINE dvec4 &operator=(const dvec4 &v) { set(v); return *this; }
	UNIGINE_INLINE dvec4 operator-() const { return dvec4(-x, -y, -z, -w); }

	UNIGINE_INLINE double length2() const { return x * x + y * y + z * z + w * w; }
	UNIGINE_INLINE double length() const { return sqrt(length2()); }
	UNIGINE_INLINE double iLength() const { return rsqrt(length2()); }

	UNIGINE_INLINE dvec4 &normalize() { mul(iLength()); return *this; }
	UNIGINE_INLINE dvec4 &normalizeValid()
	{
		double length = length2();
		if (length != 0.0f)
			mul(rsqrt(length));
		return *this;
	}

	UNIGINE_INLINE dvec4 &normalize3()
	{
		mul3(xyz.iLength());
		return *this;
	}
	UNIGINE_INLINE dvec4 &normalizeValid3()
	{
		double length = xyz.length2();
		if (length != 0.0f)
			mul3(rsqrt(length));
		return *this;
	}

	UNIGINE_INLINE dvec4 &abs()
	{
		x = Math::abs(x);
		y = Math::abs(y);
		z = Math::abs(z);
		w = Math::abs(w);
		return *this;
	}

	UNIGINE_INLINE double maxXY() const { return Math::max(x, y); }
	UNIGINE_INLINE double maxXYZ() const { return Math::max(maxXY(), z); }
	UNIGINE_INLINE double max() const { return Math::max(maxXYZ(), w); }

	UNIGINE_INLINE unsigned int hash() const { return hashCast64To32(hashCombine(hashCombine(hashCombine(hashInteger(x), y), z), w)); }

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
			double x, y, z, w;
		};
		double v[4];

		const dvec3 xyz;
		dvec2 xy;

		struct
		{
			dvec2 xy;
			dvec2 zw;
		} swizzle;

		#ifdef USE_SSE
			sse_data sse; // DO NOT TOUCH UNLESS ABSOLUTELY SURE
		#endif
	};
};

constexpr dvec4 dvec4_zero(0.0, ConstexprTag{});
constexpr dvec4 dvec4_one(1.0, ConstexprTag{});
constexpr dvec4 dvec4_eps(Consts::EPS, ConstexprTag{});
constexpr dvec4 dvec4_inf(Consts::INF, ConstexprTag{});

UNIGINE_INLINE double length(const dvec4 &v) { return v.length(); }
UNIGINE_INLINE double length2(const dvec4 &v) { return v.length2(); }

UNIGINE_INLINE dvec4 normalize(const dvec4 &v)
{
	dvec4 ret = v;
	return ret.normalize();
}
UNIGINE_INLINE dvec4 normalizeValid(const dvec4 &v)
{
	dvec4 ret = v;
	return ret.normalizeValid();
}

UNIGINE_INLINE dvec4 normalize3(const dvec4 &v)
{
	dvec4 ret = v;
	return ret.normalize3();
}
UNIGINE_INLINE dvec4 normalizeValid3(const dvec4 &v)
{
	dvec4 ret = v;
	return ret.normalizeValid3();
}

UNIGINE_INLINE int compare(const dvec4 &v0, const dvec4 &v1)
{
	return (compare(v0.x, v1.x) && compare(v0.y, v1.y) && compare(v0.z, v1.z) && compare(v0.w, v1.w));
}
UNIGINE_INLINE int compare(const dvec4 &v0, const dvec4 &v1, double epsilon)
{
	return (compare(v0.x, v1.x, epsilon) && compare(v0.y, v1.y, epsilon) && compare(v0.z, v1.z, epsilon) && compare(v0.w, v1.w, epsilon));
}
UNIGINE_INLINE int operator==(const dvec4 &v0, const dvec4 &v1) { return compare(v0, v1); }
UNIGINE_INLINE int operator!=(const dvec4 &v0, const dvec4 &v1) { return !compare(v0, v1); }

UNIGINE_INLINE dvec4 &mul(dvec4 &ret, const dvec4 &v0, double v1)
{
	#ifdef USE_SSE
		__m128d temp = _mm_set1_pd(v1);
		ret.sse.v0 = _mm_mul_pd(v0.sse.v0, temp);
		ret.sse.v1 = _mm_mul_pd(v0.sse.v1, temp);
	#else
		ret.x = v0.x * v1;
		ret.y = v0.y * v1;
		ret.z = v0.z * v1;
		ret.w = v0.w * v1;
	#endif
	return ret;
}
UNIGINE_INLINE dvec4 &mul(dvec4 &ret, const dvec4 &v0, const dvec4 &v1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_mul_pd(v0.sse.v0, v1.sse.v0);
		ret.sse.v1 = _mm_mul_pd(v0.sse.v1, v1.sse.v1);
	#else
		ret.x = v0.x * v1.x;
		ret.y = v0.y * v1.y;
		ret.z = v0.z * v1.z;
		ret.w = v0.z * v1.w;
	#endif
	return ret;
}
UNIGINE_INLINE dvec4 mul(const dvec4 &v0, double v1)
{
	dvec4 ret;
	return mul(ret, v0, v1);
}
UNIGINE_INLINE dvec4 mul(const dvec4 &v0, const dvec4 &v1)
{
	dvec4 ret;
	return mul(ret, v0, v1);
}
UNIGINE_INLINE dvec4 operator*(const dvec4 &v0, double v1) { return mul(v0, v1); }
UNIGINE_INLINE dvec4 operator*(const dvec4 &v0, const dvec4 &v1) { return mul(v0, v1); }

UNIGINE_INLINE dvec4 &div(dvec4 &ret, const dvec4 &v0, double v1) { return mul(ret, v0, rcp(v1)); }
UNIGINE_INLINE dvec4 &div(dvec4 &ret, const dvec4 &v0, const dvec4 &v1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_div_pd(v0.sse.v0, v1.sse.v0);
		ret.sse.v1 = _mm_div_pd(v0.sse.v1, v1.sse.v1);
	#else
		ret.x = v0.x / v1.x;
		ret.y = v0.y / v1.y;
		ret.z = v0.z / v1.z;
		ret.w = v0.w / v1.w;
	#endif
	return ret;
}
UNIGINE_INLINE dvec4 div(const dvec4 &v0, double v1)
{
	dvec4 ret;
	return div(ret, v0, v1);
}
UNIGINE_INLINE dvec4 div(const dvec4 &v0, const dvec4 &v1)
{
	dvec4 ret;
	return div(ret, v0, v1);
}
UNIGINE_INLINE dvec4 operator/(const dvec4 &v0, double v1) { return div(v0, v1); }
UNIGINE_INLINE dvec4 operator/(const dvec4 &v0, const dvec4 &v1) { return div(v0, v1); }

UNIGINE_INLINE dvec4 &add(dvec4 &ret, const dvec4 &v0, double v1)
{
	#ifdef USE_SSE
		__m128d temp = _mm_set1_pd(v1);
		ret.sse.v0 = _mm_add_pd(v0.sse.v0, temp);
		ret.sse.v1 = _mm_add_pd(v0.sse.v1, temp);
	#else
		ret.x = v0.x + v1;
		ret.y = v0.y + v1;
		ret.z = v0.z + v1;
		ret.w = v0.w + v1;
	#endif
	return ret;
}
UNIGINE_INLINE dvec4 &add(dvec4 &ret, const dvec4 &v0, const dvec4 &v1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_add_pd(v0.sse.v0, v1.sse.v0);
		ret.sse.v1 = _mm_add_pd(v0.sse.v1, v1.sse.v1);
	#else
		ret.x = v0.x + v1.x;
		ret.y = v0.y + v1.y;
		ret.z = v0.z + v1.z;
		ret.w = v0.w + v1.w;
	#endif
	return ret;
}
UNIGINE_INLINE dvec4 add(const dvec4 &v0, double v1)
{
	dvec4 ret;
	return add(ret, v0, v1);
}
UNIGINE_INLINE dvec4 add(const dvec4 &v0, const dvec4 &v1)
{
	dvec4 ret;
	return add(ret, v0, v1);
}
UNIGINE_INLINE dvec4 operator+(const dvec4 &v0, double v1) { return add(v0, v1); }
UNIGINE_INLINE dvec4 operator+(const dvec4 &v0, const dvec4 &v1) { return add(v0, v1); }

UNIGINE_INLINE dvec4 &sub(dvec4 &ret, const dvec4 &v0, double v1)
{
	#ifdef USE_SSE
		__m128d temp = _mm_set1_pd(v1);
		ret.sse.v0 = _mm_sub_pd(v0.sse.v0, temp);
		ret.sse.v1 = _mm_sub_pd(v0.sse.v1, temp);
	#else
		ret.x = v0.x - v1;
		ret.y = v0.y - v1;
		ret.z = v0.z - v1;
		ret.w = v0.w - v1;
	#endif
	return ret;
}
UNIGINE_INLINE dvec4 &sub(dvec4 &ret, const dvec4 &v0, const dvec4 &v1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_sub_pd(v0.sse.v0, v1.sse.v0);
		ret.sse.v1 = _mm_sub_pd(v0.sse.v1, v1.sse.v1);
	#else
		ret.x = v0.x - v1.x;
		ret.y = v0.y - v1.y;
		ret.z = v0.z - v1.z;
		ret.w = v0.w - v1.w;
	#endif
	return ret;
}
UNIGINE_INLINE dvec4 sub(const dvec4 &v0, double v1)
{
	dvec4 ret;
	return sub(ret, v0, v1);
}
UNIGINE_INLINE dvec4 sub(const dvec4 &v0, const dvec4 &v1)
{
	dvec4 ret;
	return sub(ret, v0, v1);
}
UNIGINE_INLINE dvec4 operator-(const dvec4 &v0, double v1) { return sub(v0, v1); }
UNIGINE_INLINE dvec4 operator-(const dvec4 &v0, const dvec4 &v1) { return sub(v0, v1); }

UNIGINE_INLINE dvec4 &mad(dvec4 &ret, const dvec4 &v0, double v1, const dvec4 &v2)
{
	#ifdef USE_SSE
		__m128d temp = _mm_set1_pd(v1);
		ret.sse.v0 = _mm_add_pd(_mm_mul_pd(v0.sse.v0, temp), v2.sse.v0);
		ret.sse.v1 = _mm_add_pd(_mm_mul_pd(v0.sse.v1, temp), v2.sse.v1);
	#else
		ret.x = v0.x * v1 + v2.x;
		ret.y = v0.y * v1 + v2.y;
		ret.z = v0.z * v1 + v2.z;
		ret.w = v0.w * v1 + v2.w;
	#endif
	return ret;
}
UNIGINE_INLINE dvec4 &mad(dvec4 &ret, const dvec4 &v0, const dvec4 &v1, const dvec4 &v2)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_add_pd(_mm_mul_pd(v0.sse.v0, v1.sse.v0), v2.sse.v0);
		ret.sse.v1 = _mm_add_pd(_mm_mul_pd(v0.sse.v1, v1.sse.v1), v2.sse.v1);
	#else
		ret.x = v0.x * v1.x + v2.x;
		ret.y = v0.y * v1.y + v2.y;
		ret.z = v0.z * v1.z + v2.z;
		ret.w = v0.w * v1.w + v2.w;
	#endif
	return ret;
}
UNIGINE_INLINE dvec4 mad(const dvec4 &v0, double v1, const dvec4 &v2)
{
	dvec4 ret;
	return mad(ret, v0, v1, v2);
}
UNIGINE_INLINE dvec4 mad(const dvec4 &v0, const dvec4 &v1, const dvec4 &v2)
{
	dvec4 ret;
	return mad(ret, v0, v1, v2);
}

UNIGINE_INLINE double dot(const dvec3 &v0, const dvec4 &v1)
{
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v1.w;
}
UNIGINE_INLINE double dot(const dvec4 &v0, const dvec3 &v1)
{
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w;
}
UNIGINE_INLINE double dot(const dvec4 &v0, const dvec4 &v1)
{
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
}

UNIGINE_INLINE double dot3(const dvec3 &v0, const dvec4 &v1) { return dot(v0, v1.xyz); }
UNIGINE_INLINE double dot3(const dvec4 &v0, const dvec3 &v1) { return dot(v0.xyz, v1); }
UNIGINE_INLINE double dot3(const dvec4 &v0, const dvec4 &v1) { return dot(v0.xyz, v1.xyz); }

UNIGINE_INLINE dvec4 &min(dvec4 &ret, const dvec4 &v0, const dvec4 &v1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_min_pd(v0.sse.v0, v1.sse.v0);
		ret.sse.v1 = _mm_min_pd(v0.sse.v1, v1.sse.v1);
	#else
		ret.x = min(v0.x, v1.x);
		ret.y = min(v0.y, v1.y);
		ret.z = min(v0.z, v1.z);
		ret.w = min(v0.w, v1.w);
	#endif
	return ret;
}
UNIGINE_INLINE dvec4 min(const dvec4 &v0, const dvec4 &v1)
{
	dvec4 ret;
	return min(ret, v0, v1);
}

UNIGINE_INLINE dvec4 &max(dvec4 &ret, const dvec4 &v0, const dvec4 &v1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_max_pd(v0.sse.v0, v1.sse.v0);
		ret.sse.v1 = _mm_max_pd(v0.sse.v1, v1.sse.v1);
	#else
		ret.x = max(v0.x, v1.x);
		ret.y = max(v0.y, v1.y);
		ret.z = max(v0.z, v1.z);
		ret.w = max(v0.w, v1.w);
	#endif
	return ret;
}
UNIGINE_INLINE dvec4 max(const dvec4 &v0, const dvec4 &v1)
{
	dvec4 ret;
	return max(ret, v0, v1);
}

UNIGINE_INLINE dvec4 &clamp(dvec4 &ret, const dvec4 &v, const dvec4 &v0, const dvec4 &v1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_min_pd(_mm_max_pd(v.sse.v0, v0.sse.v0), v1.sse.v0);
		ret.sse.v1 = _mm_min_pd(_mm_max_pd(v.sse.v1, v0.sse.v1), v1.sse.v1);
	#else
		ret.x = clamp(v.x, v0.x, v1.x);
		ret.y = clamp(v.y, v0.y, v1.y);
		ret.z = clamp(v.z, v0.z, v1.z);
		ret.w = clamp(v.w, v0.w, v1.w);
	#endif
	return ret;
}
UNIGINE_INLINE dvec4 clamp(const dvec4 &v, const dvec4 &v0, const dvec4 &v1)
{
	dvec4 ret;
	return clamp(ret, v, v0, v1);
}

UNIGINE_INLINE dvec4 &saturate(dvec4 &ret, const dvec4 &v)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_min_pd(_mm_max_pd(v.sse.v0, dvec4_zero.sse.v0), dvec4_one.sse.v0);
		ret.sse.v1 = _mm_min_pd(_mm_max_pd(v.sse.v1, dvec4_zero.sse.v1), dvec4_one.sse.v1);
	#else
		ret.x = saturate(v.x);
		ret.y = saturate(v.y);
		ret.z = saturate(v.z);
		ret.w = saturate(v.w);
	#endif
	return ret;
}
UNIGINE_INLINE dvec4 saturate(const dvec4 &v)
{
	dvec4 ret;
	return saturate(ret, v);
}

UNIGINE_INLINE dvec4 &lerp(dvec4 &ret, const dvec4 &v0, const dvec4 &v1, double k)
{
	#ifdef USE_SSE
		__m128d temp = _mm_set1_pd(k);
		ret.sse.v0 = _mm_add_pd(v0.sse.v0, _mm_mul_pd(_mm_sub_pd(v1.sse.v0, v0.sse.v0), temp));
		ret.sse.v1 = _mm_add_pd(v0.sse.v1, _mm_mul_pd(_mm_sub_pd(v1.sse.v1, v0.sse.v1), temp));
	#else
		ret.x = lerp(v0.x, v1.x, k);
		ret.y = lerp(v0.y, v1.y, k);
		ret.z = lerp(v0.z, v1.z, k);
		ret.w = lerp(v0.w, v1.w, k);
	#endif
	return ret;
}
UNIGINE_INLINE dvec4 lerp(const dvec4 &v0, const dvec4 &v1, double k)
{
	dvec4 ret;
	return lerp(ret, v0, v1, k);
}

UNIGINE_INLINE dvec4 &lerp3(dvec4 &ret, const dvec4 &v0, const dvec4 &v1, const dvec4 &v2, float k)
{
	if (k < 0.5f)
		lerp(ret, v0, v1, k * 2.0);
	else
		lerp(ret, v1, v2, k * 2.0 - 1.0);
	return ret;
}
UNIGINE_INLINE dvec4 lerp3(const dvec4 &v0, const dvec4 &v1, const dvec4 &v2, float k)
{
	dvec4 ret;
	return lerp3(ret, v0, v1, v2, k);
}

UNIGINE_INLINE dvec4 &cross(dvec4 &ret, const dvec3 &v0, const dvec3 &v1)
{
	ret.x = v0.y * v1.z - v0.z * v1.y;
	ret.y = v0.z * v1.x - v0.x * v1.z;
	ret.z = v0.x * v1.y - v0.y * v1.x;
	return ret;
}

UNIGINE_INLINE dvec4 round(const dvec4 &v) { return {round(v.x), round(v.y), round(v.z), round(v.w)}; }
UNIGINE_INLINE dvec4 floor(const dvec4 &v)
{
	#ifdef USE_SSE
		return dvec4(_mm_floor_pd(v.sse.v0), _mm_floor_pd(v.sse.v1));
	#else
		return {floor(v.x), floor(v.y), floor(v.z), floor(v.w)};
	#endif
}
UNIGINE_INLINE dvec4 ceil(const dvec4 &v)
{
	#ifdef USE_SSE
		return dvec4(_mm_ceil_pd(v.sse.v0), _mm_ceil_pd(v.sse.v1));
	#else
		return {ceil(v.x), ceil(v.y), ceil(v.z), ceil(v.w)};
	#endif
}

UNIGINE_INLINE dvec4 abs(const dvec4 &v)
{
	return {abs(v.x), abs(v.y), abs(v.z), abs(v.w)};
}
UNIGINE_INLINE dvec4 frac(const dvec4 &v)
{
	return {frac(v.x), frac(v.y), frac(v.z), frac(v.w)};
}

UNIGINE_INLINE dvec4 inverseLerp(const dvec4 &v0, const dvec4 &v1, const dvec4 &v)
{
	return saturate((v - v0) / (v1 - v0));
}

UNIGINE_INLINE double changeRange(double value, const dvec4 &range)
{
	return saturate(value - range.x) / range.y * range.w + range.z;
}

}
}