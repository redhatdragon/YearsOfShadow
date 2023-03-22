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
#include "UnigineTextures.h"
#include "UnigineImage.h"

namespace Unigine
{

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Field : public Node
{
public:
	static bool convertible(Node *node) { return node && node->isField(); }

	void setFieldMask(int mask);
	int getFieldMask() const;
	void setViewportMask(int mask);
	int getViewportMask() const;
};
typedef Ptr<Field> FieldPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API FieldAnimation : public Field
{
public:
	static int type() { return Node::FIELD_ANIMATION; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<FieldAnimation> create(const Math::vec3 &size);
	void setEllipse(bool ellipse);
	bool isEllipse() const;
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
	void setAttenuation(float attenuation);
	float getAttenuation() const;
	void setStem(float stem);
	float getStem() const;
	void setLeaf(float leaf);
	float getLeaf() const;
	void setAnimationScale(float scale);
	float getAnimationScale() const;
	void setWind(const Math::vec3 &wind);
	Math::vec3 getWind() const;
};
typedef Ptr<FieldAnimation> FieldAnimationPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API FieldHeight : public Field
{
public:
	static int type() { return Node::FIELD_HEIGHT; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<FieldHeight> create();
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
	void setAttenuation(float attenuation);
	float getAttenuation() const;
	void setPower(float power);
	float getPower() const;
	void setOrder(int order);
	int getOrder() const;
	void setBlendMode(int mode);
	int getBlendMode() const;

	enum
	{
		BLEND_ADDITIVE = 0,
		BLEND_MULTIPLICATIVE,
	};
	void setTexturePath(const char *path);
	const char *getTexturePath() const;
	int setTextureImage(const Ptr<Image> &image);
	int getTextureImage(const Ptr<Image> &image) const;
	void setTexture(const Ptr<Texture> &texture);
	Ptr<Texture> getTexture() const;
};
typedef Ptr<FieldHeight> FieldHeightPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API FieldWeather : public Field
{
public:
	static int type() { return Node::FIELD_WEATHER; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<FieldWeather> create();
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
	void setAttenuationType(int type);
	int getAttenuationType() const;
	void setAttenuation(float attenuation);
	float getAttenuation() const;
	void setPower(float power);
	float getPower() const;
	void setIntensity(float intensity);
	float getIntensity() const;
	void setTexturePath(const char *path);
	const char *getTexturePath() const;
	int setTextureImage(const Ptr<Image> &image);
	int getTextureImage(const Ptr<Image> &image) const;
	void setTexture(const Ptr<Texture> &texture);
	Ptr<Texture> getTexture() const;
	void setWindAnimation(const Math::vec3 &animation);
	Math::vec3 getWindAnimation() const;
	void setWindEnabled(int enabled);
	int getWindEnabled() const;
	void setCloudsAnimationOffset(const Math::vec3 &offset);
	Math::vec3 getCloudsAnimationOffset() const;
};
typedef Ptr<FieldWeather> FieldWeatherPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API FieldShoreline : public Field
{
public:
	static int type() { return Node::FIELD_SHORELINE; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<FieldShoreline> create();
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
	void setTexturePath(const char *path);
	const char *getTexturePath() const;
	int setTextureImage(const Ptr<Image> &image);
	int getTextureImage(const Ptr<Image> &image) const;
	void setTexture(const Ptr<Texture> &texture);
	Ptr<Texture> getTexture() const;
	int bakeWaterLevel(const Ptr<Image> &image);
	int createShorelineDistanceField(const Ptr<Image> &image, int shoreline_radius, int blur_radius, int downsample_resolution);
	void *addProgressCallback(CallbackBase1<float> *func);
	bool removeProgressCallback(void *id);
	void clearProgressCallbacks();
};
typedef Ptr<FieldShoreline> FieldShorelinePtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API FieldSpacer : public Field
{
public:
	static int type() { return Node::FIELD_SPACER; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<FieldSpacer> create(const Math::vec3 &size);
	void setEllipse(bool ellipse);
	bool isEllipse() const;
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
	void setAttenuation(float attenuation);
	float getAttenuation() const;
};
typedef Ptr<FieldSpacer> FieldSpacerPtr;

} // namespace Unigine
