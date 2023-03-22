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
#include "UnigineMathLibGeometry.h"
#include "UnigineMathLibSimd.h"
#include "UnigineVector.h"

#ifdef USE_SSE
	#include <emmintrin.h>
	#include <xmmintrin.h>
	#include <smmintrin.h>
#endif

#if defined(USE_DOUBLE) || defined(UNIGINE_DOUBLE)
#else
	#define WorldBoundBox BoundBox
	#define WorldBoundSphere BoundSphere
	#define WorldBoundFrustum BoundFrustum
#endif

namespace Unigine
{

namespace Math
{

struct BoundSphere;
struct BoundBox;
struct BoundFrustum;

#if defined(USE_DOUBLE) || defined(UNIGINE_DOUBLE)
	struct WorldBoundSphere;
	struct WorldBoundBox;
	struct WorldBoundFrustum;
#endif

constexpr float BOUNDS_EPSILON = 1e-4f;

//////////////////////////////////////////////////////////////////////////
// BoundSphere
//////////////////////////////////////////////////////////////////////////
struct alignas(16) BoundSphere
{
	union
	{
		vec4 data; // bounding sphere center and radius
		const vec3 center;
		struct
		{
			float x;
			float y;
			float z;
			float radius;
		};
	};

	UNIGINE_INLINE constexpr BoundSphere(float x, float y, float z, float radius, ConstexprTag): x(x), y(y), z(z), radius(radius) {}

	UNIGINE_INLINE BoundSphere()
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		clear();
	}
	UNIGINE_INLINE BoundSphere(const vec3 &center, float radius)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(center, radius);
	}
	UNIGINE_INLINE BoundSphere(const vec3 *points, int num_points, bool optimal)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(points, num_points, optimal);
	}
	UNIGINE_INLINE BoundSphere(const BoundSphere &bs)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bs);
	}
	UNIGINE_INLINE BoundSphere(const BoundSphere &bs, const mat4 &transform)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bs, transform);
	}
	UNIGINE_INLINE explicit BoundSphere(const BoundBox &bb)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bb);
	}
	#if defined(USE_DOUBLE) || defined(UNIGINE_DOUBLE)
		UNIGINE_INLINE BoundSphere(const WorldBoundSphere &bs)
		{
			UNIGINE_ASSERT_ALIGNED16(this);
			set(bs);
		}
		UNIGINE_INLINE void set(const WorldBoundSphere &bs);
	#endif

	UNIGINE_INLINE BoundSphere &operator=(const BoundSphere &bs)
	{
		set(bs);
		return *this;
	}

	UNIGINE_INLINE void clear() { x = 0.0f; y = 0.0f; z = 0.0f; radius = -1.0f; }
	UNIGINE_INLINE void setCenter(const vec3 &center) { x = center.x, y = center.y, z = center.z; }

	UNIGINE_INLINE void set(const vec3 &center_, float radius_) { setCenter(center_);  radius = radius_; }
	UNIGINE_INLINE void set(float x_, float y_, float z_, float radius_) { x = x_, y = y_, z = z_;  radius = radius_; }
	UNIGINE_INLINE void set(const vec3 *points, int num_points, bool optimal)
	{
		clear();
		if (num_points > 2 && optimal)
		{
			radius = Consts::INF;
			for (int i = 0; i < num_points; i++)
			{
				for (int j = i + 1; j < num_points; j++)
				{
					float radius2 = -Consts::INF;
					vec3 point = (points[i] + points[j]) * 0.5f;
					for (int k = 0; k < num_points; k++)
					{
						float length2 = (points[k] - point).length2();
						if (radius2 < length2)
							radius2 = length2;
					}
					if (radius > radius2)
						set(point, radius2);
				}
			}
			radius = (radius > 0.0f) ? fsqrt(radius) : -1.0f;
		} else
		{
			expand(points, num_points);
		}
	}
	UNIGINE_INLINE void set(const BoundSphere &bs) { data = bs.data; }
	UNIGINE_INLINE void set(const BoundSphere &bs, const mat4 &transform)
	{
		data = bs.data;
		setTransform(transform);
	}
	UNIGINE_INLINE void set(const BoundBox &bb);

	// parameters
	UNIGINE_INLINE bool isValid() const { return radius > 0.0f; }

	// transformation
	UNIGINE_INLINE void setTransform(const mat4 &transform)
	{
		float r = radius;
		#ifdef USE_SSE
			__m128 col_0 = transform.sse.v0;
			__m128 col_1 = transform.sse.v1;
			__m128 col_2 = transform.sse.v2;
			__m128 res_0 = _mm_mul_ps(col_0, _MM_SWIZZLE(data.vec, X, X, X, W));
			__m128 res_1 = _mm_mul_ps(col_1, _MM_SWIZZLE(data.vec, Y, Y, Y, W));
			__m128 res_2 = _mm_mul_ps(col_2, _MM_SWIZZLE(data.vec, Z, Z, Z, W));
			data = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, transform.sse.v3));
			col_0 = _mm_mul_ps(col_0, col_0);
			col_1 = _mm_mul_ps(col_1, col_1);
			col_2 = _mm_mul_ps(col_2, col_2);
			col_0 = _mm_add_ps(col_0, _MM_SWIZZLE(col_0, Y, X, W, Z));
			col_1 = _mm_add_ps(col_1, _MM_SWIZZLE(col_1, Y, X, W, Z));
			col_2 = _mm_add_ps(col_2, _MM_SWIZZLE(col_2, Y, X, W, Z));
			col_0 = _mm_add_ss(col_0, _MM_SWIZZLE(col_0, Z, W, X, Y));
			col_1 = _mm_add_ss(col_1, _MM_SWIZZLE(col_1, Z, W, X, Y));
			col_2 = _mm_add_ss(col_2, _MM_SWIZZLE(col_2, Z, W, X, Y));
			col_0 = _mm_max_ss(_mm_max_ss(col_0, col_1), col_2);
			col_0 = _mm_mul_ss(col_0, _mm_rsqrt_ss(col_0));
			radius = r * _mm_cvtss_f32(col_0);
		#else
			data = vec4(transform * vec3(data));
			float x = transform.m00 * transform.m00 + transform.m10 * transform.m10 + transform.m20 * transform.m20;
			float y = transform.m01 * transform.m01 + transform.m11 * transform.m11 + transform.m21 * transform.m21;
			float z = transform.m02 * transform.m02 + transform.m12 * transform.m12 + transform.m22 * transform.m22;
			float scale = sqrtFast(max(max(x, y), z));
			radius = r * scale;
		#endif
	}
	UNIGINE_INLINE void setTransform(const dmat4 &transform) { setTransform(mat4(transform)); }

	// compare
	UNIGINE_INLINE int compare(const BoundSphere &bs) const { return Math::compare(data, bs.data); }
	UNIGINE_INLINE int operator==(const BoundSphere &bs) const { return compare(bs); }
	UNIGINE_INLINE int operator!=(const BoundSphere &bs) const { return !compare(bs); }

	// expand
	UNIGINE_INLINE void expand(const vec3 &point)
	{
		if (isValid())
		{
			vec3 direction = point - center;
			float length = direction.length();
			if (length > radius)
			{
				float delta = (length - radius) * 0.5f;
				data += vec4(direction * (delta / length), delta);
			}
		} else
		{
			set(point, BOUNDS_EPSILON);
		}
	}
	UNIGINE_INLINE void expand(const vec3 *points, int num_points)
	{
		if (isValid())
		{
			for (int i = 0; i < num_points; i++)
			{
				vec3 direction = points[i] - center;
				float length = direction.length();
				if (length > radius)
				{
					float delta = (length - radius) * 0.5f;
					data += vec4(direction * (delta / length), delta);
				}
			}
		} else
		{
			vec3 p_min, p_max;
			Simd::minMaxVec3(p_min, p_max, points, sizeof(vec3), num_points);

			radius = -Consts::INF;
			for (int i = 0; i < num_points; i++)
			{
				float length2 = (points[i] - center).length2();
				if (radius < length2)
					radius = length2;
			}
			radius = (radius > 0.0f) ? fsqrt(radius) : -1.0f;

			set((p_min + p_max) * 0.5f, radius);
		}
	}
	UNIGINE_INLINE void expand(const BoundSphere &bs)
	{
		if (bs.isValid())
		{
			if (isValid())
			{
				vec3 direction = bs.center - center;
				float length = direction.length();
				if (length > Consts::EPS)
				{
					if (length + radius < bs.radius)
					{
						data = bs.data;
					} else if (length + bs.radius > radius)
					{
						vec3 p0 = center - direction * (radius / length);
						vec3 p1 = bs.center + direction * (bs.radius / length);
						vec3 center = (p0 + p1) * 0.5f;
						set(center, (p1 - center).length());
					}
				} else
				{
					if (radius < bs.radius)
						radius = bs.radius;
				}
			} else
			{
				data = bs.data;
			}
		}
	}
	UNIGINE_INLINE void expand(const BoundBox &bb);

	// radius expand
	UNIGINE_INLINE void expandRadius(const vec3 &point)
	{
		if (isValid())
		{
			float r = length(center - point);
			if (radius < r)
				radius = r;
		} else
		{
			set(point, BOUNDS_EPSILON);
		}
	}
	UNIGINE_INLINE void expandRadius(const vec3 *points, int num_points)
	{
		if (isValid())
		{
			for (int i = 0; i < num_points; i++)
			{
				float r = length(center - points[i]);
				if (radius < r)
					radius = r;
			}
		} else
		{
			vec3 p_min, p_max;
			Simd::minMaxVec3(p_min, p_max, points, sizeof(vec3), num_points);
			vec3 center = (p_min + p_max) * 0.5f;
			radius = -Consts::INF;
			for (int i = 0; i < num_points; i++)
			{
				float length2 = (points[i] - center).length2();
				if (radius < length2)
					radius = length2;
			}
			radius = (radius > 0.0f) ? fsqrt(radius) : -1.0f;
			set(center, radius);
		}
	}
	UNIGINE_INLINE void expandRadius(const BoundSphere &bs)
	{
		if (!bs.isValid())
			return;

		if (isValid())
		{
			float r = length(bs.center - center) + bs.radius;
			if (radius < r)
				radius = r;
		} else
		{
			data = bs.data;
		}
	}
	UNIGINE_INLINE void expandRadius(const BoundBox &bb);

	// inside points
	UNIGINE_INLINE bool inside(const vec3 &point) const
	{
		return isValid() && insideValid(point);
	}
	UNIGINE_INLINE bool inside(const vec3 &point, float radius) const
	{
		return isValid() && insideValid(point, radius);
	}
	UNIGINE_INLINE bool inside(const vec3 &minimum, const vec3 &maximum) const
	{
		return isValid() && insideValid(minimum, maximum);
	}

	UNIGINE_INLINE bool insideValid(const vec3 &point) const
	{
		return length2(center - point) <= radius * radius;
	}
	UNIGINE_INLINE bool insideValid(const vec3 &point, float radius_) const
	{
		return length2(center - point) <= pow2(radius_ + radius);
	}
	UNIGINE_INLINE bool insideValid(const vec3 &minimum, const vec3 &maximum) const
	{
		#ifdef USE_SSE
			__m128 r = _MM_SWIZZLE(data.vec, W, W, W, W);
			__m128 direction = _mm_sub_ps(_mm_min_ps(_mm_max_ps(center.vec, minimum.vec), maximum.vec), center.vec);
			direction = _mm_sub_ps(_mm_dot33_ps(direction, direction), _mm_mul_ss(r, r));
			return ((_mm_movemask_ps(direction) & 0x01) != 0);
		#else
			vec3 direction;
			if (minimum.x > center.x)
				direction.x = minimum.x - center.x;
			else if (maximum.x < center.x)
				direction.x = maximum.x - center.x;
			else
				direction.x = 0.0f;
			if (minimum.y > center.y)
				direction.y = minimum.y - center.y;
			else if (maximum.y < center.y)
				direction.y = maximum.y - center.y;
			else
				direction.y = 0.0f;
			if (minimum.z > center.z)
				direction.z = minimum.z - center.z;
			else if (maximum.z < center.z)
				direction.z = maximum.z - center.z;
			else
				direction.z = 0.0f;
			return (direction.length2() < center.w * center.w);
		#endif
	}

	// inside bounds
	UNIGINE_INLINE bool inside(const BoundSphere &bs) const
	{
		return isValid() && bs.isValid() && insideValid(bs.center, bs.radius);
	}
	UNIGINE_INLINE bool inside(const BoundBox &bb) const;

	UNIGINE_INLINE bool insideValid(const BoundSphere &bs) const
	{
		return insideValid(bs.center, bs.radius);
	}
	UNIGINE_INLINE bool insideValid(const BoundBox &bb) const;

	UNIGINE_INLINE bool insideAll(const BoundSphere &bs) const
	{
		return isValid() && bs.isValid() && insideAllValid(bs);
	}
	UNIGINE_INLINE bool insideAll(const BoundBox &bb) const;

	UNIGINE_INLINE bool insideAllValid(const BoundSphere &bs) const
	{
		float r = radius - bs.radius;
		if (r > 0.0f)
			return length2(center - bs.center) <= r * r;
		return false;
	}
	UNIGINE_INLINE bool insideAllValid(const BoundBox &bb) const;

	// intersections
	UNIGINE_INLINE bool rayIntersection(const vec3 &point, const vec3 &direction) const
	{
		return isValid() && rayIntersectionValid(point, direction);
	}
	UNIGINE_INLINE bool getIntersection(const vec3 &p0, const vec3 &p1) const
	{
		return isValid() && getIntersectionValid(p0, p1);
	}

	UNIGINE_INLINE bool rayIntersectionValid(const vec3 &point, const vec3 &direction) const
	{
		float k = saturate(dot(direction, center - point) / length2(direction));
		return length2(center - point - direction * k) <= radius * radius;
	}
	UNIGINE_INLINE bool getIntersectionValid(const vec3 &p0, const vec3 &p1) const
	{
		return rayIntersectionValid(p0, p1 - p0);
	}

	// distance
	UNIGINE_INLINE float distance() const
	{
		if (isValid())
			return distanceValid();
		return Consts::INF;
	}
	UNIGINE_INLINE float distance(const vec3 &point) const
	{
		if (isValid())
			return distanceValid(point);
		return Consts::INF;
	}
	UNIGINE_INLINE float distanceValid() const
	{
		#ifdef USE_SSE
			float ret;
			_mm_store_ss(&ret, _mm_rcp_ss(_mm_rsqrt_ss(_mm_dp_ps(center.vec, center.vec, 0x7f))));
			return ret - radius;
		#else
			return sqrtFast(center.length2()) - radius;
		#endif
	}
	UNIGINE_INLINE float distanceValid(const vec3 &point) const
	{
		#ifdef USE_SSE
			float distance;
			__m128 direction = _mm_sub_ps(center.vec, point.vec);
			_mm_store_ss(&distance, _mm_rcp_ss(_mm_rsqrt_ss(_mm_dp_ps(direction, direction, 0x7f))));
			return distance - radius;
		#else
			return (center - camera).lengthFast() - radius;
		#endif
	}
	
	UNIGINE_INLINE bool isCameraVisible(const vec3 &camera, float min_distance, float max_distance) const
	{
		#ifdef USE_SSE
			float distance;
			__m128 direction = _mm_sub_ps(center.vec, camera.vec);
			_mm_store_ss(&distance, _mm_rcp_ss(_mm_rsqrt_ss(_mm_dp_ps(direction, direction, 0x7f))));
		#else
			float distance = (center - camera).lengthFast();
		#endif

		return	((distance - radius) <= max_distance) &&
				((distance + radius) >= min_distance);
	}
};

