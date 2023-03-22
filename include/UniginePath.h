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


class UNIGINE_API Path : public APIInterface
{
public:
	static Ptr<Path> create();
	static Ptr<Path> create(const char *name);
	static Ptr<Path> create(const Ptr<Path> &path);
	int addFrame();
	void removeFrame(int num);
	bool load(const char *name);
	bool save(const char *name) const;
	void clear();
	Math::vec3 getAngularVelocity(float time, bool loop = false) const;
	Math::Vec3 getLinearVelocity(float time, bool loop = false) const;
	int getClosestFrame(const Math::Vec3 &position) const;
	float getClosestTime(const Math::Vec3 &position) const;
	Math::Vec3 getFramePosition(int num) const;
	void setFramePosition(int num, const Math::Vec3 &pos);
	Math::quat getFrameRotation(int num) const;
	void setFrameRotation(int num, const Math::quat &rot);
	Math::vec3 getFrameScale(int num) const;
	void setFrameScale(int num, const Math::vec3 &scale);
	float getFrameTime(int num) const;
	void setFrameTime(int num, float time);
	Math::Mat4 getFrameTransform(int num) const;
	void setFrameTransform(int num, const Math::Mat4 &transform);
	void setNumFrames(int frames);
	int getNumFrames() const;
	Math::Vec3 getPosition(float time, bool loop = false) const;
	Math::quat getRotation(float time, bool loop = false) const;
	Math::vec3 getScale(float time, bool loop = false) const;
	Math::Mat4 getTransform(float time, bool loop = false) const;
};
typedef Ptr<Path> PathPtr;

} // namespace Unigine
