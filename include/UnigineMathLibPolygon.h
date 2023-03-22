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
#include "UnigineVector.h"
#include "UnigineSort.h"
#include "UnigineMathLibCommon.h"
#include "UnigineMathLibVec3.h"
#include "UnigineMathLibVec4.h"

namespace Unigine
{
namespace Math
{

class Polygon
{
public:
	UNIGINE_INLINE void clear()
	{
		vertex.clear();
		indices.clear();
	}

	bool createConvex(const vec3 *v, int num_vertex, const vec3 &normal)
	{
		clear();

		if (num_vertex < 3)
			return false;

		// vertex buffer
		VectorStack<Vertex, NUM_VERTEX> buffer(num_vertex);
		for (int i = 0; i < num_vertex; i++)
		{
			buffer[i].xyz = v[i];
			buffer[i].index = i;
		}

		// optimize vertices
		VectorStack<int, NUM_VERTEX> vbuf(num_vertex);
		num_vertex = quickOptimize2(buffer.get(), vbuf.get(), num_vertex, vertex_compare);
		buffer.resize(num_vertex);

		// first vertex
		int index = 0;
		for (int i = 1; i < buffer.size(); i++)
		{
			if (buffer[index].xyz.x < buffer[i].xyz.x)
				continue;
			if (buffer[index].xyz.y < buffer[i].xyz.y)
				continue;
			if (buffer[index].xyz.z < buffer[i].xyz.z)
				continue;
			index = i;
		}
		vertex.append(buffer[index]);
		buffer.removeFast(index);

		// create convex polygon
		int iterations = buffer.size() * 2;
		while (iterations-- > 0)
		{
			int index = -1;
			float distance = 0.0f;
			const vec3 &v0 = vertex.last().xyz;
			for (int i = 0; i < buffer.size(); i++)
			{
				vec3 direction = buffer[i].xyz - v0;
				float d = length2(direction);
				if (distance > d)
					continue;
				vec3 left = cross(direction, normal);
				vec4 plane = vec4(left, -dot(left, v0));
				int is_convex = 1;
				for (int j = 0; j < buffer.size(); j++)
				{
					if (i != j && dot(plane, buffer[j].xyz) < -Consts::EPS)
					{
						is_convex = 0;
						break;
					}
				}
				if (is_convex == 0)
					continue;
				for (int j = 0; j < vertex.size() - 1; j++)
				{
					if (dot(plane, vertex[j].xyz) < -Consts::EPS)
					{
						is_convex = 0;
						break;
					}
				}
				if (is_convex == 0)
					continue;
				distance = d;
				index = i;
			}

			if (index == -1)
				break;

			vertex.append(buffer[index]);
			buffer.removeFast(index);
		}

		// create polygon indices
		for (int i = 2; i < vertex.size(); i++)
		{
			indices.append(0);
			indices.append(i - 1);
			indices.append(i + 0);
		}

		return true;
	}
	UNIGINE_INLINE bool createConvex(const Vector<vec3> &v, const vec3 &normal) { return createConvex(v.get(), v.size(), normal); }

	bool createConcave(const vec3 *v, int num_vertex, const vec3 &normal)
	{
		clear();

		if (num_vertex < 3)
			return 0;

		// copy vertices
		vertex.resize(num_vertex);
		for (int i = 0; i < num_vertex; i++)
		{
			vertex[i].xyz = v[i];
			vertex[i].index = i;
		}

		// polygon area
		vec3 area = cross(vertex[0].xyz, vertex[num_vertex - 1].xyz);
		for (int i = 1; i < num_vertex; i++)
			area += cross(vertex[i].xyz, vertex[i - 1].xyz);

		// indices buffer
		VectorStack<int, NUM_INDICES> buffer(num_vertex);
		if (dot(area, normal) < 0.0f)
		{
			for (int i = 0; i < num_vertex; i++)
				buffer[i] = num_vertex - i - 1;
		} else
		{
			for (int i = 0; i < num_vertex; i++)
				buffer[i] = i;
		}

		// create indices
		int i1 = buffer.size() - 1;
		int iterations = buffer.size() * 2;
		while (iterations-- > 0 && buffer.size() > 2)
		{
			int i0, i2;
			i0 = i1;
			if (i0 >= buffer.size())
				i0 = 0;
			i1 = i0 + 1;
			if (i1 >= buffer.size())
				i1 = 0;
			i2 = i1 + 1;
			if (i2 >= buffer.size())
				i2 = 0;

			const vec3 &v0 = vertex[buffer[i0]].xyz;
			const vec3 &v1 = vertex[buffer[i1]].xyz;
			const vec3 &v2 = vertex[buffer[i2]].xyz;

			if (dot(cross(v2 - v0, v1 - v0), normal) < 0.0f)
				continue;

			int inside = 0;
			for (int j = 0; j < buffer.size(); j++)
			{
				if (j == i0 || j == i1 || j == i2)
					continue;
				if (pointTriangleInside(vertex[buffer[j]].xyz, v0, v1, v2, normal))
				{
					inside = 1;
					break;
				}
			}
			if (inside)
				continue;

			indices.append(buffer[i0]);
			indices.append(buffer[i1]);
			indices.append(buffer[i2]);

			buffer.remove(i1);

			iterations = buffer.size() * 2;
		}

		return iterations > 0;
	}
	UNIGINE_INLINE bool createConcave(const Vector<vec3> &v, const vec3 &normal) { return createConcave(v.get(), v.size(), normal); }

	UNIGINE_INLINE int getNumVertex() const { return vertex.size(); }
	UNIGINE_INLINE const vec3 &getVertex(int num) const { return vertex[num].xyz; }
	UNIGINE_INLINE int getVertexIndex(int num) const { return vertex[num].index; }

	UNIGINE_INLINE int getNumIndices() const { return indices.size(); }
	UNIGINE_INLINE int getIndex(int num) const { return indices[num]; }

private:
	enum
	{
		NUM_VERTEX = 64,
		NUM_INDICES = 128,
	};

	struct Vertex
	{
		vec3 xyz;
		int index;
	};


	static int vertex_compare(const Vertex &v0, const Vertex &v1)
	{
		for (int i = 0; i < 3; i++)
		{
			float d = v0.xyz[i] - v1.xyz[i];
			if (d > Consts::EPS)
				return 1;
			if (d < -Consts::EPS)
				return -1;
		}
		return 0;
	}

	VectorStack<Vertex, NUM_VERTEX> vertex;
	VectorStack<int, NUM_INDICES> indices;
};

}
} // namespace Unigine
