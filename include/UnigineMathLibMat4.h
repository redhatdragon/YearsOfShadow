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
#include "UnigineMathLibMat3.h"

#ifdef __GNUC__
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wreorder"
#endif

namespace Unigine
{
namespace Math
{

struct alignas(16) mat4
{
	UNIGINE_INLINE constexpr mat4(float v, ConstexprTag)
		: m00(v), m10(v), m20(v), m30(v)
		, m01(v), m11(v), m21(v), m31(v)
		, m02(v), m12(v), m22(v), m32(v)
		, m03(v), m13(v), m23(v), m33(v)
	{}
	UNIGINE_INLINE constexpr mat4(
		float m00_, float m10_, float m20_, float m30_,
		float m01_, float m11_, float m21_, float m31_,
		float m02_, float m12_, float m22_, float m32_,
		float m03_, float m13_, float m23_, float m33_
		, ConstexprTag)
		: m00(m00_), m10(m10_), m20(m20_), m30(m30_)
		, m01(m01_), m11(m11_), m21(m21_), m31(m31_)
		, m02(m02_), m12(m12_), m22(m22_), m32(m32_)
		, m03(m03_), m13(m13_), m23(m23_), m33(m33_)
	{}

	UNIGINE_INLINE mat4()
		: m00(1.0f)
		, m01(0.0f)
		, m02(0.0f)
		, m03(0.0f)
		, m10(0.0f)
		, m11(1.0f)
		, m12(0.0f)
		, m13(0.0f)
		, m20(0.0f)
		, m21(0.0f)
		, m22(1.0f)
		, m23(0.0f)
		, m30(0.0f)
		, m31(0.0f)
		, m32(0.0f)
		, m33(1.0f)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE mat4(
		float m00_, float m10_, float m20_, float m30_,
		float m01_, float m11_, float m21_, float m31_,
		float m02_, float m12_, float m22_, float m32_,
		float m03_, float m13_, float m23_, float m33_)
		: m00(m00_), m10(m10_), m20(m20_), m30(m30_)
		, m01(m01_), m11(m11_), m21(m21_), m31(m31_)
		, m02(m02_), m12(m12_), m22(m22_), m32(m32_)
		, m03(m03_), m13(m13_), m23(m23_), m33(m33_)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE mat4(const mat4 &m)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m);
	}
	UNIGINE_INLINE explicit mat4(float v)
		: m00(v)
		, m10(v)
		, m20(v)
		, m30(v)
		, m01(v)
		, m11(v)
		, m21(v)
		, m31(v)
		, m02(v)
		, m12(v)
		, m22(v)
		, m32(v)
		, m03(v)
		, m13(v)
		, m23(v)
		, m33(v)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit mat4(const mat2 &m)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m);
	}
	UNIGINE_INLINE explicit mat4(const mat3 &m)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m);
	}
	UNIGINE_INLINE explicit mat4(const dmat4 &m)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m);
	}
	UNIGINE_INLINE explicit mat4(const quat &q)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(q);
	}
	UNIGINE_INLINE explicit mat4(const float *m, int transpose)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m, transpose);
	}
	UNIGINE_INLINE explicit mat4(const float *m)
		: m00(m[0])
		, m10(m[1])
		, m20(m[2])
		, m30(m[3])
		, m01(m[4])
		, m11(m[5])
		, m21(m[6])
		, m31(m[7])
		, m02(m[8])
		, m12(m[9])
		, m22(m[10])
		, m32(m[11])
		, m03(m[12])
		, m13(m[13])
		, m23(m[14])
		, m33(m[15])
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE mat4(const mat3 &m, const vec3 &v)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m, v);
	}
	UNIGINE_INLINE mat4(const quat &q, const vec3 &v)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(q, v);
	}
	UNIGINE_INLINE mat4(const vec4 &c0, const vec4 &c1, const vec4 &c2, const vec4 &c3)
	{
		setColumn(0, c0);
		setColumn(1, c1);
		setColumn(2, c2);
		setColumn(3, c3);
		UNIGINE_ASSERT_ALIGNED16(this);
	}

	UNIGINE_INLINE void set(int row, int column, float v)
	{
		assert((unsigned int)row < 4 && "mat4::set(): bad row");
		assert((unsigned int)column < 4 && "mat4::set(): bad column");
		mat[column * 4 + row] = v;
	}
	UNIGINE_INLINE void set(const mat2 &m)
	{
		m00 = m.m00;
		m01 = m.m01;
		m02 = 0.0f;
		m03 = 0.0f;
		m10 = m.m10;
		m11 = m.m11;
		m12 = 0.0f;
		m13 = 0.0f;
		m20 = 0.0f;
		m21 = 0.0f;
		m22 = 1.0f;
		m23 = 0.0f;
		m30 = 0.0f;
		m31 = 0.0f;
		m32 = 0.0f;
		m33 = 1.0f;
	}
	UNIGINE_INLINE void set(const mat3 &m)
	{
		m00 = m.m00;
		m01 = m.m01;
		m02 = m.m02;
		m03 = 0.0f;
		m10 = m.m10;
		m11 = m.m11;
		m12 = m.m12;
		m13 = 0.0f;
		m20 = m.m20;
		m21 = m.m21;
		m22 = m.m22;
		m23 = 0.0f;
		m30 = 0.0f;
		m31 = 0.0f;
		m32 = 0.0f;
		m33 = 1.0f;
	}
	UNIGINE_INLINE void set(const mat4 &m)
	{
		sse.v0 = m.sse.v0;
		sse.v1 = m.sse.v1;
		sse.v2 = m.sse.v2;
		sse.v3 = m.sse.v3;
	}
	UNIGINE_INLINE void set(const dmat4 &m);
	UNIGINE_INLINE void set(const quat &q);
	UNIGINE_INLINE void set(const float *m, int transpose = 0)
	{
		if (transpose)
		{
			m00 = m[0];
			m01 = m[1];
			m02 = m[2];
			m03 = m[3];
			m10 = m[4];
			m11 = m[5];
			m12 = m[6];
			m13 = m[7];
			m20 = m[8];
			m21 = m[9];
			m22 = m[10];
			m23 = m[11];
			m30 = m[12];
			m31 = m[13];
			m32 = m[14];
			m33 = m[15];
		} else
		{
			m00 = m[0];
			m01 = m[4];
			m02 = m[8];
			m03 = m[12];
			m10 = m[1];
			m11 = m[5];
			m12 = m[9];
			m13 = m[13];
			m20 = m[2];
			m21 = m[6];
			m22 = m[10];
			m23 = m[14];
			m30 = m[3];
			m31 = m[7];
			m32 = m[11];
			m33 = m[15];
		}
	}
	UNIGINE_INLINE void set(const mat3 &m, const vec3 &v)
	{
		m00 = m.m00;
		m01 = m.m01;
		m02 = m.m02;
		m03 = v.x;
		m10 = m.m10;
		m11 = m.m11;
		m12 = m.m12;
		m13 = v.y;
		m20 = m.m20;
		m21 = m.m21;
		m22 = m.m22;
		m23 = v.z;
		m30 = 0.0f;
		m31 = 0.0f;
		m32 = 0.0f;
		m33 = 1.0f;
	}
	UNIGINE_INLINE void set(const quat &q, const vec3 &v);

	UNIGINE_INLINE void get(float *m, int transpose = 0) const
	{
		if (transpose)
		{
			m[0] = m00;
			m[1] = m01;
			m[2] = m02;
			m[3] = m03;
			m[4] = m10;
			m[5] = m11;
			m[6] = m12;
			m[7] = m13;
			m[8] = m20;
			m[9] = m21;
			m[10] = m22;
			m[11] = m23;
			m[12] = m30;
			m[13] = m31;
			m[14] = m32;
			m[15] = m33;
		} else
		{
			m[0] = m00;
			m[4] = m01;
			m[8] = m02;
			m[12] = m03;
			m[1] = m10;
			m[5] = m11;
			m[9] = m12;
			m[13] = m13;
			m[2] = m20;
			m[6] = m21;
			m[10] = m22;
			m[14] = m23;
			m[3] = m30;
			m[7] = m31;
			m[11] = m32;
			m[15] = m33;
		}
	}
	UNIGINE_INLINE float *get() { return mat; }
	UNIGINE_INLINE const float *get() const { return mat; }
	UNIGINE_INLINE float &get(int row, int column)
	{
		assert((unsigned int)row < 4 && "mat4::get(): bad row");
		assert((unsigned int)column < 4 && "mat4::get(): bad column");
		return mat[column * 4 + row];
	}
	UNIGINE_INLINE float get(int row, int column) const
	{
		assert((unsigned int)row < 4 && "mat4::get(): bad row");
		assert((unsigned int)column < 4 && "mat4::get(): bad column");
		return mat[column * 4 + row];
	}

	UNIGINE_INLINE operator float *() { return mat; }
	UNIGINE_INLINE operator const float *() const { return mat; }
	UNIGINE_INLINE operator void *() { return mat; }
	UNIGINE_INLINE operator const void *() const { return mat; }

	UNIGINE_INLINE float &operator[](int i)
	{
		assert((unsigned int)i < 16 && "mat4::operator[](): bad index");
		return mat[i];
	}
	UNIGINE_INLINE float operator[](int i) const
	{
		assert((unsigned int)i < 16 && "mat4::operator[](): bad index");
		return mat[i];
	}

	UNIGINE_INLINE mat4 &operator=(const mat4 &m)
	{
		set(m);
		return *this;
	}
	UNIGINE_INLINE mat4 operator-() const
	{
		mat4 ret;
		ret.m00 = -m00;
		ret.m01 = -m01;
		ret.m02 = -m02;
		ret.m03 = -m03;
		ret.m10 = -m10;
		ret.m11 = -m11;
		ret.m12 = -m12;
		ret.m13 = -m13;
		ret.m20 = -m20;
		ret.m21 = -m21;
		ret.m22 = -m22;
		ret.m23 = -m23;
		ret.m30 = -m30;
		ret.m31 = -m31;
		ret.m32 = -m32;
		ret.m33 = -m33;
		return ret;
	}

	UNIGINE_INLINE mat4 &operator*=(float v);
	UNIGINE_INLINE mat4 &operator*=(const mat4 &m);
	UNIGINE_INLINE mat4 &operator+=(const mat4 &m);
	UNIGINE_INLINE mat4 &operator-=(const mat4 &m);

	UNIGINE_INLINE void setRow(int row, const vec4 &v)
	{
		assert(row < 4 && row >= 0 && "mat4::setRow(): bad row");
		mat[row + 0] = v.x;
		mat[row + 4] = v.y;
		mat[row + 8] = v.z;
		mat[row + 12] = v.w;
	}
	UNIGINE_INLINE void setRow3(int row, const vec3 &v)
	{
		assert(row < 4 && row >= 0 && "mat4::setRow3(): bad row");
		mat[row + 0] = v.x;
		mat[row + 4] = v.y;
		mat[row + 8] = v.z;
	}
	UNIGINE_INLINE vec4 getRow(int row) const
	{
		assert(row < 4 && row >= 0 && "mat4::getRow(): bad row");
		return vec4(mat[row + 0], mat[row + 4], mat[row + 8], mat[row + 12]);
	}
	UNIGINE_INLINE vec3 getRow3(int row) const
	{
		assert(row < 4 && row >= 0 && "mat4::getRow3(): bad row");
		return vec3(mat[row + 0], mat[row + 4], mat[row + 8]);
	}

	UNIGINE_INLINE void setColumn(int column, const vec4 &v)
	{
		assert(column < 4 && column >= 0 && "mat4::setColumn(): bad column");
		column *= 4;
		mat[column + 0] = v.x;
		mat[column + 1] = v.y;
		mat[column + 2] = v.z;
		mat[column + 3] = v.w;
	}
	UNIGINE_INLINE void setColumn3(int column, const vec3 &v)
	{
		assert(column < 4 && column >= 0 && "mat4::setColumn3(): bad column");
		column *= 4;
		mat[column + 0] = v.x;
		mat[column + 1] = v.y;
		mat[column + 2] = v.z;
	}
	UNIGINE_INLINE vec4 getColumn(int column) const
	{
		assert(column < 4 && column >= 0 && "mat4::getColumn(): bad column");
		return vec4(mat + column * 4);
	}
	UNIGINE_INLINE vec3 getColumn3(int column) const
	{
		assert(column < 4 && column >= 0 && "mat4::getColumn3(): bad column");
		return vec3(mat + column * 4);
	}

	UNIGINE_INLINE vec3 getAxisX() const { return getColumn3(0).normalize(); }
	UNIGINE_INLINE vec3 getAxisY() const { return getColumn3(1).normalize(); }
	UNIGINE_INLINE vec3 getAxisZ() const { return getColumn3(2).normalize(); }

	UNIGINE_INLINE void setDiagonal(const vec4 &v)
	{
		m00 = v.x;
		m01 = 0.0f;
		m02 = 0.0f;
		m03 = 0.0f;
		m10 = 0.0f;
		m11 = v.y;
		m12 = 0.0f;
		m13 = 0.0f;
		m20 = 0.0f;
		m21 = 0.0f;
		m22 = v.z;
		m23 = 0.0f;
		m30 = 0.0f;
		m31 = 0.0f;
		m32 = 0.0f;
		m33 = v.w;
	}
	UNIGINE_INLINE vec4 getDiagonal() const { return vec4(m00, m11, m22, m33); }

	UNIGINE_INLINE void setZero()
	{
		m00 = 0.0f;
		m01 = 0.0f;
		m02 = 0.0f;
		m03 = 0.0f;
		m10 = 0.0f;
		m11 = 0.0f;
		m12 = 0.0f;
		m13 = 0.0f;
		m20 = 0.0f;
		m21 = 0.0f;
		m22 = 0.0f;
		m23 = 0.0f;
		m30 = 0.0f;
		m31 = 0.0f;
		m32 = 0.0f;
		m33 = 0.0f;
	}
	UNIGINE_INLINE void setIdentity()
	{
		m00 = 1.0f;
		m01 = 0.0f;
		m02 = 0.0f;
		m03 = 0.0f;
		m10 = 0.0f;
		m11 = 1.0f;
		m12 = 0.0f;
		m13 = 0.0f;
		m20 = 0.0f;
		m21 = 0.0f;
		m22 = 1.0f;
		m23 = 0.0f;
		m30 = 0.0f;
		m31 = 0.0f;
		m32 = 0.0f;
		m33 = 1.0f;
	}

	UNIGINE_INLINE void setTranslate(const vec3 &v)
	{
		m00 = 1.0f;
		m01 = 0.0f;
		m02 = 0.0f;
		m03 = v.x;
		m10 = 0.0f;
		m11 = 1.0f;
		m12 = 0.0f;
		m13 = v.y;
		m20 = 0.0f;
		m21 = 0.0f;
		m22 = 1.0f;
		m23 = v.z;
		m30 = 0.0f;
		m31 = 0.0f;
		m32 = 0.0f;
		m33 = 1.0f;
	}
	UNIGINE_INLINE vec3 getTranslate() const { return vec3(m03, m13, m23); }

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
		m03 = 0.0f;

		m10 = (1.0f - c) * xy + zs;
		m11 = (1.0f - c) * yy + c;
		m12 = (1.0f - c) * yz - xs;
		m13 = 0.0f;

		m20 = (1.0f - c) * zx - ys;
		m21 = (1.0f - c) * yz + xs;
		m22 = (1.0f - c) * zz + c;
		m23 = 0.0f;

		m30 = 0.0f;
		m31 = 0.0f;
		m32 = 0.0f;
		m33 = 1.0f;
	}
	UNIGINE_INLINE quat getRotate() const;

	UNIGINE_INLINE void setRotateX(float angle)
	{
		float s, c;
		Math::sincos(angle * Consts::DEG2RAD, s, c);

		m00 = 1.0f;
		m01 = 0.0f;
		m02 = 0.0f;
		m03 = 0.0f;

		m10 = 0.0f;
		m11 = c;
		m12 = -s;
		m13 = 0.0f;

		m20 = 0.0f;
		m21 = s;
		m22 = c;
		m23 = 0.0f;

		m30 = 0.0f;
		m31 = 0.0f;
		m32 = 0.0f;
		m33 = 1.0f;
	}
	UNIGINE_INLINE void setRotateY(float angle)
	{
		float s, c;
		Math::sincos(angle * Consts::DEG2RAD, s, c);

		m00 = c;
		m01 = 0.0f;
		m02 = s;
		m03 = 0.0f;

		m10 = 0.0f;
		m11 = 1.0f;
		m12 = 0.0f;
		m13 = 0.0f;

		m20 = -s;
		m21 = 0.0f;
		m22 = c;
		m23 = 0.0f;

		m30 = 0.0f;
		m31 = 0.0f;
		m32 = 0.0f;
		m33 = 1.0f;
	}
	UNIGINE_INLINE void setRotateZ(float angle)
	{
		float s, c;
		Math::sincos(angle * Consts::DEG2RAD, s, c);

		m00 = c;
		m01 = -s;
		m02 = 0.0f;
		m03 = 0.0f;

		m10 = s;
		m11 = c;
		m12 = 0.0f;
		m13 = 0.0f;

		m20 = 0.0f;
		m21 = 0.0f;
		m22 = 1.0f;
		m23 = 0.0f;

		m30 = 0.0f;
		m31 = 0.0f;
		m32 = 0.0f;
		m33 = 1.0f;
	}

	UNIGINE_INLINE void setScale(const vec3 &v)
	{
		m00 = v.x;
		m01 = 0.0f;
		m02 = 0.0f;
		m03 = 0.0f;

		m10 = 0.0f;
		m11 = v.y;
		m12 = 0.0f;
		m13 = 0.0f;

		m20 = 0.0f;
		m21 = 0.0f;
		m22 = v.z;
		m23 = 0.0f;

		m30 = 0.0f;
		m31 = 0.0f;
		m32 = 0.0f;
		m33 = 1.0f;
	}
	UNIGINE_INLINE vec3 getScale() const
	{
		mat3 rotate, rotation = mat3(*this);
		orthonormalize(rotate, rotation);
		float sx = rotate.m00 * rotation.m00 + rotate.m10 * rotation.m10 + rotate.m20 * rotation.m20;
		float sy = rotate.m01 * rotation.m01 + rotate.m11 * rotation.m11 + rotate.m21 * rotation.m21;
		float sz = rotate.m02 * rotation.m02 + rotate.m12 * rotation.m12 + rotate.m22 * rotation.m22;
		return vec3(sx, sy, sz);
	}

	UNIGINE_INLINE float trace() const { return m00 + m11 + m22 + m33; }
	UNIGINE_INLINE float determinant() const
	{
		float det = m00 * (m11 * (m22 * m33 - m23 * m32) - m12 * (m21 * m33 - m23 * m31) + m13 * (m21 * m32 - m22 * m31));
		det -= m01 * (m10 * (m22 * m33 - m23 * m32) - m12 * (m20 * m33 - m23 * m30) + m13 * (m20 * m32 - m22 * m30));
		det += m02 * (m10 * (m21 * m33 - m23 * m31) - m11 * (m20 * m33 - m23 * m30) + m13 * (m20 * m31 - m21 * m30));
		det -= m03 * (m10 * (m21 * m32 - m22 * m31) - m11 * (m20 * m32 - m22 * m30) + m12 * (m20 * m31 - m21 * m30));
		return det;
	}
	UNIGINE_INLINE float determinant3() const
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
			__m128 v3;
		};
	#endif

	union
	{
		struct
		{
			float m00, m10, m20, m30;
			float m01, m11, m21, m31;
			float m02, m12, m22, m32;
			float m03, m13, m23, m33;
		};
		float mat[16];

		#ifdef USE_SSE
			sse_data sse; // DO NOT TOUCH UNLESS ABSOLUTELY SURE
		#endif
	};
};

