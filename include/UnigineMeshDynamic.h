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

#include "UniginePtr.h"
#include "UnigineVector.h"
#include "UnigineStreams.h"

namespace Unigine
{


class UNIGINE_API MeshDynamic : public APIInterface
{
public:

	enum
	{
		IMMUTABLE_VERTEX = 1 << 0,
		IMMUTABLE_INDICES = 1 << 1,
		IMMUTABLE_ALL = (IMMUTABLE_VERTEX | IMMUTABLE_INDICES),
		DYNAMIC_VERTEX = 1 << 2,
		DYNAMIC_INDICES = 1 << 3,
		DYNAMIC_ALL = (DYNAMIC_VERTEX | DYNAMIC_INDICES),
		FLUSH_VERTEX = 1 << 4,
		FLUSH_INDICES = 1 << 5,
		FLUSH_ALL = (FLUSH_VERTEX | FLUSH_INDICES),
	};

	enum
	{
		TYPE_HALF = 0,
		TYPE_FLOAT,
		TYPE_UCHAR,
		TYPE_USHORT,
		NUM_TYPES,
	};

	enum
	{
		MODE_POINTS = 0,
		MODE_LINES,
		MODE_TRIANGLES,
		MODE_TRIANGLE_PATCHES,
		MODE_QUAD_PATCHES,
		NUM_MODES,
	};
	struct Attribute
	{
	int offset;
	int type;
	int size;
	};
	static Ptr<MeshDynamic> create(int flags);
	void setFlags(int flags);
	int getFlags() const;
	void bind();
	void unbind();
	void flushVertex();
	void flushIndices();
	int renderSurface(int mode);
	int renderInstancedSurface(int mode, int num);
	int renderSurface(int mode, int base, int begin, int end);
	int renderInstancedSurface(int mode, int base, int begin, int end, int num);
	int render(int mode, int flush);
	size_t getSystemMemoryUsage() const;
	size_t getVideoMemoryUsage() const;
	void setVertexFormat(const MeshDynamic::Attribute *attributes, int attributes_size);
	int getVertexSize() const;
	int getNumAttributes() const;
	const MeshDynamic::Attribute *getAttributes() const;
	void clearVertex();
	void allocateVertex(int num);
	void removeVertex(int num, int size);
	void setNumVertex(int num);
	int getNumVertex() const;
	const void *getVertex() const;
	void *getVertex();
	void addVertex(const void *vertex);
	void *addVertexArray(int num_vertex);
	void addVertexArray(const void *vertex, int num_vertex);
	void setVertexArray(const void *vertex, int num_vertex);
	Vector<unsigned char> &getVertexArray();
	void addVertexHalf1(int attribute, float x);
	void addVertexHalf2(int attribute, float x, float y);
	void addVertexHalf3(int attribute, float x, float y, float z);
	void addVertexHalf4(int attribute, float x, float y, float z, float w);
	void addVertexFloat(int attribute, const float *value, int value_size);
	void addVertexFloat1(int attribute, float x);
	void addVertexFloat2(int attribute, float x, float y);
	void addVertexFloat3(int attribute, float x, float y, float z);
	void addVertexFloat4(int attribute, float x, float y, float z, float w);
	void addVertexUChar(int attribute, const unsigned char *value, int value_size);
	void addVertexUChar1(int attribute, unsigned char x);
	void addVertexUChar2(int attribute, unsigned char x, unsigned char y);
	void addVertexUChar3(int attribute, unsigned char x, unsigned char y, unsigned char z);
	void addVertexUChar4(int attribute, unsigned char x, unsigned char y, unsigned char z, unsigned char w);
	void addVertexUShort(int attribute, const unsigned short *value, int value_size);
	void addVertexUShort1(int attribute, unsigned short x);
	void addVertexUShort2(int attribute, unsigned short x, unsigned short y);
	void addVertexUShort3(int attribute, unsigned short x, unsigned short y, unsigned short z);
	void addVertexUShort4(int attribute, unsigned short x, unsigned short y, unsigned short z, unsigned short w);
	void setVertexHalf1(int attribute, float x);
	void setVertexHalf2(int attribute, float x, float y);
	void setVertexHalf3(int attribute, float x, float y, float z);
	void setVertexHalf4(int attribute, float x, float y, float z, float w);
	void setVertexFloat(int attribute, const float *value, int value_size);
	void setVertexFloat1(int attribute, float x);
	void setVertexFloat2(int attribute, float x, float y);
	void setVertexFloat3(int attribute, float x, float y, float z);
	void setVertexFloat4(int attribute, float x, float y, float z, float w);
	void setVertexUChar(int attribute, const unsigned char *value, int value_size);
	void setVertexUChar1(int attribute, unsigned char x);
	void setVertexUChar2(int attribute, unsigned char x, unsigned char y);
	void setVertexUChar3(int attribute, unsigned char x, unsigned char y, unsigned char z);
	void setVertexUChar4(int attribute, unsigned char x, unsigned char y, unsigned char z, unsigned char w);
	void setVertexUShort(int attribute, const unsigned short *value, int value_size);
	void setVertexUShort1(int attribute, unsigned short x);
	void setVertexUShort2(int attribute, unsigned short x, unsigned short y);
	void setVertexUShort3(int attribute, unsigned short x, unsigned short y, unsigned short z);
	void setVertexUShort4(int attribute, unsigned short x, unsigned short y, unsigned short z, unsigned short w);
	void setVertexHalf1(int vertex, int attribute, float x);
	void setVertexHalf2(int vertex, int attribute, float x, float y);
	void setVertexHalf3(int vertex, int attribute, float x, float y, float z);
	void setVertexHalf4(int vertex, int attribute, float x, float y, float z, float w);
	void setVertexFloat(int vertex, int attribute, const float *value, int value_size);
	void setVertexFloat1(int vertex, int attribute, float x);
	void setVertexFloat2(int vertex, int attribute, float x, float y);
	void setVertexFloat3(int vertex, int attribute, float x, float y, float z);
	void setVertexFloat4(int vertex, int attribute, float x, float y, float z, float w);
	void setVertexUChar(int vertex, int attribute, const unsigned char *value, int value_size);
	void setVertexUChar1(int vertex, int attribute, unsigned char x);
	void setVertexUChar2(int vertex, int attribute, unsigned char x, unsigned char y);
	void setVertexUChar3(int vertex, int attribute, unsigned char x, unsigned char y, unsigned char z);
	void setVertexUChar4(int vertex, int attribute, unsigned char x, unsigned char y, unsigned char z, unsigned char w);
	void setVertexUShort(int vertex, int attribute, const unsigned short *value, int value_size);
	void setVertexUShort1(int vertex, int attribute, unsigned short x);
	void setVertexUShort2(int vertex, int attribute, unsigned short x, unsigned short y);
	void setVertexUShort3(int vertex, int attribute, unsigned short x, unsigned short y, unsigned short z);
	void setVertexUShort4(int vertex, int attribute, unsigned short x, unsigned short y, unsigned short z, unsigned short w);
	void clearIndices();
	void allocateIndices(int num);
	void removeIndices(int num, int size);
	void setNumIndices(int num);
	int getNumIndices() const;
	const int *getIndices() const;
	int *getIndices();
	void addIndex(int index);
	void addIndexFast(int index);
	void addIndices(int i0, int i1);
	void addIndices(int i0, int i1, int i2);
	void addIndices(int i0, int i1, int i2, int i3);
	int *addIndicesArray(int num_indices);
	void addIndicesArray(const int *indices, int indices_size);
	void setIndicesArray(const int *indices, int indices_size);
	Vector<int> &getIndicesArray();
	void addPoints(int num_points);
	void addLineStrip(int num_vertex);
	void addTriangles(int num_triangles);
	void addTriangleFan(int num_vertex);
	void addTriangleStrip(int num_vertex);
	void addTriangleQuads(int num_quads);
	int saveState(const Ptr<Stream> &stream) const;
	int restoreState(const Ptr<Stream> &stream);
	bool copy(const Ptr<MeshDynamic> &dest) const;
};
typedef Ptr<MeshDynamic> MeshDynamicPtr;

} // namespace Unigine
