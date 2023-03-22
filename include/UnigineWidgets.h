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
#include "UnigineVector.h"
#include "UnigineString.h"
#include "UnigineImage.h"
#include "UnigineTextures.h"
#include "UnigineSounds.h"
#include "UnigineMaterial.h"
#include "UnigineCamera.h"
#include "UnigineCallback.h"
#include "UnigineGui.h"

namespace Unigine
{


class UNIGINE_API Widget : public APIInterface
{
public:
	static bool convertible(Widget *obj) { return obj != nullptr; }

	enum TYPE
	{
		WIDGET_VBOX = 0,
		WIDGET_HBOX,
		WIDGET_GRID_BOX,
		WIDGET_VPANED,
		WIDGET_HPANED,
		WIDGET_LABEL,
		WIDGET_BUTTON,
		WIDGET_EDIT_LINE,
		WIDGET_EDIT_TEXT,
		WIDGET_CHECK_BOX,
		WIDGET_COMBO_BOX,
		WIDGET_CANVAS,
		WIDGET_GROUP_BOX,
		WIDGET_LIST_BOX,
		WIDGET_TREE_BOX,
		WIDGET_TAB_BOX,
		WIDGET_SCROLL,
		WIDGET_SCROLL_BOX,
		WIDGET_SPACER,
		WIDGET_SLIDER,
		WIDGET_SPIN_BOX,
		WIDGET_SPIN_BOX_DOUBLE,
		WIDGET_ICON,
		WIDGET_SPRITE,
		WIDGET_SPRITE_VIDEO,
		WIDGET_SPRITE_SHADER,
		WIDGET_SPRITE_VIEWPORT,
		WIDGET_SPRITE_NODE,
		WIDGET_MENU_BAR,
		WIDGET_MENU_BOX,
		WIDGET_WINDOW,
		WIDGET_DIALOG,
		WIDGET_DIALOG_MESSAGE,
		WIDGET_DIALOG_FILE,
		WIDGET_DIALOG_COLOR,
		WIDGET_DIALOG_IMAGE,
		WIDGET_MANIPULATOR,
		WIDGET_MANIPULATOR_TRANSLATOR,
		WIDGET_MANIPULATOR_ROTATOR,
		WIDGET_MANIPULATOR_SCALER,
		WIDGET_EXTERN,
		WIDGET_ENGINE,
		WIDGET_DRAG_AREA,
		NUM_WIDGETS,
	};

	enum LIFETIME
	{
		LIFETIME_WORLD,
		LIFETIME_WINDOW,
		LIFETIME_ENGINE,
		LIFETIME_MANUAL,
	};
	Widget::TYPE getType() const;
	void setFlags(int flags);
	int getFlags() const;
	bool isExpanded() const;
	bool isOverlapped() const;
	bool isBackground() const;
	bool isFixed() const;
	bool isLayout() const;
	const char *getTypeName() const;
	void setGui(const Ptr<Gui> &gui);
	Ptr<Gui> getGui() const;
	Ptr<Gui> getParentGui() const;
	void setParent(const Ptr<Widget> &parent);
	Ptr<Widget> getParent() const;
	void setEnabled(bool enabled);
	bool isEnabled() const;
	void setHidden(bool hidden);
	bool isHidden() const;
	void setOrder(int order);
	int getOrder() const;
	void setData(const char *data);
	const char *getData() const;
	Widget::LIFETIME getLifetimeSelf() const;
	void setLifetime(Widget::LIFETIME lifetime);
	Widget::LIFETIME getLifetime() const;
	void setFocus();
	void setPermanentFocus();
	void removeFocus();
	void setNextFocus(const Ptr<Widget> &focus);
	Ptr<Widget> getNextFocus() const;
	int isFocused() const;
	void setPosition(int x, int y);
	int getPositionX() const;
	int getPositionY() const;
	int getScreenPositionX() const;
	int getScreenPositionY() const;
	void setWidth(int width);
	int getWidth() const;
	void setHeight(int height);
	int getHeight() const;
	bool getIntersection(int local_pos_x, int local_pos_y) const;
	Ptr<Widget> getHierarchyIntersection(int screen_pos_x, int screen_pos_y);
	void setIntersectionEnabled(bool enabled);
	bool isIntersectionEnabled() const;
	int getMouseX() const;
	int getMouseY() const;
	void setMouseCursor(int cursor);
	int getMouseCursor() const;
	void setFont(const char *name);
	void setFontSize(int size);
	int getFontSize() const;
	void setFontColor(const Math::vec4 &color);
	Math::vec4 getFontColor() const;
	void setFontPermanent(int permanent);
	int getFontPermanent() const;
	void setFontHSpacing(int hspacing);
	int getFontHSpacing() const;
	void setFontVSpacing(int vspacing);
	int getFontVSpacing() const;
	void setFontHOffset(int hoffset);
	int getFontHOffset() const;
	void setFontVOffset(int voffset);
	int getFontVOffset() const;
	void setFontOutline(int outline);
	int getFontOutline() const;
	void setFontRich(int rich);
	int getFontRich() const;
	void setFontWrap(int wrap);
	int getFontWrap() const;
	void setToolTip(const char *str, int reset = 0);
	const char *getToolTip() const;
	void *addCallback(Gui::CALLBACK_INDEX callback, CallbackBase *func);
	void *addCallback(Gui::CALLBACK_INDEX callback, CallbackBase1<Ptr<Widget>> *func);
	void *addCallback(Gui::CALLBACK_INDEX callback, CallbackBase2<Ptr<Widget>, Ptr<Widget>> *func);
	void *addCallback(Gui::CALLBACK_INDEX callback, CallbackBase3<Ptr<Widget>, Ptr<Widget>, int> *func);
	bool removeCallback(Gui::CALLBACK_INDEX callback, void *id);
	void clearCallbacks(Gui::CALLBACK_INDEX callback);
	bool isCallback(int callback) const;
	void setCallbackAccel(int callback, unsigned int key, int ctrl, int alt, int shift);
	bool getCallbackAccel(int callback, unsigned int &key, int &ctrl, int &alt, int &shift) const;
	int isCallbackAccel(unsigned int key, int ctrl, int alt, int shift) const;
	void setCallbackEnabled(int callback, bool enabled);
	int isCallbackEnabled(int callback) const;
	void runCallback(int callback) const;
	void addAttach(const Ptr<Widget> &w, const char *format = 0, int multiplier = 1, int flags = 0);
	void removeAttach(const Ptr<Widget> &w);
	void addChild(const Ptr<Widget> &w, int flags = -1);
	void removeChild(const Ptr<Widget> &w);
	void replaceChild(const Ptr<Widget> &w, const Ptr<Widget> &old_w, int flags = -1);
	int isChild(const Ptr<Widget> &w) const;
	int getNumChildren() const;
	Ptr<Widget> getChild(int num) const;
	int getKeyActivity(unsigned int key) const;
	void raise(const Ptr<Widget> &w);
	void arrange();
};
typedef Ptr<Widget> WidgetPtr;


class UNIGINE_API WidgetVBox : public Widget
{
public:
	static bool convertible(Widget *obj) {
		if(!obj)
			return false;
		int type = obj->getType();
		switch(type)
		{
			case Widget::WIDGET_VBOX:
			case Widget::WIDGET_HBOX:
			case Widget::WIDGET_GRID_BOX:
			return true;
		}
		return false;
	}
	static Ptr<WidgetVBox> create(const Ptr<Gui> &gui, int x = 0, int y = 0);
	static Ptr<WidgetVBox> create(int x = 0, int y = 0);
	void setSpace(int x, int y);
	int getSpaceX() const;
	int getSpaceY() const;
	void setPadding(int l, int r, int t, int b);
	int getPaddingLeft() const;
	int getPaddingRight() const;
	int getPaddingTop() const;
	int getPaddingBottom() const;
	void setStencil(int stencil);
	int getStencil() const;
	void setColor(const Math::vec4 &color);
	Math::vec4 getColor() const;
	void setBackgroundColor(const Math::vec4 &color);
	Math::vec4 getBackgroundColor() const;
	void setBackground(int background);
	int getBackground() const;
	void setBorder(bool border);
	bool isBorder() const;
};
typedef Ptr<WidgetVBox> WidgetVBoxPtr;


class UNIGINE_API WidgetHBox : public WidgetVBox
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_HBOX; }
	static Ptr<WidgetHBox> create(const Ptr<Gui> &gui, int x = 0, int y = 0);
	static Ptr<WidgetHBox> create(int x = 0, int y = 0);
};
typedef Ptr<WidgetHBox> WidgetHBoxPtr;


