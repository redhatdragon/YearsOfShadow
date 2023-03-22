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

#include <UnigineMathLib.h>
#include <UnigineObjects.h>

#include "UnigineEditorGlobal.h"
#include "UnigineCollection.h"

namespace UnigineEditor
{


class UNIGINE_EDITOR_API LandscapeMapImportParameters : public Unigine::APIInterface
{
public:

	enum DATA_FILLING_MODE
	{
		DATA_FILLING_MODE_MANUAL = 0,
		DATA_FILLING_MODE_FROM_TILESET,
	};

	enum DATA_SOURCES_TYPE
	{
		DATA_SOURCES_TYPE_ASSETS = 0,
		DATA_SOURCES_TYPE_EXTERNAL_FILES,
	};

	enum RESOLUTION_TYPE
	{
		RESOLUTION_TYPE_AUTO = 0,
		RESOLUTION_TYPE_MANUAL,
	};

	enum PATTERN_MATCHING_MODE
	{
		PATTERN_MATCHING_MODE_AUTO = 0,
		PATTERN_MATCHING_MODE_MANUAL,
	};

	enum NAMING_MODE
	{
		NAMING_MODE_ROW_COLUMN = 0,
		NAMING_MODE_INDEXED,
	};

	enum ORDER
	{
		ORDER_DIRECT = 0,
		ORDER_INVERSE,
	};

	enum COMPRESSION_TYPE
	{
		COMPRESSION_TYPE_OFF = 0,
		COMPRESSION_TYPE_LOSSLESS,
		COMPRESSION_TYPE_LOSSY,
		COMPRESSION_TYPE_CUSTOM,
	};

	enum COMPRESSION_METHOD
	{
		COMPRESSION_METHOD_JACKALLESS = Unigine::Landscape::COMPRESSOR_TYPE_JACKALLESS,
		COMPRESSION_METHOD_LZ4 = Unigine::Landscape::COMPRESSOR_TYPE_LZ4,
		COMPRESSION_METHOD_ZLIB = Unigine::Landscape::COMPRESSOR_TYPE_ZLIB,
	};

	enum DATA_TYPE
	{
		DATA_TYPE_NONE = 0,
		DATA_TYPE_ONLY_DATA,
		DATA_TYPE_DATA_WITH_OPACITY,
	};

	enum BLENDING_MODE
	{
		BLENDING_MODE_ALPHA_BLEND = Unigine::Landscape::ALPHA_BLEND,
		BLENDING_MODE_ADDITIVE = Unigine::Landscape::ADDITIVE,
		BLENDING_MODE_OVERLAY = Unigine::Landscape::OVERLAY,
		BLENDING_MODE_MULTIPLICATIVE = Unigine::Landscape::MULTIPLICATIVE,
	};

	enum HEIGHTMAP_VALUES_TYPE
	{
		HEIGHTMAP_VALUES_TYPE_NORMALIZED = 0,
		HEIGHTMAP_VALUES_TYPE_UNNORMALIZED,
	};

	enum CHANNEL
	{
		CHANNEL_R = 0,
		CHANNEL_G,
		CHANNEL_B,
		CHANNEL_A,
	};

