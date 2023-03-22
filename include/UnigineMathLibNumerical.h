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

#include "UnigineBase.h"
#include "UnigineMathLibCommon.h"
#include "UnigineMathLibGeometry.h"
#include "UnigineMathLibSimd.h"

namespace Unigine
{
namespace Math
{
namespace Numerical
{

template <int Size>
class vecX;
template <int Rows, int Columns>
class matX;

//////////////////////////////////////////////////////////////////////////
// vecX
//////////////////////////////////////////////////////////////////////////

template <int Size>
class alignas(16) vecX
{
public:
	UNIGINE_INLINE vecX() {}
	UNIGINE_INLINE explicit vecX(float v) { set(v); }
	UNIGINE_INLINE explicit vecX(const float *v) { set(v); }
	UNIGINE_INLINE vecX(const vecX<Size> &v) { set(v.get()); }
	UNIGINE_INLINE ~vecX() {}

	UNIGINE_INLINE vecX<Size> &operator=(const vecX<Size> &v)
	{
		if (this == &v)
			return *this;
		set(v.get());
		return *this;
	}

	UNIGINE_INLINE vecX<Size> operator-() const
	{
		vecX<Size> ret;
		for (int i = 0; i < Size; i++)
			ret.data[i] = -data[i];
		return ret;
	}
	UNIGINE_INLINE vecX<Size> &operator*=(float v) { return mul(*this, *this, v); }
	UNIGINE_INLINE vecX<Size> &operator*=(const vecX<Size> &v) { return mul(*this, this, v); }
	UNIGINE_INLINE vecX<Size> &operator+=(const vecX<Size> &v) { return add(*this, *this, v); }
	UNIGINE_INLINE vecX<Size> &operator-=(const vecX<Size> &v) { return sub(*this, *this, v); }

	UNIGINE_INLINE float &operator[](int index)
	{
		assert(index < Size && Size >= 0 && "vecX::operator[](): bad index");
		return data[index];
	}
	UNIGINE_INLINE float operator[](int index) const
	{
		assert(index < Size && Size >= 0 && "vecX::operator[](): bad index");
		return data[index];
	}

	UNIGINE_INLINE void set(float v)
	{
		for (int i = 0; i < Size; i++)
			data[i] = v;
	}
	UNIGINE_INLINE void set(const float *v) { memcpy(data, v, sizeof(float) * Size); }
	UNIGINE_INLINE void get(float *v) const { memcpy(v, data, sizeof(float) * Size); }

