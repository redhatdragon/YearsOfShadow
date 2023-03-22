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
#include "UnigineVector.h"
#include "UnigineProperties.h"
#include "UnigineCallback.h"
#include "UnigineImage.h"
#include "UnigineMathLib.h"

namespace Unigine
{

class WorldTrigger;
class Body;
class BodyRigid;
class Variable;
class GeodeticPivot;

class UNIGINE_API Node : public APIInterface
{
public:

	enum TYPE
	{
		ANY_TYPE = -1,
		NODE_BEGIN = 0,	// nodes
		NODE_DUMMY = NODE_BEGIN,
		NODE_LAYER,
		NODE_TRIGGER,
		NODE_REFERENCE,
		NODE_EXTERN,
		NODE_END = NODE_EXTERN,
		WORLD_BEGIN,	// worlds
		WORLD_SPLINE_GRAPH = WORLD_BEGIN,
		WORLD_TRIGGER,
		WORLD_CLUTTER,
		WORLD_SWITCHER,
		WORLD_OCCLUDER,
		WORLD_OCCLUDER_MESH,
		WORLD_TRANSFORM_PATH,
		WORLD_TRANSFORM_BONE,
		WORLD_EXPRESSION,
		WORLD_EXTERN,
		WORLD_END = WORLD_EXTERN,
		GEODETIC_BEGIN,	// geodetics
		GEODETIC_PIVOT = GEODETIC_BEGIN,
		GEODETIC_END = GEODETIC_PIVOT,
		FIELD_BEGIN,	// fields
		FIELD_SPACER = FIELD_BEGIN,
		FIELD_ANIMATION,
		FIELD_HEIGHT,
		FIELD_SHORELINE,
		FIELD_WEATHER,
		FIELD_END = FIELD_WEATHER,
		LIGHT_BEGIN,	// lights
		LIGHT_VOXEL_PROBE = LIGHT_BEGIN,
		LIGHT_ENVIRONMENT_PROBE,
		LIGHT_PLANAR_PROBE,
		LIGHT_OMNI,
		LIGHT_PROJ,
		LIGHT_WORLD,
		LIGHT_END = LIGHT_WORLD,
		DECAL_BEGIN,	// decals
		DECAL_PROJ = DECAL_BEGIN,
		DECAL_ORTHO,
		DECAL_MESH,
		DECAL_END = DECAL_MESH,
		LANDSCAPE_LAYER_BEGIN,	// landscape map
		LANDSCAPE_LAYER_MAP = LANDSCAPE_LAYER_BEGIN,
		LANDSCAPE_LAYER_END = LANDSCAPE_LAYER_MAP,
		OBJECT_BEGIN,	// objects
		OBJECT_DUMMY = OBJECT_BEGIN,
		OBJECT_DYNAMIC,
		OBJECT_MESH_STATIC,
		OBJECT_MESH_CLUSTER,
		OBJECT_MESH_CLUTTER,
		OBJECT_MESH_SKINNED,
		OBJECT_MESH_DYNAMIC,
		OBJECT_MESH_SPLINE_CLUSTER,
		OBJECT_LANDSCAPE_TERRAIN,
		OBJECT_TERRAIN_GLOBAL,
		OBJECT_GRASS,
		OBJECT_PARTICLES,
		OBJECT_BILLBOARDS,
		OBJECT_VOLUME_BOX,
		OBJECT_VOLUME_SPHERE,
		OBJECT_VOLUME_OMNI,
		OBJECT_VOLUME_PROJ,
		OBJECT_GUI,
		OBJECT_GUI_MESH,
		OBJECT_WATER_GLOBAL,
		OBJECT_WATER_MESH,
		OBJECT_SKY,
		OBJECT_CLOUD_LAYER,
		OBJECT_EXTERN,
		OBJECT_TEXT,
		OBJECT_END = OBJECT_TEXT,
		PLAYER_BEGIN,	// players
		PLAYER_DUMMY = PLAYER_BEGIN,
		PLAYER_SPECTATOR,
		PLAYER_PERSECUTOR,
		PLAYER_ACTOR,
		PLAYER_END = PLAYER_ACTOR,
		PHYSICAL_BEGIN,	// physicals
		PHYSICAL_WIND = PHYSICAL_BEGIN,
		PHYSICAL_FORCE,
		PHYSICAL_NOISE,
		PHYSICAL_WATER,
		PHYSICAL_TRIGGER,
		PHYSICAL_END = PHYSICAL_TRIGGER,
		NAVIGATION_BEGIN,	// navigations
		NAVIGATION_SECTOR = NAVIGATION_BEGIN,
		NAVIGATION_MESH,
		NAVIGATION_END = NAVIGATION_MESH,
		OBSTACLE_BEGIN,	// obstacles
		OBSTACLE_BOX = OBSTACLE_BEGIN,
		OBSTACLE_SPHERE,
		OBSTACLE_CAPSULE,
		OBSTACLE_END = OBSTACLE_CAPSULE,
		SOUND_BEGIN,	// sounds
		SOUND_SOURCE = SOUND_BEGIN,
		SOUND_REVERB,
		SOUND_END = SOUND_REVERB,
		NUM_NODES,
		NUM_WORLDS = WORLD_END - WORLD_BEGIN + 1,
		NUM_GEODETICS = GEODETIC_END - GEODETIC_BEGIN + 1,
		NUM_FIELDS = FIELD_END - FIELD_BEGIN + 1,
		NUM_LIGHTS = LIGHT_END - LIGHT_BEGIN + 1,
		NUM_DECALS = DECAL_END - DECAL_BEGIN + 1,
		NUM_OBJECTS = OBJECT_END - OBJECT_BEGIN + 1,
		NUM_PLAYERS = PLAYER_END - PLAYER_BEGIN + 1,
		NUM_PHYSICALS = PHYSICAL_END - PHYSICAL_BEGIN + 1,
		NUM_NAVIGATIONS = NAVIGATION_END - NAVIGATION_BEGIN + 1,
		NUM_OBSTACLES = OBSTACLE_BEGIN - OBSTACLE_END + 1,
		NUM_SOUNDS = SOUND_END - SOUND_BEGIN + 1,
		DUMMY = NODE_DUMMY,
		LAYER = NODE_LAYER,
		TRIGGER = NODE_TRIGGER,
		REFERENCE = NODE_REFERENCE,
		EXTERN = NODE_EXTERN,
	};