constexpr BoundSphere BoundSphere_inf(0.0f, 0.0f, 0.0f, Consts::INF, ConstexprTag{});

UNIGINE_INLINE BoundSphere operator*(const mat4 &m, const BoundSphere &bs)
{
	BoundSphere ret = bs;
	ret.setTransform(m);
	return ret;
}
UNIGINE_INLINE BoundSphere operator*(const dmat4 &m, const BoundSphere &bs)
{
	BoundSphere ret = bs;
	ret.setTransform(m);
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// BoundBox
//////////////////////////////////////////////////////////////////////////
struct alignas(16) BoundBox
{
	vec3 minimum{vec3_inf};
	vec3 maximum{-vec3_inf};

	UNIGINE_INLINE constexpr BoundBox(float x_min, float y_min, float z_min, float x_max, float y_max, float z_max, ConstexprTag)
		: minimum(x_min, y_min, z_min, ConstexprTag{})
		, maximum(x_max, y_max, z_max, ConstexprTag{}) {}

	UNIGINE_INLINE BoundBox()
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE BoundBox(const vec3 &minimum, const vec3 &maximum)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(minimum, maximum);
	}
	UNIGINE_INLINE BoundBox(const vec3 *points, int num_points)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(points, num_points);
	}
	UNIGINE_INLINE BoundBox(const BoundBox &bb)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bb);
	}
	UNIGINE_INLINE BoundBox(const BoundBox &bb, const mat4 &transform)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bb, transform);
	}
	UNIGINE_INLINE explicit BoundBox(const BoundSphere &bs)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bs);
	}
	#if defined(USE_DOUBLE) || defined(UNIGINE_DOUBLE)
		UNIGINE_INLINE BoundBox(const WorldBoundBox &bb)
		{
			UNIGINE_ASSERT_ALIGNED16(this);
			set(bb);
		}
		UNIGINE_INLINE void set(const WorldBoundBox &bb);
	#endif

	UNIGINE_INLINE BoundBox &operator=(const BoundBox &bb) { set(bb); return *this; }

	UNIGINE_INLINE void clear()
	{
		minimum = vec3_inf;
		maximum = -vec3_inf;
	}

	UNIGINE_INLINE void set(const vec3 &min_, const vec3 &max_)
	{
		minimum = min_;
		maximum = max_;
	}
	UNIGINE_INLINE void set(const vec3 *points, int num_points)
	{
		clear();
		expand(points, num_points);
	}
	UNIGINE_INLINE void set(const BoundSphere &bs)
	{
		clear();
		expand(bs);
	}
	UNIGINE_INLINE void set(const BoundBox &bb)
	{
		minimum = bb.minimum;
		maximum = bb.maximum;
	}
	UNIGINE_INLINE void set(const BoundBox &bb, const mat4 &transform)
	{
		minimum = bb.minimum;
		maximum = bb.maximum;
		setTransform(transform);
	}

	// parameters
	UNIGINE_INLINE bool isValid() const { return minimum.x <= maximum.x; }
	UNIGINE_INLINE vec3 getCenter() const { return (minimum + maximum) * 0.5f; }
	UNIGINE_INLINE vec3 getSize() const { return maximum - minimum; }
	UNIGINE_INLINE void getPoints(vec3 *points, int num_points) const
	{
		UNIGINE_UNUSED(num_points);
		assert(num_points == 8 && "BoundBox::getPoints(): bad points number");
		points[0].set(minimum);
		points[1].set(maximum.x, minimum.y, minimum.z);
		points[2].set(minimum.x, maximum.y, minimum.z);
		points[3].set(maximum.x, maximum.y, minimum.z);
		points[4].set(minimum.x, minimum.y, maximum.z);
		points[5].set(maximum.x, minimum.y, maximum.z);
		points[6].set(minimum.x, maximum.y, maximum.z);
		points[7].set(maximum);
	}
	UNIGINE_INLINE void getPlanes(vec4 *planes, int num_planes) const
	{
		UNIGINE_UNUSED(num_planes);
		assert(num_planes == 6 && "BoundBox::getPlanes(): bad planes number");
		planes[0].set(1.0f, 0.0f, 0.0f, -maximum.x);
		planes[1].set(-1.0f, 0.0f, 0.0f, minimum.x);
		planes[2].set(0.0f, 1.0f, 0.0f, -maximum.y);
		planes[3].set(0.0f, -1.0f, 0.0f, minimum.y);
		planes[4].set(0.0f, 0.0f, 1.0f, -maximum.z);
		planes[5].set(0.0f, 0.0f, -1.0f, minimum.z);
	}

	// transformation
	UNIGINE_INLINE void setTransform(const mat4 &transform)
	{
		#ifdef USE_SSE
			__m128 sign = _mm_set1_ps(IntFloat(0x7fffffff).f);
			__m128 center = _mm_mul_ps(_mm_add_ps(minimum.vec, maximum.vec), _mm_set1_ps(0.5f));
			__m128 axis = _mm_sub_ps(maximum.vec, center);
			__m128 col_0 = transform.sse.v0;
			__m128 col_1 = transform.sse.v1;
			__m128 col_2 = transform.sse.v2;
			__m128 res_0 = _mm_mul_ps(col_0, _MM_SWIZZLE(center, X, X, X, W));
			__m128 res_1 = _mm_mul_ps(col_1, _MM_SWIZZLE(center, Y, Y, Y, W));
			__m128 res_2 = _mm_mul_ps(col_2, _MM_SWIZZLE(center, Z, Z, Z, W));
			center = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, transform.sse.v3));
			col_0 = _mm_mul_ps(_mm_and_ps(col_0, sign), _MM_SWIZZLE(axis, X, X, X, W));
			col_1 = _mm_mul_ps(_mm_and_ps(col_1, sign), _MM_SWIZZLE(axis, Y, Y, Y, W));
			col_2 = _mm_mul_ps(_mm_and_ps(col_2, sign), _MM_SWIZZLE(axis, Z, Z, Z, W));
			axis = _mm_add_ps(_mm_add_ps(col_0, col_1), col_2);
			minimum = _mm_sub_ps(center, axis);
			maximum = _mm_add_ps(center, axis);
		#else
			vec3 center = (minimum + maximum) * 0.5f;
			vec3 axis = maximum - center;
			float x = abs(transform.m00) * axis.x + abs(transform.m01) * axis.y + abs(transform.m02) * axis.z;
			float y = abs(transform.m10) * axis.x + abs(transform.m11) * axis.y + abs(transform.m12) * axis.z;
			float z = abs(transform.m20) * axis.x + abs(transform.m21) * axis.y + abs(transform.m22) * axis.z;
			center = transform * center;
			minimum = center - vec3(x, y, z);
			maximum = center + vec3(x, y, z);
		#endif
	}
	UNIGINE_INLINE void setTransform(const dmat4 &transform)
	{
		setTransform(mat4(transform));
	}
	UNIGINE_INLINE void setTransform(const BoundSphere &bs, const mat4 &transform)
	{
		setTransform(transform);
		#ifdef USE_SSE
			__m128 col_0 = transform.sse.v0;
			__m128 col_1 = transform.sse.v1;
			__m128 col_2 = transform.sse.v2;
			__m128 res_0 = _mm_mul_ps(col_0, _MM_SWIZZLE(bs.center.vec, X, X, X, W));
			__m128 res_1 = _mm_mul_ps(col_1, _MM_SWIZZLE(bs.center.vec, Y, Y, Y, W));
			__m128 res_2 = _mm_mul_ps(col_2, _MM_SWIZZLE(bs.center.vec, Z, Z, Z, W));
			__m128 center = _mm_add_ps(_mm_add_ps(res_0, res_1), _mm_add_ps(res_2, transform.sse.v3));
			col_0 = _mm_mul_ps(col_0, col_0);
			col_1 = _mm_mul_ps(col_1, col_1);
			col_2 = _mm_mul_ps(col_2, col_2);
			col_0 = _mm_add_ps(col_0, _MM_SWIZZLE(col_0, Y, X, W, Z));
			col_1 = _mm_add_ps(col_1, _MM_SWIZZLE(col_1, Y, X, W, Z));
			col_2 = _mm_add_ps(col_2, _MM_SWIZZLE(col_2, Y, X, W, Z));
			col_0 = _mm_add_ps(col_0, _MM_SWIZZLE(col_0, Z, W, X, Y));
			col_1 = _mm_add_ps(col_1, _MM_SWIZZLE(col_1, Z, W, X, Y));
			col_2 = _mm_add_ps(col_2, _MM_SWIZZLE(col_2, Z, W, X, Y));
			col_0 = _mm_max_ps(_mm_max_ps(col_0, col_1), col_2);
			col_0 = _mm_mul_ps(col_0, _mm_rsqrt_ps(col_0));
			__m128 radius = _mm_mul_ps(col_0, vec3(bs.radius).vec);
			minimum = _mm_max_ps(minimum.vec, _mm_sub_ps(center, radius));
			maximum = _mm_min_ps(maximum.vec, _mm_add_ps(center, radius));
		#else
			vec3 center = transform * bs.getCenter();
			float x = transform.m00 * transform.m00 + transform.m10 * transform.m10 + transform.m20 * transform.m20;
			float y = transform.m01 * transform.m01 + transform.m11 * transform.m11 + transform.m21 * transform.m21;
			float z = transform.m02 * transform.m02 + transform.m12 * transform.m12 + transform.m22 * transform.m22;
			float radius = sqrtFast(maximum(maximum(x, y), z)) * bs.radius;
			minimum = maximum(minimum, center - vec3(radius));
			maximum = minimum(maximum, center + vec3(radius));
		#endif
	}
	UNIGINE_INLINE void setTransform(const BoundSphere &bs, const dmat4 &transform)
	{
		setTransform(bs, mat4(transform));
	}

	// compare
	UNIGINE_INLINE int compare(const BoundBox &bb) const
	{
		return Math::compare(minimum, bb.minimum) && Math::compare(maximum, bb.maximum);
	}
	UNIGINE_INLINE int operator==(const BoundBox &bb) const { return compare(bb); }
	UNIGINE_INLINE int operator!=(const BoundBox &bb) const { return !compare(bb); }

	// expand
	UNIGINE_INLINE void expand(const vec3 &point)
	{
		if (isValid())
		{
			#ifdef USE_SSE
				minimum = _mm_min_ps(minimum.vec, point.vec);
				maximum = _mm_max_ps(maximum.vec, point.vec);
			#else
				minimum = min(minimum, point);
				maximum = max(maximum, point);
			#endif
		} else
		{
			minimum = point - BOUNDS_EPSILON;
			maximum = point + BOUNDS_EPSILON;
		}
	}
	UNIGINE_INLINE void expand(const vec3 *points, int num_points)
	{
		if (isValid())
		{
			vec3 min_, max_;
			Simd::minMaxVec3(min_, max_, points, sizeof(vec3), num_points);
			#ifdef USE_SSE
				minimum = _mm_min_ps(minimum.vec, min_.vec);
				maximum = _mm_max_ps(maximum.vec, max_.vec);
			#else
				minimum = min(minimum, min_);
				maximum = max(maximum, max_);
			#endif
		} else
		{
			Simd::minMaxVec3(minimum, maximum, points, sizeof(vec3), num_points);
		}
	}
	UNIGINE_INLINE void expand(const BoundSphere &bs)
	{
		if (!bs.isValid())
			return;
		const vec3 &center = bs.center;
		vec3 r(bs.radius);
		if (isValid())
		{
			#ifdef USE_SSE
				minimum = _mm_min_ps(minimum.vec, _mm_sub_ps(center.vec, r.vec));
				maximum = _mm_max_ps(maximum.vec, _mm_add_ps(center.vec, r.vec));
			#else
				minimum = min(minimum, center - r);
				maximum = max(maximum, center + r);
			#endif
		} else
		{
			sub(minimum, center, r);
			add(maximum, center, r);
		}
	}
	UNIGINE_INLINE void expand(const BoundBox &bb)
	{
		if (!bb.isValid())
			return;
		if (isValid())
		{
			#ifdef USE_SSE
				minimum = _mm_min_ps(minimum.vec, bb.minimum.vec);
				maximum = _mm_max_ps(maximum.vec, bb.maximum.vec);
			#else
				minimum = minimum(minimum, bb.minimum);
				maximum = maximum(maximum, bb.maximum);
			#endif
		} else
		{
			minimum = bb.minimum;
			maximum = bb.maximum;
		}
	}

	// inside points
	UNIGINE_INLINE bool inside(const vec3 &point) const
	{
		return isValid() && insideValid(point);
	}
	UNIGINE_INLINE bool inside(const vec3 &point, float radius) const
	{
		return isValid() && insideValid(point, radius);
	}
	UNIGINE_INLINE bool inside(const vec3 &minimum, const vec3 &maximum) const
	{
		return isValid() && insideValid(minimum, maximum);
	}

	UNIGINE_INLINE bool insideValid(const vec3 &point) const
	{
		#ifdef USE_SSE
			__m128 res_0 = _mm_cmpgt_ps(minimum.vec, point.vec);
			__m128 res_1 = _mm_cmplt_ps(maximum.vec, point.vec);
			return ((_mm_movemask_ps(_mm_or_ps(res_0, res_1)) & 0x07) == 0);
		#else
			if (minimum.x > point.x || maximum.x < point.x) return false;
			if (minimum.y > point.y || maximum.y < point.y) return false;
			if (minimum.z > point.z || maximum.z < point.z) return false;
			return true;
		#endif
	}
	UNIGINE_INLINE bool insideValid(const vec3 &point, float radius) const
	{
		#ifdef USE_SSE
			__m128 r = vec3(radius).vec;
			__m128 direction = _mm_sub_ps(_mm_min_ps(_mm_max_ps(point.vec, minimum.vec), maximum.vec), point.vec);
			direction = _mm_sub_ps(_mm_dot33_ps(direction, direction), _mm_mul_ss(r, r));
			return ((_mm_movemask_ps(direction) & 0x01) != 0);
		#else
			vec3 direction;
			if (minimum.x > point.x)
				direction.x = minimum.x - point.x;
			else if (maximum.x < point.x)
				direction.x = maximum.x - point.x;
			else
				direction.x = 0.0f;
			if (minimum.y > point.y)
				direction.y = minimum.y - point.y;
			else if (maximum.y < point.y)
				direction.y = maximum.y - point.y;
			else
				direction.y = 0.0f;
			if (minimum.z > point.z)
				direction.z = minimum.z - point.z;
			else if (maximum.z < point.z)
				direction.z = maximum.z - point.z;
			else
				direction.z = 0.0f;
			return (direction.length2() < radius * radius);
		#endif
	}
	UNIGINE_INLINE bool insideValid(const vec3 &min_, const vec3 &max_) const
	{
		#ifdef USE_SSE
			__m128 res_0 = _mm_cmpgt_ps(minimum.vec, max_.vec);
			__m128 res_1 = _mm_cmplt_ps(maximum.vec, min_.vec);
			return ((_mm_movemask_ps(_mm_or_ps(res_0, res_1)) & 0x07) == 0);
		#else
			if (minimum.x > max_.x || maximum.x < min_.x) return false;
			if (minimum.y > max_.y || maximum.y < min_.y) return false;
			if (minimum.z > max_.z || maximum.z < min_.z) return false;

			return true;
		#endif
	}
	UNIGINE_INLINE bool insideValid(const vec3 *points, int num_points) const
	{
		for (int i = 0; i < num_points; i++)
		{
			if (insideValid(points[i]))
				return true;
		}
		return false;
	}

	// inside bounds
	UNIGINE_INLINE bool inside(const BoundSphere &bs) const
	{
		return isValid() && insideValid(bs.center, bs.radius);
	}
	UNIGINE_INLINE bool inside(const BoundBox &bb) const
	{
		return isValid() && bb.isValid() && insideValid(bb.minimum, bb.maximum);
	}

	UNIGINE_INLINE bool insideValid(const BoundSphere &bs) const
	{
		#ifdef USE_SSE
			__m128 radius = _MM_SWIZZLE(bs.center.vec, W, W, W, W);
			__m128 direction = _mm_sub_ps(_mm_min_ps(_mm_max_ps(bs.center.vec, minimum.vec), maximum.vec), bs.center.vec);
			direction = _mm_sub_ps(_mm_dot33_ps(direction, direction), _mm_mul_ss(radius, radius));
			return ((_mm_movemask_ps(direction) & 0x01) != 0);
		#else
			vec3 direction;
			float radius = bs.radius;
			if (minimum.x > center.x)
				direction.x = minimum.x - center.x;
			else if (maximum.x < center.x)
				direction.x = maximum.x - center.x;
			else
				direction.x = 0.0f;
			if (minimum.y > center.y)
				direction.y = minimum.y - center.y;
			else if (maximum.y < center.y)
				direction.y = maximum.y - center.y;
			else
				direction.y = 0.0f;
			if (minimum.z > center.z)
				direction.z = minimum.z - center.z;
			else if (maximum.z < center.z)
				direction.z = maximum.z - center.z;
			else
				direction.z = 0.0f;
			return (direction.length2() < radius * radius);
		#endif
	}
	UNIGINE_INLINE bool insideValid(const BoundBox &bb) const
	{
		return insideValid(bb.minimum, bb.maximum);
	}

	UNIGINE_INLINE bool insideAll(const BoundSphere &bs) const
	{
		return isValid() && bs.isValid() && insideAllValid(bs);
	}
	UNIGINE_INLINE bool insideAll(const BoundBox &bb) const
	{
		return isValid() && bb.isValid() && insideAllValid(bb);
	}

	UNIGINE_INLINE bool insideAllValid(const BoundSphere &bs) const
	{
		#ifdef USE_SSE
			__m128 radius = _MM_SWIZZLE(bs.center.vec, W, W, W, W);
			__m128 res_0 = _mm_cmpgt_ps(minimum.vec, _mm_sub_ps(bs.center.vec, radius));
			__m128 res_1 = _mm_cmplt_ps(maximum.vec, _mm_add_ps(bs.center.vec, radius));
			return ((_mm_movemask_ps(_mm_or_ps(res_0, res_1)) & 0x07) == 0);
		#else
			if (minimum.x > center.x - bs.radius || maximum.x < center.x + bs.radius) return false;
			if (minimum.y > center.y - bs.radius || maximum.y < center.y + bs.radius) return false;
			if (minimum.z > center.z - rbs.adius || maximum.z < center.z + bs.radius) return false;

			return true;
		#endif
	}
	UNIGINE_INLINE bool insideAllValid(const BoundBox &bb) const
	{
		#ifdef USE_SSE
			__m128 res_0 = _mm_cmpgt_ps(minimum.vec, bb.minimum.vec);
			__m128 res_1 = _mm_cmplt_ps(maximum.vec, bb.maximum.vec);
			return ((_mm_movemask_ps(_mm_or_ps(res_0, res_1)) & 0x07) == 0);
		#else
			if (minimum.x > bb.minimum.x || maximum.x < bb.maximum.x) return false;
			if (minimum.y > bb.minimum.y || maximum.y < bb.maximum.y) return false;
			if (minimum.z > bb.minimum.z || maximum.z < bb.maximum.z) return false;

			return true;
		#endif
	}
	UNIGINE_INLINE bool insideAllValid(const vec3 *points, int num_points) const
	{
		for (int i = 0; i < num_points; i++)
		{
			if (!insideValid(points[i]))
				return false;
		}
		return true;
	}

	// inside cube
	UNIGINE_INLINE bool insideCube(int face, const vec3 &offset) const
	{
		if (!isValid())
			return false;

		vec3 min_temp = minimum - offset;
		vec3 max_temp = maximum - offset;
		switch (face)
		{
			case 0: return (max_temp.x >= 0.0f && min_temp.y <=  max_temp.x && max_temp.y >= -max_temp.x && min_temp.z <=  max_temp.x && max_temp.z >= -max_temp.x);
			case 1: return (min_temp.x <= 0.0f && min_temp.y <= -min_temp.x && max_temp.y >=  min_temp.x && min_temp.z <= -min_temp.x && max_temp.z >=  min_temp.x);
			case 2: return (max_temp.y >= 0.0f && min_temp.x <=  max_temp.y && max_temp.x >= -max_temp.y && min_temp.z <=  max_temp.y && max_temp.z >= -max_temp.y);
			case 3: return (min_temp.y <= 0.0f && min_temp.x <= -min_temp.y && max_temp.x >=  min_temp.y && min_temp.z <= -min_temp.y && max_temp.z >=  min_temp.y);
			case 4: return (max_temp.z >= 0.0f && min_temp.x <=  max_temp.z && max_temp.x >= -max_temp.z && min_temp.y <=  max_temp.z && max_temp.y >= -max_temp.z);
			case 5: return (min_temp.z <= 0.0f && min_temp.x <= -min_temp.z && max_temp.x >=  min_temp.z && min_temp.y <= -min_temp.z && max_temp.y >=  min_temp.z);
		};
		assert(0 && "BoundBox::insideCube(): bad face number");
		return false;
	}

	// intersections
	UNIGINE_INLINE bool rayIntersection(const vec3 &point, const vec3 &direction) const
	{
		return isValid() && rayIntersectionValid(point, direction);
	}
	UNIGINE_INLINE bool irayIntersection(const vec3 &point, const vec3 &idirection) const
	{
		return isValid() && irayIntersectionValid(point, idirection);
	}
	UNIGINE_INLINE bool getIntersection(const vec3 &p0, const vec3 &p1) const
	{
		return isValid() && getIntersectionValid(p0, p1);
	}

	UNIGINE_INLINE bool rayIntersectionValid(const vec3 &point, const vec3 &direction) const
	{
		return rayBoundBoxIntersection(point, direction, minimum, maximum);
	}
	UNIGINE_INLINE bool irayIntersectionValid(const vec3 &point, const vec3 &idirection) const
	{
		return irayBoundBoxIntersection(point, idirection, minimum, maximum);
	}
	UNIGINE_INLINE bool getIntersectionValid(const vec3 &p0, const vec3 &p1) const
	{
		return rayBoundBoxIntersection(p0, p1 - p0, minimum, maximum);
	}

	// distance
	UNIGINE_INLINE float distance() const
	{
		if (isValid())
			return distanceValid();
		return Consts::INF;
	}
	UNIGINE_INLINE float distance(const vec3 &point) const
	{
		if (isValid())
			return distanceValid(point);
		return Consts::INF;
	}

	UNIGINE_INLINE float distanceValid() const
	{
		#ifdef USE_SSE
			float ret;
			__m128 direction = _mm_min_ps(_mm_max_ps(vec3_zero.vec, minimum.vec), maximum.vec);
			direction = _mm_rcp_ss(_mm_rsqrt_ss(_mm_dot33_ps(direction, direction)));
			_mm_store_ss(&ret, direction);
			return ret;
		#else
			vec3 direction;
			if (minimum.x > 0.0f)
				direction.x = minimum.x;
			else if (maximum.x < 0.0f)
				direction.x = maximum.x;
			else
				direction.x = 0.0f;
			if (minimum.y > 0.0f)
				direction.y = minimum.y;
			else if (maximum.y < 0.0f)
				direction.y = maximum.y;
			else
				direction.y = 0.0f;
			if (minimum.z > 0.0f)
				direction.z = minimum.z;
			else if (maximum.z < 0.0f)
				direction.z = maximum.z;
			else
				direction.z = 0.0f;
			return sqrtFast(direction.length2());
		#endif
	}
	UNIGINE_INLINE float distanceValid(const vec3 &point) const
	{
		#ifdef USE_SSE
			float ret;
			__m128 direction = _mm_sub_ps(_mm_min_ps(_mm_max_ps(point.vec, minimum.vec), maximum.vec), point.vec);
			direction = _mm_rcp_ss(_mm_rsqrt_ss(_mm_dot33_ps(direction, direction)));
			_mm_store_ss(&ret, direction);
			return ret;
		#else
			vec3 direction;
			if (minimum.x > point.x)
				direction.x = minimum.x - point.x;
			else if (maximum.x < point.x)
				direction.x = maximum.x - point.x;
			else
				direction.x = 0.0f;
			if (minimum.y > point.y)
				direction.y = minimum.y - point.y;
			else if (maximum.y < point.y)
				direction.y = maximum.y - point.y;
			else
				direction.y = 0.0f;
			if (minimum.z > point.z)
				direction.z = minimum.z - point.z;
			else if (maximum.z < point.z)
				direction.z = maximum.z - point.z;
			else
				direction.z = 0.0f;
			return sqrtFast(direction.length2());
		#endif
	}
};

