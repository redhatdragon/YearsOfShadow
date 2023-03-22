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

#include "UnigineBase.h"
#include "UnigineObjects.h"
#include "UniginePhysics.h"
#include "UnigineImage.h"

namespace Unigine
{

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Physical : public Node
{
public:
	static bool convertible(Node *node) { return node && node->isPhysical(); }

	void setPhysicalMask(int mask);
	int getPhysicalMask() const;
};
typedef Ptr<Physical> PhysicalPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API PhysicalForce : public Physical
{
public:
	static int type() { return Node::PHYSICAL_FORCE; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<PhysicalForce> create(float radius);
	void setAttenuation(float attenuation);
	float getAttenuation() const;
	void setAttractor(float attractor);
	float getAttractor() const;
	void setRadius(float radius);
	float getRadius() const;
	void setRotator(float rotator);
	float getRotator() const;
};
typedef Ptr<PhysicalForce> PhysicalForcePtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API PhysicalTrigger : public Physical
{
public:
	static int type() { return Node::PHYSICAL_TRIGGER; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<PhysicalTrigger> create(Shape::TYPE type, const Math::vec3 &size);
	Ptr<Body> getBody(int num);
	void setCollisionMask(int mask);
	int getCollisionMask() const;
	float getContactDepth(int contact) const;
	Math::vec3 getContactNormal(int contact) const;
	Ptr<Object> getContactObject(int contact) const;
	Math::Vec3 getContactPoint(int contact) const;
	Ptr<Shape> getContactShape(int contact) const;
	int getContactSurface(int contact) const;
	void setEnterCallbackName(const char *name);
	const char *getEnterCallbackName() const;
	void setExclusionMask(int mask);
	int getExclusionMask() const;
	void setLeaveCallbackName(const char *name);
	const char *getLeaveCallbackName() const;
	int getNumBodies() const;
	int getNumContacts() const;
	void setShapeType(int type);
	int getShapeType() const;
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
	void updateContacts();
	void *addEnterCallback(CallbackBase1<Ptr<Body>> *func);
	bool removeEnterCallback(void *id);
	void clearEnterCallbacks();
	void *addLeaveCallback(CallbackBase1<Ptr<Body>> *func);
	bool removeLeaveCallback(void *id);
	void clearLeaveCallbacks();
};
typedef Ptr<PhysicalTrigger> PhysicalTriggerPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API PhysicalWater : public Physical
{
public:
	static int type() { return Node::PHYSICAL_WATER; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<PhysicalWater> create(const Math::vec3 &size);
	void setAngularDamping(float damping);
	float getAngularDamping() const;
	Ptr<Body> getContactBody(int num) const;
	float getContactDepth(int num) const;
	Math::vec3 getContactForce(int num) const;
	Math::Vec3 getContactPoint(int num) const;
	Math::vec3 getContactVelocity(int num) const;
	void setDensity(float density);
	float getDensity() const;
	void setLinearDamping(float damping);
	float getLinearDamping() const;
	int getNumContacts() const;
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
	void setVelocity(const Math::vec3 &velocity);
	Math::vec3 getVelocity() const;
};
typedef Ptr<PhysicalWater> PhysicalWaterPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API PhysicalWind : public Physical
{
public:
	static int type() { return Node::PHYSICAL_WIND; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<PhysicalWind> create(const Math::vec3 &size);
	void setAngularDamping(float damping);
	float getAngularDamping() const;
	void setLinearDamping(float damping);
	float getLinearDamping() const;
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
	void setThreshold(const Math::vec3 &threshold);
	Math::vec3 getThreshold() const;
	void setVelocity(const Math::vec3 &velocity);
	Math::vec3 getVelocity() const;
};
typedef Ptr<PhysicalWind> PhysicalWindPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API PhysicalNoise : public Physical
{
public:
	static int type() { return Node::PHYSICAL_NOISE; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<PhysicalNoise> create(const Math::vec3 &size);
	Ptr<Image> getImage() const;
	void setForce(float force);
	float getForce() const;
	void setFrequency(int frequency);
	int getFrequency() const;
	void setImageSize(int size);
	int getImageSize() const;
	void setOffset(const Math::vec3 &offset);
	Math::vec3 getOffset() const;
	void setNoiseScale(float scale);
	float getNoiseScale() const;
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
	void setStep(const Math::vec3 &step);
	Math::vec3 getStep() const;
	void setThreshold(const Math::vec3 &threshold);
	Math::vec3 getThreshold() const;
};
typedef Ptr<PhysicalNoise> PhysicalNoisePtr;

} // namespace Unigine
