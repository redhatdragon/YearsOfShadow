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
#include "UnigineMathLibSimd.h"
#include "UnigineMathLibVec3.h"
#include "UnigineMathLibVec4.h"
#include "UnigineMathLibDVec3.h"
#include "UnigineMathLibDVec4.h"
#include "UnigineMathLibPolygon.h"

namespace Unigine
{
namespace Math
{

#ifdef _LINUX
	#ifdef Convex
		#undef Convex
	#endif
#endif

class Convex
{
public:
	UNIGINE_INLINE Convex() { clear(); }
	Convex(const Convex &convex)
	{
		clear();

		volume = convex.getVolume();
		threshold = convex.getThreshold();
		vertex = convex.vertex;
		faces = convex.faces;
	}
	UNIGINE_INLINE ~Convex() { clear(); }

	Convex &operator=(const Convex &convex)
	{
		if (this == &convex)
			return *this;

		clear();

		volume = convex.getVolume();
		threshold = convex.getThreshold();
		vertex = convex.vertex;
		faces = convex.faces;

		return *this;
	}

	// clear
	void clear()
	{
		volume = 0.0;
		threshold = 0.0;
		vertex.clear();
		edges.clear();
		for (int i = 0; i < triangles.size(); i++)
			delete triangles[i];
		triangles.clear();
		faces.clear();
	}

	// create convex
	bool create(const vec3 *v, int num_vertex, float error = 0.01f)
	{
		// copy vertices
		static thread_local Vector<dvec3> temp_vertex;
		temp_vertex.clear();
		temp_vertex.allocate(num_vertex);
		for (int i = 0; i < num_vertex; i++)
			temp_vertex.appendFast(dvec3(v[i]));

		return create(temp_vertex.get(), temp_vertex.size(), error);
	}
	bool create(const dvec3 *v, int num_vertex, float error = 0.01f)
	{
		clear();

		if (num_vertex < 4)
			return false;

		// copy vertices
		vertex.append(v, num_vertex);

		// optimize vertices
		int num_optimized_vertex = 0;
		quickSort2(vertex.get(), vertex.size(), vertex_compare);
		for (int i = 0; i < vertex.size(); i++)
		{
			if (i == 0 || vertex_compare(vertex[num_optimized_vertex - 1], vertex[i]))
				vertex[num_optimized_vertex++] = vertex[i];
		}
		if (num_optimized_vertex < 3)
		{
			vertex.clear();
			return false;
		}

		// optimized vertices
		vertex.resize(num_optimized_vertex);

		// convex threshold
		dvec3 min = dvec3_inf;
		dvec3 max = -dvec3_inf;
		for (int i = 0; i < vertex.size(); i++)
		{
			min = Math::min(min, vertex[i]);
			max = Math::max(max, vertex[i]);
		}
		threshold = Math::max(length(max - min) / 2.0 * saturate(error), 0.0) + Consts::EPS;

		// convex base triangle
		int v0 = 0;
		int v1 = 1;
		int v2 = 2;
		dvec3 size = max - min;
		int axis = (size.x > size.y) ? ((size.x > size.z) ? 0 : 2) : ((size.y > size.z) ? 1 : 2);
		for (int i = 0; i < vertex.size(); i++)
		{
			if (vertex[v0][axis] < vertex[i][axis])
				v0 = i;
			if (vertex[v1][axis] > vertex[i][axis])
				v1 = i;
		}
		double area = -Consts::INF;
		for (int i = 0; i < vertex.size(); i++)
		{
			double a = length2(cross(vertex[v0] - vertex[i], vertex[v1] - vertex[i]));
			if (area < a)
			{
				area = a;
				v2 = i;
			}
		}

		// initial convex
		Triangle *t0 = add_triangle(v0, v1, v2);
		Triangle *t1 = add_triangle(v0, v2, v1);
		for (int i = 0; i < vertex.size(); i++)
		{
			add_vertex(t0, i);
			add_vertex(t1, i);
		}

		// create convex
		if (create_convex() == 0)
		{
			clear();
			return false;
		}

		// create convex vertices
		create_vertex();

		// create convex faces
		create_faces();

		// calculate convex volume
		for (int i = 0; i < faces.size(); i++)
		{
			const Face &f = faces[i];
			const dvec3 &v0 = vertex[f.vertex[0]];
			for (int j = 2; j < f.vertex.size(); j++)
			{
				const dvec3 &v1 = vertex[f.vertex[j - 1]];
				const dvec3 &v2 = vertex[f.vertex[j]];
				dvec3 normal = cross(v1 - v0, v2 - v0);
				volume += normal.x * (v0.x + v1.x + v2.x);
			}
		}
		volume /= 6.0;

		return true;
	}