UNIGINE_INLINE BoundBox operator*(const mat4 &m, const BoundBox &bb)
{
	BoundBox ret = bb;
	ret.setTransform(m);
	return ret;
}
UNIGINE_INLINE BoundBox operator*(const dmat4 &m, const BoundBox &bb)
{
	BoundBox ret = bb;
	ret.setTransform(m);
	return ret;
}

UNIGINE_INLINE void BoundSphere::set(const BoundBox &bb)
{
	clear();
	expand(bb);
}
UNIGINE_INLINE void BoundSphere::expand(const BoundBox &bb)
{
	if (!bb.isValid())
		return;

	if (isValid())
	{
		expand(bb.minimum);
		expand(bb.maximum);
		expand(vec3(bb.maximum.x, bb.minimum.y, bb.minimum.z));
		expand(vec3(bb.minimum.x, bb.maximum.y, bb.minimum.z));
		expand(vec3(bb.maximum.x, bb.maximum.y, bb.minimum.z));
		expand(vec3(bb.minimum.x, bb.minimum.y, bb.maximum.z));
		expand(vec3(bb.maximum.x, bb.minimum.y, bb.maximum.z));
		expand(vec3(bb.minimum.x, bb.maximum.y, bb.maximum.z));
	} else
	{
		vec3 center = (bb.minimum + bb.maximum) * 0.5f;
		set(center, length(bb.maximum - center));
	}
}
UNIGINE_INLINE void BoundSphere::expandRadius(const BoundBox &bb)
{
	if (!bb.isValid())
		return;

	if (isValid())
	{
		expandRadius(bb.minimum);
		expandRadius(bb.maximum);
		expandRadius(vec3(bb.maximum.x, bb.minimum.y, bb.minimum.z));
		expandRadius(vec3(bb.minimum.x, bb.maximum.y, bb.minimum.z));
		expandRadius(vec3(bb.maximum.x, bb.maximum.y, bb.minimum.z));
		expandRadius(vec3(bb.minimum.x, bb.minimum.y, bb.maximum.z));
		expandRadius(vec3(bb.maximum.x, bb.minimum.y, bb.maximum.z));
		expandRadius(vec3(bb.minimum.x, bb.maximum.y, bb.maximum.z));
	} else
	{
		vec3 center = (bb.minimum + bb.maximum) * 0.5f;
		set(center, length(bb.maximum - center));
	}
}
UNIGINE_INLINE bool BoundSphere::inside(const BoundBox &bb) const
{
	return isValid() && bb.isValid() && insideValid(bb.minimum, bb.maximum);
}
UNIGINE_INLINE bool BoundSphere::insideAll(const BoundBox &bb) const
{
	return isValid() && bb.isValid() && insideAllValid(bb);
}
UNIGINE_INLINE bool BoundSphere::insideValid(const BoundBox &bb) const
{
	return insideValid(bb.minimum, bb.maximum);
}
UNIGINE_INLINE bool BoundSphere::insideAllValid(const BoundBox &bb) const
{
	return insideValid(bb.minimum)
		&& insideValid(bb.maximum)
		&& insideValid(vec3(bb.maximum.x, bb.minimum.y, bb.minimum.z))
		&& insideValid(vec3(bb.minimum.x, bb.maximum.y, bb.minimum.z))
		&& insideValid(vec3(bb.maximum.x, bb.maximum.y, bb.minimum.z))
		&& insideValid(vec3(bb.minimum.x, bb.minimum.y, bb.maximum.z))
		&& insideValid(vec3(bb.maximum.x, bb.minimum.y, bb.maximum.z))
		&& insideValid(vec3(bb.minimum.x, bb.maximum.y, bb.maximum.z));
}

