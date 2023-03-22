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

namespace Unigine
{
namespace Math
{

struct SHBasis
{
public:
	UNIGINE_INLINE SHBasis() {}
	UNIGINE_INLINE ~SHBasis() {}

	UNIGINE_INLINE double factorial(int x) const
	{
		static const double table[16] = {1.0, 1.0, 2.0, 6.0, 24.0, 120.0, 720.0, 5040.0,
			40320.0, 362880.0, 3628800.0, 39916800.0, 479001600.0, 6227020800.0, 87178291200.0, 1307674368000.0};
		if (x < 16)
			return table[x];
		double ret = table[15];
		for (int i = 16; i <= x; i++)
			ret *= (double)i;
		return ret;
	}

	UNIGINE_INLINE double get(int l, int m, const float *dir) const
	{
		double length = dsqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
		return get(l, m, atan2(-dir[1], -dir[0]), acos(dir[2] / length));
	}
	UNIGINE_INLINE double get(int l, int m, double phi, double theta) const
	{
		const double sqrt2 = dsqrt(2.0);
		if (m > 0)
			return sqrt2 * K(l, m) * cos(phi * m) * P(l, m, cos(theta));
		if (m < 0)
			return sqrt2 * K(l, -m) * sin(-phi * m) * P(l, -m, cos(theta));
		return K(l, 0) * P(l, m, cos(theta));
	}

private:
	UNIGINE_INLINE double K(int l, int m) const
	{
		return dsqrt((2.0 * l + 1.0) * factorial(l - m) / (4.0 * 3.14159265358979323846 * factorial(l + m)));
	}
	double P(int l, int m, double x) const
	{
		double pmm = 1.0;
		if (m > 0)
		{
			double fact = 1.0;
			double somx2 = Math::dsqrt((1.0 - x) * (1.0 + x));
			for (int i = 0; i < m; i++)
			{
				pmm *= -fact * somx2;
				fact += 2.0;
			}
		}
		if (l == m)
			return pmm;
		double pmmp1 = x * (2.0 * m + 1.0) * pmm;
		if (l == m + 1)
			return pmmp1;
		double pll = 0.0;
		for (int i = m + 2; i <= l; i++)
		{
			pll = ((2.0 * i - 1.0) * x * pmmp1 - (i + m - 1.0) * pmm) / (i - m);
			pmm = pmmp1;
			pmmp1 = pll;
		}
		return pll;
	}
};

}
} // namespace Unigine