	UNIGINE_INLINE float *get() { return data; }
	UNIGINE_INLINE const float *get() const { return data; }
	UNIGINE_INLINE int getSize() const { return Size; }

private:
	float data[Size];
};

template <int Size>
UNIGINE_INLINE bool operator==(const vecX<Size> &v0, const vecX<Size> &v1)
{
	return compare(v0, v1);
}

template <int Size>
UNIGINE_INLINE bool operator!=(const vecX<Size> &v0, const vecX<Size> &v1)
{
	return !compare(v0, v1);
}

template <int Size>
UNIGINE_INLINE vecX<Size> operator*(const vecX<Size> &v0, float v1)
{
	vecX<Size> ret;
	return mul(ret, v0, v1);
}

template <int Size>
UNIGINE_INLINE vecX<Size> operator*(const vecX<Size> &v0, const vecX<Size> &v1)
{
	vecX<Size> ret;
	return mul(ret, v0, v1);
}

template <int Size>
UNIGINE_INLINE vecX<Size> operator+(const vecX<Size> &v0, const vecX<Size> &v1)
{
	vecX<Size> ret;
	return add(ret, v0, v1);
}

template <int Size>
UNIGINE_INLINE vecX<Size> operator-(const vecX<Size> &v0, const vecX<Size> &v1)
{
	vecX<Size> ret;
	return sub(ret, v0, v1);
}

template <int Size>
UNIGINE_INLINE bool compare(const vecX<Size> &v0, const vecX<Size> &v1)
{
	for (int i = 0; i < Size; i++)
	{
		if (compare(v0[i], v1[i]) == 0)
			return false;
	}
	return true;
}

template <int Size>
UNIGINE_INLINE bool compare(const vecX<Size> &v0, const vecX<Size> &v1, float epsilon)
{
	for (int i = 0; i < Size; i++)
	{
		if (compare(v0[i], v1[i], epsilon) == 0)
			return false;
	}
	return true;
}

template <int Size>
UNIGINE_INLINE float dot(const vecX<Size> &v0, const vecX<Size> &v1)
{
	float ret = 0.0;
	for (int i = 0; i < Size; i++)
		ret += v0[i] * v1[i];
	return ret;
}

template <int Size>
UNIGINE_INLINE vecX<Size> &mul(vecX<Size> &ret, const vecX<Size> &v0, float v1)
{
	Simd::mul(ret.get(), v0.get(), v1, Size);
	return ret;
}

template <int Size>
UNIGINE_INLINE vecX<Size> &mul(vecX<Size> &ret, const vecX<Size> &v0, const vecX<Size> &v1)
{
	Simd::mul(ret.get(), v0.get(), v1.get(), Size);
	return ret;
}

template <int Size>
UNIGINE_INLINE vecX<Size> &mad(vecX<Size> &ret, const vecX<Size> &v0, float v1, const vecX<Size> &v2)
{
	Simd::mad(ret.get(), v0.get(), v1, v2.get(), Size);
	return ret;
}

template <int Size>
UNIGINE_INLINE vecX<Size> &add(vecX<Size> &ret, const vecX<Size> &v0, const vecX<Size> &v1)
{
	Simd::add(ret.get(), v0.get(), v1.get(), Size);
	return ret;
}

template <int Size>
UNIGINE_INLINE vecX<Size> &sub(vecX<Size> &ret, const vecX<Size> &v0, const vecX<Size> &v1)
{
	Simd::sub(ret.get(), v0.get(), v1.get(), Size);
	return ret;
}

template <int Size>
UNIGINE_INLINE vecX<Size> &clamp(vecX<Size> &ret, const vecX<Size> &v, float v0, float v1)
{
	for (int i = 0; i < Size; i++)
		ret[i] = clamp(v[i], v0, v1);
	return ret;
}

template <int Size>
UNIGINE_INLINE vecX<Size> &clamp(vecX<Size> &ret, const vecX<Size> &v, const vecX<Size> &v0, const vecX<Size> &v1)
{
	for (int i = 0; i < Size; i++)
		ret[i] = clamp(v[i], v0[i], v1[i]);
	return ret;
}

template <int Size>
UNIGINE_INLINE vecX<Size> &lerp(vecX<Size> &ret, const vecX<Size> &v0, const vecX<Size> &v1, float k)
{
	for (int i = 0; i < Size; i++)
		ret[i] = lerp(v0[i], v1[i], k);
	return ret;
}

template <int Size>
UNIGINE_INLINE vecX<Size> clamp(const vecX<Size> &v, float v0, float v1)
{
	vecX<Size> ret;
	return clamp(ret, v, v0, v1);
}

template <int Size>
UNIGINE_INLINE vecX<Size> clamp(const vecX<Size> &v, const vecX<Size> &v0, const vecX<Size> &v1)
{
	vecX<Size> ret;
	return clamp(ret, v, v0, v1);
}

template <int Size>
UNIGINE_INLINE vecX<Size> lerp(const vecX<Size> &v0, const vecX<Size> &v1, float k)
{
	vecX<Size> ret;
	return lerp(ret, v0, v1, k);
}

//////////////////////////////////////////////////////////////////////////
// Bisection solver
//////////////////////////////////////////////////////////////////////////

namespace
{
UNIGINE_INLINE float bi_polynom(const float *c, int size, float x)
{
	float ret = 0.0f;
	if (size)
	{
		ret = c[0];
		float value = x;
		for (int i = 1; i < size; i++)
		{
			ret += c[i] * value;
			value *= x;
		}
	}
	return ret;
}
UNIGINE_INLINE int bi_bisection(float &x, const float *c, int size, float x0, float x1, float iepsilon)
{
	float y0 = bi_polynom(c, size, x0);
	if (Math::abs(y0) < Consts::EPS)
	{
		x = x0;
		return 1;
	}
	float y1 = bi_polynom(c, size, x1);
	if (Math::abs(y1) < Consts::EPS)
	{
		x = x1;
		return 1;
	}
	if (y0 * y1 > 0.0f)
		return 0;
	int num_iterations = Math::ftoi(Math::log((x1 - x0) * iepsilon) / Consts::LOG2 + 0.5f);
	for (int i = 0; i < num_iterations; i++)
	{
		x = (x0 + x1) * 0.5f;
		float y = bi_polynom(c, size, x);
		float v = y0 * y;
		if (v < 0.0f)
		{
			x1 = x;
			y1 = y;
		} else if (v > 0.0f)
		{
			x0 = x;
			y0 = y;
		} else
			break;
	}
	return 1;
}
UNIGINE_INLINE int bi_solve(float *ret, const float *c, int size, float x0, float x1, float iepsilon)
{
	float *r = ret;
	float roots[32];
	float derivative[32];
	if (size == 1)
	{
		if (bi_bisection(*r, c, size, x0, x1, iepsilon))
			r++;
	} else
	{
		for (int i = 1; i < size; i++)
			derivative[i - 1] = c[i] * Math::itof(i);
		int num = bi_solve(roots, derivative, size - 1, x0, x1, iepsilon);
		if (num > 0)
		{
			if (bi_bisection(*r, c, size, x0, roots[0], iepsilon))
				r++;
			for (int i = 0; i < num - 1; i++)
			{
				if (bi_bisection(*r, c, size, roots[i], roots[i + 1], iepsilon))
					r++;
			}
			if (bi_bisection(*r, c, size, roots[num - 1], x1, iepsilon))
				r++;
		} else
		{
			if (bi_bisection(*r, c, size, x0, x1, iepsilon))
				r++;
		}
	}
	return (int)(r - ret);
}
}
UNIGINE_INLINE int biSolve(float *ret, const float *c, int size, float iepsilon)
{
	float range = 0.0f;
	int degree = size - 1;
	for (int i = degree; i >= 0; i--)
	{
		if (Math::abs(c[i]) > Consts::EPS)
			break;
	}
	float idegree = 1.0f / c[degree];
	for (int i = 0; i < degree; i++)
	{
		float r = Math::abs(c[i]) * idegree;
		if (range < r)
			range = r;
	}
	range += 1.0f;
	return bi_solve(ret, c, size, -range, range, iepsilon);
}

template <int Size>
UNIGINE_INLINE int biSolve(vecX<Size> &ret, const vecX<Size> &c, float iepsilon = 1e6f)
{
	return biSolve(ret.get(), c.get(), Size, iepsilon);
}

//////////////////////////////////////////////////////////////////////////
// matX
//////////////////////////////////////////////////////////////////////////

template <int Rows, int Columns>
class alignas(16) matX
{
public:
	UNIGINE_INLINE matX() {}
	UNIGINE_INLINE explicit matX(float v) { set(v); }
	UNIGINE_INLINE explicit matX(const float *m) { set(m); }
	UNIGINE_INLINE matX(const matX<Rows, Columns> &m) { set(m.get()); }
	UNIGINE_INLINE ~matX() {}

