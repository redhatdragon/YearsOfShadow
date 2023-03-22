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

struct alignas(16) vec3
{
	UNIGINE_INLINE constexpr vec3(float v, ConstexprTag): x(v), y(v), z(v), align(0.0f) {}
	UNIGINE_INLINE constexpr vec3(float x, float y, float z, ConstexprTag) : x(x), y(y), z(z), align(0.0f) {}

	UNIGINE_INLINE vec3()
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
		, align(0.0f)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit vec3(float v)
		: x(v)
		, y(v)
		, z(v)
		, align(0.0f)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE vec3(float x, float y, float z)
		: x(x)
		, y(y)
		, z(z)
		, align(0.0f)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit vec3(const float *v)
		: x(v[0])
		, y(v[1])
		, z(v[2])
		, align(0.0f)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}

	UNIGINE_INLINE vec3(const vec2 &v, float z)
		: x(v.x)
		, y(v.y)
		, z(z)
		, align(0.0f)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit vec3(const vec2 &v)
		: x(v.x)
		, y(v.y)
		, z(0.0f)
		, align(0.0f)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}

	#ifdef USE_SSE
		UNIGINE_INLINE vec3(const vec3 &v): vec(v.vec)
		{
			UNIGINE_ASSERT_ALIGNED16(this);
		}
		UNIGINE_INLINE vec3(const __m128 &v): vec(v)
		{
			UNIGINE_ASSERT_ALIGNED16(this);
		}
		UNIGINE_INLINE vec3 &operator=(const __m128 &val) { vec =val; return *this; }
	#endif

	UNIGINE_INLINE explicit vec3(const vec4 &v);
	UNIGINE_INLINE explicit vec3(const dvec3 &v);
	UNIGINE_INLINE explicit vec3(const hvec3 &v);
	UNIGINE_INLINE explicit vec3(const ivec3 &v);

	UNIGINE_INLINE void set(float val)
	{
		x = val;
		y = val;
		z = val;
	}
	UNIGINE_INLINE void set(float x_, float y_, float z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}
	UNIGINE_INLINE void set(const vec2 &val, float z_ = 0.0f)
	{
		x = val.x;
		y = val.y;
		z = z_;
	}
	UNIGINE_INLINE void set(const vec3 &val)
	{
		#ifdef USE_SSE
			vec = val.vec;
		#else
			x = val.x;
			y = val.y;
			z = val.z;
		#endif
	}
	UNIGINE_INLINE void set(const float *val)
	{
		x = val[0];
		y = val[1];
		z = val[2];
	}
	UNIGINE_INLINE void set(const vec4 &v);

	UNIGINE_INLINE void get(float *val) const
	{
		val[0] = x;
		val[1] = y;
		val[2] = z;
	}
	UNIGINE_INLINE float *get() { return v; }
	UNIGINE_INLINE const float *get() const { return v; }

	UNIGINE_INLINE operator float *() { return v; }
	UNIGINE_INLINE operator const float *() const { return v; }
	UNIGINE_INLINE operator void *() { return v; }
	UNIGINE_INLINE operator const void *() const { return v; }

	UNIGINE_INLINE float &operator[](int i)
	{
		UNIGINE_ASSERT((unsigned int)i < 3 && "vec3::operator[](): bad index");
		return v[i];
	}
	UNIGINE_INLINE float operator[](int i) const
	{
		UNIGINE_ASSERT((unsigned int)i < 3 && "vec3::operator[](): bad index");
		return v[i];
	}


	UNIGINE_INLINE vec3 &operator=(const vec3 &v) { set(v); return *this; }
	UNIGINE_INLINE vec3 operator-() const { return vec3(-x, -y, -z); }


	UNIGINE_INLINE void mul(float v)
	{
		#ifdef USE_SSE
			vec =_mm_mul_ps(vec, _mm_set_ps1(v));
		#else
			x *= v;
			y *= v;
			z *= v;
		#endif
	}
	UNIGINE_INLINE void mul(const vec3 &v)
	{
		#ifdef USE_SSE
			vec =_mm_mul_ps(vec, v.vec);
		#else
			x *= v.x;
			y *= v.y;
			z *= v.z;
		#endif
	}
	UNIGINE_INLINE vec3 &operator*=(float v) { mul(v); return *this; }
	UNIGINE_INLINE vec3 &operator*=(const vec3 &v) { mul(v); return *this; }

	UNIGINE_INLINE void div(float v) { mul(rcp(v)); }
	UNIGINE_INLINE void div(const vec3 &v)
	{
		#ifdef USE_SSE
			vec =_mm_div_ps(vec, v.vec);
		#else
			x /= v.x;
			y /= v.y;
			z /= v.z;
		#endif
	}
	UNIGINE_INLINE vec3 &operator/=(float v) { div(v); return *this; }
	UNIGINE_INLINE vec3 &operator/=(const vec3 &v) { div(v); return *this; }

	UNIGINE_INLINE float sum() const { return x + y + z; }
	UNIGINE_INLINE void add(float v)
	{
		#ifdef USE_SSE
			vec =_mm_add_ps(vec, _mm_set_ps1(v));
		#else
			x += v;
			y += v;
			z += v;
		#endif
	}
	UNIGINE_INLINE void add(const vec3 &v)
	{
		#ifdef USE_SSE
			vec =_mm_add_ps(vec, v.vec);
		#else
			x += v.x;
			y += v.y;
			z += v.z;
		#endif
	}
	UNIGINE_INLINE vec3 &operator+=(float v) { add(v); return *this; }
	UNIGINE_INLINE vec3 &operator+=(const vec3 &v) { add(v); return *this; }
	
	UNIGINE_INLINE void sub(float v)
	{
		#ifdef USE_SSE
			vec =_mm_sub_ps(vec, _mm_set_ps1(v));
		#else
			x -= v;
			y -= v;
			z -= v;
		#endif
	}
	UNIGINE_INLINE void sub(const vec3 &v)
	{
		#ifdef USE_SSE
			vec =_mm_sub_ps(vec, v.vec);
		#else
			x -= v.x;
			y -= v.y;
			z -= v.z;
		#endif
	}
	UNIGINE_INLINE vec3 &operator-=(float v) { sub(v); return *this; }
	UNIGINE_INLINE vec3 &operator-=(const vec3 &v) { sub(v); return *this; }


	UNIGINE_INLINE float length2() const
	{
		#ifdef USE_SSE
			float ret;
			_mm_store_ss(&ret, _mm_dp_ps(vec, vec, 0x7f));
			return ret;
		#else
			return x * x + y * y + z * z;
		#endif
	}
	UNIGINE_INLINE float length() const { return toFloat(sqrt(length2())); }
	UNIGINE_INLINE float lengthFast() const
	{
		#ifdef USE_SSE
			float ret;
			_mm_store_ss(&ret, _mm_rcp_ss(_mm_rsqrt_ss(_mm_dp_ps(vec, vec, 0x7f))));
			return ret;
		#else
			return Math::sqrtFast(length2());
		#endif
	}
	UNIGINE_INLINE float iLength() const { return Math::rsqrt(length2()); }
	UNIGINE_INLINE float iLengthFast() const { return Math::rsqrtFast(length2()); }

	UNIGINE_INLINE vec3 &normalize()
	{
		#ifdef USE_SSE
			float length;
			_mm_store_ss(&length, _mm_dp_ps(vec, vec, 0x7f));
			vec =_mm_mul_ps(vec, _mm_set_ps1(Math::rsqrt(length)));
		#else
			mul(iLength());
		#endif
		return *this;
	}
	UNIGINE_INLINE vec3 &normalizeValid()
	{
		float length = length2();
		if (length != 0.0f)
			mul(Math::rsqrt(length));
		return *this;
	}
	UNIGINE_INLINE vec3 &normalizeFast()
	{
		#ifdef USE_SSE
			vec =_mm_mul_ps(vec, _MM_SWIZZLE(_mm_rsqrt_ss(_mm_dp_ps(vec, vec, 0x7f)), X, X, X, X));
		#else
			mul(iLengthFast());
		#endif
		return *this;
	}
	UNIGINE_INLINE vec3 &normalizeValidFast()
	{
		float length = length2();
		if (length != 0.0f)
			mul(Math::rsqrtFast(length));
		return *this;
	}

	UNIGINE_INLINE float maxXY() const { return Math::max(x, y); }
	UNIGINE_INLINE float max() const { return Math::max(maxXY(), z); }

	UNIGINE_INLINE float minXY() { return Math::min(x, y); }
	UNIGINE_INLINE float min() { return Math::min(minXY(), z); }

	UNIGINE_INLINE unsigned int hash() const { return hashCombine(hashCombine(hashInteger(x), y), z); }

	UNIGINE_INLINE vec3 &abs()
	{
		x = Math::abs(x);
		y = Math::abs(y);
		z = Math::abs(z);
		return *this;
	}
	UNIGINE_INLINE vec3 sign() const { return vec3(Math::sign(x), Math::sign(y), Math::sign(z)); }

