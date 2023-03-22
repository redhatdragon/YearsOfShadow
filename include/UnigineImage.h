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
#include "UniginePtr.h"
#include "UnigineStreams.h"
#include "UnigineJson.h"
#include "UnigineCallback.h"

namespace Unigine
{

class Blob;
class Json;

class UNIGINE_API Image : public APIInterface
{
public:
	static Ptr<Image> create();
	static Ptr<Image> create(const char *name);
	static Ptr<Image> create(const Ptr<Image> &image);
	union Pixeli
	{
		struct
		{
			int r, g, b, a;
		};
		int v[4];
		#ifdef USE_SSE
			__m128i vec;
		#endif
	};

	// float pixel
	union Pixelf
	{
		struct
		{
			float r, g, b, a;
		};
		float v[4];
		#ifdef USE_SSE
			__m128 vec;
		#endif
	};

	// pixel
	struct alignas(16) Pixel
	{
		UNIGINE_INLINE Pixel() {}
		UNIGINE_INLINE Pixel(int r, int g = 0, int b = 0, int a = 0)
		{
			i.r = r;
			i.g = g;
			i.b = b;
			i.a = a;
		}
		UNIGINE_INLINE Pixel(float r, float g = 0.0f, float b = 0.0f, float a = 0.0f)
		{
			f.r = r;
			f.g = g;
			f.b = b;
			f.a = a;
		}
		union
		{
			Pixeli i;
			Pixelf f;
		};
	};

	enum
	{
		AUTO_IMAGE_TYPE = -1,
		IMAGE_2D = 0,	// 2D image
		IMAGE_3D,	// 3D image
		IMAGE_CUBE,	// Cube image
		IMAGE_2D_ARRAY,	// 2D Array image
		IMAGE_CUBE_ARRAY,	// Cube Array image
	};

	enum
	{
		AUTO_FORMAT = -1,
		FORMAT_R8 = 0,	// 8-bits per channel integer formats
		FORMAT_RG8,
		FORMAT_RGB8,
		FORMAT_RGBA8,
		FORMAT_R16,	// 16-bits per channel integer formats
		FORMAT_RG16,
		FORMAT_RGB16,
		FORMAT_RGBA16,
		FORMAT_R16F,	// 16-bits per channel float formats
		FORMAT_RG16F,
		FORMAT_RGB16F,
		FORMAT_RGBA16F,
		FORMAT_R32F,	// 32-bits per channel float formats
		FORMAT_RG32F,
		FORMAT_RGB32F,
		FORMAT_RGBA32F,
		FORMAT_RGB565,	// combined formats
		FORMAT_RGBA4,
		FORMAT_RGB5A1,
		FORMAT_RGB10A2,
		FORMAT_RG11B10F,
		FORMAT_DXT1,	// compressed formats
		FORMAT_DXT3,
		FORMAT_DXT5,
		FORMAT_ATI1,
		FORMAT_ATI2,
	};

	enum FILE_COMPRESSION
	{
		FILE_COMPRESSION_DISABLE,
		FILE_COMPRESSION_ZLC_LOW,
		FILE_COMPRESSION_ZLC_HIGH,
		FILE_COMPRESSION_LZ4_LOW,
		FILE_COMPRESSION_LZ4_HIGH,
	};

