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

#include "UnigineMathLibIVec2.h"
#include "UnigineMathLibIVec3.h"

namespace Unigine
{
namespace Math
{

struct alignas(16) ivec4
{
	UNIGINE_INLINE constexpr ivec4(int v, ConstexprTag): x(v), y(v), z(v), w(v) {}

	UNIGINE_INLINE ivec4(): x(0), y(0), z(0), w(0)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit ivec4(int v): x(v), y(v), z(v), w(v)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE ivec4(int x, int y, int z, int w): x(x), y(y), z(z), w(w)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit ivec4(const int *v): x(v[0]), y(v[1]), z(v[2]), w(v[3])
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE ivec4(const ivec4 &v)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(v);
	}

	UNIGINE_INLINE explicit ivec4(const ivec3 &v): x(v.x), y(v.y), z(v.z), w(1)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit ivec4(const ivec3 &v, int w): x(v.x), y(v.y), z(v.z), w(w)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}

	UNIGINE_INLINE explicit ivec4(const ivec2 &v, int z, int w): x(v.x), y(v.y), z(z), w(w)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit ivec4(const ivec2 &v0, const ivec2 &v1): x(v0.x), y(v0.y), z(v1.x), w(v1.y)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}

	UNIGINE_INLINE explicit ivec4(const dvec4 &v)
		: x(toInt(v.x))
		, y(toInt(v.y))
		, z(toInt(v.z))
		, w(toInt(v.w))
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}

	#ifdef USE_SSE
		UNIGINE_INLINE explicit ivec4(const vec4 &v)
		{
			UNIGINE_ASSERT_ALIGNED16(this);
			vec = _mm_cvttps_epi32(v.vec);
		}
	#else
		UNIGINE_INLINE explicit ivec4(const vec4 &v)
			: x(toInt(v.x))
			, y(toInt(v.y))
			, z(toInt(v.z))
			, w(toInt(v.w))
		{
			UNIGINE_ASSERT_ALIGNED16(this);
		}
	#endif

	#ifdef USE_SSE
		UNIGINE_INLINE ivec4(const __m128i &v): vec(v)
		{
			UNIGINE_ASSERT_ALIGNED16(this);
		}
		UNIGINE_INLINE ivec4 &operator=(const __m128i &val) { vec = val; return *this; }
	#endif

	UNIGINE_INLINE explicit ivec4(const bvec4 &v);
	UNIGINE_INLINE explicit ivec4(const svec4 &v);



	UNIGINE_INLINE void set(int val)
	{
		x = val;
		y = val;
		z = val;
		w = val;
	}
	UNIGINE_INLINE void set(int x_, int y_, int z_, int w_)
	{
		x = x_;
		y = y_;
		z = z_;
		w = w_;
	}
	UNIGINE_INLINE void set(const ivec4 &val)
	{
		#ifdef USE_SSE
			vec = val.vec;
		#else
			x = val.x;
			y = val.y;
			z = val.z;
			w = val.w;
		#endif
	}
	UNIGINE_INLINE void set(const int *val)
	{
		x = val[0];
		y = val[1];
		z = val[2];
		w = val[3];
	}

	UNIGINE_INLINE void get(int *val) const
	{
		val[0] = x;
		val[1] = y;
		val[2] = z;
		val[3] = w;
	}
	UNIGINE_INLINE int *get() { return v; }
	UNIGINE_INLINE const int *get() const { return v; }

	UNIGINE_INLINE operator int *() { return v; }
	UNIGINE_INLINE operator const int *() const { return v; }
	UNIGINE_INLINE operator void *() { return v; }
	UNIGINE_INLINE operator const void *() const { return v; }

	UNIGINE_INLINE int &operator[](int i)
	{
		assert((unsigned int)i < 4 && "ivec4::operator[](): bad index");
		return v[i];
	}
	UNIGINE_INLINE int operator[](int i) const
	{
		assert((unsigned int)i < 4 && "ivec4::operator[](): bad index");
		return v[i];
	}


	UNIGINE_INLINE ivec4 &operator=(const ivec4 &val) { set(val); return *this; }
	UNIGINE_INLINE ivec4 &operator=(int val) { set(val); return *this; }
	UNIGINE_INLINE ivec4 operator-() const { return ivec4(-x, -y, -z, -w); }

	UNIGINE_INLINE int mul() const { return x * y * z * w; }
	UNIGINE_INLINE void mul(int v)
	{
		x *= v;
		y *= v;
		z *= v;
		w *= v;
	}
	UNIGINE_INLINE void mul(const ivec4 &v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
	}
	UNIGINE_INLINE ivec4 &operator*=(int v) { mul(v); return *this; }
	UNIGINE_INLINE ivec4 &operator*=(const ivec4 &v) { mul(v); return *this; }


	UNIGINE_INLINE void div(int v)
	{
		x /= v;
		y /= v;
		z /= v;
		w /= v;
	}
	UNIGINE_INLINE void div(const ivec4 &v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
	}
	UNIGINE_INLINE ivec4 &operator/=(int v) { div(v); return *this; }
	UNIGINE_INLINE ivec4 &operator/=(const ivec4 &v) { div(v); return *this; }


	UNIGINE_INLINE void sub(int v)
	{
		x -= v;
		y -= v;
		z -= v;
		w -= v;
	}
	UNIGINE_INLINE void sub(const ivec4 &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
	}
	UNIGINE_INLINE ivec4 &operator-=(int v) { sub(v); return *this; }
	UNIGINE_INLINE ivec4 &operator-=(const ivec4 &v) { sub(v); return *this; }


	UNIGINE_INLINE int sum() const { return x + y + z + w; }
	UNIGINE_INLINE void add(int v)
	{
		x += v;
		y += v;
		z += v;
		w += v;
	}
	UNIGINE_INLINE void add(const ivec4 &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
	}
	UNIGINE_INLINE ivec4 &operator+=(int v) { add(v); return *this; }
	UNIGINE_INLINE ivec4 &operator+=(const ivec4 &v) { add(v); return *this; }

	UNIGINE_INLINE void shiftLeft(int v)
	{
		x <<= v;
		y <<= v;
		z <<= v;
		w <<= v;
	}
	UNIGINE_INLINE void shiftLeft(const ivec4 &v)
	{
		x <<= v.x;
		y <<= v.y;
		z <<= v.z;
		w <<= v.w;
	}
	UNIGINE_INLINE ivec4 &operator<<=(int v) { shiftLeft(v); return *this; }
	UNIGINE_INLINE ivec4 &operator<<=(const ivec4 &v) { shiftLeft(v); return *this; }


	UNIGINE_INLINE void shiftRight(int v)
	{
		x >>= v;
		y >>= v;
		z >>= v;
		w >>= v;
	}
	UNIGINE_INLINE void shiftRight(const ivec4 &v)
	{
		x >>= v.x;
		y >>= v.y;
		z >>= v.z;
		w >>= v.w;
	}
	UNIGINE_INLINE ivec4 &operator>>=(int v) { shiftRight(v); return *this; }
	UNIGINE_INLINE ivec4 &operator>>=(const ivec4 &v) { shiftRight(v); return *this; }

	UNIGINE_INLINE int length2() const
	{
		return x * x + y * y + z * z + w * w;
	}

	UNIGINE_INLINE ivec4 &abs()
	{
		x = Math::abs(x);
		y = Math::abs(y);
		z = Math::abs(z);
		w = Math::abs(w);
		return *this;
	}

	UNIGINE_INLINE int max() const { return Math::max(Math::max(Math::max(x, y), z), w); }
	UNIGINE_INLINE int min() const { return Math::min(Math::min(Math::min(x, y), z), w); }

	UNIGINE_INLINE unsigned int hash() const { return hashCombine(hashCombine(hashCombine(x, y), z), w); }

	union
	{
		struct
		{
			int x, y, z, w;
		};

		const ivec3 xyz;
		ivec2 xy;

		struct
		{
			ivec2 xy;
			ivec2 zw;
		} swizzle;

		int v[4];

		#ifdef USE_SSE
			__m128i vec; // DO NOT TOUCH UNLESS ABSOLUTELY SURE
		#endif
	};
};

