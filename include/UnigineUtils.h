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


#pragma once

#include "UnigineNode.h"
#include "UnigineMesh.h"
#include "UnigineImage.h"
#include "UnigineString.h"

namespace Unigine
{

namespace Utils
{
	UNIGINE_API NodePtr loadObjectMesh(const char *name);
	UNIGINE_API void convertNodesToMesh(MeshPtr &out, const Vector<NodePtr> &nodes);
	UNIGINE_API int convertHeightsToNormals(ImagePtr &out, const ImagePtr &heights, float step_size = 1.0f);
	UNIGINE_API String date(const char *format);
	UNIGINE_API String date(const char *format, long long time);

	UNIGINE_API int makeNodeCurved(NodePtr node);
	UNIGINE_API int makeNodeFlat(NodePtr node);
}

} // namespace
