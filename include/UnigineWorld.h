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

#include "UnigineVector.h"
#include "UnigineNode.h"
#include "UnigineObjects.h"
#include "UniginePtr.h"

namespace Unigine
{


class UNIGINE_API WorldIntersection : public APIInterface
{
public:
	static bool convertible(WorldIntersection *obj) { return obj != nullptr; }

	enum TYPE
	{
		WORLD_INTERSECTION = 0,
		WORLD_INTERSECTION_NORMAL,
		WORLD_INTERSECTION_TEX_COORD,
		NUM_WORLD_INTERSECTIONS,
	};
	static Ptr<WorldIntersection> create();
	WorldIntersection::TYPE getType() const;
	const char *getTypeName() const;
	void setPoint(const Math::Vec3 &point);
	Math::Vec3 getPoint() const;
	void setIndex(int index);
	int getIndex() const;
	void setInstance(int instance);
	int getInstance() const;
	void setSurface(int surface);
	int getSurface() const;
};
typedef Ptr<WorldIntersection> WorldIntersectionPtr;


class UNIGINE_API WorldIntersectionNormal : public WorldIntersection
{
public:
	static bool convertible(WorldIntersection *obj) {
		if(!obj)
			return false;
		int type = obj->getType();
		switch(type)
		{
			case WorldIntersection::WORLD_INTERSECTION_NORMAL:
			case WorldIntersection::WORLD_INTERSECTION_TEX_COORD:
			return true;
		}
		return false;
	}
	static Ptr<WorldIntersectionNormal> create();
	void setNormal(const Math::vec3 &normal);
	Math::vec3 getNormal() const;
};
typedef Ptr<WorldIntersectionNormal> WorldIntersectionNormalPtr;


class UNIGINE_API WorldIntersectionTexCoord : public WorldIntersectionNormal
{
public:
	static bool convertible(WorldIntersection *obj) { return obj && obj->getType() == WorldIntersection::WORLD_INTERSECTION_TEX_COORD; }
	static Ptr<WorldIntersectionTexCoord> create();
	void setTexCoord(const Math::vec4 &coord);
	Math::vec4 getTexCoord() const;
};
typedef Ptr<WorldIntersectionTexCoord> WorldIntersectionTexCoordPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API World
{
protected:
	

public:
	static int isInitialized(); 

	enum CALLBACK_INDEX
	{
		CALLBACK_PRE_WORLD_LOAD,
		CALLBACK_POST_WORLD_LOAD,
		CALLBACK_PRE_WORLD_SAVE,
		CALLBACK_POST_WORLD_SAVE,
		CALLBACK_PRE_WORLD_CLEAR,
		CALLBACK_POST_WORLD_CLEAR,
		CALLBACK_PRE_NODE_SAVE,
		CALLBACK_POST_NODE_SAVE,
		CALLBACK_PRE_WORLD_INIT,
		CALLBACK_POST_WORLD_INIT,
		CALLBACK_PRE_WORLD_SHUTDOWN,
		CALLBACK_POST_WORLD_SHUTDOWN,
	};
	static void setPath(const char *path);
	static const char *getPath();
	static void setScriptName(const char *name);
	static const char *getScriptName();
	static void setRenderSettings(const char *settings);
	static const char *getRenderSettings();
	static void setSoundSettings(const char *settings);
	static const char *getSoundSettings();
	static void setPhysicsSettings(const char *settings);
	static const char *getPhysicsSettings();
	static bool isLoaded();
	static void setBudget(float budget);
	static float getBudget();
	static void setDistance(float distance);
	static float getDistance();
	static void setUpdateGridSize(float size);
	static float getUpdateGridSize();
	static void setScriptExecute(bool execute);
	static bool isScriptExecute();
	static void setAutoReloadNodeReferences(bool references);
	static bool isAutoReloadNodeReferences();
	static void setUnpackNodeReferences(bool references);
	static bool isUnpackNodeReferences();
	static void setData(const char *name, const char *data);
	static const char *getData(const char *name);
	static bool loadWorld(const char *path);
	static bool loadWorld(const char *path, bool partial_path);
	static bool loadWorldForce(const char *path);
	static bool loadWorldForce(const char *path, bool partial_path);
	static bool addWorld(const char *name);
	static bool saveWorld();
	static bool saveWorld(const char *path);
	static bool reloadWorld();
	static bool quitWorld();
	static bool isLoadWorldRequested();
	static const char *getLoadWorldRequestPath();
	static void updateSpatial();
	static bool isNode(int id);
	static void getNodes(Vector<Ptr<Node>> &nodes);
	static bool clearNode(const char *name);
	static Ptr<Node> loadNode(const char *name, int cache = 1);
	static int loadNodes(const char *name, Vector<Ptr<Node>> &nodes);
	static bool saveNode(const char *name, const Ptr<Node> &node, int binary = 0);
	static int saveNodes(const char *name, const Vector<Ptr<Node>> &nodes, int binary = 0);
	static Ptr<Node> getNodeByName(const char *name);
	static void getNodesByName(const char *name, Vector<Ptr<Node>> &nodes);
	static Ptr<Node> getNodeByType(int type);
	static void getNodesByType(int type, Vector<Ptr<Node>> &nodes);
	static Ptr<Node> getNodeByID(int node_id);
	static void getRootNodes(Vector<Ptr<Node>> &nodes);
	static int getRootNodeIndex(const Ptr<Node> &node);
	static void setRootNodeIndex(const Ptr<Node> &node, int index);
	static bool isNode(const char *name);
	static bool getCollision(const Math::WorldBoundBox &bb, Vector<Ptr<Object>> &objects);
	static bool getCollision(const Math::WorldBoundSphere &bs, Vector<Ptr<Object>> &objects);
	static bool getCollision(const Math::Vec3 &p0, const Math::Vec3 &p1, Vector<Ptr<Object>> &objects);
	static bool getIntersection(const Math::WorldBoundBox &bb, Vector<Ptr<Node>> &nodes);
	static bool getIntersection(const Math::WorldBoundBox &bb, Vector<Ptr<Object>> &objects);
	static bool getIntersection(const Math::WorldBoundBox &bb, Node::TYPE type, Vector<Ptr<Node>> &nodes);
	static bool getIntersection(const Math::WorldBoundSphere &bs, Vector<Ptr<Node>> &nodes);
	static bool getIntersection(const Math::WorldBoundSphere &bs, Vector<Ptr<Object>> &objects);
	static bool getIntersection(const Math::WorldBoundSphere &bs, Node::TYPE type, Vector<Ptr<Node>> &nodes);
	static bool getIntersection(const Math::WorldBoundFrustum &bf, Vector<Ptr<Object>> &objects);
	static bool getIntersection(const Math::WorldBoundFrustum &bf, Node::TYPE type, Vector<Ptr<Node>> &nodes);
	static bool getVisibleIntersection(const Math::Vec3 &camera, const Math::WorldBoundFrustum &bf, Vector<Ptr<Object>> &objects, float max_distance);
	static bool getVisibleIntersection(const Math::Vec3 &camera, const Math::WorldBoundFrustum &bf, Node::TYPE type, Vector<Ptr<Node>> &nodes, float max_distance);
	static bool getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, Vector<Ptr<Object>> &objects);
	static Ptr<Object> getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask);
	static Ptr<Object> getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, const Ptr<WorldIntersection> &intersection);
	static Ptr<Object> getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, const Ptr<WorldIntersectionNormal> &intersection);
	static Ptr<Object> getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, const Ptr<WorldIntersectionTexCoord> &intersection);
	static Ptr<Object> getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, const Vector<Ptr<Node>> &exclude);
	static Ptr<Object> getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, const Vector<Ptr<Node>> &exclude, const Ptr<WorldIntersection> &intersection);
	static Ptr<Object> getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, const Vector<Ptr<Node>> &exclude, const Ptr<WorldIntersectionNormal> &intersection);
	static Ptr<Object> getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, const Vector<Ptr<Node>> &exclude, const Ptr<WorldIntersectionTexCoord> &intersection);
	static void clearBindings();
	static void *addCallback(World::CALLBACK_INDEX callback, CallbackBase *func);
	static void *addCallback(World::CALLBACK_INDEX callback, CallbackBase1<const char *> *func);
	static void *addCallback(World::CALLBACK_INDEX callback, CallbackBase2<const char *, Ptr<Node>> *func);
	static bool removeCallback(World::CALLBACK_INDEX callback, void *id);
	static void clearCallbacks(World::CALLBACK_INDEX callback);
};