	// vertices
	UNIGINE_INLINE int getNumVertex() const { return vertex.size(); }
	UNIGINE_INLINE const dvec3 *getVertex() const { return vertex.get(); }
	UNIGINE_INLINE const dvec3 &getVertex(int num) const { return vertex[num]; }

	// faces
	UNIGINE_INLINE int getNumFaces() const { return faces.size(); }
	UNIGINE_INLINE int getNumFaceVertex(int face) const { return faces[face].vertex.size(); }
	UNIGINE_INLINE int getFaceVertex(int face, int num) const { return faces[face].vertex[num]; }

	// parameters
	UNIGINE_INLINE double getVolume() const { return volume; }
	UNIGINE_INLINE double getThreshold() const { return threshold; }
	dvec3 getCenter() const
	{
		double integral_x = 0.0;
		double integral_y = 0.0;
		double integral_z = 0.0;
		for (int i = 0; i < faces.size(); i++)
		{
			const Face &f = faces[i];
			const dvec3 &v0 = vertex[f.vertex[0]];
			for (int j = 2; j < f.vertex.size(); j++)
			{
				const dvec3 &v1 = vertex[f.vertex[j - 1]];
				const dvec3 &v2 = vertex[f.vertex[j]];
				dvec3 normal = cross(v1 - v0, v2 - v0);
				dvec3 t0 = v0 + v1;
				dvec3 t1 = v0 * v0;
				dvec3 t2 = t1 + t0 * v1;
				dvec3 f1 = t0 + v2;
				dvec3 f2 = t2 + f1 * v2;
				integral_x += normal.x * f2.x;
				integral_y += normal.y * f2.y;
				integral_z += normal.z * f2.z;
			}
		}
		integral_x /= 24.0;
		integral_y /= 24.0;
		integral_z /= 24.0;
		return dvec3(integral_x, integral_y, integral_z) / volume;
	}
	mat3 getInertia() const
	{
		mat3 inertia;
		double integral_xx = 0.0;
		double integral_yy = 0.0;
		double integral_zz = 0.0;
		double integral_xy = 0.0;
		double integral_yz = 0.0;
		double integral_zx = 0.0;
		for (int i = 0; i < faces.size(); i++)
		{
			const Face &f = faces[i];
			const dvec3 &v0 = vertex[f.vertex[0]];
			for (int j = 2; j < f.vertex.size(); j++)
			{
				const dvec3 &v1 = vertex[f.vertex[j - 1]];
				const dvec3 &v2 = vertex[f.vertex[j]];
				dvec3 normal = cross(v1 - v0, v2 - v0);
				dvec3 t0 = v0 + v1;
				dvec3 t1 = v0 * v0;
				dvec3 t2 = t1 + t0 * v1;
				dvec3 f1 = t0 + v2;
				dvec3 f2 = t2 + f1 * v2;
				dvec3 f3 = t1 * v0 + t2 * v1 + f2 * v2;
				dvec3 g0 = f2 + v0 * (f1 + v0);
				dvec3 g1 = f2 + v1 * (f1 + v1);
				dvec3 g2 = f2 + v2 * (f1 + v2);
				integral_xx += normal.x * f3.x;
				integral_yy += normal.y * f3.y;
				integral_zz += normal.z * f3.z;
				integral_xy += normal.x * (g0.x * v0.y + g1.x * v1.y + g2.x * v2.y);
				integral_yz += normal.y * (g0.y * v0.z + g1.y * v1.z + g2.y * v2.z);
				integral_zx += normal.z * (g0.z * v0.x + g1.z * v1.x + g2.z * v2.x);
			}
		}
		integral_xx /= 60.0;
		integral_yy /= 60.0;
		integral_zz /= 60.0;
		integral_xy /= 120.0;
		integral_yz /= 120.0;
		integral_zx /= 120.0;
		inertia.m00 = (float)((integral_yy + integral_zz) / volume);
		inertia.m11 = (float)((integral_zz + integral_xx) / volume);
		inertia.m22 = (float)((integral_xx + integral_yy) / volume);
		inertia.m01 = (float)(-integral_xy / volume);
		inertia.m02 = (float)(-integral_zx / volume);
		inertia.m12 = (float)(-integral_yz / volume);
		inertia.m10 = inertia.m01;
		inertia.m20 = inertia.m02;
		inertia.m21 = inertia.m12;
		return inertia;
	}