constexpr mat4 mat4_zero(0.0f, ConstexprTag{});
constexpr mat4 mat4_one(1.0f, ConstexprTag{});
constexpr mat4 mat4_identity(1.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 1.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 1.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 1.0f, ConstexprTag{});

UNIGINE_INLINE mat4 translate(const vec3 &v)
{
	mat4 ret;
	ret.setTranslate(v);
	return ret;
}
UNIGINE_INLINE mat4 translate(const vec4 &v)
{
	mat4 ret;
	ret.setTranslate(vec3(v));
	return ret;
}
UNIGINE_INLINE mat4 translate(const vec2 &v)
{
	mat4 ret;
	ret.setTranslate(vec3(v));
	return ret;
}
UNIGINE_INLINE mat4 translate(float x, float y, float z)
{
	mat4 ret;
	ret.setTranslate(vec3(x, y, z));
	return ret;
}

UNIGINE_INLINE mat4 rotate(const vec3 &axis, float angle)
{
	mat4 ret;
	ret.setRotate(axis, angle);
	return ret;
}
UNIGINE_INLINE mat4 rotate(float x, float y, float z, float angle)
{
	return rotate(vec3(x, y, z), angle);
}

UNIGINE_INLINE mat4 rotateX(float angle)
{
	mat4 ret;
	ret.setRotateX(angle);
	return ret;
}
UNIGINE_INLINE mat4 rotateY(float angle)
{
	mat4 ret;
	ret.setRotateY(angle);
	return ret;
}
UNIGINE_INLINE mat4 rotateZ(float angle)
{
	mat4 ret;
	ret.setRotateZ(angle);
	return ret;
}

