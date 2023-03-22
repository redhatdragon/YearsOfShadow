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
#include "UnigineMathLibVec4.h"

#include "UnigineMathLibDVec2.h"
#include "UnigineMathLibDVec3.h"
#include "UnigineMathLibDVec4.h"

#include "UnigineMathLibHVec2.h"
#include "UnigineMathLibHVec3.h"
#include "UnigineMathLibHVec4.h"

#include "UnigineMathLibIVec2.h"
#include "UnigineMathLibIVec3.h"
#include "UnigineMathLibIVec4.h"

#include "UnigineMathLibSVec4.h"
#include "UnigineMathLibBVec4.h"

#include "UnigineMathLibMat2.h"
#include "UnigineMathLibMat3.h"
#include "UnigineMathLibMat4.h"

#include "UnigineMathLibDMat4.h"

#include "UnigineMathLibQuat.h"

namespace Unigine
{

namespace Math
{

//////////////////////////////////////////////////////////////////////////
// Orthonormal basis
//////////////////////////////////////////////////////////////////////////

UNIGINE_INLINE void orthoBasis(const vec3 &v, vec3 &tangent, vec3 &binormal)
{
	if (abs(v.z) > 0.7f)
	{
		float length2 = v.y * v.y + v.z * v.z;
		float ilength = rsqrt(length2);
		tangent.x = 0.0f;
		tangent.y = -v.z * ilength;
		tangent.z = v.y * ilength;
		binormal.x = length2 * ilength;
		binormal.y = -v.x * tangent.z;
		binormal.z = v.x * tangent.y;
	} else
	{
		float length2 = v.x * v.x + v.y * v.y;
		float ilength = rsqrt(length2);
		tangent.x = -v.y * ilength;
		tangent.y = v.x * ilength;
		tangent.z = 0.0f;
		binormal.x = -v.z * tangent.y;
		binormal.y = v.z * tangent.x;
		binormal.z = length2 * ilength;
	}
}
UNIGINE_INLINE void orthoBasis(const dvec3 &v, dvec3 &tangent, dvec3 &binormal)
{
	if (abs(v.z) > 0.7)
	{
		double length2 = v.y * v.y + v.z * v.z;
		double ilength = rsqrt(length2);
		tangent.x = 0.0;
		tangent.y = -v.z * ilength;
		tangent.z = v.y * ilength;
		binormal.x = length2 * ilength;
		binormal.y = -v.x * tangent.z;
		binormal.z = v.x * tangent.y;
	} else
	{
		double length2 = v.x * v.x + v.y * v.y;
		double ilength = rsqrt(length2);
		tangent.x = -v.y * ilength;
		tangent.y = v.x * ilength;
		tangent.z = 0.0;
		binormal.x = -v.z * tangent.y;
		binormal.y = v.z * tangent.x;
		binormal.z = length2 * ilength;
	}
}

UNIGINE_INLINE void orthoTransform(const vec3 &v, mat4 &transform)
{
	if (Math::abs(v.z) > 0.7f)
	{
		float length2 = v.y * v.y + v.z * v.z;
		float ilength = Math::rsqrt(length2);
		transform.m00 = 0.0f;
		transform.m10 = -v.z * ilength;
		transform.m20 = v.y * ilength;
		transform.m01 = length2 * ilength;
		transform.m11 = -v.x * transform.m20;
		transform.m21 = v.x * transform.m10;
	} else
	{
		float length2 = v.x * v.x + v.y * v.y;
		float ilength = Math::rsqrt(length2);
		transform.m00 = -v.y * ilength;
		transform.m10 = v.x * ilength;
		transform.m20 = 0.0f;
		transform.m01 = -v.z * transform.m10;
		transform.m11 = v.z * transform.m00;
		transform.m21 = length2 * ilength;
	}

	transform.m02 = v.x;
	transform.m12 = v.y;
	transform.m22 = v.z;

	transform.m03 = 0.0f;
	transform.m13 = 0.0f;
	transform.m23 = 0.0f;

	transform.m30 = 0.0f;
	transform.m31 = 0.0f;
	transform.m32 = 0.0f;
	transform.m33 = 1.0f;

	transform = rotate(v, 90.0f) * transform;
}
UNIGINE_INLINE void orthoTransform(const dvec3 &v, dmat4 &transform)
{
	if (Math::abs(v.z) > 0.7)
	{
		double length2 = v.y * v.y + v.z * v.z;
		double ilength = Math::rsqrt(length2);
		transform.m00 = 0.0;
		transform.m10 = -v.z * ilength;
		transform.m20 = v.y * ilength;
		transform.m01 = length2 * ilength;
		transform.m11 = -v.x * transform.m20;
		transform.m21 = v.x * transform.m10;
	} else
	{
		double length2 = v.x * v.x + v.y * v.y;
		double ilength = Math::rsqrt(length2);
		transform.m00 = -v.y * ilength;
		transform.m10 = v.x * ilength;
		transform.m20 = 0.0;
		transform.m01 = -v.z * transform.m10;
		transform.m11 = v.z * transform.m00;
		transform.m21 = length2 * ilength;
	}

	transform.m02 = v.x;
	transform.m12 = v.y;
	transform.m22 = v.z;

	transform.m03 = 0.0;
	transform.m13 = 0.0;
	transform.m23 = 0.0;

	transform = rotate(v, 90.0) * transform;
}

UNIGINE_INLINE quat orthoTangent(const vec4 &t, const vec3 &n)
{
	vec3 normal(n);
	normal.normalize();

	vec3 tangent, binormal;
	if (t.w > 0.0f)
	{
		cross3(binormal, normal, t);
		cross(tangent, binormal, normal);
	} else
	{
		cross3(binormal, t, normal);
		cross(tangent, normal, binormal);
		cross(binormal, normal, tangent);
	}

	tangent.normalize();
	binormal.normalize();
	quat basis(tangent, binormal, normal);
	if (basis.w < 0.0f)
		basis = -basis;
	if (basis.w < 1e-3f)
		basis.w = 1e-3f;
	if (t.w <= 0.0f)
		basis = -basis;
	return basis;
}
UNIGINE_INLINE quat orthoTangent(const vec3 &t, const vec3 &b, const vec3 &n)
{
	vec3 normal(n);
	normal.normalize();

	vec3 tangent, binormal;
	float sign = dot(normal, cross(t, b));
	if (sign > 0.0f)
	{
		cross(binormal, normal, t);
		cross(tangent, binormal, normal);
	} else
	{
		cross(binormal, t, normal);
		cross(tangent, normal, binormal);
		cross(binormal, normal, tangent);
	}
	tangent.normalize();
	binormal.normalize();
	quat basis(tangent, binormal, normal);
	if (basis.w < 0.0f)
		basis = -basis;
	if (basis.w < 1e-3f)
		basis.w = 1e-3f;
	if (sign <= 0.0f)
		basis = -basis;
	return basis;
}

//////////////////////////////////////////////////////////////////////////
// Triangle parameters
//////////////////////////////////////////////////////////////////////////

UNIGINE_INLINE float triangleArea(const vec3 &v0, const vec3 &v1, const vec3 &v2)
{
	float a = length(v1 - v2);
	float b = length(v2 - v0);
	float c = length(v0 - v1);
	float s = (a + b + c) * 0.5f;
	return fsqrt(s * (s - a) * (s - b) * (s - c));
}
UNIGINE_INLINE double triangleArea(const dvec3 &v0, const dvec3 &v1, const dvec3 &v2)
{
	double a = length(v1 - v2);
	double b = length(v2 - v0);
	double c = length(v0 - v1);
	double s = (a + b + c) * 0.5;
	return dsqrt(s * (s - a) * (s - b) * (s - c));
}

UNIGINE_INLINE vec3 triangleNormal(const vec3 &v0, const vec3 &v1, const vec3 &v2)
{
	vec3 v10, v20, normal;
	cross(normal, sub(v10, v1, v0), sub(v20, v2, v0));
	normal.normalize();
	return normal;
}
UNIGINE_INLINE dvec3 triangleNormal(const dvec3 &v0, const dvec3 &v1, const dvec3 &v2)
{
	dvec3 v10, v20, normal;
	cross(normal, sub(v10, v1, v0), sub(v20, v2, v0));
	normal.normalize();
	return normal;
}

UNIGINE_INLINE vec4 trianglePlane(const vec3 &v0, const vec3 &v1, const vec3 &v2)
{
	vec3 v10, v20, normal;
	cross(normal, sub(v10, v1, v0), sub(v20, v2, v0));
	normal.normalize();
	return vec4(normal, -dot(normal, v0));
}
UNIGINE_INLINE dvec4 trianglePlane(const dvec3 &v0, const dvec3 &v1, const dvec3 &v2)
{
	dvec3 v10, v20, normal;
	cross(normal, sub(v10, v1, v0), sub(v20, v2, v0));
	normal.normalize();
	return dvec4(normal, -dot(normal, v0));
}

UNIGINE_INLINE vec4 triangleTangent(const vec3 &v0, const vec3 &v1, const vec3 &v2)
{
	vec3 tangent, binormal;
	vec3 normal = triangleNormal(v0, v1, v2);
	orthoBasis(normal, tangent, binormal);
	return vec4(tangent, 1.0f);
}
UNIGINE_INLINE vec4 triangleTangent(const vec3 &v0, const vec3 &v1, const vec3 &v2, const vec2 &t0, const vec2 &t1, const vec2 &t2, const vec3 &normal)
{
	vec3 v10, v20;
	vec3 t10, t20;
	sub(v10, v1, v0);
	sub(v20, v2, v0);
	t10.y = t1.x - t0.x;
	t20.y = t2.x - t0.x;
	t10.z = t1.y - t0.y;
	t20.z = t2.y - t0.y;
	vec3 tangent, binormal;
	orthoBasis(normal, tangent, binormal);
	if (t10.y != 0.0f || t10.z != 0.0f || t20.y != 0.0f || t20.z != 0.0f)
	{
		vec3 normal;
		for (int i = 0; i < 3; i++)
		{
			t10.x = v10[i];
			t20.x = v20[i];
			cross(normal, t10, t20);
			if (abs(normal.x) > Consts::EPS)
			{
				tangent[i] = -normal.y / normal.x;
				binormal[i] = -normal.z / normal.x;
			}
		}
	}
	tangent.normalize();
	binormal.normalize();

	float sign = 1.0f;
	if (dot(normal, cross(tangent, binormal)) > 0.0f)
	{
		cross(binormal, normal, tangent);
		cross(tangent, binormal, normal);
		tangent.normalize();
	} else
	{
		cross(binormal, tangent, normal);
		cross(tangent, normal, binormal);
		tangent.normalize();
		sign = -1.0f;
	}

	return vec4(tangent, sign);
}
UNIGINE_INLINE vec4 triangleTangent(const vec3 &v0, const vec3 &v1, const vec3 &v2, const vec2 &t0, const vec2 &t1, const vec2 &t2)
{
	return triangleTangent(v0, v1, v2, t0, t1, t2, triangleNormal(v0, v1, v2));
}

//////////////////////////////////////////////////////////////////////////
// Polygon parameters
//////////////////////////////////////////////////////////////////////////

namespace
{
template <class Type>
UNIGINE_INLINE vec4 polygon_plane(const vec3 *vertex, Type *indices, int num_indices)
{
	float area = 0.0f;
	vec3 v10, v20, normal;
	vec4 plane = vec4_zero;
	for (int i = 2; i < num_indices; i++)
	{
		const vec3 &v0 = vertex[indices[i - 2]];
		const vec3 &v1 = vertex[indices[i - 1]];
		const vec3 &v2 = vertex[indices[i]];
		cross(normal, sub(v10, v1, v0), sub(v20, v2, v0));
		float a = length(normal);
		if (area < a)
		{
			normal *= rcp(a);
			plane.set(normal, -dot(normal, v0));
			area = a;
		}
	}
	return plane;
}

template <class Type>
UNIGINE_INLINE dvec4 polygon_plane(const dvec3 *vertex, Type *indices, int num_indices)
{
	double area = 0.0;
	dvec3 v10, v20, normal;
	dvec4 plane = dvec4_zero;
	for (int i = 2; i < num_indices; i++)
	{
		const dvec3 &v0 = vertex[indices[i - 2]];
		const dvec3 &v1 = vertex[indices[i - 1]];
		const dvec3 &v2 = vertex[indices[i]];
		cross(normal, sub(v10, v1, v0), sub(v20, v2, v0));
		double a = length(normal);
		if (area < a)
		{
			normal *= rcp(a);
			plane.set(normal, -dot(normal, v0));
			area = a;
		}
	}
	return plane;
}
}

UNIGINE_INLINE vec4 polygonPlane(const vec3 *vertex, const int *indices, int num_indices)
{
	return polygon_plane(vertex, indices, num_indices);
}
UNIGINE_INLINE dvec4 polygonPlane(const dvec3 *vertex, const int *indices, int num_indices)
{
	return polygon_plane(vertex, indices, num_indices);
}
UNIGINE_INLINE vec4 polygonPlane(const vec3 *vertex, const unsigned short *indices, int num_indices)
{
	return polygon_plane(vertex, indices, num_indices);
}
UNIGINE_INLINE dvec4 polygonPlane(const dvec3 *vertex, const unsigned short *indices, int num_indices)
{
	return polygon_plane(vertex, indices, num_indices);
}

//////////////////////////////////////////////////////////////////////////
// Point tringle intersections
//////////////////////////////////////////////////////////////////////////

UNIGINE_INLINE bool pointTriangleInside(const vec3 &point, const vec3 &v0, const vec3 &v1, const vec3 &v2)
{
	vec3 axis, normal;
	vec3 v10, v20, pv0;
	sub(v10, v1, v0);
	sub(v20, v2, v0);
	cross(normal, v10, v20);
	cross(axis, normal, v20);
	float det = dot(v10, axis);
	if (det > 0.0f)
	{
		sub(pv0, point, v0);
		float s = dot(pv0, axis);
		if (s < 0.0f || s > det)
			return 0;
		cross(axis, pv0, v10);
		float t = dot(normal, axis);
		if (t < 0.0f || t + s > det)
			return 0;
		return 1;
	} else if (det < 0.0f)
	{
		sub(pv0, point, v0);
		float s = dot(pv0, axis);
		if (s > 0.0f || s < det)
			return 0;
		cross(axis, pv0, v10);
		float t = dot(normal, axis);
		if (t > 0.0f || t + s < det)
			return 0;
		return 1;
	}
	return 0;
}
UNIGINE_INLINE bool pointTriangleInside(const dvec3 &point, const dvec3 &v0, const dvec3 &v1, const dvec3 &v2)
{
	dvec3 axis, normal;
	dvec3 v10, v20, pv0;
	sub(v10, v1, v0);
	sub(v20, v2, v0);
	cross(normal, v10, v20);
	cross(axis, normal, v20);
	double det = dot(v10, axis);
	if (det > 0.0)
	{
		sub(pv0, point, v0);
		double s = dot(pv0, axis);
		if (s < 0.0 || s > det)
			return 0;
		cross(axis, pv0, v10);
		double t = dot(normal, axis);
		if (t < 0.0 || t + s > det)
			return 0;
		return 1;
	} else if (det < 0.0)
	{
		sub(pv0, point, v0);
		double s = dot(pv0, axis);
		if (s > 0.0 || s < det)
			return 0;
		cross(axis, pv0, v10);
		double t = dot(normal, axis);
		if (t > 0.0 || t + s < det)
			return 0;
		return 1;
	}
	return 0;
}

UNIGINE_INLINE bool pointTriangleInside(const vec3 &point, const vec3 &v0, const vec3 &v1, const vec3 &v2, const vec3 &normal)
{
	vec3 axis;
	vec3 v10, v20, pv0;
	sub(v10, v1, v0);
	sub(v20, v2, v0);
	cross(axis, normal, v20);
	float det = dot(v10, axis);
	if (det > 0.0f)
	{
		sub(pv0, point, v0);
		float s = dot(pv0, axis);
		if (s < 0.0f || s > det)
			return 0;
		cross(axis, pv0, v10);
		float t = dot(normal, axis);
		if (t < 0.0f || t + s > det)
			return 0;
		return 1;
	} else if (det < 0.0f)
	{
		sub(pv0, point, v0);
		float s = dot(pv0, axis);
		if (s > 0.0f || s < det)
			return 0;
		cross(axis, pv0, v10);
		float t = dot(normal, axis);
		if (t > 0.0f || t + s < det)
			return 0;
		return 1;
	}
	return 0;
}
UNIGINE_INLINE bool pointTriangleInside(const dvec3 &point, const dvec3 &v0, const dvec3 &v1, const dvec3 &v2, const dvec3 &normal)
{
	dvec3 axis;
	dvec3 v10, v20, pv0;
	sub(v10, v1, v0);
	sub(v20, v2, v0);
	cross(axis, normal, v20);
	double det = dot(v10, axis);
	if (det > 0.0)
	{
		sub(pv0, point, v0);
		double s = dot(pv0, axis);
		if (s < 0.0 || s > det)
			return 0;
		cross(axis, pv0, v10);
		double t = dot(normal, axis);
		if (t < 0.0 || t + s > det)
			return 0;
		return 1;
	} else if (det < 0.0)
	{
		sub(pv0, point, v0);
		double s = dot(pv0, axis);
		if (s > 0.0 || s < det)
			return 0;
		cross(axis, pv0, v10);
		double t = dot(normal, axis);
		if (t > 0.0 || t + s < det)
			return 0;
		return 1;
	}
	return 0;
}

UNIGINE_INLINE float pointTriangleDistance(const vec3 &point, const vec3 &v0, const vec3 &v1, const vec3 &v2, const vec4 &plane)
{
	vec3 v10, v20, pv0;
	sub(v10, v1, v0);
	sub(v20, v2, v0);

	vec3 axis;
	cross(axis, plane.xyz, v20);

	float det = dot(v10, axis);
	if (det > 0.0f)
	{
		sub(pv0, point, v0);
		float s = dot(pv0, axis);
		if (s >= 0.0f && s <= det)
		{
			cross(axis, pv0, v10);
			float t = dot(plane.xyz, axis);
			if (t >= 0.0f && t + s <= det)
				return abs(dot(plane, point));
		}
	} else if (det < 0.0f)
	{
		sub(pv0, point, v0);
		float s = dot(pv0, axis);
		if (s <= 0.0f && s >= det)
		{
			cross(axis, pv0, v10);
			float t = dot(plane.xyz, axis);
			if (t <= 0.0f && t + s >= det)
				return abs(dot(plane, point));
		}
	}
	vec3 v21;
	sub(v21, v2, v1);
	float k0 = saturate(dot(v10, sub(pv0, point, v0)) * rcp(length2(v10)));
	float k1 = saturate(dot(v20, sub(pv0, point, v0)) * rcp(length2(v20)));
	float k2 = saturate(dot(v21, sub(pv0, point, v1)) * rcp(length2(v21)));
	k0 = length2(sub(pv0, point, mad(pv0, v10, k0, v0)));
	k1 = length2(sub(pv0, point, mad(pv0, v20, k1, v0)));
	k2 = length2(sub(pv0, point, mad(pv0, v21, k2, v1)));
	return fsqrt(min(k0, min(k1, k2)));
}
UNIGINE_INLINE double pointTriangleDistance(const dvec3 &point, const dvec3 &v0, const dvec3 &v1, const dvec3 &v2, const dvec4 &plane)
{
	dvec3 v10, v20, pv0;
	sub(v10, v1, v0);
	sub(v20, v2, v0);

	dvec3 axis;
	cross(axis, plane.xyz, v20);

	double det = dot(v10, axis);
	if (det > 0.0)
	{
		sub(pv0, point, v0);
		double s = dot(pv0, axis);
		if (s >= 0.0 && s <= det)
		{
			cross(axis, pv0, v10);
			double t = dot(plane.xyz, axis);
			if (t >= 0.0 && t + s <= det)
				return abs(dot(plane, point));
		}
	} else if (det < 0.0)
	{
		sub(pv0, point, v0);
		double s = dot(pv0, axis);
		if (s <= 0.0 && s >= det)
		{
			cross(axis, pv0, v10);
			double t = dot(plane.xyz, axis);
			if (t <= 0.0 && t + s >= det)
				return abs(dot(plane, point));
		}
	}
	dvec3 v21;
	sub(v21, v2, v1);
	double k0 = saturate(dot(v10, sub(pv0, point, v0)) * rcp(length2(v10)));
	double k1 = saturate(dot(v20, sub(pv0, point, v0)) * rcp(length2(v20)));
	double k2 = saturate(dot(v21, sub(pv0, point, v1)) * rcp(length2(v21)));
	k0 = length2(sub(pv0, point, mad(pv0, v10, k0, v0)));
	k1 = length2(sub(pv0, point, mad(pv0, v20, k1, v0)));
	k2 = length2(sub(pv0, point, mad(pv0, v21, k2, v1)));
	return dsqrt(min(k0, min(k1, k2)));
}

UNIGINE_INLINE void pointTriangleCoordinates(const vec2 &point, const vec2 &v0, const vec2 &v1, const vec2 &v2, float &a, float &b)
{
	vec2 v20, v10, v0p;
	sub(v20, v2, v0);
	sub(v10, v1, v0);
	float area = abs(v20.x * v10.y - v20.y * v10.x);
	if (area < Consts::EPS)
	{
		a = 0.0f;
		b = 0.0f;
		return;
	}
	sub(v20, v2, point);
	sub(v10, v1, point);
	sub(v0p, v0, point);
	float iarea = 1.0f / area;
	a = abs(v20.x * v0p.y - v20.y * v0p.x) * iarea;
	b = abs(v10.x * v0p.y - v10.y * v0p.x) * iarea;
}
UNIGINE_INLINE void pointTriangleCoordinates(const vec3 &point, const vec3 &v0, const vec3 &v1, const vec3 &v2, float &a, float &b)
{
	vec3 v, v20, v10, v0p;
	sub(v20, v2, v0);
	sub(v10, v1, v0);
	float area = length(cross(v, v20, v10));
	if (area < Consts::EPS)
	{
		a = 0.0f;
		b = 0.0f;
		return;
	}
	sub(v20, v2, point);
	sub(v10, v1, point);
	sub(v0p, v0, point);
	float iarea = 1.0f / area;
	a = length(cross(v, v20, v0p)) * iarea;
	b = length(cross(v, v10, v0p)) * iarea;
}
UNIGINE_INLINE void pointTriangleCoordinates(const dvec3 &point, const dvec3 &v0, const dvec3 &v1, const dvec3 &v2, double &a, double &b)
{
	dvec3 v, v20, v10, v0p;
	sub(v20, v2, v0);
	sub(v10, v1, v0);
	double area = length(cross(v, v20, v10));
	if (area < Consts::EPS)
	{
		a = 0.0;
		b = 0.0;
		return;
	}
	sub(v20, v2, point);
	sub(v10, v1, point);
	sub(v0p, v0, point);
	double iarea = 1.0 / area;
	a = length(cross(v, v20, v0p)) * iarea;
	b = length(cross(v, v10, v0p)) * iarea;
}

//////////////////////////////////////////////////////////////////////////
// Point polygon intersections
//////////////////////////////////////////////////////////////////////////

namespace
{
template <class Type>
UNIGINE_INLINE bool point_polygon_inside(const vec3 &point, const vec3 *vertex, Type *indices, int num_indices, const vec3 &normal)
{
	vec3 clip;
	vec3 v10, v20;
	for (int i = 0, j = 1; i < num_indices; i++, j++)
	{
		if (j == num_indices)
			j = 0;
		const vec3 &v0 = vertex[indices[i]];
		const vec3 &v1 = vertex[indices[j]];
		sub(v10, v1, v0);
		sub(v20, point, v0);
		cross(clip, v10, normal);
		float d = dot(clip, v20);
		if (d > 0.0f)
			return 0;
	}
	return 1;
}

template <class Type>
UNIGINE_INLINE bool point_polygon_inside(const dvec3 &point, const dvec3 *vertex, Type *indices, int num_indices, const dvec3 &normal)
{
	dvec3 clip;
	dvec3 v10, v20;
	for (int i = 0, j = 1; i < num_indices; i++, j++)
	{
		if (j == num_indices)
			j = 0;
		const dvec3 &v0 = vertex[indices[i]];
		const dvec3 &v1 = vertex[indices[j]];
		sub(v10, v1, v0);
		sub(v20, point, v0);
		cross(clip, v10, normal);
		double d = dot(clip, v20);
		if (d > 0.0)
			return 0;
	}
	return 1;
}
}

UNIGINE_INLINE bool pointPolygonInside(const vec3 &point, const vec3 *vertex, const int *indices, int num_indices, const vec3 &normal)
{
	return point_polygon_inside(point, vertex, indices, num_indices, normal);
}
UNIGINE_INLINE bool pointPolygonInside(const dvec3 &point, const dvec3 *vertex, const int *indices, int num_indices, const dvec3 &normal)
{
	return point_polygon_inside(point, vertex, indices, num_indices, normal);
}
UNIGINE_INLINE bool pointPolygonInside(const vec3 &point, const vec3 *vertex, const unsigned short *indices, int num_indices, const vec3 &normal)
{
	return point_polygon_inside(point, vertex, indices, num_indices, normal);
}
UNIGINE_INLINE bool pointPolygonInside(const dvec3 &point, const dvec3 *vertex, const unsigned short *indices, int num_indices, const dvec3 &normal)
{
	return point_polygon_inside(point, vertex, indices, num_indices, normal);
}

namespace
{
template <class Type>
UNIGINE_INLINE float point_polygon_distance(const vec3 &point, const vec3 *vertex, Type *indices, int num_indices, const vec4 &plane)
{
	vec3 clip;
	vec3 v10, v20;
	for (int i = 0, j = 1; i < num_indices; i++, j++)
	{
		if (j == num_indices)
			j = 0;
		const vec3 &v0 = vertex[indices[i]];
		const vec3 &v1 = vertex[indices[j]];
		sub(v10, v1, v0);
		sub(v20, point, v0);
		cross(clip, v10, plane.xyz);
		float d = dot(clip, v20);
		if (d > 0.0f)
		{
			float distance = Consts::INF;
			for (; i < num_indices; i++, j++)
			{
				if (j == num_indices)
					j = 0;
				const vec3 &v0 = vertex[indices[i]];
				const vec3 &v1 = vertex[indices[j]];
				sub(v10, v1, v0);
				sub(v20, point, v0);
				float k = saturate(dot(v10, v20) * rcp(length2(v10)));
				float d = length2(sub(v20, point, mad(v20, v10, k, v0)));
				if (distance > d)
					distance = d;
			}
			return fsqrt(distance);
		}
	}
	return abs(dot(plane, point));
}

template <class Type>
UNIGINE_INLINE double point_polygon_distance(const dvec3 &point, const dvec3 *vertex, Type *indices, int num_indices, const dvec4 &plane)
{
	dvec3 clip;
	dvec3 v10, v20;
	for (int i = 0, j = 1; i < num_indices; i++, j++)
	{
		if (j == num_indices)
			j = 0;
		const dvec3 &v0 = vertex[indices[i]];
		const dvec3 &v1 = vertex[indices[j]];
		sub(v10, v1, v0);
		sub(v20, point, v0);
		cross(clip, v10, plane.xyz);
		double d = dot(clip, v20);
		if (d > 0.0)
		{
			double distance = Consts::INF;
			for (; i < num_indices; i++, j++)
			{
				if (j == num_indices)
					j = 0;
				const dvec3 &v0 = vertex[indices[i]];
				const dvec3 &v1 = vertex[indices[j]];
				sub(v10, v1, v0);
				sub(v20, point, v0);
				double k = saturate(dot(v10, v20) * rcp(length2(v10)));
				double d = length2(sub(v20, point, mad(v20, v10, k, v0)));
				if (distance > d)
					distance = d;
			}
			return dsqrt(distance);
		}
	}
	return abs(dot(plane, point));
}
}

UNIGINE_INLINE float pointPolygonDistance(const vec3 &point, const vec3 *vertex, const int *indices, int num_indices, const vec4 &plane)
{
	return point_polygon_distance(point, vertex, indices, num_indices, plane);
}
UNIGINE_INLINE double pointPolygonDistance(const dvec3 &point, const dvec3 *vertex, const int *indices, int num_indices, const dvec4 &plane)
{
	return point_polygon_distance(point, vertex, indices, num_indices, plane);
}
UNIGINE_INLINE float pointPolygonDistance(const vec3 &point, const vec3 *vertex, const unsigned short *indices, int num_indices, const vec4 &plane)
{
	return point_polygon_distance(point, vertex, indices, num_indices, plane);
}
UNIGINE_INLINE double pointPolygonDistance(const dvec3 &point, const dvec3 *vertex, const unsigned short *indices, int num_indices, const dvec4 &plane)
{
	return point_polygon_distance(point, vertex, indices, num_indices, plane);
}


//////////////////////////////////////////////////////////////////////////
// Bounding box intersections
//////////////////////////////////////////////////////////////////////////

UNIGINE_INLINE bool rayBoundBoxIntersection(const vec3 &point, const vec3 &direction, const vec3 &min, const vec3 &max)
{
	vec3 idirection;
	div(idirection, vec3_one, direction);

	vec3 t0;
	sub(t0, min, point);
	mul(t0, t0, idirection);

	vec3 t1;
	sub(t1, max, point);
	mul(t1, t1, idirection);

	vec3 mn, mx;
	Math::min(mn, t0, t1);
	Math::max(mx, t0, t1);
	if (Math::abs(direction.x) < Consts::EPS)
	{
		if (min.x <= point.x && point.x <= max.x)
		{
			mn.x = -Consts::INF;
			mx.x = Consts::INF;
		} else
			return false;
	}
	if (Math::abs(direction.y) < Consts::EPS)
	{
		if (min.y <= point.y && point.y <= max.y)
		{
			mn.y = -Consts::INF;
			mx.y = Consts::INF;
		} else
			return false;
	}
	if (Math::abs(direction.z) < Consts::EPS)
	{
		if (min.z <= point.z && point.z <= max.z)
		{
			mn.z = -Consts::INF;
			mx.z = Consts::INF;
		} else
			return false;
	}
	float tmin = Math::max(Math::max(mn.x, mn.y), mn.z);
	float tmax = Math::min(Math::min(mx.x, mx.y), mx.z);
	return tmax >= 0 && tmin <= tmax;
}
UNIGINE_INLINE bool rayBoundBoxIntersection(const dvec3 &point, const dvec3 &direction, const dvec3 &min, const dvec3 &max)
{
	dvec3 idirection;
	div(idirection, dvec3_one, direction);

	dvec3 t0;
	sub(t0, min, point);
	mul(t0, t0, idirection);

	dvec3 t1;
	sub(t1, max, point);
	mul(t1, t1, idirection);

	dvec3 mn, mx;
	Math::min(mn, t0, t1);
	Math::max(mx, t0, t1);
	if (Math::abs(direction.x) < Consts::EPS)
	{
		if (min.x <= point.x && point.x <= max.x)
		{
			mn.x = -Consts::INF;
			mx.x = Consts::INF;
		} else
			return false;
	}
	if (Math::abs(direction.y) < Consts::EPS)
	{
		if (min.y <= point.y && point.y <= max.y)
		{
			mn.y = -Consts::INF;
			mx.y = Consts::INF;
		} else
			return false;
	}
	if (Math::abs(direction.z) < Consts::EPS)
	{
		if (min.z <= point.z && point.z <= max.z)
		{
			mn.z = -Consts::INF;
			mx.z = Consts::INF;
		} else
			return false;
	}
	double tmin = Math::max(Math::max(mn.x, mn.y), mn.z);
	double tmax = Math::min(Math::min(mx.x, mx.y), mx.z);
	return tmax >= 0 && tmin <= tmax;
}

UNIGINE_INLINE bool irayBoundBoxIntersection(const vec3 &point, const vec3 &idirection, const vec3 &min, const vec3 &max)
{
	vec3 t0;
	sub(t0, min, point);
	mul(t0, t0, idirection);

	vec3 t1;
	sub(t1, max, point);
	mul(t1, t1, idirection);

	vec3 mn, mx;
	Math::min(mn, t0, t1);
	Math::max(mx, t0, t1);
	float tmin = Math::max(Math::max(mn.x, mn.y), mn.z);
	float tmax = Math::min(Math::min(mx.x, mx.y), mx.z);
	return tmax >= 0 && tmin <= tmax;
}
UNIGINE_INLINE bool irayBoundBoxIntersection(const dvec3 &point, const dvec3 &idirection, const dvec3 &min, const dvec3 &max)
{
	dvec3 t0;
	sub(t0, min, point);
	mul(t0, t0, idirection);

	dvec3 t1;
	sub(t1, max, point);
	mul(t1, t1, idirection);

	dvec3 mn, mx;
	Math::min(mn, t0, t1);
	Math::max(mx, t0, t1);
	double tmin = Math::max(Math::max(mn.x, mn.y), mn.z);
	double tmax = Math::min(Math::min(mx.x, mx.y), mx.z);
	return tmax >= 0 && tmin <= tmax;
}

UNIGINE_INLINE bool rayTriangleIntersection(const vec3& point, const vec3& direction, const vec3& v0, const vec3& v1, const vec3& v2)
{
	vec3 sxyz;
	vec3 a;
	vec3 b;
	vec3 c;
	vec3 sa;
	vec3 sb;
	vec3 sc;

#ifdef USE_SSE

#define WATERTIGHT(KX,KY,KZ)\
	sxyz.vec = _mm_div_ps(direction.vec, _mm_set_ps1(direction[KZ]));\
\
	a.vec = _mm_sub_ps(v0.vec, point.vec);\
	b.vec = _mm_sub_ps(v1.vec, point.vec);\
	c.vec = _mm_sub_ps(v2.vec, point.vec);\
\
	sa.vec = _mm_sub_ps(a.vec, _mm_mul_ps(sxyz.vec, _mm_set_ps1(a[KZ])));\
	sb.vec = _mm_sub_ps(b.vec, _mm_mul_ps(sxyz.vec, _mm_set_ps1(b[KZ])));\
	sc.vec = _mm_sub_ps(c.vec, _mm_mul_ps(sxyz.vec, _mm_set_ps1(c[KZ])));\
\
	float u = sc[KX] * sb[KY] - sc[KY] * sb[KX];\
	float v = sa[KX] * sc[KY] - sa[KY] * sc[KX];\
	float w = sb[KX] * sa[KY] - sb[KY] * sa[KX];\
\
	return !((u < 0 || v < 0 || w < 0) && (u > 0 || v > 0 || w > 0));\
\

#else

#define WATERTIGHT(KX,KY,KZ)\
	sxyz = direction / direction[KZ];\
\
	a = v0 - point;\
	b = v1 - point;\
	c = v2 - point;\
\
	sa = a - sxyz * a[KZ];\
	sb = b - sxyz * b[KZ];\
	sc = c - sxyz * c[KZ];\
\
	float u = sc[KX] * sb[KY] - sc[KY] * sb[KX];\
	float v = sa[KX] * sc[KY] - sa[KY] * sc[KX];\
	float w = sb[KX] * sa[KY] - sb[KY] * sa[KX];\
\
	return !((u < 0 || v < 0 || w < 0) && (u > 0 || v > 0 || w > 0));\
\

#endif

	vec3 abs_dir = abs(direction);
	int k = abs_dir.y > abs_dir.x;
	if (abs_dir.z > abs_dir.y)
		k = 2;

	if (k == 0)
	{
		WATERTIGHT(1, 2, 0)
	}
	if (k == 1)
	{
		WATERTIGHT(2, 0, 1)
	}
	{
		WATERTIGHT(0, 1, 2)
	}

#undef WATERTIGHT
}

UNIGINE_INLINE bool rayTriangleIntersection(const dvec3& point, const dvec3& direction, const dvec3& v0, const dvec3& v1, const dvec3& v2)
{
#define WATERTIGHT(KX,KY,KZ)\
	double sx = direction[KX] / direction[KZ];\
	double sy = direction[KY] / direction[KZ];\
\
	dvec3 a;\
	dvec3 b;\
	dvec3 c;\
\
	a = v0 - point;\
	b = v1 - point;\
	c = v2 - point;\
\
	double ax = a[KX] - sx * a[KZ];\
	double ay = a[KY] - sy * a[KZ];\
	double bx = b[KX] - sx * b[KZ];\
	double by = b[KY] - sy * b[KZ];\
	double cx = c[KX] - sx * c[KZ];\
	double cy = c[KY] - sy * c[KZ];\
\
	double u = cx * by - cy * bx;\
	double v = ax * cy - ay * cx;\
	double w = bx * ay - by * ax;\
	return !((u < 0 || v < 0 || w < 0) && (u > 0 || v > 0 || w > 0));\
\

	dvec3 abs_dir = abs(direction);
	int k = abs_dir.y > abs_dir.x;
	if (abs_dir.z > abs_dir.y)
		k = 2;

	if (k == 0)
	{
		WATERTIGHT(1, 2, 0)
	}
	if (k == 1)
	{
		WATERTIGHT(2, 0, 1)
	}
	{
		WATERTIGHT(0, 1, 2)
	}

#undef WATERTIGHT
}

//////////////////////////////////////////////////////////////////////////
// Plane intersections
//////////////////////////////////////////////////////////////////////////

UNIGINE_INLINE bool linePlaneIntersection(const vec3 &p0, const vec3 &p1, const vec4 &plane)
{
	return abs(dot(p1 - p0, plane.xyz)) >= Consts::EPS;
}
UNIGINE_INLINE bool linePlaneIntersection(const dvec3 &p0, const dvec3 &p1, const dvec4 &plane)
{
	return abs(dot(p1 - p0, plane.xyz)) >= Consts::EPS;
}
UNIGINE_INLINE bool linePlaneIntersection(const vec3 &p0, const vec3 &p1, const vec3 &plane_point, const vec3 &plane_normal)
{
	return linePlaneIntersection(p0, p1, vec4(plane_normal, -dot(plane_point, plane_normal)));
}
UNIGINE_INLINE bool linePlaneIntersection(const dvec3 &p0, const dvec3 &p1, const dvec3 &plane_point, const dvec3 &plane_normal)
{
	return linePlaneIntersection(p0, p1, dvec4(plane_normal, -dot(plane_point, plane_normal)));
}
UNIGINE_INLINE bool linePlaneIntersection(const vec3 &p0, const vec3 &p1, const vec4 &plane, vec3 &ret)
{
	vec3 d = p1 - p0;

	float r = dot(d, plane.xyz);
	if (abs(r) < Consts::EPS)
		return false;

	float t = -dot(plane, p0) / r;
	if (t < 0.0f)
		return false;

	ret = p0 + d * t;
	return true;
}
UNIGINE_INLINE bool linePlaneIntersection(const dvec3 &p0, const dvec3 &p1, const dvec4 &plane, dvec3 &ret)
{
	dvec3 d = p1 - p0;

	double r = dot(d, plane.xyz);
	if (abs(r) < Consts::EPS)
		return false;

	double t = -dot(plane, p0) / r;
	if (t < 0.0)
		return false;

	ret = p0 + d * t;
	return true;
}
UNIGINE_INLINE bool linePlaneIntersection(const vec3 &p0, const vec3 &p1, const vec3 &plane_point, const vec3 &plane_normal, vec3 &ret)
{
	return linePlaneIntersection(p0, p1, vec4(plane_normal, -dot(plane_point, plane_normal)), ret);
}
UNIGINE_INLINE bool linePlaneIntersection(const dvec3 &p0, const dvec3 &p1, const dvec3 &plane_point, const dvec3 &plane_normal, dvec3 &ret)
{
	return linePlaneIntersection(p0, p1, dvec4(plane_normal, -dot(plane_point, plane_normal)), ret);
}

UNIGINE_INLINE bool rayPlaneIntersection(const vec3 &point, const vec3 &direction, const vec4 &plane)
{
	float r = dot(direction, plane.xyz);
	if (abs(r) < Consts::EPS)
		return false;
	float t = -(plane.w + dot(point, plane.xyz)) / r;
	return t >= 0.0f;
}
UNIGINE_INLINE bool rayPlaneIntersection(const dvec3 &point, const dvec3 &direction, const dvec4 &plane)
{
	double r = dot(direction, plane.xyz);
	if (abs(r) < Consts::EPS)
		return false;
	double t = -(plane.w + dot(point, plane.xyz)) / r;
	return t >= 0.0;
}
UNIGINE_INLINE bool rayPlaneIntersection(const vec3 &point, const vec3 &direction, const vec3 &plane_point, const vec3 &plane_normal)
{
	return rayPlaneIntersection(point, direction, vec4(plane_normal, -dot(plane_point, plane_normal)));
}
UNIGINE_INLINE bool rayPlaneIntersection(const dvec3 &point, const dvec3 &direction, const dvec3 &plane_point, const dvec3 &plane_normal)
{
	return rayPlaneIntersection(point, direction, dvec4(plane_normal, -dot(plane_point, plane_normal)));
}
UNIGINE_INLINE bool rayPlaneIntersection(const vec3 &point, const vec3 &direction, const vec4 &plane, vec3 &ret)
{
	float r = dot(direction, plane.xyz);
	if (abs(r) < Consts::EPS)
		return false;

	float t = -(plane.w + dot(point, plane.xyz)) / r;
	if (t < 0.0f)
		return false;

	ret = point + direction * t;
	return true;
}

UNIGINE_INLINE bool rayPlaneIntersection(const dvec3 &point, const dvec3 &direction, const dvec4 &plane, dvec3 &ret)
{
	double r = dot(direction, plane.xyz);
	if (abs(r) < Consts::EPS)
		return false;

	double t = -(plane.w + dot(point, plane.xyz)) / r;
	if (t < 0.0)
		return false;

	ret = point + direction * t;
	return true;
}
UNIGINE_INLINE bool rayPlaneIntersection(const vec3 &point, const vec3 &direction, const vec3 &plane_point, const vec3 &plane_normal, vec3 &ret)
{
	return rayPlaneIntersection(point, direction, vec4(plane_normal, -dot(plane_point, plane_normal)), ret);
}
UNIGINE_INLINE bool rayPlaneIntersection(const dvec3 &point, const dvec3 &direction, const dvec3 &plane_point, const dvec3 &plane_normal, dvec3 &ret)
{
	return rayPlaneIntersection(point, direction, dvec4(plane_normal, -dot(plane_point, plane_normal)), ret);
}

UNIGINE_INLINE bool segmentPlaneIntersection(const vec3 &p0, const vec3 &p1, const vec4 &plane)
{
	float r = dot(p1 - p0, plane.xyz);
	if (abs(r) < Consts::EPS)
		return false;
	float t = -(plane.w + dot(p0, plane.xyz)) / r;
	return t >= 0.0f && t <= 1.0f;
}
UNIGINE_INLINE bool segmentPlaneIntersection(const dvec3 &p0, const dvec3 &p1, const dvec4 &plane)
{
	double r = dot(p1 - p0, plane.xyz);
	if (abs(r) < Consts::EPS)
		return false;
	double t = -(plane.w + dot(p0, plane.xyz)) / r;
	return t >= 0.0 && t <= 1.0;
}
UNIGINE_INLINE bool segmentPlaneIntersection(const vec3 &p0, const vec3 &p1, const vec3 &plane_point, const vec3 &plane_normal)
{
	vec4 plane(plane_normal, -dot(plane_point, plane_normal));
	return segmentPlaneIntersection(p0, p1, plane);
}
UNIGINE_INLINE bool segmentPlaneIntersection(const dvec3 &p0, const dvec3 &p1, const dvec3 &plane_point, const dvec3 &plane_normal)
{
	dvec4 plane(plane_normal, -dot(plane_point, plane_normal));
	return segmentPlaneIntersection(p0, p1, plane);
}
UNIGINE_INLINE bool segmentPlaneIntersection(const vec3 &p0, const vec3 &p1, const vec4 &plane, vec3 &ret)
{
	vec3 d = p1 - p0;

	float r = dot(d, plane.xyz);
	if (abs(r) < Consts::EPS)
		return false;

	float t = -(plane.w + dot(p0, plane.xyz)) / r;
	if (t < 0.0f || t > 1.0f)
		return false;

	ret = p0 + d * t;
	return true;
}
UNIGINE_INLINE bool segmentPlaneIntersection(const dvec3 &p0, const dvec3 &p1, const dvec4 &plane, dvec3 &ret)
{
	dvec3 d = p1 - p0;

	double r = dot(d, plane.xyz);
	if (abs(r) < Consts::EPS)
		return false;

	double t = -(plane.w + dot(p0, plane.xyz)) / r;
	if (t < 0.0 || t > 1.0)
		return false;

	ret = p0 + d * t;
	return true;
}
UNIGINE_INLINE bool segmentPlaneIntersection(const vec3 &p0, const vec3 &p1, const vec3 &plane_point, const vec3 &plane_normal, vec3 &ret)
{
	return segmentPlaneIntersection(p0, p1, vec4(plane_normal, -dot(plane_point, plane_normal)), ret);
}
UNIGINE_INLINE bool segmentPlaneIntersection(const dvec3 &p0, const dvec3 &p1, const dvec3 &plane_point, const dvec3 &plane_normal, dvec3 &ret)
{
	return segmentPlaneIntersection(p0, p1, dvec4(plane_normal, -dot(plane_point, plane_normal)), ret);
}


//////////////////////////////////////////////////////////////////////////
// Closest points
//////////////////////////////////////////////////////////////////////////

UNIGINE_INLINE void getClosestPointOnLine(const vec3 &point, const vec3 &p0, const vec3 &p1, vec3 &ret)
{
	vec3 direction;
	sub(direction, p1, p0);
	float d = direction.length2();
	if (d < Consts::EPS)
	{
		ret = p0;
		return;
	}
	vec3 v0;
	float k = dot(direction, sub(v0, point, p0)) * rcp(d);
	mad(ret, direction, saturate(k), p0);
}
UNIGINE_INLINE void getClosestPointOnLine(const dvec3 &point, const dvec3 &p0, const dvec3 &p1, dvec3 &ret)
{
	dvec3 direction;
	sub(direction, p1, p0);
	double d = direction.length2();
	if (d < Consts::EPS)
	{
		ret = p0;
		return;
	}
	dvec3 v0;
	double k = dot(direction, sub(v0, point, p0)) * rcp(d);
	mad(ret, direction, saturate(k), p0);
}

UNIGINE_INLINE bool getClosestPointOnTriangle(const vec3 &point, const vec3 &v0, const vec3 &v1, const vec3 &v2, vec3 &ret)
{
	vec3 axis, normal;
	vec3 v10, v20, pv0;
	sub(v10, v1, v0);
	sub(v20, v2, v0);
	cross(normal, v10, v20);
	cross(axis, normal, v20);
	float det = dot(v10, axis);
	if (det > 0.0f)
	{
		sub(pv0, point, v0);
		float s = dot(pv0, axis);
		if (s >= 0.0f && s <= det)
		{
			cross(axis, pv0, v10);
			float t = dot(normal, axis);
			if (t >= 0.0f && t + s <= det)
			{
				mad(ret, normal, dot(normal, v0) - dot(normal, point), point);
				return true;
			}
		}
	} else if (det < 0.0f)
	{
		sub(pv0, point, v0);
		float s = dot(pv0, axis);
		if (s <= 0.0f && s >= det)
		{
			cross(axis, pv0, v10);
			float t = dot(normal, axis);
			if (t <= 0.0f && t + s >= det)
			{
				mad(ret, normal, dot(normal, v0) - dot(normal, point), point);
				return true;
			}
		}
	}
	vec3 v21, p0, p1;
	sub(v21, v2, v1);
	float k0 = saturate(dot(v10, sub(pv0, point, v0)) * rcp(length2(v10)));
	float k1 = saturate(dot(v20, sub(pv0, point, v0)) * rcp(length2(v20)));
	float k2 = saturate(dot(v21, sub(pv0, point, v1)) * rcp(length2(v21)));
	k0 = length2(sub(pv0, point, mad(p0, v10, k0, v0)));
	k1 = length2(sub(pv0, point, mad(p1, v20, k1, v0)));
	k2 = length2(sub(pv0, point, mad(ret, v21, k2, v1)));
	if (k0 < k1)
	{
		if (k0 < k2)
			ret = p0;
	} else if (k1 < k2)
		ret = p1;

	return false;
}
UNIGINE_INLINE bool getClosestPointOnTriangle(const dvec3 &point, const dvec3 &v0, const dvec3 &v1, const dvec3 &v2, dvec3 &ret)
{
	dvec3 axis, normal;
	dvec3 v10, v20, pv0;
	sub(v10, v1, v0);
	sub(v20, v2, v0);
	cross(normal, v10, v20);
	cross(axis, normal, v20);
	double det = dot(v10, axis);
	if (det > 0.0)
	{
		sub(pv0, point, v0);
		double s = dot(pv0, axis);
		if (s >= 0.0 && s <= det)
		{
			cross(axis, pv0, v10);
			double t = dot(normal, axis);
			if (t >= 0.0 && t + s <= det)
			{
				mad(ret, normal, dot(normal, v0) - dot(normal, point), point);
				return true;
			}
		}
	} else if (det < 0.0)
	{
		sub(pv0, point, v0);
		double s = dot(pv0, axis);
		if (s <= 0.0 && s >= det)
		{
			cross(axis, pv0, v10);
			double t = dot(normal, axis);
			if (t <= 0.0 && t + s >= det)
			{
				mad(ret, normal, dot(normal, v0) - dot(normal, point), point);
				return true;
			}
		}
	}
	dvec3 v21, p0, p1;
	sub(v21, v2, v1);
	double k0 = saturate(dot(v10, sub(pv0, point, v0)) * rcp(length2(v10)));
	double k1 = saturate(dot(v20, sub(pv0, point, v0)) * rcp(length2(v20)));
	double k2 = saturate(dot(v21, sub(pv0, point, v1)) * rcp(length2(v21)));
	k0 = length2(sub(pv0, point, mad(p0, v10, k0, v0)));
	k1 = length2(sub(pv0, point, mad(p1, v20, k1, v0)));
	k2 = length2(sub(pv0, point, mad(ret, v21, k2, v1)));
	if (k0 < k1)
	{
		if (k0 < k2)
			ret = p0;
	} else if (k1 < k2)
			ret = p1;

	return false;
}

UNIGINE_INLINE bool getClosestPointsOnLines(const vec3 &p00, const vec3 &p01, const vec3 &p10, const vec3 &p11, vec3 &ret_0, vec3 &ret_1)
{
	if (p00 == p01)
	{
		ret_0 = p00;
		getClosestPointOnLine(p00, p10, p11, ret_1);
		return false;
	}
	if (p10 == p11)
	{
		ret_1 = p10;
		getClosestPointOnLine(p10, p00, p01, ret_0);
		return false;
	}
	vec3 v0, v1, v2;
	sub(v0, p01, p00);
	sub(v1, p11, p10);
	sub(v2, p10, p00);
	float d20 = dot(v2, v0);
	float d21 = dot(v2, v1);
	if (d20 < Consts::EPS && d21 > -Consts::EPS)
	{
		ret_0 = p00;
		ret_1 = p10;
		return false;
	}
	vec3 v3;
	sub(v3, p11, p01);
	float d30 = dot(v3, v0);
	float d31 = dot(v3, v1);
	if (d30 > -Consts::EPS && d31 < Consts::EPS)
	{
		ret_0 = p01;
		ret_1 = p11;
		return false;
	}
	vec3 v4;
	sub(v4, p11, p00);
	float d40 = dot(v4, v0);
	float d41 = dot(v4, v1);
	if (d40 < Consts::EPS && d41 < Consts::EPS)
	{
		ret_0 = p00;
		ret_1 = p11;
		return false;
	}
	vec3 v5;
	sub(v5, p10, p01);
	float d50 = dot(v5, v0);
	float d51 = dot(v5, v1);
	if (d50 > -Consts::EPS && d51 > -Consts::EPS)
	{
		ret_0 = p01;
		ret_1 = p10;
		return false;
	}
	vec3 v6;
	float d00 = dot(v0, v0);
	if (d20 > -Consts::EPS && d50 < Consts::EPS)
	{
		float k = d20 / d00;
		if (dot(mad(v6, v0, -k, v2), v1) > -Consts::EPS)
		{
			mad(ret_0, v0, k, p00);
			ret_1 = p10;
			return false;
		}
	}
	if (d40 > -Consts::EPS && d30 < Consts::EPS)
	{
		float k = d40 / d00;
		if (dot(mad(v6, v0, -k, v4), v1) < Consts::EPS)
		{
			mad(ret_0, v0, k, p00);
			ret_1 = p11;
			return false;
		}
	}
	float d11 = dot(v1, v1);
	if (d21 < Consts::EPS && d41 > -Consts::EPS)
	{
		float k = -d21 / d11;
		if (dot(mad(v6, v1, k, v2), v0) < Consts::EPS)
		{
			ret_0 = p00;
			mad(ret_1, v1, k, p10);
			return false;
		}
	}
	if (d51 < Consts::EPS && d31 > -Consts::EPS)
	{
		float k = -d51 / d11;
		if (dot(mad(v6, v1, k, v5), v0) > -Consts::EPS)
		{
			ret_0 = p01;
			mad(ret_1, v1, k, p10);
			return false;
		}
	}
	float d10 = dot(v1, v0);
	float det = d10 * d10 - d00 * d11;
	if (abs(det) < Consts::EPS)
	{
		float distance = length2(v2);
		ret_0 = p00;
		ret_1 = p10;

		float d3 = length2(v3);
		if (distance > d3)
		{
			distance = d3;
			ret_0 = p01;
			ret_1 = p11;
		}
		float d4 = length2(v4);
		if (distance > d4)
		{
			distance = d4;
			ret_0 = p00;
			ret_1 = p11;
		}
		float d5 = length2(v5);
		if (distance > d5)
		{
			distance = d5;
			ret_0 = p01;
			ret_1 = p10;
		}
		return false;
	}
	float idet = rcp(det);
	mad(ret_0, v0, saturate((d10 * d21 - d11 * d20) * idet), p00);
	mad(ret_1, v1, saturate((d00 * d21 - d10 * d20) * idet), p10);
	return true;
}
UNIGINE_INLINE bool getClosestPointsOnLines(const dvec3 &p00, const dvec3 &p01, const dvec3 &p10, const dvec3 &p11, dvec3 &ret_0, dvec3 &ret_1)
{
	if (p00 == p01)
	{
		ret_0 = p00;
		getClosestPointOnLine(p00, p10, p11, ret_1);
		return false;
	}
	if (p10 == p11)
	{
		ret_1 = p10;
		getClosestPointOnLine(p10, p00, p01, ret_0);
		return false;
	}
	dvec3 v0, v1, v2;
	sub(v0, p01, p00);
	sub(v1, p11, p10);
	sub(v2, p10, p00);
	double d20 = dot(v2, v0);
	double d21 = dot(v2, v1);
	if (d20 < Consts::EPS && d21 > -Consts::EPS)
	{
		ret_0 = p00;
		ret_1 = p10;
		return false;
	}
	dvec3 v3;
	sub(v3, p11, p01);
	double d30 = dot(v3, v0);
	double d31 = dot(v3, v1);
	if (d30 > -Consts::EPS && d31 < Consts::EPS)
	{
		ret_0 = p01;
		ret_1 = p11;
		return false;
	}
	dvec3 v4;
	sub(v4, p11, p00);
	double d40 = dot(v4, v0);
	double d41 = dot(v4, v1);
	if (d40 < Consts::EPS && d41 < Consts::EPS)
	{
		ret_0 = p00;
		ret_1 = p11;
		return false;
	}
	dvec3 v5;
	sub(v5, p10, p01);
	double d50 = dot(v5, v0);
	double d51 = dot(v5, v1);
	if (d50 > -Consts::EPS && d51 > -Consts::EPS)
	{
		ret_0 = p01;
		ret_1 = p10;
		return false;
	}
	dvec3 v6;
	double d00 = dot(v0, v0);
	if (d20 > -Consts::EPS && d50 < Consts::EPS)
	{
		double k = d20 / d00;
		if (dot(mad(v6, v0, -k, v2), v1) > -Consts::EPS)
		{
			mad(ret_0, v0, k, p00);
			ret_1 = p10;
			return false;
		}
	}
	if (d40 > -Consts::EPS && d30 < Consts::EPS)
	{
		double k = d40 / d00;
		if (dot(mad(v6, v0, -k, v4), v1) < Consts::EPS)
		{
			mad(ret_0, v0, k, p00);
			ret_1 = p11;
			return false;
		}
	}
	double d11 = dot(v1, v1);
	if (d21 < Consts::EPS && d41 > -Consts::EPS)
	{
		double k = -d21 / d11;
		if (dot(mad(v6, v1, k, v2), v0) < Consts::EPS)
		{
			ret_0 = p00;
			mad(ret_1, v1, k, p10);
			return false;
		}
	}
	if (d51 < Consts::EPS && d31 > -Consts::EPS)
	{
		double k = -d51 / d11;
		if (dot(mad(v6, v1, k, v5), v0) > -Consts::EPS)
		{
			ret_0 = p01;
			mad(ret_1, v1, k, p10);
			return false;
		}
	}
	double d10 = dot(v1, v0);
	double det = d10 * d10 - d00 * d11;
	if (abs(det) < Consts::EPS)
	{
		double distance = length2(v2);
		ret_0 = p00;
		ret_1 = p10;

		double d3 = length2(v3);
		if (distance > d3)
		{
			distance = d3;
			ret_0 = p01;
			ret_1 = p11;
		}
		double d4 = length2(v4);
		if (distance > d4)
		{
			distance = d4;
			ret_0 = p00;
			ret_1 = p11;
		}
		double d5 = length2(v5);
		if (distance > d5)
		{
			distance = d5;
			ret_0 = p01;
			ret_1 = p10;
		}
		return false;
	}
	double idet = rcp(det);
	mad(ret_0, v0, saturate((d10 * d21 - d11 * d20) * idet), p00);
	mad(ret_1, v1, saturate((d00 * d21 - d10 * d20) * idet), p10);
	return true;
}

UNIGINE_INLINE vec3 projectOntoPlane(const vec3 &v, const vec3 &plane_normal)
{
	return v - plane_normal * dot(v, plane_normal);
}
UNIGINE_INLINE dvec3 projectOntoPlane(const dvec3 &v, const dvec3 &plane_normal)
{
	return v - plane_normal * dot(v, plane_normal);
}


//////////////////////////////////////////////////////////////////////////
// Triangle rasterization
//////////////////////////////////////////////////////////////////////////

template <class Vertex, class Shader>
UNIGINE_INLINE void triangleRasterize(const Vertex *vertex, int x0, int y0, int x1, int y1, Shader &shader)
{
	const Vertex *v0 = vertex + 0;
	const Vertex *v1 = vertex + 1;
	const Vertex *v2 = vertex + 2;
	if (v0->y > v1->y)
		std::swap(v0, v1);
	if (v0->y > v2->y)
		std::swap(v0, v2);
	if (v1->y > v2->y)
		std::swap(v1, v2);

	const Vertex *v[3] = {v0, v1, v2};

	int v0_y = toInt(ceil(v0->y));
	int v1_y = toInt(ceil(v1->y));
	int v2_y = toInt(ceil(v2->y));

	float v10_x = v1->x - v0->x;
	float v20_x = v2->x - v0->x;
	float v21_x = v2->x - v1->x;
	float v10_y = v1->y - v0->y;
	float v20_y = v2->y - v0->y;
	float v21_y = v2->y - v1->y;

	float det = v20_x * v10_y - v20_y * v10_x;
	if (abs(det) <= Consts::EPS)
		return;
	float idet = 1.0f / det;

	float ddx[2], ddy[2];

	if (abs(v20_y) > 0.0f && abs(v10_y) > 0.0f)
	{
		float iv20_y = 1.0f / v20_y;
		float iv10_y = 1.0f / v10_y;
		int y2 = clamp(v0_y, y0, y1);
		int y3 = clamp(v1_y, y0, y1);
		for (int y = y2; y < y3; y++)
		{
			int x2 = clamp(toInt(ceil(v0->x + v20_x * saturate((y - v0->y) * iv20_y))), x0, x1);
			int x3 = clamp(toInt(ceil(v0->x + v10_x * saturate((y - v0->y) * iv10_y))), x0, x1);
			if (x2 > x3)
				std::swap(x2, x3);
			for (int x = x2; x < x3; x++)
			{
				float a = ((y - v0->y) * v20_x - (x - v0->x) * v20_y) * idet;
				float b = ((x - v0->x) * v10_y - (y - v0->y) * v10_x) * idet;
				ddx[0] = -v20_y * idet;
				ddy[0] = v10_y * idet;
				ddx[1] = v20_x * idet;
				ddy[1] = -v10_x * idet;
				shader(v, x, y, a, b, ddx, ddy);
			}
		}
	}

	if (abs(v20_y) > 0.0f && abs(v21_y) > 0.0f)
	{
		float iv20_y = 1.0f / v20_y;
		float iv21_y = 1.0f / v21_y;
		int y2 = clamp(v1_y, y0, y1);
		int y3 = clamp(v2_y, y0, y1);
		for (int y = y2; y < y3; y++)
		{
			int x2 = clamp(toInt(ceil(v0->x + v20_x * saturate((y - v0->y) * iv20_y))), x0, x1);
			int x3 = clamp(toInt(ceil(v1->x + v21_x * saturate((y - v1->y) * iv21_y))), x0, x1);
			if (x2 > x3)
				std::swap(x2, x3);
			for (int x = x2; x < x3; x++)
			{
				float a = ((y - v0->y) * v20_x - (x - v0->x) * v20_y) * idet;
				float b = ((x - v0->x) * v10_y - (y - v0->y) * v10_x) * idet;
				ddx[0] = -v20_y * idet;
				ddy[0] = v10_y * idet;
				ddx[1] = v20_x * idet;
				ddy[1] = -v10_x * idet;
				shader(v, x, y, a, b, ddx, ddy);
			}
		}
	}
}

} // end namespace Math

} // end namespace Unigine
