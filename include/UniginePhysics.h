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

#include "UnigineXml.h"
#include "UnigineStreams.h"
#include "UnigineObjects.h"
#include "UniginePath.h"
#include "UnigineMaterial.h"

namespace Unigine
{

// forward decls
class Body;
class BodyRigid;
class Shape;
class Joint;


class UNIGINE_API PhysicsIntersection : public APIInterface
{
public:
	static bool convertible(PhysicsIntersection *obj) { return obj != nullptr; }

	enum TYPE
	{
		PHYSICS_INTERSECTION = 0,
		PHYSICS_INTERSECTION_NORMAL,
		NUM_PHYSICS_INTERSECTIONS,
	};
	PhysicsIntersection::TYPE getType() const;
	const char *getTypeName() const;
	static Ptr<PhysicsIntersection> create();
	void setPoint(const Math::Vec3 &point);
	Math::Vec3 getPoint() const;
	void setSurface(int surface);
	int getSurface() const;
	void setShape(const Ptr<Shape> &shape);
	Ptr<Shape> getShape() const;
};
typedef Ptr<PhysicsIntersection> PhysicsIntersectionPtr;


class UNIGINE_API PhysicsIntersectionNormal : public PhysicsIntersection
{
public:
	static bool convertible(PhysicsIntersection *obj) { return obj && obj->getType() == PhysicsIntersection::PHYSICS_INTERSECTION_NORMAL; }
	static Ptr<PhysicsIntersectionNormal> create();
	void setNormal(const Math::vec3 &normal);
	Math::vec3 getNormal() const;
};
typedef Ptr<PhysicsIntersectionNormal> PhysicsIntersectionNormalPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Physics
{
protected:
	

public:
	static int isInitialized(); 

	enum UPDATE_MODE
	{
		UPDATE_MODE_BEFORE_RENDERING = 0,
		UPDATE_MODE_ASYNC_RENDERING,
	};
	static void setEnabled(bool enable);
	static bool isEnabled();
	static void setStableFPS(bool stablefps);
	static bool isStableFPS();
	static void setDeterminism(bool determinism); ///< "determinism" mode means that objects are updated in a specific order
	static bool isDeterminism();
	static void setSyncEngineUpdateWithPhysics(bool physics); ///< "fixed" mode caps render FPS to physics FPS (otherwise physics state can be interpolated between rendered frames)
	static bool isSyncEngineUpdateWithPhysics();
	static void setUpdateMode(Physics::UPDATE_MODE mode);
	static Physics::UPDATE_MODE getUpdateMode();
	static void setMissedFrameLifetime(float lifetime);
	static float getMissedFrameLifetime();
	static void setBudget(float budget); ///< physics time budget, in seconds, defaults to 0.05 sec
	static float getBudget();
	static void setDistance(float distance); ///< max physics simulation distance
	static float getDistance();
	static void setNumIterations(int iterations);
	static int getNumIterations();
	static void setIFps(float ifps);
	static float getIFps();
	static void setScale(float scale);
	static float getScale();
	static void setLinearDamping(float damping);
	static float getLinearDamping();
	static void setAngularDamping(float damping);
	static float getAngularDamping();
	static void setMaxAngularVelocity(float velocity);
	static float getMaxAngularVelocity();
	static void setMaxLinearVelocity(float velocity);
	static float getMaxLinearVelocity();
	static void setFrozenLinearVelocity(float velocity);
	static float getFrozenLinearVelocity();
	static void setFrozenAngularVelocity(float velocity);
	static float getFrozenAngularVelocity();
	static void setNumFrozenFrames(int frames);
	static int getNumFrozenFrames();
	static void setPenetrationFactor(float factor);
	static float getPenetrationFactor();
	static void setPenetrationTolerance(float tolerance);
	static float getPenetrationTolerance();
	static void setGravity(const Math::vec3 &gravity);
	static Math::vec3 getGravity();
	static void setData(const char *data); ///< world user data, stored as is in a "data" element in .world XML
	static const char *getData();
	static void setCurrentSubframeTime(float time);
	static float getCurrentSubframeTime();
	static int getFrame();
	static int getNumBodies();
	static int getNumContacts();
	static int getNumIslands();
	static int getNumJoints();
	static float getCollisionTime();
	static float getIntegrateTime();
	static float getResponseTime();
	static float getSimulationTime();
	static float getTotalTime();
	static int saveScene();
	static int restoreScene(int id);
	static int removeScene(int id);
	static bool loadSettings(const char *name, bool clear = false);
	static bool saveSettings(const char *name, int force = 0);
	static bool loadWorld(const Ptr<Xml> &xml);
	static bool saveWorld(const Ptr<Xml> &xml, int force = 0);
	static bool saveState(const Ptr<Stream> &stream);
	static bool restoreState(const Ptr<Stream> &stream);
	static Ptr<Object> getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, const Ptr<PhysicsIntersection> &intersection);
	static Ptr<Object> getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, const Ptr<PhysicsIntersectionNormal> &intersection);
	static Ptr<Object> getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, const Vector<Ptr<Node>> &exclude, const Ptr<PhysicsIntersection> &intersection);
	static Ptr<Object> getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, const Vector<Ptr<Node>> &exclude, const Ptr<PhysicsIntersectionNormal> &intersection);
	static bool isBody(int id);
	static Ptr<Body> getBody(int id);
	static bool isJoint(int id);
	static Ptr<Joint> getJoint(int id);
	static bool isShape(int id);
	static Ptr<Shape> getShape(int id);
	static void addUpdateNode(const Ptr<Node> &node);
	static void addUpdateNodes(const Vector<Ptr<Node>> &nodes);
};