constexpr BoundBox BoundBox_inf(-Consts::INF, -Consts::INF, -Consts::INF, Consts::INF, Consts::INF, Consts::INF, ConstexprTag{});

//////////////////////////////////////////////////////////////////////////
// BoundFrustum
//////////////////////////////////////////////////////////////////////////
struct alignas(16) BoundFrustum
{
	vec3 camera;
	BoundBox bound_box;
	vec3 points[8];		// bounding points
	vec4 planes[6];		// aos clipping planes

	UNIGINE_INLINE BoundFrustum()
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE BoundFrustum(const mat4 &projection, const mat4 &modelview)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(projection, modelview);
	}
	UNIGINE_INLINE BoundFrustum(const BoundFrustum &bf)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bf);
	}
	UNIGINE_INLINE BoundFrustum(const BoundFrustum &bf, const mat4 &itransform)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bf, itransform);
	}
	#if defined(USE_DOUBLE) || defined(UNIGINE_DOUBLE)
		UNIGINE_INLINE BoundFrustum(const WorldBoundFrustum &bf)
		{
			UNIGINE_ASSERT_ALIGNED16(this);
			set(bf);
		}
		UNIGINE_INLINE void set(const WorldBoundFrustum &bf);
	#endif

	UNIGINE_INLINE BoundFrustum &operator=(const BoundFrustum &bf)
	{
		set(bf);
		return *this;
	}

	UNIGINE_INLINE void clear()
	{
		// camera
		camera = vec3_zero;
		bound_box.clear();

		// clipping planes
		for (int i = 0; i < 6; i++)
			planes[i] = vec4_zero;

		// clipping planes and points
		for (int i = 0; i < 8; i++)
			points[i] = vec3_zero;
	}

	UNIGINE_INLINE void set(const mat4 &projection, const mat4 &modelview)
	{
		set(projection * modelview);
	}
	UNIGINE_INLINE void set(const mat4 &proj)
	{
		mat4 iproj = inverse(proj);

		// camera
		camera.x = iproj.m03;
		camera.y = iproj.m13;
		camera.z = iproj.m23;
		camera /= iproj.m33;

		// points
		points[0].set(-1.0f, -1.0f, -1.0f);
		points[1].set(1.0f, -1.0f, -1.0f);
		points[2].set(-1.0f, 1.0f, -1.0f);
		points[3].set(1.0f, 1.0f, -1.0f);
		points[4].set(-1.0f, -1.0f, 1.0f);
		points[5].set(1.0f, -1.0f, 1.0f);
		points[6].set(-1.0f, 1.0f, 1.0f);
		points[7].set(1.0f, 1.0f, 1.0f);
		Simd::projMat4Vec3(points, sizeof(vec3), iproj, points, sizeof(vec3), 8);

		// clipping planes
		planes[0].set(proj.m30 + proj.m00, proj.m31 + proj.m01, proj.m32 + proj.m02, proj.m33 + proj.m03);
		planes[1].set(proj.m30 - proj.m00, proj.m31 - proj.m01, proj.m32 - proj.m02, proj.m33 - proj.m03);
		planes[2].set(proj.m30 + proj.m10, proj.m31 + proj.m11, proj.m32 + proj.m12, proj.m33 + proj.m13);
		planes[3].set(proj.m30 - proj.m10, proj.m31 - proj.m11, proj.m32 - proj.m12, proj.m33 - proj.m13);
		planes[4].set(proj.m30 + proj.m20, proj.m31 + proj.m21, proj.m32 + proj.m22, proj.m33 + proj.m23);
		planes[5].set(proj.m30 - proj.m20, proj.m31 - proj.m21, proj.m32 - proj.m22, proj.m33 - proj.m23);
		for (int i = 0; i < 6; i++)
			planes[i] /= length(planes[i].xyz);

		update_bounds();
	}
	UNIGINE_INLINE void set(const BoundFrustum &bf)
	{
		camera = bf.camera;
		bound_box = bf.bound_box;

		for (int i = 0; i < 6; i++)
			planes[i] = bf.planes[i];

		for (int i = 0; i < 8; i++)
			points[i] = bf.points[i];
	}
	UNIGINE_INLINE void set(const BoundFrustum &bf, const mat4 &itransform)
	{
		set(bf);
		setITransform(itransform);
	}

	// parameters
	UNIGINE_INLINE bool isValid() const { return bound_box.isValid(); }

	// transformation
	UNIGINE_INLINE void setITransform(const mat4 &itransform)
	{
		mat4 transform = inverse(itransform);
		camera = transform * camera;

		// points
		Simd::mulMat4Vec3(points, sizeof(vec3), transform, points, sizeof(vec3), 8);

		// clipping planes
		for (int i = 0; i < 6; i++)
		{
			planes[i] = planes[i] * itransform;
			planes[i] /= length(planes[i].xyz);
		}

		update_bounds();
	}
	UNIGINE_INLINE void setITransform(const dmat4 &itransform)
	{
		setITransform(mat4(itransform));
	}
	UNIGINE_INLINE void setTransform(const mat4 &transform)
	{
		camera = transform * camera;

		// points
		Simd::mulMat4Vec3(points, sizeof(vec3), transform, points, sizeof(vec3), 8);

		// clipping planes
		mat4 itransform = inverse(transform);
		for (int i = 0; i < 6; i++)
		{
			planes[i] = planes[i] * itransform;
			planes[i] /= length(planes[i].xyz);
		}

		update_bounds();
	}
	UNIGINE_INLINE void setTransform(const dmat4 &transform)
	{
		setTransform(mat4(transform));
	}

	// compare
	UNIGINE_INLINE int compare(const BoundFrustum &bf) const
	{
		if (camera != bf.camera) return false;
		if (bound_box != bf.bound_box) return false;

		// clipping planes
		if (planes[0] != bf.planes[0]) return false;
		if (planes[1] != bf.planes[1]) return false;
		if (planes[2] != bf.planes[2]) return false;
		if (planes[3] != bf.planes[3]) return false;
		if (planes[4] != bf.planes[4]) return false;
		if (planes[5] != bf.planes[5]) return false;

		return true;
	}
	UNIGINE_INLINE int operator==(const BoundFrustum &bf) const { return compare(bf); }
	UNIGINE_INLINE int operator!=(const BoundFrustum &bf) const { return !compare(bf); }

	// inside points
	UNIGINE_INLINE bool inside(const vec3 &point) const
	{
		return bound_box.insideValid(point)
			&& (dot(planes[0], point) > 0.0f)
			&& (dot(planes[1], point) > 0.0f)
			&& (dot(planes[2], point) > 0.0f)
			&& (dot(planes[3], point) > 0.0f)
			&& (dot(planes[4], point) > 0.0f)
			&& (dot(planes[5], point) > 0.0f);
	}
	UNIGINE_INLINE bool inside(const vec3 &point, float radius) const
	{
		return bound_box.insideValid(point, radius)
			&& (dot(planes[0], point) > -radius)
			&& (dot(planes[1], point) > -radius)
			&& (dot(planes[2], point) > -radius)
			&& (dot(planes[3], point) > -radius)
			&& (dot(planes[4], point) > -radius)
			&& (dot(planes[5], point) > -radius);
	}
	UNIGINE_INLINE bool inside(const vec3 &minimum, const vec3 &maximum) const
	{
		return bound_box.insideValid(minimum, maximum)
			&& inside_plane(planes[0], minimum, maximum)
			&& inside_plane(planes[1], minimum, maximum)
			&& inside_plane(planes[2], minimum, maximum)
			&& inside_plane(planes[3], minimum, maximum)
			&& inside_plane(planes[4], minimum, maximum)
			&& inside_plane(planes[5], minimum, maximum);
	}
	UNIGINE_INLINE bool inside(const vec3 *points, int num_points) const
	{
		return bound_box.insideValid(points, num_points)
			&& inside_plane(planes[0], points, num_points)
			&& inside_plane(planes[1], points, num_points)
			&& inside_plane(planes[2], points, num_points)
			&& inside_plane(planes[3], points, num_points)
			&& inside_plane(planes[4], points, num_points)
			&& inside_plane(planes[5], points, num_points);
	}

	UNIGINE_INLINE bool insideFast(const vec3 &point) const
	{
		return bound_box.inside(point)
			&& (dot(planes[0], point) > 0.0)
			&& (dot(planes[1], point) > 0.0)
			&& (dot(planes[2], point) > 0.0)
			&& (dot(planes[3], point) > 0.0);
	}
	UNIGINE_INLINE bool insideFast(const vec3 &point, float radius) const
	{
		return bound_box.inside(point, radius)
			&& (dot(planes[0], point) > -radius)
			&& (dot(planes[1], point) > -radius)
			&& (dot(planes[2], point) > -radius)
			&& (dot(planes[3], point) > -radius);
	}
	UNIGINE_INLINE bool insideFast(const vec3 &minimum, const vec3 &maximum) const
	{
		return bound_box.insideValid(minimum, maximum)
			&& inside_plane(planes[0], minimum, maximum)
			&& inside_plane(planes[1], minimum, maximum)
			&& inside_plane(planes[2], minimum, maximum)
			&& inside_plane(planes[3], minimum, maximum);
	}
	UNIGINE_INLINE bool insideFast(const vec3 *points, int num_points) const
	{
		return bound_box.insideValid(points, num_points)
			&& inside_plane(planes[0], points, num_points)
			&& inside_plane(planes[1], points, num_points)
			&& inside_plane(planes[2], points, num_points)
			&& inside_plane(planes[3], points, num_points);
	}

	// inside bounds
	UNIGINE_INLINE bool inside(const BoundSphere &bs) const { return bs.isValid() ? insideValid(bs) : false; }
	UNIGINE_INLINE bool inside(const BoundBox &bb) const { return bb.isValid() ? insideValid(bb) : false; }
	UNIGINE_INLINE bool inside(const BoundFrustum &bf) const { return bf.isValid() ? insideValid(bf) : false; }

	UNIGINE_INLINE bool insideValid(const BoundSphere &bs) const { return inside(bs.center, bs.radius); }
	UNIGINE_INLINE bool insideValid(const BoundBox &bb) const { return inside(bb.minimum, bb.maximum); }
	UNIGINE_INLINE bool insideValid(const BoundFrustum &bf) const
	{
		return bound_box.insideValid(bf.bound_box)
			&& inside_plane(planes[0], bf.points, 8)
			&& inside_plane(planes[1], bf.points, 8)
			&& inside_plane(planes[2], bf.points, 8)
			&& inside_plane(planes[3], bf.points, 8)
			&& inside_plane(planes[4], bf.points, 8)
			&& inside_plane(planes[5], bf.points, 8);
	}

	UNIGINE_INLINE bool insideValidFast(const BoundSphere &bs) const { return insideFast(bs.center, bs.radius); }
	UNIGINE_INLINE bool insideValidFast(const BoundBox &bb) const { return insideFast(bb.minimum, bb.maximum); }
	UNIGINE_INLINE bool insideValidFast(const BoundFrustum &bf) const
	{
		return bound_box.insideValid(bf.bound_box)
			&& inside_plane(planes[0], bf.points, 8)
			&& inside_plane(planes[1], bf.points, 8)
			&& inside_plane(planes[2], bf.points, 8)
			&& inside_plane(planes[3], bf.points, 8);
	}

	// inside all bounds
	UNIGINE_INLINE bool insideAll(const BoundSphere &bs) const { return bs.isValid() ? insideAllValid(bs) : false; }
	UNIGINE_INLINE bool insideAll(const BoundBox &bb) const { return bb.isValid() ? insideAllValid(bb) : false; }
	UNIGINE_INLINE bool insideAll(const BoundFrustum &bf) const { return bf.isValid() ? insideAllValid(bf) : false; }

	UNIGINE_INLINE bool insideAllValid(const BoundSphere &bs) const { return inside(bs.center, -bs.radius); }
	UNIGINE_INLINE bool insideAllValid(const BoundBox &bb) const
	{
		return bound_box.insideAllValid(bb)
			&& inside(bb.minimum)
			&& inside(bb.maximum)
			&& inside(vec3(bb.maximum.x, bb.minimum.y, bb.minimum.z))
			&& inside(vec3(bb.minimum.x, bb.maximum.y, bb.minimum.z))
			&& inside(vec3(bb.maximum.x, bb.maximum.y, bb.minimum.z))
			&& inside(vec3(bb.minimum.x, bb.minimum.y, bb.maximum.z))
			&& inside(vec3(bb.maximum.x, bb.minimum.y, bb.maximum.z))
			&& inside(vec3(bb.minimum.x, bb.maximum.y, bb.maximum.z));
	}
	UNIGINE_INLINE bool insideAllValid(const BoundFrustum &bf) const
	{
		return bound_box.insideValid(bf.bound_box)
			&& inside(bf.points[0])
			&& inside(bf.points[1])
			&& inside(bf.points[2])
			&& inside(bf.points[3])
			&& inside(bf.points[4])
			&& inside(bf.points[5])
			&& inside(bf.points[6])
			&& inside(bf.points[7]);
	}

	UNIGINE_INLINE bool insideAllValidFast(const BoundSphere &bs) const { return insideFast(bs.center, -bs.radius); }
	UNIGINE_INLINE bool insideAllValidFast(const BoundBox &bb) const
	{
		return bound_box.insideAllValid(bb)
			&& insideFast(bb.minimum)
			&& insideFast(bb.maximum)
			&& insideFast(vec3(bb.maximum.x, bb.minimum.y, bb.minimum.z))
			&& insideFast(vec3(bb.minimum.x, bb.maximum.y, bb.minimum.z))
			&& insideFast(vec3(bb.maximum.x, bb.maximum.y, bb.minimum.z))
			&& insideFast(vec3(bb.minimum.x, bb.minimum.y, bb.maximum.z))
			&& insideFast(vec3(bb.maximum.x, bb.minimum.y, bb.maximum.z))
			&& insideFast(vec3(bb.minimum.x, bb.maximum.y, bb.maximum.z));
	}
	UNIGINE_INLINE bool insideAllValidFast(const BoundFrustum &bf) const
	{
		return bound_box.insideValid(bf.bound_box)
			&& insideFast(bf.points[0])
			&& insideFast(bf.points[1])
			&& insideFast(bf.points[2])
			&& insideFast(bf.points[3])
			&& insideFast(bf.points[4])
			&& insideFast(bf.points[5])
			&& insideFast(bf.points[6])
			&& insideFast(bf.points[7]);
	}

	// inside planes bounds
	UNIGINE_INLINE bool insidePlanes(const BoundSphere &bs) const { return bs.isValid() ? insidePlanesValid(bs) : false; }
	UNIGINE_INLINE bool insidePlanes(const BoundBox &bb) const { return bb.isValid() ? insidePlanesValid(bb) : false; }
	UNIGINE_INLINE bool insidePlanes(const BoundFrustum &bf) const { return bf.isValid() ? insidePlanesValid(bf) : false; }

	UNIGINE_INLINE bool insidePlanesValid(const BoundSphere &bs) const { return inside(bs.center, bs.radius); }
	UNIGINE_INLINE bool insidePlanesValid(const BoundBox &bb) const { return inside(bb.minimum, bb.maximum); }
	UNIGINE_INLINE bool insidePlanesValid(const BoundFrustum &bf) const { return inside(bf.points, 8); }

	UNIGINE_INLINE bool insidePlanesValidFast(const BoundSphere &bs) const { return insideFast(bs.center, bs.radius); }
	UNIGINE_INLINE bool insidePlanesValidFast(const BoundBox &bb) const { return insideFast(bb.minimum, bb.maximum); }
	UNIGINE_INLINE bool insidePlanesValidFast(const BoundFrustum &bf) const { return insideFast(bf.points, 8); }

	// inside shadow
	UNIGINE_INLINE bool insideShadowValid(const BoundSphere &object, const vec3 &direction) const
	{
		// object is inside the bound frustum
		if (insideFast(object.center, object.radius))
			return true;

		// shadow volume is inside the bound frustum
		for (int i = 0; i < 4; i++)
		{
			float k = dot3(planes[i], direction);
			if (abs(k) < Consts::EPS)
				continue;
			k = -dot(planes[i], object.center) / k;
			if (k > object.radius)
				continue;
			if (insideFast(object.center + direction * k, object.radius))
				return true;
		}

		return false;
	}
	UNIGINE_INLINE bool insideShadowValid(const BoundSphere &object, const BoundSphere &light, const vec3 &offset) const
	{
		// object is outside the light bounds
		if (light.insideValid(object) == false)
			return false;

		// object is inside the bound frustum
		if (insideFast(object.center, object.radius))
			return true;

		// direction from light center to object center
		vec3 direction = object.center - offset;
		float distance = length(direction);
		if (distance < object.radius + Consts::EPS)
			return true;
		direction /= distance;

		// basis
		vec3 x, y;
		orthoBasis(direction, x, y);

		// near points
		vec3 x0 = x * object.radius;
		vec3 y0 = y * object.radius;
		vec3 z0 = offset + direction * (distance - object.radius);

		// far points
		float radius = light.radius + dot(direction, light.center - offset);
		float k = object.radius * radius / (distance - object.radius);
		vec3 x1 = x * k;
		vec3 y1 = y * k;
		vec3 z1 = offset + direction * radius;

		// check visibility
		vec3 points[8] = {
			x0 + y0 + z0, x0 - y0 + z0, -x0 - y0 + z0, -x0 + y0 + z0,
			x1 + y1 + z1, x1 - y1 + z1, -x1 - y1 + z1, -x1 + y1 + z1,
		};

		return insideFast(points, 8);
	}