public:
	union
	{
		struct
		{
			float x, y, z, align;
		};
		vec2 xy;
		float v[3];

		#ifdef USE_SSE
			__m128 vec; // DO NOT TOUCH UNLESS ABSOLUTELY SURE
		#endif
	};
};

constexpr vec3 vec3_zero(0.0f, ConstexprTag{});
constexpr vec3 vec3_one(1.0f, ConstexprTag{});
constexpr vec3 vec3_half(0.5f, ConstexprTag{});
constexpr vec3 vec3_eps(Consts::EPS, ConstexprTag{});
constexpr vec3 vec3_inf(Consts::INF, ConstexprTag{});
constexpr vec3 vec3_up(0.0f, 0.0f, 1.0f, ConstexprTag{});
constexpr vec3 vec3_down(0.0f, 0.0f, -1.0f, ConstexprTag{});
constexpr vec3 vec3_forward(0.0f, 1.0f, 0.0f, ConstexprTag{});
constexpr vec3 vec3_back(0.0f, -1.0f, 0.0f, ConstexprTag{});
constexpr vec3 vec3_right(1.0f, 0.0f, 0.0f, ConstexprTag{});
constexpr vec3 vec3_left(-1.0f, 0.0f, 0.0f, ConstexprTag{});


UNIGINE_INLINE float length(const vec3 &v) { return v.length(); }
UNIGINE_INLINE float length2(const vec3 &v) { return v.length2(); }
UNIGINE_INLINE float length2nan(const vec3 &v)
{
	// in gcc isnan is inside std namespace but in Microsoft compiler it's not
	using namespace std;
	float t = v.length2();
	return isnan(t) ? 0.0f : t;
}