	UNIGINE_INLINE matX<Rows, Columns> &operator=(const matX<Rows, Columns> &m)
	{
		if (this == &m)
			return *this;
		set(m.get());
		return *this;
	}

	UNIGINE_INLINE matX<Rows, Columns> operator-() const
	{
		matX<Rows, Columns> ret;
		for (int i = 0; i < Size; i++)
			ret.data[i] = -data[i];
		return ret;
	}
	UNIGINE_INLINE matX<Rows, Columns> &operator*=(float v) { return mul(*this, *this, v); }
	UNIGINE_INLINE matX<Rows, Columns> &operator*=(const matX<Rows, Columns> &m) { return mul(*this, matX<Rows, Columns>(*this), m); }
	UNIGINE_INLINE matX<Rows, Columns> &operator+=(const matX<Rows, Columns> &m) { return add(*this, *this, m); }
	UNIGINE_INLINE matX<Rows, Columns> &operator-=(const matX<Rows, Columns> &m) { return sub(*this, *this, m); }

	UNIGINE_INLINE float &operator[](int index)
	{
		assert(index < Size && index >= 0 && "matX::operator[](): bad index");
		return data[index];
	}
	UNIGINE_INLINE float operator[](int index) const
	{
		assert(index < Size && index >= 0 && "matX::operator[](): bad index");
		return data[index];
	}

	UNIGINE_INLINE void set(int row, int column, float v)
	{
		assert(row < Rows && row >= 0 && "matX::set(): bad row");
		assert(column < Columns && column >= 0 && "matX::set(): bad column");
		data[Rows * column + row] = v;
	}
	UNIGINE_INLINE float &get(int row, int column)
	{
		assert(row < Rows && row >= 0 && "matX::get(): bad row");
		assert(column < Columns && column >= 0 && "matX::get(): bad column");
		return data[Rows * column + row];
	}
	UNIGINE_INLINE float get(int row, int column) const
	{
		assert(row < Rows && row >= 0 && "matX::get(): bad row");
		assert(column < Columns && column >= 0 && "matX::get(): bad column");
		return data[Rows * column + row];
	}

	UNIGINE_INLINE void set(float v)
	{
		for (int i = 0; i < Size; i++)
			data[i] = v;
	}
	UNIGINE_INLINE void set(const float *m) { memcpy(data, m, sizeof(float) * Size); }
	UNIGINE_INLINE void get(float *m) const { memcpy(m, data, sizeof(float) * Size); }

