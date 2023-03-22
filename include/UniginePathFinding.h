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
#include "UnigineMesh.h"

namespace Unigine
{

class PathRouteIntersection;
//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Navigation : public Node
{
public:
	static bool convertible(Node *node) { return node && node->isNavigation(); }

	void setDangerous(float dangerous);
	float getDangerous() const;
	void setNavigationMask(int mask);
	int getNavigationMask() const;
	int getNumNavigations() const;
	Ptr<Navigation> getNavigation(int num) const;
	void setQuality(int quality);
	int getQuality() const;
	void setVelocity(float velocity);
	float getVelocity() const;
	int inside2D(const Math::Vec3 &point, float radius) const;
	int inside3D(const Math::Vec3 &point, float radius) const;
	int inside(const Ptr<Navigation> &navigation) const;
};
typedef Ptr<Navigation> NavigationPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API NavigationMesh : public Navigation
{
public:
	static int type() { return Node::NAVIGATION_MESH; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<NavigationMesh> create();
	static Ptr<NavigationMesh> create(const char *arg1);
	void setDepth(int depth);
	int getDepth() const;
	void setHeight(float height);
	float getHeight() const;
	bool setMeshName(const char *path, bool force_load = false);
	const char *getMeshName() const;
	int setMesh(const Ptr<Mesh> &mesh);
	int getMesh(Ptr<Mesh> &mesh) const;
	int loadMesh(const char *path);
	int saveMesh(const char *path) const;
};
typedef Ptr<NavigationMesh> NavigationMeshPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API NavigationSector : public Navigation
{
public:
	static int type() { return Node::NAVIGATION_SECTOR; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<NavigationSector> create(const Math::vec3 &arg1);
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
};
typedef Ptr<NavigationSector> NavigationSectorPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Obstacle : public Node
{
public:
	static bool convertible(Node *node) { return node && node->isObstacle(); }

	void setObstacleMask(int mask);
	int getObstacleMask() const;
};
typedef Ptr<Obstacle> ObstaclePtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObstacleBox : public Obstacle
{
public:
	static int type() { return Node::OBSTACLE_BOX; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObstacleBox> create(const Math::vec3 &arg1);
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
};
typedef Ptr<ObstacleBox> ObstacleBoxPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObstacleCapsule : public Obstacle
{
public:
	static int type() { return Node::OBSTACLE_CAPSULE; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObstacleCapsule> create(float radius, float height);
	void setHeight(float height);
	float getHeight() const;
	void setRadius(float radius);
	float getRadius() const;
};
typedef Ptr<ObstacleCapsule> ObstacleCapsulePtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObstacleSphere : public Obstacle
{
public:
	static int type() { return Node::OBSTACLE_SPHERE; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObstacleSphere> create(float radius);
	void setRadius(float radius);
	float getRadius() const;
};
typedef Ptr<ObstacleSphere> ObstacleSpherePtr;


class UNIGINE_API PathRoute : public APIInterface
{
public:
	static Ptr<PathRoute> create(float radius = 0.0f);
	void addExcludeNavigation(const Ptr<Node> &navigation);
	Vector<Ptr<Node>> getExcludeNavigations() const;
	void setExcludeNavigations(const Vector<Ptr<Node>> &exclude_navigations);
	Vector<Ptr<Node>> getExcludeObstacles() const;
	void setExcludeObstacles(const Vector<Ptr<Node>> &obstacles);
	void addExcludeObstacle(const Ptr<Node> &obstacle);
	void create2D(const Math::Vec3 &p0, const Math::Vec3 &p1, int delay = 0);
	void create3D(const Math::Vec3 &p0, const Math::Vec3 &p1, int delay = 0);
	float getDangerous() const;
	float getDistance() const;
	void setHeight(float height);
	float getHeight() const;
	Ptr<Obstacle> getIntersection(const Ptr<PathRouteIntersection> &intersection) const;
	void setMaxAngle(float angle);
	float getMaxAngle() const;
	void setMaxDangerous(float dangerous);
	float getMaxDangerous() const;
	void setMaxHeight(float height);
	float getMaxHeight() const;
	void setMaxTime(float time);
	float getMaxTime() const;
	void setNavigationMask(int mask);
	int getNavigationMask() const;
	Ptr<Navigation> getNavigation(int num);
	int getNumPoints() const;
	void setObstacleMask(int mask);
	int getObstacleMask() const;
	Ptr<Obstacle> getObstacle(int num);
	Math::Vec3 getPoint(int num);
	void setRadius(float radius);
	float getRadius() const;
	float getTime() const;
	void setVelocity(float velocity);
	float getVelocity() const;
	bool isQueued() const;
	bool isReached() const;
	bool isReady() const;
	void removeExcludeNavigation(const Ptr<Node> &navigation);
	void removeExcludeObstacle(const Ptr<Node> &obstacle);
	void renderVisualizer(const Math::vec4 &color);
};
typedef Ptr<PathRoute> PathRoutePtr;


class UNIGINE_API PathRouteIntersection : public APIInterface
{
public:
	static Ptr<PathRouteIntersection> create();
	void setPoint(const Math::Vec3 &point);
	Math::Vec3 getPoint() const;
	void setIndex(int index);
	int getIndex() const;
};
typedef Ptr<PathRouteIntersection> PathRouteIntersectionPtr;

} // namespace Unigine