	// closest point
	dvec3 getClosestPoint(const dvec3 &point) const
	{
		dvec3 p, clip, v10, v20;
		dvec3 ret = dvec3_zero;
		double distance = Consts::INF;
		for (int i = 0; i < faces.size(); i++)
		{
			const Face &f = faces[i];
			dvec3 normal = triangleNormal(vertex[f.vertex[0]], vertex[f.vertex[1]], vertex[f.vertex[2]]);
			dvec4 plane = dvec4(normal, -dot(normal, vertex[f.vertex[0]]));
			double d = dot(plane, point);
			if (distance < Math::abs(d))
				continue;
			int inside = 1;
			mad(p, normal, -d, point);
			for (int j = 0; j < f.vertex.size(); j++)
			{
				const dvec3 &v0 = vertex[f.vertex[j]];
				const dvec3 &v1 = vertex[f.vertex[(j + 1) % f.vertex.size()]];
				sub(v10, v1, v0);
				sub(v20, p, v0);
				cross(clip, v10, normal);
				if (dot(clip, v20) > Consts::EPS)
				{
					inside = 0;
					break;
				}
			}
			if (inside)
			{
				distance = Math::abs(d);
				ret = p;
				continue;
			}
			for (int j = 0; j < f.vertex.size(); j++)
			{
				const dvec3 &v0 = vertex[f.vertex[j]];
				const dvec3 &v1 = vertex[f.vertex[(j + 1) % f.vertex.size()]];
				getClosestPointOnLine(point, v0, v1, p);
				d = length(point - p);
				if (distance > d)
				{
					distance = d;
					ret = p;
				}
			}
		}
		return ret;
	}

	// bounding box
	UNIGINE_INLINE void getBoundBox(dvec3 &min, dvec3 &max) const
	{
		Simd::minMaxVec3(min, max, vertex.get(), sizeof(dvec3), vertex.size());
	}

private:
	struct Vertex
	{
		double distance;
		int id;
	};
	struct Edge
	{
		int v[2];		// edge vertices
		int counter;	// edge counter
	};
	struct Triangle
	{
		int v[3];						// triangle vertices
		int e[3];						// triangle edges
		dvec4 plane;					// triangle plane
		VectorStack<Vertex, 16> vertex;	// separate vertices
	};
	struct Face
	{
		VectorStack<int, 16> vertex;	// face vertices
	};

	static char vertex_compare(const dvec3 &v0, const dvec3 &v1)
	{
		for (int i = 0; i < 3; i++)
		{
			double d = v0[i] - v1[i];
			if (d > Consts::EPS)
				return 1;
			if (d < -Consts::EPS)
				return -1;
		}

		return 0;
	}
	static char triangle_compare(const Triangle *t0, const Triangle *t1)
	{
		for (int i = 0; i < 4; i++)
		{
			double d = t0->plane[i] - t1->plane[i];
			if (d > Consts::EPS)
				return 1;
			if (d < -Consts::EPS)
				return -1;
		}

		return 0;
	}

	void add_vertex(Triangle *t, int v)
	{
		double distance = dot(t->plane, vertex[v]);
		if (distance > -Consts::EPS)
		{
			Vertex *vertex = &t->vertex.append();
			vertex->distance = distance;
			vertex->id = v;
		}
	}
	Triangle *add_triangle(int v0, int v1, int v2)
	{
		Triangle *t = new Triangle();
		triangles.append(t);
		t->v[0] = v0;
		t->v[1] = v1;
		t->v[2] = v2;
		t->plane = trianglePlane(vertex[v0], vertex[v1], vertex[v2]);
		return t;
	}

