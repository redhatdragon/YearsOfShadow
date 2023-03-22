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
#include "UnigineMathLib.h"
#include "UnigineImage.h"

namespace Unigine
{


class UNIGINE_API Tileset : public APIInterface
{
public:

	enum
	{
		TILE_STATE_NONE = 0,
		TILE_STATE_MODIFIED,
		TILE_STATE_REMOVED,
	};
	Math::ivec2 getTileCoord(const Math::dvec2 &flat_position) const;
	Math::vec2 getTilePixelCoord(const Math::dvec2 &flat_position, int use_half_pixel_offset = 0) const;
	Math::dvec2 getTileFlatPosition(const Math::ivec2 &tile_coord) const;
	Math::dvec2 getTilePixelFlatPosition(const Math::ivec2 &tile_coord, const Math::vec2 &pixel_coord, int use_half_pixel_offset = 0) const;
	int getTileDataState(const Math::ivec2 &tile_coord);
	int getTileMaskState(const Math::ivec2 &tile_coord);
	int isTileDataLoaded(const Math::ivec2 &tile_coord);
	int isTileMaskLoaded(const Math::ivec2 &tile_coord);
	int hasTileFile(const Math::ivec2 &tile_coord);
	int hasTileFileMask(const Math::ivec2 &tile_coord);
	int hasTile(const Math::ivec2 &tile_coord);
	int hasTileMask(const Math::ivec2 &tile_coord);
	int getTileData(const Math::ivec2 &tile_coord, Ptr<Image> &data, int force = 1);
	int setTileData(const Math::ivec2 &tile_coord, const Ptr<Image> &data);
	int getTileMask(const Math::ivec2 &tile_coord, Ptr<Image> &mask, int force = 1);
	int setTileMask(const Math::ivec2 &tile_coord, const Ptr<Image> &mask);
	int removeTile(const Math::ivec2 &tile_coord);
	int removeTileMask(const Math::ivec2 &tile_coord);
	int reloadTileData(const Math::ivec2 &tile_coord);
	int reloadTileMask(const Math::ivec2 &tile_coord);
	int saveTile(const Math::ivec2 &tile_coord, int force = 0);
	int saveAll(int force = 1);
	int setLifeTime(const Math::ivec2 &tile_coord, float seconds);
	float getLifeTime(const Math::ivec2 &tile_coord);
	int getTileResolution() const;
	float getTileDensity() const;
	float getTileSize() const;
	int getImageType() const;
	int getDataFormat() const;
	int getMaskFormat() const;
	float fetchData(const Math::Vec2 &position, Math::vec4 &result, int layer, bool force);
	const char *getPath() const;
};
typedef Ptr<Tileset> TilesetPtr;

} // namespace Unigine
