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

#include "UnigineRender.h"
#include "UnigineXml.h"
#include "UnigineUlon.h"
#include "UnigineStreams.h"
#include "UnigineImage.h"
#include "UnigineTextures.h"
#include "UnigineShader.h"
#include "UnigineGUID.h"
#include "UnigineNode.h"
#include "UnigineCurve2d.h"

namespace Unigine
{


class UNIGINE_API Material : public APIInterface
{
public:

	enum
	{
		OPTION_TRANSPARENT = 0,
		OPTION_ORDER,
		OPTION_SHADOW_MASK,
		OPTION_VIEWPORT_MASK,
		OPTION_BLEND_SRC,
		OPTION_BLEND_DEST,
		OPTION_DEPTH_MASK,
		OPTION_DEPTH_TEST,
		OPTION_TWO_SIDED,
		OPTION_CAST_PROJ_OMNI_SHADOW,
		OPTION_CAST_WORLD_SHADOW,
		OPTION_OVERLAP,
	};

	enum
	{
		TRANSPARENT_NONE = 0,
		TRANSPARENT_ALPHA_TEST,
		TRANSPARENT_BLEND,
		TRANSPARENT_WATER,
	};

	enum
	{
		STATE_TOGGLE = 0,
		STATE_SWITCH,
		STATE_INT,
	};

	enum
	{
		TEXTURE_SOURCE_ASSET = 0,
		TEXTURE_SOURCE_RAMP,
		TEXTURE_SOURCE_PROCEDURAL,
		TEXTURE_SOURCE_SURFACE_CUSTOM_TEXTURE,
		TEXTURE_SOURCE_GBUFFER_ALBEDO,
		TEXTURE_SOURCE_GBUFFER_SHADING,
		TEXTURE_SOURCE_GBUFFER_NORMAL,
		TEXTURE_SOURCE_GBUFFER_VELOCITY,
		TEXTURE_SOURCE_GBUFFER_MATERIAL_MASK,
		TEXTURE_SOURCE_GBUFFER_FEATURES,
		TEXTURE_SOURCE_AUXILIARY,
		TEXTURE_SOURCE_REFRACTION,
		TEXTURE_SOURCE_REFRACTION_MASK,
		TEXTURE_SOURCE_TRANSPARENT_BLUR,
		TEXTURE_SOURCE_LIGHTS,
		TEXTURE_SOURCE_BENT_NORMAL,
		TEXTURE_SOURCE_SSAO,
		TEXTURE_SOURCE_SSGI,
		TEXTURE_SOURCE_SSR,
		TEXTURE_SOURCE_CURVATURE,
		TEXTURE_SOURCE_SHADOW_SHAFTS,
		TEXTURE_SOURCE_DOF_MASK,
		TEXTURE_SOURCE_AUTO_EXPOSURE,
		TEXTURE_SOURCE_AUTO_WHITE_BALANCE,
		TEXTURE_SOURCE_SCREEN_COLOR,
		TEXTURE_SOURCE_SCREEN_COLOR_OPACITY,
		TEXTURE_SOURCE_SCREEN_COLOR_OLD,
		TEXTURE_SOURCE_SCREEN_COLOR_OLD_REPROJECTION,
		TEXTURE_SOURCE_NORMAL_UNPACK,
		TEXTURE_SOURCE_CURRENT_DEPTH,
		TEXTURE_SOURCE_OPACITY_DEPTH,
		TEXTURE_SOURCE_LINEAR_DEPTH,
		TEXTURE_SOURCE_LIGHT_IMAGE,
		TEXTURE_SOURCE_LIGHT_SHADOW_DEPTH,
		TEXTURE_SOURCE_LIGHT_SHADOW_COLOR,
		TEXTURE_SOURCE_HAZE_AMBIENT,
		TEXTURE_SOURCE_TRANSPARENT_ENVIRONMENT,
		TEXTURE_SOURCE_REFLECTION_2D,
		TEXTURE_SOURCE_REFLECTION_CUBE,
		TEXTURE_SOURCE_SCATTERING_SKY_LUT,
		TEXTURE_SOURCE_WBUFFER_CONSTANT_ID,
		TEXTURE_SOURCE_WBUFFER_DIFFUSE,
		TEXTURE_SOURCE_WBUFFER_NORMAL,
		TEXTURE_SOURCE_WBUFFER_WATER,
		TEXTURE_SOURCE_WBUFFER_SS_ENVIRONMENT,
		TEXTURE_SOURCE_WBUFFER_WU_MASK,
		TEXTURE_SOURCE_WBUFFER_PLANAR_REFLECTION,
		TEXTURE_SOURCE_WBUFFER_OPACITY_SCREEN,
		TEXTURE_SOURCE_CLOUDS_SCREEN,
		TEXTURE_SOURCE_CLOUDS_STATIC_COVERAGE,
		TEXTURE_SOURCE_CLOUDS_REGION_MASK,
		TEXTURE_SOURCE_CLOUDS_DYNAMIC_COVERAGE,
		TEXTURE_SOURCE_TERRAIN_GLOBAL_DEPTH,
		TEXTURE_SOURCE_TERRAIN_GLOBAL_FLAT_POSITION,
		TEXTURE_SOURCE_FIELD_HEIGHT_ARRAY,
		TEXTURE_SOURCE_FIELD_SHORELINE_ARRAY,
		TEXTURE_SOURCE_DECAL_DEPTH,
		TEXTURE_SOURCE_DECAL_ALBEDO,
		TEXTURE_SOURCE_DECAL_NORMAL,
		TEXTURE_SOURCE_DECAL_SHADING,
		TEXTURE_SOURCE_CUSTOM,
	};

