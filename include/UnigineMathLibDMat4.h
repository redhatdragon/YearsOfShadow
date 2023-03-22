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
#include "UnigineMathLibMat4.h"

#ifdef __GNUC__
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wreorder"
#endif

namespace Unigine
{
namespace Math
{

struct alignas(16) dmat4
{
	UNIGINE_INLINE constexpr dmat4(
		double m00_, double m10_, double m20_,
		double m01_, double m11_, double m21_,
		double m02_, double m12_, double m22_,
		double m03_, double m13_, double m23_,
		ConstexprTag)
		: m00(m00_), m10(m10_), m20(m20_)
		, m01(m01_), m11(m11_), m21(m21_)
		, m02(m02_), m12(m12_), m22(m22_)
		, m03(m03_), m13(m13_), m23(m23_)
	{
	}
	UNIGINE_INLINE constexpr dmat4(double v,ConstexprTag)
		: m00(v), m10(v), m20(v)
		, m01(v), m11(v), m21(v)
		, m02(v), m12(v), m22(v)
		, m03(v), m13(v), m23(v)
	{
	}


	
	UNIGINE_INLINE dmat4(
		double m00_, double m10_, double m20_,
		double m01_, double m11_, double m21_,
		double m02_, double m12_, double m22_,
		double m03_, double m13_, double m23_)
		: m00(m00_), m10(m10_), m20(m20_)
		, m01(m01_), m11(m11_), m21(m21_)
		, m02(m02_), m12(m12_), m22(m22_)
		, m03(m03_), m13(m13_), m23(m23_)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE dmat4()
		: m00(1.0)
		, m01(0.0)
		, m02(0.0)
		, m03(0.0)
		, m10(0.0)
		, m11(1.0)
		, m12(0.0)
		, m13(0.0)
		, m20(0.0)
		, m21(0.0)
		, m22(1.0)
		, m23(0.0)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE dmat4(const dmat4 &m)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m);
	}
	UNIGINE_INLINE explicit dmat4(double v)
		: m00(v)
		, m10(v)
		, m20(v)
		, m01(v)
		, m11(v)
		, m21(v)
		, m02(v)
		, m12(v)
		, m22(v)
		, m03(v)
		, m13(v)
		, m23(v)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit dmat4(const mat2 &m)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m);
	}
	UNIGINE_INLINE explicit dmat4(const mat3 &m)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m);
	}
	UNIGINE_INLINE explicit dmat4(const mat4 &m)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m);
	}
	UNIGINE_INLINE explicit dmat4(const quat &q)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(q);
	}
	UNIGINE_INLINE explicit dmat4(const double *m, int transpose = 0)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m, transpose);
	}
	UNIGINE_INLINE explicit dmat4(const float *m, int transpose)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m, transpose);
	}
	UNIGINE_INLINE explicit dmat4(const float *m)
		: m00(m[0])
		, m10(m[1])
		, m20(m[2])
		, m01(m[4])
		, m11(m[5])
		, m21(m[6])
		, m02(m[8])
		, m12(m[9])
		, m22(m[10])
		, m03(m[12])
		, m13(m[13])
		, m23(m[14])
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE explicit dmat4(const mat3 &m, const dvec3 &v)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(m, v);
	}
	UNIGINE_INLINE explicit dmat4(const quat &q, const dvec3 &v)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(q, v);
	}
	UNIGINE_INLINE explicit dmat4(const dvec3 &c0, const dvec3 &c1, const dvec3 &c2, const dvec3 &c3)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		m00 = c0.x;
		m01 = c1.x;
		m02 = c2.x;
		m03 = c3.x;
		m10 = c0.y;
		m11 = c1.y;
		m12 = c2.y;
		m13 = c3.y;
		m20 = c0.z;
		m21 = c1.z;
		m22 = c2.z;
		m23 = c3.z;
	}

	UNIGINE_INLINE void set(int row, int column, double v)
	{
		assert((unsigned int)row < 3 && "dmat4::set(): bad row");
		assert((unsigned int)column < 4 && "dmat4::set(): bad column");
		mat[column * 3 + row] = v;
	}
	UNIGINE_INLINE void set(const mat2 &m)
	{
		m00 = m.m00;
		m01 = m.m01;
		m02 = 0.0;
		m03 = 0.0;
		m10 = m.m10;
		m11 = m.m11;
		m12 = 0.0;
		m13 = 0.0;
		m20 = 0.0;
		m21 = 0.0;
		m22 = 1.0;
		m23 = 0.0;
	}
	UNIGINE_INLINE void set(const mat3 &m)
	{
		m00 = m.m00;
		m01 = m.m01;
		m02 = m.m02;
		m03 = 0.0;
		m10 = m.m10;
		m11 = m.m11;
		m12 = m.m12;
		m13 = 0.0;
		m20 = m.m20;
		m21 = m.m21;
		m22 = m.m22;
		m23 = 0.0;
	}
	UNIGINE_INLINE void set(const mat4 &m)
	{
		m00 = m.m00;
		m01 = m.m01;
		m02 = m.m02;
		m03 = m.m03;
		m10 = m.m10;
		m11 = m.m11;
		m12 = m.m12;
		m13 = m.m13;
		m20 = m.m20;
		m21 = m.m21;
		m22 = m.m22;
		m23 = m.m23;
	}
	UNIGINE_INLINE void set(const dmat4 &m)
	{
		sse.v0 = m.sse.v0;
		sse.v1 = m.sse.v1;
		sse.v2 = m.sse.v2;
		sse.v3 = m.sse.v3;
		sse.v4 = m.sse.v4;
		sse.v5 = m.sse.v5;
	}
	UNIGINE_INLINE void set(const quat &q);
	UNIGINE_INLINE void set(const double *m, int transpose = 0)
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
		}
	}
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
		}
	}
	UNIGINE_INLINE void set(const mat3 &m, const dvec3 &v)
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
	}
	UNIGINE_INLINE void set(const quat &q, const dvec3 &v);

	UNIGINE_INLINE void get(double *m, int transpose = 0) const
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
		}
	}
	UNIGINE_INLINE double *get() { return mat; }
	UNIGINE_INLINE const double *get() const { return mat; }
	UNIGINE_INLINE double &get(int row, int column)
	{
		assert((unsigned int)row < 3 && "dmat4::get(): bad row");
		assert((unsigned int)column < 4 && "dmat4::get(): bad column");
		return mat[column * 3 + row];
	}
	UNIGINE_INLINE double get(int row, int column) const
	{
		assert((unsigned int)row < 3 && "dmat4::get(): bad row");
		assert((unsigned int)column < 4 && "dmat4::get(): bad column");
		return mat[column * 3 + row];
	}

	UNIGINE_INLINE operator double *() { return mat; }
	UNIGINE_INLINE operator const double *() const { return mat; }
	UNIGINE_INLINE operator void *() { return mat; }
	UNIGINE_INLINE operator const void *() const { return mat; }

	UNIGINE_INLINE double &operator[](int i)
	{
		assert((unsigned int)i < 12 && "dmat4::operator[](): bad index");
		return mat[i];
	}
	UNIGINE_INLINE double operator[](int i) const
	{
		assert((unsigned int)i < 12 && "dmat4::operator[](): bad index");
		return mat[i];
	}

	UNIGINE_INLINE dmat4 &operator=(const dmat4 &m) { set(m); return *this; }
	UNIGINE_INLINE dmat4 operator-() const
	{
		dmat4 ret;
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
		return ret;
	}

	UNIGINE_INLINE dmat4 &operator*=(double v);
	UNIGINE_INLINE dmat4 &operator*=(const dmat4 &m);

	UNIGINE_INLINE dmat4 &operator+=(const dmat4 &m);
	UNIGINE_INLINE dmat4 &operator-=(const dmat4 &m);

	UNIGINE_INLINE void setRow(int row, const dvec4 &v)
	{
		assert(row < 4 && row >= 0 && "dmat4::setRow(): bad row");
		switch (row)
		{
			case 0:
				m00 = v.x;
				m01 = v.y;
				m02 = v.z;
				m03 = v.w;
				break;
			case 1:
				m10 = v.x;
				m11 = v.y;
				m12 = v.z;
				m13 = v.w;
				break;
			case 2:
				m20 = v.x;
				m21 = v.y;
				m22 = v.z;
				m23 = v.w;
				break;
		}
	}
	UNIGINE_INLINE void setRow3(int row, const dvec3 &v)
	{
		assert(row < 4 && row >= 0 && "dmat4::setRow3(): bad row");
		switch (row)
		{
			case 0:
				m00 = v.x;
				m01 = v.y;
				m02 = v.z;
				break;
			case 1:
				m10 = v.x;
				m11 = v.y;
				m12 = v.z;
				break;
			case 2:
				m20 = v.x;
				m21 = v.y;
				m22 = v.z;
				break;
		}
	}
	UNIGINE_INLINE dvec4 getRow(int row) const
	{
		assert(row < 4 && row >= 0 && "dmat4::getRow(): bad row");
		switch (row)
		{
			case 0: return dvec4(m00, m01, m02, m03);
			case 1: return dvec4(m10, m11, m12, m13);
			case 2: return dvec4(m20, m21, m22, m23);
			case 3: return dvec4(0.0, 0.0, 0.0, 1.0);
		}
		return dvec4_zero;
	}
	UNIGINE_INLINE dvec3 getRow3(int row) const
	{
		assert(row < 4 && row >= 0 && "dmat4::getRow3(): bad row");
		switch (row)
		{
			case 0: return dvec3(m00, m01, m02);
			case 1: return dvec3(m10, m11, m12);
			case 2: return dvec3(m20, m21, m22);
			case 3: return dvec3(0.0, 0.0, 0.0);
		}
		return dvec3_zero;
	}

	UNIGINE_INLINE void setColumn(int column, const dvec4 &v)
	{
		assert(column < 4 && column >= 0 && "dmat4::setColumn(): bad column");
		switch (column)
		{
			case 0:
				m00 = v.x;
				m10 = v.y;
				m20 = v.z;
				break;
			case 1:
				m01 = v.x;
				m11 = v.y;
				m21 = v.z;
				break;
			case 2:
				m02 = v.x;
				m12 = v.y;
				m22 = v.z;
				break;
			case 3:
				m03 = v.x;
				m13 = v.y;
				m23 = v.z;
				break;
		}
	}
	UNIGINE_INLINE void setColumn3(int column, const dvec3 &v)
	{
		assert(column < 4 && column >= 0 && "dmat4::setColumn3(): bad column");
		switch (column)
		{
			case 0:
				m00 = v.x;
				m10 = v.y;
				m20 = v.z;
				break;
			case 1:
				m01 = v.x;
				m11 = v.y;
				m21 = v.z;
				break;
			case 2:
				m02 = v.x;
				m12 = v.y;
				m22 = v.z;
				break;
			case 3:
				m03 = v.x;
				m13 = v.y;
				m23 = v.z;
				break;
		}
	}
	UNIGINE_INLINE dvec4 getColumn(int column) const
	{
		assert(column < 4 && column >= 0 && "dmat4::getColumn(): bad column");
		switch (column)
		{
			case 0: return dvec4(m00, m10, m20, 0.0);
			case 1: return dvec4(m01, m11, m21, 0.0);
			case 2: return dvec4(m02, m12, m22, 0.0);
			case 3: return dvec4(m03, m13, m23, 1.0);
		}
		return dvec4_zero;
	}
	UNIGINE_INLINE dvec3 getColumn3(int column) const
	{
		assert(column < 4 && column >= 0 && "dmat4::getColumn3(): bad column");
		switch (column)
		{
			case 0: return dvec3(m00, m10, m20);
			case 1: return dvec3(m01, m11, m21);
			case 2: return dvec3(m02, m12, m22);
			case 3: return dvec3(m03, m13, m23);
		}
		return dvec3_zero;
	}

	UNIGINE_INLINE dvec3 getAxisX() const { return getColumn3(0).normalize(); }
	UNIGINE_INLINE dvec3 getAxisY() const { return getColumn3(1).normalize(); }
	UNIGINE_INLINE dvec3 getAxisZ() const { return getColumn3(2).normalize(); }

	UNIGINE_INLINE void setZero()
	{
		m00 = 0.0;
		m01 = 0.0;
		m02 = 0.0;
		m03 = 0.0;
		m10 = 0.0;
		m11 = 0.0;
		m12 = 0.0;
		m13 = 0.0;
		m20 = 0.0;
		m21 = 0.0;
		m22 = 0.0;
		m23 = 0.0;
	}
	UNIGINE_INLINE void setIdentity()
	{
		m00 = 1.0;
		m01 = 0.0;
		m02 = 0.0;
		m03 = 0.0;
		m10 = 0.0;
		m11 = 1.0;
		m12 = 0.0;
		m13 = 0.0;
		m20 = 0.0;
		m21 = 0.0;
		m22 = 1.0;
		m23 = 0.0;
	}

	UNIGINE_INLINE void setTranslate(const dvec3 &v)
	{
		m00 = 1.0;
		m01 = 0.0;
		m02 = 0.0;
		m03 = v.x;
		m10 = 0.0;
		m11 = 1.0;
		m12 = 0.0;
		m13 = v.y;
		m20 = 0.0;
		m21 = 0.0;
		m22 = 1.0;
		m23 = v.z;
	}
	UNIGINE_INLINE dvec3 getTranslate() const
	{
		return dvec3(m03, m13, m23);
	}

	UNIGINE_INLINE void setRotate(const dvec3 &axis, double angle)
	{
		double s, c;
		Math::sincos(angle * Consts::DEG2RAD, s, c);
		dvec3 v = normalize(axis);
		double xx = v.x * v.x;
		double yy = v.y * v.y;
		double zz = v.z * v.z;
		double xy = v.x * v.y;
		double yz = v.y * v.z;
		double zx = v.z * v.x;
		double xs = v.x * s;
		double ys = v.y * s;
		double zs = v.z * s;
		m00 = (1.0 - c) * xx + c;
		m01 = (1.0 - c) * xy - zs;
		m02 = (1.0 - c) * zx + ys;
		m03 = 0.0;
		m10 = (1.0 - c) * xy + zs;
		m11 = (1.0 - c) * yy + c;
		m12 = (1.0 - c) * yz - xs;
		m13 = 0.0;
		m20 = (1.0 - c) * zx - ys;
		m21 = (1.0 - c) * yz + xs;
		m22 = (1.0 - c) * zz + c;
		m23 = 0.0;
	}
	UNIGINE_INLINE quat getRotate() const;

	UNIGINE_INLINE void setRotateX(double angle)
	{
		double s, c;
		Math::sincos(angle * Consts::DEG2RAD, s, c);
		m00 = 1.0;
		m01 = 0.0;
		m02 = 0.0;
		m03 = 0.0;
		m10 = 0.0;
		m11 = c;
		m12 = -s;
		m13 = 0.0;
		m20 = 0.0;
		m21 = s;
		m22 = c;
		m23 = 0.0;
	}
	UNIGINE_INLINE void setRotateY(double angle)
	{
		double s, c;
		Math::sincos(angle * Consts::DEG2RAD, s, c);
		m00 = c;
		m01 = 0.0;
		m02 = s;
		m03 = 0.0;
		m10 = 0.0;
		m11 = 1.0;
		m12 = 0.0;
		m13 = 0.0;
		m20 = -s;
		m21 = 0.0;
		m22 = c;
		m23 = 0.0;
	}
	UNIGINE_INLINE void setRotateZ(double angle)
	{
		double s, c;
		Math::sincos(angle * Consts::DEG2RAD, s, c);
		m00 = c;
		m01 = -s;
		m02 = 0.0;
		m03 = 0.0;
		m10 = s;
		m11 = c;
		m12 = 0.0;
		m13 = 0.0;
		m20 = 0.0;
		m21 = 0.0;
		m22 = 1.0;
		m23 = 0.0;
	}

	UNIGINE_INLINE void setScale(const dvec3 &v)
	{
		m00 = v.x;
		m01 = 0.0;
		m02 = 0.0;
		m03 = 0.0;
		m10 = 0.0;
		m11 = v.y;
		m12 = 0.0;
		m13 = 0.0;
		m20 = 0.0;
		m21 = 0.0;
		m22 = v.z;
		m23 = 0.0;
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

	UNIGINE_INLINE double determinant() const
	{
		double det = m00 * (m11 * m22 - m12 * m21);
		det -= m01 * (m10 * m22 - m12 * m20);
		det += m02 * (m10 * m21 - m11 * m20);
		return det;
	}

	UNIGINE_INLINE unsigned int hash() const
	{
		unsigned long long h = hashInteger(m00);
		h = hashCombine(h, m10);
		h = hashCombine(h, m20);
		h = hashCombine(h, m01);
		h = hashCombine(h, m11);
		h = hashCombine(h, m21);
		h = hashCombine(h, m03);
		h = hashCombine(h, m13);
		h = hashCombine(h, m23);
		return hashCast64To32(h);
	}

	#ifdef USE_SSE
		struct sse_data
		{
			__m128d v0;
			__m128d v1;
			__m128d v2;
			__m128d v3;
			__m128d v4;
			__m128d v5;
		}; 
	#endif

	union
	{
		struct
		{
			double m00, m10, m20;
			double m01, m11, m21;
			double m02, m12, m22;
			double m03, m13, m23;
		};
		double mat[12];

		#ifdef USE_SSE
			sse_data sse; // DO NOT TOUCH UNLESS ABSOLUTELY SURE
		#endif
	};
};