	UNIGINE_INLINE float *get() { return data; }
	UNIGINE_INLINE const float *get() const { return data; }
	UNIGINE_INLINE int getSize() const { return Size; }
	UNIGINE_INLINE int getRows() const { return Rows; }
	UNIGINE_INLINE int getColumns() const { return Columns; }
	UNIGINE_INLINE void setRow(int row, const vecX<Columns> &v)
	{
		assert(row < Rows && row >= 0 && "matX::setRow(): bad row");
		float *d = data + row;
		for (int i = 0; i < Columns; i++)
		{
			*d = v[i];
			d += Rows;
		}
	}
	UNIGINE_INLINE vecX<Columns> &getRow(vecX<Columns> &ret, int row) const
	{
		assert(row < Rows && row >= 0 && "matX::getRow(): bad row");
		const float *s = data + row;
		for (int i = 0; i < Columns; i++)
		{
			ret[i] = *s;
			s += Rows;
		}
		return ret;
	}
	UNIGINE_INLINE vecX<Columns> getRow(int row) const
	{
		vecX<Columns> ret;
		return getRow(ret, row);
	}

	UNIGINE_INLINE void setColumn(int column, const vecX<Rows> &v)
	{
		assert(column < Columns && column >= 0 && "matX::setColumn(): bad column");
		memcpy(data + Rows * column, v.get(), sizeof(float) * Rows);
	}
	UNIGINE_INLINE vecX<Rows> &getColumn(vecX<Rows> &ret, int column) const
	{
		assert(column < Columns && column >= 0 && "matX::getColumn(): bad column");
		memcpy(ret.get(), data + Rows * column, sizeof(float) * Rows);
		return ret;
	}
	UNIGINE_INLINE vecX<Rows> getColumn(int column) const
	{
		vecX<Rows> ret;
		return getColumn(ret, column);
	}

	UNIGINE_INLINE void swapRows(int row_0, int row_1)
	{
		assert(row_0 < Rows && row_0 >= 0 && "matX::swapRows(): bad row");
		assert(row_1 < Rows && row_1 >= 0 && "matX::swapRows(): bad row");
		float *d0 = data + row_0;
		float *d1 = data + row_1;
		for (int i = 0; i < Columns; i++)
		{
			float d = *d0;
			*d0 = *d1;
			*d1 = d;
			d0 += Rows;
			d1 += Rows;
		}
	}
	UNIGINE_INLINE void swapColumns(int column_0, int column_1)
	{
		assert(column_0 < Columns && column_0 >= 0 && "matX::swapColumns(): bad column");
		assert(column_1 < Columns && column_1 >= 0 && "matX::swapColumns(): bad column");
		float *d0 = data + Rows * column_0;
		float *d1 = data + Rows * column_1;
		for (int i = 0; i < Rows; i++)
		{
			float d = *d0;
			*d0 = *d1;
			*d1 = d;
			d0++;
			d1++;
		}
	}

	UNIGINE_INLINE void setBlock(int row, int column, const mat3 &m)
	{
		assert(row >= 0 && row + 2 < Rows && "matX::setBlock(): bad row");
		assert(column >= 0 && column + 2 < Columns && "matX::setBlock(): bad column");
		float *d = data + Rows * column + row;
		const float *s = m.get();
		for (int i = 0; i < 3; i++)
		{
			*d++ = *s++;
			*d++ = *s++;
			*d++ = *s++;
			d += Rows - 3;
			s++;
		}
	}
	UNIGINE_INLINE mat3 &getBlock(mat3 &ret, int row, int column) const
	{
		assert(row >= 0 && row + 2 < Rows && "matX::getBlock(): bad row");
		assert(column >= 0 && column + 2 < Columns && "matX::getBlock(): bad column");
		float *d = ret.get();
		const float *s = data + Rows * column + row;
		for (int i = 0; i < 3; i++)
		{
			*d++ = *s++;
			*d++ = *s++;
			*d++ = *s++;
			s += Rows - 3;
			d++;
		}
		return ret;
	}

	UNIGINE_INLINE void setBlock(int row, int column, const mat3 &m, int rows, int columns)
	{
		assert(row >= 0 && row + rows - 1 < Rows && "matX::setBlock(): bad row");
		assert(column >= 0 && column + columns - 1 < Columns && "matX::setBlock(): bad column");
		assert(rows > 0 && rows < 4 && "matX::setBlock(): bad rows");
		assert(columns > 0 && columns < 4 && "matX::setBlock(): bad columns");
		float *d = data + Rows * column + row;
		const float *s = m.get();
		for (int i = 0; i < columns; i++)
		{
			for (int j = 0; j < rows; j++)
				*d++ = *s++;
			d += Rows - rows;
			s += 4 - rows;
		}
	}
	UNIGINE_INLINE mat3 &getBlock(mat3 &ret, int row, int column, int rows, int columns) const
	{
		assert(row >= 0 && row + rows - 1 < Rows && "matX::getBlock(): bad row");
		assert(column >= 0 && column + columns - 1 < Columns && "matX::getBlock(): bad column");
		assert(rows > 0 && rows < 4 && "matX::getBlock(): bad rows");
		assert(columns > 0 && columns < 4 && "matX::getBlock(): bad columns");
		float *d = ret.get();
		const float *s = data + Rows * column + row;
		for (int i = 0; i < columns; i++)
		{
			for (int j = 0; j < rows; j++)
				*d++ = *s++;
			d += 4 - rows;
			s += Rows - rows;
		}
		return ret;
	}