UNIGINE_INLINE mat4 scale(const vec3 &v)
{
	mat4 ret;
	ret.setScale(v);
	return ret;
}
UNIGINE_INLINE mat4 scale(float x, float y, float z)
{
	return scale(vec3(x, y, z));
}
UNIGINE_INLINE mat4 scale(float x)
{
	return scale(vec3(x, x, x));
}

UNIGINE_INLINE float trace(const mat4 &m) { return m.trace(); }
UNIGINE_INLINE float determinant(const mat4 &m) { return m.determinant(); }
UNIGINE_INLINE float determinant3(const mat4 &m) { return m.determinant3(); }

UNIGINE_INLINE int compare(const mat4 &m0, const mat4 &m1)
{
	return (compare(m0.m00, m1.m00) && compare(m0.m10, m1.m10) && compare(m0.m20, m1.m20) && compare(m0.m30, m1.m30) &&
			compare(m0.m01, m1.m01) && compare(m0.m11, m1.m11) && compare(m0.m21, m1.m21) && compare(m0.m31, m1.m31) &&
			compare(m0.m02, m1.m02) && compare(m0.m12, m1.m12) && compare(m0.m22, m1.m22) && compare(m0.m32, m1.m32) &&
			compare(m0.m03, m1.m03) && compare(m0.m13, m1.m13) && compare(m0.m23, m1.m23) && compare(m0.m33, m1.m33));
}
UNIGINE_INLINE int compare(const mat4 &m0, const mat4 &m1, float epsilon)
{
	return (compare(m0.m00, m1.m00, epsilon) && compare(m0.m10, m1.m10, epsilon) && compare(m0.m20, m1.m20, epsilon) && compare(m0.m30, m1.m30, epsilon) &&
			compare(m0.m01, m1.m01, epsilon) && compare(m0.m11, m1.m11, epsilon) && compare(m0.m21, m1.m21, epsilon) && compare(m0.m31, m1.m31, epsilon) &&
			compare(m0.m02, m1.m02, epsilon) && compare(m0.m12, m1.m12, epsilon) && compare(m0.m22, m1.m22, epsilon) && compare(m0.m32, m1.m32, epsilon) &&
			compare(m0.m03, m1.m03, epsilon) && compare(m0.m13, m1.m13, epsilon) && compare(m0.m23, m1.m23, epsilon) && compare(m0.m33, m1.m33, epsilon));
}
UNIGINE_INLINE int operator==(const mat4 &m0, const mat4 &m1) { return compare(m0, m1); }
UNIGINE_INLINE int operator!=(const mat4 &m0, const mat4 &m1) { return !compare(m0, m1); }

