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

#include "UnigineXml.h"
#include "UnigineStreams.h"
#include "UnigineMathLib.h"
#include "UnigineGUID.h"
#include "UnigineCallback.h"

namespace Unigine
{

class Variable;
class Node;
class Material;
class Curve2d;
class PropertyParameter;

class UNIGINE_API Property : public APIInterface
{
public:
	static Ptr<Property> create();

	enum
	{
		PARAMETER_INT = 0,
		PARAMETER_FLOAT,
		PARAMETER_DOUBLE,
		PARAMETER_TOGGLE,
		PARAMETER_SWITCH,
		PARAMETER_STRING,
		PARAMETER_COLOR,
		PARAMETER_VEC2,
		PARAMETER_VEC3,
		PARAMETER_VEC4,
		PARAMETER_DVEC2,
		PARAMETER_DVEC3,
		PARAMETER_DVEC4,
		PARAMETER_IVEC2,
		PARAMETER_IVEC3,
		PARAMETER_IVEC4,
		PARAMETER_MASK,
		PARAMETER_FILE,
		PARAMETER_PROPERTY,
		PARAMETER_MATERIAL,
		PARAMETER_NODE,
		PARAMETER_CURVE2D,
		PARAMETER_ARRAY,
		PARAMETER_STRUCT,
	};

	enum
	{
		PARAMETER_MASK_GENERAL = 0,
		PARAMETER_MASK_PHYSICS_INTERSECTION,
		PARAMETER_MASK_INTERSECTION,
		PARAMETER_MASK_COLLISION,
		PARAMETER_MASK_EXCLUSION,
		PARAMETER_MASK_VIEWPORT,
		PARAMETER_MASK_SHADOWS,
		PARAMETER_MASK_MATERIAL,
		PARAMETER_MASK_SOUND_SOURCE,
		PARAMETER_MASK_SOUND_REVERB,
		PARAMETER_MASK_SOUND_OCCLUSION,
		PARAMETER_MASK_NAVIGATION,
		PARAMETER_MASK_OBSTACLE,
		PARAMETER_MASK_PHYSICAL,
		PARAMETER_MASK_FIELD,
	};

	enum CALLBACK_INDEX
	{
		CALLBACK_RELOADED = 0,
		CALLBACK_MOVED,
		CALLBACK_RENAMED,
		CALLBACK_REPARENTED,
		CALLBACK_PARAMETER_CHANGED,
		CALLBACK_DESTROY,
	};

