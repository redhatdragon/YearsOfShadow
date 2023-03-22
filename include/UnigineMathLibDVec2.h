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

namespace Unigine
{
namespace Math
{

struct alignas(16) dvec2
{
	UNIGINE_INLINE constexpr dvec2(double v, ConstexprTag): x(v), y(v) {}
	UNIGINE_INLINE constexpr dvec2(double x, double y, ConstexprTag) : x(x), y(y) {}

	UNIGINE_INLINE dvec2()
		: x(0.0)
		, y(0.0)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE dvec2(double x, double y)
		: x(x)
		, y(y)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit dvec2(double v)
		: x(v)
		, y(v)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit dvec2(const double *v)
		: x(v[0])
		, y(v[1])
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE dvec2(const dvec2 &v)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(v);
	}
	UNIGINE_INLINE explicit dvec2(const vec2 &v)
		: x(v.x)
		, y(v.y)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}

	UNIGINE_INLINE explicit dvec2(const dvec3 &v);
	UNIGINE_INLINE explicit dvec2(const dvec4 &v);
	UNIGINE_INLINE explicit dvec2(const hvec2 &v);
	UNIGINE_INLINE explicit dvec2(const ivec2 &v);

	#ifdef USE_SSE
		UNIGINE_INLINE dvec2(const __m128d &v): vec(v)
		{
			UNIGINE_ASSERT_ALIGNED16(this);
		}
		UNIGINE_INLINE dvec2 &operator=(const __m128d &val) { vec = val; return *this; }
	#endif

	UNIGINE_INLINE void set(double val) { x = val; y = val; }
	UNIGINE_INLINE void set(double x_, double y_) { x = x_; y = y_; }
	UNIGINE_INLINE void set(const dvec2 &val)
	{
		#ifdef USE_SSE
			vec = val.vec;
		#else
			x = val.x;
			y = val.y;
		#endif
	}
	UNIGINE_INLINE void set(const double *val)
	{
		x = val[0];
		y = val[1];
	}

	UNIGINE_INLINE void get(double *val) const
	{
		val[0] = x;
		val[1] = y;
	}
	UNIGINE_INLINE double *get() { return v; }
	UNIGINE_INLINE const double *get() const { return v; }

	UNIGINE_INLINE operator double *() { return v; }
	UNIGINE_INLINE operator const double *() const { return v; }
	UNIGINE_INLINE operator void *() { return v; }
	UNIGINE_INLINE operator const void *() const { return v; }

	UNIGINE_INLINE double &operator[](int i)
	{
		assert((unsigned int)i < 2 && "dvec2::operator[](): bad index");
		return v[i];
	}
	UNIGINE_INLINE double operator[](int i) const
	{
		assert((unsigned int)i < 2 && "dvec2::operator[](): bad index");
		return v[i];
	}

	UNIGINE_INLINE dvec2 &operator=(const dvec2 &v) { set(v); return *this; }
	UNIGINE_INLINE dvec2 operator-() const { return dvec2(-x, -y); }

	UNIGINE_INLINE void mul(double v)
	{
		#ifdef USE_SSE
			vec = _mm_mul_pd(vec, _mm_set1_pd(v));
		#else
			x *= v;
			y *= v;
		#endif
	}
	UNIGINE_INLINE void mul(const dvec2 &v)
	{
		#ifdef USE_SSE
			vec = _mm_mul_pd(vec, v.vec);
		#else
			x *= v.x;
			y *= v.y;
		#endif
	}
	UNIGINE_INLINE dvec2 &operator*=(double v) { mul(v); return *this; }
	UNIGINE_INLINE dvec2 &operator*=(const dvec2 &v) { mul(v); return *this; }