	enum FILTER
	{
		FILTER_POINT = 0,
		FILTER_LINEAR,
		FILTER_MIN,
	};
	bool create2D(int width, int height, int format, int num_mipmaps = 1, bool clear = true, bool allocate = true);
	bool create3D(int width, int height, int depth, int format, int num_mipmaps = 1, bool clear = true, bool allocate = true);
	bool createCube(int width, int height, int format, int num_mipmaps = 1, bool clear = true, bool allocate = true);
	bool create2DArray(int width, int height, int num_layers, int format, int num_mipmaps = 1, bool clear = true, bool allocate = true);
	bool createCubeArray(int width, int height, int num_layers, int format, int num_mipmaps = 1, bool clear = true, bool allocate = true);
	void create3DTurbulence(int size, float scale, int frequency, int seed);
	bool allocData(bool clear = true);
	void clearData();
	bool info(const char *file);
	bool load(const char *file);
	bool load(const char *file, int mip_offset);
	bool save(const char *file, float quality) const;
	bool save(const char *file) const;
	bool save(const Ptr<Stream> &stream) const;
	bool load(const Ptr<Stream> &stream);
	bool saveDDS(const Ptr<Stream> &stream) const;
	bool loadDDS(const Ptr<Stream> &stream);
	static Vector<String> getSupportedExtensions();
	static bool isSupportedExtension(const char *ext);
	static bool isSupported(const char *path);
	void clear();
	bool isLoaded() const;
	int getType() const;
	const char *getTypeName() const;
	static const char *getTypeName(int type);
	bool is2DType() const;
	bool is3DType() const;
	bool isCubeType() const;
	bool isArrayType() const;
	int getFormat() const;
	const char *getFormatName() const;
	static const char *getFormatName(int format);
	static int getFormat(int num_channels, int format);
	static int getRawFormat(int format);
	static bool isRawFormat(int format);
	bool isRawFormat() const;
	static bool isUCharFormat(int format);
	bool isUCharFormat() const;
	static bool isUShortFormat(int format);
	bool isUShortFormat() const;
	static bool isHalfFormat(int format);
	bool isHalfFormat() const;
	static bool isFloatFormat(int format);
	bool isFloatFormat() const;
	static bool isCombinedFormat(int format);
	bool isCombinedFormat() const;
	static bool isCompressedFormat(int format);
	bool isCompressedFormat() const;
	static int getNumChannels(int format);
	int getNumChannels() const;
	static int getPixelSize(int format);
	int getPixelSize() const;
	static size_t getPixelsSize(int format, int w, int h);
	size_t getPixelsSize() const;
	static int getBlockSize(int format);
	int getBlockSize() const;
	void setFileCompression(Image::FILE_COMPRESSION compression);
	Image::FILE_COMPRESSION getFileCompression() const;
	int getWidth() const;
	int getWidth(int level) const;
	int getHeight() const;
	int getHeight(int level) const;
	int getDepth() const;
	int getDepth(int level) const;
	bool hasMipmaps() const;
	int getNumMipmaps() const;
	int getNumLayers() const;
	int getNumFaces() const;
	size_t getNumPixels() const;
	size_t getOffset(int level) const;
	size_t getSize() const;
	size_t getSize(int level) const;
	size_t getStride(int level = 0) const;
	size_t getDepthStride(int level = 0) const;
	bool setChannelInt(int channel, int value);
	bool setChannelFloat(int channel, float value);
	bool swapChannels(int channel, int index);
	void assignFrom(const Ptr<Image> &src_image);
	bool resizeFrom(const Ptr<Image> &src_image, Image::FILTER filter = Image::FILTER_LINEAR);
	bool copy(const Ptr<Image> &src_image, int layer);
	bool copy(const Ptr<Image> &src_image, int dest_channel, int src_channel);
	bool copy(const Ptr<Image> &src_image, int x_dst, int y_dst, int x_src, int y_src, int width, int height, bool safe = false);
	bool copyFast(const Ptr<Image> &src_image, size_t x_dst, size_t y_dst, size_t x_src, size_t y_src, size_t w, size_t h);
	bool blend(const Ptr<Image> &image, int x0, int y0, int x1, int y1, int width, int height, float scale = 1.0f, bool safe = false);
	bool compare(const Ptr<Image> &image, int x0, int y0, int x1, int y1, int width, int height, int threshold = 0);
	bool resize(int new_width, int new_height, Image::FILTER filter = Image::FILTER_LINEAR);
	bool rotate(int angle);
	bool blur(int size);
	bool extrude(int pixels);
	bool normalize();
	bool flipX();
	bool flipY();
	bool flipCubemapX();
	bool flipCubemapY();
	bool flipCubemapZ();
	bool sign();
	bool invert();
	bool invertChannel(unsigned char channel);
	void calcRange(Math::dvec2 &range);
	void calcRange(Math::dvec2 &r, Math::dvec2 &g, Math::dvec2 &b, Math::dvec2 &a);
	void changeRange(const Math::dvec4 &range);
	void changeRange(const Math::dvec4 &range_r, const Math::dvec4 &range_g, const Math::dvec4 &range_b, const Math::dvec4 &range_a);
	void normalizeRange(bool per_cahnnel);
	bool createMipmaps(Image::FILTER filter = Image::FILTER_LINEAR, float gamma = 1.0f);
	bool removeMipmaps();
	bool combineMipmaps();
	bool decombineMipmaps();
	int convertToType(int type);
	int convertToFormat(int new_format);
	bool decombine();
	bool combine(int new_format = -1);
	bool decompress();
	bool compress(int new_format = -1);
	void swap(const Ptr<Image> &image);
	static Image::Pixel toPixel(int format, const Math::vec4 &color);
	Image::Pixel toPixel(const Math::vec4 &color);
	static Math::vec4 toVec4(int format, const Image::Pixel &p);
	Math::vec4 toVec4(const Image::Pixel &pixel);
	void get(int id, Math::dvec4 &value) const;
	void set(int id, const Math::dvec4 &value);
	Math::vec4 get(const Math::ivec2 &coord, int offset) const;
	void get(const Math::ivec2 &coord, int offset, Math::vec4 &p00, Math::vec4 &p01, Math::vec4 &p10, Math::vec4 &p11) const;
	void get8F(const Math::ivec2 &coord, Math::vec4 &p) const;
	float get8F(const Math::ivec2 &coord) const;
	void get16F(const Math::ivec2 &coord, Math::vec4 &p) const;
	float get16F(const Math::ivec2 &coord) const;
	void get32F(const Math::ivec2 &coord, Math::vec4 &p) const;
	float get32F(const Math::ivec2 &coord) const;
	void set2D(int x, int y, const Image::Pixel &p);
	void set2D(const Math::ivec2 &coord, const Image::Pixel &p);
	Image::Pixel get2D(int x, int y) const;
	Image::Pixel get2D(const Math::ivec2 &coord) const;
	Image::Pixel get2D(float x, float y) const;
	Image::Pixel get2D(const Math::vec2 &uv) const;
	Image::Pixel get2D(float x, float y, const Image::Pixel &skip_pixel) const;
	Image::Pixel get2D(const Math::vec2 &uv, const Image::Pixel &skip_pixel) const;
	void set3D(int x, int y, int z, const Image::Pixel &p);
	void set3D(const Math::ivec3 &coord, const Image::Pixel &p);
	Image::Pixel get3D(int x, int y, int z) const;
	Image::Pixel get3D(const Math::ivec3 &coord) const;
	Image::Pixel get3D(float x, float y, float z) const;
	Image::Pixel get3D(const Math::vec3 &uvw) const;
	Image::Pixel get3DSmooth(float x, float y, float z) const;
	Image::Pixel get3DSmooth(const Math::vec3 &uvw) const;
	void setCube(int x, int y, int face, const Image::Pixel &p);
	Image::Pixel getCube(int x, int y, int face) const;
	Image::Pixel getCube(const Math::ivec2 &coord, int face) const;
	Image::Pixel getCube(float x, float y, int face) const;
	Image::Pixel getCube(const Math::vec2 &uv, int face) const;
	Image::Pixel getCube(const Math::vec3 &direction) const;
	void set2DArray(int x, int y, int layer, const Image::Pixel &p);
	void set2DArray(const Math::ivec2 &coord, int layer, const Image::Pixel &p);
	Image::Pixel get2DArray(int x, int y, int layer) const;
	Image::Pixel get2DArray(const Math::ivec2 &coord, int layer) const;
	Image::Pixel get2DArray(float x, float y, int layer) const;
	Image::Pixel get2DArray(const Math::vec2 &uv, int layer) const;
	void setCubeArray(int x, int y, int face, int layer, const Image::Pixel &p);
	void setCubeArray(const Math::ivec2 &coord, int face, int layer, const Image::Pixel &p);
	Image::Pixel getCubeArray(int v1, int v2, int face, int layer) const;
	Image::Pixel getCubeArray(const Math::ivec2 &coord, int face, int layer) const;
	Image::Pixel getCubeArray(float v1, float v2, int face, int layer) const;
	Image::Pixel getCubeArray(const Math::vec2 &uv, int face, int layer) const;
	Image::Pixel getCubeArray(const Math::vec3 &direction, int layer) const;
	unsigned char * getPixels() const;
	unsigned char * getPixels2D(int level = 0) const;
	unsigned char * getPixels3D(int level = 0) const;
	unsigned char * getPixelsCube(int face, int level = 0) const;
	unsigned char * getPixels2DArray(int layer, int level = 0) const;
	unsigned char * getPixelsCubeArray(int face, int layer, int level = 0) const;
	void setPixels(unsigned char *pixels);
	void getPixels(const Ptr<Blob> &blob) const;
	int setPixels(const Ptr<Blob> &blob);
};
typedef Ptr<Image> ImagePtr;


class UNIGINE_API ImageConverter : public APIInterface
{
public:
	static Ptr<ImageConverter> create();
	static Ptr<ImageConverter> create(const Ptr<ImageConverter> &imageconverter);