UNIGINE_INLINE float distance2(const vec3 &v0, const vec3 &v1)
{
	#ifdef USE_SSE
		float ret;
		__m128	vec = _mm_sub_ps(v0.vec, v1.vec);
		_mm_store_ss(&ret, _mm_dp_ps(vec, vec, 0x7f));
		return ret;
	#else
		return length2(v0 - v1);
	#endif
}
UNIGINE_INLINE float distance(const vec3 &v0, const vec3 &v1)
{
	#ifdef USE_SSE
		float ret;
		__m128	vec = _mm_sub_ps(v0.vec, v1.vec);
		_mm_store_ss(&ret, _mm_dp_ps(vec, vec, 0x7f));
		return sqrtf(ret);
	#else
		return length(v0 - v1);
	#endif
}

UNIGINE_INLINE vec3 normalize(const vec3 &v)
{
	vec3 ret = v;
	return ret.normalize();
}
UNIGINE_INLINE vec3 normalizeValid(const vec3 &v)
{
	vec3 ret = v;
	return ret.normalizeValid();
}

UNIGINE_INLINE int compare(const vec3 &v0, const vec3 &v1)
{
	return (compare(v0.x, v1.x) && compare(v0.y, v1.y) && compare(v0.z, v1.z));
}
UNIGINE_INLINE int compare(const vec3 &v0, const vec3 &v1, float epsilon)
{
	return (compare(v0.x, v1.x, epsilon) && compare(v0.y, v1.y, epsilon) && compare(v0.z, v1.z, epsilon));
}
UNIGINE_INLINE int operator==(const vec3 &v0, const vec3 &v1) { return compare(v0, v1); }
UNIGINE_INLINE int operator!=(const vec3 &v0, const vec3 &v1) { return !compare(v0, v1); }

UNIGINE_INLINE int operator<(const vec3 &v0, const vec3 &v1) { return v0.x < v1.x && v0.y < v1.y && v0.z < v1.z; }
UNIGINE_INLINE int operator>(const vec3 &v0, const vec3 &v1) { return v0.x > v1.x && v0.y > v1.y && v0.z > v1.z; }

