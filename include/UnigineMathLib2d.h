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

#include <UnigineMathLib.h>
#include <UnigineVector.h>

namespace Unigine
{
	namespace Math
	{

		// return the polygon's area in "square units".
		// the value will be negative if the polygon is
		// oriented clockwise
		UNIGINE_INLINE float getPolygonAreaSigned(const Vector<vec2> &points)
		{
			// get the area
			float area = 0;
			int count = points.size();
			if (count <= 1)
				return 0;

			for (int i = 0; i < count - 1; i++)
			{
				area +=
					(points[i + 1].x - points[i].x) *
					(points[i + 1].y + points[i].y) / 2;
			}
			area +=
				(points[0].x - points[count - 1].x) *
				(points[0].y + points[count - 1].y) / 2;

			// return the result
			return area;
		}

		// return true if the polygon is oriented clockwise
		UNIGINE_INLINE int polygonIsOrientedClockwise(const Vector<vec2> &points)
		{
			return (getPolygonAreaSigned(points) < 0);
		}

		// find the area of a triangle. This function uses the 1/2 determinant
		// method. Given three points (x1, y1), (x2, y2), (x3, y3):
		//             | x1 y1 1 |
		// Area = .5 * | x2 y2 1 |
		//             | x3 y3 1 |
		// From: http://mcraefamily.com/MathHelp/GeometryTriangleAreaDeterminant.htm
		UNIGINE_INLINE float getTriangleArea(const vec2 &p1, const vec2 &p2, const vec2 &p3)
		{
			float det = (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
			return (det / 2.0f);
		}

		// return on which side of the segment the point is
		UNIGINE_INLINE float sign(const vec2 &point, const vec2 &segment_p1, const vec2 &segment_p2)
		{
			return (point.x - segment_p2.x) * (segment_p1.y - segment_p2.y) - (segment_p1.x - segment_p2.x) * (point.y - segment_p2.y);
		}

		// return true if the point is inside triangle
		UNIGINE_INLINE int pointTriangleInside(const vec2 &point, const vec2 &v0, const vec2 &v1, const vec2 &v2)
		{
			int b1, b2, b3;

			// checking on which side of the half-plane created by the edges the point is
			b1 = sign(point, v0, v1) < 0.0f;
			b2 = sign(point, v1, v2) < 0.0f;
			b3 = sign(point, v2, v0) < 0.0f;

			return ((b1 == b2) && (b2 == b3));
		}

		/*
		Types of 2D polygon:
			1) convex, concave
			2) holed, solid
			3) looped, line
			4) CCW or CW winding
		*/

		// sets polygon to counter-clockwise orientation
		UNIGINE_INLINE int setPolygonToCCW(Vector<vec2> &points)
		{
			if (points.size() < 3)
				return 0;

			// if polygon oriented CCW already
			if (!polygonIsOrientedClockwise(points))
				return 0;

			// invert array
			int count = points.size();
			for (int i = 0; i < count / 2; ++i)
				std::swap(points[i], points[count - i - 1]);
			return 1;
		}

		// triangulation by ear clipping algorithm (complexity: O(n^2)/O(n))
		// support concave polygons, for CCW orientation only
		UNIGINE_INLINE void triangulatePolygon(const Vector<vec2> &points, Vector<unsigned short> &indices, int append_to_result = 0)
		{
			if (!append_to_result)
				indices.clear();

			if (points.size() < 3)
				return;

			if (points.size() == 3)
			{
				indices.append(0);
				indices.append(1);
				indices.append(2);
				return;
			}

			// create a list of the vertices (points array must be in CCW order)
			Vector<vec2> p_ccw = points;
			Vector<unsigned short> p_ccw_id;
			p_ccw_id.resize(p_ccw.size());
			for (int i = 0; i < p_ccw.size(); i++)
				p_ccw_id[i] = (unsigned short)i;

			// triangulate process
			bool new_triangle_created;
			do
			{
				new_triangle_created = false;
				for (int j = 0; j < p_ccw.size(); j++)
				{
					// order: i -> j -> k
					int i = (j - 1);
					if (i < 0) i += p_ccw.size();
					int k = (j + 1) % p_ccw.size();

					// if the vertex is not an interior vertex (the wedge product <= 0, for CCW winding)
					if (sign(p_ccw[k], p_ccw[i], p_ccw[j]) >= 0)
						continue;

					// if there are any vertices in the polygon inside the triangle made by the current vertex and the two adjacent ones
					int vertex_inside = 0;
					for (int z = 0; z < p_ccw.size(); z++)
						if (z != i && z != j && z != k && pointTriangleInside(p_ccw[z], p_ccw[i], p_ccw[j], p_ccw[k]))
						{
							vertex_inside = 1;
							break;
						}
					if (vertex_inside)
						continue;

					// create the triangle with the points (i, j, k) for a CCW triangle
					indices.append(p_ccw_id[i]);
					indices.append(p_ccw_id[j]);
					indices.append(p_ccw_id[k]);
					new_triangle_created = true;

					// remove current point from the list
					p_ccw.remove(j);
					p_ccw_id.remove(j);
					j--;
				}
			} while (new_triangle_created && p_ccw.size() > 2);
		}

		// find the point of intersection between the lines p1 --> p2 and p3 --> p4
		UNIGINE_INLINE void findIntersection(const vec2 &p1, const vec2 &p2, const vec2 &p3, const vec2 &p4,
											 int &lines_intersect, int &segments_intersect, vec2 &intersection,
											 vec2 &close_p1, vec2 &close_p2)
		{
			// get the segments' parameters
			float dx12 = p2.x - p1.x;
			float dy12 = p2.y - p1.y;
			float dx34 = p4.x - p3.x;
			float dy34 = p4.y - p3.y;

			// solve for t1 and t2
			float denominator = (dy12 * dx34 - dx12 * dy34);

			float t1 = denominator != 0 ? (((p1.x - p3.x) * dy34 + (p3.y - p1.y) * dx34) / denominator) : Consts::INF;
			if (t1 >= Consts::INF)
			{
				// the lines are parallel (or close enough to it)
				lines_intersect = 0;
				segments_intersect = 0;
				intersection = vec2_zero;
				close_p1 = vec2_zero;
				close_p2 = vec2_zero;
				return;
			}
			lines_intersect = 1;

			float t2 = ((p3.x - p1.x) * dy12 + (p1.y - p3.y) * dx12) / -denominator;

			// find the point of intersection
			intersection = vec2(p1.x + dx12 * t1, p1.y + dy12 * t1);

			// the segments intersect if t1 and t2 are between 0 and 1
			segments_intersect = ((t1 >= 0) && (t1 <= 1) && (t2 >= 0) && (t2 <= 1));

			// find the closest points on the segments
			t1 = saturate(t1);
			t2 = saturate(t2);

			close_p1 = vec2(p1.x + dx12 * t1, p1.y + dy12 * t1);
			close_p2 = vec2(p3.x + dx34 * t2, p3.y + dy34 * t2);
		}
		UNIGINE_INLINE int findIntersection(const vec2 &p1, const vec2 &p2, const vec2 &p3, const vec2 &p4, vec2 &intersection)
		{
			int lines_intersects = 0;
			int segments_intersects = 0;
			vec2 close_p1, close_p2;
			findIntersection(p1, p2, p3, p4, lines_intersects, segments_intersects, intersection, close_p1, close_p2);
			return segments_intersects;
		}

		// find the point of intersection between the lines p1 --> p2 and p3 --> p4
		UNIGINE_INLINE void findIntersection(const dvec2 &p1, const dvec2 &p2, const dvec2 &p3, const dvec2 &p4,
											 int &lines_intersect, int &segments_intersect, dvec2 &intersection,
											 dvec2 &close_p1, dvec2 &close_p2)
		{
			// get the segments' parameters
			double dx12 = p2.x - p1.x;
			double dy12 = p2.y - p1.y;
			double dx34 = p4.x - p3.x;
			double dy34 = p4.y - p3.y;

			// solve for t1 and t2
			double denominator = (dy12 * dx34 - dx12 * dy34);

			double t1 = denominator != 0 ? (((p1.x - p3.x) * dy34 + (p3.y - p1.y) * dx34) / denominator) : double(Consts::INF);
			if (t1 >= double(Consts::INF))
			{
				// the lines are parallel (or close enough to it)
				lines_intersect = 0;
				segments_intersect = 0;
				intersection = dvec2_zero;
				close_p1 = dvec2_zero;
				close_p2 = dvec2_zero;
				return;
			}
			lines_intersect = 1;

			double t2 = ((p3.x - p1.x) * dy12 + (p1.y - p3.y) * dx12) / -denominator;

			// find the point of intersection
			intersection = dvec2(p1.x + dx12 * t1, p1.y + dy12 * t1);

			// the segments intersect if t1 and t2 are between 0 and 1
			segments_intersect = ((t1 >= 0) && (t1 <= 1) && (t2 >= 0) && (t2 <= 1));

			// find the closest points on the segments
			t1 = saturate(t1);
			t2 = saturate(t2);

			close_p1 = dvec2(p1.x + dx12 * t1, p1.y + dy12 * t1);
			close_p2 = dvec2(p3.x + dx34 * t2, p3.y + dy34 * t2);
		}

		UNIGINE_INLINE int findIntersection(const dvec2 &p1, const dvec2 &p2, const dvec2 &p3, const dvec2 &p4, dvec2 &intersection)
		{
			int lines_intersects = 0;
			int segments_intersects = 0;
			dvec2 close_p1, close_p2;
			findIntersection(p1, p2, p3, p4, lines_intersects, segments_intersects, intersection, close_p1, close_p2);
			return segments_intersects;
		}

		// return points representing an shrunk/enlarged polygon
		// support concave polygons, CCW and CW orientations
		UNIGINE_INLINE void resizePolygon(const Vector<vec2> &points, float offset, Vector<vec2> &result_points, int append_to_result = 0)
		{
			if (!append_to_result)
				result_points.clear();

			int num_points = points.size();
			for (int j = 0; j < num_points; j++)
			{
				// order: i -> j -> k

				// find the new location for point j
				// find the points before and after j
				int i = (j - 1);
				if (i < 0) i += num_points;
				int k = (j + 1) % num_points;

				// move the points by the offset
				vec2 v1 = vec2(points[j].x - points[i].x, points[j].y - points[i].y); // get first vector
				v1.normalize();
				v1 *= offset;
				vec2 n1 = vec2(-v1.y, v1.x); // get its normal

				vec2 pij1 = vec2(points[i].x + n1.x, points[i].y + n1.y); // new points position
				vec2 pij2 = vec2(points[j].x + n1.x, points[j].y + n1.y);

				vec2 v2 = vec2(points[k].x - points[j].x, points[k].y - points[j].y); // get second vector
				v2.normalize();
				v2 *= offset;
				vec2 n2 = vec2(-v2.y, v2.x); // get its normal

				vec2 pjk1 = vec2(points[j].x + n2.x, points[j].y + n2.y);  // new points position
				vec2 pjk2 = vec2(points[k].x + n2.x, points[k].y + n2.y);

				// see where the shifted lines ij and jk intersect
				int lines_intersect, segments_intersect;
				vec2 poi, close1, close2;
				findIntersection(pij1, pij2, pjk1, pjk2, lines_intersect, segments_intersect, poi, close1, close2);
				if (!lines_intersect)
					result_points.append(pij2);
				else
					result_points.append(poi);
			}
		}

		// return the polygon's area in "square units"
		UNIGINE_INLINE float getPolygonArea(const Vector<vec2> &points)
		{
			return Math::abs(getPolygonAreaSigned(points));
		}

	}
}