	enum
	{
		RESTORE_MODE_REPLACE = 0,
		RESTORE_MODE_OVERRIDES,
		RESTORE_MODE_MERGE,
		RESTORE_MODE_FULL,
	};
	int getID() const;
	bool load(const char *path);
	bool load();
	bool save(const char *path);
	bool save();
	bool reload();
	bool isBase() const;
	bool isHidden() const;
	bool isEditable() const;
	bool isInternal() const;
	bool isManual() const;
	bool isEngine() const;
	bool canSaveInFile() const;
	void setName(const char *name);
	const char *getName() const;
	UGUID getGUID() const;
	void setPath(const char *path);
	const char *getPath() const;
	void setFileGUID(const UGUID &fileguid);
	UGUID getFileGUID() const;
	void setEditable(int editable);
	const char *getTooltip() const;
	bool isHierarchyValid() const;
	int setParent(const Ptr<Property> &property, bool save_all_values = false);
	Ptr<Property> getParent() const;
	int getNumChildren() const;
	Ptr<Property> getChild(int num) const;
	bool isParent(const char *name) const;
	bool isParent(const UGUID &guid) const;
	Ptr<Property> clone(const char *name, const char *path, const UGUID &guid);
	Ptr<Property> clone(const char *name, const char *path);
	Ptr<Property> clone(const char *name);
	Ptr<Property> clone();
	Ptr<Property> inherit(const char *name, const char *path, const UGUID &guid);
	Ptr<Property> inherit(const char *name, const char *path);
	Ptr<Property> inherit(const char *name);
	Ptr<Property> inherit();
	bool hasOverrides() const;
	Ptr<PropertyParameter> getParameterPtr() const;
	Ptr<PropertyParameter> getParameterPtr(const char *name);
	Ptr<PropertyParameter> getParameterPtr(int id);
	int getNumStructs() const;
	int findStruct(const char *name) const;
	const char *getStructName(int num) const;
	bool loadXml(const Ptr<Xml> &xml);
	bool saveXml(const Ptr<Xml> &xml) const;
	bool loadWorld(const Ptr<Xml> &xml);
	bool saveWorld(const Ptr<Xml> &xml, int force = 0) const;
	bool saveState(const Ptr<Stream> &stream) const;
	bool restoreState(const Ptr<Stream> &stream, int restore_mode = 0);
	int parameterTypeByName(const char *param_type) const;
	const char *parameterNameByType(int param_type) const;
	Ptr<Node> getNode() const;
	void *addCallback(Property::CALLBACK_INDEX callback, CallbackBase1<Ptr<Property>> *func);
	void *addCallback(Property::CALLBACK_INDEX callback, CallbackBase2<Ptr<Property>, int> *func);
	bool removeCallback(Property::CALLBACK_INDEX callback, void *id);
	void clearCallbacks(Property::CALLBACK_INDEX callback);
};
typedef Ptr<Property> PropertyPtr;


class UNIGINE_API PropertyParameter : public APIInterface
{
public:
	int getID() const;
	Ptr<Property> getProperty() const;
	UGUID getPropertyGUID() const;
	Ptr<PropertyParameter> getParent() const;
	int getNumChildren() const;
	bool isChild(const Ptr<PropertyParameter> &parameter) const;
	int findChild(const char *name) const;
	Ptr<PropertyParameter> getChild(int num) const;
	Ptr<PropertyParameter> getChild(const char *name) const;
	int getChildIndex(const Ptr<PropertyParameter> &parameter) const;
	bool isExist() const;
	bool isHidden() const;
	bool isInherited() const;
	bool isOverridden() const;
	int getType() const;
	const char *getName() const;
	const char *getTitle() const;
	const char *getTooltip() const;
	const char *getGroup() const;
	const char *getFilter() const;
	bool isInt() const;
	bool isFloat() const;
	bool isDouble() const;
	bool isToggle() const;
	bool isSwitch() const;
	bool isString() const;
	bool isColor() const;
	bool isVec2() const;
	bool isVec3() const;
	bool isVec4() const;
	bool isDVec2() const;
	bool isDVec3() const;
	bool isDVec4() const;
	bool isIVec2() const;
	bool isIVec3() const;
	bool isIVec4() const;
	bool isMask() const;
	bool isFile() const;
	bool isProperty() const;
	bool isMaterial() const;
	bool isNode() const;
	bool isCurve2d() const;
	bool isArray() const;
	bool isStruct() const;
	void setValue(int value);
	void setValue(float value);
	void setValue(double value);
	void setValue(const char *value);
	void setValue(const Math::vec2 &value);
	void setValue(const Math::vec3 &value);
	void setValue(const Math::vec4 &value);
	void setValue(const Math::dvec2 &value);
	void setValue(const Math::dvec3 &value);
	void setValue(const Math::dvec4 &value);
	void setValue(const Math::ivec2 &value);
	void setValue(const Math::ivec3 &value);
	void setValue(const Math::ivec4 &value);
	void setValue(const UGUID &value);
	void setValue(const Ptr<Node> &value);
	void setValue(const Ptr<Material> &value);
	void setValue(const Ptr<Property> &value);
	void setValue(const Ptr<Curve2d> &value);
	void resetValue();
	void setValueInt(int val);
	int getValueInt() const;
	void setValueFloat(float val);
	float getValueFloat() const;
	void setValueDouble(double val);
	double getValueDouble() const;
	void setValueColor(const Math::vec4 &color);
	Math::vec4 getValueColor() const;
	void setValueString(const char *val);
	const char *getValueString() const;
	void setValueVec2(const Math::vec2 &vec2);
	Math::vec2 getValueVec2() const;
	void setValueVec3(const Math::vec3 &vec3);
	Math::vec3 getValueVec3() const;
	void setValueVec4(const Math::vec4 &vec4);
	Math::vec4 getValueVec4() const;
	void setValueDVec2(const Math::dvec2 &dvec2);
	Math::dvec2 getValueDVec2() const;
	void setValueDVec3(const Math::dvec3 &dvec3);
	Math::dvec3 getValueDVec3() const;
	void setValueDVec4(const Math::dvec4 &dvec4);
	Math::dvec4 getValueDVec4() const;
	void setValueIVec2(const Math::ivec2 &ivec2);
	Math::ivec2 getValueIVec2() const;
	void setValueIVec3(const Math::ivec3 &ivec3);
	Math::ivec3 getValueIVec3() const;
	void setValueIVec4(const Math::ivec4 &ivec4);
	Math::ivec4 getValueIVec4() const;
	void setValueProperty(const Ptr<Property> &property);
	Ptr<Property> getValueProperty() const;
	void setValueMaterial(const Ptr<Material> &material);
	Ptr<Material> getValueMaterial() const;
	void setValueNode(const Ptr<Node> &node);
	Ptr<Node> getValueNode() const;
	void setValueNodeID(int valuenodeid);
	int getValueNodeID() const;
	void setValueToggle(bool toggle);
	bool getValueToggle() const;
	void setValueSwitch(int val);
	int getValueSwitch() const;
	void setValueMask(int mask);
	int getValueMask() const;
	void setValueGUID(const UGUID &valueguid);
	UGUID getValueGUID() const;
	void setValueCurve2d(const Ptr<Curve2d> &curve2d);
	Ptr<Curve2d> getValueCurve2d() const;
	bool setValue(const Variable &value);
	Variable  getValue() const;
	int getIntMinValue() const;
	int getIntMaxValue() const;
	float getFloatMinValue() const;
	float getFloatMaxValue() const;
	double getDoubleMinValue() const;
	double getDoubleMaxValue() const;
	bool hasSliderMinValue() const;
	bool hasSliderMaxValue() const;
	int getSliderLog10() const;
	int getSliderMinExpand() const;
	int getSliderMaxExpand() const;
	int getSwitchNumItems() const;
	const char *getSwitchItemName(int item) const;
	int getSwitchItemValue(int item) const;
	void setSwitchItem(int item);
	int getSwitchItem() const;
	int getMaskType() const;
	void setValueFile(const char *file);
	const char *getValueFile() const;
	const char *getValueFile(bool fast) const;
	bool getFileIsAsset() const;
	bool getFileIsRuntime() const;
	bool getFileIsAbsPath() const;
	bool isFileExist() const;
	bool isValuePropertyInternal() const;
	bool isValueFitFilter(const Ptr<Node> &value) const;
	bool isValueFitFilter(const Ptr<Property> &value) const;
	bool isValueFitFilter(const Ptr<Material> &value) const;
	bool isValueFitFilter(const UGUID &value) const;
	Ptr<Curve2d> getValueCurve2dOverride() const;
	void setArraySize(int size);
	int getArraySize() const;
	int getArrayType() const;
	const char *getArrayTypeName() const;
	int getArrayDim() const;
	const char *getStructName() const;
	bool saveState(const Ptr<Stream> &stream) const;
	bool restoreState(const Ptr<Stream> &stream, int restore_mode = 0);
};
typedef Ptr<PropertyParameter> PropertyParameterPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Properties
{
protected:
	

public:
	static int isInitialized(); 

	enum CALLBACK_INDEX
	{
		CALLBACK_CREATED = 0,
		CALLBACK_MOVED,
		CALLBACK_RENAMED,
		CALLBACK_REPARENTED,
		CALLBACK_REMOVED,
	};
	static Ptr<Property> loadProperty(const char *path);
	static int saveProperties();
	static void reloadProperties();
	static int getNumProperties();
	static Ptr<Property> getProperty(int num);
	static const char *getPropertyName(int num);
	static bool isProperty(const char *name);
	static bool isManualProperty(const char *name);
	static bool isProperty(const UGUID &guid);
	static Ptr<Property> findProperty(const char *name);
	static Ptr<Property> findManualProperty(const char *name);
	static Ptr<Property> findPropertyByGUID(const UGUID &guid);
	static Ptr<Property> findPropertyByFileGUID(const UGUID &guid);
	static Ptr<Property> findPropertyByPath(const char *path);
	static Ptr<Property> cloneProperty(const UGUID &guid, const char *name = 0, const char *path = 0);
	static Ptr<Property> inheritProperty(const UGUID &guid, const char *name = 0, const char *path = 0);
	static bool renameProperty(const UGUID &guid, const char *new_name);
	static bool reparentProperty(const UGUID &guid, const UGUID &new_parent, bool save_all_values = false);
	static bool removeProperty(const UGUID &guid, bool remove_file = false, bool remove_children = true);
	static bool replaceProperty(const Ptr<Property> &property, const Ptr<Property> &new_property);
	static void *addCallback(Properties::CALLBACK_INDEX callback, CallbackBase1<Ptr<Property>> *func);
	static bool removeCallback(Properties::CALLBACK_INDEX callback, void *id);
	static void clearCallbacks(Properties::CALLBACK_INDEX callback);
};

} // namespace Unigine
