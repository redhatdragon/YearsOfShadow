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
#include "UnigineMathLibVec3.h"

#include "UnigineMathLibDVec2.h"
#include "UnigineMathLibDVec3.h"

#include "UnigineMathLibMat2.h"

#ifdef __GNUC__
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wreorder"
#endif

namespace Unigine
{
namespace Math
{


struct alignas(16) mat3
{
	UNIGINE_INLINE constexpr mat3(
		float m00_, float m10_, float m20_,
		float m01_, float m11_, float m21_,
		float m02_, float m12_, float m22_,
		ConstexprTag)
		: m00(m00_), m10(m10_), m20(m20_), align0(0.0f)
		, m01(m01_), m11(m11_), m21(m21_), align1(0.0f)
		, m02(m02_), m12(m12_), m22(m22_), align2(0.0f)
	{
	}
	UNIGINE_INLINE constexpr mat3(float v, ConstexprTag)
		: m00(v), m10(v), m20(v), align0(0.0f)
		, m01(v), m11(v), m21(v), align1(0.0f)
		, m02(v), m12(v), m22(v), align2(0.0f)
	{
	}


	UNIGINE_INLINE mat3()
		: m00(1.0f)
		, m01(0.0f)
		, m02(0.0f)
		, m10(0.0f)
		, m11(1.0f)
		, m12(0.0f)
		, m20(0.0f)
		, m21(0.0f)
		, m22(1.0f)
		, align0(0.0f)
		, align1(0.0f)
		, align2(0.0f)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit mat3(float v)
		: m00(v)
		, m10(v)
		, m20(v)
		, m01(v)
		, m11(v)
		, m21(v)
		, m02(v)
		, m12(v)
		, m22(v)
		, align0(0.0f)
		, align1(0.0f)
		, align2(0.0f)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit mat3(const float *m)
		: m00(m[0])
		, m10(m[1])
		, m20(m[2])
		, m01(m[3])
		, m11(m[4])
		, m21(m[5])
		, m02(m[6])
		, m12(m[7])
		, m22(m[8])
		, align0(0.0f)
		, align1(0.0f)
		, align2(0.0f)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE mat3(const mat3 &m)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m);
	}