constexpr ivec4 ivec4_zero(0, ConstexprTag{});
constexpr ivec4 ivec4_one(1, ConstexprTag{});

UNIGINE_INLINE int length2(const ivec4 &v) { return v.length2(); }

UNIGINE_INLINE int compare(const ivec4 &v0, const ivec4 &v1) { return v0.x == v1.x && v0.y == v1.y && v0.z == v1.z && v0.w == v1.w; }
UNIGINE_INLINE int operator==(const ivec4 &v0, const ivec4 &v1) { return compare(v0, v1); }
UNIGINE_INLINE int operator!=(const ivec4 &v0, const ivec4 &v1) { return !compare(v0, v1); }

UNIGINE_INLINE int operator<(const ivec4 &v0, const ivec4 &v1) { return v0.x < v1.x &&v0.y < v1.y && v0.z < v1.z && v0.w < v1.w; }
UNIGINE_INLINE int operator>(const ivec4 &v0, const ivec4 &v1) { return v0.x > v1.x && v0.y > v1.y && v0.z > v1.z && v0.w > v1.w; }

UNIGINE_INLINE int operator<=(const ivec4 &v0, const ivec4 &v1) { return v0.x <= v1.x && v0.y <= v1.y && v0.z <= v1.z && v0.w <= v1.w; }
UNIGINE_INLINE int operator>=(const ivec4 &v0, const ivec4 &v1) { return v0.x >= v1.x && v0.y >= v1.y && v0.z >= v1.z && v0.w >= v1.w; }