	enum MIPMAPS_MODE
	{
		MIPMAPS_MODE_DISABLE = 0,
		MIPMAPS_MODE_POINT,
		MIPMAPS_MODE_LINEAR,
		MIPMAPS_MODE_COMBINED,
		MIPMAPS_MODE_GGX,
	};

	enum RESOLUTION_MODE
	{
		RESOLUTION_MODE_AUTO = 0,
		RESOLUTION_MODE_ORIGINAL,
		RESOLUTION_MODE_MANUAL,
	};

	enum COMPRESSION_QUALITY
	{
		COMPRESSION_QUALITY_LOW = 0,
		COMPRESSION_QUALITY_HIGH,
	};

	enum RESIZE_FILTER
	{
		RESIZE_FILTER_POINT = 0,
		RESIZE_FILTER_LINEAR,
		RESIZE_FILTER_MIN,
	};

	enum RANGE_MODE
	{
		RANGE_MODE_DISABLE = 0,
		RANGE_MODE_NORMALIZE,
		RANGE_MODE_NORMALIZE_PER_CHANNEL,
		RANGE_MODE_MANUAL,
	};
	void setType(int type);
	int getType() const;
	void setFormat(int format);
	int getFormat() const;
	void setResolutionMode(ImageConverter::RESOLUTION_MODE mode);
	ImageConverter::RESOLUTION_MODE getResolutionMode() const;
	void setResizeFilter(ImageConverter::RESIZE_FILTER filter);
	ImageConverter::RESIZE_FILTER getResizeFilter() const;
	void setWidth(int width);
	int getWidth() const;
	void setHeight(int height);
	int getHeight() const;
	void setDepth(int depth);
	int getDepth() const;
	void setFileCompression(Image::FILE_COMPRESSION compression);
	Image::FILE_COMPRESSION getFileCompression() const;
	void setCompressionQuality(ImageConverter::COMPRESSION_QUALITY quality);
	ImageConverter::COMPRESSION_QUALITY getCompressionQuality() const;
	void setGPUCompression(bool compression);
	bool isGPUCompression() const;
	void setMipmapsMode(ImageConverter::MIPMAPS_MODE mode);
	ImageConverter::MIPMAPS_MODE getMipmapsMode() const;
	void setMipmapsGamma(float gamma);
	float getMipmapsGamma() const;
	void setGGXMipmapsQuality(float quality);
	float getGGXMipmapsQuality() const;
	void setRangeMode(ImageConverter::RANGE_MODE mode);
	ImageConverter::RANGE_MODE getRangeMode() const;
	Math::dvec4 getRange(int channel) const;
	void setRange(int channel, const Math::dvec4 &value);
	void setFlipX(bool flipx);
	bool isFlipX() const;
	void setFlipY(bool flipy);
	bool isFlipY() const;
	void setBlur(int blur);
	int getBlur() const;
	void setRotate(int rotate);
	int getRotate() const;
	bool getInvertChannel(int channel);
	void setInvertChannel(int channel, bool value);
	void reset();
	void copy(Ptr<ImageConverter> &converter) const;
	Ptr<ImageConverter> clone() const;
	bool load(const char *path);
	bool save(const char *path) const;
	void load(const Ptr<Json> &json);
	void save(const Ptr<Json> &json) const;
	void updateParameters(const Ptr<Image> &image);
	bool run(Ptr<Image> &image);
	void *addBeginCallback(CallbackBase2<Ptr<ImageConverter>, Ptr<Image>> *func);
	bool removeBeginCallback(void *id);
	void clearBeginCallbacks();
	void *addEndCallback(CallbackBase2<Ptr<ImageConverter>, Ptr<Image>> *func);
	bool removeEndCallback(void *id);
	void clearEndCallbacks();
};
typedef Ptr<ImageConverter> ImageConverterPtr;

} // namespace Unigine