class UNIGINE_API ShapeContact : public APIInterface
{
public:
	static Ptr<ShapeContact> create();
	void setID(int id);
	int getID() const;
	void setSurface(int surface);
	int getSurface() const;
	void setTime(float time);
	float getTime() const;
	void setDepth(float depth);
	float getDepth() const;
	void setPoint(const Math::Vec3 &point);
	Math::Vec3 getPoint() const;
	void setNormal(const Math::vec3 &normal);
	Math::vec3 getNormal() const;
	void setShape0(const Ptr<Shape> &shape0);
	Ptr<Shape> getShape0() const;
	void setShape1(const Ptr<Shape> &shape1);
	Ptr<Shape> getShape1() const;
	void setObject(const Ptr<Object> &val);
	Ptr<Object> getObject() const;
};
typedef Ptr<ShapeContact> ShapeContactPtr;


class UNIGINE_API Shape : public APIInterface
{
public:
	static bool convertible(Shape *obj) { return obj != nullptr; }

	enum TYPE
	{
		SHAPE_SPHERE = 0,
		SHAPE_CAPSULE,
		SHAPE_CYLINDER,
		SHAPE_BOX,
		SHAPE_CONVEX,
		NUM_SHAPES,
	};
	static Ptr<Shape> createShape(int type);
	static Ptr<Shape> createShape(const char *type_name);
	void setID(int id);
	int getID() const;
	Shape::TYPE getType() const;
	static const char *getTypeName(int type);
	const char *getTypeName() const;
	int getNumber() const;
	void setBody(const Ptr<Body> &body);
	Ptr<Body> getBody() const;
	void setEnabled(bool enable);
	bool isEnabled() const;
	bool isEnabledSelf() const;
	void setContinuous(bool continuous);
	bool isContinuous() const;
	void setName(const char *name);
	const char *getName() const;
	void setPhysicsIntersectionMask(int mask);
	int getPhysicsIntersectionMask() const;
	void setCollisionMask(int mask);
	int getCollisionMask() const;
	void setExclusionMask(int mask);
	int getExclusionMask() const;
	void setMass(float mass);
	float getMass() const;
	void setDensity(float density);
	float getDensity() const;
	void setFriction(float friction);
	float getFriction() const;
	void setRestitution(float restitution);
	float getRestitution() const;
	Math::vec3 getArea() const;
	float getVolume() const;
	Math::mat3 getInertia() const;
	Math::vec3 getCenterOfMass() const;
	void setTransform(const Math::Mat4 &transform);
	Math::Mat4 getTransform() const;
	void setBodyShapeTransform(const Math::mat4 &transform);
	Math::mat4 getBodyShapeTransform() const;
	void setPosition(const Math::Vec3 &position);
	Math::Vec3 getPosition() const;
	bool isIdentity() const;
	void setVelocity(const Math::vec3 &velocity, float ifps);
	Math::vec3 getVelocity() const;
	int getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, Math::Vec3 *ret_point, Math::vec3 *ret_normal) const;
	int getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, const Ptr<PhysicsIntersection> &intersection) const;
	int getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, const Ptr<PhysicsIntersectionNormal> &intersection) const;
	int getCollision(Vector<Ptr<ShapeContact>> &contacts, float ifps = 0.0f) const;
	int getCollision(const Ptr<Object> &object, Vector<Ptr<ShapeContact>> &contacts, float ifps = 0.0f) const;
	void renderVisualizer(const Math::vec4 &color);
	Ptr<Shape> clone() const;
	void swap(const Ptr<Shape> &shape);
	int saveState(const Ptr<Stream> &stream) const;
	int restoreState(const Ptr<Stream> &stream);
};
typedef Ptr<Shape> ShapePtr;


class UNIGINE_API ShapeSphere : public Shape
{
public:
	static bool convertible(Shape *obj) { return obj && obj->getType() == Shape::SHAPE_SPHERE; }
	static Ptr<ShapeSphere> create();
	static Ptr<ShapeSphere> create(float radius);
	static Ptr<ShapeSphere> create(const Ptr<Body> &body, float radius);
	void setRadius(float radius);
	float getRadius() const;
	void setCenter(const Math::Vec3 &center);
	Math::Vec3 getCenter() const;
};
typedef Ptr<ShapeSphere> ShapeSpherePtr;


class UNIGINE_API ShapeCapsule : public Shape
{
public:
	static bool convertible(Shape *obj) { return obj && obj->getType() == Shape::SHAPE_CAPSULE; }
	static Ptr<ShapeCapsule> create();
	static Ptr<ShapeCapsule> create(float radius, float height);
	static Ptr<ShapeCapsule> create(const Ptr<Body> &body, float radius, float height);
	void setRadius(float radius);
	float getRadius() const;
	void setHeight(float height);
	float getHeight() const;
	Math::Vec3 getBottomCap() const;
	Math::Vec3 getTopCap() const;
};
typedef Ptr<ShapeCapsule> ShapeCapsulePtr;


class UNIGINE_API ShapeCylinder : public Shape
{
public:
	static bool convertible(Shape *obj) { return obj && obj->getType() == Shape::SHAPE_CYLINDER; }
	static Ptr<ShapeCylinder> create();
	static Ptr<ShapeCylinder> create(float radius, float height);
	static Ptr<ShapeCylinder> create(const Ptr<Body> &body, float radius, float height);
	void setRadius(float radius);
	float getRadius() const;
	void setHeight(float height);
	float getHeight() const;
};
typedef Ptr<ShapeCylinder> ShapeCylinderPtr;


class UNIGINE_API ShapeBox : public Shape
{
public:
	static bool convertible(Shape *obj) { return obj && obj->getType() == Shape::SHAPE_BOX; }
	static Ptr<ShapeBox> create();
	static Ptr<ShapeBox> create(const Math::vec3 &size);
	static Ptr<ShapeBox> create(const Ptr<Body> &body, const Math::vec3 &size);
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
};
typedef Ptr<ShapeBox> ShapeBoxPtr;