constexpr dmat4 dmat4_zero(0.0, ConstexprTag{});
constexpr dmat4 dmat4_one(1.0, ConstexprTag{});
constexpr dmat4 dmat4_identity(1.0, 0.0, 0.0,
							   0.0, 1.0, 0.0,
							   0.0, 0.0, 1.0,
							   0.0, 0.0, 0.0, ConstexprTag{});

UNIGINE_INLINE dmat4 translate(const dvec3 &v)
{
	dmat4 ret;
	ret.setTranslate(v);
	return ret;
}
UNIGINE_INLINE dmat4 translate(const dvec4 &v)
{
	dmat4 ret;
	ret.setTranslate(dvec3(v));
	return ret;
}
UNIGINE_INLINE dmat4 translate(const dvec2 &v)
{
	dmat4 ret;
	ret.setTranslate(dvec3(v));
	return ret;
}
UNIGINE_INLINE dmat4 translate(double x, double y = 0.0, double z = 0.0)
{
	dmat4 ret;
	ret.setTranslate(dvec3(x, y, z));
	return ret;
}

UNIGINE_INLINE dmat4 rotate(const dvec3 &axis, double angle)
{
	dmat4 ret;
	ret.setRotate(axis, angle);
	return ret;
}
UNIGINE_INLINE dmat4 rotate(double x, double y, double z, double angle)
{
	return rotate(dvec3(x, y, z), angle);
}