class UNIGINE_API WidgetVPaned : public Widget
{
public:
	static bool convertible(Widget *obj) {
		if(!obj)
			return false;
		int type = obj->getType();
		switch(type)
		{
			case Widget::WIDGET_VPANED:
			case Widget::WIDGET_HPANED:
			return true;
		}
		return false;
	}
	static Ptr<WidgetVPaned> create(const Ptr<Gui> &gui);
	static Ptr<WidgetVPaned> create();
	void setValue(int value);
	int getValue() const;
	void setFixed(int fixed);
	int getFixed() const;
	void setFirstSize(int size);
	int getFirstSize() const;
	void setSecondSize(int size);
	int getSecondSize() const;
};
typedef Ptr<WidgetVPaned> WidgetVPanedPtr;


class UNIGINE_API WidgetHPaned : public WidgetVPaned
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_HPANED; }
	static Ptr<WidgetHPaned> create(const Ptr<Gui> &gui);
	static Ptr<WidgetHPaned> create();
};
typedef Ptr<WidgetHPaned> WidgetHPanedPtr;


class UNIGINE_API WidgetGridBox : public WidgetVBox
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_GRID_BOX; }
	static Ptr<WidgetGridBox> create(const Ptr<Gui> &gui, int num = 2, int x = 0, int y = 0);
	static Ptr<WidgetGridBox> create(int num = 2, int x = 0, int y = 0);
	void setNumColumns(int columns);
	int getNumColumns() const;
	void setColumnRatio(int num, int ratio);
	int getColumnRatio(int num) const;
};
typedef Ptr<WidgetGridBox> WidgetGridBoxPtr;


class UNIGINE_API WidgetGroupBox : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_GROUP_BOX; }
	static Ptr<WidgetGroupBox> create(const Ptr<Gui> &gui, const char *str = 0, int x = 0, int y = 0);
	static Ptr<WidgetGroupBox> create(const char *str = 0, int x = 0, int y = 0);
	void setSpace(int x, int y);
	int getSpaceX() const;
	int getSpaceY() const;
	void setBackgroundColor(const Math::vec4 &color);
	Math::vec4 getBackgroundColor() const;
	void setBorderColor(const Math::vec4 &color);
	Math::vec4 getBorderColor() const;
	void setPadding(int l, int r, int t, int b);
	int getPaddingLeft() const;
	int getPaddingRight() const;
	int getPaddingTop() const;
	int getPaddingBottom() const;
	void setBorder(int border);
	int getBorder() const;
	void setStencil(int stencil);
	int getStencil() const;
	void setColor(const Math::vec4 &color);
	Math::vec4 getColor() const;
	void setBackground(int background);
	int getBackground() const;
	void setText(const char *text);
	const char *getText() const;
};
typedef Ptr<WidgetGroupBox> WidgetGroupBoxPtr;


class UNIGINE_API WidgetTabBox : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_TAB_BOX; }
	static Ptr<WidgetTabBox> create(const Ptr<Gui> &gui, int x = 0, int y = 0);
	static Ptr<WidgetTabBox> create(int x = 0, int y = 0);
	void setSpace(int x, int y);
	int getSpaceX() const;
	int getSpaceY() const;
	void setPadding(int l, int r, int t, int b);
	int getPaddingLeft() const;
	int getPaddingRight() const;
	int getPaddingTop() const;
	int getPaddingBottom() const;
	void setImage(const Ptr<Image> &image);
	Ptr<Image> getImage() const;
	void setTexture(const char *texture);
	const char *getTexture() const;
	void setButtonColor(const Math::vec4 &color);
	Math::vec4 getButtonColor() const;
	void setBorderColor(const Math::vec4 &color);
	Math::vec4 getBorderColor() const;
	void clear();
	int addTab(const char *str, int texture = -1);
	void removeTab(int num);
	int getNumTabs() const;
	void setTabHidden(int num, bool hidden);
	bool isTabHidden(int num) const;
	void setTabText(int num, const char *str);
	const char *getTabText(int num) const;
	void setTabData(int num, const char *str);
	const char *getTabData(int num) const;
	void setTabToolTip(int num, const char *str);
	const char *getTabToolTip(int num) const;
	void setTabTexture(int num, int texture);
	int getTabTexture(int num) const;
	void setCurrentTab(int tab);
	int getCurrentTab() const;
	const char *getCurrentTabText() const;
	const char *getCurrentTabData() const;
};
typedef Ptr<WidgetTabBox> WidgetTabBoxPtr;


class UNIGINE_API WidgetScroll : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_SCROLL; }
	static Ptr<WidgetScroll> create(const Ptr<Gui> &gui, int object = 100, int frame = 10, int step = 1, int value = 0);
	static Ptr<WidgetScroll> create(int object = 100, int frame = 10, int step = 1, int value = 0);
	void setScrollColor(const Math::vec4 &color);
	Math::vec4 getScrollColor() const;
	void setOrientation(int orientation);
	int getOrientation() const;
	void setMouseWheelOrientation(int orientation);
	int getMouseWheelOrientation() const;
	void setObjectSize(int size);
	int getObjectSize() const;
	void setFrameSize(int size);
	int getFrameSize() const;
	void setStepSize(int size);
	int getStepSize() const;
	void setValue(int value);
	int getValue() const;
	void setSliderButton(bool button);
	bool isSliderButton() const;
};
typedef Ptr<WidgetScroll> WidgetScrollPtr;


class UNIGINE_API WidgetScrollBox : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_SCROLL_BOX; }

	enum SCROLL_RENDER_MODE
	{
		ALWAYS_RENDER,
		AUTO_HIDE,
		ALWAYS_HIDE,
		ALWAYS_HIDE_NO_BOUNDS,
	};
	static Ptr<WidgetScrollBox> create(const Ptr<Gui> &gui, int x = 0, int y = 0);
	static Ptr<WidgetScrollBox> create(int x = 0, int y = 0);
	void setSpace(int x, int y);
	int getSpaceX() const;
	int getSpaceY() const;
	void setBorderColor(const Math::vec4 &color);
	Math::vec4 getBorderColor() const;
	void setBackgroundColor(const Math::vec4 &color);
	Math::vec4 getBackgroundColor() const;
	void setHscrollColor(const Math::vec4 &color);
	Math::vec4 getHscrollColor() const;
	void setVscrollColor(const Math::vec4 &color);
	Math::vec4 getVscrollColor() const;
	void setPadding(int l, int r, int t, int b);
	int getPaddingLeft() const;
	int getPaddingRight() const;
	int getPaddingTop() const;
	int getPaddingBottom() const;
	void setBorder(int border);
	int getBorder() const;
	void setBackground(int background);
	int getBackground() const;
	int getScrollScale() const;
	void setVScrollEnabled(bool enabled);
	bool isVScrollEnabled() const;
	void setVScrollHidden(WidgetScrollBox::SCROLL_RENDER_MODE hidden);
	WidgetScrollBox::SCROLL_RENDER_MODE getVScrollHidden() const;
	int getVScrollObjectSize() const;
	int getVScrollFrameSize() const;
	int getVScrollStepSize() const;
	void setVScrollValue(int value);
	int getVScrollValue() const;
	Ptr<WidgetScroll> getVScroll() const;
	void setHScrollEnabled(bool enabled);
	bool isHScrollEnabled() const;
	void setHScrollHidden(WidgetScrollBox::SCROLL_RENDER_MODE hidden);
	WidgetScrollBox::SCROLL_RENDER_MODE getHScrollHidden() const;
	int getHScrollObjectSize() const;
	int getHScrollFrameSize() const;
	int getHScrollStepSize() const;
	void setHScrollValue(int value);
	int getHScrollValue() const;
	Ptr<WidgetScroll> getHScroll() const;
};
typedef Ptr<WidgetScrollBox> WidgetScrollBoxPtr;