UNIGINE_INLINE ivec4 &mul(ivec4 &ret, const ivec4 &v0, int v1)
{
	ret.x = v0.x * v1;
	ret.y = v0.y * v1;
	ret.z = v0.z * v1;
	ret.w = v0.w * v1;
	return ret;
}
UNIGINE_INLINE ivec4 &mul(ivec4 &ret, const ivec4 &v0, const ivec4 &v1)
{
	ret.x = v0.x * v1.x;
	ret.y = v0.y * v1.y;
	ret.z = v0.z * v1.z;
	ret.w = v0.w * v1.z;
	return ret;
}
UNIGINE_INLINE ivec4 mul(const ivec4 &v0, int v1)
{
	ivec4 ret;
	return mul(ret, v0, v1);
}
UNIGINE_INLINE ivec4 mul(const ivec4 &v0, const ivec4 &v1)
{
	ivec4 ret;
	return mul(ret, v0, v1);
}
UNIGINE_INLINE ivec4 operator*(const ivec4 &v0, int v1) { return mul(v0, v1); }
UNIGINE_INLINE ivec4 operator*(const ivec4 &v0, const ivec4 &v1) { return mul(v0, v1); }

UNIGINE_INLINE ivec4 &div(ivec4 &ret, const ivec4 &v0, int v1)
{
	ret.x = v0.x / v1;
	ret.y = v0.y / v1;
	ret.z = v0.z / v1;
	ret.w = v0.w / v1;
	return ret;
}
UNIGINE_INLINE ivec4 &div(ivec4 &ret, const ivec4 &v0, const ivec4 &v1)
{
	ret.x = v0.x / v1.x;
	ret.y = v0.y / v1.y;
	ret.z = v0.z / v1.z;
	ret.w = v0.w / v1.w;
	return ret;
}
UNIGINE_INLINE ivec4 div(const ivec4 &v0, int v1)
{
	ivec4 ret;
	return div(ret, v0, v1);
}
UNIGINE_INLINE ivec4 div(const ivec4 &v0, const ivec4 &v1)
{
	ivec4 ret;
	return div(ret, v0, v1);
}
UNIGINE_INLINE ivec4 operator/(const ivec4 &v0, int v1) { return div(v0, v1); }
UNIGINE_INLINE ivec4 operator/(const ivec4 &v0, const ivec4 &v1) { return div(v0, v1); }

UNIGINE_INLINE ivec4 &mod(ivec4 &ret, const ivec4 &v0, int v1)
{
	ret.x = v0.x % v1;
	ret.y = v0.y % v1;
	ret.z = v0.z % v1;
	ret.w = v0.w % v1;
	return ret;
}
UNIGINE_INLINE ivec4 &mod(ivec4 &ret, const ivec4 &v0, const ivec4 &v1)
{
	ret.x = v0.x % v1.x;
	ret.y = v0.y % v1.y;
	ret.z = v0.z % v1.z;
	ret.w = v0.w % v1.w;
	return ret;
}
UNIGINE_INLINE ivec4 mod(const ivec4 &v0, int v1)
{
	ivec4 ret;
	return mod(ret, v0, v1);
}
UNIGINE_INLINE ivec4 mod(const ivec4 &v0, const ivec4 &v1)
{
	ivec4 ret;
	return mod(ret, v0, v1);
}
UNIGINE_INLINE ivec4 operator%(const ivec4 &v0, int v1) { return mod(v0, v1); }
UNIGINE_INLINE ivec4 operator%(const ivec4 &v0, const ivec4 &v1) { return mod(v0, v1); }

