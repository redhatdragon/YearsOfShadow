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

#include "UnigineObjects.h"

namespace Unigine
{

namespace Primitives
{
	UNIGINE_API void addBoxSurface(Ptr<ObjectMeshDynamic> &object, const Math::vec3 &size, const Math::mat4 &transform);
	UNIGINE_API Ptr<ObjectMeshDynamic> createBox(const Math::vec3 &size);
	UNIGINE_API void addPlaneSurface(Ptr<ObjectMeshDynamic> &object, float width, float height, float step, const Math::mat4 &transform);
	UNIGINE_API Ptr<ObjectMeshDynamic> createPlane(float width, float height, float step);
	UNIGINE_API void addSphereSurface(Ptr<ObjectMeshDynamic> &object, float radius, const Math::mat4 &transform, int stacks = 16, int slices = 32);
	UNIGINE_API Ptr<ObjectMeshDynamic> createSphere(float radius, int stacks = 16, int slices = 32);
	UNIGINE_API void addCapsuleSurface(Ptr<ObjectMeshDynamic> &object, float radius, float height, const Math::mat4 &transform, int stacks = 16, int slices = 32);
	UNIGINE_API Ptr<ObjectMeshDynamic> createCapsule(float radius, float height, int stacks = 16, int slices = 32);
	UNIGINE_API void addCylinderSurface(Ptr<ObjectMeshDynamic> &object, float radius, float height, const Math::mat4 &transform, int stacks = 1, int slices = 32);
	UNIGINE_API Ptr<ObjectMeshDynamic> createCylinder(float radius, float height, int stacks = 1, int slices = 32);
	UNIGINE_API void addPrismSurface(Ptr<ObjectMeshDynamic> &object, float size_0, float size_1, float height, const Math::mat4 &transform, int sides = 8);
	UNIGINE_API Ptr<ObjectMeshDynamic> createPrism(float size_0, float size_1, float height, int sides = 8);
	UNIGINE_API void addIcosahedronSurface(Ptr<ObjectMeshDynamic> &object, float radius, const Math::mat4 &transform);
	UNIGINE_API Ptr<ObjectMeshDynamic> createIcosahedron(float radius);
	UNIGINE_API void addDodecahedronSurface(Ptr<ObjectMeshDynamic> &object, float radius, const Math::mat4 &transform);
	UNIGINE_API Ptr<ObjectMeshDynamic> createDodecahedron(float radius);
} // namespace

} // namespace Unigine