class UNIGINE_API ShapeConvex : public Shape
{
public:
	static bool convertible(Shape *obj) { return obj && obj->getType() == Shape::SHAPE_CONVEX; }
	static Ptr<ShapeConvex> create();
	static Ptr<ShapeConvex> create(const Ptr<Object> &object, int surface);
	static Ptr<ShapeConvex> create(const Ptr<Body> &body, const Ptr<Object> &object, int surface);
	int setObject(const Ptr<Object> &object, int surface, float error = 0.01);
};
typedef Ptr<ShapeConvex> ShapeConvexPtr;


class UNIGINE_API Joint : public APIInterface
{
public:
	static bool convertible(Joint *obj) { return obj != nullptr; }

	enum TYPE
	{
		JOINT_FIXED = 0,
		JOINT_BALL,
		JOINT_HINGE,
		JOINT_PRISMATIC,
		JOINT_CYLINDRICAL,
		JOINT_SUSPENSION,
		JOINT_WHEEL,
		JOINT_PARTICLES,
		JOINT_PATH,
		NUM_JOINTS,
	};
	static Ptr<Joint> createJoint(int type);
	static Ptr<Joint> createJoint(const char *type_name);
	int setID(int id);
	int getID() const;
	Joint::TYPE getType() const;
	static const char *getTypeName(int type);
	const char *getTypeName() const;
	void setNode0(const Ptr<Node> &node0);
	Ptr<Node> getNode0() const;
	void setNode1(const Ptr<Node> &node1);
	Ptr<Node> getNode1() const;
	void setBody0(const Ptr<Body> &body);
	Ptr<Body> getBody0() const;
	void setBody1(const Ptr<Body> &body);
	Ptr<Body> getBody1() const;
	Ptr<BodyRigid> getBodyRigid0();
	Ptr<BodyRigid> getBodyRigid1();
	void setEnabled(bool enable);
	bool isEnabled() const;
	bool isEnabledSelf() const;
	void setCollision(int c);
	int getCollision() const;
	void setBroken(bool broken);
	bool isBroken() const;
	void setFrozen(bool f);
	bool isFrozen() const;
	void setName(const char *name);
	const char *getName() const;
	void setNumIterations(int num_iterations);
	int getNumIterations() const;
	void setMaxForce(float force);
	float getMaxForce() const;
	void setMaxTorque(float torque);
	float getMaxTorque() const;
	void setLinearRestitution(float restitution);
	float getLinearRestitution() const;
	void setAngularRestitution(float restitution);
	float getAngularRestitution() const;
	void setLinearSoftness(float softness);
	float getLinearSoftness() const;
	void setAngularSoftness(float softness);
	float getAngularSoftness() const;
	void setAnchor0(const Math::Vec3 &anchor);
	Math::Vec3 getAnchor0() const;
	void setAnchor1(const Math::Vec3 &anchor);
	Math::Vec3 getAnchor1() const;
	void setWorldAnchor(const Math::Vec3 &anchor);
	Math::Vec3 getWorldAnchor() const;
	void *addBrokenCallback(CallbackBase1<Ptr<Joint>> *func);
	bool removeBrokenCallback(void *id);
	void clearBrokenCallbacks();
	void renderVisualizer(const Math::vec4 &color);
	Ptr<Joint> clone() const;
	void swap(const Ptr<Joint> &joint);
	int saveState(const Ptr<Stream> &stream) const;
	int restoreState(const Ptr<Stream> &stream);
};
typedef Ptr<Joint> JointPtr;


class UNIGINE_API JointFixed : public Joint
{
public:
	static bool convertible(Joint *obj) { return obj && obj->getType() == Joint::JOINT_FIXED; }
	static Ptr<JointFixed> create();
	static Ptr<JointFixed> create(const Ptr<Body> &body0, const Ptr<Body> &body1);
	static Ptr<JointFixed> create(const Ptr<Body> &body0, const Ptr<Body> &body1, const Math::Vec3 & anchor);
	void setRotation0(const Math::mat3 &rotation0);
	Math::mat3 getRotation0() const;
	void setRotation1(const Math::mat3 &rotation1);
	Math::mat3 getRotation1() const;
	void setWorldRotation(const Math::mat3 &rotation);
	Math::mat3 getWorldRotation() const;
};
typedef Ptr<JointFixed> JointFixedPtr;


class UNIGINE_API JointBall : public Joint
{
public:
	static bool convertible(Joint *obj) { return obj && obj->getType() == Joint::JOINT_BALL; }
	static Ptr<JointBall> create();
	static Ptr<JointBall> create(const Ptr<Body> &body0, const Ptr<Body> &body1);
	static Ptr<JointBall> create(const Ptr<Body> &body0, const Ptr<Body> &body1, const Math::Vec3 & anchor);
	static Ptr<JointBall> create(const Ptr<Body> &body0, const Ptr<Body> &body1, const Math::Vec3 & anchor, const Math::vec3 &axis);
	void setAxis0(const Math::vec3 &axis0);
	Math::vec3 getAxis0() const;
	void setAxis1(const Math::vec3 &axis1);
	Math::vec3 getAxis1() const;
	void setAngularDamping(float damping);
	float getAngularDamping() const;
	void setAngularLimitAngle(float angle);
	float getAngularLimitAngle() const;
	void setAngularLimitFrom(float from);
	float getAngularLimitFrom() const;
	void setAngularLimitTo(float to);
	float getAngularLimitTo() const;
	void setWorldAxis(const Math::vec3 &axis);
	Math::vec3 getWorldAxis() const;
};
typedef Ptr<JointBall> JointBallPtr;


