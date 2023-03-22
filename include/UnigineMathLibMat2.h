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

#ifdef __GNUC__
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wreorder"
#endif

namespace Unigine
{
namespace Math
{

struct alignas(16) mat2
{
	UNIGINE_INLINE constexpr mat2(float v, ConstexprTag): m00(v), m10(v), m01(v), m11(v) {}
	UNIGINE_INLINE constexpr mat2(
		float m00_, float m10_,
		float m01_, float m11_,
		ConstexprTag
		)
		: m00(m00_), m10(m10_)
		, m01(m01_), m11(m11_)
	{
	}

	UNIGINE_INLINE mat2(): m00{1.0f}, m01{0.0f}, m10{0.0f}, m11{1.0f}
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE mat2(
		float m00_, float m10_,
		float m01_, float m11_
	)
		: m00(m00_), m10(m10_)
		, m01(m01_), m11(m11_)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit mat2(float v): m00(v), m10(v), m01(v), m11(v)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit mat2(const float *m): m00(m[0]), m10(m[2]), m01(m[1]), m11(m[3]) 
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE mat2(const mat2 &m)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m);
	}

	UNIGINE_INLINE mat2(const vec2 &col0, const vec2 &col1)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		m00 = col0.x;
		m01 = col1.x;
		m10 = col0.y;
		m11 = col1.y;
	}

	UNIGINE_INLINE explicit mat2(const mat3 &m)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m);
	}
	UNIGINE_INLINE explicit mat2(const mat4 &m)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m);
	}
	UNIGINE_INLINE explicit mat2(const dmat4 &m)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m);
	}

	UNIGINE_INLINE mat2 &operator=(const mat2 &m) { set(m); return *this; }
	UNIGINE_INLINE mat2 operator-() const
	{
		mat2 ret;
		ret.m00 = -m00;
		ret.m01 = -m01;
		ret.m10 = -m10;
		ret.m11 = -m11;
		return ret;
	}

	UNIGINE_INLINE mat2 &operator*=(float v);
	UNIGINE_INLINE mat2 &operator*=(const mat2 &m);

	UNIGINE_INLINE mat2 &operator+=(const mat2 &m);
	UNIGINE_INLINE mat2 &operator-=(const mat2 &m);

	UNIGINE_INLINE operator float *() { return mat; }
	UNIGINE_INLINE operator const float *() const { return mat; }
	UNIGINE_INLINE operator void *() { return mat; }
	UNIGINE_INLINE operator const void *() const { return mat; }

	UNIGINE_INLINE float &operator[](int i)
	{
		assert((unsigned int)i < 4 && "mat2::operator[](): bad index");
		return mat[i];
	}
	UNIGINE_INLINE float operator[](int i) const
	{
		assert((unsigned int)i < 4 && "mat2::operator[](): bad index");
		return mat[i];
	}

	UNIGINE_INLINE void set(int row, int column, float v)
	{
		assert((unsigned int)row < 2 && "mat2::set(): bad row");
		assert((unsigned int)column < 2 && "mat2::set(): bad column");
		mat[column * 2 + row] = v;
	}

	UNIGINE_INLINE float &get(int row, int column)
	{
		assert((unsigned int)row < 2 && "mat2::get(): bad row");
		assert((unsigned int)column < 2 && "mat2::get(): bad column");
		return mat[column * 2 + row];
	}
	UNIGINE_INLINE float get(int row, int column) const
	{
		assert((unsigned int)row < 2 && "mat2::get(): bad row");
		assert((unsigned int)column < 2 && "mat2::get(): bad column");
		return mat[column * 2 + row];
	}

	UNIGINE_INLINE void set(const mat2 &m)
	{
		#ifdef USE_SSE
			vec = m.vec;
		#else
			m00 = m.m00;
			m10 = m.m10;
			m01 = m.m01;
			m11 = m.m11;
		#endif
	}
	UNIGINE_INLINE void set(const mat3 &m);
	UNIGINE_INLINE void set(const mat4 &m);
	UNIGINE_INLINE void set(const dmat4 &m);
	UNIGINE_INLINE void set(const float *m)
	{
		m00 = m[0];
		m01 = m[2];
		m10 = m[1];
		m11 = m[3];
	}

	UNIGINE_INLINE void get(float *m) const
	{
		m[0] = m00;
		m[2] = m01;
		m[1] = m10;
		m[3] = m11;
	}
	UNIGINE_INLINE float *get() { return mat; }
	UNIGINE_INLINE const float *get() const { return mat; }

	UNIGINE_INLINE void setRow(int row, const vec2 &v)
	{
		assert(row < 2 && row >= 0 && "mat2::setRow(): bad row");
		mat[row + 0] = v.x;
		mat[row + 2] = v.y;
	}
	UNIGINE_INLINE vec2 getRow(int row) const
	{
		assert(row < 2 && row >= 0 && "mat2::getRow(): bad row");
		return vec2(mat[row + 0], mat[row + 2]);
	}

	UNIGINE_INLINE void setColumn(int column, const vec2 &v)
	{
		assert(column < 2 && column >= 0 && "mat2::setColumn(): bad column");
		column *= 2;
		mat[column + 0] = v.x;
		mat[column + 1] = v.y;
	}
	UNIGINE_INLINE vec2 getColumn(int column) const
	{
		assert(column < 2 && column >= 0 && "mat2::getColumn(): bad column");
		return vec2(mat + column * 2);
	}

	UNIGINE_INLINE void setZero()
	{
		m00 = 0.0f;
		m01 = 0.0f;
		m10 = 0.0f;
		m11 = 0.0f;
	}
	UNIGINE_INLINE void setIdentity()
	{
		m00 = 1.0f;
		m01 = 0.0f;
		m10 = 0.0f;
		m11 = 1.0f;
	}

	UNIGINE_INLINE void setRotate(float angle)
	{
		float s, c;
		Math::sincos(angle * Consts::DEG2RAD, s, c);
		m00 = c;
		m01 = -s;
		m10 = s;
		m11 = c;
	}
	UNIGINE_INLINE void setScale(const vec2 &v)
	{
		m00 = v.x;
		m01 = 0.0f;
		m10 = 0.0f;
		m11 = v.y;
	}

	UNIGINE_INLINE float trace() const { return m00 + m11; }
	UNIGINE_INLINE float determinant() const { return m00 * m11 - m10 * m01; }

	union
	{
		struct
		{
			float m00;
			float m10;
			float m01;
			float m11;
		};
		float mat[4];
		#ifdef USE_SSE
			__m128 vec; // DO NOT TOUCH UNLESS ABSOLUTELY SURE
		#endif
	};
};