private:
	UNIGINE_INLINE static bool inside_plane(const vec4 &plane, const vec3 &minimum, const vec3 &maximum)
	{
		#ifdef USE_SSE
			vec3 p = _mm_max_ps(_mm_mul_ps(minimum.vec, plane.vec), _mm_mul_ps(maximum.vec, plane.vec));
			return p.sum() > -plane.w;
		#else
			return max(minimum * plane.xyz, maximum * plane.xyz).sum() > -plane.w;
		#endif
	}
	UNIGINE_INLINE static bool inside_plane(const vec4 &plane, const vec3 *points, int num_points)
	{
		for (int i = 0; i < num_points; i++)
		{
			if (dot(plane, points[i]) > 0.0f)
				return true;
		}
		return false;
	}
	UNIGINE_INLINE void update_bounds()
	{
		bound_box.set(points, 8);
	}
};

UNIGINE_INLINE BoundFrustum operator*(const mat4 &m, const BoundFrustum &bb)
{
	BoundFrustum ret = bb;
	ret.setTransform(m);
	return ret;
}
UNIGINE_INLINE BoundFrustum operator*(const dmat4 &m, const BoundFrustum &bb)
{
	BoundFrustum ret = bb;
	ret.setTransform(m);
	return ret;
}

#if defined(USE_DOUBLE) || defined(UNIGINE_DOUBLE)

//////////////////////////////////////////////////////////////////////////
// WorldBoundSphere
//////////////////////////////////////////////////////////////////////////

struct alignas(16) WorldBoundSphere
{
	union
	{
		dvec4 data; // bounding sphere center and radius
		const dvec3 center;
		struct
		{
			double x;
			double y;
			double z;
			double radius;
		};
	};

	UNIGINE_INLINE constexpr WorldBoundSphere(double x, double y, double z, double radius, ConstexprTag): x(x), y(y), z(z), radius(radius) {}

	UNIGINE_INLINE WorldBoundSphere()
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		clear();
	}
	UNIGINE_INLINE WorldBoundSphere(const dvec3 &center, double radius)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(center, radius);
	}
	UNIGINE_INLINE WorldBoundSphere(const dvec3 *points, int num_points, bool optimal)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(points, num_points, optimal);
	}
	UNIGINE_INLINE WorldBoundSphere(const WorldBoundSphere &bs)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bs);
	}
	UNIGINE_INLINE WorldBoundSphere(const WorldBoundSphere &bs, const dmat4 &transform)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bs, transform);
	}
	UNIGINE_INLINE WorldBoundSphere(const BoundSphere &bs)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bs);
	}
	UNIGINE_INLINE WorldBoundSphere(const BoundSphere &bs, const dmat4 &transform)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bs, transform);
	}
	UNIGINE_INLINE explicit WorldBoundSphere(const WorldBoundBox &bb)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bb);
	}

	UNIGINE_INLINE WorldBoundSphere &operator=(const WorldBoundSphere &bs)
	{
		set(bs);
		return *this;
	}

	UNIGINE_INLINE void clear() { x = 0.0; y = 0.0; z = 0.0; radius = -1.0; }
	UNIGINE_INLINE void setCenter(const dvec3 &center) { x = center.x, y = center.y, z = center.z; }

	UNIGINE_INLINE void set(const dvec3 &center_, double radius_) { setCenter(center_);  radius = radius_; }
	UNIGINE_INLINE void set(double x_, double y_, double z_, double radius_) { x = x_, y = y_, z = z_;  radius = radius_; }
	UNIGINE_INLINE void set(const dvec3 *points, int num_points, bool optimal)
	{
		clear();
		if (num_points > 2 && optimal)
		{
			radius = Consts::INF;
			for (int i = 0; i < num_points; i++)
			{
				for (int j = i + 1; j < num_points; j++)
				{
					double radius2 = -Consts::INF;
					dvec3 point = (points[i] + points[j]) * 0.5f;
					for (int k = 0; k < num_points; k++)
					{
						double length2 = (points[k] - point).length2();
						if (radius2 < length2)
							radius2 = length2;
					}
					if (radius > radius2)
						set(point, radius2);
				}
			}
			radius = (radius > 0.0) ? sqrt(radius) : -1.0;
		} else
		{
			expand(points, num_points);
		}
	}
	UNIGINE_INLINE void set(const WorldBoundSphere &bs) { data = bs.data; }
	UNIGINE_INLINE void set(const WorldBoundSphere &bs, const dmat4 &transform)
	{
		data = bs.data;
		setTransform(transform);
	}
	UNIGINE_INLINE void set(const BoundSphere &bs) { data = dvec4(bs.data); }
	UNIGINE_INLINE void set(const BoundSphere &bs, const dmat4 &transform)
	{
		set(bs);
		setTransform(transform);
	}
	UNIGINE_INLINE void set(const WorldBoundBox &bb);

	// parameters
	UNIGINE_INLINE bool isValid() const { return radius > 0.0f; }

	// transformation
	UNIGINE_INLINE void setTransform(const dmat4 &transform)
	{
		double r = radius;
		data = dvec4(transform * dvec3(data));
		double x = transform.m00 * transform.m00 + transform.m10 * transform.m10 + transform.m20 * transform.m20;
		double y = transform.m01 * transform.m01 + transform.m11 * transform.m11 + transform.m21 * transform.m21;
		double z = transform.m02 * transform.m02 + transform.m12 * transform.m12 + transform.m22 * transform.m22;
		double scale = sqrt(max(max(x, y), z));
		radius = r * scale;
	}

	// equals
	UNIGINE_INLINE int compare(const WorldBoundSphere &bs) const { return Math::compare(data, bs.data); }
	UNIGINE_INLINE int operator==(const WorldBoundSphere &bs) const { return compare(bs); }
	UNIGINE_INLINE int operator!=(const WorldBoundSphere &bs) const { return !compare(bs); }

	// expand
	UNIGINE_INLINE void expand(const dvec3 &point)
	{
		if (isValid())
		{
			dvec3 direction = point - center;
			double length = direction.length();
			if (length > radius)
			{
				double delta = (length - radius) * 0.5f;
				data += dvec4(direction * (delta / length), delta);
			}
		} else
		{
			set(point, BOUNDS_EPSILON);
		}
	}
	UNIGINE_INLINE void expand(const dvec3 *points, int num_points)
	{
		if (isValid())
		{
			for (int i = 0; i < num_points; i++)
			{
				dvec3 direction = points[i] - center;
				double length = direction.length();
				if (length > radius)
				{
					double delta = (length - radius) * 0.5f;
					data += dvec4(direction * (delta / length), delta);
				}
			}
		} else
		{
			dvec3 minimum, maximum;
			Simd::minMaxVec3(minimum, maximum, points, sizeof(dvec3), num_points);

			radius = -Consts::INF;
			for (int i = 0; i < num_points; i++)
			{
				double length2 = (points[i] - center).length2();
				if (radius < length2)
					radius = length2;
			}
			radius = (radius > 0.0) ? sqrt(radius) : -1.0;

			set((minimum + maximum) * 0.5, radius);
		}
	}
	UNIGINE_INLINE void expand(const WorldBoundSphere &bs)
	{
		if (bs.isValid())
		{
			if (isValid())
			{
				dvec3 direction = bs.center - center;
				double length = direction.length();
				if (length > Consts::EPS)
				{
					if (length + radius < bs.radius)
					{
						data = bs.data;
					} else if (length + bs.radius > radius)
					{
						dvec3 p0 = center - direction * (radius / length);
						dvec3 p1 = bs.center + direction * (bs.radius / length);
						dvec3 center = (p0 + p1) * 0.5;
						set(center, (p1 - center).length());
					}
				} else
				{
					if (radius < bs.radius)
						radius = bs.radius;
				}
			} else
			{
				data = bs.data;
			}
		}
	}
	UNIGINE_INLINE void expand(const WorldBoundBox &bb);

	// radius expand
	UNIGINE_INLINE void expandRadius(const dvec3 &point)
	{
		if (isValid())
		{
			double r = length(center - point);
			if (radius < r)
				radius = r;
		} else
		{
			set(point, BOUNDS_EPSILON);
		}
	}
	UNIGINE_INLINE void expandRadius(const dvec3 *points, int num_points)
	{
		if (isValid())
		{
			for (int i = 0; i < num_points; i++)
			{
				double r = length(center - points[i]);
				if (radius < r)
					radius = r;
			}
		} else
		{
			dvec3 minimum, maximum;
			Simd::minMaxVec3(minimum, maximum, points, sizeof(dvec3), num_points);
			dvec3 center = (minimum + maximum) * 0.5;
			radius = -Consts::INF;
			for (int i = 0; i < num_points; i++)
			{
				double length2 = (points[i] - center).length2();
				if (radius < length2)
					radius = length2;
			}
			radius = (radius > 0.0) ? sqrt(radius) : -1.0;
			set(center, radius);
		}
	}
	UNIGINE_INLINE void expandRadius(const WorldBoundSphere &bs)
	{
		if (!bs.isValid())
			return;

		if (isValid())
		{
			double r = length(bs.center - center) + bs.radius;
			if (radius < r)
				radius = r;
		} else
		{
			data = bs.data;
		}
	}
	UNIGINE_INLINE void expandRadius(const WorldBoundBox &bb);

	// inside points
	UNIGINE_INLINE bool inside(const dvec3 &point) const
	{
		return isValid() && insideValid(point);
	}
	UNIGINE_INLINE bool inside(const dvec3 &point, double radius) const
	{
		return isValid() && insideValid(point, radius);
	}
	UNIGINE_INLINE bool inside(const dvec3 &minimum, const dvec3 &maximum) const
	{
		return isValid() && insideValid(minimum, maximum);
	}

	UNIGINE_INLINE bool insideValid(const dvec3 &point) const
	{
		return length2(center - point) <= radius * radius;
	}
	UNIGINE_INLINE bool insideValid(const dvec3 &point, double radius_) const
	{
		return length2(center - point) <= pow2(radius_ + radius);
	}
	UNIGINE_INLINE bool insideValid(const dvec3 &minimum, const dvec3 &maximum) const
	{
		dvec3 direction;
		if (minimum.x > center.x)
			direction.x = minimum.x - center.x;
		else if (maximum.x < center.x)
			direction.x = maximum.x - center.x;
		else
			direction.x = 0.0;

		if (minimum.y > center.y)
			direction.y = minimum.y - center.y;
		else if (maximum.y < center.y)
			direction.y = maximum.y - center.y;
		else
			direction.y = 0.0;

		if (minimum.z > center.z)
			direction.z = minimum.z - center.z;
		else if (maximum.z < center.z)
			direction.z = maximum.z - center.z;
		else
			direction.z = 0.0;

		return direction.length2() < radius * radius;
	}

	// inside bounds
	UNIGINE_INLINE bool inside(const WorldBoundSphere &bs) const
	{
		return isValid() && bs.isValid() && insideValid(bs.center, bs.radius);
	}
	UNIGINE_INLINE bool inside(const WorldBoundBox &bb) const;

	UNIGINE_INLINE bool insideValid(const WorldBoundSphere &bs) const
	{
		return insideValid(bs.center, bs.radius);
	}
	UNIGINE_INLINE bool insideValid(const WorldBoundBox &bb) const;

	UNIGINE_INLINE bool insideAll(const WorldBoundSphere &bs) const
	{
		return isValid() && bs.isValid() && insideAllValid(bs);
	}
	UNIGINE_INLINE bool insideAll(const WorldBoundBox &bb) const;

	UNIGINE_INLINE bool insideAllValid(const WorldBoundSphere &bs) const
	{
		double r = radius - bs.radius;
		if (r > 0.0)
			return length2(center - bs.center) <= r * r;
		return false;
	}
	UNIGINE_INLINE bool insideAllValid(const WorldBoundBox &bb) const;

	// intersections
	UNIGINE_INLINE bool rayIntersection(const dvec3 &point, const dvec3 &direction) const
	{
		return isValid() && rayIntersectionValid(point, direction);
	}
	UNIGINE_INLINE bool getIntersection(const dvec3 &p0, const dvec3 &p1) const
	{
		return isValid() && getIntersectionValid(p0, p1);
	}

	UNIGINE_INLINE bool rayIntersectionValid(const dvec3 &point, const dvec3 &direction) const
	{
		double k = saturate(dot(direction, center - point) / length2(direction));
		return length2(center - point - direction * k) <= radius * radius;
	}
	UNIGINE_INLINE bool getIntersectionValid(const dvec3 &p0, const dvec3 &p1) const
	{
		return rayIntersectionValid(p0, p1 - p0);
	}

	// distance
	UNIGINE_INLINE double distance() const
	{
		if (isValid())
			return distanceValid();
		return Consts::INF;
	}
	UNIGINE_INLINE double distance(const dvec3 &point) const
	{
		if (isValid())
			return distanceValid(point);
		return Consts::INF;
	}
	UNIGINE_INLINE double distanceValid() const
	{
		return center.length() - radius;
	}
	UNIGINE_INLINE double distanceValid(const dvec3 &camera) const
	{
		return length(center - camera) - radius;
	}
	
	UNIGINE_INLINE bool isCameraVisible(const dvec3 &camera, double min_distance, double max_distance) const
	{
		double distance = length(center - camera);
		return	((distance - radius) <= max_distance) &&
				((distance + radius) >= min_distance);
	}
};