class UNIGINE_API JointCylindrical : public Joint
{
public:
	static bool convertible(Joint *obj) { return obj && obj->getType() == Joint::JOINT_CYLINDRICAL; }
	static Ptr<JointCylindrical> create();
	static Ptr<JointCylindrical> create(const Ptr<Body> &body0, const Ptr<Body> &body1);
	static Ptr<JointCylindrical> create(const Ptr<Body> &body0, const Ptr<Body> &body1, const Math::Vec3 & anchor, const Math::vec3 &axis);
	void setAxis0(const Math::vec3 &axis0);
	Math::vec3 getAxis0() const;
	void setAxis1(const Math::vec3 &axis1);
	Math::vec3 getAxis1() const;
	void setWorldAxis(const Math::vec3 &axis);
	Math::vec3 getWorldAxis() const;
	void setAngularAngle(float angle);
	float getAngularAngle() const;
	void setAngularDamping(float damping);
	float getAngularDamping() const;
	void setAngularTorque(float torque);
	float getAngularTorque() const;
	void setAngularLimitFrom(float from);
	float getAngularLimitFrom() const;
	void setAngularLimitTo(float to);
	float getAngularLimitTo() const;
	void setAngularSpring(float spring);
	float getAngularSpring() const;
	void setAngularVelocity(float velocity);
	float getAngularVelocity() const;
	void setLinearDamping(float damping);
	float getLinearDamping() const;
	void setLinearDistance(float distance);
	float getLinearDistance() const;
	void setLinearForce(float force);
	float getLinearForce() const;
	void setLinearLimitFrom(float from);
	float getLinearLimitFrom() const;
	void setLinearLimitTo(float to);
	float getLinearLimitTo() const;
	void setLinearSpring(float spring);
	float getLinearSpring() const;
	void setLinearVelocity(float velocity);
	float getLinearVelocity() const;
	float getCurrentAngularAngle() const;
	float getCurrentAngularVelocity() const;
	float getCurrentLinearDistance() const;
	float getCurrentLinearVelocity() const;
};
typedef Ptr<JointCylindrical> JointCylindricalPtr;


class UNIGINE_API JointHinge : public Joint
{
public:
	static bool convertible(Joint *obj) { return obj && obj->getType() == Joint::JOINT_HINGE; }
	static Ptr<JointHinge> create();
	static Ptr<JointHinge> create(const Ptr<Body> &body0, const Ptr<Body> &body1);
	static Ptr<JointHinge> create(const Ptr<Body> &body0, const Ptr<Body> &body1, const Math::Vec3 & anchor, const Math::vec3 &axis);
	void setAxis0(const Math::vec3 &axis0);
	Math::vec3 getAxis0() const;
	void setAxis1(const Math::vec3 &axis1);
	Math::vec3 getAxis1() const;
	void setWorldAxis(const Math::vec3 &axis);
	Math::vec3 getWorldAxis() const;
	void setAngularAngle(float angle);
	float getAngularAngle() const;
	void setAngularDamping(float damping);
	float getAngularDamping() const;
	void setAngularTorque(float torque);
	float getAngularTorque() const;
	void setAngularLimitFrom(float from);
	float getAngularLimitFrom() const;
	void setAngularLimitTo(float to);
	float getAngularLimitTo() const;
	void setAngularSpring(float spring);
	float getAngularSpring() const;
	void setAngularVelocity(float velocity);
	float getAngularVelocity() const;
	float getCurrentAngularAngle() const;
	float getCurrentAngularVelocity() const;
};
typedef Ptr<JointHinge> JointHingePtr;


class UNIGINE_API JointParticles : public Joint
{
public:
	static bool convertible(Joint *obj) { return obj && obj->getType() == Joint::JOINT_PARTICLES; }
	static Ptr<JointParticles> create();
	static Ptr<JointParticles> create(const Ptr<Body> &body0, const Ptr<Body> &body1);
	static Ptr<JointParticles> create(const Ptr<Body> &body0, const Ptr<Body> &body1, const Math::Vec3 & anchor, const Math::vec3 &size);
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
	void setThreshold(float threshold);
	float getThreshold() const;
	void clearParticles();
	int getNumParticles() const;
	float getParticleMass(int num) const;
	Math::vec3 getParticlePosition(int num) const;
};
typedef Ptr<JointParticles> JointParticlesPtr;


class UNIGINE_API JointPath : public Joint
{
public:
	static bool convertible(Joint *obj) { return obj && obj->getType() == Joint::JOINT_PATH; }
	static Ptr<JointPath> create();
	static Ptr<JointPath> create(const Ptr<Body> &body0, const Ptr<Body> &body1);
	static Ptr<JointPath> create(const Ptr<Body> &body0, const Ptr<Body> &body1, const Math::Vec3 & anchor);
	void setLinearDamping(float damping);
	float getLinearDamping() const;
	void setLinearForce(float force);
	float getLinearForce() const;
	void setLinearVelocity(float velocity);
	float getLinearVelocity() const;
	void setRotation0(const Math::mat3 &rotation0);
	Math::mat3 getRotation0() const;
	void setWorldRotation(const Math::mat3 &rotation);
	Math::mat3 getWorldRotation() const;
	float getCurrentLinearVelocity() const;
};
typedef Ptr<JointPath> JointPathPtr;