class UNIGINE_API WidgetLabel : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_LABEL; }
	static Ptr<WidgetLabel> create(const Ptr<Gui> &gui, const char *str = 0);
	static Ptr<WidgetLabel> create(const char *str = 0);
	void setTextAlign(int align);
	int getTextAlign() const;
	void setText(const char *text);
	const char *getText() const;
};
typedef Ptr<WidgetLabel> WidgetLabelPtr;


class UNIGINE_API WidgetSprite : public Widget
{
public:
	static bool convertible(Widget *obj) {
		if(!obj)
			return false;
		int type = obj->getType();
		switch(type)
		{
			case Widget::WIDGET_SPRITE:
			case Widget::WIDGET_SPRITE_VIDEO:
			case Widget::WIDGET_SPRITE_SHADER:
			case Widget::WIDGET_SPRITE_VIEWPORT:
			case Widget::WIDGET_SPRITE_NODE:
			return true;
		}
		return false;
	}
	static Ptr<WidgetSprite> create(const Ptr<Gui> &gui, const char *name = 0);
	static Ptr<WidgetSprite> create(const char *name = 0);
	int addLayer();
	void removeLayer(int layer);
	int getNumLayers() const;
	void setColor(const Math::vec4 &color);
	Math::vec4 getColor() const;
	void setWrapRepeat(int repeat);
	int getWrapRepeat() const;
	void setBufferMask(int mask);
	int getBufferMask() const;
	void setBlendFunc(int src, int dest);
	int getBlendSrcFunc() const;
	int getBlendDestFunc() const;
	void setTexCoord(const Math::vec4 &coord);
	Math::vec4 getTexCoord() const;
	void setTransform(const Math::mat4 &transform);
	Math::mat4 getTransform() const;
	void setImage(const Ptr<Image> &image, int dynamic = 0);
	Ptr<Image> getImage() const;
	void setTexture(const char *texture);
	const char *getTexture() const;
	void setRender(const Ptr<Texture> &texture, int flipped = 0);
	Ptr<Texture> getRender() const;
	void setLayerEnabled(int layer, bool enabled);
	int isLayerEnabled(int layer) const;
	int getLayerWidth(int layer) const;
	int getLayerHeight(int layer) const;
	void setLayerColor(int layer, const Math::vec4 &color);
	Math::vec4 getLayerColor(int layer) const;
	void setLayerWrapRepeat(int layer, int repeat);
	int getLayerWrapRepeat(int layer) const;
	void setLayerBufferMask(int layer, int mask);
	int getLayerBufferMask(int layer) const;
	void setLayerBlendFunc(int layer, int src, int dest);
	int getLayerBlendSrcFunc(int layer) const;
	int getLayerBlendDestFunc(int layer) const;
	void setLayerTexCoord(int layer, const Math::vec4 &texcoord);
	Math::vec4 getLayerTexCoord(int layer) const;
	void setLayerTransform(int layer, const Math::mat4 &transform);
	Math::mat4 getLayerTransform(int layer) const;
	void setLayerImage(int layer, const Ptr<Image> &image, int dynamic = 0);
	Ptr<Image> getLayerImage(int layer) const;
	void setLayerTexture(int layer, const char *name);
	const char *getLayerTexture(int layer) const;
	void setLayerRender(int layer, const Ptr<Texture> &texture, int flipped = 0);
	Ptr<Texture> getLayerRender(int layer) const;
	void setIntersectionImageEnabled(bool enabled);
	bool isIntersectionImageEnabled() const;
	Ptr<Image> getIntersectionImage() const;
	void setIntersectionImage(const Ptr<Image> &image);
	void setIntersectionImageName(const char *name);
	void setIntersectionImageTransform(const Math::mat4 &transform);
	Math::mat4 getIntersectionImageTransform() const;
	void setIntersectionImageThreshold(float threshold);
	float getIntersectionImageThreshold() const;
};
typedef Ptr<WidgetSprite> WidgetSpritePtr;


class UNIGINE_API WidgetSpriteVideo : public WidgetSprite
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_SPRITE_VIDEO; }
	static Ptr<WidgetSpriteVideo> create(const Ptr<Gui> &gui, const char *name = 0, int mode = 1);
	static Ptr<WidgetSpriteVideo> create(const char *name = 0, int mode = 1);
	void setLoop(int loop);
	int getLoop() const;
	void setYUV(int yuv);
	int getYUV() const;
	void setVideoTime(float time);
	float getVideoTime() const;
	void setSoundSource(const Ptr<SoundSource> &source);
	Ptr<SoundSource> getSoundSource() const;
	void setAmbientSource(const Ptr<AmbientSource> &source);
	Ptr<AmbientSource> getAmbientSource() const;
	void play();
	void stop();
	bool isPlaying() const;
	bool isStopped() const;
};
typedef Ptr<WidgetSpriteVideo> WidgetSpriteVideoPtr;


class UNIGINE_API WidgetSpriteShader : public WidgetSprite
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_SPRITE_SHADER; }
	static Ptr<WidgetSpriteShader> create(const Ptr<Gui> &gui, const char *name = 0);
	static Ptr<WidgetSpriteShader> create(const char *name = 0);
	void setMaterial(const Ptr<Material> &material);
	Ptr<Material> getMaterial() const;
};
typedef Ptr<WidgetSpriteShader> WidgetSpriteShaderPtr;


class UNIGINE_API WidgetSpriteViewport : public WidgetSprite
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_SPRITE_VIEWPORT; }
	static Ptr<WidgetSpriteViewport> create(const Ptr<Gui> &gui, int width, int height);
	static Ptr<WidgetSpriteViewport> create(int width, int height);
	void setCamera(const Ptr<Camera> &camera);
	Ptr<Camera> getCamera() const;
	void setAspectCorrection(bool correction);
	bool isAspectCorrection() const;
	void setUseTAAOffset(bool offset);
	bool isUseTAAOffset() const;
	void setTextureWidth(int width);
	int getTextureWidth() const;
	void setTextureHeight(int height);
	int getTextureHeight() const;
	void setViewportMask(int mask);
	int getViewportMask() const;
	void setReflectionViewportMask(int mask);
	int getReflectionViewportMask() const;
	void setProjection(const Math::mat4 &projection);
	Math::mat4 getProjection() const;
	void setModelview(const Math::Mat4 &modelview);
	Math::Mat4 getModelview() const;
	void setSkipFlags(int flags);
	int getSkipFlags() const;
	int checkSkipFlags(int flags);
	void appendSkipFlags(int flags);
	void removeSkipFlags(int flags);
	void setIFps(float ifps);
	float getIFps() const;
	void renderImage(const Ptr<Image> &image);
};
typedef Ptr<WidgetSpriteViewport> WidgetSpriteViewportPtr;