UNIGINE_INLINE int operator<=(const vec3 &v0, const vec3 &v1) { return v0.x <= v1.x && v0.y <= v1.y && v0.z <= v1.z; }
UNIGINE_INLINE int operator>=(const vec3 &v0, const vec3 &v1) { return v0.x >= v1.x && v0.y >= v1.y && v0.z >= v1.z; }

UNIGINE_INLINE vec3 &mul(vec3 &ret, const vec3 &v0, float v1)
{
	#ifdef USE_SSE
		ret.vec = _mm_mul_ps(v0.vec, _mm_set_ps1(v1));
	#else
		ret.x = v0.x * v1;
		ret.y = v0.y * v1;
		ret.z = v0.z * v1;
	#endif
	return ret;
}
UNIGINE_INLINE vec3 &mul(vec3 &ret, const vec3 &v0, const vec3 &v1)
{
	#ifdef USE_SSE
		ret.vec = _mm_mul_ps(v0.vec, v1.vec);
	#else
		ret.x = v0.x * v1.x;
		ret.y = v0.y * v1.y;
		ret.z = v0.z * v1.z;
	#endif
	return ret;
}
UNIGINE_INLINE vec3 mul(const vec3 &v0, float v1)
{
	vec3 ret;
	return mul(ret, v0, v1);
}
UNIGINE_INLINE vec3 mul(const vec3 &v0, const vec3 &v1)
{
	vec3 ret;
	return mul(ret, v0, v1);
}
UNIGINE_INLINE vec3 operator*(const vec3 &v0, float v1) { return mul(v0, v1); }
UNIGINE_INLINE vec3 operator*(const vec3 &v0, const vec3 &v1) { return mul(v0, v1); }

UNIGINE_INLINE vec3 &div(vec3 &ret, const vec3 &v0, float v1) { return mul(ret, v0, rcp(v1)); }
UNIGINE_INLINE vec3 &div(vec3 &ret, const vec3 &v0, const vec3 &v1)
{
	#ifdef USE_SSE
		ret.vec = _mm_div_ps(v0.vec, v1.vec);
	#else
		ret.x = v0.x / v1.x;
		ret.y = v0.y / v1.y;
		ret.z = v0.z / v1.z;
	#endif
	return ret;
}
UNIGINE_INLINE vec3 div(const vec3 &v0, float v1)
{
	vec3 ret;
	return div(ret, v0, v1);
}
UNIGINE_INLINE vec3 div(const vec3 &v0, const vec3 &v1)
{
	vec3 ret;
	return div(ret, v0, v1);
}
UNIGINE_INLINE vec3 operator/(const vec3 &v0, float v1) { return div(v0, v1); }
UNIGINE_INLINE vec3 operator/(const vec3 &v0, const vec3 &v1) { return div(v0, v1); }


UNIGINE_INLINE vec3 &add(vec3 &ret, const vec3 &v0, float v1)
{
	#ifdef USE_SSE
		ret.vec = _mm_add_ps(v0.vec, _mm_set_ps1(v1));
	#else
		ret.x = v0.x + v1;
		ret.y = v0.y + v1;
		ret.z = v0.z + v1;
	#endif
	return ret;
}
UNIGINE_INLINE vec3 &add(vec3 &ret, const vec3 &v0, const vec3 &v1)
{
	#ifdef USE_SSE
		ret.vec = _mm_add_ps(v0.vec, v1.vec);
	#else
		ret.x = v0.x + v1.x;
		ret.y = v0.y + v1.y;
		ret.z = v0.z + v1.z;
	#endif
	return ret;
}
UNIGINE_INLINE vec3 add(const vec3 &v0, float v1)
{
	vec3 ret;
	return add(ret, v0, v1);
}
UNIGINE_INLINE vec3 add(const vec3 &v0, const vec3 &v1)
{
	vec3 ret;
	return add(ret, v0, v1);
}
UNIGINE_INLINE vec3 operator+(const vec3 &v0, float v1) { return add(v0, v1); }
UNIGINE_INLINE vec3 operator+(const vec3 &v0, const vec3 &v1) { return add(v0, v1); }

