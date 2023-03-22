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

#include "UnigineCallback.h"
#include "UnigineNode.h"
#include "UnigineNodes.h"
#include "UniginePath.h"
#include "UniginePtr.h"
#include "UnigineSet.h"
#include "UnigineImage.h"
#include "UnigineMesh.h"
#include "UnigineSplineGraph.h"

namespace Unigine
{

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API WorldTrigger : public Node
{
public:
	static int type() { return Node::WORLD_TRIGGER; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<WorldTrigger> create(const Math::vec3 &size);
	void setTouch(bool touch);
	bool isTouch() const;
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
	void setTargetNodes(const Set<Ptr<Node>> &nodes);
	Set<Ptr<Node>> getTargetNodes() const;
	void setTargetTypes(const Set<int> &types);
	Set<int> getTargetTypes() const;
	void setExcludeNodes(const Set<Ptr<Node>> &nodes);
	Set<Ptr<Node>> getExcludeNodes() const;
	void setExcludeTypes(const Set<int> &types);
	Set<int> getExcludeTypes() const;
	int getNumNodes() const;
	Ptr<Node> getNode(int num) const;
	Vector<Ptr<Node>> getNodes() const;
	void setEnterCallbackName(const char *name);
	const char *getEnterCallbackName() const;
	void setLeaveCallbackName(const char *name);
	const char *getLeaveCallbackName() const;
	void *addEnterCallback(CallbackBase1<Ptr<Node>> *func);
	bool removeEnterCallback(void *id);
	void clearEnterCallbacks();
	void *addLeaveCallback(CallbackBase1<Ptr<Node>> *func);
	bool removeLeaveCallback(void *id);
	void clearLeaveCallbacks();
};
typedef Ptr<WorldTrigger> WorldTriggerPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API WorldClutter : public Node
{
public:
	static int type() { return Node::WORLD_CLUTTER; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<WorldClutter> create();
	int addReference(const char *name);
	void removeReference(int num);
	void clearReferences();
	int getNumReferences() const;
	void setReferenceName(int num, const char *name);
	const char *getReferenceName(int num) const;
	void setReferenceProbability(int num, float probability);
	float getReferenceProbability(int num) const;
	void setOrientation(int orientation);
	int getOrientation() const;
	void setIntersection(int intersection);
	int getIntersection() const;
	void setVisibleDistance(float distance);
	float getVisibleDistance() const;
	void setFadeDistance(float distance);
	float getFadeDistance() const;
	void setSpawnRate(int rate);
	int getSpawnRate() const;
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
	void setNodesRotation(const Math::vec3 &mean, const Math::vec3 &spread);
	Math::vec3 getNodesRotationMean() const;
	Math::vec3 getNodesRotationSpread() const;
	int setMaskImage(const Ptr<Image> &image, bool invalidate = true);
	int getMaskImage(const Ptr<Image> &image) const;
	void setMaskImageName(const char *image_name, bool invalidate = true);
	const char *getMaskImageName() const;
	void setMaskFlipX(int maskflipx);
	int getMaskFlipX() const;
	void setMaskFlipY(int maskflipy);
	int getMaskFlipY() const;
	void setMaskMinValue(int value);
	int getMaskMinValue() const;
	void setMaskMaxValue(int value);
	int getMaskMaxValue() const;
	int setMaskMesh(const Ptr<Mesh> &mesh, bool invalidate = true);
	int getMaskMesh(const Ptr<Mesh> &mesh) const;
	void setMaskMeshName(const char *mesh_name, bool invalidate = true);
	const char *getMaskMeshName() const;
	void setMaskInverse(int inverse);
	int getMaskInverse() const;
	void setCutoutIntersectionMask(int mask);
	int getCutoutIntersectionMask() const;
	void setCutoutInverse(int inverse);
	int getCutoutInverse() const;
	void invalidate();
	void invalidate(const Math::WorldBoundBox &bounds);
	bool saveStateReferences(const Ptr<Stream> &stream) const;
	bool restoreStateReferences(const Ptr<Stream> &stream);
};
typedef Ptr<WorldClutter> WorldClutterPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API WorldOccluder : public Node
{
public:
	static int type() { return Node::WORLD_OCCLUDER; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<WorldOccluder> create(const Math::vec3 &size);
	void setBackFace(bool face);
	bool isBackFace() const;
	void setSize(const Math::vec3 &size);
	Math::vec3 getSize() const;
	void setDistance(float distance);
	float getDistance() const;
};
typedef Ptr<WorldOccluder> WorldOccluderPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API WorldOccluderMesh : public Node
{
public:
	static int type() { return Node::WORLD_OCCLUDER_MESH; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<WorldOccluderMesh> create();
	static Ptr<WorldOccluderMesh> create(const char *name);
	bool loadMesh(const char *name);
	bool saveMesh(const char *name) const;
	bool setMesh(const Ptr<Mesh> &mesh);
	bool getMesh(Ptr<Mesh> &mesh) const;
	bool setMeshName(const char *name, bool force_load = false);
	const char *getMeshName() const;
	void setDistance(float distance);
	float getDistance() const;
};
typedef Ptr<WorldOccluderMesh> WorldOccluderMeshPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API WorldTransformPath : public Node
{
public:
	static int type() { return Node::WORLD_TRANSFORM_PATH; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<WorldTransformPath> create(const char *name, int unique = 0);
	Ptr<Path> getPath() const;
	void setPathName(const char *name, int unique = 0);
	const char *getPathName() const;
	void setUpdateDistanceLimit(float limit);
	float getUpdateDistanceLimit() const;
	void setOrientation(int orientation);
	int getOrientation() const;
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
};
typedef Ptr<WorldTransformPath> WorldTransformPathPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API WorldTransformBone : public Node
{
public:
	static int type() { return Node::WORLD_TRANSFORM_BONE; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<WorldTransformBone> create(const char *name = 0);
	void setBoneName(const char *name);
	const char *getBoneName() const;
};
typedef Ptr<WorldTransformBone> WorldTransformBonePtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API WorldExpression : public Node
{
public:
	static int type() { return Node::WORLD_EXPRESSION; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<WorldExpression> create();
	void setIFps(float ifps);
	float getIFps() const;
	void setUpdateDistanceLimit(float limit);
	float getUpdateDistanceLimit() const;
	bool setExpressionName(const char *name);
	const char *getExpressionName() const;
	bool setExpression(const char *src);
	const char *getExpression() const;
	bool isFunction(const char *name, int num_args) const;
	bool isCompiled() const;
};
typedef Ptr<WorldExpression> WorldExpressionPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API WorldSwitcher : public Node
{
public:
	static int type() { return Node::WORLD_SWITCHER; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<WorldSwitcher> create();
	void setMinDistance(float distance);
	float getMinDistance() const;
	void setMaxDistance(float distance);
	float getMaxDistance() const;
};
typedef Ptr<WorldSwitcher> WorldSwitcherPtr;

class SplineSegment;
class WorldSplineGraph;

class UNIGINE_API SplinePoint : public APIInterface
{
public:
	void setEnabled(bool enable, bool with_segments = true);
	bool isEnabled() const;
	void setPosition(const Math::Vec3 &position);
	Math::Vec3 getPosition() const;
	int getLinkWorldPosition(const Ptr<SplineSegment> &segment, Math::Vec3 &position) const;
	Ptr<WorldSplineGraph> getParent() const;
	int getNumSegments() const;
	void getSplineSegments(Vector<Ptr<SplineSegment>> &segments) const;
	void clearSources();
	void getSources(Vector<String> &sources) const;
	int getNumSources() const;
	void assignSource(const char *name);
	void removeSource(const char *name);
	int getSourceNodeType(const char *name) const;
	void setLinkVariant(const char *name, int variant);
	int getLinkVariant(const char *name) const;
	void copy(const Ptr<SplinePoint> &src);
	Ptr<SplinePoint> clone();
	Ptr<Node> getNode(int index);
	int getNumNodes() const;
};
typedef Ptr<SplinePoint> SplinePointPtr;


class UNIGINE_API SplineSegment : public APIInterface
{
public:
	void setStartPoint(const Ptr<SplinePoint> &point);
	Ptr<SplinePoint> getStartPoint() const;
	void setEndPoint(const Ptr<SplinePoint> &point);
	Ptr<SplinePoint> getEndPoint() const;
	void setStartTangent(const Math::vec3 &tangent);
	Math::vec3 getStartTangent() const;
	void setStartUp(const Math::vec3 &up);
	Math::vec3 getStartUp() const;
	void setEndTangent(const Math::vec3 &tangent);
	Math::vec3 getEndTangent() const;
	void setEndUp(const Math::vec3 &up);
	Math::vec3 getEndUp() const;
	void setEnabled(bool enable, bool with_points = true);
	bool isEnabled() const;
	Ptr<WorldSplineGraph> getParent() const;
	Math::Vec3 calcPoint(float t) const;
	Math::vec3 calcTangent(float t) const;
	Math::vec3 calcUpVector(float t) const;
	float linearToParametric(float l) const;
	float getLength() const;
	void clearSources();
	void getSources(Vector<String> &sources) const;
	int getNumSources() const;
	void assignSource(const char *name, int forward_axis = FORWARD_Y);
	void removeSource(const char *name, int forward_axis = FORWARD_Y);
	int getForwardAxis(const char *name);

	enum
	{
		FORWARD_X = 0,
		FORWARD_Y,
		FORWARD_Z,
	};

	enum
	{
		SEGMENT_STRETCH,
		SEGMENT_TILING,
		SEGMENT_ADAPTIVE,
	};
	void setSegmentMode(const char *name, int segment_mode = SEGMENT_TILING);
	int getSegmentMode(const char *name) const;
	void setUVTiling(const char *name, int enable_uv_tiling = 0);
	int getUVTiling(const char *name) const;
	void setAdaptiveAngleThreshold(const char *name, float adaptive_angle_threshold = 1.0f);
	float getAdaptiveAngleThreshold(const char *name) const;
	void setGap(const char *name, float gap = 0.0f);
	float getGap(const char *name) const;
	Ptr<Node> getNode(int index);
	int getNumNodes() const;
	void copy(const Ptr<SplineSegment> &src, int copy_endpoints_data = 1);
	Ptr<SplineSegment> clone(int clone_points = 0);
};
typedef Ptr<SplineSegment> SplineSegmentPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API WorldSplineGraph : public Node
{
public:
	static int type() { return Node::WORLD_SPLINE_GRAPH; }
	static bool convertible(Node *node) { return (node && node->getType() == type()); }

	static Ptr<WorldSplineGraph> create();
	static Ptr<WorldSplineGraph> create(const char *name);
	bool load(const char *name);
	bool save(const char *name);
	void clear();
	void setSplineGraphName(const char *name, bool force_load = false);
	const char *getSplineGraphName() const;
	void loadSegmentNodes(int segment_index = -1);
	void getSegmentNodeMesh(const Ptr<Mesh> &mesh, const Ptr<SplineSegment> &segment, int node_index, bool bake_transform = false);
	void getPointNodeMesh(const Ptr<Mesh> &mesh, const Ptr<SplinePoint> &point, int node_index, bool bake_transform = false);
	void makeCurved();
	void makeFlat();
	bool isCurved() const;
	Ptr<SplinePoint> createSplinePoint(const Math::Vec3 &position);
	void removeSplinePoint(const Ptr<SplinePoint> &point, bool merge = false);
	int getNumSplinePoints() const;
	void getSplinePoints(Vector<Ptr<SplinePoint>> &points) const;
	Ptr<SplineSegment> createSplineSegment(const Ptr<SplinePoint> &start_point, const Math::vec3 &start_tangent, const Math::vec3 &start_up, const Ptr<SplinePoint> &end_point, const Math::vec3 &end_tangent, const Math::vec3 &end_up);
	void removeSplineSegment(const Ptr<SplineSegment> &segment, bool with_points = false);
	int getNumSplineSegments() const;
	void getSplineSegments(Vector<Ptr<SplineSegment>> &segments) const;
	void splitSplineSegment(const Ptr<SplineSegment> &segment, float new_point_t);
	void breakSplinePoint(const Ptr<SplinePoint> &point);
	void weldSplinePoints(Vector<Ptr<SplinePoint>> &points);
	bool isLinked(const Ptr<SplinePoint> &point) const;
	void rebuild(int try_force = 0);
	int isRebuilding() const;
	void *addPointAddedCallback(CallbackBase2<Ptr<WorldSplineGraph>, Ptr<SplinePoint>> *func);
	bool removePointAddedCallback(void *id);
	void clearPointAddedCallbacks();
	void *addPointChangedCallback(CallbackBase2<Ptr<WorldSplineGraph>, Ptr<SplinePoint>> *func);
	bool removePointChangedCallback(void *id);
	void clearPointChangedCallbacks();
	void *addPointRemovedCallback(CallbackBase2<Ptr<WorldSplineGraph>, Ptr<SplinePoint>> *func);
	bool removePointRemovedCallback(void *id);
	void clearPointRemovedCallbacks();
	void *addSegmentAddedCallback(CallbackBase2<Ptr<WorldSplineGraph>, Ptr<SplineSegment>> *func);
	bool removeSegmentAddedCallback(void *id);
	void clearSegmentAddedCallbacks();
	void *addSegmentChangedCallback(CallbackBase2<Ptr<WorldSplineGraph>, Ptr<SplineSegment>> *func);
	bool removeSegmentChangedCallback(void *id);
	void clearSegmentChangedCallbacks();
	void *addSegmentRemovedCallback(CallbackBase2<Ptr<WorldSplineGraph>, Ptr<SplineSegment>> *func);
	bool removeSegmentRemovedCallback(void *id);
	void clearSegmentRemovedCallbacks();
	void *addRebuildingFinishedCallback(CallbackBase1<Ptr<WorldSplineGraph>> *func);
	bool removeRebuildingFinishedCallback(void *id);
	void clearRebuildingFinishedCallbacks();
};
typedef Ptr<WorldSplineGraph> WorldSplineGraphPtr;

} // namespace Unigine
