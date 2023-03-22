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
#include "UnigineCurve2d.h"

namespace Unigine
{


class UNIGINE_API Texture : public APIInterface
{
public:

	enum
	{
		TEXTURE_2D = 0,	// 2D texture
		TEXTURE_3D,	// 3D texture
		TEXTURE_CUBE,	// Cube texture
		TEXTURE_2D_ARRAY,	// 2D Array texture
		TEXTURE_CUBE_ARRAY,	// Cube Array texture
	};

	enum
	{
		// 8-bits per channel normalized formats
		FORMAT_R8 = 0,
		FORMAT_RG8,
		FORMAT_RGB8,
		FORMAT_RGBA8,
		// 16-bits per channel normalized formats
		FORMAT_R16,
		FORMAT_RG16,
		FORMAT_RGB16,
		FORMAT_RGBA16,
		// 16-bits per channel integer formats
		FORMAT_R16U,
		FORMAT_RG16U,
		FORMAT_RGB16U,
		FORMAT_RGBA16U,
		// 32-bits per channel integer formats
		FORMAT_R32U,
		FORMAT_RG32U,
		FORMAT_RGB32U,
		FORMAT_RGBA32U,
		// 16-bits per channel float formats
		FORMAT_R16F,
		FORMAT_RG16F,
		FORMAT_RGB16F,
		FORMAT_RGBA16F,
		// 32-bits per channel float formats
		FORMAT_R32F,
		FORMAT_RG32F,
		FORMAT_RGB32F,
		FORMAT_RGBA32F,
		// combined formats
		FORMAT_RGB565,
		FORMAT_RGBA4,
		FORMAT_RGB5A1,
		FORMAT_RGB10A2,
		FORMAT_RG11B10F,
		FORMAT_R24B8,
		// compressed formats
		FORMAT_DXT1,
		FORMAT_DXT3,
		FORMAT_DXT5,
		FORMAT_ATI1,
		FORMAT_ATI2,
		// depth formats
		FORMAT_D16,
		FORMAT_D24,
		FORMAT_D24S8,
		FORMAT_D32F,
		FORMAT_D32FS8,
		NUM_FORMATS,
	};

	enum
	{
		// format
		FORMAT_SRGB = 1 << 0,
		FORMAT_SIGNED = 1 << 1,
		FORMAT_INTEGER = 1 << 2,
		FORMAT_MIPMAPS = 1 << 3,
		FORMAT_MASK = FORMAT_SRGB | FORMAT_SIGNED | FORMAT_INTEGER | FORMAT_MIPMAPS,
		// multisample
		FORMAT_MULTISAMPLE_2 = 1 << 4,
		FORMAT_MULTISAMPLE_4 = 1 << 5,
		FORMAT_MULTISAMPLE_8 = 1 << 6,
		FORMAT_MULTISAMPLE_16 = 1 << 7,
		FORMAT_MULTISAMPLE_MASK = FORMAT_MULTISAMPLE_2 | FORMAT_MULTISAMPLE_4 | FORMAT_MULTISAMPLE_8 | FORMAT_MULTISAMPLE_16,
		// usage flag
		FORMAT_USAGE_UNORDERED_ACCESS = 1 << 8,
		FORMAT_USAGE_RENDER = 1 << 9,
		FORMAT_USAGE_IMMUTABLE = 1 << 10,
		FORMAT_USAGE_DYNAMIC = 1 << 11,
		FORMAT_USAGE_STAGING = 1 << 12,
		FORMAT_USAGE_MASK = FORMAT_USAGE_UNORDERED_ACCESS | FORMAT_USAGE_RENDER | FORMAT_USAGE_IMMUTABLE | FORMAT_USAGE_DYNAMIC | FORMAT_USAGE_STAGING,
		FORMAT_FLAGS = FORMAT_MASK | FORMAT_MULTISAMPLE_MASK | FORMAT_USAGE_MASK,
	};