UNIGINE_INLINE ivec4 &add(ivec4 &ret, const ivec4 &v0, int v1)
{
	ret.x = v0.x + v1;
	ret.y = v0.y + v1;
	ret.z = v0.z + v1;
	ret.w = v0.w + v1;
	return ret;
}
UNIGINE_INLINE ivec4 &add(ivec4 &ret, const ivec4 &v0, const ivec4 &v1)
{
	ret.x = v0.x + v1.x;
	ret.y = v0.y + v1.y;
	ret.z = v0.z + v1.z;
	ret.w = v0.w + v1.w;
	return ret;
}
UNIGINE_INLINE ivec4 add(const ivec4 &v0, int v1)
{
	ivec4 ret;
	return add(ret, v0, v1);
}
UNIGINE_INLINE ivec4 add(const ivec4 &v0, const ivec4 &v1)
{
	ivec4 ret;
	return add(ret, v0, v1);
}
UNIGINE_INLINE ivec4 operator+(const ivec4 &v0, int v1) { return add(v0, v1); }
UNIGINE_INLINE ivec4 operator+(const ivec4 &v0, const ivec4 &v1) { return add(v0, v1); }

UNIGINE_INLINE ivec4 &sub(ivec4 &ret, const ivec4 &v0, int v1)
{
	ret.x = v0.x - v1;
	ret.y = v0.y - v1;
	ret.z = v0.z - v1;
	ret.w = v0.w - v1;
	return ret;
}
UNIGINE_INLINE ivec4 &sub(ivec4 &ret, const ivec4 &v0, const ivec4 &v1)
{
	ret.x = v0.x - v1.x;
	ret.y = v0.y - v1.y;
	ret.z = v0.z - v1.z;
	ret.w = v0.w - v1.w;
	return ret;
}
UNIGINE_INLINE ivec4 sub(const ivec4 &v0, int v1)
{
	ivec4 ret;
	return sub(ret, v0, v1);
}
UNIGINE_INLINE ivec4 sub(const ivec4 &v0, const ivec4 &v1)
{
	ivec4 ret;
	return sub(ret, v0, v1);
}
UNIGINE_INLINE ivec4 operator-(const ivec4 &v0, int v1) { return sub(v0, v1); }
UNIGINE_INLINE ivec4 operator-(const ivec4 &v0, const ivec4 &v1) { return sub(v0, v1); }

UNIGINE_INLINE ivec4 &shiftLeft(ivec4 &ret, const ivec4 &v0, int v1)
{
	ret.x = v0.x << v1;
	ret.y = v0.y << v1;
	ret.z = v0.z << v1;
	ret.w = v0.w << v1;
	return ret;
}
UNIGINE_INLINE ivec4 &shiftLeft(ivec4 &ret, const ivec4 &v0, const ivec4 &v1)
{
	ret.x = v0.x << v1.x;
	ret.y = v0.y << v1.y;
	ret.z = v0.z << v1.z;
	ret.w = v0.w << v1.w;
	return ret;
}
UNIGINE_INLINE ivec4 shiftLeft(const ivec4 &v0, int v1)
{
	ivec4 ret;
	return shiftLeft(ret, v0, v1);
}
UNIGINE_INLINE ivec4 shiftLeft(const ivec4 &v0, const ivec4 &v1)
{
	ivec4 ret;
	return shiftLeft(ret, v0, v1);
}
UNIGINE_INLINE ivec4 operator<<(const ivec4 &v0, int v1) { return shiftLeft(v0, v1); }
UNIGINE_INLINE ivec4 operator<<(const ivec4 &v0, const ivec4 &v1) { return shiftLeft(v0, v1); }

UNIGINE_INLINE ivec4 &shiftRight(ivec4 &ret, const ivec4 &v0, int v1)
{
	ret.x = v0.x >> v1;
	ret.y = v0.y >> v1;
	ret.z = v0.z >> v1;
	ret.w = v0.w >> v1;
	return ret;
}
UNIGINE_INLINE ivec4 &shiftRight(ivec4 &ret, const ivec4 &v0, const ivec4 &v1)
{
	ret.x = v0.x >> v1.x;
	ret.y = v0.y >> v1.y;
	ret.z = v0.z >> v1.z;
	ret.w = v0.w >> v1.w;
	return ret;
}
UNIGINE_INLINE ivec4 shiftRight(const ivec4 &v0, int v1)
{
	ivec4 ret;
	return shiftRight(ret, v0, v1);
}
UNIGINE_INLINE ivec4 shiftRight(const ivec4 &v0, const ivec4 &v1)
{
	ivec4 ret;
	return shiftRight(ret, v0, v1);
}
UNIGINE_INLINE ivec4 operator>>(const ivec4 &v0, int v1) { return shiftRight(v0, v1); }
UNIGINE_INLINE ivec4 operator>>(const ivec4 &v0, const ivec4 &v1) { return shiftRight(v0, v1); }