constexpr WorldBoundSphere WorldBoundSphere_inf(0.0, 0.0, 0.0, Consts::INF, ConstexprTag{});

UNIGINE_INLINE WorldBoundSphere operator*(const dmat4 &m, const WorldBoundSphere &bs)
{
	WorldBoundSphere ret = bs;
	ret.setTransform(m);
	return ret;
}

//////////////////////////////////////////////////////////////////////////
// WorldBoundBox
//////////////////////////////////////////////////////////////////////////
struct alignas(16) WorldBoundBox
{
	dvec3 minimum{dvec3_inf};
	dvec3 maximum{-dvec3_inf};

	UNIGINE_INLINE constexpr WorldBoundBox(double x_min, double y_min, double z_min, double x_max, double y_max, double z_max, ConstexprTag)
		: minimum(x_min, y_min, z_min, ConstexprTag{})
		, maximum(x_max, y_max, z_max, ConstexprTag{}) {}

	UNIGINE_INLINE WorldBoundBox()
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE WorldBoundBox(const dvec3 &minimum, const dvec3 &maximum)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(minimum, maximum);
	}
	UNIGINE_INLINE WorldBoundBox(const dvec3 *points, int num_points)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(points, num_points);
	}
	UNIGINE_INLINE WorldBoundBox(const WorldBoundBox &bb)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bb);
	}
	UNIGINE_INLINE WorldBoundBox(const WorldBoundBox &bb, const dmat4 &transform)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bb, transform);
	}
	UNIGINE_INLINE WorldBoundBox(const BoundBox &bb, const dmat4 &transform)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bb, transform);
	}
	UNIGINE_INLINE WorldBoundBox(const BoundBox &bb)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bb);
	}
	UNIGINE_INLINE explicit WorldBoundBox(const WorldBoundSphere &bs)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bs);
	}

	UNIGINE_INLINE WorldBoundBox &operator=(const WorldBoundBox &bb) { set(bb); return *this; }

	UNIGINE_INLINE void clear()
	{
		minimum = dvec3_inf;
		maximum = -dvec3_inf;
	}

	UNIGINE_INLINE void set(const dvec3 &min_, const dvec3 &max_)
	{
		minimum = min_;
		maximum = max_;
	}
	UNIGINE_INLINE void set(const dvec3 *points, int num_points)
	{
		Simd::minMaxVec3(minimum, maximum, points, sizeof(dvec3), num_points);
	}
	UNIGINE_INLINE void set(const WorldBoundSphere &bs)
	{
		clear();
		expand(bs);
	}
	UNIGINE_INLINE void set(const WorldBoundBox &bb)
	{
		minimum = bb.minimum;
		maximum = bb.maximum;
	}
	UNIGINE_INLINE void set(const WorldBoundBox &bb, const dmat4 &transform)
	{
		set(bb);
		setTransform(transform);
	}
	UNIGINE_INLINE void set(const BoundBox &bb)
	{
		minimum = dvec3(bb.minimum);
		maximum = dvec3(bb.maximum);
	}
	UNIGINE_INLINE void set(const BoundBox &bb, const dmat4 &transform)
	{
		set(bb);
		setTransform(transform);
	}

	// parameters
	UNIGINE_INLINE bool isValid() const { return minimum.x <= maximum.x; }
	UNIGINE_INLINE dvec3 getCenter() const { return (minimum + maximum) * 0.5f; }
	UNIGINE_INLINE dvec3 getSize() const { return maximum - minimum; }
	UNIGINE_INLINE void getPoints(dvec3 *points, int num_points) const
	{
		assert(num_points == 8 && "WorldBoundBox::getPoints(): bad points number");
		points[0].set(minimum);
		points[1].set(maximum.x, minimum.y, minimum.z);
		points[2].set(minimum.x, maximum.y, minimum.z);
		points[3].set(maximum.x, maximum.y, minimum.z);
		points[4].set(minimum.x, minimum.y, maximum.z);
		points[5].set(maximum.x, minimum.y, maximum.z);
		points[6].set(minimum.x, maximum.y, maximum.z);
		points[7].set(maximum);
	}
	UNIGINE_INLINE void getPlanes(dvec4 *planes, int num_planes) const
	{
		assert(num_planes == 6 && "WorldBoundBox::getPlanes(): bad planes number");
		planes[0].set(1.0, 0.0, 0.0, -maximum.x);
		planes[1].set(-1.0, 0.0, 0.0, minimum.x);
		planes[2].set(0.0, 1.0, 0.0, -maximum.y);
		planes[3].set(0.0, -1.0, 0.0, minimum.y);
		planes[4].set(0.0, 0.0, 1.0, -maximum.z);
		planes[5].set(0.0, 0.f, -1.0, minimum.z);
	}

	// transformation
	UNIGINE_INLINE void setTransform(const dmat4 &transform)
	{
		dvec3 center = (minimum + maximum) * 0.5f;
		dvec3 axis = maximum - center;
		double x = abs(transform.m00) * axis.x + abs(transform.m01) * axis.y + abs(transform.m02) * axis.z;
		double y = abs(transform.m10) * axis.x + abs(transform.m11) * axis.y + abs(transform.m12) * axis.z;
		double z = abs(transform.m20) * axis.x + abs(transform.m21) * axis.y + abs(transform.m22) * axis.z;
		center = transform * center;
		minimum = center - dvec3(x, y, z);
		maximum = center + dvec3(x, y, z);
	}
	UNIGINE_INLINE void setTransform(const WorldBoundSphere &bs, const dmat4 &transform)
	{
		setTransform(transform);
		dvec3 center = transform * bs.center;
		double x = transform.m00 * transform.m00 + transform.m10 * transform.m10 + transform.m20 * transform.m20;
		double y = transform.m01 * transform.m01 + transform.m11 * transform.m11 + transform.m21 * transform.m21;
		double z = transform.m02 * transform.m02 + transform.m12 * transform.m12 + transform.m22 * transform.m22;
		double radius = sqrt(max(max(x, y), z)) * bs.radius;
		minimum = max(minimum, center - radius);
		maximum = min(maximum, center + radius);
	}

	// compare
	UNIGINE_INLINE int compare(const WorldBoundBox &bb) const
	{
		return Math::compare(minimum, bb.minimum) && Math::compare(maximum, bb.maximum);
	}
	UNIGINE_INLINE int operator==(const WorldBoundBox &bb) const { return compare(bb); }
	UNIGINE_INLINE int operator!=(const WorldBoundBox &bb) const { return !compare(bb); }

	// expand
	UNIGINE_INLINE void expand(const dvec3 &point)
	{
		if (isValid())
		{
			minimum = min(minimum, point);
			maximum = max(maximum, point);
		} else
		{
			minimum = point - BOUNDS_EPSILON;
			maximum = point + BOUNDS_EPSILON;
		}
	}
	UNIGINE_INLINE void expand(const dvec3 *points, int num_points)
	{
		if (isValid())
		{
			dvec3 min_, max_;
			Simd::minMaxVec3(min_, max_, points, sizeof(dvec3), num_points);
			minimum = min(minimum, min_);
			maximum = max(maximum, max_);
		} else
		{
			Simd::minMaxVec3(minimum, maximum, points, sizeof(dvec3), num_points);
		}
	}
	UNIGINE_INLINE void expand(const WorldBoundSphere &bs)
	{
		if (!bs.isValid())
			return;
		const dvec3 &center = bs.center;
		dvec3 r(bs.radius);
		if (isValid())
		{
			minimum = min(minimum, center - r);
			maximum = max(maximum, center + r);
		} else
		{
			sub(minimum, center, r);
			add(maximum, center, r);
		}
	}
	UNIGINE_INLINE void expand(const WorldBoundBox &bb)
	{
		if (!bb.isValid())
			return;
		if (isValid())
		{
			minimum = min(minimum, bb.minimum);
			maximum = max(maximum, bb.maximum);
		} else
		{
			minimum = bb.minimum;
			maximum = bb.maximum;
		}
	}

	// inside points
	UNIGINE_INLINE bool inside(const dvec3 &point) const
	{
		return isValid() && insideValid(point);
	}
	UNIGINE_INLINE bool inside(const dvec3 &point, double radius) const
	{
		return isValid() && insideValid(point, radius);
	}
	UNIGINE_INLINE bool inside(const dvec3 &minimum, const dvec3 &maximum) const
	{
		return isValid() && insideValid(minimum, maximum);
	}

	UNIGINE_INLINE bool insideValid(const dvec3 &point) const
	{
		if (minimum.x > point.x || maximum.x < point.x) return false;
		if (minimum.y > point.y || maximum.y < point.y) return false;
		if (minimum.z > point.z || maximum.z < point.z) return false;
		return true;
	}
	UNIGINE_INLINE bool insideValid(const dvec3 &point, double radius) const
	{
		dvec3 direction;
		if (minimum.x > point.x)
			direction.x = minimum.x - point.x;
		else if (maximum.x < point.x)
			direction.x = maximum.x - point.x;
		else
			direction.x = 0.0;

		if (minimum.y > point.y)
			direction.y = minimum.y - point.y;
		else if (maximum.y < point.y)
			direction.y = maximum.y - point.y;
		else
			direction.y = 0.0;

		if (minimum.z > point.z)
			direction.z = minimum.z - point.z;
		else if (maximum.z < point.z)
			direction.z = maximum.z - point.z;
		else
			direction.z = 0.0;

		return direction.length2() < radius * radius;
	}
	UNIGINE_INLINE bool insideValid(const dvec3 &min_, const dvec3 &max_) const
	{
		if (minimum.x > max_.x || maximum.x < min_.x) return false;
		if (minimum.y > max_.y || maximum.y < min_.y) return false;
		if (minimum.z > max_.z || maximum.z < min_.z) return false;
		return true;
	}
	UNIGINE_INLINE bool insideValid(const dvec3 *points, int num_points) const
	{
		for (int i = 0; i < num_points; i++)
		{
			if (insideValid(points[i]))
				return true;
		}
		return false;
	}

	// inside bounds
	UNIGINE_INLINE bool inside(const WorldBoundSphere &bs) const
	{
		return isValid() && insideValid(bs.center, bs.radius);
	}
	UNIGINE_INLINE bool inside(const WorldBoundBox &bb) const
	{
		return isValid() && bb.isValid() && insideValid(bb.minimum, bb.maximum);
	}

	UNIGINE_INLINE bool insideValid(const WorldBoundSphere &bs) const
	{
		dvec3 direction;
		double radius = bs.radius;
		if (minimum.x > bs.center.x)
			direction.x = minimum.x - bs.center.x;
		else if (maximum.x < bs.center.x)
			direction.x = maximum.x - bs.center.x;
		else
			direction.x = 0.0;

		if (minimum.y > bs.center.y)
			direction.y = minimum.y - bs.center.y;
		else if (maximum.y < bs.center.y)
			direction.y = maximum.y - bs.center.y;
		else
			direction.y = 0.0;

		if (minimum.z > bs.center.z)
			direction.z = minimum.z - bs.center.z;
		else if (maximum.z < bs.center.z)
			direction.z = maximum.z - bs.center.z;
		else
			direction.z = 0.0;

		return direction.length2() < radius * radius;
	}
	UNIGINE_INLINE bool insideValid(const WorldBoundBox &bb) const
	{
		return insideValid(bb.minimum, bb.maximum);
	}

	UNIGINE_INLINE bool insideAll(const WorldBoundSphere &bs) const
	{
		return isValid() && bs.isValid() && insideAllValid(bs);
	}
	UNIGINE_INLINE bool insideAll(const WorldBoundBox &bb) const
	{
		return isValid() && bb.isValid() && insideAllValid(bb);
	}

	UNIGINE_INLINE bool insideAllValid(const WorldBoundSphere &bs) const
	{
		if (minimum.x > bs.center.x - bs.radius || maximum.x < bs.center.x + bs.radius) return false;
		if (minimum.y > bs.center.y - bs.radius || maximum.y < bs.center.y + bs.radius) return false;
		if (minimum.z > bs.center.z - bs.radius || maximum.z < bs.center.z + bs.radius) return false;

		return true;
	}
	UNIGINE_INLINE bool insideAllValid(const WorldBoundBox &bb) const
	{
		if (minimum.x > bb.minimum.x || maximum.x < bb.maximum.x) return false;
		if (minimum.y > bb.minimum.y || maximum.y < bb.maximum.y) return false;
		if (minimum.z > bb.minimum.z || maximum.z < bb.maximum.z) return false;
		return true;
	}
	UNIGINE_INLINE bool insideAllValid(const dvec3 *points, int num_points) const
	{
		for (int i = 0; i < num_points; i++)
		{
			if (!insideValid(points[i]))
				return false;
		}
		return true;
	}

	// inside cube
	UNIGINE_INLINE bool insideCube(int face, const dvec3 &offset) const
	{
		if (!isValid())
			return false;

		dvec3 min_temp = minimum - offset;
		dvec3 max_temp = maximum - offset;
		switch (face)
		{
			case 0: return (max_temp.x >= 0.0 && min_temp.y <=  max_temp.x && max_temp.y >= -max_temp.x && min_temp.z <=  max_temp.x && max_temp.z >= -max_temp.x);
			case 1: return (min_temp.x <= 0.0 && min_temp.y <= -min_temp.x && max_temp.y >=  min_temp.x && min_temp.z <= -min_temp.x && max_temp.z >=  min_temp.x);
			case 2: return (max_temp.y >= 0.0 && min_temp.x <=  max_temp.y && max_temp.x >= -max_temp.y && min_temp.z <=  max_temp.y && max_temp.z >= -max_temp.y);
			case 3: return (min_temp.y <= 0.0 && min_temp.x <= -min_temp.y && max_temp.x >=  min_temp.y && min_temp.z <= -min_temp.y && max_temp.z >=  min_temp.y);
			case 4: return (max_temp.z >= 0.0 && min_temp.x <=  max_temp.z && max_temp.x >= -max_temp.z && min_temp.y <=  max_temp.z && max_temp.y >= -max_temp.z);
			case 5: return (min_temp.z <= 0.0 && min_temp.x <= -min_temp.z && max_temp.x >=  min_temp.z && min_temp.y <= -min_temp.z && max_temp.y >=  min_temp.z);
		};

		assert(0 && "WorldBoundBox::insideCube(): bad face number");
		return false;
	}

	// intersections
	UNIGINE_INLINE bool rayIntersection(const dvec3 &point, const dvec3 &direction) const
	{
		return isValid() && rayIntersectionValid(point, direction);
	}
	UNIGINE_INLINE bool irayIntersection(const dvec3 &point, const dvec3 &idirection) const
	{
		return isValid() && irayIntersectionValid(point, idirection);
	}
	UNIGINE_INLINE bool getIntersection(const dvec3 &p0, const dvec3 &p1) const
	{
		return isValid() && getIntersectionValid(p0, p1);
	}

	UNIGINE_INLINE bool rayIntersectionValid(const dvec3 &point, const dvec3 &direction) const
	{
		return rayBoundBoxIntersection(point, direction, minimum, maximum);
	}
	UNIGINE_INLINE bool irayIntersectionValid(const dvec3 &point, const dvec3 &idirection) const
	{
		return irayBoundBoxIntersection(point, idirection, minimum, maximum);
	}
	UNIGINE_INLINE bool getIntersectionValid(const dvec3 &p0, const dvec3 &p1) const
	{
		return rayBoundBoxIntersection(p0, p1 - p0, minimum, maximum);
	}

	// distance
	UNIGINE_INLINE double distance() const
	{
		if (isValid())
			return distanceValid();
		return Consts::INF;
	}
	UNIGINE_INLINE double distance(const dvec3 &point) const
	{
		if (isValid())
			return distanceValid(point);
		return Consts::INF;
	}

	UNIGINE_INLINE double distanceValid() const
	{
		dvec3 direction;

		if (minimum.x > 0.0)
			direction.x = minimum.x;
		else if (maximum.x < 0.0)
			direction.x = maximum.x;
		else
			direction.x = 0.0;

		if (minimum.y > 0.0)
			direction.y = minimum.y;
		else if (maximum.y < 0.0)
			direction.y = maximum.y;
		else
			direction.y = 0.0;

		if (minimum.z > 0.0)
			direction.z = minimum.z;
		else if (maximum.z < 0.0)
			direction.z = maximum.z;
		else
			direction.z = 0.0;

		return sqrt(direction.length2());
	}
	UNIGINE_INLINE double distanceValid(const dvec3 &point) const
	{
		dvec3 direction;
		if (minimum.x > point.x)
			direction.x = minimum.x - point.x;
		else if (maximum.x < point.x)
			direction.x = maximum.x - point.x;
		else
			direction.x = 0.0;

		if (minimum.y > point.y)
			direction.y = minimum.y - point.y;
		else if (maximum.y < point.y)
			direction.y = maximum.y - point.y;
		else
			direction.y = 0.0;

		if (minimum.z > point.z)
			direction.z = minimum.z - point.z;
		else if (maximum.z < point.z)
			direction.z = maximum.z - point.z;
		else
			direction.z = 0.0;

		return sqrt(direction.length2());
	}
};