UNIGINE_INLINE vec3 &sub(vec3 &ret, const vec3 &v0, float v1)
{
	#ifdef USE_SSE
		ret.vec = _mm_sub_ps(v0.vec, _mm_set_ps1(v1));
	#else
		ret.x = v0.x - v1;
		ret.y = v0.y - v1;
		ret.z = v0.z - v1;
	#endif
	return ret;
}
UNIGINE_INLINE vec3 &sub(vec3 &ret, const vec3 &v0, const vec3 &v1)
{
	#ifdef USE_SSE
		ret.vec = _mm_sub_ps(v0.vec, v1.vec);
	#else
		ret.x = v0.x - v1.x;
		ret.y = v0.y - v1.y;
		ret.z = v0.z - v1.z;
	#endif
	return ret;
}
UNIGINE_INLINE vec3 sub(const vec3 &v0, float v1)
{
	vec3 ret;
	return sub(ret, v0, v1);
}
UNIGINE_INLINE vec3 sub(const vec3 &v0, const vec3 &v1)
{
	vec3 ret;
	return sub(ret, v0, v1);
}
UNIGINE_INLINE vec3 operator-(const vec3 &v0, float v1) { return sub(v0, v1); }
UNIGINE_INLINE vec3 operator-(const vec3 &v0, const vec3 &v1) { return sub(v0, v1); }

UNIGINE_INLINE vec3 &mad(vec3 &ret, const vec3 &v0, float v1, const vec3 &v2)
{
	#ifdef USE_SSE
		ret.vec = _mm_add_ps(_mm_mul_ps(v0.vec, _mm_set_ps1(v1)), v2.vec);
	#else
		ret.x = v0.x * v1 + v2.x;
		ret.y = v0.y * v1 + v2.y;
		ret.z = v0.z * v1 + v2.z;
	#endif
	return ret;
}
UNIGINE_INLINE vec3 &mad(vec3 &ret, const vec3 &v0, const vec3 &v1, const vec3 &v2)
{
	#ifdef USE_SSE
		ret.vec = _mm_add_ps(_mm_mul_ps(v0.vec, v1.vec), v2.vec);
	#else
		ret.x = v0.x * v1.x + v2.x;
		ret.y = v0.y * v1.y + v2.y;
		ret.z = v0.z * v1.z + v2.z;
	#endif
	return ret;
}
UNIGINE_INLINE vec3 mad(const vec3 &v0, float v1, const vec3 &v2)
{
	vec3 ret;
	return mad(ret, v0, v1, v2);
}
UNIGINE_INLINE vec3 mad(const vec3 &v0, const vec3 &v1, const vec3 &v2)
{
	vec3 ret;
	return mad(ret, v0, v1, v2);
}

UNIGINE_INLINE float dot(const vec2 &v0, const vec3 &v1) { return v0.x * v1.x + v0.y * v1.y + v1.z; }
UNIGINE_INLINE float dot(const vec3 &v0, const vec2 &v1) { return v0.x * v1.x + v0.y * v1.y + v0.z; }
UNIGINE_INLINE float dot(const vec3 &v0, const vec3 &v1)
{
	#ifdef USE_SSE
		float ret;
		_mm_store_ss(&ret, _mm_dp_ps(v0.vec, v1.vec, 0x7f));
		return ret;
	#else
		return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
	#endif
}

UNIGINE_INLINE vec3 &min(vec3 &ret, const vec3 &v0, const vec3 &v1)
{
	#ifdef USE_SSE
		ret.vec = _mm_min_ps(v0.vec, v1.vec);
	#else
		ret.x = min(v0.x, v1.x);
		ret.y = min(v0.y, v1.y);
		ret.z = min(v0.z, v1.z);
	#endif
	return ret;
}
UNIGINE_INLINE vec3 &min(vec3 &ret, const vec3 &v0, float v1)
{
	#ifdef USE_SSE
		ret.vec = _mm_min_ps(v0.vec, _mm_set_ps1(v1));
	#else
		ret.x = min(v0.x, v1);
		ret.y = min(v0.y, v1);
		ret.z = min(v0.z, v1);
	#endif
	return ret;
}
UNIGINE_INLINE vec3 min(const vec3 &v0, const vec3 &v1)
{
	vec3 ret;
	return min(ret, v0, v1);
}
UNIGINE_INLINE vec3 min(const vec3 &v0, float v1)
{
	vec3 ret;
	return min(ret, v0, v1);
}

