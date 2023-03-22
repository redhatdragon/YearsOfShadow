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

#include "UnigineString.h"
#include "UnigineMesh.h"
#include "UnigineImage.h"
#include "UnigineNode.h"

namespace Unigine
{

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API AsyncQueue
{
protected:
	

public:
	static int isInitialized(); 

	enum CALLBACK_INDEX
	{
		CALLBACK_BEGIN = 0,
		CALLBACK_FILE_LOADED = CALLBACK_BEGIN,
		CALLBACK_IMAGE_LOADED,
		CALLBACK_MESH_LOADED,
		CALLBACK_NODE_LOADED,
		CALLBACK_END = CALLBACK_NODE_LOADED,
		NUM_CALLBACKS,
	};
	static int loadFile(const char *name, int group = 0, float weight = 0.0f);
	static int removeFile(int id);
	static int forceFile(int id);
	static int checkFile(int id);
	static int loadImageInfo(const char *name, int group = 0, float weight = 0.0f);
	static int loadImage(const char *name, int group = 0, float weight = 0.0f);
	static int removeImage(int id);
	static int forceImage(int id);
	static int checkImage(int id);
	static Ptr<Image> getImage(int id);
	static Ptr<Image> takeImage(int id);
	static int loadMesh(const char *name, int group = 0, float weight = 0.0f);
	static int removeMesh(int id);
	static int forceMesh(int id);
	static int checkMesh(int id);
	static Ptr<Mesh> getMesh(int id);
	static Ptr<Mesh> takeMesh(int id);
	static int loadNode(const char *name, int group = 0, float weight = 0.0f);
	static int removeNode(int id);
	static int forceNode(int id);
	static int checkNode(int id);
	static Ptr<Node> getNode(int id);
	static Ptr<Node> takeNode(int id);
	static int getNodes(int id, Vector<Ptr<Node>> &nodes);
	static int takeNodes(int id, Vector<Ptr<Node>> &nodes);
	static float getTotalTime();
	static int getNumQueuedData();
	static int getNumQueuedFiles();
	static int getNumQueuedImages();
	static int getNumQueuedMeshes();
	static int getNumQueuedNodes();
	static int getNumQueuedResources();
	static int getNumLoadedData();
	static int getNumLoadedFiles();
	static int getNumLoadedImages();
	static int getNumLoadedMeshes();
	static int getNumLoadedNodes();
	static int getNumLoadedResources();
	static void *addCallback(AsyncQueue::CALLBACK_INDEX callback, CallbackBase2<const char *, int> *func);
	static bool removeCallback(AsyncQueue::CALLBACK_INDEX callback, void *id);
	static void clearCallbacks(AsyncQueue::CALLBACK_INDEX callback);
};

} // namespace Unigine
