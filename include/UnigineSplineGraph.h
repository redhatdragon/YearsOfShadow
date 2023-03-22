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

// DO NOT EDIT DIRECTLY. This is an auto-generated file. Your changes will be lost.

#pragma once

#include "UniginePtr.h"
#include "UnigineMathLib.h"
#include "UnigineVector.h"

namespace Unigine
{


class UNIGINE_API SplineGraph : public APIInterface
{
public:
	static Ptr<SplineGraph> create();
	static Ptr<SplineGraph> create(const char *name);
	static Ptr<SplineGraph> create(const Ptr<SplineGraph> &graph);
	bool load(const char *name);
	bool save(const char *name) const;
	void clear();
	int getNumPoints() const;
	int addPoint(const Math::Vec3 &point);
	void removePoint(int index, bool merge = false);
	void splitPoint(int index);
	void weldPoints(Vector<int> &indices);
	void insertPointToSegment(int segment_index, float new_point_t);
	Math::Vec3 getPoint(int index) const;
	void setPoint(int index, const Math::Vec3 &point);
	void getPointSegmentsIndices(int index, Vector<int> &indices) const;
	int getNumSegments() const;
	int addSegment(int start_index, const Math::vec3 &start_tangent, const Math::vec3 &start_up, int end_index, const Math::vec3 &end_tangent, const Math::vec3 &end_up);
	void removeSegment(int index, bool with_points = false);
	int getSegmentStartPointIndex(int index) const;
	Math::Vec3 getSegmentStartPoint(int index) const;
	Math::vec3 getSegmentStartTangent(int index) const;
	Math::vec3 getSegmentStartUpVector(int index) const;
	void setSegmentStartPoint(int index, const Math::Vec3 &point);
	void setSegmentStartTangent(int index, const Math::vec3 &tangent);
	void setSegmentStartUpVector(int index, const Math::vec3 &up);
	int getSegmentEndPointIndex(int index) const;
	Math::Vec3 getSegmentEndPoint(int index) const;
	Math::vec3 getSegmentEndTangent(int index) const;
	Math::vec3 getSegmentEndUpVector(int index) const;
	void setSegmentEndPoint(int index, const Math::Vec3 &point);
	void setSegmentEndTangent(int index, const Math::vec3 &tangent);
	void setSegmentEndUpVector(int index, const Math::vec3 &up);
	Math::Vec3 calcSegmentPoint(int index, float t) const;
	Math::vec3 calcSegmentTangent(int index, float t) const;
	Math::vec3 calcSegmentUpVector(int index, float t) const;
	float getLength(int index) const;
};
typedef Ptr<SplineGraph> SplineGraphPtr;

} // namespace Unigine