	UNIGINE_INLINE void div(double v) { mul(Math::rcp(v)); }
	UNIGINE_INLINE void div(const dvec2 &v)
	{
		#ifdef USE_SSE
			vec = _mm_div_pd(vec, v.vec);
		#else
			x /= v.x;
			y /= v.y;
		#endif
	}
	UNIGINE_INLINE dvec2 &operator/=(double v) { div(v); return *this; }
	UNIGINE_INLINE dvec2 &operator/=(const dvec2 &v) { div(v); return *this; }

	UNIGINE_INLINE void sub(double v)
	{
		#ifdef USE_SSE
			vec = _mm_sub_pd(vec, _mm_set1_pd(v));
		#else
			x -= v;
			y -= v;
		#endif
	}
	UNIGINE_INLINE void sub(const dvec2 &v)
	{
		#ifdef USE_SSE
			vec = _mm_sub_pd(vec, v.vec);
		#else
			x -= v.x;
			y -= v.y;
		#endif
	}
	UNIGINE_INLINE dvec2 &operator-=(double v) { sub(v); return *this; }
	UNIGINE_INLINE dvec2 &operator-=(const dvec2 &v) { sub(v); return *this; }


	UNIGINE_INLINE double sum() const { return x + y; }
	UNIGINE_INLINE void add(double v)
	{
		#ifdef USE_SSE
			vec = _mm_add_pd(vec, _mm_set1_pd(v));
		#else
			x += v;
			y += v;
		#endif
	}
	UNIGINE_INLINE void add(const dvec2 &v)
	{
		#ifdef USE_SSE
			vec = _mm_add_pd(vec, v.vec);
		#else
			x += v.x;
			y += v.y;
		#endif
	}
	UNIGINE_INLINE dvec2 &operator+=(double v) { add(v); return *this; }
	UNIGINE_INLINE dvec2 &operator+=(const dvec2 &v) { add(v); return *this; }

	UNIGINE_INLINE double length2() const { return x * x + y * y; }
	UNIGINE_INLINE double length() const { return sqrt(length2()); }
	UNIGINE_INLINE double iLength() const { return Math::rsqrt(length2()); }

	UNIGINE_INLINE dvec2 &normalize()
	{
		mul(iLength());
		return *this;
	}
	UNIGINE_INLINE dvec2 &normalizeValid()
	{
		double length = length2();
		if (length != 0.0)
			mul(Math::rsqrt(length));
		return *this;
	}

	UNIGINE_INLINE double max() const { return Math::max(x, y); }
	UNIGINE_INLINE double min() const { return Math::min(x, y); }
	UNIGINE_INLINE dvec2 frac() const
	{
		return dvec2(Math::frac(x), Math::frac(y));
	}
	UNIGINE_INLINE dvec2 &abs()
	{
		x = Math::abs(x);
		y = Math::abs(y);
		return *this;
	}
	UNIGINE_INLINE ivec2 floor() const;
	UNIGINE_INLINE ivec2 ceil() const;

	UNIGINE_INLINE unsigned int hash() const { return hashCast64To32(hashCombine(hashInteger(x), y)); }

	union
	{
		struct
		{
			double x, y;
		};
		double v[2];
		#ifdef USE_SSE
			__m128d vec; // DO NOT TOUCH UNLESS ABSOLUTELY SURE
		#endif
	};
};

constexpr dvec2 dvec2_zero(0.0, ConstexprTag{});
constexpr dvec2 dvec2_one(1.0, ConstexprTag{});
constexpr dvec2 dvec2_half(0.5, ConstexprTag{});
constexpr dvec2 dvec2_eps(Consts::EPS, ConstexprTag{});
constexpr dvec2 dvec2_inf(Consts::INF, ConstexprTag{});

UNIGINE_INLINE double length(const dvec2 &v) { return v.length(); }
UNIGINE_INLINE double length2(const dvec2 &v) { return v.length2(); }

UNIGINE_INLINE dvec2 normalize(const dvec2 &v)
{
	dvec2 ret = v;
	return ret.normalize();
}
UNIGINE_INLINE dvec2 normalizeValid(const dvec2 &v)
{
	dvec2 ret = v;
	return ret.normalizeValid();
}