	UNIGINE_INLINE explicit mat3(const mat2 &m)
		: m00(m.m00)
		, m01(m.m01)
		, m02(0.0f)
		, m10(m.m10)
		, m11(m.m11)
		, m12(0.0f)
		, m20(0.0f)
		, m21(0.0f)
		, m22(1.0f)
		, align0(0.0f)
		, align1(0.0f)
		, align2(0.0f)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE mat3(const vec3 &c0, const vec3 &c1, const vec3 &c2)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		#ifdef USE_SSE
			sse.v0 = c0.vec;
			sse.v1 = c1.vec;
			sse.v2 = c2.vec;
		#else
			m00 = col0.x; m10 = col0.y; m20 = col0.z; m30 = 0.0f;
			m01 = col1.x; m11 = col1.y; m21 = col1.z; m31 = 0.0f;
			m02 = col2.x; m12 = col2.y; m22 = col2.z; m32 = 0.0f;
		#endif
	}

	UNIGINE_INLINE explicit mat3(const mat4 &m)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m);
	}
	UNIGINE_INLINE explicit mat3(const dmat4 &m)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m);
	}
	UNIGINE_INLINE explicit mat3(const quat &q)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(q);
	}

	UNIGINE_INLINE void set(int row, int column, float v)
	{
		assert((unsigned int)row < 3 && "mat3::set(): bad row");
		assert((unsigned int)column < 3 && "mat3::set(): bad column");
		mat[column * 4 + row] = v;
	}
	UNIGINE_INLINE void set(const mat2 &m)
	{
		m00 = m.m00;
		m01 = m.m01;
		m02 = 0.0f;
		m10 = m.m10;
		m11 = m.m11;
		m12 = 0.0f;
		m20 = 0.0f;
		m21 = 0.0f;
		m22 = 1.0f;
	}
	UNIGINE_INLINE void set(const mat3 &m)
	{
		sse.v0 = m.sse.v0;
		sse.v1 = m.sse.v1;
		sse.v2 = m.sse.v2;
	}
	UNIGINE_INLINE void set(const mat4 &m);
	UNIGINE_INLINE void set(const dmat4 &m);
	UNIGINE_INLINE void set(const quat &q);
	UNIGINE_INLINE void set(const float *m)
	{
		m00 = m[0];
		m01 = m[3];
		m02 = m[6];
		m10 = m[1];
		m11 = m[4];
		m12 = m[7];
		m20 = m[2];
		m21 = m[5];
		m22 = m[8];
	}

	UNIGINE_INLINE void get(float *m) const
	{
		m[0] = m00;
		m[3] = m01;
		m[6] = m02;
		m[1] = m10;
		m[4] = m11;
		m[7] = m12;
		m[2] = m20;
		m[5] = m21;
		m[8] = m22;
	}
	UNIGINE_INLINE float *get() { return mat; }
	UNIGINE_INLINE const float *get() const { return mat; }
	UNIGINE_INLINE float &get(int row, int column)
	{
		assert((unsigned int)row < 3 && "mat3::get(): bad row");
		assert((unsigned int)column < 3 && "mat3::get(): bad column");
		return mat[column * 4 + row];
	}
	UNIGINE_INLINE float get(int row, int column) const
	{
		assert((unsigned int)row < 3 && "mat3::get(): bad row");
		assert((unsigned int)column < 3 && "mat3::get(): bad column");
		return mat[column * 4 + row];
	}

	UNIGINE_INLINE operator float *() { return mat; }
	UNIGINE_INLINE operator const float *() const { return mat; }
	UNIGINE_INLINE operator void *() { return mat; }
	UNIGINE_INLINE operator const void *() const { return mat; }

	UNIGINE_INLINE float &operator[](int i)
	{
		assert((unsigned int)i < 12 && "mat3::operator[](): bad index");
		return mat[i];
	}
	UNIGINE_INLINE float operator[](int i) const
	{
		assert((unsigned int)i < 12 && "mat3::operator[](): bad index");
		return mat[i];
	}

	UNIGINE_INLINE mat3 &operator=(const mat3 &m)
	{
		set(m);
		return *this;
	}
	UNIGINE_INLINE mat3 operator-() const
	{
		mat3 ret;
		ret.m00 = -m00;
		ret.m01 = -m01;
		ret.m02 = -m02;
		ret.m10 = -m10;
		ret.m11 = -m11;
		ret.m12 = -m12;
		ret.m20 = -m20;
		ret.m21 = -m21;
		ret.m22 = -m22;
		return ret;
	}

	UNIGINE_INLINE mat3 &operator*=(float v);
	UNIGINE_INLINE mat3 &operator*=(const mat3 &m);
	UNIGINE_INLINE mat3 &operator+=(const mat3 &m);
	UNIGINE_INLINE mat3 &operator-=(const mat3 &m);

	UNIGINE_INLINE void setRow(int row, const vec3 & v)
	{
		assert(row < 3 && row >= 0 && "mat3::setRow(): bad row");
		mat[row + 0] = v.x;
		mat[row + 4] = v.y;
		mat[row + 8] = v.z;
	}
	UNIGINE_INLINE vec3 getRow(int row) const
	{
		assert(row < 3 && row >= 0 && "mat3::getRow(): bad row");
		return vec3(mat[row + 0], mat[row + 4], mat[row + 8]);
	}

	UNIGINE_INLINE void setColumn(int column, const vec3 &v)
	{
		assert(column < 3 && column >= 0 && "mat3::setColumn(): bad column");
		column *= 4;
		mat[column + 0] = v.x;
		mat[column + 1] = v.y;
		mat[column + 2] = v.z;
	}
	UNIGINE_INLINE vec3 getColumn(int column) const
	{
		assert(column < 3 && column >= 0 && "mat3::getColumn(): bad column");
		return vec3(mat + column * 4);
	}

	UNIGINE_INLINE vec3 getAxisX() const { return getColumn(0).normalize(); }
	UNIGINE_INLINE vec3 getAxisY() const { return getColumn(1).normalize(); }
	UNIGINE_INLINE vec3 getAxisZ() const { return getColumn(2).normalize(); }

	UNIGINE_INLINE void setDiagonal(const vec3 &v)
	{
		m00 = v.x;
		m01 = 0.0f;
		m02 = 0.0f;
		m10 = 0.0f;
		m11 = v.y;
		m12 = 0.0f;
		m20 = 0.0f;
		m21 = 0.0f;
		m22 = v.z;
	}
	UNIGINE_INLINE vec3 getDiagonal() const
	{
		return vec3(m00, m11, m22);
	}

	UNIGINE_INLINE void setZero()
	{
		m00 = 0.0f;
		m01 = 0.0f;
		m02 = 0.0f;
		m10 = 0.0f;
		m11 = 0.0f;
		m12 = 0.0f;
		m20 = 0.0f;
		m21 = 0.0f;
		m22 = 0.0f;
	}
	UNIGINE_INLINE void setIdentity()
	{
		m00 = 1.0f;
		m01 = 0.0f;
		m02 = 0.0f;
		m10 = 0.0f;
		m11 = 1.0f;
		m12 = 0.0f;
		m20 = 0.0f;
		m21 = 0.0f;
		m22 = 1.0f;
	}

	UNIGINE_INLINE void setSkewSymmetric(const vec3 &v)
	{
		m00 = 0.0f;
		m01 = -v.z;
		m02 = v.y;
		m10 = v.z;
		m11 = 0.0f;
		m12 = -v.x;
		m20 = -v.y;
		m21 = v.x;
		m22 = 0.0f;
	}

	UNIGINE_INLINE void setRotate(const vec3 &axis, float angle)
	{
		float s, c;
		Math::sincos(angle * Consts::DEG2RAD, s, c);
		vec3 v = normalize(axis);
		float xx = v.x * v.x;
		float yy = v.y * v.y;
		float zz = v.z * v.z;
		float xy = v.x * v.y;
		float yz = v.y * v.z;
		float zx = v.z * v.x;
		float xs = v.x * s;
		float ys = v.y * s;
		float zs = v.z * s;
		m00 = (1.0f - c) * xx + c;
		m01 = (1.0f - c) * xy - zs;
		m02 = (1.0f - c) * zx + ys;
		m10 = (1.0f - c) * xy + zs;
		m11 = (1.0f - c) * yy + c;
		m12 = (1.0f - c) * yz - xs;
		m20 = (1.0f - c) * zx - ys;
		m21 = (1.0f - c) * yz + xs;
		m22 = (1.0f - c) * zz + c;
	}
	UNIGINE_INLINE void setRotateX(float angle)
	{
		float s, c;
		Math::sincos(angle * Consts::DEG2RAD, s, c);
		m00 = 1.0f;
		m01 = 0.0f;
		m02 = 0.0f;
		m10 = 0.0f;
		m11 = c;
		m12 = -s;
		m20 = 0.0f;
		m21 = s;
		m22 = c;
	}
	UNIGINE_INLINE void setRotateY(float angle)
	{
		float s, c;
		Math::sincos(angle * Consts::DEG2RAD, s, c);
		m00 = c;
		m01 = 0.0f;
		m02 = s;
		m10 = 0.0f;
		m11 = 1.0f;
		m12 = 0.0f;
		m20 = -s;
		m21 = 0.0f;
		m22 = c;
	}
	UNIGINE_INLINE void setRotateZ(float angle)
	{
		float s, c;
		Math::sincos(angle * Consts::DEG2RAD, s, c);
		m00 = c;
		m01 = -s;
		m02 = 0.0f;
		m10 = s;
		m11 = c;
		m12 = 0.0f;
		m20 = 0.0f;
		m21 = 0.0f;
		m22 = 1.0f;
	}

	UNIGINE_INLINE void setScale(const vec3 &v)
	{
		m00 = v.x;
		m01 = 0.0f;
		m02 = 0.0f;
		m10 = 0.0f;
		m11 = v.y;
		m12 = 0.0f;
		m20 = 0.0f;
		m21 = 0.0f;
		m22 = v.z;
	}

	UNIGINE_INLINE quat getQuat() const;

	UNIGINE_INLINE float trace() const
	{
		return m00 + m11 + m22;
	}
	UNIGINE_INLINE float determinant() const
	{
		float det = m00 * (m11 * m22 - m12 * m21);
		det -= m01 * (m10 * m22 - m12 * m20);
		det += m02 * (m10 * m21 - m11 * m20);
		return det;
	}

	#ifdef USE_SSE
		struct sse_data
		{
			__m128 v0;
			__m128 v1;
			__m128 v2;
		}; 
	#endif

	union
	{
		struct
		{
			float m00, m10, m20, align0;
			float m01, m11, m21, align1;
			float m02, m12, m22, align2;
		};
		float mat[12];

		#ifdef USE_SSE
			sse_data sse; // DO NOT TOUCH UNLESS ABSOLUTELY SURE
		#endif
	};
};

