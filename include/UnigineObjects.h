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

#include "UnigineRender.h"
#include "UnigineGui.h"
#include "UnigineMaterial.h"
#include "UnigineMesh.h"
#include "UnigineMeshStatic.h"
#include "UnigineNode.h"
#include "UnigineTextures.h"
#include "UnigineTileset.h"
#include "UnigineCurve2d.h"

namespace Unigine
{

class Body;
class BodyRigid;

class UNIGINE_API ObjectIntersection : public APIInterface
{
public:
	static bool convertible(ObjectIntersection *obj) { return obj != nullptr; }

	enum TYPE
	{
		OBJECT_INTERSECTION = 0,
		OBJECT_INTERSECTION_NORMAL,
		OBJECT_INTERSECTION_TEX_COORD,
		NUM_OBJECT_INTERSECTIONS,
	};
	static Ptr<ObjectIntersection> create();
	ObjectIntersection::TYPE getType() const;
	const char *getTypeName() const;
	void setPoint(const Math::Vec3 &point);
	Math::Vec3 getPoint() const;
	void setIndex(int index);
	int getIndex() const;
	void setInstance(int instance);
	int getInstance() const;
};
typedef Ptr<ObjectIntersection> ObjectIntersectionPtr;


class UNIGINE_API ObjectIntersectionNormal : public ObjectIntersection
{
public:
	static bool convertible(ObjectIntersection *obj) {
		if(!obj)
			return false;
		int type = obj->getType();
		switch(type)
		{
			case ObjectIntersection::OBJECT_INTERSECTION_NORMAL:
			case ObjectIntersection::OBJECT_INTERSECTION_TEX_COORD:
			return true;
		}
		return false;
	}
	static Ptr<ObjectIntersectionNormal> create();
	void setNormal(const Math::vec3 &normal);
	Math::vec3 getNormal() const;
};
typedef Ptr<ObjectIntersectionNormal> ObjectIntersectionNormalPtr;


class UNIGINE_API ObjectIntersectionTexCoord : public ObjectIntersectionNormal
{
public:
	static bool convertible(ObjectIntersection *obj) { return obj && obj->getType() == ObjectIntersection::OBJECT_INTERSECTION_TEX_COORD; }
	static Ptr<ObjectIntersectionTexCoord> create();
	void setTexCoord(const Math::vec4 &coord);
	Math::vec4 getTexCoord() const;
};
typedef Ptr<ObjectIntersectionTexCoord> ObjectIntersectionTexCoordPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Object : public Node
{
public:
	static bool convertible(Node *node) { return node && node->isObject(); }


	enum SURFACE_SHADOW_MODE
	{
		SURFACE_SHADOW_MODE_MIXED = 0,
		SURFACE_SHADOW_MODE_DYNAMIC,
	};

	enum SURFACE_LIGHTING_MODE
	{
		SURFACE_LIGHTING_MODE_STATIC = 0,
		SURFACE_LIGHTING_MODE_DYNAMIC,
		SURFACE_LIGHTING_MODE_ADVANCED,
	};
	using Node::getParent;
	using Node::setParent;
	using Node::isEnabled;
	using Node::setEnabled;
	using Node::getBoundSphere;
	using Node::getBoundBox;
	using Node::getWorldBoundBox;
	using Node::getWorldBoundSphere;
	void setBody(const Ptr<Body> &body, bool update);
	void setBody(const Ptr<Body> &body);
	Ptr<Body> getBody() const;
	bool isVisibleCamera() const;
	bool isVisibleShadow() const;
	Ptr<BodyRigid> getBodyRigid() const;
	void flushBodyTransform();
	void setEnabled(bool enabled);
	bool isEnabled() const;
	int getNumSurfaces() const;
	const char *getSurfaceName(int surface) const;
	int findSurface(const char *name) const;
	void setEnabled(bool enabled, int surface);
	bool isEnabled(int surface) const;
	void setParent(int parent, int surface);
	int getParent(int surface) const;
	void setCastShadow(bool enabled, int surface);
	bool getCastShadow(int surface) const;
	void setCastWorldShadow(bool enabled, int surface);
	bool getCastWorldShadow(int surface) const;
	void setBakeToEnvProbe(bool enabled, int surface);
	bool getBakeToEnvProbe(int surface) const;
	void setBakeToGI(bool enabled, int surface);
	bool getBakeToGI(int surface) const;
	void setCastEnvProbeShadow(bool enabled, int surface);
	bool getCastEnvProbeShadow(int surface) const;
	void setShadowMode(Object::SURFACE_SHADOW_MODE mode, int surface);
	Object::SURFACE_SHADOW_MODE getShadowMode(int surface) const;
	void setPhysicsIntersection(bool enabled, int surface);
	bool getPhysicsIntersection(int surface) const;
	void setIntersection(bool enabled, int surface);
	bool getIntersection(int surface) const;
	void setCollision(bool enabled, int surface);
	bool getCollision(int surface) const;
	void setShadowMask(int mask, int surface);
	int getShadowMask(int surface) const;
	void setViewportMask(int mask, int surface);
	int getViewportMask(int surface) const;
	void setPhysicsIntersectionMask(int mask, int surface);
	int getPhysicsIntersectionMask(int surface) const;
	void setIntersectionMask(int mask, int surface);
	int getIntersectionMask(int surface) const;
	void setCollisionMask(int mask, int surface);
	int getCollisionMask(int surface) const;
	void setMinParent(int parent, int surface);
	int getMinParent(int surface) const;
	void setMinVisibleDistance(float distance, int surface);
	float getMinVisibleDistance(int surface) const;
	void setMinFadeDistance(float distance, int surface);
	float getMinFadeDistance(int surface) const;
	void setMaxParent(int parent, int surface);
	int getMaxParent(int surface) const;
	void setMaxVisibleDistance(float distance, int surface);
	float getMaxVisibleDistance(int surface) const;
	void setMaxFadeDistance(float distance, int surface);
	float getMaxFadeDistance(int surface) const;
	void setSoundOcclusionMask(int mask, int surface);
	int getSoundOcclusionMask(int surface) const;
	void setSoundOcclusion(float occlusion, int surface);
	float getSoundOcclusion(int surface) const;
	void setPhysicsFriction(float value, int surface);
	float getPhysicsFriction(int surface) const;
	void setPhysicsRestitution(float value, int surface);
	float getPhysicsRestitution(int surface) const;
	void setMaterialPath(const char *path, int surface);
	void setMaterialPath(const char *path, const char *pattern);
	const char *getMaterialPath(int surface) const;
	void setMaterialGUID(const UGUID &guid, int surface);
	void setMaterialGUID(const UGUID &guid, const char *pattern);
	UGUID getMaterialGUID(int surface) const;
	void setMaterial(const Ptr<Material> &mat, int surface);
	void setMaterial(const Ptr<Material> &mat, const char *pattern);
	Ptr<Material> getMaterial(int surface) const;
	Ptr<Material> getMaterialInherit(int surface);
	bool isMaterialInherited(int surface) const;
	void clearMaterialInherit(int surface);
	void setMaterialState(const char *name, int state, int surface);
	int getMaterialState(const char *name, int surface) const;
	void setMaterialTexture(const char *name, const char *texture, int surface);
	const char *getMaterialTexture(const char *name, int surface) const;
	void setMaterialParameterFloat4(const char *name, const Math::vec4 &parameter, int surface);
	Math::vec4 getMaterialParameterFloat4(const char *name, int surface) const;
	void setMaterialParameterFloat3(const char *name, const Math::vec3 &parameter, int surface);
	Math::vec3 getMaterialParameterFloat3(const char *name, int surface) const;
	void setMaterialParameterFloat2(const char *name, const Math::vec2 &parameter, int surface);
	Math::vec2 getMaterialParameterFloat2(const char *name, int surface) const;
	void setMaterialParameterFloat(const char *name, float parameter, int surface);
	float getMaterialParameterFloat(const char *name, int surface) const;
	void setMaterialParameterInt4(const char *name, const Math::ivec4 &parameter, int surface);
	Math::ivec4 getMaterialParameterInt4(const char *name, int surface) const;
	void setMaterialParameterInt3(const char *name, const Math::ivec3 &parameter, int surface);
	Math::ivec3 getMaterialParameterInt3(const char *name, int surface) const;
	void setMaterialParameterInt2(const char *name, const Math::ivec2 &parameter, int surface);
	Math::ivec2 getMaterialParameterInt2(const char *name, int surface) const;
	void setMaterialParameterInt(const char *name, int parameter, int surface);
	int getMaterialParameterInt(const char *name, int surface) const;
	UGUID getLostMaterialGUID(int surface) const;
	int setSurfaceProperty(const char *name, int surface);
	int setSurfaceProperty(const char *name, const char *pattern);
	int setSurfaceProperty(const UGUID &guid, int surface);
	int setSurfaceProperty(const UGUID &guid, const char *pattern);
	int setSurfaceProperty(const Ptr<Property> &property, int surface);
	int setSurfaceProperty(const Ptr<Property> &property, const char *pattern);
	void clearSurfaceProperty(int surface);
	Ptr<Property> getSurfaceProperty(int surface) const;
	const char *getSurfacePropertyName(int surface) const;
	Ptr<Property> getSurfacePropertyInherit(int surface);
	void clearSurfacePropertyInherit(int surface);
	bool isSurfacePropertyInherited(int surface) const;
	UGUID getLostSurfacePropertyGUID(int surface) const;
	Math::BoundBox getBoundBox(int surface) const;
	Math::BoundSphere getBoundSphere(int surface) const;
	bool getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, Math::Vec3 *ret_point, Math::vec3 *ret_normal, Math::vec4 *ret_texcoord, int *ret_index, int *ret_instance, int surface) const;
	bool getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, Math::Vec3 *ret_point, Math::vec3 *ret_normal, Math::vec4 *ret_texcoord, int *ret_index, int *ret_instance, int *ret_surface) const;
	bool getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, const Ptr<ObjectIntersection> &v, int *ret_surface) const;
	bool getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, const Ptr<ObjectIntersectionNormal> &v, int *ret_surface) const;
	bool getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, int mask, const Ptr<ObjectIntersectionTexCoord> &v, int *ret_surface) const;
	bool getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, const Ptr<ObjectIntersection> &v, int surface) const;
	bool getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, const Ptr<ObjectIntersectionNormal> &v, int surface) const;
	bool getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, const Ptr<ObjectIntersectionTexCoord> &v, int surface) const;
	int getNumTriangles(int surface) const;
	Math::WorldBoundBox getWorldBoundBox(int surface) const;
	Math::WorldBoundSphere getWorldBoundSphere(int surface) const;
	void render(Render::PASS pass, int surface);
	void setLightingMode(Object::SURFACE_LIGHTING_MODE mode, int surface);
	Object::SURFACE_LIGHTING_MODE getLightingMode(int surface) const;
};
typedef Ptr<Object> ObjectPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectDummy : public Object
{
public:
	static int type() { return Node::OBJECT_DUMMY; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectDummy> create();
};
typedef Ptr<ObjectDummy> ObjectDummyPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectDynamic : public Object
{
public:
	static int type() { return Node::OBJECT_DYNAMIC; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }


	enum
	{
		IMMUTABLE_VERTEX = 1 << 0,
		IMMUTABLE_INDICES = 1 << 1,
		DYNAMIC_VERTEX = 1 << 2,
		DYNAMIC_INDICES = 1 << 3,
		IMMUTABLE_ALL = (IMMUTABLE_VERTEX | IMMUTABLE_INDICES),
		DYNAMIC_ALL = (DYNAMIC_VERTEX | DYNAMIC_INDICES),
	};

	enum MODE
	{
		MODE_POINTS = 0,
		MODE_LINES,
		MODE_TRIANGLES,
		MODE_TRIANGLE_PATCHES,
		MODE_QUAD_PATCHES,
	};

	enum
	{
		TYPE_HALF = 0,
		TYPE_FLOAT,
		TYPE_UCHAR,
		TYPE_USHORT,
	};
	static Ptr<ObjectDynamic> create(int flags = 0);
	struct Attribute
	{
	int offset;
	int type;
	int size;
	};
	void setMaterialNodeType(Node::TYPE type);
	Node::TYPE getMaterialNodeType() const;
	void setInstancing(bool instancing);
	bool getInstancing() const;
	void setParameterBool(const char *name, bool value);
	void setParameterInt(const char *name, const int *value, int value_size);
	void setParameterFloat(const char *name, const float *value, int value_size);
	void setParameterFloatArray(const char *name, const float *value, int value_size, int num);
	void clearSurfaces();
	void addSurface(const char *name);
	void setSurfaceName(const char *name, int surface);
	void updateSurfaceBegin(int surface);
	void setSurfaceBegin(int begin, int surface);
	int getSurfaceBegin(int surface) const;
	void updateSurfaceEnd(int surface);
	void setSurfaceEnd(int end, int surface);
	int getSurfaceEnd(int surface) const;
	void setSurfaceMode(ObjectDynamic::MODE mode, int surface);
	ObjectDynamic::MODE getSurfaceMode(int surface) const;
	void setVertexFormat(const ObjectDynamic::Attribute *attributes, int attributes_size);
	int getVertexSize() const;
	int getNumAttributes() const;
	const ObjectDynamic::Attribute * getAttributes() const;
	void clearVertex();
	void allocateVertex(int num);
	void removeVertex(int num, int size, int indices);
	void setNumVertex(int vertex);
	int getNumVertex() const;
	void addVertex(const void *vertex);
	void setVertex(int num, const void *vertex);
	void addVertexArray(const void *vertex, int num_vertex);
	void setVertexArray(const void *vertex, int num_vertex);
	const void *getVertexArray() const;
	const void *getVertex(int num) const;
	void addVertexHalf(int attribute, const float *value, int value_size);
	void setVertexHalf(int attribute, const float *value, int value_size);
	void setVertexHalf(int vertex, int attribute, const float *value, int value_size);
	void addVertexFloat(int attribute, const float *value, int value_size);
	void setVertexFloat(int attribute, const float *value, int value_size);
	void setVertexFloat(int vertex, int attribute, const float *value, int value_size);
	void addVertexUChar(int attribute, const unsigned char *value, int value_size);
	void setVertexUChar(int attribute, const unsigned char *value, int value_size);
	void setVertexUChar(int vertex, int attribute, const unsigned char *value, int value_size);
	void addVertexUShort(int attribute, const unsigned short *value, int value_size);
	void setVertexUShort(int attribute, const unsigned short *value, int value_size);
	void setVertexUShort(int vertex, int attribute, const unsigned short *value, int value_size);
	void clearIndices();
	void allocateIndices(int num);
	void removeIndices(int num, int size);
	void setNumIndices(int indices);
	int getNumIndices() const;
	void addIndex(int index);
	void setIndex(int num, int index);
	void addIndicesArray(const int *indices, int indices_size);
	void setIndicesArray(const int *indices, int indices_size);
	const int *getIndicesArray() const;
	int getIndex(int num) const;
	void addTriangles(int num_triangles);
	void addTriangleFan(int num_vertex);
	void addTriangleStrip(int num_vertex);
	void addTriangleQuads(int num_quads);
	void addLineStrip(int num_vertex);
	void addPoints(int num_points);
	void setBoundBox(const Math::BoundBox &bb, int surface);
	void setBoundBox(const Math::BoundBox &bb);
	void flushVertex();
	void flushIndices();
};
typedef Ptr<ObjectDynamic> ObjectDynamicPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectMeshStatic : public Object
{
public:
	static int type() { return Node::OBJECT_MESH_STATIC; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }


	enum LIGHTMAP_MODE
	{
		LIGHTMAP_MODE_UNIQUE = 0,
		LIGHTMAP_MODE_CUSTOM,
		LIGHTMAP_MODE_SURFACE,
	};

	enum LIGHTMAP_QUALITY
	{
		LIGHTMAP_QUALITY_GLOBAL = 0,
		LIGHTMAP_QUALITY_DRAFT,
		LIGHTMAP_QUALITY_LOW,
		LIGHTMAP_QUALITY_MEDIUM,
		LIGHTMAP_QUALITY_HIGH,
		LIGHTMAP_QUALITY_ULTRA,
	};

	enum SURFACE_CUSTOM_TEXTURE_MODE
	{
		SURFACE_CUSTOM_TEXTURE_MODE_UNIQUE = 0,
		SURFACE_CUSTOM_TEXTURE_MODE_SURFACE,
	};
	static Ptr<ObjectMeshStatic> create(const Ptr<Mesh> &mesh);
	static Ptr<ObjectMeshStatic> create(const char *path, bool unique = false);
	bool createMesh(const char *path, bool unique = false);
	bool loadMesh(const char *path);
	bool saveMesh(const char *path) const;
	bool setMesh(const Ptr<Mesh> &mesh, bool unique = true);
	bool getMesh(const Ptr<Mesh> &mesh) const;
	Ptr<MeshStatic> getMeshStatic() const;
	void flushMesh();
	bool isFlushed() const;
	void setMeshName(const char *name);
	const char *getMeshName() const;
	void setMeshNameForce(const char *path);
	void setLightmapEnabled(bool enabled, int surface);
	bool isLightmapEnabled(int surface) const;
	void setLightmapCompression(bool enabled, int surface);
	bool isLightmapCompression(int surface) const;
	void setLightmapQuality(ObjectMeshStatic::LIGHTMAP_QUALITY quality, int surface);
	ObjectMeshStatic::LIGHTMAP_QUALITY getLightmapQuality(int surface) const;
	void setLightmapMode(ObjectMeshStatic::LIGHTMAP_MODE mode, int surface);
	ObjectMeshStatic::LIGHTMAP_MODE getLightmapMode(int surface) const;
	void setLightmapSourceSurface(int source_surface, int surface);
	int getLightmapSourceSurface(int surface) const;
	void setLightmapTexturePath(const char *path, int surface);
	const char *getLightmapTexturePath(int surface) const;
	void setSurfaceCustomTextureEnabled(bool enabled, int surface);
	bool isSurfaceCustomTextureEnabled(int surface) const;
	void setSurfaceCustomTextureMode(ObjectMeshStatic::SURFACE_CUSTOM_TEXTURE_MODE mode, int surface);
	ObjectMeshStatic::SURFACE_CUSTOM_TEXTURE_MODE getSurfaceCustomTextureMode(int surface) const;
	void setSurfaceCustomTextureSourceSurface(int source_surface, int surface);
	int getSurfaceCustomTextureSourceSurface(int surface) const;
	void setSurfaceCustomTexturePath(const char *path, int surface);
	const char *getSurfaceCustomTexturePath(int surface) const;
	void setSurfaceCustomTexture(const Ptr<Texture> &texture, int surface);
	Ptr<Texture> getSurfaceCustomTexture(int surface) const;
	void updateSurfaceBounds(int surface = -1);
	void setSurfaceTransform(const Math::mat4 &transform, int surface, int target = -1);
	int addMeshSurface(const char *name, const Ptr<Mesh> &mesh, int surface, int target = -1);
	int addMeshSurface(const char *name, const Ptr<ObjectMeshStatic> &mesh, int surface, int target = -1);
	int addMeshSurface(int dest_surface, const Ptr<ObjectMeshStatic> &mesh, int surface, int target = -1);
	int getMeshSurface(const Ptr<Mesh> &mesh, int surface, int target = -1) const;
	int addEmptySurface(const char *name, int num_vertex, int num_indices);
	int addSurfaceTarget(int surface, const char *name = 0);
	int getNumVertex(int surface) const;
	void setVertex(int num, const Math::vec3 &vertex, int surface, int target = 0);
	Math::vec3 getVertex(int num, int surface, int target = 0) const;
	int getNumTangents(int surface) const;
	void setTangent(int num, const Math::quat &tangent, int surface, int target = 0);
	Math::quat getTangent(int num, int surface, int target = 0) const;
	Math::vec3 getNormal(int num, int surface, int target = 0) const;
	void setNumTexCoords0(int num, int surface);
	int getNumTexCoords0(int surface) const;
	void setTexCoord0(int num, const Math::vec2 &texcoord, int surface);
	Math::vec2 getTexCoord0(int num, int surface) const;
	void setNumTexCoords1(int num, int surface);
	int getNumTexCoords1(int surface) const;
	void setTexCoord1(int num, const Math::vec2 &texcoord, int surface);
	Math::vec2 getTexCoord1(int num, int surface) const;
	int getNumColors(int surface) const;
	void setColor(int num, const Math::vec4 &color, int surface);
	Math::vec4 getColor(int num, int surface) const;
	int getNumCIndices(int surface) const;
	void setCIndex(int num, int index, int surface);
	int getCIndex(int num, int surface) const;
	int getNumTIndices(int surface) const;
	void setTIndex(int num, int index, int surface);
	int getTIndex(int num, int surface) const;
	int getNumSurfaceTargets(int surface) const;
	const char *getSurfaceTargetName(int surface, int target) const;
	int findSurfaceTarget(const char *name, int surface) const;
};
typedef Ptr<ObjectMeshStatic> ObjectMeshStaticPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectMeshSkinned : public Object
{
public:
	static int type() { return Node::OBJECT_MESH_SKINNED; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }


	enum BIND_MODE
	{
		BIND_MODE_OVERRIDE = 0,
		BIND_MODE_ADDITIVE,
	};

	enum NODE_SPACE
	{
		NODE_SPACE_WORLD = 0,
		NODE_SPACE_LOCAL,
	};

	enum BONE_SPACE
	{
		BONE_SPACE_WORLD = 0,
		BONE_SPACE_OBJECT,
		BONE_SPACE_LOCAL,
	};
	static Ptr<ObjectMeshSkinned> create(const Ptr<Mesh> &mesh);
	static Ptr<ObjectMeshSkinned> create(const char *path, bool unique = false);
	int createMesh(const char *path, bool unique = false);
	int loadMesh(const char *path);
	int saveMesh(const char *path);
	int setMesh(const Ptr<Mesh> &mesh);
	int getMesh(Ptr<Mesh> &mesh) const;
	void flushMesh();
	bool isFlushed() const;
	void setMeshName(const char *name);
	const char *getMeshName() const;
	void setMeshNameForce(const char *path);
	void setAnimName(const char *name);
	const char *getAnimName() const;
	void setAnimNameForce(const char *name);
	void setFPSVisibleCamera(int camera);
	int getFPSVisibleCamera() const;
	void setFPSVisibleShadow(int shadow);
	int getFPSVisibleShadow() const;
	void setFPSInvisible(int invisible);
	int getFPSInvisible() const;
	void setUpdateDistanceLimit(float limit);
	float getUpdateDistanceLimit() const;
	void setQuaternion(bool quaternion);
	bool isQuaternion() const;
	void setControlled(bool controlled);
	bool isControlled() const;
	void setLoop(int loop);
	int getLoop() const;
	void setTime(float time);
	float getTime() const;
	void setSpeed(float speed);
	float getSpeed() const;
	void play();
	void stop();
	bool isPlaying() const;
	bool isStopped() const;
	void updateSurfaceBounds(int surface = -1);
	void setSurfaceTransform(const Math::mat4 &transform, int surface, int target = -1);
	int addMeshSurface(const char *name, const Ptr<Mesh> &mesh, int surface, int target = -1);
	int addMeshSurface(const char *name, const Ptr<ObjectMeshSkinned> &mesh, int surface, int target = -1);
	int addMeshSurface(int dest_surface, const Ptr<ObjectMeshSkinned> &mesh, int surface, int target = -1);
	int getMeshSurface(const Ptr<Mesh> &mesh, int surface, int target = -1) const;
	int addEmptySurface(const char *name, int num_vertex, int num_indices);
	int addSurfaceTarget(int surface, const char *name = 0);
	int addSurfaceTarget(int dest_surface, const Ptr<ObjectMeshSkinned> &src_mesh, int src_surface, int src_target = -1);
	void mergeMeshSurface(int dest_surface, const Ptr<ObjectMeshSkinned> &src_mesh, int src_surface);
	int getNumVertex(int surface) const;
	void setVertex(int num, const Math::vec3 &vertex, int surface, int target = 0);
	Math::vec3 getVertex(int num, int surface, int target = 0) const;
	Math::vec3 getSkinnedVertex(int num, int surface) const;
	int getNumTangents(int surface) const;
	void setTangent(int num, const Math::quat &tangent, int surface, int target = 0);
	Math::quat getTangent(int num, int surface, int target = 0) const;
	Math::vec3 getNormal(int num, int surface, int target = 0) const;
	Math::quat getSkinnedTangent(int num, int index, int surface) const;
	Math::vec3 getSkinnedNormal(int num, int index, int surface) const;
	void setNumTexCoords0(int num, int surface);
	int getNumTexCoords0(int surface) const;
	void setTexCoord0(int num, const Math::vec2 &texcoord, int surface);
	Math::vec2 getTexCoord0(int num, int surface) const;
	void setNumTexCoords1(int num, int surface);
	int getNumTexCoords1(int surface) const;
	void setTexCoord1(int num, const Math::vec2 &texcoord, int surface);
	Math::vec2 getTexCoord1(int num, int surface) const;
	int getNumColors(int surface) const;
	void setColor(int num, const Math::vec4 &color, int surface);
	Math::vec4 getColor(int num, int surface) const;
	int getNumCIndices(int surface) const;
	void setCIndex(int num, int index, int surface);
	int getCIndex(int num, int surface) const;
	int getNumTIndices(int surface) const;
	void setTIndex(int num, int index, int surface);
	int getTIndex(int num, int surface) const;
	int addTarget(int surface);
	void removeTarget(int target, int surface);
	void setNumTargets(int num, int surface);
	int getNumTargets(int surface) const;
	void setTarget(int target, bool enabled, int index, float weight, int surface);
	void setTargetEnabled(int target, bool enabled, int surface);
	int isTargetEnabled(int target, int surface) const;
	void setTargetIndex(int target, int index, int surface);
	int getTargetIndex(int target, int surface) const;
	void setTargetWeight(int target, float weight, int surface);
	float getTargetWeight(int target, int surface) const;
	int addLayer();
	void removeLayer(int layer);
	void setNumLayers(int layers);
	int getNumLayers() const;
	void setLayer(int layer, bool enabled, float weight);
	void setLayerEnabled(int layer, bool enabled);
	bool isLayerEnabled(int layer) const;
	void setLayerWeight(int layer, float weight);
	float getLayerWeight(int layer) const;
	void clearLayer(int layer);
	void importLayer(int layer);
	void copyLayer(int dest, int src);
	void inverseLayer(int dest, int src);
	void lerpLayer(int dest, int layer0, int layer1, float weight);
	void mulLayer(int dest, int layer0, int layer1, float weight = 1.0f);
	int getNumBones() const;
	const char *getBoneName(int bone) const;
	int findBone(const char *name) const;
	int getBoneParent(int bone) const;
	int getNumBoneChildren(int bone) const;
	int getBoneChild(int bone, int child) const;
	Math::mat4 getBoneBindTransform(int bone) const;
	Math::mat4 getBoneBindITransform(int bone) const;
	void setBoneTransform(int bone, const Math::mat4 &transform);
	void setBoneTransformWithChildren(int bone, const Math::mat4 &transform);
	void setBoneTransforms(const int *bones, const Math::mat4 *transforms, int num_bones);
	Math::mat4 getBoneTransform(int bone) const;
	Math::mat4 getBoneITransform(int bone) const;
	void setBoneWorldTransform(int bone, const Math::Mat4 &transform);
	void setBoneWorldTransformWithChildren(int bone, const Math::Mat4 &transform);
	Math::Mat4 getBoneWorldTransform(int bone) const;
	void setBoneLayerTransformEnabled(int layer, int bone, bool enabled);
	void setBoneLayerTransform(int layer, int bone, const Math::mat4 &transform);
	Math::mat4 getBoneLayerTransform(int layer, int bone) const;
	bool isBoneLayerTransform(int layer, int bone) const;
	Math::mat4 getBoneNotAdditionalBindLocalTransform(int bone) const;
	Math::mat4 getBoneNotAdditionalBindObjectTransform(int bone) const;
	Math::Mat4 getBoneNotAdditionalBindWorldTransform(int bone) const;
	void setBindNode(int bone, const Ptr<Node> &node);
	void removeBindNode(int bone);
	Ptr<Node> getBindNode(int bone) const;
	void setBindNodeSpace(int bone, ObjectMeshSkinned::NODE_SPACE space);
	ObjectMeshSkinned::NODE_SPACE getBindNodeSpace(int bone) const;
	void setBindBoneSpace(int bone, ObjectMeshSkinned::BONE_SPACE space);
	ObjectMeshSkinned::BONE_SPACE getBindBoneSpace(int bone) const;
	void setBindMode(int bone, ObjectMeshSkinned::BIND_MODE mode);
	ObjectMeshSkinned::BIND_MODE getBindMode(int bone) const;
	void setBindNodeOffset(int bone, const Math::Mat4 &offset);
	Math::Mat4 getBindNodeOffset(int bone) const;
	void addVisualizeBone(int bone);
	void removeVisualizeBone(int bone);
	void clearVisualizeBones();
	void setVisualizeAllBones(bool bones);
	bool isVisualizeAllBones() const;
	int addAnimation(const char *path);
	int addAnimation(const Ptr<Mesh> &mesh, const char *path = 0);
	void removeAnimation(int animation);
	int getNumAnimations() const;
	int getAnimationID(int num) const;
	const char *getAnimationPath(int animation) const;
	int getNumAnimationBones(int animation) const;
	int getNumAnimationFrames(int animation) const;
	int findAnimation(const char *path) const;
	int setAnimation(int layer, const char *path);
	int setAnimation(int layer, int animation);
	int getAnimation(int layer) const;
	int getNumFrames(int layer) const;
	float setFrame(int layer, float frame, int from = -1, int to = -1);
	float getFrame(int layer) const;
	int getFrameFrom(int layer) const;
	int getFrameTo(int layer) const;
	int getNumSurfaceTargets(int surface) const;
	const char *getSurfaceTargetName(int surface, int target) const;
	int findSurfaceTarget(const char *name, int surface) const;
	void updateSkinned();
};
typedef Ptr<ObjectMeshSkinned> ObjectMeshSkinnedPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectMeshDynamic : public Object
{
public:
	static int type() { return Node::OBJECT_MESH_DYNAMIC; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectMeshDynamic> create(const Ptr<Mesh> &mesh, int flags = 0);
	static Ptr<ObjectMeshDynamic> create(const char *path, int flags = 0);
	static Ptr<ObjectMeshDynamic> create(int flags = 0);
	struct Vertex
	{
		Math::vec3 xyz;
		Math::vec4 texcoord;
		Math::vec4 tangent;
		Math::vec4 color;
	};
	bool loadMesh(const char *path);
	bool saveMesh(const char *path);
	int setMesh(const Ptr<Mesh> &mesh);
	int getMesh(Ptr<Mesh> &mesh) const;
	void setMeshName(const char *name);
	const char *getMeshName() const;
	bool isUniqueMesh() const;
	void putUniqueMesh();
	void setFPSVisibleCamera(int camera);
	int getFPSVisibleCamera() const;
	void setFPSVisibleShadow(int shadow);
	int getFPSVisibleShadow() const;
	void setFPSInvisible(int invisible);
	int getFPSInvisible() const;
	void setUpdateDistanceLimit(float limit);
	float getUpdateDistanceLimit() const;
	void clearSurfaces();
	void addSurface(const char *name);
	void setSurfaceName(const char *name, int surface);
	void removeSurface(int surface);
	void removeSurfaces(const char *name);
	void updateSurfaceBegin(int surface);
	void setSurfaceBegin(int begin, int surface);
	int getSurfaceBegin(int surface) const;
	void updateSurfaceEnd(int surface);
	void setSurfaceEnd(int end, int surface);
	int getSurfaceEnd(int surface) const;
	void clearVertex();
	void allocateVertex(int num);
	void removeVertex(int num, int size, int indices);
	void setNumVertex(int vertex);
	int getNumVertex() const;
	void addVertex(const Math::vec3 &xyz);
	void setVertex(int num, const Math::vec3 &xyz);
	void addVertexArray(const ObjectMeshDynamic::Vertex *vertex, int vertex_size);
	void setVertexArray(const ObjectMeshDynamic::Vertex *vertex, int vertex_size);
	const ObjectMeshDynamic::Vertex * getVertexArray() const;
	Math::vec3 getVertex(int num) const;
	void addTexCoord(const Math::vec4 &texcoord);
	void setTexCoord(int num, const Math::vec4 &texcoord);
	Math::vec4 getTexCoord(int num) const;
	void addTangent(const Math::quat &tangent);
	void setTangent(int num, const Math::quat &tangent);
	Math::quat getTangent(int num) const;
	Math::vec3 getNormal(int num) const;
	void addColor(const Math::vec4 &color);
	void setColor(int num, const Math::vec4 &color);
	Math::vec4 getColor(int num) const;
	void clearIndices();
	void allocateIndices(int num);
	void removeIndices(int num, int size);
	void setNumIndices(int indices);
	int getNumIndices() const;
	void addIndex(int index);
	void setIndex(int num, int index);
	void addIndicesArray(const int *indices, int indices_size);
	void setIndicesArray(const int *indices, int indices_size);
	const int *getIndicesArray() const;
	int getIndex(int num) const;
	void addTriangles(int num_triangles);
	void addTriangleFan(int num_vertex);
	void addTriangleStrip(int num_vertex);
	void addTriangleQuads(int num_quads);
	void setBoundBox(const Math::BoundBox &bb, int surface);
	void setBoundBox(const Math::BoundBox &bb);
	bool updateBounds();
	bool updateIndices();
	bool updateTangents();
	void flushVertex();
	void flushIndices();
	bool isFlushed() const;

	enum
	{
		// immutable flags
		IMMUTABLE_VERTEX = 1 << 0,
		IMMUTABLE_INDICES = 1 << 1,
		IMMUTABLE_ALL = (IMMUTABLE_VERTEX | IMMUTABLE_INDICES),
		// dynamic flags
		DYNAMIC_VERTEX = 1 << 2,
		DYNAMIC_INDICES = 1 << 3,
		DYNAMIC_ALL = (DYNAMIC_VERTEX | DYNAMIC_INDICES),
	};
};
typedef Ptr<ObjectMeshDynamic> ObjectMeshDynamicPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectMeshCluster : public Object
{
public:
	static int type() { return Node::OBJECT_MESH_CLUSTER; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectMeshCluster> create(const char *path, bool unique = false);
	bool createMesh(const char *path, bool unique = false);
	bool loadMesh(const char *path);
	bool saveMesh(const char *path);
	int setMesh(const Ptr<Mesh> &mesh);
	int getMesh(Ptr<Mesh> &mesh) const;
	void flushMesh();
	void updateSpatialTree();
	void setMeshName(const char *name);
	const char *getMeshName() const;
	void setMeshNameForce(const char *name);
	void setVisibleDistance(float distance);
	float getVisibleDistance() const;
	void setFadeDistance(float distance);
	float getFadeDistance() const;
	int getNumMeshes() const;
	void removeMeshTransform(int num);
	void removeMeshTransformFast(int num);
	int addMeshTransform();
	Math::mat4 getMeshTransform(int num) const;
	void setMeshTransform(int num, const Math::mat4 &transform);
	void createMeshes(const Vector<Math::Mat4> &world_transforms);
	void appendMeshes(const Vector<Math::Mat4> &world_transforms);
	void clearMeshes();
	bool getClusterTransforms(const Math::WorldBoundBox &bounds, Vector<Math::mat4> &transforms);
	bool getClusterWorldTransforms(const Math::WorldBoundBox &bounds, Vector<Math::Mat4> &transforms);
	bool getClusterWorldTransformsFromSphere(const Math::WorldBoundSphere &bounds, Vector<Math::Mat4> &transforms);
	bool detachClusterTransforms(const Math::WorldBoundBox &bb, Vector<Math::mat4> &transforms);
	bool detachClusterWorldTransforms(const Math::WorldBoundBox &bb, Vector<Math::Mat4> &transforms);
	bool detachClusterWorldTransformsFromSphere(const Math::WorldBoundSphere &bb, Vector<Math::Mat4> &transforms);
	bool removeClusterTransforms(const Math::WorldBoundBox &bb);
	bool removeClusterTransformsFromSphere(const Math::WorldBoundSphere &bb);
	int getNumSurfaceTargets(int surface) const;
	const char *getSurfaceTargetName(int surface, int target) const;
	int findSurfaceTarget(const char *name, int surface) const;
	bool getInstancesFromSphere(const Math::WorldBoundSphere &bb, Vector<int> &instances);
};
typedef Ptr<ObjectMeshCluster> ObjectMeshClusterPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectMeshClutter : public Object
{
public:
	static int type() { return Node::OBJECT_MESH_CLUTTER; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	using Object::getCollision;
	using Object::setCollision;
	using Object::getIntersection;
	using Object::setIntersection;
	static Ptr<ObjectMeshClutter> create(const char *arg1, bool unique = false);
	bool createMesh(const char *name, bool unique = false);
	bool loadMesh(const char *name);
	bool saveMesh(const char *name);
	int setMesh(const Ptr<Mesh> &mesh);
	int getMesh(Ptr<Mesh> &mesh) const;
	void flushMesh();
	void setMeshName(const char *name);
	const char *getMeshName() const;
	void setMeshNameForce(const char *name);
	void setCollision(bool collision);
	bool getCollision() const;
	void setOrientation(bool orientation);
	bool getOrientation() const;
	void setIntersection(bool intersection);
	bool getIntersection() const;
	void setVisibleDistance(float distance);
	float getVisibleDistance() const;
	void setFadeDistance(float distance);
	float getFadeDistance() const;
	int getSpawnCount() const;
	void setSeed(int seed);
	int getSeed() const;
	void setSizeX(float sizex);
	float getSizeX() const;
	void setSizeY(float sizey);
	float getSizeY() const;
	void setStep(float step);
	float getStep() const;
	void setDensity(float density);
	float getDensity() const;
	void setThreshold(float threshold);
	float getThreshold() const;
	void setAngle(float angle);
	float getAngle() const;
	void setMinScale(float mean, float spread);
	float getMinScaleMean() const;
	float getMinScaleSpread() const;
	void setMaxScale(float mean, float spread);
	float getMaxScaleMean() const;
	float getMaxScaleSpread() const;
	void setOffset(float mean, float spread);
	float getOffsetMean() const;
	float getOffsetSpread() const;
	void setMeshesRotation(const Math::vec3 &mean, const Math::vec3 &spread);
	Math::vec3 getMeshesRotationMean() const;
	Math::vec3 getMeshesRotationSpread() const;
	int setMaskImage(const Ptr<Image> &image, bool invalidate = true);
	int getMaskImage(const Ptr<Image> &image) const;
	void setMaskImageName(const char *image_name, bool invalidate);
	void setMaskImageName(const char *name);
	const char *getMaskImageName() const;
	void setMaskFlipX(int value);
	int getMaskFlipX() const;
	void setMaskFlipY(int value);
	int getMaskFlipY() const;
	void setMaskMinValue(int value);
	int getMaskMinValue() const;
	void setMaskMaxValue(int value);
	int getMaskMaxValue() const;
	int setMaskMesh(const Ptr<Mesh> &mesh, bool invalidate = true);
	int getMaskMesh(const Ptr<Mesh> &mesh) const;
	void setMaskMeshName(const char *mesh_name, bool invalidate);
	void setMaskMeshName(const char *name);
	const char *getMaskMeshName() const;
	void setTerrainMask(int mask);
	int getTerrainMask() const;
	void setMaskInverse(int inverse);
	int getMaskInverse() const;
	void setCutoutIntersectionMask(int mask);
	int getCutoutIntersectionMask() const;
	void setCutoutInverse(int inverse);
	int getCutoutInverse() const;
	int getNumSurfaceTargets(int surface) const;
	const char *getSurfaceTargetName(int surface, int target) const;
	int findSurfaceTarget(const char *name, int surface) const;
	void invalidate();
	void invalidate(const Math::WorldBoundBox &bounds);
	void createClutterTransforms();
	bool getClutterTransforms(Vector<Math::mat4> &transforms) const;
	bool getClutterWorldTransforms(Vector<Math::Mat4> &transforms) const;
	bool getClutterTransforms(const Math::WorldBoundBox &bounds, Vector<Math::mat4> &transforms, bool precise = true, bool force = true) const;
	bool getClutterWorldTransforms(const Math::WorldBoundBox &bounds, Vector<Math::Mat4> &transforms, bool precise = true, bool force = true) const;
	int getClutterLocalTransforms(const Math::BoundBox &bounds, Vector<Math::mat4> &transforms, int precise = 1, int force = 1) const;
	void setClutterExclude(const Math::WorldBoundBox &bounds, int exclude);
	void clearClutterExcludes();
};
typedef Ptr<ObjectMeshClutter> ObjectMeshClutterPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectMeshSplineCluster : public Object
{
public:
	static int type() { return Node::OBJECT_MESH_SPLINE_CLUSTER; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	const char *getMeshName() const;
	int getNumMeshes() const;
	Math::mat4 getMeshTransform(int num) const;
	bool getClusterTransforms(const Math::WorldBoundBox &bounds, Vector<Math::mat4> &transforms);
	bool getClusterWorldTransforms(const Math::WorldBoundBox &bounds, Vector<Math::Mat4> &transforms);
	int getNumSurfaceTargets(int surface) const;
	const char *getSurfaceTargetName(int surface, int target) const;
	int findSurfaceTarget(const char *name, int surface) const;
};
typedef Ptr<ObjectMeshSplineCluster> ObjectMeshSplineClusterPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectSky : public Object
{
public:
	static int type() { return Node::OBJECT_SKY; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectSky> create();
	void setSpherical(bool spherical);
	bool isSpherical() const;
};
typedef Ptr<ObjectSky> ObjectSkyPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectGrass : public Object
{
public:
	static int type() { return Node::OBJECT_GRASS; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	using Object::getIntersection;
	using Object::setIntersection;
	static Ptr<ObjectGrass> create();
	void setFieldMask(int mask);
	int getFieldMask() const;
	void setThinning(int thinning);
	int getThinning() const;
	void setVariation(int variation);
	int getVariation() const;
	void setOrientation(int orientation);
	int getOrientation() const;
	void setIntersection(int intersection);
	int getIntersection() const;
	void setNumTextures(int textures);
	int getNumTextures() const;
	int getSpawnCount() const;
	void setSeed(int seed);
	int getSeed() const;
	void setSizeX(float sizex);
	float getSizeX() const;
	void setSizeY(float sizey);
	float getSizeY() const;
	void setStep(float step);
	float getStep() const;
	void setSubdivision(int subdivision);
	int getSubdivision() const;
	void setDensity(float density);
	float getDensity() const;
	void setThreshold(float threshold);
	float getThreshold() const;
	void setAngle(float angle);
	float getAngle() const;
	void setProbability(const Math::vec4 &probability);
	Math::vec4 getProbability() const;
	void setMinBend(const Math::vec4 &mean, const Math::vec4 &spread);
	Math::vec4 getMinBendMean() const;
	Math::vec4 getMinBendSpread() const;
	void setMaxBend(const Math::vec4 &mean, const Math::vec4 &spread);
	Math::vec4 getMaxBendMean() const;
	Math::vec4 getMaxBendSpread() const;
	void setMinHeight(const Math::vec4 &mean, const Math::vec4 &spread);
	Math::vec4 getMinHeightMean() const;
	Math::vec4 getMinHeightSpread() const;
	void setMaxHeight(const Math::vec4 &mean, const Math::vec4 &spread);
	Math::vec4 getMaxHeightMean() const;
	Math::vec4 getMaxHeightSpread() const;
	void setAspect(const Math::vec4 &mean, const Math::vec4 &spread);
	Math::vec4 getAspectMean() const;
	Math::vec4 getAspectSpread() const;
	void setOffset(const Math::vec4 &mean, const Math::vec4 &spread);
	Math::vec4 getOffsetMean() const;
	Math::vec4 getOffsetSpread() const;
	void setRotation(const Math::vec4 &mean, const Math::vec4 &spread);
	Math::vec4 getRotationMean() const;
	Math::vec4 getRotationSpread() const;
	int setMaskImage(const Ptr<Image> &image, int invalidate = 1);
	int getMaskImage(const Ptr<Image> &image) const;
	void setMaskImageName(const char *image_name, int invalidate = 1);
	const char *getMaskImageName() const;
	void setMaskFlipX(int maskflipx);
	int getMaskFlipX() const;
	void setMaskFlipY(int maskflipy);
	int getMaskFlipY() const;
	void setMaskMinValue(int value);
	int getMaskMinValue() const;
	void setMaskMaxValue(int value);
	int getMaskMaxValue() const;
	int setMaskMesh(const Ptr<Mesh> &mesh, int invalidate = 1);
	int getMaskMesh(const Ptr<Mesh> &mesh) const;
	void setMaskMeshName(const char *mesh_name, int invalidate = 1);
	const char *getMaskMeshName() const;
	void setTerrainMasks(const Math::ivec4 &masks);
	Math::ivec4 getTerrainMasks() const;
	void setMaskInverse(int inverse);
	int getMaskInverse() const;
	void setCutoutIntersectionMask(int mask);
	int getCutoutIntersectionMask() const;
	void setCutoutInverse(int inverse);
	int getCutoutInverse() const;
	void invalidate();
	void invalidate(const Math::WorldBoundBox &bounds);
};
typedef Ptr<ObjectGrass> ObjectGrassPtr;

class TerrainDetail;
class TerrainDetailMask;
class ObjectLandscapeTerrain;

class UNIGINE_API TerrainDetail : public APIInterface
{
public:
	void setName(const char *name);
	const char *getName() const;
	void setEnabled(bool enabled);
	bool isEnabled() const;
	bool isActive() const;
	void setMinVisibleHeight(float height);
	float getMinVisibleHeight() const;
	void setMaxVisibleHeight(float height);
	float getMaxVisibleHeight() const;
	void setMinFadeHeight(float height);
	float getMinFadeHeight() const;
	void setMaxFadeHeight(float height);
	float getMaxFadeHeight() const;
	void setMinVisibleTexelSize(float size);
	float getMinVisibleTexelSize() const;
	void setMaxVisibleTexelSize(float size);
	float getMaxVisibleTexelSize() const;
	void setMinFadeTexelSize(float size);
	float getMinFadeTexelSize() const;
	void setMaxFadeTexelSize(float size);
	float getMaxFadeTexelSize() const;
	void setMaskByAlbedo(const Math::vec4 &albedo);
	Math::vec4 getMaskByAlbedo() const;
	void setMaskThreshold(float threshold);
	float getMaskThreshold() const;
	void setMaskContrast(float contrast);
	float getMaskContrast() const;
	void setDetailMask(const Ptr<TerrainDetailMask> &mask);
	Ptr<TerrainDetailMask> getDetailMask() const;
	void setMaterialPath(const char *path);
	const char *getMaterialPath() const;
	void setMaterialGUID(const UGUID &materialguid);
	UGUID getMaterialGUID() const;
	void setMaterial(const Ptr<Material> &material);
	Ptr<Material> getMaterial() const;
	Ptr<TerrainDetail> copy(const Ptr<TerrainDetail> &dest_detail) const;
	Ptr<TerrainDetail> clone() const;
};
typedef Ptr<TerrainDetail> TerrainDetailPtr;


class UNIGINE_API TerrainDetailMask : public APIInterface
{
public:
	void setName(const char *name);
	const char *getName() const;
	void setEnabled(bool enabled);
	bool isEnabled() const;
	void setDithering(float dithering);
	float getDithering() const;
	void setDefaultValue(float value);
	float getDefaultValue() const;
	void setMaskByAlbedo(const Math::vec4 &albedo);
	Math::vec4 getMaskByAlbedo() const;
	int getRenderOrder() const;
	void swapRenderOrder(const Ptr<TerrainDetailMask> &mask);
	Ptr<TerrainDetail> addDetail();
	int getNumDetails() const;
	Ptr<TerrainDetail> getDetail(int num) const;
	bool isDetail(const Ptr<TerrainDetail> &detail) const;
	int findDetailIndex(const char *name) const;
	Ptr<TerrainDetail> findDetail(const char *name) const;
	void setDetailIndex(const Ptr<TerrainDetail> &detail, int index);
	int getDetailIndex(const Ptr<TerrainDetail> &detail) const;
	void swapDetail(int num_0, int num_1);
	int getIndex() const;
};
typedef Ptr<TerrainDetailMask> TerrainDetailMaskPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectLandscapeTerrain : public Object
{
public:
	static int type() { return Node::OBJECT_LANDSCAPE_TERRAIN; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectLandscapeTerrain> create();
	void setActiveTerrain(bool terrain);
	bool isActiveTerrain() const;
	void setIntersectionPrecision(float precision);
	float getIntersectionPrecision() const;
	int getNumDetailMasks() const;
	Ptr<TerrainDetailMask> getDetailMask(int num);
	Ptr<TerrainDetailMask> getDetailMaskSortRender(int num);
	Ptr<TerrainDetailMask> findDetailMask(const char *name);
	long long getLastStreamingFrame() const;
	void getDetailMasks(Vector<Ptr<TerrainDetailMask>> &masks);
	void getDetailMasksSortRender(Vector<Ptr<TerrainDetailMask>> &masks);
};
typedef Ptr<ObjectLandscapeTerrain> ObjectLandscapeTerrainPtr;


class UNIGINE_API LandscapeImages : public APIInterface
{
public:
	static Ptr<LandscapeImages> create(const Math::ivec2 &resolution);
	Ptr<Image> getHeight();
	Ptr<Image> getAlbedo();
	Ptr<Image> getMask(int num);
	Ptr<Image> getOpacityHeight();
	Ptr<Image> getOpacityMask(int num);
	Ptr<Image> get(int type);
	Math::ivec2 getResolution() const;
	void resize(const Math::ivec2 &new_resolution);
	Ptr<LandscapeImages> copy(const Ptr<LandscapeImages> &image) const;
	Ptr<LandscapeImages> clone();
};
typedef Ptr<LandscapeImages> LandscapeImagesPtr;


class UNIGINE_API LandscapeTextures : public APIInterface
{
public:
	static Ptr<LandscapeTextures> create(const Math::ivec2 &resolution);
	Ptr<Texture> getHeight() const;
	Ptr<Texture> getAlbedo() const;
	Ptr<Texture> getMask(int num);
	Ptr<Texture> getOpacityHeight() const;
	Ptr<Texture> getOpacityMask(int num);
	Ptr<Texture> get(int type);
	Ptr<RenderTarget> getRenderTarget() const;
	Math::ivec2 getResolution() const;
	int getNumMipmaps() const;
	void createMipmaps();
	void clearBuffers();
};
typedef Ptr<LandscapeTextures> LandscapeTexturesPtr;

class LandscapeLayerMap;
//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Landscape
{
protected:
	

public:
	static int isInitialized(); 

	enum TYPE_DATA
	{
		TYPE_DATA_HEIGHT = 0,
		TYPE_DATA_ALBEDO,
		TYPE_DATA_MASK_0,
		TYPE_DATA_MASK_1,
		TYPE_DATA_MASK_2,
		TYPE_DATA_MASK_3,
		TYPE_DATA_MASK_4,
		TYPE_DATA_MASK_5,
		TYPE_DATA_MASK_6,
		TYPE_DATA_MASK_7,
		TYPE_DATA_MASK_8,
		TYPE_DATA_MASK_9,
		TYPE_DATA_MASK_10,
		TYPE_DATA_MASK_11,
		TYPE_DATA_MASK_12,
		TYPE_DATA_MASK_13,
		TYPE_DATA_MASK_14,
		TYPE_DATA_MASK_15,
		TYPE_DATA_MASK_16,
		TYPE_DATA_MASK_17,
		TYPE_DATA_MASK_18,
		TYPE_DATA_MASK_19,
	};

	enum FLAGS_DATA
	{
		FLAGS_DATA_HEIGHT = 1 << 0,
		FLAGS_DATA_ALBEDO = 1 << 1,
		FLAGS_DATA_MASK_0 = 1 << 2,
		FLAGS_DATA_MASK_1 = 1 << 3,
		FLAGS_DATA_MASK_2 = 1 << 4,
		FLAGS_DATA_MASK_3 = 1 << 5,
		FLAGS_DATA_MASK_4 = 1 << 6,
		FLAGS_DATA_MASK_5 = 1 << 7,
		FLAGS_DATA_MASK_6 = 1 << 8,
		FLAGS_DATA_MASK_7 = 1 << 9,
		FLAGS_DATA_MASK_8 = 1 << 10,
		FLAGS_DATA_MASK_9 = 1 << 11,
		FLAGS_DATA_MASK_10 = 1 << 12,
		FLAGS_DATA_MASK_11 = 1 << 13,
		FLAGS_DATA_MASK_12 = 1 << 14,
		FLAGS_DATA_MASK_13 = 1 << 15,
		FLAGS_DATA_MASK_14 = 1 << 16,
		FLAGS_DATA_MASK_15 = 1 << 17,
		FLAGS_DATA_MASK_16 = 1 << 18,
		FLAGS_DATA_MASK_17 = 1 << 19,
		FLAGS_DATA_MASK_18 = 1 << 20,
		FLAGS_DATA_MASK_19 = 1 << 21,
	};

	enum TYPE_FILE_DATA
	{
		TYPE_FILE_DATA_HEIGHT,
		TYPE_FILE_DATA_ALBEDO,
		TYPE_FILE_DATA_MASK_0,
		TYPE_FILE_DATA_MASK_1,
		TYPE_FILE_DATA_MASK_2,
		TYPE_FILE_DATA_MASK_3,
		TYPE_FILE_DATA_MASK_4,
		TYPE_FILE_DATA_OPACITY_HEIGHT,
		TYPE_FILE_DATA_OPACITY_MASK_0,
		TYPE_FILE_DATA_OPACITY_MASK_1,
		TYPE_FILE_DATA_OPACITY_MASK_2,
		TYPE_FILE_DATA_OPACITY_MASK_3,
		TYPE_FILE_DATA_OPACITY_MASK_4,
	};

	enum FLAGS_FILE_DATA
	{
		FLAGS_FILE_DATA_HEIGHT = 1 << 0,
		FLAGS_FILE_DATA_ALBEDO = 1 << 1,
		FLAGS_FILE_DATA_MASK_0 = 1 << 2,
		FLAGS_FILE_DATA_MASK_1 = 1 << 3,
		FLAGS_FILE_DATA_MASK_2 = 1 << 4,
		FLAGS_FILE_DATA_MASK_3 = 1 << 5,
		FLAGS_FILE_DATA_MASK_4 = 1 << 6,
		FLAGS_FILE_DATA_OPACITY_HEIGHT = 1 << 7,
		FLAGS_FILE_DATA_OPACITY_MASK_0 = 1 << 8,
		FLAGS_FILE_DATA_OPACITY_MASK_1 = 1 << 9,
		FLAGS_FILE_DATA_OPACITY_MASK_2 = 1 << 10,
		FLAGS_FILE_DATA_OPACITY_MASK_3 = 1 << 11,
		FLAGS_FILE_DATA_OPACITY_MASK_4 = 1 << 12,
	};

	enum COMPRESSOR_TYPE
	{
		COMPRESSOR_TYPE_NONE = 0,
		COMPRESSOR_TYPE_JACKALLESS = 1,
		COMPRESSOR_TYPE_LZ4 = 2,
		COMPRESSOR_TYPE_ZLIB = 3,
	};

	enum BLENDING_MODE
	{
		ALPHA_BLEND = 0,
		ADDITIVE = 1,
		OVERLAY = 2,
		MULTIPLICATIVE = 3,
	};
	static Ptr<LandscapeTextures> getTemporaryTexture(const Math::ivec2 &resolution);
	static void releaseTemporaryTexture(const Ptr<LandscapeTextures> &texture);
	static bool terrainLoad(const Math::WorldBoundBox &bb);
	static bool render(const Ptr<LandscapeTextures> &buffers, const Math::Mat4 &transform, Math::Scalar texel_size, int padding);
	static bool render(const Ptr<LandscapeTextures> &buffers, const Math::Mat4 &transform, Math::Scalar texel_size);
	static bool render(const Vector<Ptr<LandscapeLayerMap>> &maps, const Ptr<LandscapeTextures> &buffers, const Math::Mat4 &transform, Math::Scalar texel_size, int padding);
	static bool render(const Vector<Ptr<LandscapeLayerMap>> &maps, const Ptr<LandscapeTextures> &buffers, const Math::Mat4 &transform, Math::Scalar texel_size);
	static int generateOperationID();
	static void asyncTextureDraw(int operation_id, const UGUID &file_guid, const Math::ivec2 &coord, const Math::ivec2 &resolution, int flags_file_data, const Vector<Math::WorldBoundBox> &bounds_preload);
	static void asyncTextureDraw(int operation_id, const UGUID &file_guid, const Math::ivec2 &coord, const Math::ivec2 &resolution, int flags_file_data);
	static void asyncTextureDraw(int operation_id, const UGUID &file_guid, const Math::ivec2 &coord, const Math::ivec2 &resolution);
	static void asyncTextureDraw(const UGUID &file_guid, const Math::ivec2 &coord, const Math::ivec2 &resolution, int flags_file_data, const Vector<Math::WorldBoundBox> &bounds_preload);
	static void asyncTextureDraw(const UGUID &file_guid, const Math::ivec2 &coord, const Math::ivec2 &resolution, int flags_file_data);
	static void asyncTextureDraw(const UGUID &file_guid, const Math::ivec2 &coord, const Math::ivec2 &resolution);
	static void *addTextureDrawCallback(CallbackBase5<UGUID, int, Ptr<LandscapeTextures>, Math::ivec2, int> *func);
	static bool removeTextureDrawCallback(void *id);
	static void clearTextureDrawCallbacks();
	static void asyncApplyDiff(int operation_id, const UGUID &file_guid, const char *path);
	static void asyncApplyDiff(const UGUID &file_guid, const char *path);
	static void *addApplyDiffCallback(CallbackBase3<UGUID, int, const char *> *func);
	static bool removeApplyDiffCallback(void *id);
	static void clearApplyDiffCallbacks();
	static void asyncSaveFile(int operation_id, const UGUID &file_guid, const char *path_new_diff, const char *path_old_diff);
	static void asyncSaveFile(int operation_id, const UGUID &file_guid);
	static void asyncSaveFile(const UGUID &file_guid, const char *path_new_diff, const char *path_old_diff);
	static void asyncSaveFile(const UGUID &file_guid);
	static void *addSaveFileCallback(CallbackBase4<UGUID, int, const char *, const char *> *func);
	static bool removeSaveFileCallback(void *id);
	static void clearSaveFileCallbacks();
	static bool isFilesClosed();
	static void filesClose(const Vector<UGUID> &reload_files);
	static void filesClose();
	static void filesOpen();
	static Ptr<ObjectLandscapeTerrain> getActiveTerrain();
};

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API LandscapeLayerMap : public Node
{
public:
	static int type() { return Node::LANDSCAPE_LAYER_MAP; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<LandscapeLayerMap> create();
	void setPath(const char *path);
	const char *getPath() const;
	UGUID getGUID();
	void setIntersectionBicubicFilter(bool filter);
	bool isIntersectionBicubicFilter() const;
	void setIntersection(bool intersection);
	bool isIntersection() const;
	void setCollision(bool collision);
	bool isCollision() const;
	void setCulling(bool culling);
	bool isCulling() const;
	void setOrder(int order);
	int getOrder() const;
	void setSize(const Math::Vec2 &size);
	Math::Vec2 getSize() const;
	void setFadeAttenuation(const Math::vec2 &attenuation);
	Math::vec2 getFadeAttenuation() const;
	void setHeightScale(float scale);
	float getHeightScale() const;
	Math::Vec2 getTexelSize() const;
	Math::ivec2 getResolution() const;
	bool isInit() const;
	Math::Vec2 getExtremumHeight(float precision = 1.0f);
	bool isEnabledHeight() const;
	bool isEnabledAlbedo() const;
	bool isEnabledMask(int mask) const;
	bool isEnabledOpacityHeight() const;
	bool isEnabledOpacityAlbedo() const;
	int isEnabledOpacityMask(int mask) const;
	Landscape::BLENDING_MODE getHeightBlending() const;
	Landscape::BLENDING_MODE getAlbedoBlending() const;
	Landscape::BLENDING_MODE getMaskBlending(int mask) const;
	Math::vec2 getHeightFadeAttenuation() const;
	Math::vec2 getAlbedoFadeAttenuation() const;
	Math::vec2 getMaskFadeAttenuation(int mask) const;
	bool isCompressed() const;
};
typedef Ptr<LandscapeLayerMap> LandscapeLayerMapPtr;


class UNIGINE_API LandscapeMapFileSettings : public APIInterface
{
public:
	static Ptr<LandscapeMapFileSettings> create();
	void setHeightBlending(Landscape::BLENDING_MODE blending);
	Landscape::BLENDING_MODE getHeightBlending() const;
	void setAlbedoBlending(Landscape::BLENDING_MODE blending);
	Landscape::BLENDING_MODE getAlbedoBlending() const;
	void setHeightFadeAttenuation(const Math::vec2 &attenuation);
	Math::vec2 getHeightFadeAttenuation() const;
	void setAlbedoFadeAttenuation(const Math::vec2 &attenuation);
	Math::vec2 getAlbedoFadeAttenuation() const;
	void setMaskBlending(int mask, Landscape::BLENDING_MODE blend);
	Landscape::BLENDING_MODE getMaskBlending(int mask) const;
	void setMaskFadeAttenuation(int mask, const Math::vec2 &fade_attenuation);
	Math::vec2 getMaskFadeAttenuation(int mask) const;
	void setEnabledHeight(bool height);
	bool isEnabledHeight() const;
	void setEnabledAlbedo(bool albedo);
	bool isEnabledAlbedo() const;
	void setEnabledMask(int mask, bool enabled);
	bool isEnabledMask(int mask) const;
	void setEnabledOpacityHeight(bool height);
	bool isEnabledOpacityHeight() const;
	void setEnabledOpacityAlbedo(bool albedo);
	bool isEnabledOpacityAlbedo() const;
	void setEnabledOpacityMask(int mask, bool enabled);
	bool isEnabledOpacityMask(int mask) const;
	Landscape::COMPRESSOR_TYPE getHeightCompressor() const;
	Landscape::COMPRESSOR_TYPE getAlbedoCompressor() const;
	Landscape::COMPRESSOR_TYPE getOpacityHeightCompressor() const;
	bool isCompressed() const;
	Landscape::COMPRESSOR_TYPE getMaskCompressor(int mask);
	Landscape::COMPRESSOR_TYPE getMaskOpacityCompressor(int mask);
	bool isEnabledAlbedoTextureCompression() const;
	bool isEnabledOpacityHeightTextureCompression() const;
	bool isEnabledMaskTextureCompression(int mask);
	bool isEnabledMaskOpacityTextureCompression(int mask);
	Math::ivec2 getTilesSize() const;
	Math::ivec2 getResolution() const;
	UGUID getGUID() const;
	bool isLoaded() const;
	bool load(const UGUID &guid);
	bool apply();
};
typedef Ptr<LandscapeMapFileSettings> LandscapeMapFileSettingsPtr;


class UNIGINE_API LandscapeMapFileCompression : public APIInterface
{
public:
	static Ptr<LandscapeMapFileCompression> create();
	bool isCompressing() const;
	bool isDecompressing() const;
	int getProgress() const;
	bool compress(bool is_safe);
	bool decompress(bool is_safe);
	void stop();
	void setHeightCompressor(Landscape::COMPRESSOR_TYPE compressor);
	Landscape::COMPRESSOR_TYPE getHeightCompressor() const;
	void setAlbedoCompressor(Landscape::COMPRESSOR_TYPE compressor);
	Landscape::COMPRESSOR_TYPE getAlbedoCompressor() const;
	void setOpacityHeightCompressor(Landscape::COMPRESSOR_TYPE compressor);
	Landscape::COMPRESSOR_TYPE getOpacityHeightCompressor() const;
	void setMaskCompressor(int mask, Landscape::COMPRESSOR_TYPE compressor_type);
	void setMaskOpacityCompressor(int mask, Landscape::COMPRESSOR_TYPE compressor_type);
	int getMaskCompressor(int mask);
	int getMaskOpacityCompressor(int mask);
	void setEnabledAlbedoTextureCompression(bool compression);
	bool isEnabledAlbedoTextureCompression() const;
	void setEnabledOpacityHeightTextureCompression(bool compression);
	bool isEnabledOpacityHeightTextureCompression() const;
	void setEnabledMaskTextureCompression(int mask, bool enable);
	void setEnabledMaskOpacityTextureCompression(int mask, bool enable);
	bool isEnabledMaskTextureCompression(int mask);
	bool isEnabledMaskOpacityTextureCompression(int mask);
	void setCompressorAll(Landscape::COMPRESSOR_TYPE compressor_type);
	void setCacheDirectory(const char *directory);
	const char *getCacheDirectory() const;
	void setGUID(const UGUID &guid);
	UGUID getGUID() const;
	void *addBeginCallback(CallbackBase1<Ptr<LandscapeMapFileCompression>> *func);
	bool removeBeginCallback(void *id);
	void clearBeginCallbacks();
	void *addProgressCallback(CallbackBase1<Ptr<LandscapeMapFileCompression>> *func);
	bool removeProgressCallback(void *id);
	void clearProgressCallbacks();
	void *addEndCallback(CallbackBase1<Ptr<LandscapeMapFileCompression>> *func);
	bool removeEndCallback(void *id);
	void clearEndCallbacks();
};
typedef Ptr<LandscapeMapFileCompression> LandscapeMapFileCompressionPtr;


class UNIGINE_API LandscapeFetch : public APIInterface
{
public:
	static Ptr<LandscapeFetch> create();
	Math::Vec3 getPosition() const;
	float getHeight() const;
	bool isIntersection() const;
	Math::vec3 getNormal() const;
	Math::vec4 getAlbedo() const;
	float getMask(int num) const;
	void setUses(int uses);
	int getUses() const;
	void setUsesHeight(bool height);
	bool isUsesHeight() const;
	void setUsesNormal(bool normal);
	bool isUsesNormal() const;
	void setUsesAlbedo(bool albedo);
	bool isUsesAlbedo() const;
	bool isUsesMask(int num) const;
	void setUsesMask(int num, bool value);
	void setHolesEnabled(bool enabled);
	bool isHolesEnabled() const;
	void setIntersectionPrecision(float precision);
	float getIntersectionPrecision() const;
	void setIntersectionPositionBegin(const Math::Vec3 &begin);
	Math::Vec3 getIntersectionPositionBegin() const;
	void setIntersectionPositionEnd(const Math::Vec3 &end);
	Math::Vec3 getIntersectionPositionEnd() const;
	void setFetchPosition(const Math::Vec2 &position);
	Math::Vec2 getFetchPosition() const;
	bool fetchForce(const Math::Vec2 &position);
	bool fetchForce();
	bool intersectionForce(const Math::Vec3 &p0, const Math::Vec3 &p1);
	bool intersectionForce();
	void fetchAsync(const Math::Vec2 &position, bool critical = false);
	void fetchAsync(bool critical = false);
	void intersectionAsync(const Math::Vec3 &p0, const Math::Vec3 &p1, bool critical = false);
	void intersectionAsync(bool critical = false);
	static void fetchForce(const Vector<Ptr<LandscapeFetch>> &fetches);
	static void intersectionForce(const Vector<Ptr<LandscapeFetch>> &fetches);
	static void fetchAsync(const Vector<Ptr<LandscapeFetch>> &fetches, bool critical = false);
	static void intersectionAsync(const Vector<Ptr<LandscapeFetch>> &fetches, bool critical = false);
	static void wait(const Vector<Ptr<LandscapeFetch>> &fetches);
	void wait();
	bool isAsyncCompleted() const;
	void *addStartCallback(CallbackBase *func);
	bool removeStartCallback(void *id);
	void clearStartCallbacks();
	void *addEndCallback(CallbackBase *func);
	bool removeEndCallback(void *id);
	void clearEndCallbacks();
};
typedef Ptr<LandscapeFetch> LandscapeFetchPtr;


class UNIGINE_API LandscapeMapFileCreator : public APIInterface
{
public:
	static Ptr<LandscapeMapFileCreator> create();
	void setResolution(const Math::ivec2 &resolution);
	Math::ivec2 getResolution() const;
	void setGrid(const Math::ivec2 &grid);
	Math::ivec2 getGrid() const;
	void setDownscaleFilter(Landscape::TYPE_FILE_DATA file_data_type, Image::FILTER filter);
	Image::FILTER getDownscaleFilter(Landscape::TYPE_FILE_DATA file_data_type) const;
	float getProgress() const;
	double getTimeSeconds() const;
	void setPath(const char *path);
	const char *getPath() const;
	void *addCreateCallback(CallbackBase4<Ptr<LandscapeMapFileCreator>, Ptr<LandscapeImages>, int, int> *func);
	bool removeCreateCallback(void *id);
	void clearCreateCallbacks();
	void *addProgressCallback(CallbackBase1<Ptr<LandscapeMapFileCreator>> *func);
	bool removeProgressCallback(void *id);
	void clearProgressCallbacks();
	void *addBeginCallback(CallbackBase1<Ptr<LandscapeMapFileCreator>> *func);
	bool removeBeginCallback(void *id);
	void clearBeginCallbacks();
	void *addEndCallback(CallbackBase1<Ptr<LandscapeMapFileCreator>> *func);
	bool removeEndCallback(void *id);
	void clearEndCallbacks();
	bool run(bool is_empty = false, bool is_safe = true);
};
typedef Ptr<LandscapeMapFileCreator> LandscapeMapFileCreatorPtr;


class UNIGINE_API TerrainGlobalLod : public APIInterface
{
public:
	static bool convertible(TerrainGlobalLod *obj) { return obj != nullptr; }

	enum TYPE
	{
		TERRAIN_GLOBAL_LOD = 0,
		TERRAIN_GLOBAL_LOD_HEIGHT,
		NUM_TERRAIN_GLOBAL_LODS,
	};
	TerrainGlobalLod::TYPE getType() const;
	const char *getTypeName() const;
	void setEnabled(bool enabled);
	bool isEnabled() const;
	Ptr<Tileset> getTileset() const;
	void setPath(const char *path);
	const char *getPath() const;
	int renamePath(const char *new_path);
	void setTileDensity(float density);
	float getTileDensity() const;
	void setViewportMask(int mask);
	int getViewportMask() const;
	void setVisibleDistance(float distance);
	float getVisibleDistance() const;
	void setLoadDistance(float distance);
	float getLoadDistance() const;
	void setClearDistance(float distance);
	float getClearDistance() const;
	void reload();
};
typedef Ptr<TerrainGlobalLod> TerrainGlobalLodPtr;


class UNIGINE_API TerrainGlobalLodHeight : public TerrainGlobalLod
{
public:
	static bool convertible(TerrainGlobalLod *obj) { return obj && obj->getType() == TerrainGlobalLod::TERRAIN_GLOBAL_LOD_HEIGHT; }
	void setIntersection(bool intersection);
	bool isIntersection() const;
	void setCollision(bool collision);
	bool isCollision() const;
	void setCastShadows(bool shadows);
	bool isCastShadows() const;
	void setIntersectionMask(int mask);
	int getIntersectionMask() const;
	void setCollisionMask(int mask);
	int getCollisionMask() const;
};
typedef Ptr<TerrainGlobalLodHeight> TerrainGlobalLodHeightPtr;


class UNIGINE_API TerrainGlobalLods : public APIInterface
{
public:
	const char *getName() const;
	int getType() const;
	int getDataType() const;
	int getDataFormat() const;
	int getMaskFormat() const;
	void setNumLayers(int layers);
	int getNumLayers() const;
	int getNumLods() const;
	Ptr<TerrainGlobalLod> getLod(int num) const;
	int addLod();
	void insertLod(int num);
	void removeLod(int num);
	void clearLods();
	String getVideoMemoryUsage() const;
	int fetch(const Math::BoundSphere &bs, const Math::BoundBox &bb, int force = 0);
	int fetchData(double x, double y, Image::Pixel &ret_pixel, int layer, bool force);
};
typedef Ptr<TerrainGlobalLods> TerrainGlobalLodsPtr;


class UNIGINE_API TerrainGlobalDetail : public APIInterface
{
public:
	void setName(const char *name);
	const char *getName() const;
	void setEnabled(bool enabled);
	bool isEnabled() const;
	void setMaskNumber(int number);
	int getMaskNumber() const;
	void setMaskColor(const Math::vec4 &color);
	Math::vec4 getMaskColor() const;
	void setTransform(const Math::vec4 &transform);
	Math::vec4 getTransform() const;
	void setMinVisibleHeight(float height);
	float getMinVisibleHeight() const;
	void setMaxVisibleHeight(float height);
	float getMaxVisibleHeight() const;
	void setMinFadeHeight(float height);
	float getMinFadeHeight() const;
	void setMaxFadeHeight(float height);
	float getMaxFadeHeight() const;
	void setMinVisibleDistance(float distance);
	float getMinVisibleDistance() const;
	void setMaxVisibleDistance(float distance);
	float getMaxVisibleDistance() const;
	void setMinFadeDistance(float distance);
	float getMinFadeDistance() const;
	void setMaxFadeDistance(float distance);
	float getMaxFadeDistance() const;
	void setOverlap(bool overlap);
	bool isOverlap() const;
	void setDetail(bool detail);
	bool isDetail() const;
	void setTriplanar(bool triplanar);
	bool isTriplanar() const;
	void setBlendTriplanar(float triplanar);
	float getBlendTriplanar() const;
	void setMaskThreshold(float threshold);
	float getMaskThreshold() const;
	void setMaskWidth(float width);
	float getMaskWidth() const;
	void setMaskContrast(float contrast);
	float getMaskContrast() const;
	void setAlbedoTextureName(const char *name);
	const char *getAlbedoTextureName() const;
	void setRoughnessTextureName(const char *name);
	const char *getRoughnessTextureName() const;
	void setNormalTextureName(const char *name);
	const char *getNormalTextureName() const;
	void setHeightTextureName(const char *name);
	const char *getHeightTextureName() const;
	void setAlbedoColor(const Math::vec4 &color);
	Math::vec4 getAlbedoColor() const;
	void setRoughness(float roughness);
	float getRoughness() const;
	void setAlbedoScale(float scale);
	float getAlbedoScale() const;
	void setRoughnessScale(float scale);
	float getRoughnessScale() const;
	void setNormalScale(float scale);
	float getNormalScale() const;
	void setDisplacement(float displacement);
	float getDisplacement() const;
	void setDisplacementOffset(float offset);
	float getDisplacementOffset() const;
};
typedef Ptr<TerrainGlobalDetail> TerrainGlobalDetailPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectTerrainGlobal : public Object
{
public:
	static int type() { return Node::OBJECT_TERRAIN_GLOBAL; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectTerrainGlobal> create();
	void clear();
	int fetchTopologyData(double x, double y, Math::Vec3 &ret_point, Math::vec3 &ret_normal, Math::vec3 &ret_up, int force = 0);
	Ptr<TerrainGlobalLods> getHeightLods();
	Ptr<TerrainGlobalLods> getAlbedoLods();
	Ptr<TerrainGlobalLods> getNormalLods();
	Ptr<TerrainGlobalLods> getMaskLods();
	int addMask(const char *name);
	void removeMask(int num);
	void resizeMasks(int size);
	int getNumMasks() const;
	void swapMask(int num_0, int num_1);
	int cloneMask(int num);
	void setMaskName(int num, const char *name);
	const char *getMaskName(int num) const;
	void replaceMasks(const Vector<String> &names);
	int getNumDetails() const;
	Ptr<TerrainGlobalDetail> getDetail(int num);
	int addDetail(const char *name);
	void removeDetail(int num);
	void resizeDetails(int size);
	void swapDetail(int num_0, int num_1);
	int cloneDetail(int num);
	void setAlbedoTextureArrayName(const char *name);
	const char *getAlbedoTextureArrayName() const;
	Ptr<Texture> getAlbedoTextureArray() const;
	void setNormalTextureArrayName(const char *name);
	const char *getNormalTextureArrayName() const;
	Ptr<Texture> getNormalTextureArray() const;
	void setHeightTextureArrayName(const char *name);
	const char *getHeightTextureArrayName() const;
	Ptr<Texture> getHeightTextureArray() const;
	void setForceIntersection(bool intersection);
	bool isForceIntersection() const;
};
typedef Ptr<ObjectTerrainGlobal> ObjectTerrainGlobalPtr;


class UNIGINE_API ParticleModifier : public APIInterface
{
public:
	static bool convertible(ParticleModifier *obj) { return obj != nullptr; }

	enum TYPE
	{
		PARTICLE_MODIFIER_SCALAR = 0,
		PARTICLE_MODIFIER_VECTOR,
	};

	enum MODE
	{
		MODE_CONSTANT = 0,
		MODE_RANDOM_BETWEEN_TWO_CONSTANTS,
		MODE_CURVE,
		MODE_RANDOM_BETWEEN_TWO_CURVES,
		NUM_MODES,
	};
	bool isSignedValues() const;
	bool saveState(const Ptr<Stream> &stream) const;
	bool restoreState(const Ptr<Stream> &stream);
	bool save(const Ptr<Xml> &xml) const;
	bool load(const Ptr<Xml> &xml);
	long long getSystemMemoryUsage() const;
	ParticleModifier::TYPE getType() const;
	void setMode(ParticleModifier::MODE mode);
	ParticleModifier::MODE getMode() const;
};
typedef Ptr<ParticleModifier> ParticleModifierPtr;


class UNIGINE_API ParticleModifierScalar : public ParticleModifier
{
public:
	static bool convertible(ParticleModifier *obj) { return obj && obj->getType() == ParticleModifier::PARTICLE_MODIFIER_SCALAR; }
	void setCurveScale(float scale);
	float getCurveScale() const;
	void setConstant(float constant);
	float getConstant() const;
	void setConstantMin(float val);
	float getConstantMin() const;
	void setConstantMax(float val);
	float getConstantMax() const;
	void setCurve(const Ptr<Curve2d> &curve);
	Ptr<Curve2d> getCurve() const;
	void setCurveMin(const Ptr<Curve2d> &val);
	Ptr<Curve2d> getCurveMin() const;
	void setCurveMax(const Ptr<Curve2d> &val);
	Ptr<Curve2d> getCurveMax() const;
	float getMaxValue() const;
	float getMinValue() const;
};
typedef Ptr<ParticleModifierScalar> ParticleModifierScalarPtr;


class UNIGINE_API ParticleModifierVector : public ParticleModifier
{
public:
	static bool convertible(ParticleModifier *obj) { return obj && obj->getType() == ParticleModifier::PARTICLE_MODIFIER_VECTOR; }
	void setConstant(const Math::vec3 &constant);
	Math::vec3 getConstant() const;
	void setConstantMin(const Math::vec3 &val);
	Math::vec3 getConstantMin() const;
	void setConstantMax(const Math::vec3 &val);
	Math::vec3 getConstantMax() const;
	void setCurveScale(float scale);
	float getCurveScale() const;
	void setCurveX(const Ptr<Curve2d> &curvex);
	Ptr<Curve2d> getCurveX() const;
	void setCurveY(const Ptr<Curve2d> &curvey);
	Ptr<Curve2d> getCurveY() const;
	void setCurveZ(const Ptr<Curve2d> &curvez);
	Ptr<Curve2d> getCurveZ() const;
	void setCurveXMin(const Ptr<Curve2d> &xmin);
	Ptr<Curve2d> getCurveXMin() const;
	void setCurveYMin(const Ptr<Curve2d> &ymin);
	Ptr<Curve2d> getCurveYMin() const;
	void setCurveZMin(const Ptr<Curve2d> &zmin);
	Ptr<Curve2d> getCurveZMin() const;
	void setCurveXMax(const Ptr<Curve2d> &xmax);
	Ptr<Curve2d> getCurveXMax() const;
	void setCurveYMax(const Ptr<Curve2d> &ymax);
	Ptr<Curve2d> getCurveYMax() const;
	void setCurveZMax(const Ptr<Curve2d> &zmax);
	Ptr<Curve2d> getCurveZMax() const;
	Math::vec3 getMaxValue() const;
	Math::vec3 getMinValue() const;
};
typedef Ptr<ParticleModifierVector> ParticleModifierVectorPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectParticles : public Object
{
public:
	static int type() { return Node::OBJECT_PARTICLES; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	using Object::getIntersection;
	using Object::setIntersection;
	using Object::getCollision;
	using Object::setCollision;
	static Ptr<ObjectParticles> create();
	void setFPSVisibleCamera(int camera);
	int getFPSVisibleCamera() const;
	void setFPSVisibleShadow(int shadow);
	int getFPSVisibleShadow() const;
	void setFPSInvisible(int invisible);
	int getFPSInvisible() const;
	void setUpdateDistanceLimit(float limit);
	float getUpdateDistanceLimit() const;
	void setSeed(unsigned int seed);
	unsigned int getSeed() const;
	void setParticlesType(int type);
	int getParticlesType() const;
	void setWarming(int warming);
	int getWarming() const;
	void setMaxWarmingTime(float time);
	float getMaxWarmingTime() const;
	void setDepthSort(int sort);
	int getDepthSort() const;
	void setVariationX(int variationx);
	int getVariationX() const;
	void setVariationY(int variationy);
	int getVariationY() const;
	void setTextureAtlas(int atlas);
	int getTextureAtlas() const;
	void setTextureAtlasSize(const Math::ivec2 &size);
	Math::ivec2 getTextureAtlasSize() const;
	void setPhysicsIntersection(int intersection);
	int getPhysicsIntersection() const;
	void setCollision(int collision);
	int getCollision() const;
	void setCulling(int culling);
	int getCulling() const;
	void setClearOnEnable(bool enable);
	bool isClearOnEnable() const;
	void setNumberPerSpawn(int spawn);
	int getNumberPerSpawn() const;
	void setSpawnRate(float rate);
	float getSpawnRate() const;
	void setSpawnScale(float scale);
	float getSpawnScale() const;
	void setSpawnThreshold(float threshold);
	float getSpawnThreshold() const;
	void clearParticles();
	int getNumParticles() const;
	float getParticleRadius(int num) const;
	Math::Vec3 getParticlePosition(int num) const;
	Math::vec3 getParticleVelocity(int num) const;
	void getParticleTransforms(Vector<Math::Mat4> &transforms) const;
	void setPhysicalMask(int mask);
	int getPhysicalMask() const;
	void setPhysicalMass(float mass);
	float getPhysicalMass() const;
	void setLinearDamping(float damping);
	float getLinearDamping() const;
	void setRestitution(float restitution);
	float getRestitution() const;
	void setRoughness(float roughness);
	float getRoughness() const;
	void setEmitterType(int type);
	int getEmitterType() const;
	void setProceduralRendering(bool rendering);
	bool isProceduralRendering() const;
	void setProceduralPositioning(int positioning);
	int getProceduralPositioning() const;
	void setProceduralParenting(int parenting);
	int getProceduralParenting() const;
	void setProceduralTextureResolution(const Math::vec3 &res);
	Math::vec3 getProceduralTextureResolution() const;
	void setEmitterEnabled(bool enabled);
	bool isEmitterEnabled() const;
	void setEmitterBased(bool based);
	bool isEmitterBased() const;
	void setEmitterShift(bool shift);
	bool isEmitterShift() const;
	void setEmitterContinuous(bool continuous);
	bool isEmitterContinuous() const;
	void setEmitterSequence(int sequence);
	int getEmitterSequence() const;
	void setEmitterLimitPerSpawn(int spawn);
	int getEmitterLimitPerSpawn() const;
	void setEmitterSync(int sync);
	int getEmitterSync() const;
	void setEmitterSize(const Math::vec3 &size);
	Math::vec3 getEmitterSize() const;
	void setEmitterVelocity(const Math::vec3 &velocity);
	Math::vec3 getEmitterVelocity() const;
	void addEmitterSpark(const Math::Vec3 &point, const Math::vec3 &normal, const Math::vec3 &velocity);
	void setDelay(float mean, float spread);
	float getDelayMean() const;
	float getDelaySpread() const;
	void setPeriod(float mean, float spread);
	float getPeriodMean() const;
	float getPeriodSpread() const;
	void setDuration(float mean, float spread);
	float getDurationMean() const;
	float getDurationSpread() const;
	void setLife(float mean, float spread);
	float getLifeMean() const;
	float getLifeSpread() const;
	Ptr<ParticleModifierScalar> getAngleOverTimeModifier() const;
	Ptr<ParticleModifierScalar> getRotationOverTimeModifier() const;
	Ptr<ParticleModifierScalar> getRadiusOverTimeModifier() const;
	Ptr<ParticleModifierScalar> getGrowthOverTimeModifier() const;
	Ptr<ParticleModifierScalar> getLengthStretchOverTimeModifier() const;
	Ptr<ParticleModifierScalar> getLengthFlatteningOverTimeModifier() const;
	Ptr<ParticleModifierScalar> getVelocityOverTimeModifier() const;
	Ptr<ParticleModifierVector> getDirectionOverTimeModifier() const;
	Ptr<ParticleModifierVector> getPositionOverTimeModifier() const;
	Ptr<ParticleModifierVector> getGravityOverTimeModifier() const;
	int addForce();
	void removeForce(int num);
	void setNumForces(int forces);
	int getNumForces() const;
	void setForceEnabled(int num, bool enabled);
	bool isForceEnabled(int num) const;
	void setForceAttached(int num, int attached);
	int isForceAttached(int num) const;
	void setForceTransform(int num, const Math::Mat4 &transform);
	Math::Mat4 getForceTransform(int num) const;
	void setForceRadius(int num, float radius);
	float getForceRadius(int num) const;
	void setForceAttenuation(int num, float attenuation);
	float getForceAttenuation(int num) const;
	void setForceAttractor(int num, float attractor);
	float getForceAttractor(int num) const;
	void setForceRotator(int num, float rotator);
	float getForceRotator(int num) const;
	int addNoise();
	void removeNoise(int num);
	void setNumNoises(int noises);
	int getNumNoises() const;
	void setNoiseEnabled(int num, bool enabled);
	bool isNoiseEnabled(int num) const;
	void setNoiseAttached(int num, int attached);
	int isNoiseAttached(int num) const;
	void setNoiseTransform(int num, const Math::Mat4 &transform);
	Math::Mat4 getNoiseTransform(int num) const;
	void setNoiseOffset(int num, const Math::vec3 &offset);
	Math::vec3 getNoiseOffset(int num) const;
	void setNoiseStep(int num, const Math::vec3 &step);
	Math::vec3 getNoiseStep(int num) const;
	void setNoiseForce(int num, float force);
	float getNoiseForce(int num) const;
	void setNoiseScale(int num, float scale);
	float getNoiseScale(int num) const;
	void setNoiseFrequency(int num, int frequency);
	int getNoiseFrequency(int num) const;
	void setNoiseSize(int num, int size);
	int getNoiseSize(int num) const;
	void setNoiseSeed(int num, int seed);
	int getNoiseSeed(int num) const;
	Ptr<Image> getNoiseImage(int num) const;
	int addDeflector();
	void removeDeflector(int num);
	void setNumDeflectors(int deflectors);
	int getNumDeflectors() const;
	void setDeflectorType(int num, int type);
	int getDeflectorType(int num) const;
	void setDeflectorEnabled(int num, bool enabled);
	bool isDeflectorEnabled(int num) const;
	void setDeflectorAttached(int num, int attached);
	int isDeflectorAttached(int num) const;
	void setDeflectorTransform(int num, const Math::Mat4 &transform);
	Math::Mat4 getDeflectorTransform(int num) const;
	void setDeflectorSize(int num, const Math::vec3 &size);
	Math::vec3 getDeflectorSize(int num) const;
	void setDeflectorRestitution(int num, float restitution);
	float getDeflectorRestitution(int num) const;
	void setDeflectorRoughness(int num, float roughness);
	float getDeflectorRoughness(int num) const;
	int getNumContacts() const;
	Math::Vec3 getContactPoint(int num) const;
	Math::vec3 getContactNormal(int num) const;
	Math::vec3 getContactVelocity(int num) const;
	Ptr<Object> getContactObject(int num) const;
	Math::Vec3 getWorldOffset() const;
	int saveStateSelf(const Ptr<Stream> &stream) const;
	int restoreStateSelf(const Ptr<Stream> &stream);
	int saveStateForces(const Ptr<Stream> &stream) const;
	int restoreStateForces(const Ptr<Stream> &stream);
	int saveStateNoises(const Ptr<Stream> &stream) const;
	int restoreStateNoises(const Ptr<Stream> &stream);
	int saveStateDeflectors(const Ptr<Stream> &stream) const;
	int restoreStateDeflectors(const Ptr<Stream> &stream);
	Math::BoundBox getBoundBoxParticles() const;
	Math::BoundBox getBoundBoxSimulation() const;
	Math::WorldBoundBox getWorldBoundBoxParticles() const;

	enum SYNC_MODE
	{
		SYNC_MODE_NONE = 0,
		SYNC_MODE_MASTER,
		SYNC_MODE_SLAVE,
	};
	void setSyncMode(ObjectParticles::SYNC_MODE mode);
	ObjectParticles::SYNC_MODE getSyncMode() const;
	void takeSyncData(const Ptr<Stream> &stream);
	void applySyncData(const Ptr<Stream> &stream);

	enum
	{
		TYPE_BILLBOARD = 0,
		TYPE_FLAT,
		TYPE_POINT,
		TYPE_LENGTH,
		TYPE_RANDOM,
		TYPE_ROUTE,
		TYPE_CHAIN,
	};

	enum
	{
		EMITTER_POINT = 0,
		EMITTER_SPHERE,
		EMITTER_CYLINDER,
		EMITTER_BOX,
		EMITTER_RANDOM,
		EMITTER_SPARK,
	};

	enum
	{
		DEFLECTOR_REFLECTOR = 0,
		DEFLECTOR_CLIPPER,
	};
};
typedef Ptr<ObjectParticles> ObjectParticlesPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectBillboards : public Object
{
public:
	static int type() { return Node::OBJECT_BILLBOARDS; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectBillboards> create();
	void setDepthSort(int sort);
	int getDepthSort() const;
	void clearBillboards();
	void allocateBillboards(int num);
	int addBillboard(float width, float height);
	void removeBillboard(int num);
	int getNumBillboards() const;
	void setBillboardPosition(int num, const Math::vec3 &position);
	Math::vec3 getBillboardPosition(int num) const;
	void setTexCoord(int num, const Math::vec4 &texcoord);
	Math::vec4 getTexCoord(int num) const;
	void setWidth(int num, float width);
	float getWidth(int num) const;
	void setHeight(int num, float height);
	float getHeight(int num) const;
	void setAngle(int num, float angle);
	float getAngle(int num) const;
	void setNormal(int num, const Math::vec3 &normal);
	Math::vec3 getNormal(int num) const;
	int saveStateSelf(const Ptr<Stream> &stream) const;
	int restoreStateSelf(const Ptr<Stream> &stream);
	int saveStateBillboards(const Ptr<Stream> &stream) const;
	int restoreStateBillboards(const Ptr<Stream> &stream);
};
typedef Ptr<ObjectBillboards> ObjectBillboardsPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectVolumeBox : public Object
{
public:
	static int type() { return Node::OBJECT_VOLUME_BOX; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectVolumeBox> create(const Math::vec3 &size);
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
};
typedef Ptr<ObjectVolumeBox> ObjectVolumeBoxPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectVolumeSphere : public Object
{
public:
	static int type() { return Node::OBJECT_VOLUME_SPHERE; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectVolumeSphere> create(const Math::vec3 &radius);
	void setRadius(const Math::vec3 &radius);
	Math::vec3 getRadius() const;
};
typedef Ptr<ObjectVolumeSphere> ObjectVolumeSpherePtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectVolumeOmni : public Object
{
public:
	static int type() { return Node::OBJECT_VOLUME_OMNI; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectVolumeOmni> create(float width, float height, float radius);
	void setSize(float width, float height);
	float getWidth() const;
	float getHeight() const;
	void setRadius(float radius);
	float getRadius() const;
	void setAttenuation(float attenuation);
	float getAttenuation() const;
};
typedef Ptr<ObjectVolumeOmni> ObjectVolumeOmniPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectVolumeProj : public Object
{
public:
	static int type() { return Node::OBJECT_VOLUME_PROJ; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectVolumeProj> create(float width, float height, float fov);
	void setSize(float size);
	float getSize() const;
	void setRadius(float radius);
	float getRadius() const;
	void setFov(float fov);
	float getFov() const;
	void setStep(float step);
	float getStep() const;
	void setVelocity(float velocity);
	float getVelocity() const;
	void setVolumeRotation(float rotation);
	float getVolumeRotation() const;
};
typedef Ptr<ObjectVolumeProj> ObjectVolumeProjPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectGui : public Object
{
public:
	static int type() { return Node::OBJECT_GUI; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }


	enum
	{
		MOUSE_STANDARD = 0,
		MOUSE_VIRTUAL,
	};
	static Ptr<ObjectGui> create(float width, float height, const char *name = 0);
	Ptr<Gui> getGui() const;
	void setBillboard(bool billboard);
	bool isBillboard() const;
	void setBackground(bool background);
	bool isBackground() const;
	void setDepthTest(bool test);
	bool isDepthTest() const;
	void setMouseShow(bool show);
	bool isMouseShow() const;
	void setMouseMode(int mode);
	int getMouseMode() const;
	void setMouse(const Math::Vec3 &p0, const Math::Vec3 &p1, int mouse_buttons, bool mouse_show);
	void setControlDistance(float distance);
	float getControlDistance() const;
	void setPolygonOffset(float offset);
	float getPolygonOffset() const;
	void setPhysicalSize(float width, float height);
	float getPhysicalWidth() const;
	float getPhysicalHeight() const;
	void setScreenSize(int width, int height);
	int getScreenWidth() const;
	int getScreenHeight() const;
};
typedef Ptr<ObjectGui> ObjectGuiPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectGuiMesh : public Object
{
public:
	static int type() { return Node::OBJECT_GUI_MESH; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }


	enum
	{
		MOUSE_STANDARD = 0,
		MOUSE_VIRTUAL,
	};
	static Ptr<ObjectGuiMesh> create(const Ptr<Mesh> &mesh, const char *path = 0);
	static Ptr<ObjectGuiMesh> create(const char *mesh_name, const char *path = 0, bool unique = false);
	Ptr<Gui> getGui() const;
	int createMesh(const char *path, bool unique = false);
	int loadMesh(const char *path);
	int saveMesh(const char *path);
	int setMesh(const Ptr<Mesh> &mesh);
	int getMesh(Ptr<Mesh> &mesh) const;
	void flushMesh();
	void setMeshName(const char *name);
	const char *getMeshName() const;
	void setMouseShow(bool show);
	bool isMouseShow() const;
	void setBackground(bool background);
	bool isBackground() const;
	void setMouseMode(int mode);
	int getMouseMode() const;
	void setMouse(const Math::Vec3 &p0, const Math::Vec3 &p1, int mouse_buttons, bool mouse_show);
	void setControlDistance(float distance);
	float getControlDistance() const;
	void setScreenSize(int width, int height);
	int getScreenWidth() const;
	int getScreenHeight() const;
};
typedef Ptr<ObjectGuiMesh> ObjectGuiMeshPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectWaterMesh : public Object
{
public:
	static int type() { return Node::OBJECT_WATER_MESH; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectWaterMesh> create();
	static Ptr<ObjectWaterMesh> create(const Ptr<Mesh> &mesh);
	static Ptr<ObjectWaterMesh> create(const char *path);
	int loadMesh(const char *path);
	int saveMesh(const char *path);
	int setMesh(const Ptr<Mesh> &mesh);
	int getMesh(Ptr<Mesh> &mesh) const;
	int setMeshName(const char *path, bool force_load = false);
	const char *getMeshName() const;
	void setFieldMask(int mask);
	int getFieldMask() const;
	void setWave(int num, const Math::vec4 &wave);
	Math::vec4 getWave(int num) const;
	void setWaveAngle(int num, float angle);
	float getWaveAngle(int num) const;
	void setWaveSpeed(int num, float speed);
	float getWaveSpeed(int num) const;
	void setWaveLength(int num, float length);
	float getWaveLength(int num) const;
	void setWaveHeight(int num, float height);
	float getWaveHeight(int num) const;
	float getHeight(const Math::Vec3 &position) const;
	Math::vec3 getNormal(const Math::Vec3 &position) const;
};
typedef Ptr<ObjectWaterMesh> ObjectWaterMeshPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectText : public Object
{
public:
	static int type() { return Node::OBJECT_TEXT; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectText> create();
	static Ptr<ObjectText> create(const char *font_name);
	static Ptr<ObjectText> create(const char *font_name, const char *text);
	void setFontName(const char *name);
	const char *getFontName() const;
	void setFontRich(int rich);
	int getFontRich() const;
	void setFontSize(int size);
	int getFontSize() const;
	void setFontResolution(int resolution);
	int getFontResolution() const;
	void setFontHSpacing(int hspacing);
	int getFontHSpacing() const;
	void setFontVSpacing(int vspacing);
	int getFontVSpacing() const;
	void setFontOutline(int outline);
	int getFontOutline() const;
	void setDepthTest(int test);
	int getDepthTest() const;
	void setText(const char *text);
	const char *getText() const;
	void setTextColor(const Math::vec4 &color);
	Math::vec4 getTextColor() const;
	void setTextWrapWidth(float width);
	float getTextWrapWidth() const;
};
typedef Ptr<ObjectText> ObjectTextPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectWaterGlobal : public Object
{
public:
	static int type() { return Node::OBJECT_WATER_GLOBAL; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectWaterGlobal> create();
	void setActiveWater(bool water);
	bool isActiveWater() const;
	void setAnimationTime(float time);
	float getAnimationTime() const;
	float getMeanLevel() const;
	void setWindDirectionAngle(float angle);
	float getWindDirectionAngle() const;
	void setWindAffect(float affect);
	float getWindAffect() const;
	void setWavesSpeedScale(float scale);
	float getWavesSpeedScale() const;

	enum STEEPNESS_QUALITY
	{
		STEEPNESS_QUALITY_LOW,
		STEEPNESS_QUALITY_MEDIUM,
		STEEPNESS_QUALITY_HIGH,
		STEEPNESS_QUALITY_ULTRA,
		STEEPNESS_QUALITY_EXTREME,
	};
	float fetchHeight(const Math::Vec3 &position) const;
	Math::vec3 fetchNormal(const Math::Vec3 &position) const;
	void setFetchAmplitudeThreshold(float threshold);
	float getFetchAmplitudeThreshold() const;
	void setFetchSteepnessQuality(ObjectWaterGlobal::STEEPNESS_QUALITY quality);
	ObjectWaterGlobal::STEEPNESS_QUALITY getFetchSteepnessQuality() const;
	using Object::getIntersection;
	void setIntersectionPrecision(float precision);
	float getIntersectionPrecision() const;
	void setIntersectionAmplitudeThreshold(float threshold);
	float getIntersectionAmplitudeThreshold() const;
	void setIntersectionSteepnessQuality(ObjectWaterGlobal::STEEPNESS_QUALITY quality);
	ObjectWaterGlobal::STEEPNESS_QUALITY getIntersectionSteepnessQuality() const;
	int getPhysicsFieldMask() const;
	int getVisualFieldMask() const;
	void takeSyncData(const Ptr<Stream> &stream);
	void applySyncData(const Ptr<Stream> &stream);

	enum WAVES_MODE
	{
		WAVES_MODE_MANUAL,
		WAVES_MODE_LAYERS,
		WAVES_MODE_BEAUFORTS,
	};
	void setWavesMode(ObjectWaterGlobal::WAVES_MODE mode);
	ObjectWaterGlobal::WAVES_MODE getWavesMode() const;
	int addWave(float length, float amplitude, float steepness, float direction_angle, float phase);
	void removeWave(int index);
	int getNumWaves() const;
	float getWaveLength(int index) const;
	void setWaveLength(int index, float value);
	float getWaveAmplitude(int index) const;
	void setWaveAmplitude(int index, float value);
	float getWaveSteepness(int index) const;
	void setWaveSteepness(int index, float value);
	float getWaveDirectionAngle(int index) const;
	void setWaveDirectionAngle(int index, float value);
	float getWavePhaseOffset(int index) const;
	void setWavePhaseOffset(int index, float value);
	int addLayer();
	void removeLayer(int layer);
	int getNumLayers() const;
	void swapLayer(int num_0, int num_1);
	void setLayerEnabled(int layer, bool enabled);
	bool isLayerEnabled(int layer) const;
	int getLayerNumWaves(int layer) const;
	void setLayerNumWaves(int layer, int num);
	Math::vec2 getLayerLengthRange(int layer) const;
	void setLayerLengthRange(int layer, const Math::vec2 &value);
	Math::vec2 getLayerAmplitudeRange(int layer) const;
	void setLayerAmplitudeRange(int layer, const Math::vec2 &value);
	float getLayerSteepnessScale(int layer) const;
	void setLayerSteepnessScale(int layer, float value);
	float getLayerDirectionAngleVariance(int layer) const;
	void setLayerDirectionAngleVariance(int layer, float value);
	float getLayerWeight(int layer) const;
	void setLayerWeight(int layer, float value);
	const char *getLayerName(int layer) const;
	void setLayerName(int layer, const char *value);
	void setBeaufort(float beaufort);
	float getBeaufort() const;
	void setGeometryNormalIntensity(float intensity);
	float getGeometryNormalIntensity() const;
	void setTextureNormalBlur(float blur);
	float getTextureNormalBlur() const;
	void setTextureNormalIntensity(float intensity);
	float getTextureNormalIntensity() const;
	void setDetailTexturePath(const char *path);
	const char *getDetailTexturePath() const;
	void setDetail0UVSize(const Math::vec2 &size);
	Math::vec2 getDetail0UVSize() const;
	void setDetail0UVSpeed(const Math::vec2 &speed);
	Math::vec2 getDetail0UVSpeed() const;
	void setDetail0Intensity(float intensity);
	float getDetail0Intensity() const;
	void setDetail1UVSize(const Math::vec2 &size);
	Math::vec2 getDetail1UVSize() const;
	void setDetail1UVSpeed(const Math::vec2 &speed);
	Math::vec2 getDetail1UVSpeed() const;
	void setDetail1Intensity(float intensity);
	float getDetail1Intensity() const;
	void setDistantWavesTexturePath(const char *path);
	const char *getDistantWavesTexturePath() const;
	void setDistantWavesUVTransform(const Math::vec4 &transform);
	Math::vec4 getDistantWavesUVTransform() const;
	void setDistantWavesIntensity(float intensity);
	float getDistantWavesIntensity() const;
	void setDistantWavesBlendDistanceStart(float start);
	float getDistantWavesBlendDistanceStart() const;
	void setDistantWavesBlendDistanceEnd(float end);
	float getDistantWavesBlendDistanceEnd() const;
	void setDistantWavesBlendMin(float val);
	float getDistantWavesBlendMin() const;
	void setDistantWavesBlendMax(float val);
	float getDistantWavesBlendMax() const;
	void setFoamTexturePath(const char *path);
	const char *getFoamTexturePath() const;
	void setFoam0UVScale(float scale);
	float getFoam0UVScale() const;
	void setFoam0UVSpeed(float speed);
	float getFoam0UVSpeed() const;
	void setFoam1UVScale(float scale);
	float getFoam1UVScale() const;
	void setFoam1UVSpeed(float speed);
	float getFoam1UVSpeed() const;
	void setFoamPeakContrast(float contrast);
	float getFoamPeakContrast() const;
	void setFoamPeakIntensity(float intensity);
	float getFoamPeakIntensity() const;
	void setFoamWhitecapContrast(float contrast);
	float getFoamWhitecapContrast() const;
	void setFoamWhitecapIntensity(float intensity);
	float getFoamWhitecapIntensity() const;
	void setFoamWindContrast(float contrast);
	float getFoamWindContrast() const;
	void setFoamWindIntensity(float intensity);
	float getFoamWindIntensity() const;
	void setFoamContactIntensity(float intensity);
	float getFoamContactIntensity() const;
	void setFoamTextureAffect(float affect);
	float getFoamTextureAffect() const;
	void setSubsurfaceColor(const Math::vec4 &color);
	Math::vec4 getSubsurfaceColor() const;
	void setSubsurfaceAmbientIntensity(float intensity);
	float getSubsurfaceAmbientIntensity() const;
	void setSubsurfaceWaveIntensity(float intensity);
	float getSubsurfaceWaveIntensity() const;
	void setSubsurfaceWaveFoamIntensity(float intensity);
	float getSubsurfaceWaveFoamIntensity() const;
	void setSubsurfaceDiffuseIntensity(float intensity);
	float getSubsurfaceDiffuseIntensity() const;
	void setDepthLUTTexturePath(const char *path);
	const char *getDepthLUTTexturePath() const;
	void setUnderwaterFogColor(const Math::vec4 &color);
	Math::vec4 getUnderwaterFogColor() const;
	void setUnderwaterFogTransparency(float transparency);
	float getUnderwaterFogTransparency() const;
	void setUnderwaterFogDepth(float depth);
	float getUnderwaterFogDepth() const;
	void setUnderwaterFogOffset(float offset);
	float getUnderwaterFogOffset() const;
	void setUnderwaterFogEnvironmentInfluence(float influence);
	float getUnderwaterFogEnvironmentInfluence() const;
	void setUnderwaterFogSunInfluence(float influence);
	float getUnderwaterFogSunInfluence() const;
	void setUnderwaterShaftIntensity(float intensity);
	float getUnderwaterShaftIntensity() const;
	void setWaterlineSize(float size);
	float getWaterlineSize() const;
	void setUnderwaterDOF(bool underwaterdof);
	bool isUnderwaterDOF() const;
	void setUnderwaterDofDistance(float distance);
	float getUnderwaterDofDistance() const;
	void setPlanarReflection(bool reflection);
	bool isPlanarReflection() const;

	enum PLANAR_REFLECTION_SIZE
	{
		PLANAR_REFLECTION_SIZE_RESOLUTION_128 = 0,
		PLANAR_REFLECTION_SIZE_RESOLUTION_256,
		PLANAR_REFLECTION_SIZE_RESOLUTION_512,
		PLANAR_REFLECTION_SIZE_RESOLUTION_1024,
		PLANAR_REFLECTION_SIZE_RESOLUTION_2048,
		PLANAR_REFLECTION_SIZE_RESOLUTION_4096,
		PLANAR_REFLECTION_SIZE_RESOLUTION_HEIGHT_QUART,
		PLANAR_REFLECTION_SIZE_RESOLUTION_HEIGHT_HALF,
		PLANAR_REFLECTION_SIZE_RESOLUTION_HEIGHT,
	};
	void setPlanarReflectionMapSizeType(ObjectWaterGlobal::PLANAR_REFLECTION_SIZE type);
	ObjectWaterGlobal::PLANAR_REFLECTION_SIZE getPlanarReflectionMapSizeType() const;
	void setPlanarReflectionDistance(float distance);
	float getPlanarReflectionDistance() const;
	void setPlanarReflectionPivotOffset(const Math::vec3 &offset);
	Math::vec3 getPlanarReflectionPivotOffset() const;
	void setPlanarReflectionViewportMask(int mask);
	int getPlanarReflectionViewportMask() const;
	void setReflectionRoughness(float roughness);
	float getReflectionRoughness() const;
	void setReflectionOcclusion(float occlusion);
	float getReflectionOcclusion() const;
	void setReflectionOcclusionSlope(float slope);
	float getReflectionOcclusionSlope() const;
	void setCaustics(bool caustics);
	bool isCaustics() const;
	void setCausticsDistortion(bool distortion);
	bool isCausticsDistortion() const;
	void setCausticsTexturePath(const char *path);
	const char *getCausticsTexturePath() const;
	void setCausticUVTransform(const Math::vec4 &transform);
	Math::vec4 getCausticUVTransform() const;
	void setCausticDistanceFade(float fade);
	float getCausticDistanceFade() const;
	void setCausticAnimationSpeed(float speed);
	float getCausticAnimationSpeed() const;
	void setCausticBrightness(float brightness);
	float getCausticBrightness() const;
	void setFieldHeightEnabled(bool enabled);
	bool isFieldHeightEnabled() const;
	void setFieldHeightFoamContrast(float contrast);
	float getFieldHeightFoamContrast() const;
	void setFieldHeightFoamIntensity(float intensity);
	float getFieldHeightFoamIntensity() const;
	void setFieldHeightSteepness(float steepness);
	float getFieldHeightSteepness() const;
	void setFieldShorelineEnabled(bool enabled);
	bool isFieldShorelineEnabled() const;
	void setFieldShorelineHighPrecision(bool precision);
	bool isFieldShorelineHighPrecision() const;
	void setFieldShorelineNormal(bool normal);
	bool isFieldShorelineNormal() const;
	void setFieldShorelineGeometry(bool geometry);
	bool isFieldShorelineGeometry() const;
	void setFieldShorelineFoam(bool foam);
	bool isFieldShorelineFoam() const;
	void setFieldShorelineLUTTexturePath(const char *path);
	const char *getFieldShorelineLUTTexturePath() const;
	void setFieldShorelineWaveSpeed(float speed);
	float getFieldShorelineWaveSpeed() const;
	void setFieldShorelineWaveTiling(float tiling);
	float getFieldShorelineWaveTiling() const;
	void setFieldShorelineWaveHeight(float height);
	float getFieldShorelineWaveHeight() const;
	void setFieldShorelineWaveFalloff(float falloff);
	float getFieldShorelineWaveFalloff() const;
	void setFieldShorelineWaveExponent(float exponent);
	float getFieldShorelineWaveExponent() const;
	void setFieldShorelineWaveFrontExponent(float exponent);
	float getFieldShorelineWaveFrontExponent() const;
	void setFieldShorelineFoamStretching(float stretching);
	float getFieldShorelineFoamStretching() const;
	void setFieldShorelineFoamIntensity(float intensity);
	float getFieldShorelineFoamIntensity() const;
	void setFieldShorelineFoamExponent(float exponent);
	float getFieldShorelineFoamExponent() const;
	void setFieldShorelineMaskTiling(float tiling);
	float getFieldShorelineMaskTiling() const;
	void setFieldShorelineBeaufortFalloff(float falloff);
	float getFieldShorelineBeaufortFalloff() const;
	void setShorelineWetnessIntensity(float intensity);
	float getShorelineWetnessIntensity() const;
	void setShorelineWetnessDistance(float distance);
	float getShorelineWetnessDistance() const;
	void setShorelineWetnessOffset(float offset);
	float getShorelineWetnessOffset() const;
	void setAuxiliary(bool auxiliary);
	bool isAuxiliary() const;
	void setAuxiliaryColor(const Math::vec4 &color);
	Math::vec4 getAuxiliaryColor() const;
	void setRefractionScale(float scale);
	float getRefractionScale() const;
	void setDiffuseDistortion(float distortion);
	float getDiffuseDistortion() const;
	void setSoftIntersection(float intersection);
	float getSoftIntersection() const;
	void setFieldSpacerEnabled(bool enabled);
	bool isFieldSpacerEnabled() const;
};
typedef Ptr<ObjectWaterGlobal> ObjectWaterGlobalPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectCloudLayer : public Object
{
public:
	static int type() { return Node::OBJECT_CLOUD_LAYER; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectCloudLayer> create();
	void setIntersectionThreshold(float threshold);
	float getIntersectionThreshold() const;
	void setIntersectionAccuracy(float accuracy);
	float getIntersectionAccuracy() const;
	void setAnimationCoverageOffset(const Math::vec4 &offset);
	Math::vec4 getAnimationCoverageOffset() const;
	void setAnimationNoiseOffset(const Math::vec4 &offset);
	Math::vec4 getAnimationNoiseOffset() const;
	void refreshCloudsRegionMask();
	float getDensity(const Math::Vec3 &world_point) const;
};
typedef Ptr<ObjectCloudLayer> ObjectCloudLayerPtr;

class ObjectExternBase;
//////////////////////////////////////////////////////////////////////////

class UNIGINE_API ObjectExtern : public Object
{
public:
	static int type() { return Node::OBJECT_EXTERN; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<ObjectExtern> create(int class_id);
	ObjectExternBase *getObjectExtern() const;
	int getClassID() const;
	void setMaterialNodeType(int type);
	int getMaterialNodeType() const;
};
typedef Ptr<ObjectExtern> ObjectExternPtr;


class UNIGINE_API  ObjectExternBase : public virtual Base
{
public:
	ObjectExternBase();
	ObjectExternBase(void *object);
	virtual ~ObjectExternBase();
	virtual int getClassID() = 0;
	Ptr<Node> getNode() const;
	Ptr<Object> getObject() const;
	Ptr<ObjectExtern> getObjectExtern() const;
	typedef ObjectExternBase *(*ConstructorFunc)(void *object);
	static void addClassID(int class_id, ConstructorFunc func);
	template <class Type> static void addClassID(int class_id) { addClassID(class_id, constructor<Type>); }
	virtual int getNumSurfaces() = 0;
	virtual const char *getSurfaceName(int surface) = 0;
	virtual int findSurface(const char *name);
	void updateSurfaces() const;
	void resizeSurfaces() const;
	virtual void updateEnabled(int surface);
	virtual void updateEnabled();
	virtual int getResource(int surface);
	virtual int getOrder(const Math::Vec3 &camera, int surface);
	virtual int getSequence(const Math::Vec3 &camera, int surface);
	virtual float getTransparentDistance(const Math::Vec3 &camera, int surface);
	virtual int hasLods();
	virtual int getCollision(const Math::BoundBox &bb, Vector<int> &surfaces);
	virtual int getCollision(const Math::Vec3 &p0, const Math::Vec3 &p1, Vector<int> &surfaces);
	virtual int getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, Math::Vec3 *ret_point, Math::vec3 *ret_normal, Math::vec4 *ret_texcoord, int *ret_index, int *ret_instance, int surface);
	virtual int getRandomPoint(Math::vec3 &ret_point, Math::vec3 &ret_normal, Math::vec3 &ret_velocity, int surface);
	virtual int getNumTriangles(int surface);
	virtual const Math::BoundBox &getBoundBox(int surface) = 0;
	virtual const Math::BoundSphere &getBoundSphere(int surface) = 0;
	virtual int hasCreate();
	virtual void create(int surface);
	virtual int hasRender();
	virtual void render(Render::PASS pass, int surface);
	virtual int hasShadow();
	virtual void renderShadow(Render::PASS pass, int surface);
	virtual void updatePosition();
	virtual void updateTransform();
	virtual void update(float ifps);
	virtual void flush(float) final {} // This method was given a more meaningful name "preRender", please override it instead.
	virtual void preRender(float ifps);
	virtual const Math::BoundBox &getBoundBox() = 0;
	virtual const Math::BoundSphere &getBoundSphere() = 0;
	virtual void renderHandler();
	virtual void renderVisualizer();
	virtual void copy(ObjectExternBase *object, int is_root);
	virtual void swap(ObjectExternBase *object);
	virtual int loadWorld(const Ptr<Xml> &xml);
	virtual int saveWorld(const Ptr<Xml> &xml);
	virtual int saveState(const Ptr<Stream> &stream);
	virtual int restoreState(const Ptr<Stream> &stream);
	virtual void setUpdateDistanceLimit(float distance);
	virtual float getUpdateDistanceLimit();
	virtual void setForceUpdate(bool enabled);
	virtual bool isForceUpdate();
	static void savePointer(const StreamPtr &stream, ObjectExternBase *object);
	static ObjectExternBase *restorePointer(const StreamPtr &stream);

private:
	template <class Type> static ObjectExternBase *constructor(void *object) { return new Type(object); }
	struct ObjectExternData;
	ObjectExternData *data;
};
typedef Ptr<ObjectExternBase> ObjectExternBasePtr;

} // namespace Unigine