constexpr mat2 mat2_zero(0.0f, ConstexprTag{});
constexpr mat2 mat2_one(1.0f, ConstexprTag{});
constexpr mat2 mat2_identity(1.0f, 0.0f,
							 0.0f, 1.0f, ConstexprTag{});

UNIGINE_INLINE float trace(const mat2 &m) { return m.trace(); }
UNIGINE_INLINE float determinant(const mat2 &m) { return m.determinant(); }

UNIGINE_INLINE int compare(const mat2 &m0, const mat2 &m1)
{
	return (compare(m0.m00, m1.m00) && compare(m0.m10, m1.m10)) && 
			compare(m0.m01, m1.m01) && compare(m0.m11, m1.m11);
}
UNIGINE_INLINE int compare(const mat2 &m0, const mat2 &m1, float epsilon)
{
	return (compare(m0.m00, m1.m00, epsilon) && compare(m0.m10, m1.m10, epsilon)) &&
			compare(m0.m01, m1.m01, epsilon) && compare(m0.m11, m1.m11, epsilon);
}
UNIGINE_INLINE int operator==(const mat2 &m0, const mat2 &m1) { return compare(m0, m1); }
UNIGINE_INLINE int operator!=(const mat2 &m0, const mat2 &m1) { return !compare(m0, m1); }