// TODO: make constexrp when compiler is updated
constexpr mat3 mat3_zero(0.0f, ConstexprTag{});
constexpr mat3 mat3_one(1.0f, ConstexprTag{});
constexpr mat3 mat3_identity(
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, ConstexprTag{});

UNIGINE_INLINE float trace(const mat3 &m) { return m.trace(); }
UNIGINE_INLINE float determinant(const mat3 &m) { return m.determinant(); }

UNIGINE_INLINE int compare(const mat3 &m0, const mat3 &m1)
{
	return (compare(m0.m00, m1.m00) && compare(m0.m10, m1.m10) && compare(m0.m20, m1.m20) &&
			compare(m0.m01, m1.m01) && compare(m0.m11, m1.m11) && compare(m0.m21, m1.m21) &&
			compare(m0.m02, m1.m02) && compare(m0.m12, m1.m12) && compare(m0.m22, m1.m22));
}
UNIGINE_INLINE int compare(const mat3 &m0, const mat3 &m1, float epsilon)
{
	return (compare(m0.m00, m1.m00, epsilon) && compare(m0.m10, m1.m10, epsilon) && compare(m0.m20, m1.m20, epsilon) &&
			compare(m0.m01, m1.m01, epsilon) && compare(m0.m11, m1.m11, epsilon) && compare(m0.m21, m1.m21, epsilon) &&
			compare(m0.m02, m1.m02, epsilon) && compare(m0.m12, m1.m12, epsilon) && compare(m0.m22, m1.m22, epsilon));
}
UNIGINE_INLINE int operator==(const mat3 &m0, const mat3 &m1) { return compare(m0, m1);}
UNIGINE_INLINE int operator!=(const mat3 &m0, const mat3 &m1) { return !compare(m0, m1); }