UNIGINE_INLINE mat4 &mul(mat4 &ret, const mat4 &m, float v)
{
	#ifdef USE_SSE
		__m128 temp = _mm_set1_ps(v);
		ret.sse.v0 = _mm_mul_ps(m.sse.v0, temp);
		ret.sse.v1 = _mm_mul_ps(m.sse.v1, temp);
		ret.sse.v2 = _mm_mul_ps(m.sse.v2, temp);
		ret.sse.v3 = _mm_mul_ps(m.sse.v3, temp);
	#else
		ret.m00 = m.m00 * v;
		ret.m01 = m.m01 * v;
		ret.m02 = m.m02 * v;
		ret.m03 = m.m03 * v;
		ret.m10 = m.m10 * v;
		ret.m11 = m.m11 * v;
		ret.m12 = m.m12 * v;
		ret.m13 = m.m13 * v;
		ret.m20 = m.m20 * v;
		ret.m21 = m.m21 * v;
		ret.m22 = m.m22 * v;
		ret.m23 = m.m23 * v;
		ret.m30 = m.m30 * v;
		ret.m31 = m.m31 * v;
		ret.m32 = m.m32 * v;
		ret.m33 = m.m33 * v;
	#endif
	return ret;
}
UNIGINE_INLINE mat4 &mul(mat4 &ret, const mat4 &m0, const mat4 &m1)
{
	#ifdef USE_SSE
		__m128 m0_col0 = m0.sse.v0;
		__m128 m0_col1 = m0.sse.v1;
		__m128 m0_col2 = m0.sse.v2;
		__m128 m0_col3 = m0.sse.v3;
	
		__m128 m1_col0 = m1.sse.v0;
		__m128 m1_col1 = m1.sse.v1;
		__m128 m1_col2 = m1.sse.v2;
		__m128 m1_col3 = m1.sse.v3;
	
		__m128 res_0 = _mm_mul_ps(m0_col0, _MM_SWIZZLE(m1_col0, X, X, X, X));
		__m128 res_1 = _mm_mul_ps(m0_col1, _MM_SWIZZLE(m1_col0, Y, Y, Y, Y));
		__m128 res_2 = _mm_mul_ps(m0_col2, _MM_SWIZZLE(m1_col0, Z, Z, Z, Z));
		__m128 res_3 = _mm_mul_ps(m0_col3, _MM_SWIZZLE(m1_col0, W, W, W, W));
		ret.sse.v0 = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, res_3));
		res_0 = _mm_mul_ps(m0_col0, _MM_SWIZZLE(m1_col1, X, X, X, X));
		res_1 = _mm_mul_ps(m0_col1, _MM_SWIZZLE(m1_col1, Y, Y, Y, Y));
		res_2 = _mm_mul_ps(m0_col2, _MM_SWIZZLE(m1_col1, Z, Z, Z, Z));
		res_3 = _mm_mul_ps(m0_col3, _MM_SWIZZLE(m1_col1, W, W, W, W));
		ret.sse.v1 = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, res_3));
		res_0 = _mm_mul_ps(m0_col0, _MM_SWIZZLE(m1_col2, X, X, X, X));
		res_1 = _mm_mul_ps(m0_col1, _MM_SWIZZLE(m1_col2, Y, Y, Y, Y));
		res_2 = _mm_mul_ps(m0_col2, _MM_SWIZZLE(m1_col2, Z, Z, Z, Z));
		res_3 = _mm_mul_ps(m0_col3, _MM_SWIZZLE(m1_col2, W, W, W, W));
		ret.sse.v2 = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, res_3));
		res_0 = _mm_mul_ps(m0_col0, _MM_SWIZZLE(m1_col3, X, X, X, X));
		res_1 = _mm_mul_ps(m0_col1, _MM_SWIZZLE(m1_col3, Y, Y, Y, Y));
		res_2 = _mm_mul_ps(m0_col2, _MM_SWIZZLE(m1_col3, Z, Z, Z, Z));
		res_3 = _mm_mul_ps(m0_col3, _MM_SWIZZLE(m1_col3, W, W, W, W));
		ret.sse.v3 = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, res_3));
	#else
		ret.m00 = m0.m00 * m1.m00 + m0.m01 * m1.m10 + m0.m02 * m1.m20 + m0.m03 * m1.m30;
		ret.m10 = m0.m10 * m1.m00 + m0.m11 * m1.m10 + m0.m12 * m1.m20 + m0.m13 * m1.m30;
		ret.m20 = m0.m20 * m1.m00 + m0.m21 * m1.m10 + m0.m22 * m1.m20 + m0.m23 * m1.m30;
		ret.m30 = m0.m30 * m1.m00 + m0.m31 * m1.m10 + m0.m32 * m1.m20 + m0.m33 * m1.m30;
		ret.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11 + m0.m02 * m1.m21 + m0.m03 * m1.m31;
		ret.m11 = m0.m10 * m1.m01 + m0.m11 * m1.m11 + m0.m12 * m1.m21 + m0.m13 * m1.m31;
		ret.m21 = m0.m20 * m1.m01 + m0.m21 * m1.m11 + m0.m22 * m1.m21 + m0.m23 * m1.m31;
		ret.m31 = m0.m30 * m1.m01 + m0.m31 * m1.m11 + m0.m32 * m1.m21 + m0.m33 * m1.m31;
		ret.m02 = m0.m00 * m1.m02 + m0.m01 * m1.m12 + m0.m02 * m1.m22 + m0.m03 * m1.m32;
		ret.m12 = m0.m10 * m1.m02 + m0.m11 * m1.m12 + m0.m12 * m1.m22 + m0.m13 * m1.m32;
		ret.m22 = m0.m20 * m1.m02 + m0.m21 * m1.m12 + m0.m22 * m1.m22 + m0.m23 * m1.m32;
		ret.m32 = m0.m30 * m1.m02 + m0.m31 * m1.m12 + m0.m32 * m1.m22 + m0.m33 * m1.m32;
		ret.m03 = m0.m00 * m1.m03 + m0.m01 * m1.m13 + m0.m02 * m1.m23 + m0.m03 * m1.m33;
		ret.m13 = m0.m10 * m1.m03 + m0.m11 * m1.m13 + m0.m12 * m1.m23 + m0.m13 * m1.m33;
		ret.m23 = m0.m20 * m1.m03 + m0.m21 * m1.m13 + m0.m22 * m1.m23 + m0.m23 * m1.m33;
		ret.m33 = m0.m30 * m1.m03 + m0.m31 * m1.m13 + m0.m32 * m1.m23 + m0.m33 * m1.m33;
	#endif
	return ret;
}
UNIGINE_INLINE vec2 &mul(vec2 &ret, const mat4 &m, const vec2 &v)
{
	ret.x = m.m00 * v.x + m.m01 * v.y + m.m03;
	ret.y = m.m10 * v.x + m.m11 * v.y + m.m13;
	return ret;
}
UNIGINE_INLINE vec2 &mul(vec2 &ret, const vec2 &v, const mat4 &m)
{
	ret.x = m.m00 * v.x + m.m10 * v.y + m.m30;
	ret.y = m.m01 * v.x + m.m11 * v.y + m.m31;
	return ret;
}
UNIGINE_INLINE vec3 &mul(vec3 &ret, const mat4 &m, const vec3 &v)
{
	#ifdef USE_SSE
		__m128 vec = v.vec;
		__m128 res_0 = _mm_mul_ps(m.sse.v0, _MM_SWIZZLE(vec, X, X, X, X));
		__m128 res_1 = _mm_mul_ps(m.sse.v1, _MM_SWIZZLE(vec, Y, Y, Y, Y));
		__m128 res_2 = _mm_mul_ps(m.sse.v2, _MM_SWIZZLE(vec, Z, Z, Z, Z));
		ret = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, m.sse.v3));
	#else
		float x = v.x;
		float y = v.y;
		float z = v.z;
		ret.x = m.m00 * x + m.m01 * y + m.m02 * z + m.m03;
		ret.y = m.m10 * x + m.m11 * y + m.m12 * z + m.m13;
		ret.z = m.m20 * x + m.m21 * y + m.m22 * z + m.m23;
	#endif
	return ret;
}
UNIGINE_INLINE vec3 &mul(vec3 &ret, const vec3 &v, const mat4 &m)
{
	ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30;
	ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31;
	ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32;
	return ret;
}
UNIGINE_INLINE vec4 &mul(vec4 &ret, const mat4 &m, const vec4 &v)
{
	#ifdef USE_SSE
		const __m128 vec = v.vec;
		const __m128 res_0 = _mm_mul_ps(m.sse.v0, _MM_SWIZZLE(vec, X, X, X, X));
		const __m128 res_1 = _mm_mul_ps(m.sse.v1, _MM_SWIZZLE(vec, Y, Y, Y, Y));
		const __m128 res_2 = _mm_mul_ps(m.sse.v2, _MM_SWIZZLE(vec, Z, Z, Z, Z));
		const __m128 res_3 = _mm_mul_ps(m.sse.v3, _MM_SWIZZLE(vec, W, W, W, W));
		ret = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, res_3));
	#else
		float x = v.x;
		float y = v.y;
		float z = v.z;
		float w = v.w;
		ret.x = m.m00 * x + m.m01 * y + m.m02 * z + m.m03 * w;
		ret.y = m.m10 * x + m.m11 * y + m.m12 * z + m.m13 * w;
		ret.z = m.m20 * x + m.m21 * y + m.m22 * z + m.m23 * w;
		ret.w = m.m30 * x + m.m31 * y + m.m32 * z + m.m33 * w;
	#endif
	return ret;
}
UNIGINE_INLINE vec4 &mul(vec4 &ret, const vec4 &v, const mat4 &m)
{
	ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30 * v.w;
	ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31 * v.w;
	ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32 * v.w;
	ret.w = m.m03 * v.x + m.m13 * v.y + m.m23 * v.z + m.m33 * v.w;
	return ret;
}
UNIGINE_INLINE dvec2 &mul(dvec2 &ret, const mat4 &m, const dvec2 &v)
{
	ret.x = m.m00 * v.x + m.m01 * v.y + m.m03;
	ret.y = m.m10 * v.x + m.m11 * v.y + m.m13;
	return ret;
}
UNIGINE_INLINE dvec2 &mul(dvec2 &ret, const dvec2 &v, const mat4 &m)
{
	ret.x = m.m00 * v.x + m.m10 * v.y + m.m30;
	ret.y = m.m01 * v.x + m.m11 * v.y + m.m31;
	return ret;
}
UNIGINE_INLINE dvec3 &mul(dvec3 &ret, const mat4 &m, const dvec3 &v)
{
	ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03;
	ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13;
	ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23;
	return ret;
}
UNIGINE_INLINE dvec3 &mul(dvec3 &ret, const dvec3 &v, const mat4 &m)
{
	ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30;
	ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31;
	ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32;
	return ret;
}
UNIGINE_INLINE dvec4 &mul(dvec4 &ret, const mat4 &m, const dvec4 &v)
{
	ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * v.w;
	ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * v.w;
	ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * v.w;
	ret.w = m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * v.w;
	return ret;
}
UNIGINE_INLINE dvec4 &mul(dvec4 &ret, const dvec4 &v, const mat4 &m)
{
	ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z + m.m30 * v.w;
	ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z + m.m31 * v.w;
	ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z + m.m32 * v.w;
	ret.w = m.m03 * v.x + m.m13 * v.y + m.m23 * v.z + m.m33 * v.w;
	return ret;
}
UNIGINE_INLINE mat4 operator*(const mat4 &m, float v)
{
	mat4 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE vec2 operator*(const mat4 &m, const vec2 &v)
{
	vec2 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE vec2 operator*(const vec2 &v, const mat4 &m)
{
	vec2 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE vec3 operator*(const mat4 &m, const vec3 &v)
{
	vec3 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE vec3 operator*(const vec3 &v, const mat4 &m)
{
	vec3 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE vec4 operator*(const mat4 &m, const vec4 &v)
{
	vec4 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE vec4 operator*(const vec4 &v, const mat4 &m)
{
	vec4 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE dvec2 operator*(const mat4 &m, const dvec2 &v)
{
	dvec2 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE dvec2 operator*(const dvec2 &v, const mat4 &m)
{
	dvec2 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE dvec3 operator*(const mat4 &m, const dvec3 &v)
{
	dvec3 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE dvec3 operator*(const dvec3 &v, const mat4 &m)
{
	dvec3 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE dvec4 operator*(const mat4 &m, const dvec4 &v)
{
	dvec4 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE dvec4 operator*(const dvec4 &v, const mat4 &m)
{
	dvec4 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE mat4 operator*(const mat4 &m0, const mat4 &m1)
{
	mat4 ret;
	return mul(ret, m0, m1);
}
UNIGINE_INLINE mat4 &mat4::operator*=(float v)
{
	return mul(*this, *this, v);
}
UNIGINE_INLINE mat4 &mat4::operator*=(const mat4 &m)
{
	return mul(*this, mat4(*this), m);
}

UNIGINE_INLINE vec2 &mul3(vec2 &ret, const mat4 &m, const vec2 &v)
{
	ret.x = m.m00 * v.x + m.m01 * v.y;
	ret.y = m.m10 * v.x + m.m11 * v.y;
	return ret;
}
UNIGINE_INLINE vec2 &mul3(vec2 &ret, const vec2 &v, const mat4 &m)
{
	ret.x = m.m00 * v.x + m.m10 * v.y;
	ret.y = m.m01 * v.x + m.m11 * v.y;
	return ret;
}
UNIGINE_INLINE vec3 &mul3(vec3 &ret, const mat4 &m, const vec3 &v)
{
	#ifdef USE_SSE
		const __m128 vec = v.vec;
		const __m128 res_0 = _mm_mul_ps(m.sse.v0, _MM_SWIZZLE(vec, X, X, X, X));
		const __m128 res_1 = _mm_mul_ps(m.sse.v1, _MM_SWIZZLE(vec, Y, Y, Y, Y));
		const __m128 res_2 = _mm_mul_ps(m.sse.v2, _MM_SWIZZLE(vec, Z, Z, Z, Z));
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
UNIGINE_INLINE vec3 &mul3(vec3 &ret, const vec3 &v, const mat4 &m)
{
	ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
	ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
	ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
	return ret;
}
UNIGINE_INLINE vec4 &mul3(vec4 &ret, const mat4 &m, const vec4 &v)
{
	#ifdef USE_SSE
		const __m128 vec = v.vec;
		const __m128 res_0 = _mm_mul_ps(m.sse.v0, _MM_SWIZZLE(vec, X, X, X, X));
		const __m128 res_1 = _mm_mul_ps(m.sse.v1, _MM_SWIZZLE(vec, Y, Y, Y, Y));
		const __m128 res_2 = _mm_mul_ps(m.sse.v2, _MM_SWIZZLE(vec, Z, Z, Z, Z));
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
UNIGINE_INLINE vec4 &mul3(vec4 &ret, const vec4 &v, const mat4 &m)
{
	ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
	ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
	ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
	return ret;
}
UNIGINE_INLINE dvec2 &mul3(dvec2 &ret, const mat4 &m, const dvec2 &v)
{
	ret.x = m.m00 * v.x + m.m01 * v.y;
	ret.y = m.m10 * v.x + m.m11 * v.y;
	return ret;
}
UNIGINE_INLINE dvec2 &mul3(dvec2 &ret, const dvec2 &v, const mat4 &m)
{
	ret.x = m.m00 * v.x + m.m10 * v.y;
	ret.y = m.m01 * v.x + m.m11 * v.y;
	return ret;
}
UNIGINE_INLINE dvec3 &mul3(dvec3 &ret, const mat4 &m, const dvec3 &v)
{
	ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z;
	ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z;
	ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z;
	return ret;
}
UNIGINE_INLINE dvec3 &mul3(dvec3 &ret, const dvec3 &v, const mat4 &m)
{
	ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
	ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
	ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
	return ret;
}
UNIGINE_INLINE dvec4 &mul3(dvec4 &ret, const mat4 &m, const dvec4 &v)
{
	ret.x = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z;
	ret.y = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z;
	ret.z = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z;
	return ret;
}
UNIGINE_INLINE dvec4 &mul3(dvec4 &ret, const dvec4 &v, const mat4 &m)
{
	ret.x = m.m00 * v.x + m.m10 * v.y + m.m20 * v.z;
	ret.y = m.m01 * v.x + m.m11 * v.y + m.m21 * v.z;
	ret.z = m.m02 * v.x + m.m12 * v.y + m.m22 * v.z;
	return ret;
}
UNIGINE_INLINE mat4 &mul3(mat4 &ret, const mat4 &m0, const mat4 &m1)
{
	ret.m00 = m0.m00 * m1.m00 + m0.m01 * m1.m10 + m0.m02 * m1.m20;
	ret.m10 = m0.m10 * m1.m00 + m0.m11 * m1.m10 + m0.m12 * m1.m20;
	ret.m20 = m0.m20 * m1.m00 + m0.m21 * m1.m10 + m0.m22 * m1.m20;
	ret.m30 = 0.0f;
	ret.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11 + m0.m02 * m1.m21;
	ret.m11 = m0.m10 * m1.m01 + m0.m11 * m1.m11 + m0.m12 * m1.m21;
	ret.m21 = m0.m20 * m1.m01 + m0.m21 * m1.m11 + m0.m22 * m1.m21;
	ret.m31 = 0.0f;
	ret.m02 = m0.m00 * m1.m02 + m0.m01 * m1.m12 + m0.m02 * m1.m22;
	ret.m12 = m0.m10 * m1.m02 + m0.m11 * m1.m12 + m0.m12 * m1.m22;
	ret.m22 = m0.m20 * m1.m02 + m0.m21 * m1.m12 + m0.m22 * m1.m22;
	ret.m32 = 0.0f;
	ret.m03 = 0.0f;
	ret.m13 = 0.0f;
	ret.m23 = 0.0f;
	ret.m33 = 1.0f;
	return ret;
}

UNIGINE_INLINE mat4 &mul4(mat4 &ret, const mat4 &m0, const mat4 &m1)
{
	#ifdef USE_SSE
		__m128 m0_col0 = m0.sse.v0;
		__m128 m0_col1 = m0.sse.v1;
		__m128 m0_col2 = m0.sse.v2;
		__m128 m0_col3 = m0.sse.v3;
	
		__m128 m1_col0 = m1.sse.v0;
		__m128 m1_col1 = m1.sse.v1;
		__m128 m1_col2 = m1.sse.v2;
		__m128 m1_col3 = m1.sse.v3;
	
		__m128 res_0 = _mm_mul_ps(m0_col0, _MM_SWIZZLE(m1_col0, X, X, X, X));
		__m128 res_1 = _mm_mul_ps(m0_col1, _MM_SWIZZLE(m1_col0, Y, Y, Y, Y));
		__m128 res_2 = _mm_mul_ps(m0_col2, _MM_SWIZZLE(m1_col0, Z, Z, Z, Z));
		ret.sse.v0 = _mm_add_ps(_mm_add_ps(res_0, res_1), res_2);

		res_0 = _mm_mul_ps(m0_col0, _MM_SWIZZLE(m1_col1, X, X, X, X));
		res_1 = _mm_mul_ps(m0_col1, _MM_SWIZZLE(m1_col1, Y, Y, Y, Y));
		res_2 = _mm_mul_ps(m0_col2, _MM_SWIZZLE(m1_col1, Z, Z, Z, Z));
		ret.sse.v1 = _mm_add_ps(_mm_add_ps(res_0, res_1), res_2);

		res_0 = _mm_mul_ps(m0_col0, _MM_SWIZZLE(m1_col2, X, X, X, X));
		res_1 = _mm_mul_ps(m0_col1, _MM_SWIZZLE(m1_col2, Y, Y, Y, Y));
		res_2 = _mm_mul_ps(m0_col2, _MM_SWIZZLE(m1_col2, Z, Z, Z, Z));
		ret.sse.v2 = _mm_add_ps(_mm_add_ps(res_0, res_1), res_2);

		res_0 = _mm_mul_ps(m0_col0, _MM_SWIZZLE(m1_col3, X, X, X, X));
		res_1 = _mm_mul_ps(m0_col1, _MM_SWIZZLE(m1_col3, Y, Y, Y, Y));
		res_2 = _mm_mul_ps(m0_col2, _MM_SWIZZLE(m1_col3, Z, Z, Z, Z));
		ret.sse.v3 = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, m0_col3));
	#else
		ret.m00 = m0.m00 * m1.m00 + m0.m01 * m1.m10 + m0.m02 * m1.m20;
		ret.m10 = m0.m10 * m1.m00 + m0.m11 * m1.m10 + m0.m12 * m1.m20;
		ret.m20 = m0.m20 * m1.m00 + m0.m21 * m1.m10 + m0.m22 * m1.m20;
		ret.m30 = 0.0f;
		ret.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11 + m0.m02 * m1.m21;
		ret.m11 = m0.m10 * m1.m01 + m0.m11 * m1.m11 + m0.m12 * m1.m21;
		ret.m21 = m0.m20 * m1.m01 + m0.m21 * m1.m11 + m0.m22 * m1.m21;
		ret.m31 = 0.0f;
		ret.m02 = m0.m00 * m1.m02 + m0.m01 * m1.m12 + m0.m02 * m1.m22;
		ret.m12 = m0.m10 * m1.m02 + m0.m11 * m1.m12 + m0.m12 * m1.m22;
		ret.m22 = m0.m20 * m1.m02 + m0.m21 * m1.m12 + m0.m22 * m1.m22;
		ret.m32 = 0.0f;
		ret.m03 = m0.m00 * m1.m03 + m0.m01 * m1.m13 + m0.m02 * m1.m23 + m0.m03;
		ret.m13 = m0.m10 * m1.m03 + m0.m11 * m1.m13 + m0.m12 * m1.m23 + m0.m13;
		ret.m23 = m0.m20 * m1.m03 + m0.m21 * m1.m13 + m0.m22 * m1.m23 + m0.m23;
		ret.m33 = 1.0f;
	#endif
	return ret;
}
UNIGINE_INLINE mat4 &mult(mat4 &ret, const mat4 &m, const vec3 &v)
{
	#ifdef USE_SSE
		__m128 vec = v.vec;
		ret.sse.v0 = m.sse.v0;
		ret.sse.v1 = m.sse.v1;
		ret.sse.v2 = m.sse.v2;
		__m128 res_0 = _mm_mul_ps(m.sse.v0, _MM_SWIZZLE(vec, X, X, X, X));
		__m128 res_1 = _mm_mul_ps(m.sse.v1, _MM_SWIZZLE(vec, Y, Y, Y, Y));
		__m128 res_2 = _mm_mul_ps(m.sse.v2, _MM_SWIZZLE(vec, Z, Z, Z, Z));
		ret.sse.v3 = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, m.sse.v3));
	#else
		ret.m00 = m.m00;
		ret.m01 = m.m01;
		ret.m02 = m.m02;
		ret.m10 = m.m10;
		ret.m11 = m.m11;
		ret.m12 = m.m12;
		ret.m20 = m.m20;
		ret.m21 = m.m21;
		ret.m22 = m.m22;
		ret.m30 = m.m30;
		ret.m31 = m.m31;
		ret.m32 = m.m32;
		ret.m03 = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03;
		ret.m13 = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13;
		ret.m23 = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23;
		ret.m33 = m.m33;
	#endif
	return ret;
}

UNIGINE_INLINE mat4 &add(mat4 &ret, const mat4 &m0, const mat4 &m1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_add_ps(m0.sse.v0, m1.sse.v0);
		ret.sse.v1 = _mm_add_ps(m0.sse.v1, m1.sse.v1);
		ret.sse.v2 = _mm_add_ps(m0.sse.v2, m1.sse.v2);
		ret.sse.v3 = _mm_add_ps(m0.sse.v3, m1.sse.v3);
	#else
		ret.m00 = m0.m00 + m1.m00;
		ret.m01 = m0.m01 + m1.m01;
		ret.m02 = m0.m02 + m1.m02;
		ret.m03 = m0.m03 + m1.m03;
		ret.m10 = m0.m10 + m1.m10;
		ret.m11 = m0.m11 + m1.m11;
		ret.m12 = m0.m12 + m1.m12;
		ret.m13 = m0.m13 + m1.m13;
		ret.m20 = m0.m20 + m1.m20;
		ret.m21 = m0.m21 + m1.m21;
		ret.m22 = m0.m22 + m1.m22;
		ret.m23 = m0.m23 + m1.m23;
		ret.m30 = m0.m30 + m1.m30;
		ret.m31 = m0.m31 + m1.m31;
		ret.m32 = m0.m32 + m1.m32;
		ret.m33 = m0.m33 + m1.m33;
	#endif
	return ret;
}
UNIGINE_INLINE mat4 operator+(const mat4 &m0, const mat4 &m1)
{
	mat4 ret;
	return add(ret, m0, m1);
}
UNIGINE_INLINE mat4 &mat4::operator+=(const mat4 &m)
{
	return add(*this, *this, m);
}

UNIGINE_INLINE mat4 &sub(mat4 &ret, const mat4 &m0, const mat4 &m1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_sub_ps(m0.sse.v0, m1.sse.v0);
		ret.sse.v1 = _mm_sub_ps(m0.sse.v1, m1.sse.v1);
		ret.sse.v2 = _mm_sub_ps(m0.sse.v2, m1.sse.v2);
		ret.sse.v3 = _mm_sub_ps(m0.sse.v3, m1.sse.v3);
	#else
		ret.m00 = m0.m00 - m1.m00;
		ret.m01 = m0.m01 - m1.m01;
		ret.m02 = m0.m02 - m1.m02;
		ret.m03 = m0.m03 - m1.m03;
		ret.m10 = m0.m10 - m1.m10;
		ret.m11 = m0.m11 - m1.m11;
		ret.m12 = m0.m12 - m1.m12;
		ret.m13 = m0.m13 - m1.m13;
		ret.m20 = m0.m20 - m1.m20;
		ret.m21 = m0.m21 - m1.m21;
		ret.m22 = m0.m22 - m1.m22;
		ret.m23 = m0.m23 - m1.m23;
		ret.m30 = m0.m30 - m1.m30;
		ret.m31 = m0.m31 - m1.m31;
		ret.m32 = m0.m32 - m1.m32;
		ret.m33 = m0.m33 - m1.m33;
	#endif
	return ret;
}
UNIGINE_INLINE mat4 operator-(const mat4 &m0, const mat4 &m1)
{
	mat4 ret;
	return sub(ret, m0, m1);
}
UNIGINE_INLINE mat4 &mat4::operator-=(const mat4 &m)
{
	return sub(*this, *this, m);
}

UNIGINE_INLINE vec3 &proj(vec3 &ret, const mat4 &m, const vec3 &v)
{
	#ifdef USE_SSE
		__m128 vec = v.vec;
		__m128 res_0 = _mm_mul_ps(m.sse.v0, _MM_SWIZZLE(vec, X, X, X, X));
		__m128 res_1 = _mm_mul_ps(m.sse.v1, _MM_SWIZZLE(vec, Y, Y, Y, Y));
		__m128 res_2 = _mm_mul_ps(m.sse.v2, _MM_SWIZZLE(vec, Z, Z, Z, Z));
		__m128 res_3 = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, m.sse.v3));
		ret = _mm_div_ps(res_3, _MM_SWIZZLE(res_3, W, W, W, W));
	#else
		float x = v.x;
		float y = v.y;
		float z = v.z;
		float iw = rcp(m.m30 * x + m.m31 * y + m.m32 * z + m.m33);
		ret.x = (m.m00 * x + m.m01 * y + m.m02 * z + m.m03) * iw;
		ret.y = (m.m10 * x + m.m11 * y + m.m12 * z + m.m13) * iw;
		ret.z = (m.m20 * x + m.m21 * y + m.m22 * z + m.m23) * iw;
	#endif
	return ret;
}
UNIGINE_INLINE vec4 &proj(vec4 &ret, const mat4 &m, const vec4 &v)
{
	#ifdef USE_SSE
		__m128 vec = v.vec;
		__m128 res_0 = _mm_mul_ps(m.sse.v0, _MM_SWIZZLE(vec, X, X, X, X));
		__m128 res_1 = _mm_mul_ps(m.sse.v1, _MM_SWIZZLE(vec, Y, Y, Y, Y));
		__m128 res_2 = _mm_mul_ps(m.sse.v2, _MM_SWIZZLE(vec, Z, Z, Z, Z));
		__m128 res_3 = _mm_mul_ps(m.sse.v3, _MM_SWIZZLE(vec, W, W, W, W));
		__m128 res_4 = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, res_3));
		ret = _mm_div_ps(res_4, _MM_SWIZZLE(res_4, W, W, W, W));
	#else
		float x = v.x;
		float y = v.y;
		float z = v.z;
		float w = v.w;
		float iw = rcp(m.m30 * x + m.m31 * y + m.m32 * z + m.m33 * w);
		ret.x = (m.m00 * x + m.m01 * y + m.m02 * z + m.m03 * w) * iw;
		ret.y = (m.m10 * x + m.m11 * y + m.m12 * z + m.m13 * w) * iw;
		ret.z = (m.m20 * x + m.m21 * y + m.m22 * z + m.m23 * w) * iw;
		ret.w = 1.0f;
	#endif
	return ret;
}
UNIGINE_INLINE dvec3 &proj(dvec3 &ret, const mat4 &m, const dvec3 &v)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	double iw = Math::rcp(m.m30 * x + m.m31 * y + m.m32 * z + m.m33);
	ret.x = (m.m00 * x + m.m01 * y + m.m02 * z + m.m03) * iw;
	ret.y = (m.m10 * x + m.m11 * y + m.m12 * z + m.m13) * iw;
	ret.z = (m.m20 * x + m.m21 * y + m.m22 * z + m.m23) * iw;
	return ret;
}
UNIGINE_INLINE dvec4 &proj(dvec4 &ret, const mat4 &m, const dvec4 &v)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	double w = v.w;
	double iw = Math::rcp(m.m30 * x + m.m31 * y + m.m32 * z + m.m33 * w);
	ret.x = (m.m00 * x + m.m01 * y + m.m02 * z + m.m03 * w) * iw;
	ret.y = (m.m10 * x + m.m11 * y + m.m12 * z + m.m13 * w) * iw;
	ret.z = (m.m20 * x + m.m21 * y + m.m22 * z + m.m23 * w) * iw;
	ret.w = 1.0f;
	return ret;
}

UNIGINE_INLINE mat4 &orthonormalize(mat4 &ret, const mat4 &m)
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
		ret.sse.v3 = m.sse.v3;
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
		ret.m03 = m.m03;
		ret.m10 = x.y;
		ret.m11 = y.y;
		ret.m12 = z.y;
		ret.m13 = m.m13;
		ret.m20 = x.z;
		ret.m21 = y.z;
		ret.m22 = z.z;
		ret.m23 = m.m23;
		ret.m30 = m.m30;
		ret.m31 = m.m31;
		ret.m32 = m.m32;
		ret.m33 = m.m33;
	#endif
	return ret;
}
UNIGINE_INLINE mat4 orthonormalize(const mat4 &m)
{
	mat4 ret;
	return orthonormalize(ret, m);
}

UNIGINE_INLINE mat4 &inverse(mat4 &ret, const mat4 &m)
{
	#ifdef USE_SSE
		__m128 m_col0 = m.sse.v0;
		__m128 m_col1 = m.sse.v1;
		__m128 m_col2 = m.sse.v2;
		__m128 m_col3 = m.sse.v3;
	
		__m128 res_0 = _mm_shuffle_ps(m_col0, m_col1, _MM_PERM2(X, Y, X, Y));
		__m128 res_1 = _mm_shuffle_ps(m_col0, m_col1, _MM_PERM2(Z, W, Z, W));
		__m128 res_2 = _mm_shuffle_ps(m_col2, m_col3, _MM_PERM2(X, Y, X, Y));
		__m128 res_3 = _mm_shuffle_ps(m_col2, m_col3, _MM_PERM2(Z, W, Z, W));
		__m128 row_0 = _mm_shuffle_ps(res_0, res_2, _MM_PERM2(X, Z, X, Z));
		__m128 row_1 = _mm_shuffle_ps(res_2, res_0, _MM_PERM2(Y, W, Y, W));
		__m128 row_2 = _mm_shuffle_ps(res_1, res_3, _MM_PERM2(X, Z, X, Z));
		__m128 row_3 = _mm_shuffle_ps(res_3, res_1, _MM_PERM2(Y, W, Y, W));
		__m128 temp = _mm_mul_ps(row_2, row_3);
		temp = _MM_SWIZZLE(temp, Y, X, W, Z);
		res_0 = _mm_mul_ps(row_1, temp);
		res_1 = _mm_mul_ps(row_0, temp);
		temp = _MM_SWIZZLE(temp, Z, W, X, Y);
		res_0 = _mm_sub_ps(_mm_mul_ps(row_1, temp), res_0);
		res_1 = _mm_sub_ps(_mm_mul_ps(row_0, temp), res_1);
		res_1 = _MM_SWIZZLE(res_1, Z, W, X, Y);
		temp = _mm_mul_ps(row_1, row_2);
		temp = _MM_SWIZZLE(temp, Y, X, W, Z);
		res_0 = _mm_add_ps(_mm_mul_ps(row_3, temp), res_0);
		res_3 = _mm_mul_ps(row_0, temp);
		temp = _MM_SWIZZLE(temp, Z, W, X, Y);
		res_0 = _mm_sub_ps(res_0, _mm_mul_ps(row_3, temp));
		res_3 = _mm_sub_ps(_mm_mul_ps(row_0, temp), res_3);
		res_3 = _MM_SWIZZLE(res_3, Z, W, X, Y);
		temp = _mm_mul_ps(row_3, _MM_SWIZZLE(row_1, Z, W, X, Y));
		temp = _MM_SWIZZLE(temp, Y, X, W, Z);
		row_2 = _MM_SWIZZLE(row_2, Z, W, X, Y);
		res_0 = _mm_add_ps(_mm_mul_ps(row_2, temp), res_0);
		res_2 = _mm_mul_ps(row_0, temp);
		temp = _MM_SWIZZLE(temp, Z, W, X, Y);
		res_0 = _mm_sub_ps(res_0, _mm_mul_ps(row_2, temp));
		res_2 = _mm_sub_ps(_mm_mul_ps(row_0, temp), res_2);
		res_2 = _MM_SWIZZLE(res_2, Z, W, X, Y);
		temp = _mm_mul_ps(row_0, row_1);
		temp = _MM_SWIZZLE(temp, Y, X, W, Z);
		res_2 = _mm_add_ps(_mm_mul_ps(row_3, temp), res_2);
		res_3 = _mm_sub_ps(_mm_mul_ps(row_2, temp), res_3);
		temp = _MM_SWIZZLE(temp, Z, W, X, Y);
		res_2 = _mm_sub_ps(_mm_mul_ps(row_3, temp), res_2);
		res_3 = _mm_sub_ps(res_3, _mm_mul_ps(row_2, temp));
		temp = _mm_mul_ps(row_0, row_3);
		temp = _MM_SWIZZLE(temp, Y, X, W, Z);
		res_1 = _mm_sub_ps(res_1, _mm_mul_ps(row_2, temp));
		res_2 = _mm_add_ps(_mm_mul_ps(row_1, temp), res_2);
		temp = _MM_SWIZZLE(temp, Z, W, X, Y);
		res_1 = _mm_add_ps(_mm_mul_ps(row_2, temp), res_1);
		res_2 = _mm_sub_ps(res_2, _mm_mul_ps(row_1, temp));
		temp = _mm_mul_ps(row_0, row_2);
		temp = _MM_SWIZZLE(temp, Y, X, W, Z);
		res_1 = _mm_add_ps(_mm_mul_ps(row_3, temp), res_1);
		res_3 = _mm_sub_ps(res_3, _mm_mul_ps(row_1, temp));
		temp = _MM_SWIZZLE(temp, Z, W, X, Y);
		res_1 = _mm_sub_ps(res_1, _mm_mul_ps(row_3, temp));
		res_3 = _mm_add_ps(_mm_mul_ps(row_1, temp), res_3);
		__m128 det = _mm_mul_ps(row_0, res_0);
		det = _mm_add_ps(det, _MM_SWIZZLE(det, Y, X, W, Z));
		det = _mm_add_ss(det, _MM_SWIZZLE(det, Z, W, X, Y));
		temp = _MM_SWIZZLE(_mm_rcp_ss_nr(det), X, X, X, X);
		ret.sse.v0 = _mm_mul_ps(res_0, temp);
		ret.sse.v1 = _mm_mul_ps(res_1, temp);
		ret.sse.v2 = _mm_mul_ps(res_2, temp);
		ret.sse.v3 = _mm_mul_ps(res_3, temp);
	#else
		float temp[12];
		temp[0] = m.m22 * m.m33;
		temp[1] = m.m23 * m.m32;
		temp[2] = m.m21 * m.m33;
		temp[3] = m.m23 * m.m31;
		temp[4] = m.m21 * m.m32;
		temp[5] = m.m22 * m.m31;
		temp[6] = m.m20 * m.m33;
		temp[7] = m.m23 * m.m30;
		temp[8] = m.m20 * m.m32;
		temp[9] = m.m22 * m.m30;
		temp[10] = m.m20 * m.m31;
		temp[11] = m.m21 * m.m30;
		ret.m00 = temp[0] * m.m11 + temp[3] * m.m12 + temp[4] * m.m13;
		ret.m00 -= temp[1] * m.m11 + temp[2] * m.m12 + temp[5] * m.m13;
		ret.m10 = temp[1] * m.m10 + temp[6] * m.m12 + temp[9] * m.m13;
		ret.m10 -= temp[0] * m.m10 + temp[7] * m.m12 + temp[8] * m.m13;
		ret.m20 = temp[2] * m.m10 + temp[7] * m.m11 + temp[10] * m.m13;
		ret.m20 -= temp[3] * m.m10 + temp[6] * m.m11 + temp[11] * m.m13;
		ret.m30 = temp[5] * m.m10 + temp[8] * m.m11 + temp[11] * m.m12;
		ret.m30 -= temp[4] * m.m10 + temp[9] * m.m11 + temp[10] * m.m12;
		ret.m01 = temp[1] * m.m01 + temp[2] * m.m02 + temp[5] * m.m03;
		ret.m01 -= temp[0] * m.m01 + temp[3] * m.m02 + temp[4] * m.m03;
		ret.m11 = temp[0] * m.m00 + temp[7] * m.m02 + temp[8] * m.m03;
		ret.m11 -= temp[1] * m.m00 + temp[6] * m.m02 + temp[9] * m.m03;
		ret.m21 = temp[3] * m.m00 + temp[6] * m.m01 + temp[11] * m.m03;
		ret.m21 -= temp[2] * m.m00 + temp[7] * m.m01 + temp[10] * m.m03;
		ret.m31 = temp[4] * m.m00 + temp[9] * m.m01 + temp[10] * m.m02;
		ret.m31 -= temp[5] * m.m00 + temp[8] * m.m01 + temp[11] * m.m02;
		temp[0] = m.m02 * m.m13;
		temp[1] = m.m03 * m.m12;
		temp[2] = m.m01 * m.m13;
		temp[3] = m.m03 * m.m11;
		temp[4] = m.m01 * m.m12;
		temp[5] = m.m02 * m.m11;
		temp[6] = m.m00 * m.m13;
		temp[7] = m.m03 * m.m10;
		temp[8] = m.m00 * m.m12;
		temp[9] = m.m02 * m.m10;
		temp[10] = m.m00 * m.m11;
		temp[11] = m.m01 * m.m10;
		ret.m02 = temp[0] * m.m31 + temp[3] * m.m32 + temp[4] * m.m33;
		ret.m02 -= temp[1] * m.m31 + temp[2] * m.m32 + temp[5] * m.m33;
		ret.m12 = temp[1] * m.m30 + temp[6] * m.m32 + temp[9] * m.m33;
		ret.m12 -= temp[0] * m.m30 + temp[7] * m.m32 + temp[8] * m.m33;
		ret.m22 = temp[2] * m.m30 + temp[7] * m.m31 + temp[10] * m.m33;
		ret.m22 -= temp[3] * m.m30 + temp[6] * m.m31 + temp[11] * m.m33;
		ret.m32 = temp[5] * m.m30 + temp[8] * m.m31 + temp[11] * m.m32;
		ret.m32 -= temp[4] * m.m30 + temp[9] * m.m31 + temp[10] * m.m32;
		ret.m03 = temp[2] * m.m22 + temp[5] * m.m23 + temp[1] * m.m21;
		ret.m03 -= temp[4] * m.m23 + temp[0] * m.m21 + temp[3] * m.m22;
		ret.m13 = temp[8] * m.m23 + temp[0] * m.m20 + temp[7] * m.m22;
		ret.m13 -= temp[6] * m.m22 + temp[9] * m.m23 + temp[1] * m.m20;
		ret.m23 = temp[6] * m.m21 + temp[11] * m.m23 + temp[3] * m.m20;
		ret.m23 -= temp[10] * m.m23 + temp[2] * m.m20 + temp[7] * m.m21;
		ret.m33 = temp[10] * m.m22 + temp[4] * m.m20 + temp[9] * m.m21;
		ret.m33 -= temp[8] * m.m21 + temp[11] * m.m22 + temp[5] * m.m20;
		float idet = rcp(m.m00 * ret.m00 + m.m01 * ret.m10 + m.m02 * ret.m20 + m.m03 * ret.m30);
		ret.m00 *= idet;
		ret.m10 *= idet;
		ret.m20 *= idet;
		ret.m30 *= idet;
		ret.m01 *= idet;
		ret.m11 *= idet;
		ret.m21 *= idet;
		ret.m31 *= idet;
		ret.m02 *= idet;
		ret.m12 *= idet;
		ret.m22 *= idet;
		ret.m32 *= idet;
		ret.m03 *= idet;
		ret.m13 *= idet;
		ret.m23 *= idet;
		ret.m33 *= idet;
	#endif
	return ret;
}
UNIGINE_INLINE mat4 inverse(const mat4 &m)
{
	mat4 ret;
	return inverse(ret, m);
}

UNIGINE_INLINE mat4 &inverse4(mat4 &ret, const mat4 &m)
{
	float det = m.m00 * (m.m11 * m.m22 - m.m12 * m.m21);
	det -= m.m01 * (m.m10 * m.m22 - m.m12 * m.m20);
	det += m.m02 * (m.m10 * m.m21 - m.m11 * m.m20);

	float idet = 1.0f / det;

	ret.m00 = (m.m11 * m.m22 - m.m12 * m.m21) * idet;
	ret.m10 = -(m.m10 * m.m22 - m.m12 * m.m20) * idet;
	ret.m20 = (m.m10 * m.m21 - m.m11 * m.m20) * idet;
	ret.m30 = 0.0f;

	ret.m01 = -(m.m01 * m.m22 - m.m02 * m.m21) * idet;
	ret.m11 = (m.m00 * m.m22 - m.m02 * m.m20) * idet;
	ret.m21 = -(m.m00 * m.m21 - m.m01 * m.m20) * idet;
	ret.m31 = 0.0f;

	ret.m02 = (m.m01 * m.m12 - m.m02 * m.m11) * idet;
	ret.m12 = -(m.m00 * m.m12 - m.m02 * m.m10) * idet;
	ret.m22 = (m.m00 * m.m11 - m.m01 * m.m10) * idet;
	ret.m32 = 0.0f;

	ret.m03 = -(ret.m00 * m.m03 + ret.m01 * m.m13 + ret.m02 * m.m23);
	ret.m13 = -(ret.m10 * m.m03 + ret.m11 * m.m13 + ret.m12 * m.m23);
	ret.m23 = -(ret.m20 * m.m03 + ret.m21 * m.m13 + ret.m22 * m.m23);
	ret.m33 = 1.0f;
	return ret;
}
UNIGINE_INLINE mat4 inverse4(const mat4 &m)
{
	mat4 ret;
	return inverse4(ret, m);
}

UNIGINE_INLINE mat4 &rotation(mat4 &ret, const mat4 &m)
{
	ret.m00 = m.m00;
	ret.m01 = m.m01;
	ret.m02 = m.m02;
	ret.m03 = 0.0f;
	ret.m10 = m.m10;
	ret.m11 = m.m11;
	ret.m12 = m.m12;
	ret.m13 = 0.0f;
	ret.m20 = m.m20;
	ret.m21 = m.m21;
	ret.m22 = m.m22;
	ret.m23 = 0.0f;
	ret.m30 = 0.0f;
	ret.m31 = 0.0f;
	ret.m32 = 0.0f;
	ret.m33 = 1.0f;
	return ret;
}
UNIGINE_INLINE mat4 rotation(const mat4 &m)
{
	mat4 ret;
	return rotation(ret, m);
}

UNIGINE_INLINE mat4 &transpose(mat4 &ret, const mat4 &m)
{
	#ifdef USE_SSE
		__m128 m_col0 = m.sse.v0;
		__m128 m_col1 = m.sse.v1;
		__m128 m_col2 = m.sse.v2;
		__m128 m_col3 = m.sse.v3;
	
		__m128 res_0 = _mm_shuffle_ps(m_col0, m_col1, _MM_PERM2(X, Y, X, Y));
		__m128 res_1 = _mm_shuffle_ps(m_col0, m_col1, _MM_PERM2(Z, W, Z, W));
		__m128 res_2 = _mm_shuffle_ps(m_col2, m_col3, _MM_PERM2(X, Y, X, Y));
		__m128 res_3 = _mm_shuffle_ps(m_col2, m_col3, _MM_PERM2(Z, W, Z, W));
		ret.sse.v0 = _mm_shuffle_ps(res_0, res_2, _MM_PERM2(X, Z, X, Z));
		ret.sse.v1 = _mm_shuffle_ps(res_0, res_2, _MM_PERM2(Y, W, Y, W));
		ret.sse.v2 = _mm_shuffle_ps(res_1, res_3, _MM_PERM2(X, Z, X, Z));
		ret.sse.v3 = _mm_shuffle_ps(res_1, res_3, _MM_PERM2(Y, W, Y, W));
	#else
		ret.m00 = m.m00;
		ret.m01 = m.m10;
		ret.m02 = m.m20;
		ret.m03 = m.m30;
		ret.m10 = m.m01;
		ret.m11 = m.m11;
		ret.m12 = m.m21;
		ret.m13 = m.m31;
		ret.m20 = m.m02;
		ret.m21 = m.m12;
		ret.m22 = m.m22;
		ret.m23 = m.m32;
		ret.m30 = m.m03;
		ret.m31 = m.m13;
		ret.m32 = m.m23;
		ret.m33 = m.m33;
	#endif
	return ret;
}
UNIGINE_INLINE mat4 transpose(const mat4 &m)
{
	mat4 ret;
	return transpose(ret, m);
}

UNIGINE_INLINE mat4 &transpose3(mat4 &ret, const mat4 &m)
{
	ret.m00 = m.m00;
	ret.m01 = m.m10;
	ret.m02 = m.m20;
	ret.m03 = m.m03;
	ret.m10 = m.m01;
	ret.m11 = m.m11;
	ret.m12 = m.m21;
	ret.m13 = m.m13;
	ret.m20 = m.m02;
	ret.m21 = m.m12;
	ret.m22 = m.m22;
	ret.m23 = m.m23;
	ret.m30 = m.m30;
	ret.m31 = m.m31;
	ret.m32 = m.m32;
	ret.m33 = m.m33;
	return ret;
}
UNIGINE_INLINE mat4 transpose3(const mat4 &m)
{
	mat4 ret;
	return transpose3(ret, m);
}

UNIGINE_INLINE mat4 ortho(float l, float r, float b, float t, float n, float f)
{
	float rl = r - l;
	float tb = t - b;
	float fn = f - n;
	return mat4(
		2.0f / rl, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / tb, 0.0f, 0.0f,
		0.0f, 0.0f, -2.0f / fn, 0.0f,
		- (r + l) / rl, -(t + b) / tb, -(f + n) / fn, 1.0f
	);
}
UNIGINE_INLINE mat4 frustum(float l, float r, float b, float t, float n, float f)
{
	float rl = r - l;
	float tb = t - b;
	float fn = f - n;
	return mat4(
		2.0f * n / rl, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f * n / tb, 0.0f, 0.0f,
		(r + l) / rl, (t + b) / tb, -(f + n) / fn, -1.0f,
		0.0f, 0.0f, -2.0f * f * n / fn, 0.0f
	);
}
UNIGINE_INLINE mat4 perspective(float fov, float aspect, float n, float f)
{
	float h = 1.0f;
	float w = aspect;
	if (!compare(fov, 90.0f))
	{
		h = tan(fov * Consts::DEG2RAD * 0.5f);
		w = h * aspect;
	}
	float fn = f - n;
	return mat4(
		1.0f / w, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f / h, 0.0f, 0.0f,
		0.0f, 0.0f, -(f + n) / fn, -1.0f,
		0.0f, 0.0f, -2.0f * f * n / fn, 0.0f
	);
}

UNIGINE_INLINE mat4 reflect(const vec4 &plane)
{
	float x = plane.x;
	float y = plane.y;
	float z = plane.z;
	float x2 = x * 2.0f;
	float y2 = y * 2.0f;
	float z2 = z * 2.0f;
	return mat4(
		1.0f - x * x2,	-x * y2,		-x * z2,		0.0f,
		-y * x2,		1.0f - y * y2,	-y * z2,		0.0f,
		-z * x2,		-z * y2,		1.0f - z * z2,	0.0f,
		-plane.w * x2,	-plane.w * y2,	-plane.w * z2,	1.0f
	);
}

UNIGINE_INLINE void calculate_basis(const vec3 &position, const vec3 &target, const vec3 &up, int axis, vec3 &x, vec3 &y, vec3 &z)
{
	switch (axis)
	{
		case AXIS_X:
			x = normalize(target - position);
			y = normalize(cross(up, x));
			z = normalize(cross(x, y));
			break;
		case AXIS_Y:
			y = normalize(target - position);
			x = normalize(cross(y, up));
			z = normalize(cross(x, y));
			break;
		case AXIS_Z:
			z = normalize(target - position);
			x = normalize(cross(up, z));
			y = normalize(cross(z, x));
			break;
		case AXIS_NX:
			x = normalize(position - target);
			y = normalize(cross(up, x));
			z = normalize(cross(x, y));
			break;
		case AXIS_NY:
			y = normalize(position - target);
			x = normalize(cross(y, up));
			z = normalize(cross(x, y));
			break;
		case AXIS_NZ:
			z = normalize(position - target);
			x = normalize(cross(up, z));
			y = normalize(cross(z, x));
			break;
	}
}
UNIGINE_INLINE mat4 setTo(const vec3 &position, const vec3 &target, const vec3 &up, int axis = AXIS_NZ)
{
	mat4 ret;
	vec3 x, y, z;
	calculate_basis(position, target, up, axis, x, y, z);
	ret.m00 = x.x;
	ret.m01 = y.x;
	ret.m02 = z.x;
	ret.m03 = position.x;
	ret.m10 = x.y;
	ret.m11 = y.y;
	ret.m12 = z.y;
	ret.m13 = position.y;
	ret.m20 = x.z;
	ret.m21 = y.z;
	ret.m22 = z.z;
	ret.m23 = position.z;
	ret.m30 = 0.0f;
	ret.m31 = 0.0f;
	ret.m32 = 0.0f;
	ret.m33 = 1.0f;
	return ret;
}
UNIGINE_INLINE mat4 lookAt(const vec3 &position, const vec3 &target, const vec3 &up, int axis = AXIS_NZ)
{
	mat4 ret, m0, m1;
	vec3 x, y, z;
	calculate_basis(position, target, up, axis, x, y, z);
	m0.m00 = x.x;
	m0.m01 = x.y;
	m0.m02 = x.z;
	m0.m03 = 0.0f;
	m0.m10 = y.x;
	m0.m11 = y.y;
	m0.m12 = y.z;
	m0.m13 = 0.0f;
	m0.m20 = z.x;
	m0.m21 = z.y;
	m0.m22 = z.z;
	m0.m23 = 0.0f;
	m0.m30 = 0.0f;
	m0.m31 = 0.0f;
	m0.m32 = 0.0f;
	m0.m33 = 1.0f;
	m1.setTranslate(-position);
	return mul(ret, m0, m1);
}

UNIGINE_INLINE const mat4 &cubeTransform(int face)
{
	static const mat4 transforms[6] = {
		rotateY(90.0f) * rotateX(180.0f),	// 0 X+
		rotateY(-90.0f) * rotateX(180.0f),	// 1 X-
		rotateX(-90.0f),					// 2 Y+
		rotateX(90.0f),						// 3 Y-
		rotateX(180.0f),					// 4 Z+
		rotateZ(180.0f),					// 5 Z-
	};
	assert(face <= 5 && face >= 0 && "cubeTransform(): bad face number");
	return transforms[face];
}

UNIGINE_INLINE mat4 obliqueProjection(const mat4 &projection, const vec4 &plane)
{
	mat4 ret, transform;
	vec4 v = plane * inverse(projection);

	float z = Math::abs(v.z);
	if (z > 1.0f)
		v /= z;

	v.w -= 1.0f;
	if (v.z < 0.0f)
		v = -v;

	transform.setIdentity();
	transform.setRow(2, v);
	return mul(ret, transform, projection);
}
UNIGINE_INLINE mat4 reverseDepthProjection(const mat4 &projection)
{
	mat4 ret = projection;
	ret.m20 *= -1.0f;
	ret.m21 *= -1.0f;
	ret.m22 *= -1.0f;
	ret.m23 *= -1.0f;
	return ret;
}
UNIGINE_INLINE mat4 symmetryProjection(const mat4 &projection)
{
	float r = (projection.m02 + 1.0f) / projection.m00;
	float t = (projection.m12 + 1.0f) / projection.m11;
	float l = r - 2.0f / projection.m00;
	float b = t - 2.0f / projection.m11;
	float w = max(abs(r), abs(l));
	float h = max(abs(t), abs(b));

	return mat4 (
		1.0f / w, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f / h, 0.0f, 0.0f,
		0.0f, 0.0f, projection.m22, -1.0f,
		0.0f, 0.0f, projection.m23, 0.0f
	);
}

UNIGINE_INLINE void decomposeProjection(const mat4 &projection, float &znear, float &zfar)
{
	if (projection.m32 == 0.0f)
	{
		znear = (projection.m23 + 1.0f) / projection.m22;
		zfar = (projection.m23 - 1.0f) / projection.m22;
	} else
	{
		znear = projection.m23 / (projection.m22 - 1.0f);
		zfar = projection.m23 / (projection.m22 + 1.0f);
	}
}

UNIGINE_INLINE mat4 composeRotationXYZ(const vec3 &r);
UNIGINE_INLINE mat4 composeRotationXZY(const vec3 &r);
UNIGINE_INLINE mat4 composeRotationYXZ(const vec3 &r);
UNIGINE_INLINE mat4 composeRotationYZX(const vec3 &r);
UNIGINE_INLINE mat4 composeRotationZXY(const vec3 &r);
UNIGINE_INLINE mat4 composeRotationZYX(const vec3 &r);

UNIGINE_INLINE void decomposeTransform(const mat4 &m, vec3 &position, quat &rot, vec3 &scale);
UNIGINE_INLINE void decomposeTransform(const mat4 &m, vec4 &position, quat &rot);

UNIGINE_INLINE mat4 &composeTransform(mat4 &ret, const vec3 &position, const quat &rot, const vec3 &scale);
UNIGINE_INLINE mat4 &composeTransform(mat4 &ret, const vec4 &position, const quat &rot);

UNIGINE_INLINE mat4 &lerp(mat4 &ret, const mat4 &m0, const mat4 &m1, float k);
UNIGINE_INLINE mat4 lerp(const mat4 &m0, const mat4 &m1, float k)
{
	mat4 ret;
	return lerp(ret, m0, m1, k);
}

}
}

#ifdef __GNUC__
	#pragma GCC diagnostic pop
#endif