class UNIGINE_API WidgetSpriteNode : public WidgetSprite
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_SPRITE_NODE; }
	static Ptr<WidgetSpriteNode> create(const Ptr<Gui> &gui, int width, int height);
	static Ptr<WidgetSpriteNode> create(int width, int height);
	void setCamera(const Ptr<Camera> &camera);
	Ptr<Camera> getCamera() const;
	void setAspectCorrection(bool correction);
	bool isAspectCorrection() const;
	void setUseTAAOffset(bool offset);
	bool isUseTAAOffset() const;
	void setTextureWidth(int width);
	int getTextureWidth() const;
	void setTextureHeight(int height);
	int getTextureHeight() const;
	void setViewportMask(int mask);
	int getViewportMask() const;
	void setReflectionViewportMask(int mask);
	int getReflectionViewportMask() const;
	void setProjection(const Math::mat4 &projection);
	Math::mat4 getProjection() const;
	void setModelview(const Math::Mat4 &modelview);
	Math::Mat4 getModelview() const;
	void setEnvironmentTexturePath(const char *path);
	const char *getEnvironmentTexturePath() const;
	void setSkipFlags(int flags);
	int getSkipFlags() const;
	int checkSkipFlags(int flags);
	void appendSkipFlags(int flags);
	void removeSkipFlags(int flags);
	void setLightUsage(int usage);
	int getLightUsage() const;
	void setIFps(float ifps);
	float getIFps() const;
	void setNode(const Ptr<Node> &node);
	Ptr<Node> getNode() const;
	void renderImage(const Ptr<Image> &image);
};
typedef Ptr<WidgetSpriteNode> WidgetSpriteNodePtr;


class UNIGINE_API WidgetCanvas : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_CANVAS; }
	static Ptr<WidgetCanvas> create(const Ptr<Gui> &gui);
	static Ptr<WidgetCanvas> create();
	void setImage(const Ptr<Image> &image);
	Ptr<Image> getImage() const;
	void setTexture(const char *texture);
	const char *getTexture() const;
	void setColor(const Math::vec4 &color);
	Math::vec4 getColor() const;
	void setTransform(const Math::mat4 &transform);
	Math::mat4 getTransform() const;
	void clear();
	int addText(int order = 0);
	void removeText(int text);
	int getNumTexts() const;
	int getText(int num) const;
	int getTextIntersection(int x, int y) const;
	void setTextOrder(int text, int order);
	int getTextOrder(int text) const;
	void setTextSize(int text, int size);
	int getTextSize(int text) const;
	void setTextColor(int text, const Math::vec4 &color);
	Math::vec4 getTextColor(int text) const;
	void setTextOutline(int text, int outline);
	int getTextOutline(int text) const;
	void setTextPixelPerfect(int text, bool pixel_perfect);
	bool getTextPixelPerfect(int text) const;
	void setTextPosition(int text, const Math::vec2 &position);
	Math::vec2 getTextPosition(int text) const;
	void setTextTransform(int text, const Math::mat4 &transform);
	Math::mat4 getTextTransform(int text) const;
	void setTextText(int text, const char *str);
	const char *getTextText(int text) const;
	int getTextWidth(int text) const;
	int getTextHeight(int text) const;
	void setTextAlign(int text, int align);
	int getTextAlign(int text) const;
	int addLine(int order = 0);
	void removeLine(int line);
	int getNumLines() const;
	int getLine(int num) const;
	int getLineIntersection(int x, int y, float distance) const;
	void setLineOrder(int line, int order);
	int getLineOrder(int line) const;
	void setLineColor(int line, const Math::vec4 &color);
	Math::vec4 getLineColor(int line) const;
	void setLineTransform(int line, const Math::mat4 &transform);
	Math::mat4 getLineTransform(int line) const;
	void clearLinePoints(int line);
	int getNumLinePoints(int line) const;
	void removeLinePoint(int line, int num);
	int addLinePoint(int line, const Math::vec3 &point);
	int addLinePoint(int line, const Math::vec2 &point);
	void setLinePoint(int line, int num, const Math::vec3 &point);
	void setLinePoint(int line, int num, const Math::vec2 &point);
	Math::vec3 getLinePoint(int line, int num) const;
	void clearLineIndices(int line);
	int getNumLineIndices(int line) const;
	void removeLineIndex(int line, int num);
	int addLineIndex(int line, int index);
	int getLineIndex(int line, int num) const;
	int addPolygon(int polygon = 0);
	void removePolygon(int polygon);
	int getNumPolygons() const;
	int getPolygon(int num) const;
	int getPolygonIntersection(int x, int y) const;
	void setPolygonOrder(int polygon, int order);
	int getPolygonOrder(int polygon) const;
	void setPolygonTwoSided(int polygon, int two_sided);
	int getPolygonTwoSided(int polygon) const;
	void setPolygonColor(int polygon, const Math::vec4 &color);
	Math::vec4 getPolygonColor(int polygon) const;
	void setPolygonWrapRepeat(int polygon, int repeat);
	int getPolygonWrapRepeat(int polygon) const;
	void setPolygonBlendFunc(int polygon, int src, int dest);
	int getPolygonBlendSrcFunc(int polygon) const;
	int getPolygonBlendDestFunc(int polygon) const;
	void setPolygonTransform(int polygon, const Math::mat4 &transform);
	Math::mat4 getPolygonTransform(int polygon) const;
	void setPolygonIntersection(int polygon, bool intersection);
	bool isPolygonIntersection(int polygon) const;
	void setPolygonRender(int polygon, bool render);
	bool isPolygonRender(int polygon) const;
	void setPolygonImage(int polygon, const Ptr<Image> &image);
	Ptr<Image> getPolygonImage(int polygon);
	void setPolygonGPUTexture(int polygon, const Ptr<Texture> &texture);
	Ptr<Texture> getPolygonGPUTexture(int polygon) const;
	void setPolygonTexture(int polygon, const char *name);
	const char *getPolygonTexture(int polygon) const;
	int getPolygonTextureWidth(int polygon) const;
	int getPolygonTextureHeight(int polygon) const;
	void clearPolygonPoints(int polygon);
	int getNumPolygonPoints(int polygon) const;
	void removePolygonPoint(int polygon, int num);
	int addPolygonPoint(int polygon, const Math::vec3 &point);
	Math::vec3 getPolygonPoint(int polygon, int num) const;
	void setPolygonTexCoord(int polygon, const Math::vec2 &texcoord);
	Math::vec2 getPolygonTexCoord(int polygon, int num) const;
	void clearPolygonIndices(int polygon);
	int getNumPolygonIndices(int polygon) const;
	void removePolygonIndex(int polygon, int num);
	int addPolygonIndex(int polygon, int index);
	int getPolygonIndex(int polygon, int num) const;
};
typedef Ptr<WidgetCanvas> WidgetCanvasPtr;


class UNIGINE_API WidgetIcon : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_ICON; }
	static Ptr<WidgetIcon> create(const Ptr<Gui> &gui, const char *str = 0, int width = 0, int height = 0);
	static Ptr<WidgetIcon> create(const char *str = 0, int width = 0, int height = 0);
	void setToggleable(bool toggleable);
	bool isToggleable() const;
	void setToggled(bool toggled);
	bool isToggled() const;
	void setImage(const Ptr<Image> &image);
	Ptr<Image> getImage() const;
	void setTexture(const char *texture);
	const char *getTexture() const;
	void setTextAlign(int align);
	int getTextAlign() const;
	void setText(const char *text);
	const char *getText() const;
};
typedef Ptr<WidgetIcon> WidgetIconPtr;