UNIGINE_INLINE mat3 &mul(mat3 &ret, const mat3 &m, float v)
{
	#ifdef USE_SSE
		__m128 temp = _mm_set1_ps(v);
		ret.sse.v0 = _mm_mul_ps(m.sse.v0, temp);
		ret.sse.v1 = _mm_mul_ps(m.sse.v1, temp);
		ret.sse.v2 = _mm_mul_ps(m.sse.v2, temp);
	#else
		ret.m00 = m.m00 * v;
		ret.m01 = m.m01 * v;
		ret.m02 = m.m02 * v;
		ret.m10 = m.m10 * v;
		ret.m11 = m.m11 * v;
		ret.m12 = m.m12 * v;
		ret.m20 = m.m20 * v;
		ret.m21 = m.m21 * v;
		ret.m22 = m.m22 * v;
	#endif
	return ret;
}
UNIGINE_INLINE vec2 &mul(vec2 &ret, const mat3 &m, const vec2 &v)
{
	float x = v.x;
	float y = v.y;
	ret.x = m.m00 * x + m.m01 * y + m.m02;
	ret.y = m.m10 * x + m.m11 * y + m.m12;
	return ret;
}
UNIGINE_INLINE vec2 &mul(vec2 &ret, const vec2 &v, const mat3 &m)
{
	float x = v.x;
	float y = v.y;
	ret.x = m.m00 * x + m.m10 * y + m.m20;
	ret.y = m.m01 * x + m.m11 * y + m.m21;
	return ret;
}
UNIGINE_INLINE vec3 &mul(vec3 &ret, const mat3 &m, const vec3 &v)
{
	#ifdef USE_SSE
		__m128 vec = v.vec;
		__m128 res_0 = _mm_mul_ps(m.sse.v0, _MM_SWIZZLE(vec, X, X, X, X));
		__m128 res_1 = _mm_mul_ps(m.sse.v1, _MM_SWIZZLE(vec, Y, Y, Y, Y));
		__m128 res_2 = _mm_mul_ps(m.sse.v2, _MM_SWIZZLE(vec, Z, Z, Z, Z));
		ret = _mm_add_ps(_mm_add_ps(res_0, res_1), res_2);
	#else
		float x = v.x;
		float y = v.y;
		float z = v.z;
		ret.x = m.m00 * x + m.m01 * y + m.m02 * z;
		ret.y = m.m10 * x + m.m11 * y + m.m12 * z;
		ret.z = m.m20 * x + m.m21 * y + m.m22 * z;
	#endif
	return ret;
}
UNIGINE_INLINE vec3 &mul(vec3 &ret, const vec3 &v, const mat3 &m)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	ret.x = m.m00 * x + m.m10 * y + m.m20 * z;
	ret.y = m.m01 * x + m.m11 * y + m.m21 * z;
	ret.z = m.m02 * x + m.m12 * y + m.m22 * z;
	return ret;
}
UNIGINE_INLINE dvec2 &mul(dvec2 &ret, const mat3 &m, const dvec2 &v)
{
	double x = v.x;
	double y = v.y;
	ret.x = m.m00 * x + m.m01 * y + m.m02;
	ret.y = m.m10 * x + m.m11 * y + m.m12;
	return ret;
}
UNIGINE_INLINE dvec2 &mul(dvec2 &ret, const dvec2 &v, const mat3 &m)
{
	double x = v.x;
	double y = v.y;
	ret.x = m.m00 * x + m.m10 * y + m.m20;
	ret.y = m.m01 * x + m.m11 * y + m.m21;
	return ret;
}
UNIGINE_INLINE dvec3 &mul(dvec3 &ret, const mat3 &m, const dvec3 &v)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	ret.x = m.m00 * x + m.m01 * y + m.m02 * z;
	ret.y = m.m10 * x + m.m11 * y + m.m12 * z;
	ret.z = m.m20 * x + m.m21 * y + m.m22 * z;
	return ret;
}
UNIGINE_INLINE dvec3 &mul(dvec3 &ret, const dvec3 &v, const mat3 &m)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	ret.x = m.m00 * x + m.m10 * y + m.m20 * z;
	ret.y = m.m01 * x + m.m11 * y + m.m21 * z;
	ret.z = m.m02 * x + m.m12 * y + m.m22 * z;
	return ret;
}
UNIGINE_INLINE mat3 &mul(mat3 &ret, const mat3 &m, const vec3 &v)
{
	ret.m00 = m.m01 * v.z - m.m02 * v.y;
	ret.m10 = m.m11 * v.z - m.m12 * v.y;
	ret.m20 = m.m21 * v.z - m.m22 * v.y;
	ret.m01 = -m.m00 * v.z + m.m02 * v.x;
	ret.m11 = -m.m10 * v.z + m.m12 * v.x;
	ret.m21 = -m.m20 * v.z + m.m22 * v.x;
	ret.m02 = m.m00 * v.y - m.m01 * v.x;
	ret.m12 = m.m10 * v.y - m.m11 * v.x;
	ret.m22 = m.m20 * v.y - m.m21 * v.x;
	return ret;
}
UNIGINE_INLINE mat3 &mul(mat3 &ret, const vec3 &v, const mat3 &m)
{
	ret.m00 = -v.z * m.m10 + v.y * m.m20;
	ret.m10 = v.z * m.m00 - v.x * m.m20;
	ret.m20 = -v.y * m.m00 + v.x * m.m10;
	ret.m01 = -v.z * m.m11 + v.y * m.m21;
	ret.m11 = v.z * m.m01 - v.x * m.m21;
	ret.m21 = -v.y * m.m01 + v.x * m.m11;
	ret.m02 = -v.z * m.m12 + v.y * m.m22;
	ret.m12 = v.z * m.m02 - v.x * m.m22;
	ret.m22 = -v.y * m.m02 + v.x * m.m12;
	return ret;
}
UNIGINE_INLINE mat3 &mul(mat3 &ret, const mat3 &m0, const mat3 &m1)
{
	#ifdef USE_SSE
		__m128 m0_col0 = m0.sse.v0;
		__m128 m0_col1 = m0.sse.v1;
		__m128 m0_col2 = m0.sse.v2;

		__m128 m1_col0 = m1.sse.v0;
		__m128 m1_col1 = m1.sse.v1;
		__m128 m1_col2 = m1.sse.v2;

		__m128 res_0 = _mm_mul_ps(m0_col0, _MM_SWIZZLE(m1_col0, X, X, X, W));
		__m128 res_1 = _mm_mul_ps(m0_col1, _MM_SWIZZLE(m1_col0, Y, Y, Y, W));
		__m128 res_2 = _mm_mul_ps(m0_col2, _MM_SWIZZLE(m1_col0, Z, Z, Z, W));
		ret.sse.v0 = _mm_add_ps(_mm_add_ps(res_0, res_1), res_2);
		res_0 = _mm_mul_ps(m0_col0, _MM_SWIZZLE(m1_col1, X, X, X, W));
		res_1 = _mm_mul_ps(m0_col1, _MM_SWIZZLE(m1_col1, Y, Y, Y, W));
		res_2 = _mm_mul_ps(m0_col2, _MM_SWIZZLE(m1_col1, Z, Z, Z, W));
		ret.sse.v1 = _mm_add_ps(_mm_add_ps(res_0, res_1), res_2);
		res_0 = _mm_mul_ps(m0_col0, _MM_SWIZZLE(m1_col2, X, X, X, W));
		res_1 = _mm_mul_ps(m0_col1, _MM_SWIZZLE(m1_col2, Y, Y, Y, W));
		res_2 = _mm_mul_ps(m0_col2, _MM_SWIZZLE(m1_col2, Z, Z, Z, W));
		ret.sse.v2 = _mm_add_ps(_mm_add_ps(res_0, res_1), res_2);
	#else
		ret.m00 = m0.m00 * m1.m00 + m0.m01 * m1.m10 + m0.m02 * m1.m20;
		ret.m10 = m0.m10 * m1.m00 + m0.m11 * m1.m10 + m0.m12 * m1.m20;
		ret.m20 = m0.m20 * m1.m00 + m0.m21 * m1.m10 + m0.m22 * m1.m20;
		ret.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11 + m0.m02 * m1.m21;
		ret.m11 = m0.m10 * m1.m01 + m0.m11 * m1.m11 + m0.m12 * m1.m21;
		ret.m21 = m0.m20 * m1.m01 + m0.m21 * m1.m11 + m0.m22 * m1.m21;
		ret.m02 = m0.m00 * m1.m02 + m0.m01 * m1.m12 + m0.m02 * m1.m22;
		ret.m12 = m0.m10 * m1.m02 + m0.m11 * m1.m12 + m0.m12 * m1.m22;
		ret.m22 = m0.m20 * m1.m02 + m0.m21 * m1.m12 + m0.m22 * m1.m22;
	#endif
	return ret;
}
UNIGINE_INLINE mat3 operator*(const mat3 &m, float v)
{
	mat3 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE vec2 operator*(const mat3 &m, const vec2 &v)
{
	vec2 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE vec2 operator*(const vec2 &v, const mat3 &m)
{
	vec2 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE vec3 operator*(const mat3 &m, const vec3 &v)
{
	vec3 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE vec3 operator*(const vec3 &v, const mat3 &m)
{
	vec3 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE dvec2 operator*(const mat3 &m, const dvec2 &v)
{
	dvec2 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE dvec2 operator*(const dvec2 &v, const mat3 &m)
{
	dvec2 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE dvec3 operator*(const mat3 &m, const dvec3 &v)
{
	dvec3 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE dvec3 operator*(const dvec3 &v, const mat3 &m)
{
	dvec3 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE mat3 operator*(const mat3 &m0, const mat3 &m1)
{
	mat3 ret;
	return mul(ret, m0, m1);
}
UNIGINE_INLINE mat3 &mat3::operator*=(float v)
{
	return mul(*this, *this, v);
}
UNIGINE_INLINE mat3 &mat3::operator*=(const mat3 &m)
{
	return mul(*this, mat3(*this), m);
}

UNIGINE_INLINE mat3 &add(mat3 &ret, const mat3 &m0, const mat3 &m1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_add_ps(m0.sse.v0, m1.sse.v0);
		ret.sse.v1 = _mm_add_ps(m0.sse.v1, m1.sse.v1);
		ret.sse.v2 = _mm_add_ps(m0.sse.v2, m1.sse.v2);
	#else
		ret.m00 = m0.m00 + m1.m00;
		ret.m01 = m0.m01 + m1.m01;
		ret.m02 = m0.m02 + m1.m02;
		ret.m10 = m0.m10 + m1.m10;
		ret.m11 = m0.m11 + m1.m11;
		ret.m12 = m0.m12 + m1.m12;
		ret.m20 = m0.m20 + m1.m20;
		ret.m21 = m0.m21 + m1.m21;
		ret.m22 = m0.m22 + m1.m22;
	#endif
	return ret;
}
UNIGINE_INLINE mat3 operator+(const mat3 &m0, const mat3 &m1)
{
	mat3 ret;
	return add(ret, m0, m1);
}
UNIGINE_INLINE mat3 &mat3::operator+=(const mat3 &m)
{
	return add(*this, *this, m);
}

UNIGINE_INLINE mat3 &sub(mat3 &ret, const mat3 &m0, const mat3 &m1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_sub_ps(m0.sse.v0, m1.sse.v0);
		ret.sse.v1 = _mm_sub_ps(m0.sse.v1, m1.sse.v1);
		ret.sse.v2 = _mm_sub_ps(m0.sse.v2, m1.sse.v2);
	#else
		ret.m00 = m0.m00 - m1.m00;
		ret.m01 = m0.m01 - m1.m01;
		ret.m02 = m0.m02 - m1.m02;
		ret.m10 = m0.m10 - m1.m10;
		ret.m11 = m0.m11 - m1.m11;
		ret.m12 = m0.m12 - m1.m12;
		ret.m20 = m0.m20 - m1.m20;
		ret.m21 = m0.m21 - m1.m21;
		ret.m22 = m0.m22 - m1.m22;
	#endif
	return ret;
}
UNIGINE_INLINE mat3 operator-(const mat3 &m0, const mat3 &m1)
{
	mat3 ret;
	return sub(ret, m0, m1);
}
UNIGINE_INLINE mat3 &mat3::operator-=(const mat3 &m)
{
	return sub(*this, *this, m);
}

UNIGINE_INLINE mat3 &transpose(mat3 &ret, const mat3 &m)
{
	ret.m00 = m.m00;
	ret.m01 = m.m10;
	ret.m02 = m.m20;
	ret.m10 = m.m01;
	ret.m11 = m.m11;
	ret.m12 = m.m21;
	ret.m20 = m.m02;
	ret.m21 = m.m12;
	ret.m22 = m.m22;
	return ret;
}
UNIGINE_INLINE mat3 transpose(const mat3 &m)
{
	mat3 ret;
	return transpose(ret, m);
}

UNIGINE_INLINE mat3 &orthonormalize(mat3 &ret, const mat3 &m)
{
	#ifdef USE_SSE
		__m128 m_col0 = m.sse.v0;
		__m128 m_col1 = m.sse.v1;
		__m128 x_yzx = _MM_SWIZZLE(m_col0, Y, Z, X, W);
		__m128 x_zxy = _MM_SWIZZLE(m_col0, Z, X, Y, W);
		__m128 y_yzx = _MM_SWIZZLE(m_col1, Y, Z, X, W);
		__m128 y_zxy = _MM_SWIZZLE(m_col1, Z, X, Y, W);
		__m128 z = _mm_sub_ps(_mm_mul_ps(x_yzx, y_zxy), _mm_mul_ps(x_zxy, y_yzx));
		__m128 z_yzx = _MM_SWIZZLE(z, Y, Z, X, W);
		__m128 z_zxy = _MM_SWIZZLE(z, Z, X, Y, W);
		__m128 y = _mm_sub_ps(_mm_mul_ps(z_yzx, x_zxy), _mm_mul_ps(z_zxy, x_yzx));
		__m128 col_0 = _mm_mul_ps(m_col0, m_col0);
		__m128 col_1 = _mm_mul_ps(y, y);
		__m128 col_2 = _mm_mul_ps(z, z);
		__m128 res_0 = _mm_shuffle_ps(col_0, col_1, _MM_PERM2(X, Y, X, Y));
		__m128 res_1 = _mm_shuffle_ps(col_0, col_1, _MM_PERM2(Z, W, Z, W));
		__m128 res_2 = _mm_shuffle_ps(col_2, col_2, _MM_PERM2(X, Y, X, Y));
		__m128 res_3 = _mm_shuffle_ps(col_2, col_2, _MM_PERM2(Z, W, Z, W));
		__m128 row_0 = _mm_shuffle_ps(res_0, res_2, _MM_PERM2(X, Z, X, Z));
		__m128 row_1 = _mm_shuffle_ps(res_0, res_2, _MM_PERM2(Y, W, Y, W));
		__m128 row_2 = _mm_shuffle_ps(res_1, res_3, _MM_PERM2(X, Z, X, Z));
		__m128 ilength = _mm_rsqrt_ps_nr(_mm_add_ps(_mm_add_ps(row_0, row_1), row_2));
		ret.sse.v0 = _mm_mul_ps(m_col0, _MM_SWIZZLE(ilength, X, X, X, X));
		ret.sse.v1 = _mm_mul_ps(y, _MM_SWIZZLE(ilength, Y, Y, Y, Y));
		ret.sse.v2 = _mm_mul_ps(z, _MM_SWIZZLE(ilength, Z, Z, Z, Z));
	#else
		vec3 x = vec3(m.m00, m.m10, m.m20);
		vec3 y = vec3(m.m01, m.m11, m.m21);
		vec3 z = cross(x, y);
		cross(y, z, x);
		x.normalize();
		y.normalize();
		z.normalize();
		ret.m00 = x.x;
		ret.m01 = y.x;
		ret.m02 = z.x;
		ret.m10 = x.y;
		ret.m11 = y.y;
		ret.m12 = z.y;
		ret.m20 = x.z;
		ret.m21 = y.z;
		ret.m22 = z.z;
	#endif
	return ret;
}
UNIGINE_INLINE mat3 orthonormalize(const mat3 &m)
{
	mat3 ret;
	return orthonormalize(ret, m);
}

UNIGINE_INLINE mat3 &inverse(mat3 &ret, const mat3 &m)
{
	float idet = Math::rcp(determinant(m));
	ret.m00 = (m.m11 * m.m22 - m.m12 * m.m21) * idet;
	ret.m10 = -(m.m10 * m.m22 - m.m12 * m.m20) * idet;
	ret.m20 = (m.m10 * m.m21 - m.m11 * m.m20) * idet;
	ret.m01 = -(m.m01 * m.m22 - m.m02 * m.m21) * idet;
	ret.m11 = (m.m00 * m.m22 - m.m02 * m.m20) * idet;
	ret.m21 = -(m.m00 * m.m21 - m.m01 * m.m20) * idet;
	ret.m02 = (m.m01 * m.m12 - m.m02 * m.m11) * idet;
	ret.m12 = -(m.m00 * m.m12 - m.m02 * m.m10) * idet;
	ret.m22 = (m.m00 * m.m11 - m.m01 * m.m10) * idet;
	return ret;
}
UNIGINE_INLINE mat3 &inverse(mat3 &ret, const mat3 &m, float det)
{
	float idet = Math::rcp(det);
	ret.m00 = (m.m11 * m.m22 - m.m12 * m.m21) * idet;
	ret.m10 = -(m.m10 * m.m22 - m.m12 * m.m20) * idet;
	ret.m20 = (m.m10 * m.m21 - m.m11 * m.m20) * idet;
	ret.m01 = -(m.m01 * m.m22 - m.m02 * m.m21) * idet;
	ret.m11 = (m.m00 * m.m22 - m.m02 * m.m20) * idet;
	ret.m21 = -(m.m00 * m.m21 - m.m01 * m.m20) * idet;
	ret.m02 = (m.m01 * m.m12 - m.m02 * m.m11) * idet;
	ret.m12 = -(m.m00 * m.m12 - m.m02 * m.m10) * idet;
	ret.m22 = (m.m00 * m.m11 - m.m01 * m.m10) * idet;
	return ret;
}
UNIGINE_INLINE mat3 inverse(const mat3 &m)
{
	mat3 ret;
	return inverse(ret, m);
}
UNIGINE_INLINE mat3 inverse(const mat3 &m, float det)
{
	mat3 ret;
	return inverse(ret, m, det);
}

UNIGINE_INLINE mat3 rotate3(const vec3 &axis, float angle)
{
	mat3 ret;
	ret.setRotate(axis, angle);
	return ret;
}
UNIGINE_INLINE mat3 rotate3(float x, float y, float z, float angle)
{
	return rotate3(vec3(x, y, z), angle);
}

UNIGINE_INLINE mat3 rotateX3(float angle)
{
	mat3 ret;
	ret.setRotateX(angle);
	return ret;
}
UNIGINE_INLINE mat3 rotateY3(float angle)
{
	mat3 ret;
	ret.setRotateY(angle);
	return ret;
}
UNIGINE_INLINE mat3 rotateZ3(float angle)
{
	mat3 ret;
	ret.setRotateZ(angle);
	return ret;
}

UNIGINE_INLINE mat3 scale3(const vec3 &v)
{
	mat3 ret;
	ret.setScale(v);
	return ret;
}
UNIGINE_INLINE mat3 scale3(float x, float y, float z)
{
	return scale3(vec3(x, y, z));
}

UNIGINE_INLINE mat3 jacobi(const mat3 &m, mat3 &v)
{
	mat3 j, ret = m;
	v.setIdentity();
	float prev = Consts::INF;
	const int num_iterations = 50;
	for (int n = 0; n < num_iterations; n++)
	{
		int p = 0;
		int q = 1;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (i == j)
					continue;
				if (abs(ret.get(i, j)) > abs(ret.get(p, q)))
				{
					p = i;
					q = j;
				}
			}
		}
		float c = 1.0f;
		float s = 0.0f;
		if (abs(ret.get(p, q)) > Consts::EPS)
		{
			float r = (ret.get(q, q) - ret.get(p, p)) / (ret.get(p, q) * 2.0f);
			if (r >= 0.0f)
				r = Math::rcp(r + Math::fsqrt(1.0f + r * r));
			else
				r = -Math::rcp(-r + Math::fsqrt(1.0f + r * r));
			c = Math::rsqrt(1.0f + r * r);
			s = r * c;
		}
		j.setIdentity();
		j.set(p, p, c);
		j.set(p, q, s);
		j.set(q, p, -s);
		j.set(q, q, c);
		v = v * j;
		ret = transpose(j) * ret * j;
		float sum = 0.0f;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (i == j)
					continue;
				sum += ret.get(i, j) * ret.get(i, j);
			}
		}
		if (prev <= sum)
			break;
		prev = sum;
	}
	return ret;
}

UNIGINE_INLINE vec3 decomposeRotationXYZ(const mat3 &t)
{
	vec3 r = vec3_zero;
	if (t.m02 < 1.0f)
	{
		if (t.m02 > -1.0f)
		{
			r.x = atan2(-t.m12, t.m22);
			r.y = asin(t.m02);
			r.z = atan2(-t.m01, t.m00);
		} else
		{
			r.x = -atan2(t.m10, t.m11);
			r.y = -Consts::PI05;
		}
	} else
	{
		r.x = atan2(t.m10, t.m11);
		r.y = Consts::PI05;
	}
	return r * Consts::RAD2DEG;
}
UNIGINE_INLINE vec3 decomposeRotationXZY(const mat3 &t)
{
	vec3 r = vec3_zero;
	if (t.m01 < 1.0f)
	{
		if (t.m01 > -1.0f)
		{
			r.x = atan2(t.m21, t.m11);
			r.y = atan2(t.m02, t.m00);
			r.z = asin(-t.m01);
		} else
		{
			r.x = -atan2(-t.m20, t.m22);
			r.z = Consts::PI05;
		}
	} else
	{
		r.x = atan2(-t.m20, t.m22);
		r.z = -Consts::PI05;
	}
	return r * Consts::RAD2DEG;
}
UNIGINE_INLINE vec3 decomposeRotationYXZ(const mat3 &t)
{
	vec3 r = vec3_zero;
	if (t.m12 < 1.0f)
	{
		if (t.m12 > -1.0f)
		{
			r.x = asin(-t.m12);
			r.y = atan2(t.m02, t.m22);
			r.z = atan2(t.m10, t.m11);
		} else
		{
			r.x = Consts::PI05;
			r.y = -atan2(-t.m01, t.m00);
		}
	} else
	{
		r.x = -Consts::PI05;
		r.y = atan2(-t.m01, t.m00);
	}
	return r * Consts::RAD2DEG;
}
UNIGINE_INLINE vec3 decomposeRotationYZX(const mat3 &t)
{
	vec3 r = vec3_zero;
	if (t.m10 < 1.0f)
	{
		if (t.m10 > -1.0f)
		{
			r.x = atan2(-t.m12, t.m11);
			r.y = atan2(-t.m20, t.m00);
			r.z = asin(t.m10);
		} else
		{
			r.y = -atan2(t.m21, t.m22);
			r.z = -Consts::PI05;
		}
	} else
	{
		r.y = atan2(t.m21, t.m22);
		r.z = Consts::PI05;
	}
	return r * Consts::RAD2DEG;
}
UNIGINE_INLINE vec3 decomposeRotationZXY(const mat3 &t)
{
	vec3 r = vec3_zero;
	if (t.m21 < 1.0f)
	{
		if (t.m21 > -1.0f)
		{
			r.x = asin(t.m21);
			r.y = atan2(-t.m20, t.m22);
			r.z = atan2(-t.m01, t.m11);
		} else
		{
			r.x = -Consts::PI05;
			r.z = -atan2(t.m02, t.m00);
		}
	} else
	{
		r.x = Consts::PI05;
		r.z = atan2(t.m02, t.m00);
	}
	return r * Consts::RAD2DEG;
}
UNIGINE_INLINE vec3 decomposeRotationZYX(const mat3 &t)
{
	vec3 r = vec3_zero;
	if (t.m20 < 1.0f)
	{
		if (t.m20 > -1.0f)
		{
			r.x = atan2(t.m21, t.m22);
			r.y = asin(-t.m20);
			r.z = atan2(t.m10, t.m00);
		} else
		{
			r.y = Consts::PI05;
			r.z = -atan2(t.m01, t.m02);
		}
	} else
	{
		r.y = -Consts::PI05;
		r.z = atan2(-t.m01, -t.m02);
	}
	return r * Consts::RAD2DEG;
}

}
}

#ifdef __GNUC__
	#pragma GCC diagnostic pop
#endif