	enum CALLBACK_INDEX
	{
		CALLBACK_PROPERTY_NODE_SLOTS_CHANGED,
		CALLBACK_PROPERTY_NODE_ADD,
		CALLBACK_PROPERTY_NODE_SWAP,
		CALLBACK_PROPERTY_NODE_REMOVE,
		CALLBACK_PROPERTY_CHANGE_ENABLED,
		CALLBACK_PROPERTY_SURFACE_ADD,
		CALLBACK_PROPERTY_SURFACE_REMOVE,
		CALLBACK_CACHE_NODE_ADD,
		CALLBACK_NODE_LOAD,
		CALLBACK_NODE_CLONE,
		CALLBACK_NODE_SWAP,
		CALLBACK_NODE_REMOVE,
		CALLBACK_NODE_CHANGE_ENABLED,
	};

	enum LIFETIME
	{
		LIFETIME_WORLD,
		LIFETIME_ENGINE,
		LIFETIME_MANUAL,
	};
	static Ptr<Node> getCloneNode(const Ptr<Node> &original_node);
	static Ptr<Property> getCloneProperty(const Ptr<Property> &original_property);
	static Ptr<Node> getNode(int id);
	static int isNode(const Ptr<Node> &node);
	static int isNode(int id);
	static Node::TYPE getTypeID(const char *type);
	static const char *getTypeName(Node::TYPE type);
	Ptr<Node> clone() const;
	void swap(const Ptr<Node> &n) const;
	void setID(int id);
	int getID() const;
	Node::TYPE getType() const;
	const char *getTypeName() const;
	bool isWorld() const;
	bool isGeodetic() const;
	bool isLight() const;
	bool isDecal() const;
	bool isLandscapeLayer() const;
	bool isObject() const;
	bool isPlayer() const;
	bool isPhysical() const;
	bool isNavigation() const;
	bool isObstacle() const;
	bool isSound() const;
	bool isField() const;
	bool isExtern() const;
	bool isEnabledSelf() const;
	Node::LIFETIME getLifetimeSelf() const;
	void setEnabled(bool enabled);
	bool isEnabled() const;
	void setHandled(bool handled);
	bool isHandled() const;
	void setImmovable(bool immovable);
	bool isImmovable() const;
	void setTriggerInteractionEnabled(bool enabled);
	bool isTriggerInteractionEnabled() const;
	void setGrassInteractionEnabled(bool enabled);
	bool isGrassInteractionEnabled() const;
	void setClutterInteractionEnabled(bool enabled);
	bool isClutterInteractionEnabled() const;
	void setQuery(bool query);
	bool isQuery() const;
	bool hasQueryForce() const;
	void setShowInEditorEnabled(bool enabled);
	bool isShowInEditorEnabled() const;
	void setSaveToWorldEnabled(bool enabled);
	bool isSaveToWorldEnabled() const;
	void setLifetime(Node::LIFETIME lifetime);
	Node::LIFETIME getLifetime() const;
	bool isShowInEditorEnabledSelf() const;
	bool isSaveToWorldEnabledSelf() const;
	void setShowInEditorEnabledRecursive(bool enable);
	void setSaveToWorldEnabledRecursive(bool enable);
	void setName(const char *name);
	const char *getName() const;
	void setData(const char *name, const char *data);
	const char *getData(const char *name);
	void setPtr(void *ptr);
	void *getPtr();
	int getNumAncestors() const;
	Ptr<Node> getAncestor(int num) const;
	int findAncestor(const char *name) const;
	int findAncestor(int type) const;
	void setParent(const Ptr<Node> &parent);
	Ptr<Node> getParent() const;
	void setWorldParent(const Ptr<Node> &n);
	Ptr<Node> getRootNode() const;
	void addChild(const Ptr<Node> &n);
	void removeChild(const Ptr<Node> &n);
	void addWorldChild(const Ptr<Node> &n);
	void removeWorldChild(const Ptr<Node> &n);
	bool isChild(const Ptr<Node> &n) const;
	int getNumChildren() const;
	int findChild(const char *name) const;
	Ptr<Node> getChild(int num) const;
	void setChildIndex(const Ptr<Node> &n, int index);
	int getChildIndex(const Ptr<Node> &n);
	Ptr<Node> findNode(const char *name, int recursive = 0) const;
	Ptr<Node> getPossessor() const;
	void getHierarchy(Vector<Ptr<Node>> &hierarchy);
	int getNumProperties() const;
	int addProperty(const char *name);
	int addProperty(const UGUID &guid);
	int addProperty(const Ptr<Property> &property);
	int findProperty(const char *name) const;
	int findProperty(const UGUID &guid) const;
	int findProperty(const Ptr<Property> &property) const;
	int setProperty(const char *name);
	int setProperty(const UGUID &guid);
	int setProperty(const Ptr<Property> &property);
	int setProperty(int num, const char *name);
	int setProperty(int num, const UGUID &guid);
	int setProperty(int num, const Ptr<Property> &property);
	int insertProperty(int num, const char *name);
	int insertProperty(int num, const UGUID &guid);
	int insertProperty(int num, const Ptr<Property> &property);
	void swapProperty(int from_num, int to_num);
	void removeProperty(int num = 0);
	void removeProperty(const char *name);
	void removeProperty(const UGUID &guid);
	void removeProperty(const Ptr<Property> &property);
	void clearProperties();
	Ptr<Property> getProperty(int num = 0) const;
	const char *getPropertyName(int num = 0) const;
	void setPropertyEnabled(int num, bool enable);
	bool isPropertyEnabled(int num) const;
	UGUID getLostNodePropertyGUID(int num) const;
	void setTransform(const Math::Mat4 &transform);
	Math::Mat4 getTransform() const;
	void setWorldTransform(const Math::Mat4 &transform);
	Math::Mat4 getWorldTransform() const;
	void setOldWorldTransform(const Math::Mat4 &transform);
	Math::Mat4 getOldWorldTransform() const;
	Math::Vec3 getOldWorldPosition() const;
	Math::Mat4 getIWorldTransform() const;
	void setTransformWithoutChildren(const Math::Mat4 &transform);
	void setWorldTransformWithoutChildren(const Math::Mat4 &transform);
	Math::BoundBox getBoundBox() const;
	Math::BoundSphere getBoundSphere() const;
	Math::WorldBoundBox getWorldBoundBox() const;
	Math::WorldBoundSphere getWorldBoundSphere() const;
	Math::WorldBoundBox getSpatialBoundBox() const;
	Math::WorldBoundSphere getSpatialBoundSphere() const;
	Math::WorldBoundBox getHierarchyBoundBox() const;
	Math::WorldBoundSphere getHierarchyBoundSphere() const;
	Math::WorldBoundBox getHierarchyWorldBoundBox() const;
	Math::WorldBoundSphere getHierarchyWorldBoundSphere() const;
	Math::WorldBoundBox getHierarchySpatialBoundBox() const;
	Math::WorldBoundSphere getHierarchySpatialBoundSphere() const;
	bool loadWorld(const Ptr<Xml> &xml);
	bool saveWorld(const Ptr<Xml> &xml) const;
	bool saveState(const Ptr<Stream> &stream) const;
	bool restoreState(const Ptr<Stream> &stream);
	Ptr<Body> getObjectBody() const;
	Ptr<BodyRigid> getObjectBodyRigid() const;
	Math::vec3 getBodyLinearVelocity() const;
	Math::vec3 getBodyAngularVelocity() const;
	Math::vec3 getBodyWorldVelocity(const Math::Vec3 &point) const;
	int getNumWorldTriggers() const;
	Ptr<WorldTrigger> getWorldTrigger(int num);
	void setPosition(const Math::Vec3 &pos);
	Math::Vec3 getPosition() const;
	void setRotation(const Math::quat &rot, bool identity = false);
	Math::quat getRotation() const;
	void setDirection(const Math::vec3 &dir, const Math::vec3 &up, MathLib::AXIS axis = Math::AXIS_NZ);
	Math::vec3 getDirection(MathLib::AXIS axis = Math::AXIS_NZ) const;
	void setScale(const Math::vec3 &s);
	Math::vec3 getScale() const;
	void setWorldPosition(const Math::Vec3 &pos);
	Math::Vec3 getWorldPosition() const;
	void setWorldRotation(const Math::quat &rot, bool identity = false);
	Math::quat getWorldRotation() const;
	void setWorldDirection(const Math::vec3 &dir, const Math::vec3 &up, MathLib::AXIS axis = Math::AXIS_NZ);
	Math::vec3 getWorldDirection(MathLib::AXIS axis = Math::AXIS_NZ) const;
	void setWorldScale(const Math::vec3 &s);
	Math::vec3 getWorldScale() const;
	void translate(const Math::Vec3 &t);
	void translate(Math::Scalar x, Math::Scalar y, Math::Scalar z);
	void rotate(const Math::quat &r);
	void rotate(const Math::vec3 &angles);
	void rotate(float angle_x, float angle_y, float angle_z);
	void worldTranslate(const Math::Vec3 &t);
	void worldTranslate(Math::Scalar x, Math::Scalar y, Math::Scalar z);
	void worldRotate(const Math::quat &r);
	void worldRotate(const Math::vec3 &angles);
	void worldRotate(float angle_x, float angle_y, float angle_z);
	void worldLookAt(const Math::Vec3 &target);
	void worldLookAt(const Math::Vec3 &target, const Math::vec3 &up);
	Math::Vec3 toWorld(const Math::vec3 &p) const;
	Math::vec3 toLocal(const Math::Vec3 &p) const;
	void renderVisualizer();
	void setVariable(const Variable &variable);
	Variable  getVariable() const;
	bool hasVariable() const;
	void setVariable(const char *name, const Variable &variable);
	Variable  getVariable(const char *name) const;
	bool hasVariable(const char *name) const;
	void removeVariable(const char *name);
	Ptr<GeodeticPivot> getGeodeticPivot() const;
	void updateEnabled();
	static void *addCallback(Node::CALLBACK_INDEX callback, CallbackBase2<Ptr<Node>, Ptr<Property>> *func);
	static void *addCallback(Node::CALLBACK_INDEX callback, CallbackBase3<Ptr<Node>, Ptr<Property>, int> *func);
	static void *addCallback(Node::CALLBACK_INDEX callback, CallbackBase3<Ptr<Node>, int, int> *func);
	static void *addCallback(Node::CALLBACK_INDEX callback, CallbackBase2<Ptr<Node>, Ptr<Node>> *func);
	static void *addCallback(Node::CALLBACK_INDEX callback, CallbackBase2<Ptr<Node>, int> *func);
	static void *addCallback(Node::CALLBACK_INDEX callback, CallbackBase1<Ptr<Node>> *func);
	static bool removeCallback(Node::CALLBACK_INDEX callback, void *id);
	static void clearCallbacks(Node::CALLBACK_INDEX callback);
};
typedef Ptr<Node> NodePtr;

} // namespace Unigine