	enum
	{
		NUM_MASKS = 20,
	};
	static Unigine::Ptr<LandscapeMapImportParameters> create();
	void load(const Unigine::Ptr<Collection> &parameters);
	Unigine::Ptr<Collection> save() const;
	void setForceGenerate(bool generate);
	bool isForceGenerate() const;
	void setDataFillingMode(LandscapeMapImportParameters::DATA_FILLING_MODE mode);
	LandscapeMapImportParameters::DATA_FILLING_MODE getDataFillingMode() const;
	void setDataSourcesType(LandscapeMapImportParameters::DATA_SOURCES_TYPE type);
	LandscapeMapImportParameters::DATA_SOURCES_TYPE getDataSourcesType() const;
	void setResolutionType(LandscapeMapImportParameters::RESOLUTION_TYPE type);
	LandscapeMapImportParameters::RESOLUTION_TYPE getResolutionType() const;
	void setResolution(const Unigine::Math::ivec2 &resolution);
	Unigine::Math::ivec2 getResolution() const;
	void setPatternMatchingMode(LandscapeMapImportParameters::PATTERN_MATCHING_MODE mode);
	LandscapeMapImportParameters::PATTERN_MATCHING_MODE getPatternMatchingMode() const;
	void setFlipY(bool flipy);
	bool isFlipY() const;
	void setNamingMode(LandscapeMapImportParameters::NAMING_MODE mode);
	LandscapeMapImportParameters::NAMING_MODE getNamingMode() const;
	void setHorizontalOrder(LandscapeMapImportParameters::ORDER order);
	LandscapeMapImportParameters::ORDER getHorizontalOrder() const;
	void setVerticalOrder(LandscapeMapImportParameters::ORDER order);
	LandscapeMapImportParameters::ORDER getVerticalOrder() const;
	void setOrderDirection(LandscapeMapImportParameters::ORDER direction);
	LandscapeMapImportParameters::ORDER getOrderDirection() const;
	void setNumColumns(int columns);
	int getNumColumns() const;
	void setPattern(const char *pattern);
	const char *getPattern() const;
	void setCompressionType(LandscapeMapImportParameters::COMPRESSION_TYPE type);
	LandscapeMapImportParameters::COMPRESSION_TYPE getCompressionType() const;
	void setCompressionMethod(LandscapeMapImportParameters::COMPRESSION_METHOD method);
	LandscapeMapImportParameters::COMPRESSION_METHOD getCompressionMethod() const;
	void setHeightmapDataType(LandscapeMapImportParameters::DATA_TYPE type);
	LandscapeMapImportParameters::DATA_TYPE getHeightmapDataType() const;
	void setHeightmapBlendingMode(LandscapeMapImportParameters::BLENDING_MODE mode);
	LandscapeMapImportParameters::BLENDING_MODE getHeightmapBlendingMode() const;
	void setHeightmapValuesType(LandscapeMapImportParameters::HEIGHTMAP_VALUES_TYPE type);
	LandscapeMapImportParameters::HEIGHTMAP_VALUES_TYPE getHeightmapValuesType() const;
	void setHeightmapPath(const char *path);
	const char *getHeightmapPath() const;
	void setHeightmapOpacityPath(const char *path);
	const char *getHeightmapOpacityPath() const;
	void setHeightmapOpacityChannel(LandscapeMapImportParameters::CHANNEL channel);
	LandscapeMapImportParameters::CHANNEL getHeightmapOpacityChannel() const;
	void setHeightmapNormalizationLowerLimit(int limit);
	int getHeightmapNormalizationLowerLimit() const;
	void setHeightmapNormalizationUpperLimit(int limit);
	int getHeightmapNormalizationUpperLimit() const;
	void setHeightmapFadeAttenuation(const Unigine::Math::vec2 &attenuation);
	Unigine::Math::vec2 getHeightmapFadeAttenuation() const;
	void setHeightmapOpacityLossyCompression(bool compression);
	bool isHeightmapOpacityLossyCompression() const;
	void setAlbedoLossyCompression(bool compression);
	bool isAlbedoLossyCompression() const;
	void setAlbedoBlendingMode(LandscapeMapImportParameters::BLENDING_MODE mode);
	LandscapeMapImportParameters::BLENDING_MODE getAlbedoBlendingMode() const;
	void setAlbedoDataType(LandscapeMapImportParameters::DATA_TYPE type);
	LandscapeMapImportParameters::DATA_TYPE getAlbedoDataType() const;
	void setAlbedoPath(const char *path);
	const char *getAlbedoPath() const;
	void setAlbedoOpacityPath(const char *path);
	const char *getAlbedoOpacityPath() const;
	void setAlbedoFadeAttenuation(const Unigine::Math::vec2 &attenuation);
	Unigine::Math::vec2 getAlbedoFadeAttenuation() const;
	void setAlbedoOpacityChannel(LandscapeMapImportParameters::CHANNEL channel);
	LandscapeMapImportParameters::CHANNEL getAlbedoOpacityChannel() const;
	void setMasksLossyCompression(bool compression);
	bool isMasksLossyCompression() const;
	void setMaskDataType(int mask_index, LandscapeMapImportParameters::DATA_TYPE type);
	LandscapeMapImportParameters::DATA_TYPE getMaskDataType(int mask_index) const;
	void setMaskName(int mask_index, const char *name);
	const char *getMaskName(int mask_index) const;
	void setMaskColorPath(int mask_index, const char *path);
	const char *getMaskColorPath(int mask_index) const;
	void setMaskOpacityPath(int mask_index, const char *path);
	const char *getMaskOpacityPath(int mask_index) const;
	void setMaskFadeAttenuation(int mask_index, const Unigine::Math::vec2 &value);
	Unigine::Math::vec2 getMaskFadeAttenuation(int mask_index) const;
	void setMaskColorChannel(int mask_index, LandscapeMapImportParameters::CHANNEL channel);
	LandscapeMapImportParameters::CHANNEL getMaskColorChannel(int mask_index) const;
	void setMaskOpacityChannel(int mask_index, LandscapeMapImportParameters::CHANNEL channel);
	LandscapeMapImportParameters::CHANNEL getMaskOpacityChannel(int mask_index) const;
	void setMaskBlendingMode(int mask_index, LandscapeMapImportParameters::BLENDING_MODE type);
	LandscapeMapImportParameters::BLENDING_MODE getMaskBlendingMode(int mask_index) const;
};
typedef Unigine::Ptr<LandscapeMapImportParameters> LandscapeMapImportParametersPtr;

} // namespace UnigineEditor
