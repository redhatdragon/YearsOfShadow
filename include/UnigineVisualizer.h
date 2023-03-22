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

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Visualizer
{
protected:
	

public:
	static int isInitialized(); 

	enum MODE
	{
		MODE_DISABLED,
		MODE_ENABLED_DEPTH_TEST_ENABLED,
		MODE_ENABLED_DEPTH_TEST_DISABLED,
	};
	static void setMode(Visualizer::MODE mode);
	static Visualizer::MODE getMode();
	static void setEnabled(bool enabled);
	static bool isEnabled();
	static void setSize(int size);
	static int getSize();
	static void setTextureName(const char *name);
	static const char *getTextureName();
	static void clear();
	static void renderPoint3D(const Math::Vec3 &v, float size, const Math::vec4 &color, bool screen_space = false, float duration = 0.0f, bool depth_test = true);
	static void renderLine3D(const Math::Vec3 &v0, const Math::Vec3 &v1, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderLine3D(const Math::Vec3 &v0, const Math::Vec3 &v1, const Math::Vec3 &v2, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderLine3D(const Math::Vec3 &v0, const Math::Vec3 &v1, const Math::Vec3 &v2, const Math::Vec3 &v3, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderTriangle3D(const Math::Vec3 &v0, const Math::Vec3 &v1, const Math::Vec3 &v2, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderQuad3D(const Math::Vec3 &v0, const Math::Vec3 &v1, const Math::Vec3 &v2, const Math::Vec3 &v3, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderBillboard3D(const Math::Vec3 &v, float size, const Math::vec4 &texcoord, bool screen_space = false, float duration = 0.0f, bool depth_test = true);
	static void renderPoint2D(const Math::vec2 &v, float size, const Math::vec4 &color, float order = 0.0f, float duration = 0.0f);
	static void renderLine2D(const Math::vec2 &v0, const Math::vec2 &v1, const Math::vec4 &color, float order = 0.0f, float duration = 0.0f);
	static void renderLine2D(const Math::vec2 &v0, const Math::vec2 &v1, const Math::vec2 &v2, const Math::vec4 &color, float order = 0.0f, float duration = 0.0f);
	static void renderLine2D(const Math::vec2 &v0, const Math::vec2 &v1, const Math::vec2 &v2, const Math::vec2 &v3, const Math::vec4 &color, float order = 0.0f, float duration = 0.0f);
	static void renderTriangle2D(const Math::vec2 &v0, const Math::vec2 &v1, const Math::vec2 &v2, const Math::vec4 &color, float order = 0.0f, float duration = 0.0f);
	static void renderQuad2D(const Math::vec2 &v0, const Math::vec2 &v1, const Math::vec2 &v2, const Math::vec2 &v3, const Math::vec4 &color, float order = 0.0f, float duration = 0.0f);
	static void renderVector(const Math::Vec3 &position_start, const Math::Vec3 &position_end, const Math::vec4 &color, float arrow_size = 0.25f, bool screen_space = false, float duration = 0.0f, bool depth_test = true);
	static void renderDirection(const Math::Vec3 &position, const Math::vec3 &direction, const Math::vec4 &color, float arrow_size = 0.25f, bool screen_space = true, float duration = 0.0f, bool depth_test = true);
	static void renderBox(const Math::vec3 &size, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderFrustum(const Math::mat4 &projection, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderCircle(float radius, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderSector(float radius, float angle, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderCone(float radius, float angle, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderSphere(float radius, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderCapsule(float radius, float height, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderCylinder(float radius, float height, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderEllipse(const Math::vec3 &radius, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderSolidBox(const Math::vec3 &size, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderSolidSphere(float radius, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderSolidCapsule(float radius, float height, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderSolidCylinder(float radius, float height, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderSolidEllipse(const Math::vec3 &radius, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderRectangle(const Math::vec4 &rectangle, const Math::vec4 &color, float duration = 0.0f);
	static void renderBoundBox(const Math::BoundBox &bb, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderBoundSphere(const Math::BoundSphere &bs, const Math::Mat4 &transform, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderNodeBoundBox(const Ptr<Node> &node, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderNodeBoundSphere(const Ptr<Node> &node, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderObjectSurfaceBoundBox(const Ptr<Object> &object, int surface, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderObjectSurfaceBoundSphere(const Ptr<Object> &object, int surface, const Math::vec4 &color, float duration = 0.0f, bool depth_test = true);
	static void renderNodeHandler(const Ptr<Node> &node, float duration = 0.0f);
	static void renderObject(const Ptr<Object> &object, const Math::vec4 &color, float duration = 0.0f);
	static void renderObjectSurface(const Ptr<Object> &object, int surface, const Math::vec4 &color, float duration = 0.0f);
	static void renderSolidObject(const Ptr<Object> &object, const Math::vec4 &color, float duration = 0.0f);
	static void renderSolidObjectSurface(const Ptr<Object> &object, int surface, const Math::vec4 &color, float duration = 0.0f);
	static void renderMessage2D(const Math::vec3 &position, const Math::vec3 &center, const char *str, const Math::vec4 &color, int outline = 0, int font_size = -1, float duration = 0.0f);
	static void renderMessage3D(const Math::Vec3 &position, const Math::vec3 &center, const char *str, const Math::vec4 &color, int outline = 0, int font_size = -1, float duration = 0.0f);
};

} // namespace Unigine