class UNIGINE_API WidgetButton : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_BUTTON; }
	static Ptr<WidgetButton> create(const Ptr<Gui> &gui, const char *str = 0);
	static Ptr<WidgetButton> create(const char *str = 0);
	void setBackground(int background);
	int getBackground() const;
	void setToggleable(bool toggleable);
	bool isToggleable() const;
	void setToggled(bool toggled);
	bool isToggled() const;
	void setImage(const Ptr<Image> &image);
	Ptr<Image> getImage() const;
	void setButtonColor(const Math::vec4 &color);
	Math::vec4 getButtonColor() const;
	void setTexture(const char *texture);
	const char *getTexture() const;
	void setTextAlign(int align);
	int getTextAlign() const;
	void setText(const char *text);
	const char *getText() const;
	void setStyleImage(const Ptr<Image> &image);
	Ptr<Image> getStyleImage() const;
	void setStyleTexture(const char *texture);
	const char *getStyleTexture() const;
};
typedef Ptr<WidgetButton> WidgetButtonPtr;


class UNIGINE_API WidgetCheckBox : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_CHECK_BOX; }
	static Ptr<WidgetCheckBox> create(const Ptr<Gui> &gui, const char *str = 0);
	static Ptr<WidgetCheckBox> create(const char *str = 0);
	void setCheckedColor(const Math::vec4 &color);
	Math::vec4 getCheckedColor() const;
	void setUncheckedColor(const Math::vec4 &color);
	Math::vec4 getUncheckedColor() const;
	void setChecked(bool checked);
	bool isChecked() const;
	void setText(const char *text);
	const char *getText() const;
};
typedef Ptr<WidgetCheckBox> WidgetCheckBoxPtr;


class UNIGINE_API WidgetComboBox : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_COMBO_BOX; }
	static Ptr<WidgetComboBox> create(const Ptr<Gui> &gui);
	static Ptr<WidgetComboBox> create();
	void setImage(const Ptr<Image> &image);
	Ptr<Image> getImage() const;
	void setMainBackgroundColor(const Math::vec4 &color);
	Math::vec4 getMainBackgroundColor() const;
	void setListBackgroundColor(const Math::vec4 &color);
	Math::vec4 getListBackgroundColor() const;
	void setSelectionColor(const Math::vec4 &color);
	Math::vec4 getSelectionColor() const;
	void setBorderColor(const Math::vec4 &color);
	Math::vec4 getBorderColor() const;
	void setButtonColor(const Math::vec4 &color);
	Math::vec4 getButtonColor() const;
	void setTexture(const char *texture);
	const char *getTexture() const;
	void clear();
	int addItem(const char *str, int texture = -1);
	void removeItem(int num);
	int getNumItems() const;
	void setItemText(int item, const char *str);
	const char *getItemText(int item) const;
	void setItemData(int item, const char *str);
	const char *getItemData(int item) const;
	void setItemTexture(int item, int texture);
	int getItemTexture(int item) const;
	int findItemByText(const char *str) const;
	void setCurrentItem(int item);
	int getCurrentItem() const;
	const char *getCurrentItemText() const;
	const char *getCurrentItemData() const;
	void setStyleTextureIcon(const char *icon);
	const char *getStyleTextureIcon() const;
	void setStyleTextureButton(const char *button);
	const char *getStyleTextureButton() const;
	void setStyleTextureSelection(const char *selection);
	const char *getStyleTextureSelection() const;
	void setStyleTextureBackground(const char *background);
	const char *getStyleTextureBackground() const;
	void setStyleTextureBorder(const char *border);
	const char *getStyleTextureBorder() const;
};
typedef Ptr<WidgetComboBox> WidgetComboBoxPtr;


class UNIGINE_API WidgetMenuBox : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_MENU_BOX; }
	static Ptr<WidgetMenuBox> create(const Ptr<Gui> &gui, int x = 0, int y = 0);
	static Ptr<WidgetMenuBox> create(int x = 0, int y = 0);
	void setSpace(int x, int y);
	int getSpaceX() const;
	int getSpaceY() const;
	void setImage(const Ptr<Image> &image);
	Ptr<Image> getImage() const;
	void setBackgroundColor(const Math::vec4 &color);
	Math::vec4 getBackgroundColor() const;
	void setSelectionColor(const Math::vec4 &color);
	Math::vec4 getSelectionColor() const;
	void setCurrentItemColor(const Math::vec4 &color);
	Math::vec4 getCurrentItemColor() const;
	void setBorderColor(const Math::vec4 &color);
	Math::vec4 getBorderColor() const;
	void setTexture(const char *texture);
	const char *getTexture() const;
	void clear();
	int addItem(const char *str, int texture = -1);
	void removeItem(int item);
	int getNumItems() const;
	void setItemText(int item, const char *str);
	const char *getItemText(int item) const;
	void setItemData(int item, const char *str);
	const char *getItemData(int item) const;
	void setItemToolTip(int item, const char *str);
	const char *getItemToolTip(int item) const;
	void setItemEnabled(int item, bool enabled);
	int isItemEnabled(int item) const;
	void setItemTexture(int item, int texture);
	int getItemTexture(int item) const;
	void setItemSeparator(int item, int separator);
	int isItemSeparator(int item) const;
	void setItemSelectable(int item, int selectable);
	int isItemSelectable(int item) const;
	void setItemSpace(int item, int space);
	int getItemSpace(int item) const;
	void setItemWidget(int item, const Ptr<Widget> &w);
	Ptr<Widget> getItemWidget(int item) const;
	void setCurrentItem(int item);
	int getCurrentItem() const;
	void setCurrentItemHighlight(bool highlight);
	bool isCurrentItemHighlight() const;
	void resetCurrentItem();
	const char *getCurrentItemText() const;
	const char *getCurrentItemData() const;
};
typedef Ptr<WidgetMenuBox> WidgetMenuBoxPtr;


class UNIGINE_API WidgetMenuBar : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_MENU_BAR; }
	static Ptr<WidgetMenuBar> create(const Ptr<Gui> &gui, int x = 0, int y = 0);
	static Ptr<WidgetMenuBar> create(int x = 0, int y = 0);
	void setSpace(int x, int y);
	int getSpaceX() const;
	int getSpaceY() const;
	void clear();
	void setSelectionColor(const Math::vec4 &color);
	Math::vec4 getSelectionColor() const;
	int addItem(const char *str);
	int addItem(const char *str, const Ptr<WidgetMenuBox> &menu);
	void removeItem(int item);
	int getNumItems() const;
	void setItemText(int item, const char *str);
	const char *getItemText(int item) const;
	void setItemData(int item, const char *str);
	const char *getItemData(int item) const;
	void setItemEnabled(int item, bool enabled);
	bool isItemEnabled(int item) const;
	void setItemSpace(int item, int space);
	int getItemSpace(int item) const;
	void setItemMenu(int item, const Ptr<WidgetMenuBox> &menu);
	Ptr<WidgetMenuBox> getItemMenu(int item) const;
};
typedef Ptr<WidgetMenuBar> WidgetMenuBarPtr;


