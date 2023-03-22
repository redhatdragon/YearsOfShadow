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

#include "UnigineMathLib.h"
#include "UnigineMesh.h"
#include "UniginePtr.h"
#include "UnigineVector.h"
#include "UnigineStreams.h"

namespace Unigine
{


class UNIGINE_API MeshStatic : public APIInterface
{
public:

	enum
	{
		MODE_TRIANGLES = 0,
		MODE_TRIANGLE_PATCHES,
		MODE_QUAD_PATCHES,
	};
	static Ptr<MeshStatic> create();
	int info(const char *path);
	int load(const char *path);
	int save(const char *path) const;
	int setMesh(const Ptr<Mesh> &mesh);
	int getMesh(Ptr<Mesh> &mesh) const;
	void clear();
	void destroy();
	void flush();
	void asyncCreate();
	void asyncDestroy();
	void bind();
	void unbind();
	int renderSurface(int mode, int surface, int target = 0);
	int renderInstancedSurface(int mode, int num, int surface, int target = 0);
	int render(int mode, int surface = -1, int target = 0);
	int renderInstanced(int mode, int num, int surface = -1, int target = 0);
	size_t getSystemMemoryUsage() const;
	size_t getVideoMemoryUsage() const;
	int getNumSurfaces() const;
	const char *getSurfaceName(int surface) const;
	void setSurfaceLightmapUVChannel(int surface, char uv_channel);
	char getSurfaceLightmapUVChannel(int surface) const;
	void setSurfaceLightmapResolution(int surface, Mesh::LIGHTMAP_RESOLUTION resolution);
	Mesh::LIGHTMAP_RESOLUTION getSurfaceLightmapResolution(int surface) const;
	int findSurface(const char *name) const;
	int getNumSurfaceTargets(int surface) const;
	const char *getSurfaceTargetName(int surface, int target) const;
	int findSurfaceTarget(const char *name, int surface) const;
	void updateSurfaceBounds(int surface = -1);
	void setSurfaceTransform(const Math::mat4 &transform, int surface, int target = -1);
	int addMeshSurface(const char *name, const Ptr<Mesh> &mesh, int surface, int target = -1);
	int addMeshSurface(const char *name, const Ptr<MeshStatic> &mesh, int surface, int target = -1);
	int addMeshSurface(int dest_surface, const Ptr<MeshStatic> &mesh, int surface, int target = -1);
	int getMeshSurface(Ptr<Mesh> &mesh, int surface, int target = -1) const;
	int addEmptySurface(const char *name, int num_vertex, int num_indices);
	int addSurfaceTarget(int surface, const char *name);
	int getCollision(const Math::BoundBox &bb, Vector<int> &surfaces);
	int getCollision(const Math::BoundFrustum &bf, Vector<int> &surfaces);
	int getCollision(const Math::vec3 &p0, const Math::vec3 &p1, Vector<int> &surfaces);
	int getCollision(const Math::BoundBox &bb, Vector<int> &triangles, int surface);
	int getCollision(const Math::BoundFrustum &bf, Vector<int> &triangles, int surface);
	int getCollision(const Math::vec3 &p0, const Math::vec3 &p1, Vector<int> &triangles, int surface);
	Vector<char> &getEdges(int surface);
	int getIntersection(const Math::vec3 &p0, const Math::vec3 &p1, Math::vec3 *ret_point, Math::vec3 *ret_normal, Math::vec4 *ret_texcoord, int *ret_index, int surface, int target = 0);
	int getIntersection(const Math::dvec3 &p0, const Math::dvec3 &p1, Math::dvec3 *ret_point_, Math::vec3 *ret_normal, Math::vec4 *ret_texcoord, int *ret_index, int surface, int target = 0);
	void setBoundBox(const Math::BoundBox &bb, int surface);
	void setBoundSphere(const Math::BoundSphere &bs, int surface);
	Math::BoundBox getBoundBox(int surface) const;
	Math::BoundSphere getBoundSphere(int surface) const;
	void setBoundBox(const Math::BoundBox &bb);
	void setBoundSphere(const Math::BoundSphere &bs);
	Math::BoundBox getBoundBox() const;
	Math::BoundSphere getBoundSphere() const;
	Vector<Math::vec3> &getVertex(int surface, int target = 0);
	Vector<Math::hvec4> &getTangents(int surface, int target = 0);
	Vector<Math::vec2> &getTexCoords0(int surface);
	Vector<Math::vec2> &getTexCoords1(int surface);
	Vector<Math::bvec4> &getColors(int surface);
	Vector<int> &getCIndices(int surface);
	Vector<int> &getTIndices(int surface);
	void updateDebugName();
	void setDebugName(const char *name);
	const char *getDebugName() const;
	void createNodes();
};
typedef Ptr<MeshStatic> MeshStaticPtr;

} // namespace Unigine
