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
#include "UnigineMaterial.h"

namespace Unigine
{


class UNIGINE_API Camera : public APIInterface
{
public:

	enum
	{
		FOV_MODE_VERTICAL = 0,
		FOV_MODE_PHYSICALLY_BASED_CAMERA,
	};

	enum
	{
		FOV_FIXED_VERTICAL = 0,
		FOV_FIXED_HORIZONTAL,
	};
	static Ptr<Camera> create();
	void setModelview(const Math::Mat4 &modelview);
	Math::Mat4 getModelview() const;
	Math::Mat4 getIModelview() const;
	void setPosition(const Math::Vec3 &position);
	Math::Vec3 getPosition() const;
	void setOffset(const Math::mat4 &offset);
	Math::mat4 getOffset() const;
	void setProjection(const Math::mat4 &projection);
	Math::mat4 getProjection() const;
	Math::mat4 getAspectCorrectedProjection(float aspect) const;
	void setFovMode(int mode);
	int getFovMode() const;
	int getFovFixed() const;
	void setFov(float fov);
	float getFov() const;
	void setFilmGate(float gate);
	float getFilmGate() const;
	void setFocalLength(float length);
	float getFocalLength() const;
	void setZNear(float znear);
	float getZNear() const;
	void setZFar(float zfar);
	float getZFar() const;
	void setUp(const Math::vec3 &up);
	Math::vec3 getUp() const;
	void setObliqueFrustumPlane(const Math::Vec4 &plane);
	Math::Vec4 getObliqueFrustumPlane() const;
	void setObliqueFrustum(bool frustum);
	bool isObliqueFrustum() const;
	void setViewportMask(int mask);
	int getViewportMask() const;
	void setReflectionViewportMask(int mask);
	int getReflectionViewportMask() const;
	void setSourceMask(int mask);
	int getSourceMask() const;
	void setReverbMask(int mask);
	int getReverbMask() const;
	void clearScriptableMaterials();
	int getNumScriptableMaterials() const;
	Ptr<Material> getScriptableMaterial(int num) const;
	void setScriptableMaterial(int num, const Ptr<Material> &material);
	void insertScriptableMaterial(int num, const Ptr<Material> &material);
	int findScriptableMaterial(const Ptr<Material> &material) const;
	void addScriptableMaterial(const Ptr<Material> &material);
	void removeScriptableMaterial(int num);
	void swapScriptableMaterials(int num_0, int num_1);
	void setScriptableMaterialEnabled(int num, bool enabled);
	bool getScriptableMaterialEnabled(int num) const;
	void getDirectionFromScreen(Math::Vec3 &p0, Math::Vec3 &p1, float screen_x, float screen_y, float aspect) const;
	Math::vec3 getDirectionFromScreen(float screen_x, float screen_y, float aspect) const;
	Math::mat4 getProjectionFromScreen(float screen_x0, float screen_y0, float screen_x1, float screen_y1, float aspect) const;
	int getScreenPosition(float &screen_x, float &screen_y, const Math::Vec3 &point, float aspect) const;
	Ptr<Camera> clone() const;
	Ptr<Camera> copy(const Ptr<Camera> &camera) const;
	void swap(const Ptr<Camera> &camera);
};
typedef Ptr<Camera> CameraPtr;

} // namespace Unigine