	enum
	{
		PARAMETER_FLOAT = 0,
		PARAMETER_FLOAT2,
		PARAMETER_FLOAT3,
		PARAMETER_FLOAT4,
		PARAMETER_INT,
		PARAMETER_INT2,
		PARAMETER_INT3,
		PARAMETER_INT4,
		PARAMETER_ARRAY_FLOAT,
		PARAMETER_ARRAY_FLOAT2,
		PARAMETER_ARRAY_FLOAT4,
		PARAMETER_ARRAY_INT,
		PARAMETER_ARRAY_INT2,
		PARAMETER_ARRAY_INT4,
		PARAMETER_COMBINER,
	};

	enum DATA_TYPE
	{
		DATA_TYPE_OPTION = 0,
		DATA_TYPE_STATE,
		DATA_TYPE_PARAMETER,
		DATA_TYPE_TEXTURE,
		DATA_TYPE_GROUP,
	};

	enum WIDGET
	{
		WIDGET_EDIT_INT,
		WIDGET_EDIT_INT2,
		WIDGET_EDIT_INT3,
		WIDGET_EDIT_INT4,
		WIDGET_EDIT_FLOAT,
		WIDGET_EDIT_FLOAT2,
		WIDGET_EDIT_FLOAT3,
		WIDGET_EDIT_FLOAT4,
		WIDGET_TOGGLE,
		WIDGET_COMBOBOX,
		WIDGET_TEXTURE_ASSET,
		WIDGET_TEXTURE_RAMP,
		WIDGET_ACCORDION,
		WIDGET_SLIDER,
		WIDGET_COLOR,
		WIDGET_UV,
		WIDGET_MASK24,
		WIDGET_MASK32,
	};
	static Ptr<Material> create();
	bool setParent(const Ptr<Material> &material, bool save_all_values = true);
	Ptr<Material> getParent() const;
	bool isParent(const Ptr<Material> &parent) const;
	bool isParent(const UGUID &guid) const;
	Ptr<Material> getBaseMaterial() const;
	int getNumChildren() const;
	Ptr<Material> getChild(int num) const;
	Ptr<Material> clone(const UGUID &guid);
	Ptr<Material> clone();
	Ptr<Material> inherit(const UGUID &guid);
	Ptr<Material> inherit();
	const char *getNamespaceName() const;
	const char *getManualName() const;
	UGUID getGUID() const;
	const char *getPath() const;
	bool isNodeTypeSupported(Node::TYPE type) const;
	bool isNodeSupported(const Ptr<Node> &node) const;
	bool canRenderNode() const;
	int getNumUIItems() const;
	Material::DATA_TYPE getUIItemDataType(int item) const;
	int getUIItemDataID(int item) const;
	bool isUIItemHidden(int item) const;
	const char *getUIItemTitle(int item) const;
	const char *getUIItemTooltip(int item) const;
	Material::WIDGET getUIItemWidget(int item) const;
	int getUIItemParent(int item) const;
	int getUIItemNumChildren(int item) const;
	int getUIItemChild(int item, int num) const;
	bool isUIItemSliderMinExpand(int item) const;
	bool isUIItemSliderMaxExpand(int item) const;
	float getUIItemSliderMinValue(int item) const;
	float getUIItemSliderMaxValue(int item) const;
	int getUIItemGroupToggleStateID(int item) const;
	bool isUIItemGroupCollapsed(int item) const;
	static const char *widgetToString(Material::WIDGET widget);
	static Material::WIDGET stringToWidget(const char *str);
	void setOption(int num, int value);
	int getOption(int num) const;
	bool isOptionOverridden(int num) const;
	void resetOption(int num);
	void setTransparent(int transparent);
	int getTransparent() const;
	bool isWater() const;
	bool isDeferred() const;
	bool isForward() const;
	bool isAlphaTest() const;
	void setBlendDestFunc(int func);
	int getBlendDestFunc() const;
	void setBlendSrcFunc(int func);
	int getBlendSrcFunc() const;
	void setShadowMask(int mask);
	int getShadowMask() const;
	void setViewportMask(int mask);
	int getViewportMask() const;
	void setDepthMask(int mask);
	int getDepthMask() const;
	void setOrder(int order);
	int getOrder() const;
	void setCastShadow(bool shadow);
	bool isCastShadow() const;
	void setCastWorldShadow(bool shadow);
	bool isCastWorldShadow() const;
	void setDepthTest(bool test);
	bool isDepthTest() const;
	void setTwoSided(bool sided);
	bool isTwoSided() const;
	void setOverlap(bool overlap);
	bool isOverlap() const;
	bool checkShaderCache() const;
	bool checkShaderCache(Render::PASS pass, Node::TYPE node_type) const;
	bool compileShader(Render::PASS pass, Node::TYPE node_type);
	Ptr<Shader> fetchShader(Render::PASS pass, Node::TYPE node_type);
	Ptr<Shader> fetchShader(Render::PASS pass);
	Ptr<Shader> fetchShader(const char *pass_name, int node);
	Ptr<Shader> fetchShader(const char *pass_name);
	void createShaders(bool recursive = false);
	void destroyTextures();
	int getNumParameters() const;
	int findParameter(const char *name) const;
	void resetParameter(int num);
	bool checkParameterConditions(int num) const;
	int getParameterType(int num) const;
	bool isParameterInt(int num) const;
	bool isParameterFloat(int num) const;
	bool isParameterOverridden(int num) const;
	const char *getParameterName(int num) const;
	bool isParameterExpressionEnabled(int num) const;
	void setParameterExpressionEnabled(int num, bool enabled);
	const char *getParameterExpression(int num) const;
	int setParameterExpression(int num, const char *expression);
	void setParameterFloat(int num, float value);
	void setParameterFloat(const char *name, float value);
	float getParameterFloat(int num) const;
	float getParameterFloat(const char *name) const;
	void setParameterFloat2(int num, const Math::vec2 &value);
	void setParameterFloat2(const char *name, const Math::vec2 &value);
	Math::vec2 getParameterFloat2(int num) const;
	Math::vec2 getParameterFloat2(const char *name) const;
	void setParameterFloat3(int num, const Math::vec3 &value);
	void setParameterFloat3(const char *name, const Math::vec3 &value);
	Math::vec3 getParameterFloat3(int num) const;
	Math::vec3 getParameterFloat3(const char *name) const;
	void setParameterFloat4(int num, const Math::vec4 &value);
	void setParameterFloat4(const char *name, const Math::vec4 &value);
	Math::vec4 getParameterFloat4(int num) const;
	Math::vec4 getParameterFloat4(const char *name) const;
	void setParameterInt(int num, int value);
	void setParameterInt(const char *name, int value);
	int getParameterInt(int num) const;
	int getParameterInt(const char *name) const;
	void setParameterInt2(int num, const Math::ivec2 &value);
	void setParameterInt2(const char *name, const Math::ivec2 &value);
	Math::ivec2 getParameterInt2(int num) const;
	Math::ivec2 getParameterInt2(const char *name) const;
	void setParameterInt3(int num, const Math::ivec3 &value);
	void setParameterInt3(const char *name, const Math::ivec3 &value);
	Math::ivec3 getParameterInt3(int num) const;
	Math::ivec3 getParameterInt3(const char *name) const;
	void setParameterInt4(int num, const Math::ivec4 &value);
	void setParameterInt4(const char *name, const Math::ivec4 &value);
	Math::ivec4 getParameterInt4(int num) const;
	Math::ivec4 getParameterInt4(const char *name) const;
	int getParameterArraySize(int num) const;
	bool isParameterArray(int num) const;
	void getParameterArray(int num, Vector<float> &values) const;
	void setParameterArray(int num, const Vector<float> &values);
	void getParameterArray(int num, Vector<Math::vec2> &values) const;
	void setParameterArray(int num, const Vector<Math::vec2> &values);
	void getParameterArray(int num, Vector<Math::vec4> &values) const;
	void setParameterArray(int num, const Vector<Math::vec4> &values);
	void getParameterArray(int num, Vector<int> &values) const;
	void setParameterArray(int num, const Vector<int> &values);
	void getParameterArray(int num, Vector<Math::ivec2> &values) const;
	void setParameterArray(int num, const Vector<Math::ivec2> &values);
	void getParameterArray(int num, Vector<Math::ivec4> &values) const;
	void setParameterArray(int num, const Vector<Math::ivec4> &values);
	int getNumStates() const;
	int findState(const char *name) const;
	bool isStateOverridden(int num) const;
	bool isStateInternal(int num) const;
	void resetState(int num);
	bool checkStateConditions(int num) const;
	const char *getStateName(int num) const;
	const char *getStateSwitchItem(int num, int item) const;
	int getStateSwitchNumItems(int num) const;
	int getStateType(int num) const;
	int getState(int num) const;
	void setState(int num, int value);
	int getState(const char *name) const;
	void setState(const char *name, int value);
	int getNumTextures() const;
	int findTexture(const char *name) const;
	bool isTextureOverridden(int num) const;
	bool isTextureLoaded(int num) const;
	bool isTextureInternal(int num) const;
	void resetTexture(int num);
	bool checkTextureConditions(int num) const;
	const char *getTextureName(int num) const;
	int getTextureUnit(int num) const;
	bool isTextureEditable(int num) const;
	int getTextureSource(int num) const;
	int getTextureSamplerFlags(int num) const;
	void setTextureSamplerFlags(int num, int sampler_flags);
	int getTextureFormatFlags(int num) const;
	int getTextureImage(int num, const Ptr<Image> &image) const;
	int setTextureImage(int num, const Ptr<Image> &image);
	Ptr<Texture> getTexture(int num);
	Ptr<Texture> getTexture(const char *name);
	void setTexture(int num, const Ptr<Texture> &texture);
	void setTexture(const char *name, const Ptr<Texture> &texture);
	void setTexturePath(int num, const char *path);
	const char *getTexturePath(int num) const;
	void setTexturePath(const char *name, const char *path);
	const char *getTexturePath(const char *name) const;
	Ptr<TextureRamp> getTextureRamp(int num);
	Ptr<TextureRamp> getTextureRampOverride(int num);
	bool isEditable() const;
	bool isHidden() const;
	bool isBase() const;
	bool isBrush() const;
	bool isLegacy() const;
	bool isPreviewHidden() const;
	bool isReflection2D() const;
	bool isInternal() const;
	bool isManual() const;
	bool canSave() const;
	bool isAutoSave() const;
	bool isFileEngine() const;
	bool isEmpty() const;
	bool hasOverrides() const;
	bool saveState(const Ptr<Stream> &stream, bool forced = false) const;
	bool restoreState(const Ptr<Stream> &stream, bool forced = false);
	int loadXml(const Ptr<Xml> &xml);
	bool saveXml(const Ptr<Xml> &xml) const;
	bool loadUlon(const Ptr<UlonNode> &ulon);
	bool load(const char *path);
	bool save();
	bool reload();
	bool createMaterialFile(const char *path);
	Render::PASS getRenderPass(const char *pass_name) const;
	const char *getRenderPassName(Render::PASS type) const;
	bool runExpression(const char *name, int w, int h, int d = 1);
	bool renderScreen(const char *pass_name);
	void renderScreen(Render::PASS pass);
	bool renderCompute(const char *pass_name, int group_threads_x = 1, int group_threads_y = 1, int group_threads_z = 1);
	void renderCompute(Render::PASS pass, int group_threads_x = 1, int group_threads_y = 1, int group_threads_z = 1);
};
typedef Ptr<Material> MaterialPtr;

} // namespace Unigine