class UNIGINE_API JointPrismatic : public Joint
{
public:
	static bool convertible(Joint *obj) { return obj && obj->getType() == Joint::JOINT_PRISMATIC; }
	static Ptr<JointPrismatic> create();
	static Ptr<JointPrismatic> create(const Ptr<Body> &body0, const Ptr<Body> &body1);
	static Ptr<JointPrismatic> create(const Ptr<Body> &body0, const Ptr<Body> &body1, const Math::Vec3 & anchor, const Math::vec3 &size);
	void setAxis0(const Math::vec3 &axis0);
	Math::vec3 getAxis0() const;
	void setWorldAxis(const Math::vec3 &axis);
	Math::vec3 getWorldAxis() const;
	void setLinearDamping(float damping);
	float getLinearDamping() const;
	void setLinearDistance(float distance);
	float getLinearDistance() const;
	void setLinearForce(float force);
	float getLinearForce() const;
	void setLinearLimitFrom(float from);
	float getLinearLimitFrom() const;
	void setLinearLimitTo(float to);
	float getLinearLimitTo() const;
	void setLinearSpring(float spring);
	float getLinearSpring() const;
	void setLinearVelocity(float velocity);
	float getLinearVelocity() const;
	void setRotation0(const Math::mat3 &rotation0);
	Math::mat3 getRotation0() const;
	void setRotation1(const Math::mat3 &rotation1);
	Math::mat3 getRotation1() const;
	void setWorldRotation(const Math::mat3 &rotation);
	Math::mat3 getWorldRotation() const;
	float getCurrentLinearDistance() const;
	float getCurrentLinearVelocity() const;
};
typedef Ptr<JointPrismatic> JointPrismaticPtr;


class UNIGINE_API JointSuspension : public Joint
{
public:
	static bool convertible(Joint *obj) { return obj && obj->getType() == Joint::JOINT_SUSPENSION; }
	static Ptr<JointSuspension> create();
	static Ptr<JointSuspension> create(const Ptr<Body> &body0, const Ptr<Body> &body1);
	static Ptr<JointSuspension> create(const Ptr<Body> &body0, const Ptr<Body> &body1, const Math::Vec3 & anchor, const Math::vec3 &axis0, const Math::vec3 &axis1);
	void setAxis00(const Math::vec3 &axis00);
	Math::vec3 getAxis00() const;
	void setAxis10(const Math::vec3 &axis10);
	Math::vec3 getAxis10() const;
	void setAxis11(const Math::vec3 &axis11);
	Math::vec3 getAxis11() const;
	void setWorldAxis0(const Math::vec3 &axis0);
	Math::vec3 getWorldAxis0() const;
	void setWorldAxis1(const Math::vec3 &axis1);
	Math::vec3 getWorldAxis1() const;
	void setLinearDamping(float damping);
	float getLinearDamping() const;
	void setLinearDistance(float distance);
	float getLinearDistance() const;
	void setLinearLimitFrom(float from);
	float getLinearLimitFrom() const;
	void setLinearLimitTo(float to);
	float getLinearLimitTo() const;
	void setLinearSpring(float spring);
	float getLinearSpring() const;
	void setAngularDamping(float damping);
	float getAngularDamping() const;
	void setAngularTorque(float torque);
	float getAngularTorque() const;
	void setAngularVelocity(float velocity);
	float getAngularVelocity() const;
	float getCurrentAngularVelocity() const;
	float getCurrentLinearDistance() const;
};
typedef Ptr<JointSuspension> JointSuspensionPtr;


class UNIGINE_API JointWheel : public Joint
{
public:
	static bool convertible(Joint *obj) { return obj && obj->getType() == Joint::JOINT_WHEEL; }
	static Ptr<JointWheel> create();
	static Ptr<JointWheel> create(const Ptr<Body> &body0, const Ptr<Body> &body1);
	static Ptr<JointWheel> create(const Ptr<Body> &body0, const Ptr<Body> &body1, const Math::Vec3 & anchor, const Math::vec3 &axis0, const Math::vec3 &axis1);
	void setAxis00(const Math::vec3 &axis00);
	Math::vec3 getAxis00() const;
	void setAxis10(const Math::vec3 &axis10);
	Math::vec3 getAxis10() const;
	void setAxis11(const Math::vec3 &axis11);
	Math::vec3 getAxis11() const;
	void setWorldAxis0(const Math::vec3 &axis0);
	Math::vec3 getWorldAxis0() const;
	void setWorldAxis1(const Math::vec3 &axis1);
	Math::vec3 getWorldAxis1() const;
	void setPhysicsIntersectionMask(int mask);
	int getPhysicsIntersectionMask() const;
	void setLinearDamping(float damping);
	float getLinearDamping() const;
	void setLinearDistance(float distance);
	float getLinearDistance() const;
	void setLinearLimitFrom(float from);
	float getLinearLimitFrom() const;
	void setLinearLimitTo(float to);
	float getLinearLimitTo() const;
	void setLinearSpring(float spring);
	float getLinearSpring() const;
	void setCurrentLinearDistance(float distance);
	float getCurrentLinearDistance() const;
	void setAngularDamping(float damping);
	float getAngularDamping() const;
	void setAngularTorque(float torque);
	float getAngularTorque() const;
	void setAngularVelocity(float velocity);
	float getAngularVelocity() const;
	void setCurrentAngularVelocity(float velocity);
	float getCurrentAngularVelocity() const;
	void setBinormalAngle(float angle);
	float getBinormalAngle() const;
	void setBinormalFriction(float friction);
	float getBinormalFriction() const;
	void setTangentAngle(float angle);
	float getTangentAngle() const;
	void setTangentFriction(float friction);
	float getTangentFriction() const;
	void setWheelMass(float mass);
	float getWheelMass() const;
	void setWheelRadius(float radius);
	float getWheelRadius() const;
	void setWheelThreshold(float threshold);
	float getWheelThreshold() const;
	Math::vec3 getContactNormal() const;
	Ptr<Object> getContactObject() const;
	Math::Vec3 getContactPoint() const;
	Ptr<Shape> getContactShape() const;
	int getContactSurface() const;
	float getCurrentSlipAngle() const;
	float getCurrentSlipRatio() const;
};
typedef Ptr<JointWheel> JointWheelPtr;


class UNIGINE_API Body : public APIInterface
{
public:
	static bool convertible(Body *obj) { return obj != nullptr; }