UNIGINE_INLINE dmat4 rotateX(double angle)
{
	dmat4 ret;
	ret.setRotateX(angle);
	return ret;
}
UNIGINE_INLINE dmat4 rotateY(double angle)
{
	dmat4 ret;
	ret.setRotateY(angle);
	return ret;
}
UNIGINE_INLINE dmat4 rotateZ(double angle)
{
	dmat4 ret;
	ret.setRotateZ(angle);
	return ret;
}

UNIGINE_INLINE dmat4 scale(const dvec3 &v)
{
	dmat4 ret;
	ret.setScale(v);
	return ret;
}
UNIGINE_INLINE dmat4 scale(double x, double y, double z)
{
	return scale(dvec3(x, y, z));
}
UNIGINE_INLINE dmat4 scale(double x)
{
	return scale(dvec3(x, x, x));
}

UNIGINE_INLINE double determinant(const dmat4 &m) { return m.determinant(); }


UNIGINE_INLINE int compare(const dmat4 &m0, const dmat4 &m1)
{
	return (compare(m0.m00, m1.m00) && compare(m0.m10, m1.m10) && compare(m0.m20, m1.m20) &&
			compare(m0.m01, m1.m01) && compare(m0.m11, m1.m11) && compare(m0.m21, m1.m21) &&
			compare(m0.m02, m1.m02) && compare(m0.m12, m1.m12) && compare(m0.m22, m1.m22) &&
			compare(m0.m03, m1.m03) && compare(m0.m13, m1.m13) && compare(m0.m23, m1.m23));
}
UNIGINE_INLINE int compare(const dmat4 &m0, const dmat4 &m1, double epsilon)
{
	return (compare(m0.m00, m1.m00, epsilon) && compare(m0.m10, m1.m10, epsilon) && compare(m0.m20, m1.m20, epsilon) &&
			compare(m0.m01, m1.m01, epsilon) && compare(m0.m11, m1.m11, epsilon) && compare(m0.m21, m1.m21, epsilon) &&
			compare(m0.m02, m1.m02, epsilon) && compare(m0.m12, m1.m12, epsilon) && compare(m0.m22, m1.m22, epsilon) &&
			compare(m0.m03, m1.m03, epsilon) && compare(m0.m13, m1.m13, epsilon) && compare(m0.m23, m1.m23, epsilon));
}
UNIGINE_INLINE int operator==(const dmat4 &m0, const dmat4 &m1) { return compare(m0, m1); }
UNIGINE_INLINE int operator!=(const dmat4 &m0, const dmat4 &m1) { return !compare(m0, m1); }