class WorldExternBase;
//////////////////////////////////////////////////////////////////////////

class UNIGINE_API WorldExtern : public Node
{
public:
	static int type() { return Node::WORLD_EXTERN; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<WorldExtern> create(int class_id);
	int getClassID() const;
	WorldExternBase *getWorldExtern() const;
};
typedef Ptr<WorldExtern> WorldExternPtr;


class UNIGINE_API  WorldExternBase : public virtual Base
{
public:
	WorldExternBase();
	WorldExternBase(void * world);
	virtual ~WorldExternBase();
	virtual int getClassID() = 0;
	Ptr<Node> getNode() const;
	Ptr<WorldExtern> getWorldExtern() const;
	typedef WorldExternBase *(*ConstructorFunc)(void *world);
	static void addClassID(int class_id, ConstructorFunc func);
	template <class Type> static void addClassID(int class_id) { addClassID(class_id, constructor<Type>); }
	virtual void updateEnabled();
	virtual void updatePosition();
	virtual void updateTransform();
	virtual void update(float ifps);
	virtual void preRender(float ifps);
	virtual const Math::BoundBox &getBoundBox();
	virtual const Math::BoundSphere &getBoundSphere();
	virtual void renderHandler();
	virtual void renderVisualizer();
	virtual void copy(WorldExternBase *world, int is_root);
	virtual void swap(WorldExternBase *world);
	virtual void setUpdateDistanceLimit(float distance);
	virtual float getUpdateDistanceLimit();
	virtual void setUpdate(bool enabled);
	virtual bool isUpdate();
	virtual int loadWorld(const Ptr<Xml> &xml);
	virtual int saveWorld(const Ptr<Xml> &xml);
	virtual int saveState(const Ptr<Stream> &stream);
	virtual int restoreState(const Ptr<Stream> &stream);
	static void savePointer(const StreamPtr &stream, WorldExternBase *world);
	static WorldExternBase *restorePointer(const StreamPtr &stream);
private:
	template <class Type> static WorldExternBase *constructor(void *world) { return new Type(world); }
	struct WorldExternData;
	WorldExternData *data;
};
typedef Ptr<WorldExternBase> WorldExternBasePtr;

} // namespace Unigine