constexpr WorldBoundBox WorldBoundBox_inf(-Consts::INF, -Consts::INF, -Consts::INF, Consts::INF, Consts::INF, Consts::INF, ConstexprTag{});

UNIGINE_INLINE WorldBoundBox operator*(const dmat4 &m, const WorldBoundBox &bb)
{
	WorldBoundBox ret = bb;
	ret.setTransform(m);
	return ret;
}

UNIGINE_INLINE void WorldBoundSphere::set(const WorldBoundBox &bb)
{
	clear();
	if (bb.isValid())
	{
		Vec3 center = (bb.minimum + bb.maximum) * 0.5f;
		set(center, length(bb.maximum - center));
	}
}
UNIGINE_INLINE void WorldBoundSphere::expand(const WorldBoundBox &bb)
{
	if (!bb.isValid())
		return;

	if (isValid())
	{
		expand(bb.minimum);
		expand(bb.maximum);
		expand(dvec3(bb.maximum.x, bb.minimum.y, bb.minimum.z));
		expand(dvec3(bb.minimum.x, bb.maximum.y, bb.minimum.z));
		expand(dvec3(bb.maximum.x, bb.maximum.y, bb.minimum.z));
		expand(dvec3(bb.minimum.x, bb.minimum.y, bb.maximum.z));
		expand(dvec3(bb.maximum.x, bb.minimum.y, bb.maximum.z));
		expand(dvec3(bb.minimum.x, bb.maximum.y, bb.maximum.z));
	} else
	{
		dvec3 center = (bb.minimum + bb.maximum) * 0.5;
		set(center, length(bb.maximum - center));
	}
}
UNIGINE_INLINE void WorldBoundSphere::expandRadius(const WorldBoundBox &bb)
{
	if (!bb.isValid())
		return;

	if (isValid())
	{
		expandRadius(bb.minimum);
		expandRadius(bb.maximum);
		expandRadius(dvec3(bb.maximum.x, bb.minimum.y, bb.minimum.z));
		expandRadius(dvec3(bb.minimum.x, bb.maximum.y, bb.minimum.z));
		expandRadius(dvec3(bb.maximum.x, bb.maximum.y, bb.minimum.z));
		expandRadius(dvec3(bb.minimum.x, bb.minimum.y, bb.maximum.z));
		expandRadius(dvec3(bb.maximum.x, bb.minimum.y, bb.maximum.z));
		expandRadius(dvec3(bb.minimum.x, bb.maximum.y, bb.maximum.z));
	} else
	{
		dvec3 center = (bb.minimum + bb.maximum) * 0.5;
		set(center, length(bb.maximum - center));
	}
}

UNIGINE_INLINE bool WorldBoundSphere::inside(const WorldBoundBox &bb) const
{
	return isValid() && bb.isValid() && insideValid(bb.minimum, bb.maximum);
}
UNIGINE_INLINE bool WorldBoundSphere::insideAll(const WorldBoundBox &bb) const
{
	return isValid() && bb.isValid() && insideAllValid(bb);
}
UNIGINE_INLINE bool WorldBoundSphere::insideValid(const WorldBoundBox &bb) const
{
	return insideValid(bb.minimum, bb.maximum);
}
UNIGINE_INLINE bool WorldBoundSphere::insideAllValid(const WorldBoundBox &bb) const
{
	return insideValid(bb.minimum) && insideValid(bb.maximum)
		&& insideValid(Vec3(bb.maximum.x, bb.minimum.y, bb.minimum.z))
		&& insideValid(Vec3(bb.minimum.x, bb.maximum.y, bb.minimum.z))
		&& insideValid(Vec3(bb.maximum.x, bb.maximum.y, bb.minimum.z))
		&& insideValid(Vec3(bb.minimum.x, bb.minimum.y, bb.maximum.z))
		&& insideValid(Vec3(bb.maximum.x, bb.minimum.y, bb.maximum.z))
		&& insideValid(Vec3(bb.minimum.x, bb.maximum.y, bb.maximum.z));
}

//////////////////////////////////////////////////////////////////////////
// WorldBoundFrustum
//////////////////////////////////////////////////////////////////////////
struct alignas(16) WorldBoundFrustum
{
	dvec3 camera;
	WorldBoundBox bound_box;
	dvec3 points[8];		// bounding points
	dvec4 planes[6];		// aos clipping planes