UNIGINE_INLINE dmat4 &mul(dmat4 &ret, const dmat4 &m, double v)
{
	#ifdef USE_SSE
		__m128d temp = _mm_set1_pd(v);
		ret.sse.v0 = _mm_mul_pd(m.sse.v0, temp);
		ret.sse.v1 = _mm_mul_pd(m.sse.v1, temp);
		ret.sse.v2 = _mm_mul_pd(m.sse.v2, temp);
		ret.sse.v3 = _mm_mul_pd(m.sse.v3, temp);
		ret.sse.v4 = _mm_mul_pd(m.sse.v4, temp);
		ret.sse.v5 = _mm_mul_pd(m.sse.v5, temp);
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
	#endif
	return ret;
}
UNIGINE_INLINE dmat4 &mul(dmat4 &ret, const dmat4 &m0, const dmat4 &m1)
{
	#ifdef USE_SSE
		__m128d m0_col0 = m0.sse.v0;
		__m128d m0_col1 = m0.sse.v1;
		__m128d m0_col2 = m0.sse.v2;
		__m128d m0_col3 = m0.sse.v3;
		__m128d m0_col4 = m0.sse.v4;
		__m128d m0_col5 = m0.sse.v5;

		__m128d m1_col0 = m1.sse.v0;
		__m128d m1_col1 = m1.sse.v1;
		__m128d m1_col2 = m1.sse.v2;
		__m128d m1_col3 = m1.sse.v3;
		__m128d m1_col4 = m1.sse.v4;
		__m128d m1_col5 = m1.sse.v5;

		__m128d col_01 = _mm_shuffle_pd(m0_col0, m0_col1, _MM_PERM22(Y, X));
		__m128d col_12 = _mm_shuffle_pd(m0_col1, m0_col2, _MM_PERM22(Y, X));
		__m128d col_34 = _mm_shuffle_pd(m0_col3, m0_col4, _MM_PERM22(Y, X));

		__m128d res_0 = _mm_mul_pd(m0_col0, _MM_SWIZZLE2(m1_col0, X, X));
		__m128d res_1 = _mm_mul_pd(col_12, _MM_SWIZZLE2(m1_col0, Y, Y));
		__m128d res_2 = _mm_mul_pd(m0_col3, _MM_SWIZZLE2(m1_col1, X, X));
		ret.sse.v0 = _mm_add_pd(_mm_add_pd(res_0, res_1), res_2);

		res_0 = _mm_mul_pd(col_01, _MM_SWIZZLE2(m1_col1, Y, Y));
		res_1 = _mm_mul_pd(m0_col2, _MM_SWIZZLE2(m1_col2, X, X));
		res_2 = _mm_mul_pd(col_34, _MM_SWIZZLE2(m1_col2, Y, Y));
		ret.sse.v2 = _mm_add_pd(_mm_add_pd(res_0, res_1), res_2);

		res_0 = _mm_mul_pd(m0_col0, _MM_SWIZZLE2(m1_col3, X, X));
		res_1 = _mm_mul_pd(col_12, _MM_SWIZZLE2(m1_col3, Y, Y));
		res_2 = _mm_mul_pd(m0_col3, _MM_SWIZZLE2(m1_col4, X, X));
		ret.sse.v3 = _mm_add_pd(_mm_add_pd(res_0, res_1), res_2);

		res_0 = _mm_mul_pd(col_01, _MM_SWIZZLE2(m1_col4, Y, Y));
		res_1 = _mm_mul_pd(m0_col2, _MM_SWIZZLE2(m1_col5, X, X));
		res_2 = _mm_mul_pd(col_34, _MM_SWIZZLE2(m1_col5, Y, Y));
		ret.sse.v5 = _mm_add_pd(_mm_add_pd(res_0, res_1), _mm_add_pd(res_2, m0_col5));

		ret.m20 = m0.m20 * m1.m00 + m0.m21 * m1.m10 + m0.m22 * m1.m20;
		ret.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11 + m0.m02 * m1.m21;
		ret.m22 = m0.m20 * m1.m02 + m0.m21 * m1.m12 + m0.m22 * m1.m22;
		ret.m03 = m0.m00 * m1.m03 + m0.m01 * m1.m13 + m0.m02 * m1.m23 + m0.m03;
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
		ret.m03 = m0.m00 * m1.m03 + m0.m01 * m1.m13 + m0.m02 * m1.m23 + m0.m03;
		ret.m13 = m0.m10 * m1.m03 + m0.m11 * m1.m13 + m0.m12 * m1.m23 + m0.m13;
		ret.m23 = m0.m20 * m1.m03 + m0.m21 * m1.m13 + m0.m22 * m1.m23 + m0.m23;
	#endif
	return ret;
}
UNIGINE_INLINE vec2 &mul(vec2 &ret, const dmat4 &m, const vec2 &v)
{
	float x = v.x;
	float y = v.y;
	ret.x = (float)(m.m00 * x + m.m01 * y + m.m03);
	ret.y = (float)(m.m10 * x + m.m11 * y + m.m13);
	return ret;
}
UNIGINE_INLINE vec2 &mul(vec2 &ret, const vec2 &v, const dmat4 &m)
{
	float x = v.x;
	float y = v.y;
	ret.x = (float)(m.m00 * x + m.m10 * y);
	ret.y = (float)(m.m01 * x + m.m11 * y);
	return ret;
}
UNIGINE_INLINE vec3 &mul(vec3 &ret, const dmat4 &m, const vec3 &v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	ret.x = (float)(m.m00 * x + m.m01 * y + m.m02 * z + m.m03);
	ret.y = (float)(m.m10 * x + m.m11 * y + m.m12 * z + m.m13);
	ret.z = (float)(m.m20 * x + m.m21 * y + m.m22 * z + m.m23);
	return ret;
}
UNIGINE_INLINE vec3 &mul(vec3 &ret, const vec3 &v, const dmat4 &m)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	ret.x = (float)(m.m00 * x + m.m10 * y + m.m20 * z);
	ret.y = (float)(m.m01 * x + m.m11 * y + m.m21 * z);
	ret.z = (float)(m.m02 * x + m.m12 * y + m.m22 * z);
	return ret;
}
UNIGINE_INLINE vec4 &mul(vec4 &ret, const dmat4 &m, const vec4 &v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	float w = v.w;
	ret.x = (float)(m.m00 * x + m.m01 * y + m.m02 * z + m.m03 * w);
	ret.y = (float)(m.m10 * x + m.m11 * y + m.m12 * z + m.m13 * w);
	ret.z = (float)(m.m20 * x + m.m21 * y + m.m22 * z + m.m23 * w);
	ret.w = w;
	return ret;
}
UNIGINE_INLINE vec4 &mul(vec4 &ret, const vec4 &v, const dmat4 &m)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	float w = v.w;
	ret.x = (float)(m.m00 * x + m.m10 * y + m.m20 * z);
	ret.y = (float)(m.m01 * x + m.m11 * y + m.m21 * z);
	ret.z = (float)(m.m02 * x + m.m12 * y + m.m22 * z);
	ret.w = (float)(m.m03 * x + m.m13 * y + m.m23 * z + w);
	return ret;
}
UNIGINE_INLINE vec2 &mul(vec2 &ret, const dmat4 &m, const dvec2 &v)
{
	double x = v.x;
	double y = v.y;
	ret.x = (float)(m.m00 * x + m.m01 * y + m.m03);
	ret.y = (float)(m.m10 * x + m.m11 * y + m.m13);
	return ret;
}
UNIGINE_INLINE vec2 &mul(vec2 &ret, const dvec2 &v, const dmat4 &m)
{
	double x = v.x;
	double y = v.y;
	ret.x = (float)(m.m00 * x + m.m10 * y);
	ret.y = (float)(m.m01 * x + m.m11 * y);
	return ret;
}
UNIGINE_INLINE vec3 &mul(vec3 &ret, const dmat4 &m, const dvec3 &v)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	ret.x = (float)(m.m00 * x + m.m01 * y + m.m02 * z + m.m03);
	ret.y = (float)(m.m10 * x + m.m11 * y + m.m12 * z + m.m13);
	ret.z = (float)(m.m20 * x + m.m21 * y + m.m22 * z + m.m23);
	return ret;
}
UNIGINE_INLINE vec3 &mul(vec3 &ret, const dvec3 &v, const dmat4 &m)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	ret.x = (float)(m.m00 * x + m.m10 * y + m.m20 * z);
	ret.y = (float)(m.m01 * x + m.m11 * y + m.m21 * z);
	ret.z = (float)(m.m02 * x + m.m12 * y + m.m22 * z);
	return ret;
}
UNIGINE_INLINE vec4 &mul(vec4 &ret, const dmat4 &m, const dvec4 &v)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	double w = v.w;
	ret.x = (float)(m.m00 * x + m.m01 * y + m.m02 * z + m.m03 * w);
	ret.y = (float)(m.m10 * x + m.m11 * y + m.m12 * z + m.m13 * w);
	ret.z = (float)(m.m20 * x + m.m21 * y + m.m22 * z + m.m23 * w);
	ret.w = (float)w;
	return ret;
}
UNIGINE_INLINE vec4 &mul(vec4 &ret, const dvec4 &v, const dmat4 &m)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	double w = v.w;
	ret.x = (float)(m.m00 * x + m.m10 * y + m.m20 * z);
	ret.y = (float)(m.m01 * x + m.m11 * y + m.m21 * z);
	ret.z = (float)(m.m02 * x + m.m12 * y + m.m22 * z);
	ret.w = (float)(m.m03 * x + m.m13 * y + m.m23 * z + w);
	return ret;
}
UNIGINE_INLINE dvec2 &mul(dvec2 &ret, const dmat4 &m, const dvec2 &v)
{
	double x = v.x;
	double y = v.y;
	ret.x = m.m00 * x + m.m01 * y + m.m03;
	ret.y = m.m10 * x + m.m11 * y + m.m13;
	return ret;
}
UNIGINE_INLINE dvec2 &mul(dvec2 &ret, const dvec2 &v, const dmat4 &m)
{
	double x = v.x;
	double y = v.y;
	ret.x = m.m00 * x + m.m10 * y;
	ret.y = m.m01 * x + m.m11 * y;
	return ret;
}
UNIGINE_INLINE dvec3 &mul(dvec3 &ret, const dmat4 &m, const dvec3 &v)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	ret.x = m.m00 * x + m.m01 * y + m.m02 * z + m.m03;
	ret.y = m.m10 * x + m.m11 * y + m.m12 * z + m.m13;
	ret.z = m.m20 * x + m.m21 * y + m.m22 * z + m.m23;
	return ret;
}
UNIGINE_INLINE dvec3 &mul(dvec3 &ret, const dvec3 &v, const dmat4 &m)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	ret.x = m.m00 * x + m.m10 * y + m.m20 * z;
	ret.y = m.m01 * x + m.m11 * y + m.m21 * z;
	ret.z = m.m02 * x + m.m12 * y + m.m22 * z;
	return ret;
}
UNIGINE_INLINE dvec4 &mul(dvec4 &ret, const dmat4 &m, const dvec4 &v)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	double w = v.w;
	ret.x = m.m00 * x + m.m01 * y + m.m02 * z + m.m03 * w;
	ret.y = m.m10 * x + m.m11 * y + m.m12 * z + m.m13 * w;
	ret.z = m.m20 * x + m.m21 * y + m.m22 * z + m.m23 * w;
	ret.w = w;
	return ret;
}
UNIGINE_INLINE dvec4 &mul(dvec4 &ret, const dvec4 &v, const dmat4 &m)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	double w = v.w;
	ret.x = m.m00 * x + m.m10 * y + m.m20 * z;
	ret.y = m.m01 * x + m.m11 * y + m.m21 * z;
	ret.z = m.m02 * x + m.m12 * y + m.m22 * z;
	ret.w = m.m03 * x + m.m13 * y + m.m23 * z + w;
	return ret;
}
UNIGINE_INLINE dmat4 operator*(const dmat4 &m, double v)
{
	dmat4 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE vec2 operator*(const dmat4 &m, const vec2 &v)
{
	vec2 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE vec2 operator*(const vec2 &v, const dmat4 &m)
{
	vec2 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE vec3 operator*(const dmat4 &m, const vec3 &v)
{
	vec3 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE vec3 operator*(const vec3 &v, const dmat4 &m)
{
	vec3 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE vec4 operator*(const dmat4 &m, const vec4 &v)
{
	vec4 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE vec4 operator*(const vec4 &v, const dmat4 &m)
{
	vec4 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE dvec2 operator*(const dmat4 &m, const dvec2 &v)
{
	dvec2 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE dvec2 operator*(const dvec2 &v, const dmat4 &m)
{
	dvec2 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE dvec3 operator*(const dmat4 &m, const dvec3 &v)
{
	dvec3 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE dvec3 operator*(const dvec3 &v, const dmat4 &m)
{
	dvec3 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE dvec4 operator*(const dmat4 &m, const dvec4 &v)
{
	dvec4 ret;
	return mul(ret, m, v);
}
UNIGINE_INLINE dvec4 operator*(const dvec4 &v, const dmat4 &m)
{
	dvec4 ret;
	return mul(ret, v, m);
}
UNIGINE_INLINE dmat4 operator*(const dmat4 &m0, const dmat4 &m1)
{
	dmat4 ret;
	return mul(ret, m0, m1);
}
UNIGINE_INLINE dmat4 &dmat4::operator*=(double v)
{
	return mul(*this, *this, v);
}
UNIGINE_INLINE dmat4 &dmat4::operator*=(const dmat4 &m)
{
	return mul(*this, dmat4(*this), m);
}


UNIGINE_INLINE vec2 &mul3(vec2 &ret, const dmat4 &m, const vec2 &v)
{
	float x = v.x;
	float y = v.y;
	ret.x = (float)(m.m00 * x + m.m01 * y);
	ret.y = (float)(m.m10 * x + m.m11 * y);
	return ret;
}
UNIGINE_INLINE vec2 &mul3(vec2 &ret, const vec2 &v, const dmat4 &m)
{
	float x = v.x;
	float y = v.y;
	ret.x = (float)(m.m00 * x + m.m10 * y);
	ret.y = (float)(m.m01 * x + m.m11 * y);
	return ret;
}
UNIGINE_INLINE vec3 &mul3(vec3 &ret, const dmat4 &m, const vec3 &v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	ret.x = (float)(m.m00 * x + m.m01 * y + m.m02 * z);
	ret.y = (float)(m.m10 * x + m.m11 * y + m.m12 * z);
	ret.z = (float)(m.m20 * x + m.m21 * y + m.m22 * z);
	return ret;
}
UNIGINE_INLINE vec3 &mul3(vec3 &ret, const vec3 &v, const dmat4 &m)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	ret.x = (float)(m.m00 * x + m.m10 * y + m.m20 * z);
	ret.y = (float)(m.m01 * x + m.m11 * y + m.m21 * z);
	ret.z = (float)(m.m02 * x + m.m12 * y + m.m22 * z);
	return ret;
}
UNIGINE_INLINE vec4 &mul3(vec4 &ret, const dmat4 &m, const vec4 &v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	ret.x = (float)(m.m00 * x + m.m01 * y + m.m02 * z);
	ret.y = (float)(m.m10 * x + m.m11 * y + m.m12 * z);
	ret.z = (float)(m.m20 * x + m.m21 * y + m.m22 * z);
	return ret;
}
UNIGINE_INLINE vec4 &mul3(vec4 &ret, const vec4 &v, const dmat4 &m)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	ret.x = (float)(m.m00 * x + m.m10 * y + m.m20 * z);
	ret.y = (float)(m.m01 * x + m.m11 * y + m.m21 * z);
	ret.z = (float)(m.m02 * x + m.m12 * y + m.m22 * z);
	return ret;
}
UNIGINE_INLINE vec2 &mul3(vec2 &ret, const dmat4 &m, const dvec2 &v)
{
	double x = v.x;
	double y = v.y;
	ret.x = (float)(m.m00 * x + m.m01 * y);
	ret.y = (float)(m.m10 * x + m.m11 * y);
	return ret;
}
UNIGINE_INLINE vec2 &mul3(vec2 &ret, const dvec2 &v, const dmat4 &m)
{
	double x = v.x;
	double y = v.y;
	ret.x = (float)(m.m00 * x + m.m10 * y);
	ret.y = (float)(m.m01 * x + m.m11 * y);
	return ret;
}
UNIGINE_INLINE vec3 &mul3(vec3 &ret, const dmat4 &m, const dvec3 &v)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	ret.x = (float)(m.m00 * x + m.m01 * y + m.m02 * z);
	ret.y = (float)(m.m10 * x + m.m11 * y + m.m12 * z);
	ret.z = (float)(m.m20 * x + m.m21 * y + m.m22 * z);
	return ret;
}
UNIGINE_INLINE vec3 &mul3(vec3 &ret, const dvec3 &v, const dmat4 &m)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	ret.x = (float)(m.m00 * x + m.m10 * y + m.m20 * z);
	ret.y = (float)(m.m01 * x + m.m11 * y + m.m21 * z);
	ret.z = (float)(m.m02 * x + m.m12 * y + m.m22 * z);
	return ret;
}
UNIGINE_INLINE vec4 &mul3(vec4 &ret, const dmat4 &m, const dvec4 &v)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	ret.x = (float)(m.m00 * x + m.m01 * y + m.m02 * z);
	ret.y = (float)(m.m10 * x + m.m11 * y + m.m12 * z);
	ret.z = (float)(m.m20 * x + m.m21 * y + m.m22 * z);
	return ret;
}
UNIGINE_INLINE vec4 &mul3(vec4 &ret, const dvec4 &v, const dmat4 &m)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	ret.x = (float)(m.m00 * x + m.m10 * y + m.m20 * z);
	ret.y = (float)(m.m01 * x + m.m11 * y + m.m21 * z);
	ret.z = (float)(m.m02 * x + m.m12 * y + m.m22 * z);
	return ret;
}
UNIGINE_INLINE dvec2 &mul3(dvec2 &ret, const dmat4 &m, const dvec2 &v)
{
	double x = v.x;
	double y = v.y;
	ret.x = m.m00 * x + m.m01 * y;
	ret.y = m.m10 * x + m.m11 * y;
	return ret;
}
UNIGINE_INLINE dvec2 &mul3(dvec2 &ret, const dvec2 &v, const dmat4 &m)
{
	double x = v.x;
	double y = v.y;
	ret.x = m.m00 * x + m.m10 * y;
	ret.y = m.m01 * x + m.m11 * y;
	return ret;
}
UNIGINE_INLINE dvec3 &mul3(dvec3 &ret, const dmat4 &m, const dvec3 &v)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	ret.x = m.m00 * x + m.m01 * y + m.m02 * z;
	ret.y = m.m10 * x + m.m11 * y + m.m12 * z;
	ret.z = m.m20 * x + m.m21 * y + m.m22 * z;
	return ret;
}
UNIGINE_INLINE dvec3 &mul3(dvec3 &ret, const dvec3 &v, const dmat4 &m)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	ret.x = m.m00 * x + m.m10 * y + m.m20 * z;
	ret.y = m.m01 * x + m.m11 * y + m.m21 * z;
	ret.z = m.m02 * x + m.m12 * y + m.m22 * z;
	return ret;
}
UNIGINE_INLINE dvec4 &mul3(dvec4 &ret, const dmat4 &m, const dvec4 &v)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	ret.x = m.m00 * x + m.m01 * y + m.m02 * z;
	ret.y = m.m10 * x + m.m11 * y + m.m12 * z;
	ret.z = m.m20 * x + m.m21 * y + m.m22 * z;
	return ret;
}
UNIGINE_INLINE dvec4 &mul3(dvec4 &ret, const dvec4 &v, const dmat4 &m)
{
	double x = v.x;
	double y = v.y;
	double z = v.z;
	ret.x = m.m00 * x + m.m10 * y + m.m20 * z;
	ret.y = m.m01 * x + m.m11 * y + m.m21 * z;
	ret.z = m.m02 * x + m.m12 * y + m.m22 * z;
	return ret;
}
UNIGINE_INLINE dmat4 &mul3(dmat4 &ret, const dmat4 &m0, const dmat4 &m1)
{
	#ifdef USE_SSE
		__m128d m0_col0 = m0.sse.v0;
		__m128d m0_col1 = m0.sse.v1;
		__m128d m0_col2 = m0.sse.v2;
		__m128d m0_col3 = m0.sse.v3;
		__m128d m0_col4 = m0.sse.v4;

		__m128d m1_col0 = m1.sse.v0;
		__m128d m1_col1 = m1.sse.v1;
		__m128d m1_col2 = m1.sse.v2;
		__m128d m1_col3 = m1.sse.v3;
		__m128d m1_col4 = m1.sse.v4;

		__m128d col_01 = _mm_shuffle_pd(m0_col0, m0_col1, _MM_PERM22(Y, X));
		__m128d col_12 = _mm_shuffle_pd(m0_col1, m0_col2, _MM_PERM22(Y, X));
		__m128d col_34 = _mm_shuffle_pd(m0_col3, m0_col4, _MM_PERM22(Y, X));

		__m128d res_0 = _mm_mul_pd(m0_col0, _MM_SWIZZLE2(m1_col0, X, X));
		__m128d res_1 = _mm_mul_pd(col_12, _MM_SWIZZLE2(m1_col0, Y, Y));
		__m128d res_2 = _mm_mul_pd(m0_col3, _MM_SWIZZLE2(m1_col1, X, X));
		ret.sse.v0 = _mm_add_pd(_mm_add_pd(res_0, res_1), res_2);

		res_0 = _mm_mul_pd(col_01, _MM_SWIZZLE2(m1_col1, Y, Y));
		res_1 = _mm_mul_pd(m0_col2, _MM_SWIZZLE2(m1_col2, X, X));
		res_2 = _mm_mul_pd(col_34, _MM_SWIZZLE2(m1_col2, Y, Y));
		ret.sse.v2 = _mm_add_pd(_mm_add_pd(res_0, res_1), res_2);

		res_0 = _mm_mul_pd(m0_col0, _MM_SWIZZLE2(m1_col3, X, X));
		res_1 = _mm_mul_pd(col_12, _MM_SWIZZLE2(m1_col3, Y, Y));
		res_2 = _mm_mul_pd(m0_col3, _MM_SWIZZLE2(m1_col4, X, X));
		ret.sse.v3 = _mm_add_pd(_mm_add_pd(res_0, res_1), res_2);

		ret.m20 = m0.m20 * m1.m00 + m0.m21 * m1.m10 + m0.m22 * m1.m20;
		ret.m01 = m0.m00 * m1.m01 + m0.m01 * m1.m11 + m0.m02 * m1.m21;
		ret.m22 = m0.m20 * m1.m02 + m0.m21 * m1.m12 + m0.m22 * m1.m22;
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

UNIGINE_INLINE dmat4 &mul4(dmat4 &ret, const dmat4 &m0, const dmat4 &m1)
{
	return mul(ret, m0, m1);
}
UNIGINE_INLINE dmat4 &mult(dmat4 &ret, const dmat4 &m, const dvec3 &v)
{
	ret.m00 = m.m00;
	ret.m01 = m.m01;
	ret.m02 = m.m02;
	ret.m10 = m.m10;
	ret.m11 = m.m11;
	ret.m12 = m.m12;
	ret.m20 = m.m20;
	ret.m21 = m.m21;
	ret.m22 = m.m22;
	ret.m03 = m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03;
	ret.m13 = m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13;
	ret.m23 = m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23;
	return ret;
}

UNIGINE_INLINE dmat4 &add(dmat4 &ret, const dmat4 &m0, const dmat4 &m1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_add_pd(m0.sse.v0, m1.sse.v0);
		ret.sse.v1 = _mm_add_pd(m0.sse.v1, m1.sse.v1);
		ret.sse.v2 = _mm_add_pd(m0.sse.v2, m1.sse.v2);
		ret.sse.v3 = _mm_add_pd(m0.sse.v3, m1.sse.v3);
		ret.sse.v4 = _mm_add_pd(m0.sse.v4, m1.sse.v4);
		ret.sse.v5 = _mm_add_pd(m0.sse.v5, m1.sse.v5);
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
	#endif
	return ret;
}
UNIGINE_INLINE dmat4 operator+(const dmat4 &m0, const dmat4 &m1)
{
	dmat4 ret;
	return add(ret, m0, m1);
}
UNIGINE_INLINE dmat4 &dmat4::operator+=(const dmat4 &m)
{
	return add(*this, *this, m);
}

UNIGINE_INLINE dmat4 &sub(dmat4 &ret, const dmat4 &m0, const dmat4 &m1)
{
	#ifdef USE_SSE
		ret.sse.v0 = _mm_sub_pd(m0.sse.v0, m1.sse.v0);
		ret.sse.v1 = _mm_sub_pd(m0.sse.v1, m1.sse.v1);
		ret.sse.v2 = _mm_sub_pd(m0.sse.v2, m1.sse.v2);
		ret.sse.v3 = _mm_sub_pd(m0.sse.v3, m1.sse.v3);
		ret.sse.v4 = _mm_sub_pd(m0.sse.v4, m1.sse.v4);
		ret.sse.v5 = _mm_sub_pd(m0.sse.v5, m1.sse.v5);
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
	#endif
	return ret;
}
UNIGINE_INLINE dmat4 &dmat4::operator-=(const dmat4 &m)
{
	return sub(*this, *this, m);
}
UNIGINE_INLINE dmat4 operator-(const dmat4 &m0, const dmat4 &m1)
{
	dmat4 ret;
	return sub(ret, m0, m1);
}

UNIGINE_INLINE dmat4 &orthonormalize(dmat4 &ret, const dmat4 &m)
{
	dvec3 x = dvec3(m.m00, m.m10, m.m20);
	dvec3 y = dvec3(m.m01, m.m11, m.m21);
	dvec3 z = cross(x, y);
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
	return ret;
}
UNIGINE_INLINE dmat4 orthonormalize(const dmat4 &m)
{
	dmat4 ret;
	return orthonormalize(ret, m);
}

UNIGINE_INLINE dmat4 &inverse(dmat4 &ret, const dmat4 &m)
{
	double idet = Math::rcp(determinant(m));
	ret.m00 = (m.m11 * m.m22 - m.m12 * m.m21) * idet;
	ret.m10 = -(m.m10 * m.m22 - m.m12 * m.m20) * idet;
	ret.m20 = (m.m10 * m.m21 - m.m11 * m.m20) * idet;
	ret.m01 = -(m.m01 * m.m22 - m.m02 * m.m21) * idet;
	ret.m11 = (m.m00 * m.m22 - m.m02 * m.m20) * idet;
	ret.m21 = -(m.m00 * m.m21 - m.m01 * m.m20) * idet;
	ret.m02 = (m.m01 * m.m12 - m.m02 * m.m11) * idet;
	ret.m12 = -(m.m00 * m.m12 - m.m02 * m.m10) * idet;
	ret.m22 = (m.m00 * m.m11 - m.m01 * m.m10) * idet;
	ret.m03 = -(ret.m00 * m.m03 + ret.m01 * m.m13 + ret.m02 * m.m23);
	ret.m13 = -(ret.m10 * m.m03 + ret.m11 * m.m13 + ret.m12 * m.m23);
	ret.m23 = -(ret.m20 * m.m03 + ret.m21 * m.m13 + ret.m22 * m.m23);
	return ret;
}
UNIGINE_INLINE dmat4 inverse(const dmat4 &m)
{
	dmat4 ret;
	return inverse(ret, m);
}

UNIGINE_INLINE dmat4 &inverse4(dmat4 &ret, const dmat4 &m)
{
	return inverse(ret, m);
}
UNIGINE_INLINE dmat4 inverse4(const dmat4 &m)
{
	dmat4 ret;
	return inverse4(ret, m);
}

UNIGINE_INLINE dmat4 &rotation(dmat4 &ret, const dmat4 &m)
{
	ret.m00 = m.m00;
	ret.m01 = m.m01;
	ret.m02 = m.m02;
	ret.m03 = 0.0;
	ret.m10 = m.m10;
	ret.m11 = m.m11;
	ret.m12 = m.m12;
	ret.m13 = 0.0;
	ret.m20 = m.m20;
	ret.m21 = m.m21;
	ret.m22 = m.m22;
	ret.m23 = 0.0;
	return ret;
}
UNIGINE_INLINE dmat4 rotation(const dmat4 &m)
{
	dmat4 ret;
	return rotation(ret, m);
}

UNIGINE_INLINE dmat4 reflect(const dvec4 &plane)
{
	dmat4 ret;
	double x = plane.x;
	double y = plane.y;
	double z = plane.z;
	double x2 = x * 2.0;
	double y2 = y * 2.0;
	double z2 = z * 2.0;
	ret.m00 = 1.0 - x * x2;
	ret.m01 = -y * x2;
	ret.m02 = -z * x2;
	ret.m03 = -plane.w * x2;
	ret.m10 = -x * y2;
	ret.m11 = 1.0 - y * y2;
	ret.m12 = -z * y2;
	ret.m13 = -plane.w * y2;
	ret.m20 = -x * z2;
	ret.m21 = -y * z2;
	ret.m22 = 1.0 - z * z2;
	ret.m23 = -plane.w * z2;
	return ret;
}

UNIGINE_INLINE void calculate_basis(const dvec3 &position, const dvec3 &target, const vec3 &up, int axis, vec3 &x, vec3 &y, vec3 &z)
{
	switch (axis)
	{
		case AXIS_X:
			x = normalize(vec3(target - position));
			y = normalize(cross(up, x));
			z = normalize(cross(x, y));
			break;
		case AXIS_Y:
			y = normalize(vec3(target - position));
			x = normalize(cross(y, up));
			z = normalize(cross(x, y));
			break;
		case AXIS_Z:
			z = normalize(vec3(target - position));
			x = normalize(cross(up, z));
			y = normalize(cross(z, x));
			break;
		case AXIS_NX:
			x = normalize(vec3(position - target));
			y = normalize(cross(up, x));
			z = normalize(cross(x, y));
			break;
		case AXIS_NY:
			y = normalize(vec3(position - target));
			x = normalize(cross(y, up));
			z = normalize(cross(x, y));
			break;
		case AXIS_NZ:
			z = normalize(vec3(position - target));
			x = normalize(cross(up, z));
			y = normalize(cross(z, x));
			break;
	}
}
UNIGINE_INLINE dmat4 setTo(const dvec3 &position, const dvec3 &target, const vec3 &up, int axis = AXIS_NZ)
{
	dmat4 ret;
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
	return ret;
}
UNIGINE_INLINE dmat4 lookAt(const dvec3 &position, const dvec3 &target, const vec3 &up, int axis = AXIS_NZ)
{
	dmat4 ret, m0, m1;
	vec3 x, y, z;
	calculate_basis(position, target, up, axis, x, y, z);
	m0.m00 = x.x;
	m0.m01 = x.y;
	m0.m02 = x.z;
	m0.m03 = 0.0;
	m0.m10 = y.x;
	m0.m11 = y.y;
	m0.m12 = y.z;
	m0.m13 = 0.0;
	m0.m20 = z.x;
	m0.m21 = z.y;
	m0.m22 = z.z;
	m0.m23 = 0.0;
	m1.setTranslate(-position);
	return mul(ret, m0, m1);
}

UNIGINE_INLINE dmat4 &lerp(dmat4 &ret, const dmat4 &m0, const dmat4 &m1, double k);
UNIGINE_INLINE dmat4 lerp(const dmat4 &m0, const dmat4 &m1, double k)
{
	dmat4 ret;
	return lerp(ret, m0, m1, k);
}

UNIGINE_INLINE void decomposeTransform(const dmat4 &m, dvec3 &position, quat &rot, vec3 &scale);
UNIGINE_INLINE dmat4 &composeTransform(dmat4 &ret, const dvec3 &position, const quat &rot, const vec3 &scale);

}
}

#ifdef __GNUC__
	#pragma GCC diagnostic pop
#endif