UNIGINE_INLINE int compare(const dvec2 &v0, const dvec2 &v1)
{
	return (compare(v0.x, v1.x) && compare(v0.y, v1.y));
}
UNIGINE_INLINE int compare(const dvec2 &v0, const dvec2 &v1, double eps)
{
	return (compare(v0.x, v1.x, eps) && compare(v0.y, v1.y, eps));
}
UNIGINE_INLINE int operator==(const dvec2 &v0, const dvec2 &v1) { return compare(v0, v1); }
UNIGINE_INLINE int operator!=(const dvec2 &v0, const dvec2 &v1) { return !compare(v0, v1); }

UNIGINE_INLINE int operator<(const dvec2 &v0, const dvec2 &v1) { return v0.x < v1.x &&v0.y < v1.y; }
UNIGINE_INLINE int operator>(const dvec2 &v0, const dvec2 &v1) { return v0.x > v1.x && v0.y > v1.y; }

UNIGINE_INLINE int operator>=(const dvec2 &v0, const dvec2 &v1) { return v0.x >= v1.x && v0.y >= v1.y; }
UNIGINE_INLINE int operator<=(const dvec2 &v0, const dvec2 &v1) { return v0.x <= v1.x && v0.y <= v1.y; }

UNIGINE_INLINE dvec2 &mul(dvec2 &ret, const dvec2 &v0, double v1)
{
	#ifdef USE_SSE
		ret = _mm_mul_pd(v0.vec, _mm_set1_pd(v1));
	#else
		ret.x = v0.x * v1;
		ret.y = v0.y * v1;
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 &mul(dvec2 &ret, const dvec2 &v0, const dvec2 &v1)
{
	#ifdef USE_SSE
		ret = _mm_mul_pd(v0.vec, v1.vec);
	#else
		ret.x = v0.x * v1;
		ret.y = v0.y * v1;
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 mul(const dvec2 &v0, double v1)
{
	dvec2 ret;
	return mul(ret, v0, v1);
}
UNIGINE_INLINE dvec2 mul(const dvec2 &v0, const dvec2 &v1)
{
	dvec2 ret;
	return mul(ret, v0, v1);
}
UNIGINE_INLINE dvec2 operator*(const dvec2 &v0, double v1) { return mul(v0, v1); }
UNIGINE_INLINE dvec2 operator*(const dvec2 &v0, const dvec2 &v1) { return mul(v0, v1); }

UNIGINE_INLINE dvec2 &div(dvec2 &ret, const dvec2 &v0, double v1) { mul(ret, v0, rcp(v1)); return ret; }
UNIGINE_INLINE dvec2 &div(dvec2 &ret, const dvec2 &v0, const dvec2 &v1)
{
	#ifdef USE_SSE
		ret = _mm_div_pd(v0.vec, v1.vec);
	#else
		ret.x = v0.x / v1;
		ret.y = v0.y / v1;
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 div(const dvec2 &v0, double v1)
{
	dvec2 ret;
	return div(ret, v0, v1);
}
UNIGINE_INLINE dvec2 div(const dvec2 &v0, const dvec2 &v1)
{
	dvec2 ret;
	return div(ret, v0, v1);
}
UNIGINE_INLINE dvec2 operator/(const dvec2 &v0, double v1) { return div(v0, v1); }
UNIGINE_INLINE dvec2 operator/(const dvec2 &v0, const dvec2 &v1) { return div(v0, v1); }

UNIGINE_INLINE dvec2 &add(dvec2 &ret, const dvec2 &v0, double v1)
{
	#ifdef USE_SSE
		ret = _mm_add_pd(v0.vec, _mm_set1_pd(v1));
	#else
		ret.x = v0.x + v1;
		ret.y = v0.y + v1;
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 &add(dvec2 &ret, const dvec2 &v0, const dvec2 &v1)
{
	#ifdef USE_SSE
		ret = _mm_add_pd(v0.vec, v1.vec);
	#else
		ret.x = v0.x + v1;
		ret.y = v0.y + v1;
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 add(const dvec2 &v0, double v1)
{
	dvec2 ret;
	return add(ret, v0, v1);
}
UNIGINE_INLINE dvec2 add(const dvec2 &v0, const dvec2 &v1)
{
	dvec2 ret;
	return add(ret, v0, v1);
}
UNIGINE_INLINE dvec2 operator+(const dvec2 &v0, double v1) { return add(v0, v1); }
UNIGINE_INLINE dvec2 operator+(const dvec2 &v0, const dvec2 &v1) { return add(v0, v1); }

UNIGINE_INLINE dvec2 &sub(dvec2 &ret, const dvec2 &v0, double v1)
{
	#ifdef USE_SSE
		ret = _mm_sub_pd(v0.vec, _mm_set1_pd(v1));
	#else
		ret.x = v0.x - v1;
		ret.y = v0.y - v1;
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 &sub(dvec2 &ret, const dvec2 &v0, const dvec2 &v1)
{
	#ifdef USE_SSE
		ret = _mm_sub_pd(v0.vec, v1.vec);
	#else
		ret.x = v0.x - v1;
		ret.y = v0.y - v1;
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 sub(const dvec2 &v0, double v1)
{
	dvec2 ret;
	return sub(ret, v0, v1);
}
UNIGINE_INLINE dvec2 sub(const dvec2 &v0, const dvec2 &v1)
{
	dvec2 ret;
	return sub(ret, v0, v1);
}
UNIGINE_INLINE dvec2 operator-(const dvec2 &v0, double v1) { return sub(v0, v1); }
UNIGINE_INLINE dvec2 operator-(const dvec2 &v0, const dvec2 &v1) { return sub(v0, v1); }

UNIGINE_INLINE dvec2 &mad(dvec2 &ret, const dvec2 &v0, double v1, const dvec2 &v2)
{
	#ifdef USE_SSE
		ret = _mm_add_pd(_mm_mul_pd(v0.vec, _mm_set1_pd(v1)), v2.vec);
	#else
		ret.x = v0.x * v1 + v2.x;
		ret.y = v0.y * v1 + v2.y;
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 &mad(dvec2 &ret, const dvec2 &v0, const dvec2 &v1, const dvec2 &v2)
{
	#ifdef USE_SSE
		ret = _mm_add_pd(_mm_mul_pd(v0.vec, v1.vec), v2.vec);
	#else
		ret.x = v0.x * v1 + v2.x;
		ret.y = v0.y * v1 + v2.y;
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 mad(const dvec2 &v0, double v1, const dvec2 &v2)
{
	dvec2 ret;
	return mad(ret, v0, v1, v2);
}
UNIGINE_INLINE dvec2 mad(const dvec2 &v0, const dvec2 &v1, const dvec2 &v2)
{
	dvec2 ret;
	return mad(ret, v0, v1, v2);
}

UNIGINE_INLINE dvec2 min(const dvec2 &v0, const dvec2 &v1)
{
	dvec2 ret;
	#ifdef USE_SSE
		ret = _mm_min_pd(v0.vec, v1.vec);
	#else
		ret.x = min(v0.x, v1.x);
		ret.y = min(v0.y, v1.y);
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 max(const dvec2 &v0, const dvec2 &v1)
{
	dvec2 ret;
	#ifdef USE_SSE
		ret = _mm_max_pd(v0.vec, v1.vec);
	#else
		ret.x = max(v0.x, v1.x);
		ret.y = max(v0.y, v1.y);
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 clamp(const dvec2 &v, const dvec2 &v0, const dvec2 &v1)
{
	dvec2 ret;
	#ifdef USE_SSE
		ret = _mm_min_pd(_mm_max_pd(v.vec, v0.vec), v1.vec);
	#else
		ret.x = clamp(v.x, v0.x, v1.x);
		ret.y = clamp(v.y, v0.y, v1.y);
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 saturate(const dvec2 &v)
{
	dvec2 ret;
	#ifdef USE_SSE
		ret = _mm_min_pd(_mm_max_pd(v.vec, dvec2_zero.vec), dvec2_one.vec);
	#else
		ret.x = saturate(v.x);
		ret.y = saturate(v.y);
	#endif
	return ret;
}

UNIGINE_INLINE dvec2 &lerp(dvec2 &ret, const dvec2 &v0, double v1, double k)
{
	#ifdef USE_SSE
		ret = _mm_add_pd(v0.vec, _mm_mul_pd(_mm_sub_pd(_mm_set1_pd(v1), v0.vec), _mm_set1_pd(k)));
	#else
		ret.x = lerp(v0.x, v1, k);
		ret.y = lerp(v0.y, v1, k);
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 &lerp(dvec2 &ret, double v0, const dvec2 &v1, double k)
{
	#ifdef USE_SSE
		__m128d temp = _mm_set1_pd(v0);
		ret = _mm_add_pd(temp, _mm_mul_pd(_mm_sub_pd(v1.vec, temp), _mm_set1_pd(k)));
	#else
		ret.x = lerp(v0, v1.x, k);
		ret.y = lerp(v0, v1.y, k);
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 &lerp(dvec2 &ret, const dvec2 &v0, const dvec2 &v1, double k)
{
	#ifdef USE_SSE
		ret = _mm_add_pd(v0.vec, _mm_mul_pd(_mm_sub_pd(v1.vec, v0.vec), _mm_set1_pd(k)));
	#else
		ret.x = lerp(v0.x, v1.x, k);
		ret.y = lerp(v0.y, v1.y, k);
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 &lerp(dvec2 &ret, const dvec2 &v0, const dvec2 &v1, const dvec2 &k)
	{
	#ifdef USE_SSE
		ret = _mm_add_pd(v0.vec, _mm_mul_pd(_mm_sub_pd(v1.vec, v0.vec), k.vec));
	#else
		ret.x = lerp(v0.x, v1.x, k.x);
		ret.y = lerp(v0.y, v1.y, k.y);
	#endif
	return ret;
}
UNIGINE_INLINE dvec2 lerp(const dvec2 &v0, const dvec2 &v1, double k)
{
	dvec2 ret;
	return lerp(ret, v0, v1, k);
}

UNIGINE_INLINE dvec2 &inverseLerp(dvec2 &ret, const dvec2 &v0, const dvec2 &v1, const dvec2 &v)
{
	ret = saturate((v - v0) / (v1 - v0));
	return ret;
}
UNIGINE_INLINE dvec2 inverseLerp(const dvec2 &v0, const dvec2 &v1, const dvec2 &v)
{
	dvec2 ret;
	return inverseLerp(ret, v0, v1, v);
}

UNIGINE_INLINE double dot(const dvec2 &v0, const dvec2 &v1)
{
	return v0.x * v1.x + v0.y * v1.y;
}

UNIGINE_INLINE dvec2 round(const dvec2 &v) { return {round(v.x), round(v.y)}; }
UNIGINE_INLINE dvec2 floor(const dvec2 &v)
{
	#ifdef USE_SSE
		return _mm_floor_pd(v.vec);
	#else
		return {floor(v.x), floor(v.y)};
	#endif
}
UNIGINE_INLINE dvec2 ceil(const dvec2 &v)
{
	#ifdef USE_SSE
		return _mm_ceil_pd(v.vec);
	#else
		return {ceil(v.x), ceil(v.y)};
	#endif
}
UNIGINE_INLINE dvec2 abs(const dvec2 &v) { return {abs(v.x), abs(v.y)}; }

}
}