class UNIGINE_API WidgetListBox : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_LIST_BOX; }
	static Ptr<WidgetListBox> create(const Ptr<Gui> &gui);
	static Ptr<WidgetListBox> create();
	void setMultiSelection(bool selection);
	bool isMultiSelection() const;
	void setImage(const Ptr<Image> &image);
	Ptr<Image> getImage() const;
	void setTexture(const char *texture);
	const char *getTexture() const;
	void setSelectionColor(const Math::vec4 &color);
	Math::vec4 getSelectionColor() const;
	void setIconsEnabled(bool enabled);
	bool isIconsEnabled() const;
	void setIconsAlphaEnabled(bool enabled);
	bool isIconsAlphaEnabled() const;
	void setIconsResolution(int resolution);
	int getIconsResolution() const;
	void clear();
	int addItem(const char *str, int texture = -1);
	void removeItem(int item);
	int getNumItems() const;
	void setItemText(int item, const char *str);
	const char *getItemText(int item) const;
	void setItemData(int item, const char *str);
	const char *getItemData(int item) const;
	void setItemColor(int item, const Math::vec4 &color);
	Math::vec4 getItemColor(int item) const;
	void setItemTexture(int item, int texture);
	int getItemTexture(int item) const;
	void setItemIcon(int item, const Ptr<Texture> &texture);
	Ptr<Texture> getItemIcon(int item) const;
	void setItemSelectable(int item, int selectable);
	int isItemSelectable(int item) const;
	void setCurrentItem(int item);
	int getCurrentItem() const;
	const char *getCurrentItemText() const;
	const char *getCurrentItemData() const;
	int getNumSelectedItems() const;
	int getSelectedItem(int num) const;
	void setItemSelected(int item, int selected);
	int isItemSelected(int item) const;
	void clearSelection();
	void showItem(int item);
};
typedef Ptr<WidgetListBox> WidgetListBoxPtr;


class UNIGINE_API WidgetTreeBox : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_TREE_BOX; }
	static Ptr<WidgetTreeBox> create(const Ptr<Gui> &gui);
	static Ptr<WidgetTreeBox> create();
	void setEditable(bool editable);
	bool isEditable() const;
	void setMultiSelection(bool selection);
	bool isMultiSelection() const;
	void setImage(const Ptr<Image> &image);
	Ptr<Image> getImage() const;
	void setSelectionColor(const Math::vec4 &color);
	Math::vec4 getSelectionColor() const;
	void setTexture(const char *texture);
	const char *getTexture() const;
	void clear();
	int addItem(const char *str, int texture = -1);
	void getItems(Vector<int> &items) const;
	void removeItem(int item);
	int getNumItems() const;
	int getItem(int num) const;
	void setItemText(int item, const char *str);
	const char *getItemText(int item) const;
	void setItemData(int item, const char *str);
	const char *getItemData(int item) const;
	void setItemColor(int item, const Math::vec4 &color);
	Math::vec4 getItemColor(int item) const;
	void setItemTexture(int item, int texture);
	int getItemTexture(int item) const;
	void setItemSelectable(int item, int selectable);
	int isItemSelectable(int item) const;
	void setItemHidden(int item, int hidden);
	int isItemHidden(int item) const;
	void setItemFolded(int item, int folded);
	int isItemFolded(int item) const;
	void setItemParent(int item, int parent);
	int getItemParent(int item) const;
	void addItemChild(int item, int child);
	void removeItemChild(int item, int child);
	int getNumItemChildren(int item) const;
	int getItemChild(int item, int num) const;
	int isItemChild(int item, int child) const;
	void setCurrentItem(int item);
	int getCurrentItem() const;
	const char *getCurrentItemText() const;
	const char *getCurrentItemData() const;
	int getNumSelectedItems() const;
	int getItemUnderCursor() const;
	int getSelectedItem(int num) const;
	void setItemSelected(int item, int selected);
	int isItemSelected(int item) const;
	void clearSelection();
	void showItem(int item);
	void setNeedSortChildren(bool children);
	bool isNeedSortChildren() const;
};
typedef Ptr<WidgetTreeBox> WidgetTreeBoxPtr;


class UNIGINE_API WidgetSpacer : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_SPACER; }
	static Ptr<WidgetSpacer> create(const Ptr<Gui> &gui);
	static Ptr<WidgetSpacer> create();
	void setOrientation(int orientation);
	int getOrientation() const;
};
typedef Ptr<WidgetSpacer> WidgetSpacerPtr;


class UNIGINE_API WidgetSlider : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_SLIDER; }
	static Ptr<WidgetSlider> create(const Ptr<Gui> &gui, int min = 0, int max = 100, int value = 0);
	static Ptr<WidgetSlider> create(int min = 0, int max = 100, int value = 0);
	void setBackgroundColor(const Math::vec4 &color);
	Math::vec4 getBackgroundColor() const;
	void setButtonColor(const Math::vec4 &color);
	Math::vec4 getButtonColor() const;
	void setOrientation(int orientation);
	int getOrientation() const;
	void setButtonWidth(int width);
	int getButtonWidth() const;
	void setButtonHeight(int height);
	int getButtonHeight() const;
	void setMinValue(int value);
	int getMinValue() const;
	void setMaxValue(int value);
	int getMaxValue() const;
	void setMinExpand(int expand);
	int getMinExpand() const;
	void setMaxExpand(int expand);
	int getMaxExpand() const;
	void setValue(int value);
	int getValue() const;
};
typedef Ptr<WidgetSlider> WidgetSliderPtr;


class UNIGINE_API WidgetSpinBox : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_SPIN_BOX; }
	static Ptr<WidgetSpinBox> create(const Ptr<Gui> &gui, int min = 0, int max = 100, int value = 0, int step = 1);
	static Ptr<WidgetSpinBox> create(int min = 0, int max = 100, int value = 0, int step = 1);
	void setButtonColor(const Math::vec4 &color);
	Math::vec4 getButtonColor() const;
	void setMinValue(int value);
	int getMinValue() const;
	void setMaxValue(int value);
	int getMaxValue() const;
	void setMinExpand(int expand);
	int getMinExpand() const;
	void setMaxExpand(int expand);
	int getMaxExpand() const;
	void setValue(int value);
	int getValue() const;
	void setStep(int step);
	int getStep() const;
};
typedef Ptr<WidgetSpinBox> WidgetSpinBoxPtr;


class UNIGINE_API WidgetSpinBoxDouble : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_SPIN_BOX_DOUBLE; }
	static Ptr<WidgetSpinBoxDouble> create(const Ptr<Gui> &gui, double min = 0.0, double max = 100.0, double value = 0.0, double step = 1.0);
	static Ptr<WidgetSpinBoxDouble> create(double min = 0.0, double max = 100.0, double value = 0.0, double step = 1.0);
	void setButtonColor(const Math::vec4 &color);
	Math::vec4 getButtonColor() const;
	void setMinValue(double value);
	double getMinValue() const;
	void setMaxValue(double value);
	double getMaxValue() const;
	void setMinExpand(double expand);
	double getMinExpand() const;
	void setMaxExpand(double expand);
	double getMaxExpand() const;
	void setValue(double value);
	double getValue() const;
	void setStep(double step);
	double getStep() const;
};
typedef Ptr<WidgetSpinBoxDouble> WidgetSpinBoxDoublePtr;


class UNIGINE_API WidgetEditLine : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_EDIT_LINE; }
	static Ptr<WidgetEditLine> create(const Ptr<Gui> &gui, const char *str = 0);
	static Ptr<WidgetEditLine> create(const char *str = 0);
	void setBackgroundColor(const Math::vec4 &color);
	Math::vec4 getBackgroundColor() const;
	void setSelectionColor(const Math::vec4 &color);
	Math::vec4 getSelectionColor() const;
	void setBorderColor(const Math::vec4 &color);
	Math::vec4 getBorderColor() const;
	void setEditable(bool editable);
	bool isEditable() const;
	void setPassword(bool password);
	bool isPassword() const;
	void setValidator(int validator);
	int getValidator() const;
	void setBackground(int background);
	int getBackground() const;
	void setCapacity(int capacity);
	int getCapacity() const;
	void setCursor(int cursor);
	int getCursor() const;
	void setSelection(int selection);
	int getSelection() const;
	String getSelectionText() const;
	void clearSelectionText();
	void setText(const char *text);
	const char *getText() const;
};
typedef Ptr<WidgetEditLine> WidgetEditLinePtr;