	enum
	{
		// wrapping
		SAMPLER_WRAP_CLAMP_X = 1 << 13,
		SAMPLER_WRAP_CLAMP_Y = 1 << 14,
		SAMPLER_WRAP_CLAMP_Z = 1 << 15,
		SAMPLER_WRAP_CLAMP = SAMPLER_WRAP_CLAMP_X | SAMPLER_WRAP_CLAMP_Y | SAMPLER_WRAP_CLAMP_Z,
		SAMPLER_WRAP_BORDER_X = 1 << 16,
		SAMPLER_WRAP_BORDER_Y = 1 << 17,
		SAMPLER_WRAP_BORDER_Z = 1 << 18,
		SAMPLER_WRAP_BORDER_ONE = 1 << 19,
		SAMPLER_WRAP_BORDER = SAMPLER_WRAP_BORDER_X | SAMPLER_WRAP_BORDER_Y | SAMPLER_WRAP_BORDER_Z,
		SAMPLER_WRAP_MASK = SAMPLER_WRAP_CLAMP | SAMPLER_WRAP_BORDER | SAMPLER_WRAP_BORDER_ONE,
		// filter
		SAMPLER_FILTER_POINT = 1 << 20,
		SAMPLER_FILTER_LINEAR = 1 << 21,
		SAMPLER_FILTER_BILINEAR = 1 << 22,
		SAMPLER_FILTER_TRILINEAR = 1 << 23,
		SAMPLER_FILTER_MASK = SAMPLER_FILTER_POINT | SAMPLER_FILTER_LINEAR | SAMPLER_FILTER_BILINEAR | SAMPLER_FILTER_TRILINEAR,
		// anisotropy
		SAMPLER_ANISOTROPY_1 = 1 << 24,
		SAMPLER_ANISOTROPY_2 = 1 << 25,
		SAMPLER_ANISOTROPY_4 = 1 << 26,
		SAMPLER_ANISOTROPY_8 = 1 << 27,
		SAMPLER_ANISOTROPY_16 = 1 << 28,
		SAMPLER_ANISOTROPY_MASK = SAMPLER_ANISOTROPY_1 | SAMPLER_ANISOTROPY_2 | SAMPLER_ANISOTROPY_4 | SAMPLER_ANISOTROPY_8 | SAMPLER_ANISOTROPY_16,
		// shadow
		SAMPLER_SHADOW_COMPARE = 1 << 29,
		SAMPLER_SHADOW_MASK = SAMPLER_SHADOW_COMPARE,
		SAMPLER_FLAGS = SAMPLER_WRAP_MASK | SAMPLER_FILTER_MASK | SAMPLER_ANISOTROPY_MASK | SAMPLER_SHADOW_MASK,
	};
	static Ptr<Texture> create();
	bool create(const Ptr<Image> &image, int flags = 1<<21);
	bool create2D(int width, int height, int format, int flags = 1<<21);
	bool create3D(int width, int height, int depth, int format, int flags = 1<<21);
	bool createCube(int width, int height, int format, int flags = 1<<21);
	bool create2DArray(int width, int height, int num_layers, int format, int flags = 1<<21);
	bool createCubeArray(int width, int height, int num_layers, int format, int flags = 1<<21);
	int fromGLTexture2D(unsigned int tex_id, int width, int height, int format, int flags);
	int fromGLTexture2D(unsigned int tex_id, int width, int height, int format, int internal_format, int flags);
	int fromD3D11Texture2D(void *ptr, int width, int height, int format, int flags);
	int fromD3D11Texture2D(void *ptr, int width, int height, int format, int internal_format, int flags);
	bool load(const char *name, int flags = 1<<21);
	bool setImage(const Ptr<Image> &image);
	bool setImage2D(const Ptr<Image> &image, int offset_x, int offset_y, int texture_format = -1);
	bool setImageLayer(const Ptr<Image> &image, int layer, int texture_format = -1);
	bool getImage(const Ptr<Image> &image) const;
	bool setBlob(const Ptr<Blob> &blob) const;
	void clear();
	void destroy();
	bool isValid();
	void setOwnership(bool ownership);
	bool isOwnership() const;
	int getType() const;
	const char *getTypeName() const;
	bool is2DType() const;
	bool is3DType() const;
	bool isCubeType() const;
	bool isArrayType() const;
	static int formatImageToTexture(int image_format, int flags);
	static int formatTextureToImage(int texture_format);
	static int typeTextureToImage(int texture_type);
	static size_t getVideoMemoryUsage(int width, int height, int format, int num_mipmaps, int flags, int num_faces, int num_layers, int depth);
	int getFormat() const;
	const char *getFormatName() const;
	int getImageFormat() const;
	bool isRawFormat() const;
	bool isUCharFormat() const;
	bool isUShortFormat() const;
	bool isUIntFormat() const;
	bool isIntegerFormat() const;
	bool isHalfFormat() const;
	bool isFloatFormat() const;
	bool isCombinedFormat() const;
	bool isCompressedFormat() const;
	bool isColorFormat() const;
	bool isDepthFormat() const;
	int getNumChannels() const;
	int getWidth(int level = 0) const;
	int getHeight(int level = 0) const;
	int getDepth(int level = 0) const;
	int getNumMipmaps() const;
	int getNumLayers() const;
	int getNumFaces() const;
	void setSamplerFlags(int flags);
	int getSamplerFlags() const;
	int getFormatFlags() const;
	int getAllFlags() const;
	bool isAnisotropy() const;
	bool getMultisample() const;
	bool createMipmaps();
	void clearBuffer(const Math::vec4 &color);
	void clearBuffer();
	void swap(const Ptr<Texture> &texture);
	bool copy(const Ptr<Texture> &src);
	bool copyRegion(const Ptr<Texture> &src, const Math::ivec3 &src_coords, int src_level, const Math::ivec3 &dest_coords, int dest_level, int width, int height, int depth);
	bool copy2D(int dest_x = 0, int dest_y = 0, int src_x = 0, int src_y = 0, int w = -1, int h = -1);
	bool copyCube(int face, int dest_x = 0, int dest_y = 0, int src_x = 0, int src_y = 0, int w = -1, int h = -1);
	void render2D(float x0 = -1.0f, float y0 = -1.0f, float x1 = 1.0f, float y1 = 1.0f);
	void render2DScreen(float x0 = -1.0f, float y0 = -1.0f, float x1 = 1.0f, float y1 = 1.0f);
	void renderCube(int face, float x0 = -1.0f, float y0 = -1.0f, float x1 = 1.0f, float y1 = 1.0f);
	void render2DArray(int layer, float x0 = -1.0f, float y0 = -1.0f, float x1 = 1.0f, float y1 = 1.0f);
	void renderCubeArray(int face, int layer, float x0 = -1.0f, float y0 = -1.0f, float x1 = 1.0f, float y1 = 1.0f);
	void setDebugName(const char *name);
	const char *getDebugName() const;
	void bindVertex(int unit) const;
	void unbindVertex(int unit) const;
	void bindFragment(int unit) const;
	void unbindFragment(int unit) const;
	void bindCompute(int unit) const;
	void unbindCompute(int unit) const;
	int getGLTarget() const;
	int getGLInternalFormat() const;
	int getGLPixelFormat() const;
	int getGLPixelType() const;
	int getGLTextureID() const;
	void * getD3D11Texture() const;
	void * getD3D11ShaderResourceView() const;
	void * getD3D11UnorderedAccessView(int r32_typeless) const;
	void * getD3D11RenderTargetView() const;
	void * getD3D11DepthStencilView() const;
	void * getD3D11RenderTargetView(int index) const;
	void * getD3D11DepthStencilView(int index) const;
};
typedef Ptr<Texture> TexturePtr;


class UNIGINE_API StructuredBuffer : public APIInterface
{
public:

