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

#include "UnigineImage.h"
#include "UnigineString.h"
#include "UnigineVector.h"

namespace Unigine
{


class UNIGINE_API TilesetFile : public APIInterface
{
public:
	static Ptr<TilesetFile> create();
	int close();
	int isOpened() const;
	int getTileSize() const;
	int load(const char *name);
	long long getOffset(int x, int y) const;
	int hasTile(int x, int y) const;
	int createFile(const char *name, int tile_size);
	int getTile(int x, int y, unsigned char *data);
	int setTile(int x, int y, unsigned char *data, int force = 1);
	int removeTile(int x, int y, int force = 1);
	int flushHeader();
	int getNumTiles() const;
	int getTilePos(int num, int &x, int &y) const;
	Vector<String> getAttributeNames() const;
	int removeAttribute(const char *name, int force = 1);
	int flushAttributes();
	int setAttribute(const char *name, const char *value, int force = 1);
	String getAttribute(const char *name) const;
	int hasAttribute(const char *name) const;
	int setIntAttribute(const char *name, int value, int radix = 10, int force = 1);
	int getIntAttribute(const char *name) const;
	int setFloatAttribute(const char *name, float value, int force = 1);
	float getFloatAttribute(const char *name) const;
	int setDoubleAttribute(const char *name, double value, int force = 1);
	double getDoubleAttribute(const char *name) const;
};
typedef Ptr<TilesetFile> TilesetFilePtr;

} // namespace Unigine