class UNIGINE_API WidgetEditText : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_EDIT_TEXT; }
	static Ptr<WidgetEditText> create(const Ptr<Gui> &gui, const char *str = 0);
	static Ptr<WidgetEditText> create(const char *str = 0);
	void setEditable(bool editable);
	bool isEditable() const;
	void setBackground(int background);
	int getBackground() const;
	void setTokenColor(const char *token, const Math::vec4 &color);
	void setTokensColor(const char *tokens, const Math::vec4 &color);
	Math::vec4 getTokenColor(const char *token) const;
	void setBackgroundColor(const Math::vec4 &color);
	Math::vec4 getBackgroundColor() const;
	void setSelectionColor(const Math::vec4 &color);
	Math::vec4 getSelectionColor() const;
	void setNumberColor(const Math::vec4 &color);
	Math::vec4 getNumberColor() const;
	void setCursor(int position, int line);
	int getCursorPosition() const;
	int getCursorLine() const;
	void setSelection(int position, int line);
	int getSelectionPosition() const;
	int getSelectionLine() const;
	String getSelectionText() const;
	void clearSelectionText();
	int addLine(const char *str = 0);
	void removeLine(int line);
	int getNumLines() const;
	void setLineText(int line, const char *str);
	String getLineText(int line) const;
	void setText(const char *text);
	const char *getText() const;
};
typedef Ptr<WidgetEditText> WidgetEditTextPtr;


class UNIGINE_API WidgetWindow : public Widget
{
public:
	static bool convertible(Widget *obj) {
		if(!obj)
			return false;
		int type = obj->getType();
		switch(type)
		{
			case Widget::WIDGET_WINDOW:
			case Widget::WIDGET_DIALOG:
			case Widget::WIDGET_DIALOG_MESSAGE:
			case Widget::WIDGET_DIALOG_FILE:
			case Widget::WIDGET_DIALOG_COLOR:
			case Widget::WIDGET_DIALOG_IMAGE:
			return true;
		}
		return false;
	}
	static Ptr<WidgetWindow> create(const Ptr<Gui> &gui, const char *str = 0, int x = 0, int y = 0);
	static Ptr<WidgetWindow> create(const char *str = 0, int x = 0, int y = 0);
	void setMoveable(bool moveable);
	bool isMoveable() const;
	void setSizeable(bool sizeable);
	bool isSizeable() const;
	void setTitleable(bool titleable);
	bool isTitleable() const;
	void setBlendable(bool blendable);
	bool isBlendable() const;
	void setFloatable(bool floatable);
	bool isFloatable() const;
	void setSnapDistance(int distance);
	int getSnapDistance() const;
	void setColor(const Math::vec4 &color);
	Math::vec4 getColor() const;
	void setBackgroundColor(const Math::vec4 &color);
	Math::vec4 getBackgroundColor() const;
	void setBorderColor(const Math::vec4 &color);
	Math::vec4 getBorderColor() const;
	void setTransform(const Math::mat4 &transform);
	Math::mat4 getTransform() const;
	void setMaxWidth(int width);
	int getMaxWidth() const;
	void setMaxHeight(int height);
	int getMaxHeight() const;
	void setSpace(int x, int y);
	int getSpaceX() const;
	int getSpaceY() const;
	void setPadding(int l, int r, int t, int b);
	int getPaddingLeft() const;
	int getPaddingRight() const;
	int getPaddingTop() const;
	int getPaddingBottom() const;
	void setDragAreaEnabled(bool enabled);
	bool isDragAreaEnabled() const;
	void setDragAreaBackground(int background);
	int getDragAreaBackground() const;
	void setDragAreaBackgroundColor(const Math::vec4 &color);
	Math::vec4 getDragAreaBackgroundColor() const;
	void setDragAreaPadding(int l, int r, int t, int b);
	int getDragAreaPaddingLeft() const;
	int getDragAreaPaddingRight() const;
	int getDragAreaPaddingTop() const;
	int getDragAreaPaddingBottom() const;
	void setTextAlign(int align);
	int getTextAlign() const;
	void setText(const char *text);
	const char *getText() const;
};
typedef Ptr<WidgetWindow> WidgetWindowPtr;


class UNIGINE_API WidgetDialog : public WidgetWindow
{
public:
	static bool convertible(Widget *obj) {
		if(!obj)
			return false;
		int type = obj->getType();
		switch(type)
		{
			case Widget::WIDGET_DIALOG:
			case Widget::WIDGET_DIALOG_MESSAGE:
			case Widget::WIDGET_DIALOG_FILE:
			case Widget::WIDGET_DIALOG_COLOR:
			case Widget::WIDGET_DIALOG_IMAGE:
			return true;
		}
		return false;
	}
	static Ptr<WidgetDialog> create(const Ptr<Gui> &gui, const char *str = 0, int x = 0, int y = 0);
	static Ptr<WidgetDialog> create(const char *str = 0, int x = 0, int y = 0);
	bool isDone() const;
	int getResult() const;
	bool isOkClicked() const;
	void setOkText(const char *text);
	const char *getOkText() const;
	Ptr<WidgetButton> getOkButton() const;
	int isCancelClicked() const;
	void setCancelText(const char *text);
	const char *getCancelText() const;
	Ptr<WidgetButton> getCancelButton() const;
	int isCloseClicked() const;
	void setCloseText(const char *text);
	const char *getCloseText() const;
	Ptr<WidgetButton> getCloseButton() const;
};
typedef Ptr<WidgetDialog> WidgetDialogPtr;


class UNIGINE_API WidgetDialogMessage : public WidgetDialog
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_DIALOG_MESSAGE; }
	static Ptr<WidgetDialogMessage> create(const Ptr<Gui> &gui, const char *str = 0);
	static Ptr<WidgetDialogMessage> create(const char *str = 0);
	void setMessageHidden(bool hidden);
	bool isMessageHidden() const;
	void setMessageFont(const char *name);
	void setMessageFontSize(int size);
	void setMessageFontColor(const Math::vec4 &color);
	void setMessageFontRich(int rich);
	void setMessageText(const char *text);
	const char *getMessageText() const;
};
typedef Ptr<WidgetDialogMessage> WidgetDialogMessagePtr;


class UNIGINE_API WidgetDialogFile : public WidgetDialog
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_DIALOG_FILE; }
	static Ptr<WidgetDialogFile> create(const Ptr<Gui> &gui, const char *str = 0);
	static Ptr<WidgetDialogFile> create(const char *str = 0);
	void setPath(const char *path);
	const char *getPath() const;
	void setFile(const char *file);
	const char *getFile() const;
	void setTabs(const char *tabs);
	const char *getTabs() const;
	void setFilter(const char *filter);
	const char *getFilter() const;
};
typedef Ptr<WidgetDialogFile> WidgetDialogFilePtr;


class UNIGINE_API WidgetDialogColor : public WidgetDialog
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_DIALOG_COLOR; }
	static Ptr<WidgetDialogColor> create(const Ptr<Gui> &gui, const char *str = 0);
	static Ptr<WidgetDialogColor> create(const char *str = 0);
	void setColor(const Math::vec4 &color);
	Math::vec4 getColor() const;
	void setWebColor(const char *color);
	const char *getWebColor() const;
	void setPaletteColors(const char *colors);
	const char *getPaletteColors() const;
};
typedef Ptr<WidgetDialogColor> WidgetDialogColorPtr;