UNIGINE_INLINE vec3 &max(vec3 &ret, const vec3 &v0, const vec3 &v1)
{
	#ifdef USE_SSE
		ret.vec = _mm_max_ps(v0.vec, v1.vec);
	#else
		ret.x = max(v0.x, v1.x);
		ret.y = max(v0.y, v1.y);
		ret.z = max(v0.z, v1.z);
	#endif
	return ret;
}
UNIGINE_INLINE vec3 &max(vec3 &ret, const vec3 &v0, float v1)
{
	#ifdef USE_SSE
		ret.vec = _mm_max_ps(v0.vec, _mm_set_ps1(v1));
	#else
		ret.x = max(v0.x, v1);
		ret.y = max(v0.y, v1);
		ret.z = max(v0.z, v1);
	#endif
	return ret;
}
UNIGINE_INLINE vec3 max(const vec3 &v0, const vec3 &v1)
{
	vec3 ret;
	return max(ret, v0, v1);
}
UNIGINE_INLINE vec3 max(const vec3 &v0, float v1)
{
	vec3 ret;
	return max(ret, v0, v1);
}

UNIGINE_INLINE vec3 &clamp(vec3 &ret, const vec3 &v, const vec3 &v0, const vec3 &v1)
{
	#ifdef USE_SSE
		ret.vec = _mm_min_ps(_mm_max_ps(v.vec, v0.vec), v1.vec);
	#else
		ret.x = clamp(v.x, v0.x, v1.x);
		ret.y = clamp(v.y, v0.y, v1.y);
		ret.z = clamp(v.z, v0.z, v1.z);
	#endif
	return ret;
}
UNIGINE_INLINE vec3 clamp(const vec3 &v, const vec3 &v0, const vec3 &v1)
{
	vec3 ret;
	return clamp(ret, v, v0, v1);
}

UNIGINE_INLINE vec3 &saturate(vec3 &ret, const vec3 &v)
{
	#ifdef USE_SSE
		ret.vec = _mm_min_ps(_mm_max_ps(v.vec, vec3_zero.vec), vec3_one.vec);
	#else
		ret.x = saturate(v.x);
		ret.y = saturate(v.y);
		ret.z = saturate(v.z);
	#endif
	return ret;
}
UNIGINE_INLINE vec3 saturate(const vec3 &v)
{
	vec3 ret;
	return saturate(ret, v);
}

UNIGINE_INLINE vec3 &lerp(vec3 &ret, const vec3 &v0, const vec3 &v1, float k)
{
	#ifdef USE_SSE
		ret.vec = _mm_add_ps(v0.vec, _mm_mul_ps(_mm_sub_ps(v1.vec, v0.vec), _mm_set_ps1(k)));
	#else
		ret.x = lerp(v0.x, v1.x, k);
		ret.y = lerp(v0.y, v1.y, k);
		ret.z = lerp(v0.z, v1.z, k);
	#endif
	return ret;
}
UNIGINE_INLINE vec3 lerp(const vec3 &v0, const vec3 &v1, float k)
{
	vec3 ret;
	return lerp(ret, v0, v1, k);
}

UNIGINE_INLINE vec3 &lerp(vec3 &ret, const vec3 &v0, const vec3 &v1, const vec3 &k)
{
	#ifdef USE_SSE
		ret.vec = _mm_add_ps(v0.vec, _mm_mul_ps(_mm_sub_ps(v1.vec, v0.vec), k.vec));
	#else
		ret.x = lerp(v0.x, v1.x, k);
		ret.y = lerp(v0.y, v1.y, k);
		ret.z = lerp(v0.z, v1.z, k);
	#endif
	return ret;
}
UNIGINE_INLINE vec3 lerp(const vec3 &v0, const vec3 &v1, const vec3 &k)
{
	vec3 ret;
	return lerp(ret, v0, v1, k);
}

UNIGINE_INLINE vec3 &cross(vec3 &ret, const vec3 &v0, const vec3 &v1)
{
	#ifdef USE_SSE
		ret.vec = _mm_cross_ps(v0.vec, v1.vec);
	#else
		ret.x = v0.y * v1.z - v0.z * v1.y;
		ret.y = v0.z * v1.x - v0.x * v1.z;
		ret.z = v0.x * v1.y - v0.y * v1.x;
	#endif
	return ret;
}
UNIGINE_INLINE vec3 cross(const vec3 &v0, const vec3 &v1)
{
	vec3 ret;
	return cross(ret, v0, v1);
}