UNIGINE_INLINE mat2 &mul(mat2 &ret, const mat2 &m, float v)
{
	ret.m00 = m.m00 * v;
	ret.m01 = m.m01 * v;
	ret.m10 = m.m10 * v;
	ret.m11 = m.m11 * v;
	return ret;
}
UNIGINE_INLINE vec2 &mul(vec2 &ret, const mat2 &m, const vec2 &v)
{
	float x = v.x;
	float y = v.y;
	ret.x = m.m00 * x + m.m01 * y;
	ret.y = m.m10 * x + m.m11 * y;
	return ret;
}
UNIGINE_INLINE vec2 &mul(vec2 &ret, const vec2 &v, const mat2 &m)
{
	float x = v.x;
	float y = v.y;
	ret.x = m.m00 * x + m.m10 * y;
	ret.y = m.m01 * x + m.m11 * y;
	return ret;
}
UNIGINE_INLINE dvec2 &mul(dvec2 &ret, const mat2 &m, const dvec2 &v)
{
	double x = v.x;
	double y = v.y;
	ret.x = m.m00 * x + m.m01 * y;
	ret.y = m.m10 * x + m.m11 * y;
	return ret;
}
UNIGINE_INLINE dvec2 &mul(dvec2 &ret, const dvec2 &v, const mat2 &m)
{
	double x = v.x;
	double y = v.y;
	ret.x = m.m00 * x + m.m10 * y;
	ret.y = m.m01 * x + m.m11 * y;
	return ret;
}
UNIGINE_INLINE mat2 &mul(mat2 &ret, const mat2 &m0, const mat2 &m1)
{
	ret.m00 = m0.m00 * m1.m00 + m0.m01 * m1.m10;
	ret.m10 = m0.m10 * m1.m00 + m0.m11 * m1.m10;
	ret.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11;
	ret.m11 = m0.m10 * m1.m01 + m0.m11 * m1.m11;
	return ret;
}
UNIGINE_INLINE mat2 operator*(const mat2 &m, float v)
{
	mat2 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE vec2 operator*(const mat2 &m, const vec2 &v)
{
	vec2 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE vec2 operator*(const vec2 &v, const mat2 &m)
{
	vec2 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE dvec2 operator*(const mat2 &m, const dvec2 &v)
{
	dvec2 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE dvec2 operator*(const dvec2 &v, const mat2 &m)
{
	dvec2 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE mat2 operator*(const mat2 &m0, const mat2 &m1)
{
	mat2 ret;
	return mul(ret, m0, m1);
}
UNIGINE_INLINE mat2 &mat2::operator*=(float v) { return mul(*this, *this, v); }
UNIGINE_INLINE mat2 &mat2::operator*=(const mat2 &m) { return mul(*this, mat2(*this), m); }

UNIGINE_INLINE mat2 &add(mat2 &ret, const mat2 &m0, const mat2 &m1)
{
	ret.m00 = m0.m00 + m1.m00;
	ret.m01 = m0.m01 + m1.m01;
	ret.m10 = m0.m10 + m1.m10;
	ret.m11 = m0.m11 + m1.m11;
	return ret;
}
UNIGINE_INLINE mat2 operator+(const mat2 &m0, const mat2 &m1)
{
	mat2 ret;
	return add(ret, m0, m1);
}
UNIGINE_INLINE mat2 &mat2::operator+=(const mat2 &m) { return add(*this, *this, m); }

UNIGINE_INLINE mat2 &sub(mat2 &ret, const mat2 &m0, const mat2 &m1)
{
	ret.m00 = m0.m00 - m1.m00;
	ret.m01 = m0.m01 - m1.m01;
	ret.m10 = m0.m10 - m1.m10;
	ret.m11 = m0.m11 - m1.m11;
	return ret;
}
UNIGINE_INLINE mat2 operator-(const mat2 &m0, const mat2 &m1)
{
	mat2 ret;
	return sub(ret, m0, m1);
}
UNIGINE_INLINE mat2 &mat2::operator-=(const mat2 &m) { return sub(*this, *this, m); }

UNIGINE_INLINE mat2 &inverse(mat2 &ret, const mat2 &m)
{
	float idet = Math::rcp(m.determinant());
	ret.m00 = m.m11 * idet;
	ret.m10 = -m.m10 * idet;
	ret.m01 = -m.m01 * idet;
	ret.m11 = m.m00 * idet;
	return ret;
}
UNIGINE_INLINE mat2 &inverse(mat2 &ret, const mat2 &m, float det)
{
	float idet = Math::rcp(det);
	ret.m00 = m.m11 * idet;
	ret.m10 = -m.m10 * idet;
	ret.m01 = -m.m01 * idet;
	ret.m11 = m.m00 * idet;
	return ret;
}
UNIGINE_INLINE mat2 inverse(const mat2 &m)
{
	mat2 ret;
	return inverse(ret, m);
}
UNIGINE_INLINE mat2 inverse(const mat2 &m, float det)
{
	mat2 ret;
	return inverse(ret, m, det);
}

UNIGINE_INLINE mat2 &transpose(mat2 &ret, const mat2 &m)
{
	ret.m00 = m.m00;
	ret.m01 = m.m10;
	ret.m10 = m.m01;
	ret.m11 = m.m11;
	return ret;
}
UNIGINE_INLINE mat2 transpose(const mat2 &m)
{
	mat2 ret;
	return transpose(ret, m);
}

}
}

#ifdef __GNUC__
	#pragma GCC diagnostic pop
#endif