class UNIGINE_API WidgetDialogImage : public WidgetDialog
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_DIALOG_IMAGE; }
	static Ptr<WidgetDialogImage> create(const Ptr<Gui> &gui, const char *str = 0);
	static Ptr<WidgetDialogImage> create(const char *str = 0);
	void setImage(const Ptr<Image> &image);
	Ptr<Image> getImage() const;
	void setTexture(const char *texture);
	const char *getTexture() const;
};
typedef Ptr<WidgetDialogImage> WidgetDialogImagePtr;


class UNIGINE_API WidgetManipulator : public Widget
{
public:
	static bool convertible(Widget *obj) {
		if(!obj)
			return false;
		int type = obj->getType();
		switch(type)
		{
			case Widget::WIDGET_MANIPULATOR:
			case Widget::WIDGET_MANIPULATOR_TRANSLATOR:
			case Widget::WIDGET_MANIPULATOR_ROTATOR:
			case Widget::WIDGET_MANIPULATOR_SCALER:
			return true;
		}
		return false;
	}
	static Ptr<WidgetManipulator> create(const Ptr<Gui> &gui);
	static Ptr<WidgetManipulator> create();
	int getFocusedAxis() const;
	bool isFocusAxis() const;
	bool isHoverAxis() const;
	void setRenderGui(const Ptr<Gui> &gui);
	Ptr<Gui> getRenderGui() const;
	void setMask(int mask);
	int getMask() const;
	void setStep(float step);
	float getStep() const;
	void setSize(int size);
	int getSize() const;
	void setColor(const Math::vec4 &color);
	Math::vec4 getColor() const;
	void setBasis(const Math::Mat4 &basis);
	Math::Mat4 getBasis() const;
	void setTransform(const Math::Mat4 &transform);
	Math::Mat4 getTransform() const;
	void setProjection(const Math::mat4 &projection);
	Math::mat4 getProjection() const;
	void setModelview(const Math::Mat4 &modelview);
	Math::Mat4 getModelview() const;

	enum
	{
		MASK_X = 1 << 0,
		MASK_Y = 1 << 1,
		MASK_Z = 1 << 2,
		MASK_XYZ = (MASK_X | MASK_Y | MASK_Z),
	};
};
typedef Ptr<WidgetManipulator> WidgetManipulatorPtr;


class UNIGINE_API WidgetManipulatorTranslator : public WidgetManipulator
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_MANIPULATOR_TRANSLATOR; }
	static Ptr<WidgetManipulatorTranslator> create(const Ptr<Gui> &gui);
	static Ptr<WidgetManipulatorTranslator> create();

	enum
	{
		AXIS_UNKNOWN = -1,
		AXIS_X,
		AXIS_Y,
		AXIS_Z,
		AXIS_YZ,
		AXIS_ZX,
		AXIS_XY,
		NUM_AXIS,
	};
};
typedef Ptr<WidgetManipulatorTranslator> WidgetManipulatorTranslatorPtr;


class UNIGINE_API WidgetManipulatorRotator : public WidgetManipulator
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_MANIPULATOR_ROTATOR; }
	static Ptr<WidgetManipulatorRotator> create(const Ptr<Gui> &gui);
	static Ptr<WidgetManipulatorRotator> create();

	enum
	{
		AXIS_UNKNOWN = -1,
		AXIS_X,
		AXIS_Y,
		AXIS_Z,
		AXIS_T,
		NUM_AXIS,
	};
};
typedef Ptr<WidgetManipulatorRotator> WidgetManipulatorRotatorPtr;


class UNIGINE_API WidgetManipulatorScaler : public WidgetManipulator
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_MANIPULATOR_SCALER; }
	static Ptr<WidgetManipulatorScaler> create(const Ptr<Gui> &gui);
	static Ptr<WidgetManipulatorScaler> create();

	enum
	{
		AXIS_UNKNOWN = -1,
		AXIS_X,
		AXIS_Y,
		AXIS_Z,
		AXIS_XYZ,
		NUM_AXIS,
	};
};
typedef Ptr<WidgetManipulatorScaler> WidgetManipulatorScalerPtr;


class UNIGINE_API WidgetDragArea : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_DRAG_AREA; }
	static Ptr<WidgetDragArea> create(const Ptr<Gui> &gui);
	static Ptr<WidgetDragArea> create();
	void setBackground(int background);
	int getBackground() const;
	void setBackgroundColor(const Math::vec4 &color);
	Math::vec4 getBackgroundColor() const;
};
typedef Ptr<WidgetDragArea> WidgetDragAreaPtr;

class WidgetExternBase;

class UNIGINE_API WidgetExtern : public Widget
{
public:
	static bool convertible(Widget *obj) { return obj && obj->getType() == Widget::WIDGET_EXTERN; }
	static Ptr<WidgetExtern> create(const Ptr<Gui> &gui, int class_id);
	static Ptr<WidgetExtern> create(int class_id);
	WidgetExternBase *getWidgetExtern() const;
	int getClassID() const;
};
typedef Ptr<WidgetExtern> WidgetExternPtr;


class UNIGINE_API  WidgetExternBase : public virtual Base
{
public:
	WidgetExternBase(const Ptr<Gui> &gui);
	WidgetExternBase(void * widget);
	virtual ~WidgetExternBase();
	virtual int getClassID() = 0;
	Ptr<Gui> getGui() const;
	Ptr<Widget> getWidget() const;
	Ptr<WidgetExtern> getWidgetExtern() const;
	typedef WidgetExternBase *(*ConstructorFunc)(void *widget);
	static void addClassID(int class_id, ConstructorFunc func);
	template <class Type> static void addClassID(int class_id) { addClassID(class_id, constructor<Type>); }
	virtual void update(float ifps);
	virtual void checkCallbacks(int x, int y);
	virtual void keyPress(unsigned int key);
	virtual void textPress(unsigned int unicode);
	virtual int getKeyActivity(unsigned int key);
	virtual void arrange();
	virtual void expand(int width, int height);
	virtual void updatePositions();
	virtual void destroy();
	virtual void render();
protected:
	int get_position_x() const;
	int get_position_y() const;
	int get_default_width() const;
	int get_default_height() const;
	void set_width(int width) const;
	int get_width() const;
	void set_height(int height) const;
	int get_height() const;
	void check_default_callbacks(int x, int y) const;
	void check_clicked_callbacks(int x, int y) const;
	void check_pressed_callbacks(int x, int y) const;
	float get_expose() const;
	float get_clicked_fade() const;
	float get_selection_fade() const;
	Math::vec4 get_color(int enabled = 1) const;
	Math::vec4 get_focus_color(float fade, int enabled = 1) const;
	Math::vec4 get_selection_color(float fade, int enabled = 1) const;
	Math::vec4 get_text_color(const Math::vec4 &color, float fade, int enabled = 1) const;
	Math::vec4 get_text_color(float fade, int enabled = 1) const;
	int get_text_height() const;
	int get_text_width(unsigned int code) const;
	int get_text_space_width() const;
	void get_text_size(const char *str, int &width, int &height, int expand = 0) const;
	void render_text(int x, int y, const Math::vec4 &color, const char *str, int width, int height) const;
	void get_text_size(const unsigned int *str, int &width, int &height) const;
	void render_text(int x, int y, const Math::vec4 &color, const unsigned int *str) const;
	void push_color() const;
	void pop_color() const;
	void set_color(const Math::vec4 &color) const;
	void push_transform() const;
	void pop_transform() const;
	void set_transform(const Math::mat4 &transform) const;
	void set_translate(int x, int y) const;
	void set_scale(float x, float y) const;
	void set_blend_func(int src, int dest) const;
private:
	template <class Type> static WidgetExternBase *constructor(void *widget) { return new Type(widget); }
	struct WidgetExternData;
	WidgetExternData *data;
};
typedef Ptr<WidgetExternBase> WidgetExternBasePtr;

} // namespace Unigine