UNIGINE_INLINE ivec4 &mad(ivec4 &ret, const ivec4 &v0, int v1, const ivec4 &v2)
{
	ret.x = v0.x * v1 + v2.x;
	ret.y = v0.y * v1 + v2.y;
	ret.z = v0.z * v1 + v2.z;
	ret.w = v0.w * v1 + v2.w;
	return ret;
}
UNIGINE_INLINE ivec4 &mad(ivec4 &ret, const ivec4 &v0, const ivec4 &v1, const ivec4 &v2)
{
	ret.x = v0.x * v1.x + v2.x;
	ret.y = v0.y * v1.y + v2.y;
	ret.z = v0.z * v1.z + v2.z;
	ret.w = v0.w * v1.w + v2.w;
	return ret;
}
UNIGINE_INLINE ivec4 mad(const ivec4 &v0, int v1, const ivec4 &v2)
{
	ivec4 ret;
	return mad(ret, v0, v1, v2);
}
UNIGINE_INLINE ivec4 mad(const ivec4 &v0, const ivec4 &v1, const ivec4 &v2)
{
	ivec4 ret;
	return mad(ret, v0, v1, v2);
}

UNIGINE_INLINE ivec4 &lerp(ivec4 &ret, const ivec4 &v0, const ivec4 &v1, int k)
{
	ret.x = lerp(v0.x, v1.x, k);
	ret.y = lerp(v0.y, v1.y, k);
	ret.z = lerp(v0.z, v1.z, k);
	ret.w = lerp(v0.w, v1.w, k);
	return ret;
}
UNIGINE_INLINE ivec4 &lerp(ivec4 &ret, const ivec4 &v0, const ivec4 &v1, const ivec4 &k)
{
	ret.x = lerp(v0.x, v1.x, k.x);
	ret.y = lerp(v0.y, v1.y, k.y);
	ret.z = lerp(v0.z, v1.z, k.z);
	ret.w = lerp(v0.w, v1.w, k.w);
	return ret;
}
UNIGINE_INLINE ivec4 lerp(const ivec4 &v0, const ivec4 &v1, const ivec4 &k)
{
	ivec4 ret;
	return lerp(ret, v0, v1, k);
}
UNIGINE_INLINE ivec4 lerp(const ivec4 &v0, const ivec4 &v1, int k)
{
	ivec4 ret;
	return lerp(ret, v0, v1, k);
}

UNIGINE_INLINE int dot(const ivec4 &v0, const ivec4 &v1)
{
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
}

UNIGINE_INLINE ivec4 min(const ivec4 &v0, const ivec4 &v1)
{
	ivec4 ret;
	ret.x = min(v0.x, v1.x);
	ret.y = min(v0.y, v1.y);
	ret.z = min(v0.z, v1.z);
	ret.w = min(v0.w, v1.w);
	return ret;
}
UNIGINE_INLINE ivec4 max(const ivec4 &v0, const ivec4 &v1)
{
	ivec4 ret;
	ret.x = max(v0.x, v1.x);
	ret.y = max(v0.y, v1.y);
	ret.z = max(v0.z, v1.z);
	ret.w = max(v0.w, v1.w);
	return ret;
}
UNIGINE_INLINE ivec4 clamp(const ivec4 &v, const ivec4 &v0, const ivec4 &v1)
{
	ivec4 ret;
	ret.x = clamp(v.x, v0.x, v1.x);
	ret.y = clamp(v.y, v0.y, v1.y);
	ret.z = clamp(v.z, v0.z, v1.z);
	ret.w = clamp(v.w, v0.w, v1.w);
	return ret;
}
UNIGINE_INLINE ivec4 abs(const ivec4 &v)
{
	return {abs(v.x), abs(v.y), abs(v.z), abs(v.w)};
}

}
}