	enum TYPE
	{
		BODY_DUMMY = 0,
		BODY_RIGID,
		BODY_RAGDOLL,
		BODY_FRACTURE,
		BODY_ROPE,
		BODY_CLOTH,
		BODY_WATER,
		BODY_PATH,
		NUM_BODIES,
	};
	static Ptr<Body> createBody(int type);
	static Ptr<Body> createBody(const char *type_name);
	void setID(int id);
	int getID() const;
	Body::TYPE getType() const;
	static const char *getTypeName(int type);
	const char *getTypeName() const;
	void setObject(const Ptr<Object> &val);
	Ptr<Object> getObject() const;
	void setObject(const Ptr<Object> &object, bool update);
	void setEnabled(bool enable);
	bool isEnabled() const;
	bool isEnabledSelf() const;
	void setFrozen(bool frozen);
	bool isFrozen() const;
	void setImmovable(bool immovable);
	bool isImmovable() const;
	void setGravity(bool gravity);
	bool isGravity() const;
	void setName(const char *name);
	const char *getName() const;
	void setPhysicalMask(int mask);
	int getPhysicalMask() const;
	void setTransform(const Math::Mat4 &transform);
	Math::Mat4 getTransform() const;
	void setPreserveTransform(const Math::Mat4 &transform);
	void setVelocityTransform(const Math::Mat4 &transform);
	void flushTransform() const;
	void setPosition(const Math::Vec3 &pos);
	Math::Vec3 getPosition() const;
	void setRotation(const Math::quat &rot);
	Math::quat getRotation() const;
	void setDirection(const Math::vec3 &dir, const Math::vec3 &up);
	Math::vec3 getDirection() const;
	Ptr<Body> getParent() const;
	int isChild(const Ptr<Body> &body) const;
	int getNumChildren() const;
	int findChild(const char *name) const;
	Ptr<Body> getChild(int num) const;
	void addShape(const Ptr<Shape> &shape);
	void addShape(const Ptr<Shape> &shape, const Math::mat4 &transform);
	bool insertShape(int pos, const Ptr<Shape> &shape);
	bool insertShape(int pos, const Ptr<Shape> &shape, const Math::mat4 &transform);
	void removeShape(const Ptr<Shape> &shape, bool destroy = false);
	void removeShape(int num, bool destroy = false);
	void clearShapes(int destroy = 0);
	int isShape(const Ptr<Shape> &shape) const;
	int getNumShapes() const;
	int findShape(const char *name) const;
	Ptr<Shape> getShape(int num) const;
	void setShapeTransform(int num, const Math::mat4 &transform);
	Math::mat4 getShapeTransform(int num) const;
	void updateShapes();
	Ptr<Shape> getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, Math::Vec3 *ret_point, Math::vec3 *ret_normal);
	void addJoint(const Ptr<Joint> &joint);
	void insertJoint(const Ptr<Joint> &joint, int num);
	void removeJoint(const Ptr<Joint> &joint);
	void removeJoint(int num);
	int isJoint(const Ptr<Joint> &joint) const;
	int getNumJoints() const;
	int findJoint(const char *name) const;
	Ptr<Joint> getJoint(int num) const;
	int getNumContacts() const;
	unsigned long long getContactID(int num) const;
	int findContactByID(unsigned long long id) const;
	bool isContactInternal(int num) const;
	bool isContactEnter(int num) const;
	bool isContactLeave(int num) const;
	bool isContactStay(int num) const;
	Math::Vec3 getContactPoint(int num) const;
	Math::vec3 getContactNormal(int num) const;
	Math::vec3 getContactVelocity(int num) const;
	float getContactImpulse(int num) const;
	float getContactTime(int num) const;
	float getContactDepth(int num) const;
	float getContactFriction(int num) const;
	float getContactRestitution(int num) const;
	Ptr<Body> getContactBody0(int num) const;
	Ptr<Body> getContactBody1(int num) const;
	Ptr<Shape> getContactShape0(int num) const;
	Ptr<Shape> getContactShape1(int num) const;
	Ptr<Object> getContactObject(int num) const;
	int getContactSurface(int num) const;
	void *addFrozenCallback(CallbackBase1<Ptr<Body>> *func);
	bool removeFrozenCallback(void *id);
	void clearFrozenCallbacks();
	void *addPositionCallback(CallbackBase1<Ptr<Body>> *func);
	bool removePositionCallback(void *id);
	void clearPositionCallbacks();
	void *addContactEnterCallback(CallbackBase2<Ptr<Body>, int> *func);
	bool removeContactEnterCallback(void *id);
	void clearContactEnterCallbacks();
	void *addContactLeaveCallback(CallbackBase2<Ptr<Body>, int> *func);
	bool removeContactLeaveCallback(void *id);
	void clearContactLeaveCallbacks();
	void *addContactsCallback(CallbackBase1<Ptr<Body>> *func);
	bool removeContactsCallback(void *id);
	void clearContactsCallbacks();
	void renderShapes();
	void renderJoints();
	void renderExternalContacts();
	void renderInternalContacts();
	void renderContacts();
	void renderVisualizer();
	Ptr<Body> clone(const Ptr<Object> &object) const;
	void swap(const Ptr<Body> &body) const;
	int saveState(const Ptr<Stream> &stream) const;
	int restoreState(const Ptr<Stream> &stream);
};
typedef Ptr<Body> BodyPtr;


class UNIGINE_API BodyDummy : public Body
{
public:
	static bool convertible(Body *obj) { return obj && obj->getType() == Body::BODY_DUMMY; }
	static Ptr<BodyDummy> create();
	static Ptr<BodyDummy> create(const Ptr<Object> &object);
};
typedef Ptr<BodyDummy> BodyDummyPtr;


