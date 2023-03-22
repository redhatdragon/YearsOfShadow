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

#include "UnigineNode.h"
#include "UnigineMathLib.h"
#include "UnigineEllipsoid.h"
#include "UnigineTextures.h"
#include "UnigineMesh.h"

namespace Unigine
{

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API GeodeticPivot : public Node
{
public:
	static int type() { return Node::GEODETIC_PIVOT; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	using Node::toWorld;

	enum
	{
		ORIGIN_BASIS_LOCAL = 0,
		ORIGIN_BASIS_ENU,
	};
	static Ptr<GeodeticPivot> create();
	void setOrigin(const Math::dvec3 &origin);
	Math::dvec3 getOrigin() const;
	void setOriginBasis(int basis);
	int getOriginBasis() const;
	Ptr<Ellipsoid> getEllipsoid();
	void setEllipsoid(const Ptr<Ellipsoid> &ellipsoid);
	Math::dvec3 toGeodetic(const Math::dmat4 &world_transform);
	Math::dvec3 toGeodetic(const Math::mat4 &world_transform);
	Math::dmat4 toWorld(const Math::dvec3 &geodetic_coords);
	Math::dmat4 toWorldPreserveRotation(const Math::dmat4 &world_transform, const Math::dvec3 &geodetic_coords);
	Math::mat4 toWorldPreserveRotation(const Math::mat4 &world_transform, const Math::dvec3 &geodetic_coords);
	Math::dmat4 mapMeshFlatToEllipsoid(Ptr<Mesh> &mesh, const Math::dmat4 &flat_transform);
	Math::mat4 mapMeshFlatToEllipsoid(Ptr<Mesh> &mesh, const Math::mat4 &flat_transform);
	Math::dmat4 mapMeshEllipsoidToFlat(Ptr<Mesh> &mesh, const Math::dmat4 &ellipsoid_transform);
	Math::mat4 mapMeshEllipsoidToFlat(Ptr<Mesh> &mesh, const Math::mat4 &ellipsoid_transform);
	Math::dvec3 mapFlatToGeodetic(const Math::dvec3 &flat_point);
	Math::dvec3 mapFlatToGeodetic(const Math::vec3 &flat_point);
	void mapFlatsToGeodetic(const double *src_x, const double *src_y, int size, double *ret_lat, double *ret_lon) const;
	Math::dvec3 mapGeodeticToFlat(const Math::dvec3 &geodetic_coords);
	void mapGeodeticsToFlat(const double *lat, const double *lon, int size, double *ret_x, double *ret_y) const;
	void mapFlatToEllipsoid(const Math::dvec3 &flat_point, Math::dvec3 &ret_ellipsoid_point, Math::dvec3 &ret_ellipsoid_normal);
	void mapFlatToEllipsoid(const Math::vec3 &flat_point, Math::vec3 &ret_ellipsoid_point, Math::vec3 &ret_ellipsoid_normal);
	void mapEllipsoidToFlat(const Math::dvec3 &ellipsoid_point, Math::dvec3 &ret_flat_point, Math::dvec3 &ret_ellipsoid_normal);
	void mapEllipsoidToFlat(const Math::vec3 &ellipsoid_point, Math::vec3 &ret_flat_point, Math::vec3 &ret_ellipsoid_normal);
	Math::dmat4 mapFlatToEllipsoid(const Math::dmat4 &flat_transform);
	Math::mat4 mapFlatToEllipsoid(const Math::mat4 &flat_transform);
	Math::dmat4 mapEllipsoidToFlat(const Math::dmat4 &ellipsoid_transform);
	Math::mat4 mapEllipsoidToFlat(const Math::mat4 &ellipsoid_transform);
};
typedef Ptr<GeodeticPivot> GeodeticPivotPtr;

} // namespace Unigine
