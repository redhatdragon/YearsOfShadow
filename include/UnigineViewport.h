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
#include "UnigineCamera.h"
#include "UnigineImage.h"
#include "UnigineTextures.h"
#include "UnigineCallback.h"
#include "UnigineMesh.h"
#include "UnigineRender.h"

namespace Unigine
{


class UNIGINE_API Viewport : public APIInterface
{
public:
	static Ptr<Viewport> create();

	enum
	{
		SKIP_SHADOWS = 1 << 0,
		SKIP_VISUALIZER = 1 << 1,
		SKIP_POSTEFFECTS = 1 << 2,
		SKIP_DYNAMIC_REFLECTIONS = 1 << 3,
		SKIP_VELOCITY_BUFFER = 1 << 4,
		SKIP_SRGB = 1 << 5,
		SKIP_FORMAT_RG11B10 = 1 << 6,
		SKIP_TRANSPARENT = 1 << 7,
		SKIP_STREAMING = 1 << 8,
		SKIP_AUTO_EXPOSURE = 1 << 9,
		SKIP_AUTO_WHITE_BALANCE = 1 << 10,
	};

	enum
	{
		RENDER_DEPTH,
		RENDER_DEPTH_GBUFFER,
		RENDER_DEPTH_GBUFFER_FINAL,
	};

	enum
	{
		USAGE_WORLD_LIGHT = 0,
		USAGE_AUX_LIGHT = 1,
		USAGE_NODE_LIGHT = 2,
	};
	int getID() const;
	void setAspectCorrection(bool correction);
	bool isAspectCorrection() const;
	void setUseTAAOffset(bool offset);
	bool isUseTAAOffset() const;
	void setFirstFrame(int frame);
	int getFirstFrame() const;
	void setSkipFlags(int flags);
	int getSkipFlags() const;
	int checkSkipFlags(int flags);
	void appendSkipFlags(int flags);
	void removeSkipFlags(int flags);
	void setMode(Render::VIEWPORT_MODE mode);
	Render::VIEWPORT_MODE getMode() const;
	void setRenderMode(int mode);
	int getRenderMode() const;
	void setPaused(bool paused);
	bool isPaused() const;
	bool isStereo() const;
	bool isPanorama() const;
	void setPanoramaFisheyeFov(float fov);
	float getPanoramaFisheyeFov() const;
	void setStereoDistance(float distance);
	float getStereoDistance() const;
	void setStereoRadius(float radius);
	float getStereoRadius() const;
	void setStereoOffset(float offset);
	float getStereoOffset() const;
	void renderEngine(const Ptr<Camera> &camera);
	void render(const Ptr<Camera> &camera);
	void render(const Ptr<Camera> &camera, int width, int height);
	void renderTexture2D(const Ptr<Camera> &camera, const Ptr<Texture> &texture);
	void renderImage2D(const Ptr<Camera> &camera, const Ptr<Image> &image);
	void renderImage2D(const Ptr<Camera> &camera, const Ptr<Image> &image, int width, int height, bool hdr = false);
	void renderTextureCube(const Ptr<Camera> &camera, const Ptr<Texture> &texture, bool local_space = false);
	void renderImageCube(const Ptr<Camera> &camera, const Ptr<Image> &image, int size, bool hdr = false, bool local_space = false);
	void renderImageCube(const Ptr<Camera> &camera, const Ptr<Image> &image);
	void renderStereo(const Ptr<Camera> &camera_left, const Ptr<Camera> &camera_right, const char *stereo_material);
	void renderStereoPeripheral(const Ptr<Camera> &camera_left, const Ptr<Camera> &camera_right, const Ptr<Camera> &camera_focus_left, const Ptr<Camera> &camera_focus_right, const Ptr<Texture> &texture_left, const Ptr<Texture> &texture_right, const Ptr<Texture> &texture_focus_left, const Ptr<Texture> &texture_focus_right, const char *stereo_material);
	void setStereoHiddenAreaMesh(const Ptr<Mesh> &hidden_area_mesh_left, const Ptr<Mesh> &hidden_area_mesh_right);
	void clearStereoHiddenAreaMesh();
	void setNodeLightUsage(int usage);
	int getNodeLightUsage() const;
	void setEnvironmentTexturePath(const char *name);
	const char *getEnvironmentTexturePath();
	void setEnvironmentTexture(const Ptr<Texture> &texture);
	Ptr<Texture> getEnvironmentTexture() const;
	void resetEnvironmentTexture();
	void renderNode(const Ptr<Camera> &camera, const Ptr<Node> &node);
	void renderNode(const Ptr<Camera> &camera, const Ptr<Node> &node, int width, int height);
	void renderNodeTexture2D(const Ptr<Camera> &camera, const Ptr<Node> &node, const Ptr<Texture> &texture);
	void renderNodeImage2D(const Ptr<Camera> &camera, const Ptr<Node> &node, const Ptr<Image> &image, int width, int height, bool hdr);
	void renderNodeImage2D(const Ptr<Camera> &camera, const Ptr<Node> &node, const Ptr<Image> &image);
	void renderNodes(const Ptr<Camera> &camera, const Vector<Ptr<Node>> &nodes);
	void renderNodes(const Ptr<Camera> &camera, const Vector<Ptr<Node>> &nodes, int width, int height);
	void renderNodesTexture2D(const Ptr<Camera> &camera, const Vector<Ptr<Node>> &nodes, const Ptr<Texture> &texture);
	void renderNodesImage2D(const Ptr<Camera> &camera, const Vector<Ptr<Node>> &nodes, const Ptr<Image> &image, int width, int height, int hdr);
	void renderNodesImage2D(const Ptr<Camera> &camera, const Vector<Ptr<Node>> &nodes, const Ptr<Image> &image);
	void *addCallback(Render::CALLBACK_INDEX callback, CallbackBase *func);
	bool removeCallback(Render::CALLBACK_INDEX callback, void *id);
	void clearCallbacks(Render::CALLBACK_INDEX callback);
};
typedef Ptr<Viewport> ViewportPtr;

} // namespace Unigine
