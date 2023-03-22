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

namespace Unigine
{


class UNIGINE_API Ellipsoid : public APIInterface
{
public:
	static int isSupported();
	static Ptr<Ellipsoid> create();
	static Ptr<Ellipsoid> create(double semimajor_axis, double flattening);
	void setSemimajorAxis(double axis);
	double getSemimajorAxis() const;
	void setFlattening(double flattening);
	double getFlattening() const;
	void setMode(int mode);
	int getMode() const;
	double getSemiminorAxis() const;
	double getMeanRadius() const;
	double getSemimajorEccentricitySqr() const;
	double getSemiminorEccentricitySqr() const;
	Math::dmat4 getENUWorldTransform(const Math::dvec3 &geodetic_origin) const;
	Math::quat getENUWorldRotation(const Math::dvec3 &geodetic_origin) const;
	Math::dvec3 getENUSurfacePoint(const Math::dvec3 &geodetic_origin, const Math::dvec3 &tangent_point) const;
	Math::dvec3 getENUTangentPoint(const Math::dvec3 &geodetic_origin, const Math::dvec3 &surface_point) const;
	Math::dmat4 getNEDWorldTransform(const Math::dvec3 &geodetic_origin) const;
	Math::quat getNEDWorldRotation(const Math::dvec3 &geodetic_origin) const;
	Math::dvec3 getNEDSurfacePoint(const Math::dvec3 &geodetic_origin, const Math::dvec3 &tangent_point) const;
	Math::dvec3 getNEDTangentPoint(const Math::dvec3 &geodetic_origin, const Math::dvec3 &surface_point) const;
	Math::dvec3 toGeodetic(const Math::dvec3 &ecf_coords, int need_alt = 1) const;
	Math::dvec3 toECF(const Math::dvec3 &geodetic_coords) const;
	Math::dvec3 toNED(const Math::dvec3 &geodetic_origin, const Math::dvec3 &geodetic_coords) const;
	Math::dvec3 toENU(const Math::dvec3 &geodetic_origin, const Math::dvec3 &geodetic_coords) const;
	void solveGeodeticInverse(const Math::dvec3 &geodetic_start, const Math::dvec3 &geodetic_end, double &bearing, double &distance) const;
	Math::dvec3 solveGeodeticDirect(const Math::dvec3 &geodetic_start, double bearing, double distance) const;

	enum
	{
		MODE_FAST = 0,
		MODE_ACCURATE = 1,
	};
};
typedef Ptr<Ellipsoid> EllipsoidPtr;

} // namespace Unigine