	void create_edges(const Vector<Triangle *> &triangles)
	{
		static const int next_3[3] = {1, 2, 0};

		// create edges
		edges.clear();
		for (int i = 0; i < triangles.size(); i++)
		{
			Triangle *t = triangles[i];
			for (int j = 0; j < 3; j++)
			{
				t->e[j] = -1;
				int v0 = t->v[j];
				int v1 = t->v[next_3[j]];
				for (int k = 0; k < edges.size(); k++)
				{
					Edge *e = &edges[k];
					if ((e->v[0] == v0 && e->v[1] == v1) || (e->v[0] == v1 && e->v[1] == v0))
					{
						t->e[j] = k;
						break;
					}
				}
				if (t->e[j] == -1)
				{
					t->e[j] = edges.size();
					Edge *e = &edges.append();
					e->v[0] = v0;
					e->v[1] = v1;
				}
			}
		}

		// find silhouette edges
		for (int i = 0; i < edges.size(); i++)
			edges[i].counter = 0;
		for (int i = 0; i < triangles.size(); i++)
		{
			Triangle *t = triangles[i];
			for (int j = 0; j < 3; j++)
				edges[t->e[j]].counter++;
		}
	}
	bool create_convex()
	{
		static thread_local Vector<int> removed_vertex;
		removed_vertex.clear();

		static thread_local Vector<Triangle *> removed_triangles;
		removed_triangles.clear();

		static thread_local Vector<int> counter;
		counter.clear();
		counter.resize(vertex.size());
		
		int iterations = 1000;
		while (iterations-- > 0)
		{
			// find new vertex
			int new_vertex = -1;
			int old_triangle = -1;
			double distance = -Consts::INF;
			for (int i = 0; i < triangles.size(); i++)
			{
				Triangle *t = triangles[i];
				for (int j = 0; j < t->vertex.size(); j++)
				{
					Vertex v = t->vertex[j];
					if (distance < v.distance)
					{
						distance = v.distance;
						new_vertex = v.id;
						old_triangle = i;
					}
				}
			}

			// done
			if (new_vertex == -1)
				break;
			if (triangles.size() > 2 && distance < threshold)
				break;

			// removed vertices
			removed_vertex.clear();
			for (int i = 0; i < triangles.size(); i++)
			{
				Triangle *t = triangles[i];
				for (int j = 0; j < t->vertex.size(); j++)
					counter[t->vertex[j].id] = 0;
			}
			for (int i = 0; i < triangles.size(); i++)
			{
				Triangle *t = triangles[i];
				for (int j = 0; j < t->vertex.size(); j++)
				{
					if (counter[t->vertex[j].id] == 0)
					{
						counter[t->vertex[j].id] = 1;
						removed_vertex.append(t->vertex[j].id);
					}
				}
			}

			// removed triangles
			removed_triangles.clear();
			removed_triangles.append(triangles[old_triangle]);
			triangles.removeFast(old_triangle);
			for (int i = 0; i < triangles.size(); i++)
			{
				Triangle *t = triangles[i];
				if (dot(t->plane, vertex[new_vertex]) > -1e-12)
				{
					removed_triangles.append(t);
					triangles.removeFast(i--);
				}
			}

			// create silhouette edges
			create_edges(removed_triangles);

			// delete removed triangles
			for (int i = 0; i < removed_triangles.size(); i++)
				delete removed_triangles[i];

			// create silhouette triangles
			for (int i = 0; i < edges.size(); i++)
			{
				Edge *e = &edges[i];
				if (e->counter == 1)
				{
					Triangle *t = add_triangle(e->v[0], e->v[1], new_vertex);
					for (int j = 0; j < removed_vertex.size(); j++)
						add_vertex(t, removed_vertex[j]);
				}
			}
		}

		// can't create convex
		if (iterations < 0)
		{
			Log::error("Convex::create_convex(): can't create convex\n");
			return false;
		}

		return true;
	}
	void create_vertex()
	{
		static thread_local Vector<int> vbuf;
		vbuf.clear();
		vbuf.resize(vertex.size());

		static thread_local Vector<int> counter;
		counter.clear();
		counter.resize(vertex.size());

		static thread_local Vector<dvec3> old_vertex;
		old_vertex.clear();
		old_vertex = vertex;
		vertex.clear();

		// clear counter
		for (int i = 0; i < triangles.size(); i++)
		{
			Triangle *t = triangles[i];
			for (int j = 0; j < 3; j++)
				counter[t->v[j]] = 0;
		}

		// convex vertices
		for (int i = 0; i < triangles.size(); i++)
		{
			Triangle *t = triangles[i];
			for (int j = 0; j < 3; j++)
			{
				if (counter[t->v[j]] == 0)
				{
					counter[t->v[j]] = 1;
					vbuf[t->v[j]] = vertex.size();
					vertex.append(old_vertex[t->v[j]]);
				}
			}
		}

		// remap vertices
		for (int i = 0; i < triangles.size(); i++)
		{
			Triangle *t = triangles[i];
			for (int j = 0; j < 3; j++)
				t->v[j] = vbuf[t->v[j]];
		}
	}
	void create_faces()
	{
		static thread_local Vector<int> indices;
		static thread_local Vector<Triangle *> face_triangles;
		indices.clear();
		face_triangles.clear();

		// sort triangles
		quickSort2(triangles.get(), triangles.size(), triangle_compare);

		// create faces
		for (int i = 0; i < triangles.size(); i++)
		{
			Triangle *t = triangles[i];

			// face triangles
			face_triangles.clear();
			face_triangles.append(t);
			for (; i < triangles.size() - 1; i++)
			{
				if (triangle_compare(face_triangles[0], triangles[i + 1]))
					break;
				face_triangles.append(triangles[i + 1]);
			}

			// create silhouette edges
			create_edges(face_triangles);

			// remove internal edges
			for (int j = 0; j < edges.size(); j++)
			{
				if (edges[j].counter != 1)
					edges.removeFast(j--);
			}

			// convex edges
			int convex = 1;
			dvec3 normal = dvec3(t->plane);
			for (int j = 0; j < edges.size(); j++)
			{
				Edge *e = &edges[j];
				dvec3 v0 = vertex[e->v[0]];
				dvec3 v1 = vertex[e->v[1]];
				dvec3 clip = cross(v1 - v0, normal);
				for (int k = 0; k < edges.size(); k++)
				{
					if (dot(clip, vertex[edges[k].v[0]] - v0) > Consts::EPS)
					{
						convex = 0;
						break;
					}
				}
				if (convex == 0)
					break;
			}

			// create indices
			indices.clear();
			while (true)
			{
				int num_indices = indices.size();
				for (int j = 0; j < edges.size(); j++)
				{
					Edge *e = &edges[j];
					if (indices.size() == 0)
					{
						indices.append(e->v[0]);
						indices.append(e->v[1]);
						edges.removeFast(j--);
					} else if (indices[0] == e->v[1])
						edges.removeFast(j--);
					else if (indices.last() == e->v[0])
					{
						indices.append(e->v[1]);
						edges.removeFast(j--);
					}
				}
				if (num_indices == indices.size())
					break;
			}

			// create triangle faces
			if (edges.size() || convex == 0)
			{
				for (int j = 0; j < face_triangles.size(); j++)
				{
					Triangle *t = face_triangles[j];
					Face *face = &faces.append();
					face->vertex.clear();
					face->vertex.append(t->v[0]);
					face->vertex.append(t->v[1]);
					face->vertex.append(t->v[2]);
				}
			}

			// create polygon faces
			else if (indices.size() > 2)
			{
				Face *face = &faces.append();
				face->vertex.clear();
				for (int j = 0; j < indices.size(); j++)
					face->vertex.append(indices[j]);
			}
		}
	}

	double volume;					// convex volume
	double threshold;				// distance threshold
	Vector<dvec3> vertex;
	Vector<Edge> edges;
	Vector<Triangle *> triangles;
	Vector<Face> faces;
};

}
} // namespace Unigine
