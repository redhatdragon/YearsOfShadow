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
#include "UnigineObjects.h"
#include "UniginePhysics.h"
#include "UnigineControls.h"
#include "UnigineCamera.h"
#include "UnigineMaterial.h"

namespace Unigine
{

class EngineWindow;
//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Player : public Node
{
public:
	static bool convertible(Node *node) { return node && node->isPlayer(); }

	void setProjection(const Math::mat4 &projection);
	Math::mat4 getProjection() const;
	Math::mat4 getAspectCorrectedProjection(int width = -1, int height = -1) const;
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
	void setViewDirection(const Math::vec3 &direction);
	Math::vec3 getViewDirection() const;
	void setVelocity(const Math::vec3 &velocity);
	Math::vec3 getVelocity() const;
	void setControlled(bool controlled);
	bool isControlled() const;
	void setControls(const Ptr<Controls> &controls);
	Ptr<Controls> getControls() const;
	void setMainPlayer(bool player);
	bool isMainPlayer() const;
	void setListener(bool listener);
	bool isListener() const;
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
	void setCamera(const Ptr<Camera> &camera);
	Ptr<Camera> getCamera() const;
	void updateControls(float ifps) const;
	void flushTransform() const;
	void getDirectionFromMainWindow(Math::Vec3 &p0, Math::Vec3 &p1, int mouse_x, int mouse_y) const;
	Math::vec3 getDirectionFromMainWindow(int mouse_x, int mouse_y) const;
	void getDirectionFromWindow(Math::Vec3 &p0, Math::Vec3 &p1, int mouse_x, int mouse_y, const Ptr<EngineWindow> &window) const;
	Math::vec3 getDirectionFromWindow(int mouse_x, int mouse_y, const Ptr<EngineWindow> &window) const;
	void getDirectionFromScreen(Math::Vec3 &p0, Math::Vec3 &p1, int mouse_x, int mouse_y, int screen_x, int screen_y, int screen_width, int screen_height) const;
	Math::vec3 getDirectionFromScreen(int mouse_x, int mouse_y, int screen_x, int screen_y, int screen_width, int screen_height) const;
	Math::mat4 getProjectionFromMainWindow(int x0, int y0, int x1, int y1) const;
	Math::mat4 getProjectionFromWindow(int x0, int y0, int x1, int y1, const Ptr<EngineWindow> &window) const;
	Math::mat4 getProjectionFromScreen(int x0, int y0, int x1, int y1, int screen_width, int screen_height) const;
	int getMainWindowPosition(int &x, int &y, const Math::Vec3 &point) const;
	int getWindowPosition(int &x, int &y, const Math::Vec3 &point, const Ptr<EngineWindow> &window) const;
	int getScreenPosition(int &x, int &y, const Math::Vec3 &point, int screen_width, int screen_height) const;
};
typedef Ptr<Player> PlayerPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API PlayerDummy : public Player
{
public:
	static int type() { return Node::PLAYER_DUMMY; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<PlayerDummy> create();
	void setViewDirection(const Math::vec3 &direction);
	Math::vec3 getViewDirection() const;
};
typedef Ptr<PlayerDummy> PlayerDummyPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API PlayerActor : public Player
{
public:
	static int type() { return Node::PLAYER_ACTOR; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<PlayerActor> create();
	void setPhysical(bool physical);
	bool isPhysical() const;
	void setPhysicalMass(float mass);
	float getPhysicalMass() const;
	void setPhysicalMask(int mask);
	int getPhysicalMask() const;
	void setPhysicsIntersectionMask(int mask);
	int getPhysicsIntersectionMask() const;
	void setCollision(int collision);
	int getCollision() const;
	void setCollisionMask(int mask);
	int getCollisionMask() const;
	void setCollisionRadius(float radius);
	float getCollisionRadius() const;
	void setCollisionHeight(float height);
	float getCollisionHeight() const;
	void setMinFriction(float friction);
	float getMinFriction() const;
	void setMaxFriction(float friction);
	float getMaxFriction() const;
	void setMinVelocity(float velocity);
	float getMinVelocity() const;
	void setMaxVelocity(float velocity);
	float getMaxVelocity() const;
	void setMinThetaAngle(float angle);
	float getMinThetaAngle() const;
	void setMaxThetaAngle(float angle);
	float getMaxThetaAngle() const;
	void setAcceleration(float acceleration);
	float getAcceleration() const;
	void setDamping(float damping);
	float getDamping() const;
	void setTurning(float turning);
	float getTurning() const;
	void setJumping(float jumping);
	float getJumping() const;
	void setMaxStepHeight(float height);
	float getMaxStepHeight() const;
	void setPhiAngle(float angle);
	float getPhiAngle() const;
	void setThetaAngle(float angle);
	float getThetaAngle() const;
	void setViewDirection(const Math::vec3 &direction);
	Math::vec3 getViewDirection() const;

	enum
	{
		STATE_FORWARD = 0,
		STATE_BACKWARD,
		STATE_MOVE_LEFT,
		STATE_MOVE_RIGHT,
		STATE_TURN_UP,
		STATE_TURN_DOWN,
		STATE_TURN_LEFT,
		STATE_TURN_RIGHT,
		STATE_CROUCH,
		STATE_JUMP,
		STATE_RUN,
		STATE_USE,
		STATE_FIRE,
		NUM_STATES,
	};

	enum
	{
		STATE_DISABLED = 0,
		STATE_ENABLED,
		STATE_BEGIN,
		STATE_END,
	};
	int getState(int arg1) const;
	float getStateTime(int arg1) const;
	void setGround(int ground);
	int getGround() const;
	void setCeiling(int ceiling);
	int getCeiling() const;
	Ptr<Body> getBody() const;
	Ptr<ShapeCapsule> getShape() const;
	int getNumContacts() const;
	float getContactDepth(int contact) const;
	Math::vec3 getContactNormal(int contact) const;
	Ptr<Object> getContactObject(int contact) const;
	Math::Vec3 getContactPoint(int contact) const;
	Ptr<Shape> getContactShape(int num) const;
	int getContactSurface(int contact) const;
};
typedef Ptr<PlayerActor> PlayerActorPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API PlayerPersecutor : public Player
{
public:
	static int type() { return Node::PLAYER_PERSECUTOR; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<PlayerPersecutor> create();
	void setFixed(bool fixed);
	bool isFixed() const;
	void setCollision(int collision);
	int getCollision() const;
	void setCollisionMask(int mask);
	int getCollisionMask() const;
	void setCollisionRadius(float radius);
	float getCollisionRadius() const;
	void setTarget(const Ptr<Node> &target);
	Ptr<Node> getTarget() const;
	void setAnchor(const Math::vec3 &anchor);
	Math::vec3 getAnchor() const;
	void setMinDistance(float distance);
	float getMinDistance() const;
	void setMaxDistance(float distance);
	float getMaxDistance() const;
	void setMinThetaAngle(float angle);
	float getMinThetaAngle() const;
	void setMaxThetaAngle(float angle);
	float getMaxThetaAngle() const;
	void setTurning(float turning);
	float getTurning() const;
	void setDistance(float distance);
	float getDistance() const;
	void setPhiAngle(float angle);
	float getPhiAngle() const;
	void setThetaAngle(float angle);
	float getThetaAngle() const;
	int getNumContacts() const;
	float getContactDepth(int contact) const;
	Math::vec3 getContactNormal(int contact) const;
	Ptr<Object> getContactObject(int contact) const;
	Math::Vec3 getContactPoint(int contact) const;
	Ptr<Shape> getContactShape(int num) const;
	int getContactSurface(int contact) const;
};
typedef Ptr<PlayerPersecutor> PlayerPersecutorPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API PlayerSpectator : public Player
{
public:
	static int type() { return Node::PLAYER_SPECTATOR; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<PlayerSpectator> create();
	void setViewDirection(const Math::vec3 &direction);
	Math::vec3 getViewDirection() const;
	void setCollision(int collision);
	int getCollision() const;
	void setCollisionMask(int mask);
	int getCollisionMask() const;
	void setCollisionRadius(float radius);
	float getCollisionRadius() const;
	void setMinVelocity(float velocity);
	float getMinVelocity() const;
	void setMaxVelocity(float velocity);
	float getMaxVelocity() const;
	void setMinThetaAngle(float angle);
	float getMinThetaAngle() const;
	void setMaxThetaAngle(float angle);
	float getMaxThetaAngle() const;
	void setAcceleration(float acceleration);
	float getAcceleration() const;
	void setDamping(float damping);
	float getDamping() const;
	void setTurning(float turning);
	float getTurning() const;
	void setPhiAngle(float angle);
	float getPhiAngle() const;
	void setThetaAngle(float angle);
	float getThetaAngle() const;
	int getNumContacts() const;
	float getContactDepth(int contact) const;
	Math::vec3 getContactNormal(int contact) const;
	Ptr<Object> getContactObject(int contact) const;
	Math::Vec3 getContactPoint(int contact) const;
	Ptr<Shape> getContactShape(int num) const;
	int getContactSurface(int contact) const;
};
typedef Ptr<PlayerSpectator> PlayerSpectatorPtr;

} // namespace Unigine
