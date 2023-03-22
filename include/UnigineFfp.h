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

namespace Unigine
{

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Ffp
{
protected:
	

public:
	static int isInitialized(); 

	enum
	{
		MODE_DEFAULT = 0,
		MODE_LOADING_SCREEN,
		MODE_SOLID,
		MODE_YUV,
		MODE_SRGB,
		NUM_MODES,
	};

	enum
	{
		TEXTURE_SAMPLE_2D = 0,
		TEXTURE_SAMPLE_3D,
		TEXTURE_SAMPLE_CUBE,
		TEXTURE_SAMPLE_ARRAY,
		TEXTURE_SAMPLE_SHADOW,
		TEXTURE_SAMPLE_INTEGER,
		NUM_TEXTURES_SAMPLE,
	};

	enum
	{
		COLOR_RED = 0xffff0000u,
		COLOR_GREEN = 0xff00ff00u,
		COLOR_BLUE = 0xff0000ffu,
		COLOR_BLACK = 0xff000000u,
		COLOR_WHITE = 0xffffffffu,
	};
	/// ffp vertex
	struct Vertex
	{
		float xyz[3];
		float texcoord[4];
		union
		{
			unsigned char color[4];
			unsigned int color_vec;
		};
	};

	static void setMode(int mode);
	static int getMode();
	static void setTextureSample(int sample);
	static int getTextureSample();
	static bool isEnabled();
	static void enable(int mode = 0, int texture_sample = 0);
	static void disable();
	static void setOrtho(int width, int height);
	static void setTransform(const Math::mat4 &transform);
	static Math::mat4 getTransform();
	static void beginLines();
	static void endLines();
	static void beginTriangles();
	static void endTriangles();
	static void renderScreen();
	static int getNumVertex();
	static void addVertex(const Ffp::Vertex &vertex);
	static void addVertex(const Ffp::Vertex *vertex, int vertex_size);
	static void addVertex(float x, float y, float z = 0.0f);
	static void setTexCoord(float x, float y, float z = 0.0f, float w = 1.0f);
	static void setColor(float r, float g, float b, float a);
	static void setColor(unsigned int color);
	static int getNumIndices();
	static void addIndex(int index);
	static void addIndices(int i0, int i1);
	static void addIndices(int i0, int i1, int i2);
	static void addIndices(int i0, int i1, int i2, int i3);
	static void addIndices(const unsigned short *indices, int indices_size);
	static void addIndices(const unsigned short *indices, int indices_size, int vertex_offset);
	static void addLines(int num);
	static void addTriangles(int num);
	static void addTriangleQuads(int num);
};

} // namespace Unigine