class UNIGINE_API BodyRigid : public Body
{
public:
	static bool convertible(Body *obj) { return obj && obj->getType() == Body::BODY_RIGID; }
	static Ptr<BodyRigid> create();
	static Ptr<BodyRigid> create(const Ptr<Object> &object);
	void setShapeBased(bool based);
	bool isShapeBased() const;
	void setFreezable(bool freezable);
	bool isFreezable() const;
	void setHighPriorityContacts(bool contacts);
	bool isHighPriorityContacts() const;
	void setMass(float mass);
	float getMass() const;
	float getIMass() const;
	void setInertia(const Math::mat3 &inertia);
	Math::mat3 getInertia() const;
	Math::mat3 getIWorldInertia() const;
	void setCenterOfMass(const Math::vec3 &center);
	Math::vec3 getCenterOfMass() const;
	Math::Vec3 getWorldCenterOfMass() const;
	void setLinearScale(const Math::vec3 &scale);
	Math::vec3 getLinearScale() const;
	void setAngularScale(const Math::vec3 &scale);
	Math::vec3 getAngularScale() const;
	void setLinearDamping(float damping);
	float getLinearDamping() const;
	void setAngularDamping(float damping);
	float getAngularDamping() const;
	void setMaxLinearVelocity(float velocity);
	float getMaxLinearVelocity() const;
	void setMaxAngularVelocity(float velocity);
	float getMaxAngularVelocity() const;
	void setFrozenLinearVelocity(float velocity);
	float getFrozenLinearVelocity() const;
	void setFrozenAngularVelocity(float velocity);
	float getFrozenAngularVelocity() const;
	void setLinearVelocity(const Math::vec3 &velocity);
	Math::vec3 getLinearVelocity() const;
	void setAngularVelocity(const Math::vec3 &velocity);
	Math::vec3 getAngularVelocity() const;
	void addForce(const Math::vec3 &force);
	void addTorque(const Math::vec3 &torque);
	void addForce(const Math::vec3 &radius, const Math::vec3 &force);
	void addTorque(const Math::vec3 &radius, const Math::vec3 &torque);
	void addImpulse(const Math::vec3 &radius, const Math::vec3 &impulse);
	Math::vec3 getVelocity(const Math::vec3 &radius) const;
	void addWorldForce(const Math::Vec3 &point, const Math::vec3 &force);
	void addWorldTorque(const Math::Vec3 &point, const Math::vec3 &torque);
	void addWorldImpulse(const Math::Vec3 &point, const Math::vec3 &impulse);
	Math::vec3 getWorldVelocity(const Math::Vec3 &point) const;
	void addLinearImpulse(const Math::vec3 &impulse);
	void addAngularImpulse(const Math::vec3 &impulse);
	int createShapes(int depth = 4, float error = 0.01, float threshold = 0.01);
};
typedef Ptr<BodyRigid> BodyRigidPtr;


class UNIGINE_API BodyFracture : public Body
{
public:
	static bool convertible(Body *obj) { return obj && obj->getType() == Body::BODY_FRACTURE; }
	static Ptr<BodyFracture> create();
	static Ptr<BodyFracture> create(const Ptr<Object> &object);
	void setAngularDamping(float damping);
	float getAngularDamping() const;
	void setLinearDamping(float damping);
	float getLinearDamping() const;
	Ptr<BodyRigid> getBodyRigid() const;
	void setMass(float mass);
	float getMass() const;
	void setFrozenLinearVelocity(float velocity);
	float getFrozenLinearVelocity() const;
	void setFrozenAngularVelocity(float velocity);
	float getFrozenAngularVelocity() const;
	void setMaxLinearVelocity(float velocity);
	float getMaxLinearVelocity() const;
	void setMaxAngularVelocity(float velocity);
	float getMaxAngularVelocity() const;
	void setPhysicsIntersectionMask(int mask);
	int getPhysicsIntersectionMask() const;
	void setRestitution(float restitution);
	float getRestitution() const;
	void setThreshold(float threshold);
	float getThreshold() const;
	void setFriction(float friction);
	float getFriction() const;
	void setExclusionMask(int mask);
	int getExclusionMask() const;
	void setError(float error);
	float getError() const;
	void setDensity(float density);
	float getDensity() const;
	void setCollisionMask(int mask);
	int getCollisionMask() const;
	void setBroken(bool broken);
	bool isBroken() const;
	void setSurfaceProperty(const char *property);
	const char *getSurfaceProperty() const;
	void setMaterialPath(const char *path);
	const char *getMaterialPath() const;
	void setMaterialGUID(const UGUID &materialguid);
	UGUID getMaterialGUID() const;
	void setMaterial(const Ptr<Material> &material);
	Ptr<Material> getMaterial() const;
	void addForce(const Math::vec3 &force) const;
	void addTorque(const Math::vec3 &torque) const;
	void addForce(const Math::vec3 &radius, const Math::vec3 &force) const;
	void addTorque(const Math::vec3 &radius, const Math::vec3 &torque) const;
	void addImpulse(const Math::vec3 &radius, const Math::vec3 &impulse);
	Math::vec3 getVelocity(const Math::vec3 &radius) const;
	void addWorldForce(const Math::Vec3 &point, const Math::vec3 &force);
	void addWorldTorque(const Math::Vec3 &point, const Math::vec3 &torque);
	void addWorldImpulse(const Math::Vec3 &point, const Math::vec3 &impulse);
	Math::vec3 getWorldVelocity(const Math::Vec3 &point) const;
	int createCrackPieces(const Math::Vec3 &point, const Math::vec3 &normal, int num_cuts, int num_rings, float step);
	int createShatterPieces(int num_pieces);
	int createSlicePieces(const Math::Vec3 &point, const Math::vec3 &normal);
};
typedef Ptr<BodyFracture> BodyFracturePtr;