	UNIGINE_INLINE WorldBoundFrustum()
	{
		UNIGINE_ASSERT_ALIGNED16(this);
	}
	UNIGINE_INLINE WorldBoundFrustum(const mat4 &projection, const dmat4 &modelview)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(projection, modelview);
	}
	UNIGINE_INLINE WorldBoundFrustum(const WorldBoundFrustum &bf)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bf);
	}
	UNIGINE_INLINE WorldBoundFrustum(const BoundFrustum &bf)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bf);
	}
	UNIGINE_INLINE WorldBoundFrustum(const WorldBoundFrustum &bf, const dmat4 &itransform)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bf, itransform);
	}
	UNIGINE_INLINE WorldBoundFrustum(const BoundFrustum &bf, const dmat4 &itransform)
	{
		UNIGINE_ASSERT_ALIGNED16(this);
		set(bf, itransform);
	}

	UNIGINE_INLINE WorldBoundFrustum &operator=(const WorldBoundFrustum &bf)
	{
		set(bf);
		return *this;
	}

	UNIGINE_INLINE void clear()
	{
		// camera
		camera = dvec3_zero;
		bound_box.clear();

		// clipping planes
		for (int i = 0; i < 6; i++)
			planes[i] = dvec4_zero;

		// clipping planes and points
		for (int i = 0; i < 8; i++)
			points[i] = dvec3_zero;
	}


	UNIGINE_INLINE void set(const mat4 &projection, const dmat4 &modelview)
	{
		dvec3 offset = inverse(modelview).getColumn(3).xyz;
		mat4 proj = projection * mat4(rotation(modelview));
		mat4 iproj = inverse(proj);

		// camera
		camera.x = iproj.m03;
		camera.y = iproj.m13;
		camera.z = iproj.m23;
		camera /= iproj.m33;
		camera += offset;

		// points
		points[0].set(-1.0, -1.0, -1.0);
		points[1].set(1.0, -1.0, -1.0);
		points[2].set(-1.0, 1.0, -1.0);
		points[3].set(1.0, 1.0, -1.0);
		points[4].set(-1.0, -1.0, 1.0);
		points[5].set(1.0, -1.0, 1.0);
		points[6].set(-1.0, 1.0, 1.0);
		points[7].set(1.0, 1.0, 1.0);
		for (int i = 0; i < 8; i++)
		{
			Math::proj(points[i], iproj, points[i]);
			points[i] += offset;
		}

		// clipping planes
		planes[0].set(proj.m30 + proj.m00, proj.m31 + proj.m01, proj.m32 + proj.m02, proj.m33 + proj.m03);
		planes[1].set(proj.m30 - proj.m00, proj.m31 - proj.m01, proj.m32 - proj.m02, proj.m33 - proj.m03);
		planes[2].set(proj.m30 + proj.m10, proj.m31 + proj.m11, proj.m32 + proj.m12, proj.m33 + proj.m13);
		planes[3].set(proj.m30 - proj.m10, proj.m31 - proj.m11, proj.m32 - proj.m12, proj.m33 - proj.m13);
		planes[4].set(proj.m30 + proj.m20, proj.m31 + proj.m21, proj.m32 + proj.m22, proj.m33 + proj.m23);
		planes[5].set(proj.m30 - proj.m20, proj.m31 - proj.m21, proj.m32 - proj.m22, proj.m33 - proj.m23);
		dmat4 itransform = translate(-offset);
		for (int i = 0; i < 6; i++)
		{
			planes[i] = planes[i] * itransform;
			planes[i] /= planes[i].xyz.length();
		}

		update_bounds();
	}

	UNIGINE_INLINE void set(const WorldBoundFrustum &bf)
	{
		camera = bf.camera;
		bound_box = bf.bound_box;

		for (int i = 0; i < 6; i++)
			planes[i] = bf.planes[i];

		for (int i = 0; i < 8; i++)
			points[i] = bf.points[i];
	}
	UNIGINE_INLINE void set(const WorldBoundFrustum &bf, const dmat4 &itransform)
	{
		set(bf);
		setITransform(itransform);
	}
	UNIGINE_INLINE void set(const BoundFrustum &bf)
	{
		camera = dvec3(bf.camera);
		bound_box = WorldBoundBox(bf.bound_box);

		for (int i = 0; i < 6; i++)
			planes[i] = dvec4(bf.planes[i]);

		for (int i = 0; i < 8; i++)
			points[i] = dvec3(bf.points[i]);
	}
	UNIGINE_INLINE void set(const BoundFrustum &bf, const dmat4 &itransform)
	{
		set(bf);
		setITransform(itransform);
	}

	// parameters
	UNIGINE_INLINE bool isValid() const { return bound_box.isValid(); }

	// transformation
	UNIGINE_INLINE void setITransform(const dmat4 &itransform)
	{
		dmat4 transform = inverse(itransform);
		camera = transform * camera;

		// points
		Simd::mulMat4Vec3(points, sizeof(dvec3), transform, points, sizeof(dvec3), 8);

		// clipping planes
		for (int i = 0; i < 6; i++)
		{
			planes[i] = planes[i] * itransform;
			planes[i] /= length(planes[i].xyz);
		}

		update_bounds();
	}
	UNIGINE_INLINE void setTransform(const dmat4 &transform)
	{
		camera = transform * camera;

		// points
		Simd::mulMat4Vec3(points, sizeof(dvec3), transform, points, sizeof(dvec3), 8);

		// clipping planes
		dmat4 itransform = inverse(transform);
		for (int i = 0; i < 6; i++)
		{
			planes[i] = planes[i] * itransform;
			planes[i] /= length(planes[i].xyz);
		}

		update_bounds();
	}

	// compare
	UNIGINE_INLINE int compare(const WorldBoundFrustum &bf) const
	{
		if (camera != bf.camera) return false;
		if (bound_box != bf.bound_box) return false;

		// clipping planes
		if (planes[0] != bf.planes[0]) return false;
		if (planes[1] != bf.planes[1]) return false;
		if (planes[2] != bf.planes[2]) return false;
		if (planes[3] != bf.planes[3]) return false;
		if (planes[4] != bf.planes[4]) return false;
		if (planes[5] != bf.planes[5]) return false;

		return true;
	}
	UNIGINE_INLINE int operator==(const WorldBoundFrustum &bf) const { return compare(bf); }
	UNIGINE_INLINE int operator!=(const WorldBoundFrustum &bf) const { return !compare(bf); }

	// inside points
	UNIGINE_INLINE bool inside(const dvec3 &point) const
	{
		return bound_box.insideValid(point)
			&& (dot(planes[0], point) > 0.0)
			&& (dot(planes[1], point) > 0.0)
			&& (dot(planes[2], point) > 0.0)
			&& (dot(planes[3], point) > 0.0)
			&& (dot(planes[4], point) > 0.0)
			&& (dot(planes[5], point) > 0.0);
	}
	UNIGINE_INLINE bool inside(const dvec3 &point, double radius) const
	{
		return bound_box.insideValid(point, radius)
			&& (dot(planes[0], point) > -radius)
			&& (dot(planes[1], point) > -radius)
			&& (dot(planes[2], point) > -radius)
			&& (dot(planes[3], point) > -radius)
			&& (dot(planes[4], point) > -radius)
			&& (dot(planes[5], point) > -radius);
	}
	UNIGINE_INLINE bool inside(const dvec3 &minimum, const dvec3 &maximum) const
	{
		return bound_box.insideValid(minimum, maximum)
			&& inside_plane(planes[0], minimum, maximum)
			&& inside_plane(planes[1], minimum, maximum)
			&& inside_plane(planes[2], minimum, maximum)
			&& inside_plane(planes[3], minimum, maximum)
			&& inside_plane(planes[4], minimum, maximum)
			&& inside_plane(planes[5], minimum, maximum);
	}
	UNIGINE_INLINE bool inside(const dvec3 *points, int num_points) const
	{
		return bound_box.insideValid(points, num_points)
			&& inside_plane(planes[0], points, num_points)
			&& inside_plane(planes[1], points, num_points)
			&& inside_plane(planes[2], points, num_points)
			&& inside_plane(planes[3], points, num_points)
			&& inside_plane(planes[4], points, num_points)
			&& inside_plane(planes[5], points, num_points);
	}

	UNIGINE_INLINE bool insideFast(const dvec3 &point) const
	{
		return bound_box.inside(point)
			&& (dot(planes[0], point) > 0.0)
			&& (dot(planes[1], point) > 0.0)
			&& (dot(planes[2], point) > 0.0)
			&& (dot(planes[3], point) > 0.0);
	}
	UNIGINE_INLINE bool insideFast(const dvec3 &point, double radius) const
	{
		return bound_box.inside(point, radius)
			&& (dot(planes[0], point) > -radius)
			&& (dot(planes[1], point) > -radius)
			&& (dot(planes[2], point) > -radius)
			&& (dot(planes[3], point) > -radius);
	}
	UNIGINE_INLINE bool insideFast(const dvec3 &minimum, const dvec3 &maximum) const
	{
		return bound_box.insideValid(minimum, maximum)
			&& inside_plane(planes[0], minimum, maximum)
			&& inside_plane(planes[1], minimum, maximum)
			&& inside_plane(planes[2], minimum, maximum)
			&& inside_plane(planes[3], minimum, maximum);
	}
	UNIGINE_INLINE bool insideFast(const dvec3 *points, int num_points) const
	{
		return bound_box.insideValid(points, num_points)
			&& inside_plane(planes[0], points, num_points)
			&& inside_plane(planes[1], points, num_points)
			&& inside_plane(planes[2], points, num_points)
			&& inside_plane(planes[3], points, num_points);
	}

	// inside bounds
	UNIGINE_INLINE bool inside(const WorldBoundSphere &bs) const { return bs.isValid() ? insideValid(bs) : false; }
	UNIGINE_INLINE bool inside(const WorldBoundBox &bb) const { return bb.isValid() ? insideValid(bb) : false; }
	UNIGINE_INLINE bool inside(const WorldBoundFrustum &bf) const { return bf.isValid() ? insideValid(bf) : false; }

	UNIGINE_INLINE bool insideValid(const WorldBoundSphere &bs) const { return inside(bs.center, bs.radius); }
	UNIGINE_INLINE bool insideValid(const WorldBoundBox &bb) const { return inside(bb.minimum, bb.maximum); }
	UNIGINE_INLINE bool insideValid(const WorldBoundFrustum &bf) const
	{
		return bound_box.insideValid(bf.bound_box)
			&& inside_plane(planes[0], bf.points, 8)
			&& inside_plane(planes[1], bf.points, 8)
			&& inside_plane(planes[2], bf.points, 8)
			&& inside_plane(planes[3], bf.points, 8)
			&& inside_plane(planes[4], bf.points, 8)
			&& inside_plane(planes[5], bf.points, 8);
	}

	UNIGINE_INLINE bool insideValidFast(const WorldBoundSphere &bs) const { return insideFast(bs.center, bs.radius); }
	UNIGINE_INLINE bool insideValidFast(const WorldBoundBox &bb) const { return insideFast(bb.minimum, bb.maximum); }
	UNIGINE_INLINE bool insideValidFast(const WorldBoundFrustum &bf) const
	{
		return bound_box.insideValid(bf.bound_box)
			&& inside_plane(planes[0], bf.points, 8)
			&& inside_plane(planes[1], bf.points, 8)
			&& inside_plane(planes[2], bf.points, 8)
			&& inside_plane(planes[3], bf.points, 8);
	}

	// inside all bounds
	UNIGINE_INLINE bool insideAll(const WorldBoundSphere &bs) const { return bs.isValid() ? insideAllValid(bs) : false; }
	UNIGINE_INLINE bool insideAll(const WorldBoundBox &bb) const { return bb.isValid() ? insideAllValid(bb) : false; }
	UNIGINE_INLINE bool insideAll(const WorldBoundFrustum &bf) const { return bf.isValid() ? insideAllValid(bf) : false; }

	UNIGINE_INLINE bool insideAllValid(const WorldBoundSphere &bs) const { return inside(bs.center, -bs.radius); }
	UNIGINE_INLINE bool insideAllValid(const WorldBoundBox &bb) const
	{
		return bound_box.insideAllValid(bb)
			&& inside(bb.minimum)
			&& inside(bb.maximum)
			&& inside(dvec3(bb.maximum.x, bb.minimum.y, bb.minimum.z))
			&& inside(dvec3(bb.minimum.x, bb.maximum.y, bb.minimum.z))
			&& inside(dvec3(bb.maximum.x, bb.maximum.y, bb.minimum.z))
			&& inside(dvec3(bb.minimum.x, bb.minimum.y, bb.maximum.z))
			&& inside(dvec3(bb.maximum.x, bb.minimum.y, bb.maximum.z))
			&& inside(dvec3(bb.minimum.x, bb.maximum.y, bb.maximum.z));
	}
	UNIGINE_INLINE bool insideAllValid(const WorldBoundFrustum &bf) const
	{
		return bound_box.insideValid(bf.bound_box)
			&& inside(bf.points[0])
			&& inside(bf.points[1])
			&& inside(bf.points[2])
			&& inside(bf.points[3])
			&& inside(bf.points[4])
			&& inside(bf.points[5])
			&& inside(bf.points[6])
			&& inside(bf.points[7]);
	}

	UNIGINE_INLINE bool insideAllValidFast(const WorldBoundSphere &bs) const { return insideFast(bs.center, -bs.radius); }
	UNIGINE_INLINE bool insideAllValidFast(const WorldBoundBox &bb) const
	{
		return bound_box.insideAllValid(bb)
			&& insideFast(bb.minimum)
			&& insideFast(bb.maximum)
			&& insideFast(dvec3(bb.maximum.x, bb.minimum.y, bb.minimum.z))
			&& insideFast(dvec3(bb.minimum.x, bb.maximum.y, bb.minimum.z))
			&& insideFast(dvec3(bb.maximum.x, bb.maximum.y, bb.minimum.z))
			&& insideFast(dvec3(bb.minimum.x, bb.minimum.y, bb.maximum.z))
			&& insideFast(dvec3(bb.maximum.x, bb.minimum.y, bb.maximum.z))
			&& insideFast(dvec3(bb.minimum.x, bb.maximum.y, bb.maximum.z));
	}
	UNIGINE_INLINE bool insideAllValidFast(const WorldBoundFrustum &bf) const
	{
		return bound_box.insideValid(bf.bound_box)
			&& insideFast(bf.points[0])
			&& insideFast(bf.points[1])
			&& insideFast(bf.points[2])
			&& insideFast(bf.points[3])
			&& insideFast(bf.points[4])
			&& insideFast(bf.points[5])
			&& insideFast(bf.points[6])
			&& insideFast(bf.points[7]);
	}

	// inside planes bounds
	UNIGINE_INLINE bool insidePlanes(const WorldBoundSphere &bs) const { return bs.isValid() ? insidePlanesValid(bs) : false; }
	UNIGINE_INLINE bool insidePlanes(const WorldBoundBox &bb) const { return bb.isValid() ? insidePlanesValid(bb) : false; }
	UNIGINE_INLINE bool insidePlanes(const WorldBoundFrustum &bf) const { return bf.isValid() ? insidePlanesValid(bf) : false; }

	UNIGINE_INLINE bool insidePlanesValid(const WorldBoundSphere &bs) const { return inside(bs.center, bs.radius); }
	UNIGINE_INLINE bool insidePlanesValid(const WorldBoundBox &bb) const { return inside(bb.minimum, bb.maximum); }
	UNIGINE_INLINE bool insidePlanesValid(const WorldBoundFrustum &bf) const { return inside(bf.points, 8); }

	UNIGINE_INLINE bool insidePlanesValidFast(const WorldBoundSphere &bs) const { return insideFast(bs.center, bs.radius); }
	UNIGINE_INLINE bool insidePlanesValidFast(const WorldBoundBox &bb) const { return insideFast(bb.minimum, bb.maximum); }
	UNIGINE_INLINE bool insidePlanesValidFast(const WorldBoundFrustum &bf) const { return insideFast(bf.points, 8); }

	// inside shadow
	UNIGINE_INLINE bool insideShadowValid(const WorldBoundSphere &object, const dvec3 &direction) const
	{
		// object is inside the bound frustum
		if (insideFast(object.center, object.radius))
			return true;

		// shadow volume is inside the bound frustum
		for (int i = 0; i < 4; i++)
		{
			double k = dot3(planes[i], direction);
			if (abs(k) < Consts::EPS)
				continue;
			k = -dot(planes[i], object.center) / k;
			if (k > object.radius)
				continue;
			if (insideFast(object.center + direction * k, object.radius))
				return true;
		}

		return false;
	}
	UNIGINE_INLINE bool insideShadowValid(const WorldBoundSphere &object, const WorldBoundSphere &light, const dvec3 &offset) const
	{
		// object is outside the light bounds
		if (light.insideValid(object) == false)
			return false;

		// object is inside the bound frustum
		if (insideFast(object.center, object.radius))
			return true;

		// direction from light center to object center
		dvec3 direction = object.center - offset;
		double distance = length(direction);
		if (distance < object.radius + Consts::EPS)
			return true;
		direction /= distance;

		// basis
		dvec3 x, y;
		orthoBasis(direction, x, y);

		// near points
		dvec3 x0 = x * object.radius;
		dvec3 y0 = y * object.radius;
		dvec3 z0 = offset + direction * (distance - object.radius);

		// far points
		double radius = light.radius + dot(direction, light.center - offset);
		double k = object.radius * radius / (distance - object.radius);
		dvec3 x1 = x * k;
		dvec3 y1 = y * k;
		dvec3 z1 = offset + direction * radius;

		// check visibility
		dvec3 points[8] = {
			x0 + y0 + z0, x0 - y0 + z0, -x0 - y0 + z0, -x0 + y0 + z0,
			x1 + y1 + z1, x1 - y1 + z1, -x1 - y1 + z1, -x1 + y1 + z1,
		};

		return insideFast(points, 8);
	}

private:
	UNIGINE_INLINE static bool inside_plane(const dvec4 &plane, const dvec3 &minimum, const dvec3 &maximum)
	{
		return max(minimum * plane.xyz, maximum * plane.xyz).sum() > -plane.w;
	}
	UNIGINE_INLINE static bool inside_plane(const dvec4 &plane, const dvec3 *points, int num_points)
	{
		for (int i = 0; i < num_points; i++)
		{
			if (dot(plane, points[i]) > 0.0f)
				return true;
		}
		return false;
	}
	UNIGINE_INLINE void update_bounds()
	{
		bound_box.set(points, 8);
	}
};

UNIGINE_INLINE WorldBoundFrustum operator*(const dmat4 &m, const WorldBoundFrustum &bb)
{
	WorldBoundFrustum ret = bb;
	ret.setTransform(m);
	return ret;
}

UNIGINE_INLINE void BoundSphere::set(const WorldBoundSphere &bs)
{
	data = vec4(bs.data);
}
UNIGINE_INLINE void BoundBox::set(const WorldBoundBox &bb)
{
	maximum = vec3(bb.maximum);
	minimum = vec3(bb.minimum);
}
UNIGINE_INLINE void BoundFrustum::set(const WorldBoundFrustum &bf)
{
	camera = vec3(bf.camera);
	bound_box = BoundBox(bf.bound_box);

	for (int i = 0; i < 6; i++)
		planes[i] = vec4(bf.planes[i]);

	for (int i = 0; i < 8; i++)
		points[i] = vec3(bf.points[i]);
}

#endif /* USE_DOUBLE */

static_assert(sizeof(BoundSphere) == 4 * sizeof(float), "struct size must remain unchanged");
static_assert(sizeof(BoundBox) == 4 * (1 + 1) * sizeof(float), "struct size must remain unchanged");
static_assert(sizeof(BoundFrustum) == (4 * (1 + 2 + 6 + 8) * sizeof(float)), "struct size must remain unchanged");

#if defined(USE_DOUBLE) || defined(UNIGINE_DOUBLE)
	static_assert(sizeof(WorldBoundSphere) == 4 * sizeof(double), "struct size must remain unchanged");
	static_assert(sizeof(WorldBoundBox) == 4 * (1 + 1) * sizeof(double), "struct size must remain unchanged");
	static_assert(sizeof(WorldBoundFrustum) == 4 * (1 + 2 + 6 + 8) * sizeof(double), "struct size must remain unchanged");
#endif

}

} // namespace Unigine