	enum
	{
		GPU_RESOURCE = 0,
		IMMUTABLE = 1,
		CPU_RESOURCE = 2,
		STAGING = 3,
	};
	static Ptr<StructuredBuffer> create();
	int create(int flags, void *data, unsigned int structure_size, unsigned int num_elements);
	int create(int flags, unsigned int structure_size, unsigned int num_elements);
	int getData(void *data);
	void clear();
	void destroy();
	void clearBuffer();
	void copy(const Ptr<StructuredBuffer> &src);
	int getNumElements() const;
	void setDebugName(const char *name);
	const char *getDebugName() const;
	void bindVertex(int unit) const;
	void unbindVertex(int unit) const;
	void bindControl(int unit) const;
	void unbindControl(int unit) const;
	void bindEvaluate(int unit) const;
	void unbindEvaluate(int unit) const;
	void bindGeometry(int unit) const;
	void unbindGeometry(int unit) const;
	void bindFragment(int unit) const;
	void unbindFragment(int unit) const;
	void bindCompute(int unit) const;
	void unbindCompute(int unit) const;
	int getGLBufferID() const;
	void * getD3D11ShaderResourceView() const;
	void * getD3D11UnorderedAccessView() const;
};
typedef Ptr<StructuredBuffer> StructuredBufferPtr;


class UNIGINE_API RenderTarget : public APIInterface
{
public:
	static Ptr<RenderTarget> create();
	void destroy();
	Ptr<Texture> getColorTexture(int slot) const;
	void bindColorTexture(int slot, const Ptr<Texture> &texture);
	void bindColorTexture2D(int slot, const Ptr<Texture> &texture, int mip = 0);
	void bindColorTexture2DArray(int slot, const Ptr<Texture> &texture, int layer = -1, int mip = 0);
	void bindColorTextureCube(int slot, const Ptr<Texture> &texture, int face = -1, int mip = 0);
	void bindColorTextureCubeArray(int slot, const Ptr<Texture> &texture, int layer = -1, int face = -1, int mip = 0);
	void bindColorTexture3D(int slot, const Ptr<Texture> &texture, int depth = -1, int mip = 0);
	void unbindColorTexture(int slot);
	void unbindColorTextures();
	Ptr<Texture> getDepthTexture() const;
	void bindDepthTexture(const Ptr<Texture> &texture);
	void bindDepthTexture2D(const Ptr<Texture> &texture, int mip = 0);
	void bindDepthTexture2DArray(const Ptr<Texture> &texture, int layer = -1, int mip = 0);
	void bindDepthTextureCube(const Ptr<Texture> &texture, int face = -1, int mip = 0);
	void bindDepthTextureCubeArray(const Ptr<Texture> &texture, int layer = -1, int face = -1, int mip = 0);
	void unbindDepthTexture();
	Ptr<Texture> getUnorderedAccessTexture(int slot) const;
	void bindUnorderedAccessTexture(int slot, const Ptr<Texture> &texture, bool writeonly = false, bool atomic = false);
	void bindUnorderedAccessTexture2D(int slot, const Ptr<Texture> &texture, bool writeonly = false, bool atomic = false, int mip = 0);
	void bindUnorderedAccessTexture2DArray(int slot, const Ptr<Texture> &texture, bool writeonly = false, bool atomic = false, int layer = -1, int mip = 0);
	void bindUnorderedAccessTextureCube(int slot, const Ptr<Texture> &texture, bool writeonly = false, bool atomic = false, int face = -1, int mip = 0);
	void bindUnorderedAccessTextureCubeArray(int slot, const Ptr<Texture> &texture, bool writeonly = false, bool atomic = false, int layer = -1, int face = -1, int mip = 0);
	void bindUnorderedAccessTexture3D(int slot, const Ptr<Texture> &texture, bool writeonly = false, bool atomic = false, int depth = -1, int mip = 0);
	void unbindUnorderedAccessTexture(int slot);
	void unbindUnorderedAccessTextures();
	Ptr<StructuredBuffer> getStructuredBuffer(int slot) const;
	void bindStructuredBuffer(int slot, const Ptr<StructuredBuffer> &buffer);
	void unbindStructuredBuffer(int slot);
	void unbindStructuredBuffers();
	bool isEnabled() const;
	bool isCompleted() const;
	void enable();
	void enableCompute();
	void disable();
	void flush();
	void unbindAll();
};
typedef Ptr<RenderTarget> RenderTargetPtr;


class UNIGINE_API TextureRamp : public APIInterface
{
public:
	static Ptr<TextureRamp> create();
	static Ptr<TextureRamp> create(int num_channels, int resolution, int flags);
	static Ptr<TextureRamp> create(const Ptr<TextureRamp> &texture_ramp);
	Ptr<Texture> getTexture() const;
	void releaseTexture();
	void copy(const Ptr<TextureRamp> &src_texture_ramp);
	Ptr<TextureRamp> clone() const;
	Ptr<Curve2d> getCurve(int channel) const;
	void setDefaultCurve(int channel, const Ptr<Curve2d> &default_curve);
	bool isDefault(int channel);
	bool isDefaultAll();
	void save(const Ptr<Xml> &xml) const;
	void load(const Ptr<Xml> &xml);
	void save(const Ptr<Json> &json) const;
	void load(const Ptr<Json> &json);
	void saveState(const Ptr<Stream> &stream);
	void restoreState(const Ptr<Stream> &stream);
	void setNumChannels(int channels);
	int getNumChannels() const;
	void setResolution(int resolution);
	int getResolution() const;
	void setFlags(int flags);
	int getFlags() const;
	void *addChangedCallback(CallbackBase *func);
	bool removeChangedCallback(void *id);
	void clearChangedCallbacks();
};
typedef Ptr<TextureRamp> TextureRampPtr;

} // namespace Unigine