	UNIGINE_INLINE void setZero() { memset(data, 0, sizeof(float) * Size); }
	UNIGINE_INLINE void setIdentity()
	{
		assert(Rows == Columns && "matX::setIdentity(): bad matrix size");
		memset(data, 0, sizeof(float) * Size);
		float *d = data;
		for (int i = 0; i < Rows; i++)
		{
			*d = 1.0f;
			d += Rows + 1;
		}
	}

private:
	enum
	{
		Size = Rows * Columns,
	};

	float data[Size];
};

template <int Rows, int Columns>
UNIGINE_INLINE bool operator==(const matX<Rows, Columns> &m0, const matX<Rows, Columns> &m1) { return compare(m0, m1); }

template <int Rows, int Columns>
UNIGINE_INLINE bool operator!=(const matX<Rows, Columns> &m0, const matX<Rows, Columns> &m1) { return !compare(m0, m1); }

template <int Rows, int Columns>
UNIGINE_INLINE matX<Rows, Columns> operator*(const matX<Rows, Columns> &m, float v)
{
	matX<Rows, Columns> ret;
	return mul(ret, m, v);
}

template <int Rows, int Columns>
UNIGINE_INLINE vecX<Rows> operator*(const matX<Rows, Columns> &m, const vecX<Columns> &v)
{
	vecX<Rows> ret;
	return mul(ret, m, v);
}

template <int Rows, int Columns>
UNIGINE_INLINE vecX<Columns> operator*(const vecX<Rows> &v, const matX<Rows, Columns> &m)
{
	vecX<Columns> ret;
	return mul(ret, v, m);
}

template <int Rows, int Columns>
UNIGINE_INLINE matX<Rows, Columns> operator*(const matX<Rows, Columns> &m0, const matX<Rows, Columns> &m1)
{
	matX<Rows, Columns> ret;
	return mul(ret, m0, m1);
}

template <int Rows, int Columns>
UNIGINE_INLINE matX<Rows, Columns> operator+(const matX<Rows, Columns> &m0, const matX<Rows, Columns> &m1)
{
	matX<Rows, Columns> ret;
	return add(ret, m0, m1);
}

template <int Rows, int Columns>
UNIGINE_INLINE matX<Rows, Columns> operator-(const matX<Rows, Columns> &m0, const matX<Rows, Columns> &m1)
{
	matX<Rows, Columns> ret;
	return sub(ret, m0, m1);
}

template <int Rows, int Columns>
UNIGINE_INLINE bool compare(const matX<Rows, Columns> &m0, const matX<Rows, Columns> &m1)
{
	for (int i = 0; i < m0.getSize(); i++)
	{
		if (compare(m0[i], m1[i]) == 0)
			return false;
	}
	return true;
}

template <int Rows, int Columns>
UNIGINE_INLINE bool compare(const matX<Rows, Columns> &m0, const matX<Rows, Columns> &m1, float epsilon)
{
	for (int i = 0; i < m0.getSize(); i++)
	{
		if (compare(m0[i], m1[i], epsilon) == 0)
			return false;
	}
	return true;
}

template <int Size>
UNIGINE_INLINE float trace(const matX<Size, Size> &m)
{
	float trace = m[0];
	for (int i = 1; i < Size; i++)
		trace += m[Size * i + i];
	return trace;
}

template <int Size>
UNIGINE_INLINE float determinant(const matX<Size, Size> &m)
{
	int index[Size];
	float det = 0.0f;
	matX<Size, Size> temp = m;
	int ret = luDecompose(temp, index);
	if (ret)
		det = luDeterminant(temp, ret);
	return det;
}

template <int Rows, int Columns>
UNIGINE_INLINE matX<Rows, Columns> &mul(matX<Rows, Columns> &ret, const matX<Rows, Columns> &m, float v)
{
	Simd::mul(ret.get(), m.get(), v, ret.getSize());
	return ret;
}

template <int Rows, int Columns>
UNIGINE_INLINE vecX<Rows> &mul(vecX<Rows> &ret, const matX<Rows, Columns> &m, const vecX<Columns> &v)
{
	float *d = ret.get();
	const float *s = m.get();
	Simd::mul(d, s, v[0], Rows);
	for (int i = 1; i < Columns; i++)
	{
		s += Rows;
		Simd::mad(d, s, v[i], d, Rows);
	}
	return ret;
}

template <int Rows, int Columns>
UNIGINE_INLINE vecX<Columns> &mul(vecX<Columns> &ret, const vecX<Rows> &v, const matX<Rows, Columns> &m)
{
	const float *s = m.get();
	for (int i = 0; i < Columns; i++)
	{
		Simd::dot(ret[i], s, v.get(), Rows);
		s += Rows;
	}
	return ret;
}

template <int Rows, int Columns, int Size>
UNIGINE_INLINE matX<Rows, Columns> &mul(matX<Rows, Columns> &ret, const matX<Rows, Size> &m0, const matX<Size, Columns> &m1)
{
	float *d = ret.get();
	const float *s1 = m1.get();
	for (int j = 0; j < Columns; j++)
	{
		for (int i = 0; i < Rows; i++)
		{
			const float *s0 = m0.get() + i;
			float sum = *s0 * *s1;
			for (int k = 1; k < Size; k++)
			{
				s0 += Rows;
				sum += *s0 * s1[k];
			}
			*d++ = sum;
		}
		s1 += Size;
	}
	return ret;
}

template <int Rows, int Columns>
UNIGINE_INLINE matX<Rows, Columns> &mad(matX<Rows, Columns> &ret, const matX<Rows, Columns> &m0, float v, const matX<Rows, Columns> &m1)
{
	Simd::mad(ret.get(), m0.get(), v, m1.get(), ret.getSize());
	return ret;
}

template <int Rows, int Columns>
UNIGINE_INLINE matX<Rows, Columns> &add(matX<Rows, Columns> &ret, const matX<Rows, Columns> &m0, const matX<Rows, Columns> &m1)
{
	Simd::add(ret.get(), m0.get(), m1.get(), ret.getSize());
	return ret;
}

template <int Rows, int Columns>
UNIGINE_INLINE matX<Rows, Columns> &sub(matX<Rows, Columns> &ret, const matX<Rows, Columns> &m0, const matX<Rows, Columns> &m1)
{
	Simd::sub(ret.get(), m0.get(), m1.get(), ret.getSize());
	return ret;
}

template <int Rows, int Columns>
UNIGINE_INLINE matX<Rows, Columns> &transpose(matX<Rows, Columns> &ret, const matX<Columns, Rows> &m)
{
	float *d = ret.get();
	for (int j = 0; j < Columns; j++)
	{
		const float *s = m.get() + j;
		*d++ = *s;
		for (int i = 1; i < Rows; i++)
		{
			s += Columns;
			*d++ = *s;
		}
	}
	return ret;
}

template <int Size>
UNIGINE_INLINE matX<Size, Size> &inverse(matX<Size, Size> &ret, const matX<Size, Size> &m)
{
	int index[Size];
	matX<Size, Size> temp = m;
	if (luDecompose(temp, index))
		luInverse(ret, temp, index);
	else
		ret.set(0.0f);
	return ret;
}

template <int Rows, int Columns>
UNIGINE_INLINE matX<Rows, Columns> transpose(const matX<Columns, Rows> &m)
{
	matX<Rows, Columns> ret;
	return transpose(ret, m);
}

template <int Size>
UNIGINE_INLINE matX<Size, Size> inverse(const matX<Size, Size> &m)
{
	matX<Size, Size> ret;
	return inverse(ret, m);
}

//////////////////////////////////////////////////////////////////////////
// LU solver
//////////////////////////////////////////////////////////////////////////

namespace
{
UNIGINE_INLINE void lu_eliminate(float *ret, const float *column, const float *factor, int rows, int num)
{
	if (num > 3)
	{
		float *d0 = ret;
		float *d1 = d0 + rows;
		float *d2 = d1 + rows;
		float *d3 = d2 + rows;

		float f0 = factor[0];
		float f1 = factor[rows];
		float f2 = factor[rows + rows];
		float f3 = factor[rows + rows + rows];

		for (size_t i = num; i > 0; i--)
		{
			float c = *column++;
			*d0++ -= c * f0;
			*d1++ -= c * f1;
			*d2++ -= c * f2;
			*d3++ -= c * f3;
		}
	} else if (num == 3)
	{
		float *d0 = ret;
		float *d1 = d0 + rows;
		float *d2 = d1 + rows;

		float f0 = factor[0];
		float f1 = factor[rows];
		float f2 = factor[rows + rows];

		float c0 = *column++;
		float c1 = *column++;
		float c2 = *column++;

		*d0++ -= c0 * f0;
		*d1++ -= c0 * f1;
		*d2++ -= c0 * f2;

		*d0++ -= c1 * f0;
		*d1++ -= c1 * f1;
		*d2++ -= c1 * f2;

		*d0++ -= c2 * f0;
		*d1++ -= c2 * f1;
		*d2++ -= c2 * f2;
	} else if (num == 2)
	{
		float *d0 = ret;
		float *d1 = d0 + rows;

		float f0 = factor[0];
		float f1 = factor[rows];

		float c0 = *column++;
		float c1 = *column++;

		*d0++ -= c0 * f0;
		*d1++ -= c0 * f1;

		*d0++ -= c1 * f0;
		*d1++ -= c1 * f1;
	} else if (num == 1)
		*ret -= *column * *factor;
}
}

UNIGINE_INLINE int luDecompose(float *m, int size, int *index)
{
	int ret = 1;
	for (int i = 0; i < size; i++)
		index[i] = i;
	float *diagonal = m;
	for (int i = 1; i < size; i++)
	{
		float *d0 = diagonal + 1;
		float *d1 = diagonal + size;
		for (int j = i; j < size; j++)
		{
			float f = *d0;
			*d0++ = *d1;
			*d1 = f;
			d1 += size;
		}
		diagonal += size + 1;
	}
	diagonal = m;
	for (int i = 0; i < size; i++)
	{
		int num = i;
		float max = Math::abs(*diagonal);
		const float *s = diagonal + size;
		for (int j = i + 1; j < size; j++)
		{
			float f = Math::abs(*s);
			s += size;
			if (max < f)
			{
				max = f;
				num = j;
			}
		}
		if (Math::abs(max) < Consts::EPS)
			return 0;
		if (i != num)
		{
			ret = -ret;
			int j = index[i];
			index[i] = index[num];
			index[num] = j;
			float *d0 = m + size * i;
			float *d1 = m + size * num;
			for (int j = 0; j < size; j++)
			{
				float d = *d0;
				*d0 = *d1;
				*d1 = d;
				d0++;
				d1++;
			}
		}
		float f = 1.0f / *diagonal;
		float *d = diagonal + size;
		for (int j = i + 1; j < size; j++)
		{
			*d *= f;
			d += size;
		}
		d = diagonal + size;
		int to = size - ((size - i - 1) & 3);
		for (int j = i + 1; j < to; j += 4)
		{
			lu_eliminate(d + 1, diagonal + 1, d, size, size - i - 1);
			d += size << 2;
		}
		for (int j = to; j < size; j++)
		{
			float f = *d++;
			Simd::mad(d, diagonal + 1, -f, d, size - i - 1);
			d += size - 1;
		}
		diagonal += size + 1;
	}
	return ret;
}
UNIGINE_INLINE float luDeterminant(const float *m, int size, int ret)
{
	const float *s = m;
	float det = Math::itof(ret);
	for (int i = 0; i < size; i++)
		det *= s[size * i + i];
	return det;
}
UNIGINE_INLINE void luSolve3(float *ret, const float *m, const float *b, const int *index)
{
	float *d = ret;
	const float *s = m;
	d[0] = b[index[0]];
	d[1] = b[index[1]] - s[3] * d[0];
	d[2] = b[index[2]] - s[6] * d[0] - s[7] * d[1];
	d[2] = (d[2]) / s[8];
	d[1] = (d[1] - s[5] * d[2]) / s[4];
	d[0] = (d[0] - s[1] * d[1] - s[2] * d[2]) / s[0];
}
UNIGINE_INLINE void luSolve4(float *ret, const float *m, const float *b, const int *index)
{
	float *d = ret;
	const float *s = m;
	d[0] = b[index[0]];
	d[1] = b[index[1]] - s[4] * d[0];
	d[2] = b[index[2]] - s[8] * d[0] - s[9] * d[1];
	d[3] = b[index[3]] - s[12] * d[0] - s[13] * d[1] - s[14] * d[2];
	d[3] = (d[3]) / s[15];
	d[2] = (d[2] - s[11] * d[3]) / s[10];
	d[1] = (d[1] - s[16] * d[2] - s[17] * d[3]) / s[5];
	d[0] = (d[0] - s[1] * d[1] - s[2] * d[2] - s[3] * d[3]) / s[0];
}
UNIGINE_INLINE void luSolve5(float *ret, const float *m, const float *b, const int *index)
{
	float *d = ret;
	const float *s = m;
	d[0] = b[index[0]];
	d[1] = b[index[1]] - s[5] * d[0];
	d[2] = b[index[2]] - s[10] * d[0] - s[11] * d[1];
	d[3] = b[index[3]] - s[15] * d[0] - s[16] * d[1] - s[17] * d[2];
	d[4] = b[index[4]] - s[20] * d[0] - s[21] * d[1] - s[22] * d[2] - s[23] * d[3];
	d[4] = (d[4]) / s[24];
	d[3] = (d[3] - s[19] * d[4]) / s[18];
	d[2] = (d[2] - s[13] * d[3] - s[14] * d[4]) / s[12];
	d[1] = (d[1] - s[7] * d[2] - s[8] * d[3] - s[9] * d[4]) / s[6];
	d[0] = (d[0] - s[1] * d[1] - s[2] * d[2] - s[3] * d[3] - s[4] * d[4]) / s[0];
}
UNIGINE_INLINE void luSolve6(float *ret, const float *m, const float *b, const int *index)
{
	float *d = ret;
	const float *s = m;
	d[0] = b[index[0]];
	d[1] = b[index[1]] - s[6] * d[0];
	d[2] = b[index[2]] - s[12] * d[0] - s[13] * d[1];
	d[3] = b[index[3]] - s[18] * d[0] - s[19] * d[1] - s[20] * d[2];
	d[4] = b[index[4]] - s[24] * d[0] - s[25] * d[1] - s[26] * d[2] - s[27] * d[3];
	d[5] = b[index[5]] - s[30] * d[0] - s[31] * d[1] - s[32] * d[2] - s[33] * d[3] - s[34] * d[4];
	d[5] = (d[5]) / s[35];
	d[4] = (d[4] - s[29] * d[5]) / s[28];
	d[3] = (d[3] - s[22] * d[4] - s[23] * d[5]) / s[21];
	d[2] = (d[2] - s[15] * d[3] - s[16] * d[4] - s[17] * d[5]) / s[14];
	d[1] = (d[1] - s[8] * d[2] - s[9] * d[3] - s[10] * d[4] - s[11] * d[5]) / s[7];
	d[0] = (d[0] - s[1] * d[1] - s[2] * d[2] - s[3] * d[3] - s[4] * d[4] - s[5] * d[5]) / s[0];
}
UNIGINE_INLINE void luSolve(float *ret, const float *m, const float *b, int size, const int *index)
{
	float sum;
	float *d = ret;
	const float *s = m;
	for (int i = 0; i < size; i++)
	{
		Simd::dot(sum, s, d, i);
		d[i] = b[index[i]] - sum;
		s += size;
	}
	s = m + size * size - 1;
	for (int i = size - 1; i >= 0; i--)
	{
		Simd::dot(sum, s + 1, d + i + 1, size - i - 1);
		d[i] = (d[i] - sum) / *s;
		s -= size + 1;
	}
}

template <int Size>
UNIGINE_INLINE int luDecompose(matX<Size, Size> &m, int *index)
{
	return luDecompose(m.get(), Size, index);
}

template <int Size>
UNIGINE_INLINE float luDeterminant(const matX<Size, Size> &m, int ret)
{
	return luDeterminant(m.get(), Size, ret);
}

UNIGINE_INLINE vecX<3> &luSolve(vecX<3> &ret, const matX<3, 3> &m, const vecX<3> &b, const int *index)
{
	luSolve3(ret.get(), m.get(), b.get(), index);
	return ret;
}
UNIGINE_INLINE vecX<4> &luSolve(vecX<4> &ret, const matX<4, 4> &m, const vecX<4> &b, const int *index)
{
	luSolve4(ret.get(), m.get(), b.get(), index);
	return ret;
}
UNIGINE_INLINE vecX<5> &luSolve(vecX<5> &ret, const matX<5, 5> &m, const vecX<5> &b, const int *index)
{
	luSolve5(ret.get(), m.get(), b.get(), index);
	return ret;
}
UNIGINE_INLINE vecX<6> &luSolve(vecX<6> &ret, const matX<6, 6> &m, const vecX<6> &b, const int *index)
{
	luSolve6(ret.get(), m.get(), b.get(), index);
	return ret;
}

template <int Size>
UNIGINE_INLINE vecX<Size> &luSolve(vecX<Size> &ret, const matX<Size, Size> &m, const vecX<Size> &b, const int *index)
{
	luSolve(ret.get(), m.get(), b.get(), Size, index);
	return ret;
}

template <int Size>
UNIGINE_INLINE matX<Size, Size> &luInverse(matX<Size, Size> &ret, const matX<Size, Size> &m, const int *index)
{
	vecX<Size> x;
	vecX<Size> b(0.0f);
	for (int i = 0; i < Size; i++)
	{
		b[i] = 1.0f;
		luSolve(x.get(), m.get(), b.get(), Size, index);
		ret.setColumn(i, x);
		b[i] = 0.0f;
	}
	return ret;
}

}
}
}