UNIGINE_INLINE vec3 &reflect(vec3 &ret, const vec3 &v0, const vec3 &v1)
{
	float k = dot(v0, v1) * 2.0f;
	#ifdef USE_SSE
		ret.vec = _mm_sub_ps(v0.vec, _mm_mul_ps(v1.vec, _mm_set_ps1(k)));
	#else
		ret.x = v0.x - v1.x * k;
		ret.y = v0.y - v1.y * k;
		ret.z = v0.z - v1.z * k;
	#endif
	return ret;
}
UNIGINE_INLINE vec3 reflect(const vec3 &v0, const vec3 &v1)
{
	vec3 ret;
	return reflect(ret, v0, v1);
}

UNIGINE_INLINE vec3 round(const vec3 &v)
{
	return {round(v.x), round(v.y), round(v.z)};
}
UNIGINE_INLINE vec3 floor(const vec3 &v)
{
	#ifdef USE_SSE
		return _mm_floor_ps(v.vec);
	#else
		return {floor(v.x), floor(v.y), floor(v.z)};
	#endif
}
UNIGINE_INLINE vec3 ceil(const vec3 &v)
{
	#ifdef USE_SSE
		return _mm_ceil_ps(v.vec);
	#else
		return {ceil(v.x), ceil(v.y), ceil(v.z)};
	#endif
}

UNIGINE_INLINE vec3 frac(const vec3 &v) { return {frac(v.x), frac(v.y), frac(v.z)}; }
UNIGINE_INLINE vec3 abs(const vec3 &v) { return {abs(v.x), abs(v.y), abs(v.z)}; }

UNIGINE_INLINE vec3 step(const vec3 &a, const vec3 &b)
{
	return {toFloat(a.x > b.x), toFloat(a.y > b.y), toFloat(a.z > b.z)};
}
UNIGINE_INLINE vec3 step(const vec3 &a, float b)
{
	return {toFloat(a.x > b), toFloat(a.y > b), toFloat(a.z > b)};
}

UNIGINE_INLINE vec3 inverseLerp(const vec3 &v0, const vec3 &v1, const vec3 &v)
{
	return saturate((v - v0) / (v1 - v0));
}

UNIGINE_INLINE vec3 contrastLerp(const vec3 &point_a, const vec3 &point_b, float coef_min_bound, float coef_max_bound, float coef)
{
	return lerp(point_a, point_b, smoothstep(coef_min_bound, coef_max_bound, coef));
}

UNIGINE_INLINE vec3 overlay(const vec3 &a, const vec3 &b, float x)
{
	return max(lerp(a, lerp(vec3_one - (vec3_one - a) * (vec3_one - b) * 2.0f, a * b * 2.0f, step(a, vec3_half)), x), vec3_zero);
}
UNIGINE_INLINE vec3 overlay(const vec3 &a, const vec3 &b, const vec3 &x)
{
	return max(lerp(a, lerp(vec3_one - (vec3_one - a) * (vec3_one - b) * 2.0f, a * b * 2.0f, step(a, vec3_half)), x), vec3_zero);
}

UNIGINE_INLINE bool areCollinear(const vec3 &v0, const vec3 &v1) { return length(cross(v0, v1)) < 1e-6f; }

UNIGINE_INLINE vec3 srgb(const vec3 &color) { return vec3(srgb(color.x), srgb(color.y), srgb(color.z)); }
UNIGINE_INLINE vec3 isrgb(const vec3 &color) { return vec3(isrgb(color.x), isrgb(color.y), isrgb(color.z)); }

UNIGINE_INLINE float rgbToLuma(const vec3 &color)
{
	return dot(color, vec3(0.299f, 0.587f, 0.114f));
}

UNIGINE_INLINE float getAngle(const vec3 &v0, const vec3 &v1)
{
	float d = dot(v0, v1) / (length(v0) * length(v1));
	return acos(clamp(d, -1.0f, 1.0f)) * Consts::RAD2DEG;
}
UNIGINE_INLINE float getAngle(const vec3 &v0, const vec3 &v1, const vec3 &up)
{
	vec3 n = cross(v0, v1);
	return atan2(dot(normalize(up), n), dot(v0, v1)) * Consts::RAD2DEG;
}


}
}