class UNIGINE_API BodyPath : public Body
{
public:
	static bool convertible(Body *obj) { return obj && obj->getType() == Body::BODY_PATH; }
	static Ptr<BodyPath> create();
	static Ptr<BodyPath> create(const Ptr<Object> &object);
	Math::Vec3 getClosestPosition(const Math::Vec3 &position) const;
	Math::quat getClosestRotation(const Math::Vec3 &position) const;
	const char *getPathName() const;
	void setPathName(const char *name, int unique = 0);
	Ptr<Path> getPath() const;
};
typedef Ptr<BodyPath> BodyPathPtr;


class UNIGINE_API BodyRagdoll : public Body
{
public:
	static bool convertible(Body *obj) { return obj && obj->getType() == Body::BODY_RAGDOLL; }
	static Ptr<BodyRagdoll> create();
	static Ptr<BodyRagdoll> create(const Ptr<Object> &object);
	void setMass(float mass);
	float getMass() const;
	void setRigidity(float rigidity);
	float getRigidity() const;
	void setFrameBased(bool based);
	bool isFrameBased() const;
	int createBones(float error = 0.2, float threshold = 0.01, int capsule = 0);
	void setBoneFrameBased(int bone, int based);
	void setBones(const Ptr<Node> &node);
	int findBone(const char *name) const;
	const char *getBoneName(int bone) const;
	int getBoneNumber(int bone) const;
	Math::Mat4 getBoneTransform(int bone) const;
	Ptr<Node> getBones() const;
	int getNumBones() const;
	bool isBoneFrameBased(int bone) const;
	bool updateBones();
};
typedef Ptr<BodyRagdoll> BodyRagdollPtr;


class UNIGINE_API BodyWater : public Body
{
public:
	static bool convertible(Body *obj) { return obj && obj->getType() == Body::BODY_WATER; }
	using Body::getIntersection;
	static Ptr<BodyWater> create();
	static Ptr<BodyWater> create(const Ptr<Object> &object);
	void addParticleHeight(const Math::vec3 &position, float height);
	void addParticleVelocity(const Math::vec3 &position, const Math::vec3 &velocity);
	float getParticleHeight(const Math::vec3 &position) const;
	Math::vec3 getParticleVelocity(const Math::vec3 &position) const;
	void setAbsorption(int absorption);
	int getAbsorption() const;
	void setDensity(float density);
	float getDensity() const;
	void setDepth(float depth);
	float getDepth() const;
	void setDistance(float distance);
	float getDistance() const;
	void setAngularDamping(float damping);
	float getAngularDamping() const;
	void setLinearDamping(float damping);
	float getLinearDamping() const;
	void setInteractionForce(float force);
	float getInteractionForce() const;
	void setIntersection(int intersection);
	int getIntersection() const;
	void setLiquidity(float liquidity);
	float getLiquidity() const;
};
typedef Ptr<BodyWater> BodyWaterPtr;


class UNIGINE_API BodyParticles : public Body
{
public:
	static bool convertible(Body *obj) {
		if(!obj)
			return false;
		int type = obj->getType();
		switch(type)
		{
			case Body::BODY_CLOTH:
			case Body::BODY_ROPE:
			return true;
		}
		return false;
	}

	enum ITERATIONS_MODE
	{
		ITERATIONS_MODE_OVERRIDE = 0,
		ITERATIONS_MODE_MULTIPLICATION,
	};
	void addParticleForce(int num, const Math::vec3 &force);
	void addParticleImpulse(int num, const Math::vec3 &impulse);
	void setAngularRestitution(float restitution);
	float getAngularRestitution() const;
	void setAngularThreshold(float threshold);
	float getAngularThreshold() const;
	void setCollisionMask(int mask);
	int getCollisionMask() const;
	void setCollision(int collision);
	int getCollision() const;
	void setDistance(float distance);
	float getDistance() const;
	void setFriction(float friction);
	float getFriction() const;
	void setLinearDamping(float damping);
	float getLinearDamping() const;
	void setLinearRestitution(float restitution);
	float getLinearRestitution() const;
	void setLinearStretch(float stretch);
	float getLinearStretch() const;
	void setLinearThreshold(float threshold);
	float getLinearThreshold() const;
	void setMass(float mass);
	float getMass() const;
	void setNumIterations(int iterations);
	int getNumIterations() const;
	void setIterationsMode(BodyParticles::ITERATIONS_MODE mode);
	BodyParticles::ITERATIONS_MODE getIterationsMode() const;
	void setRadius(float radius);
	float getRadius() const;
	void setRestitution(float restitution);
	float getRestitution() const;
	void setRigidity(float rigidity);
	float getRigidity() const;
	int getNumParticles() const;
	void setParticleMass(int num, float mass);
	float getParticleMass(int num) const;
	void setParticlePosition(int num, const Math::Vec3 &position);
	Math::Vec3 getParticlePosition(int num) const;
	void setParticleVelocity(int num, const Math::vec3 &velocity);
	Math::vec3 getParticleVelocity(int num) const;
};
typedef Ptr<BodyParticles> BodyParticlesPtr;


class UNIGINE_API BodyCloth : public BodyParticles
{
public:
	static bool convertible(Body *obj) { return obj && obj->getType() == Body::BODY_CLOTH; }
	static Ptr<BodyCloth> create();
	static Ptr<BodyCloth> create(const Ptr<Object> &object);
	void setTwoSided(int sided);
	int getTwoSided() const;
	int getNumIndices() const;
	int getParticleIndex(int num) const;
};
typedef Ptr<BodyCloth> BodyClothPtr;


class UNIGINE_API BodyRope : public BodyParticles
{
public:
	static bool convertible(Body *obj) { return obj && obj->getType() == Body::BODY_ROPE; }
	static Ptr<BodyRope> create();
	static Ptr<BodyRope> create(const Ptr<Object> &object);
};
typedef Ptr<BodyRope> BodyRopePtr;

} // namespace Unigine
