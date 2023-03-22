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


#pragma once

#include "UnigineType.h"
#include "UnigineXml.h"
#include "UnigineGui.h"
#include "UniginePath.h"
#include "UnigineMesh.h"
#include "UnigineNode.h"
#include "UnigineLights.h"
#include "UnigineObjects.h"
#include "UniginePlayers.h"
#include "UnigineDecals.h"
#include "UnigineImage.h"
#include "UnigineTextures.h"
#include "UnigineStreams.h"
#include "UnigineControls.h"
#include "UnigineInput.h"
#include "UnigineMaterial.h"
#include "UnigineProperties.h"
#include "UnigineWidgets.h"
#include "UnigineMathLib.h"
#include "UnigineCamera.h"
#include "UnigineEllipsoid.h"
#include "UnigineRender.h"

#if !defined(NDEBUG) && !defined(UNIGINE_EXTERN_INFO)
	#define UNIGINE_EXTERN_INFO
#endif

namespace Unigine
{

class ExternVariableBase;
class ExternFunctionBase;
class ExternClassBase;
class Variable;

////////////////////////////////////////////////////////////////////////////////
// Interpreter
////////////////////////////////////////////////////////////////////////////////

class Interpreter
{
public:
	static UNIGINE_API int addGroup(const char *group_name);
	static UNIGINE_API void removeGroup(const char *group_name);

	static UNIGINE_API void addExternDefine(const char *name, int group_id = -1);
	static UNIGINE_API void removeExternDefine(const char *name);

	static UNIGINE_API void addExternLibrary(const char *name, int group_id = -1);
	static UNIGINE_API void removeExternLibrary(const char *name);

	static UNIGINE_API void addExternVariable(const char *name, ExternVariableBase *extern_variable, int group_id = -1);
	static UNIGINE_API void removeExternVariable(const char *name);
	static UNIGINE_API ExternVariableBase *getExternVariable(const char *name);

	static UNIGINE_API void addExternFunction(const char *name, ExternFunctionBase *extern_function, int group_id = -1);
	static UNIGINE_API void removeExternFunction(const char *name);
	static UNIGINE_API ExternFunctionBase *getExternFunction(const char *name);

	static UNIGINE_API void addExternClass(const char *name, ExternClassBase *extern_class, int group_id = -1);
	static UNIGINE_API void removeExternClass(const char *name);
	static UNIGINE_API ExternClassBase *getExternClass(const char *name);

	static UNIGINE_API void *get();
	static UNIGINE_API int getStack();
	static UNIGINE_API Variable popStack();

	static UNIGINE_API void error(const char *format, ...) UNIGINE_PRINTF(1, 2);

	static UNIGINE_API int runScript(const char *filepath, const Vector<String> &arguments = {},
		const Vector<String> &defines = {}, const Vector<String> &include_paths = {},
		const char *entry_point = "main", bool silent = false);
};

////////////////////////////////////////////////////////////////////////////////
// Variable
////////////////////////////////////////////////////////////////////////////////

class UNIGINE_API Variable
{
public:
	// Possible variable types
	enum
	{
		ERROR_STRING = -1,
		INT = 0,
		LONG,
		FLOAT,
		DOUBLE,
		VEC2,
		VEC3,
		VEC4,
		DVEC2,
		DVEC3,
		DVEC4,
		IVEC2,
		IVEC3,
		IVEC4,
		MAT4,
		DMAT4,
		QUAT,
		STRING,
		GUID,
		PALETTE,
		USER_CLASS,
		EXTERN_CLASS,
	};

	Variable();
	Variable(const Variable &v);
	explicit Variable(int v);
	explicit Variable(long long v);
	explicit Variable(float v);
	explicit Variable(double v);
	explicit Variable(const Math::vec2 &v);
	explicit Variable(const Math::vec3 &v);
	explicit Variable(const Math::vec4 &v);
	explicit Variable(const Math::dvec2 &v);
	explicit Variable(const Math::dvec3 &v);
	explicit Variable(const Math::dvec4 &v);
	explicit Variable(const Math::ivec2 &v);
	explicit Variable(const Math::ivec3 &v);
	explicit Variable(const Math::ivec4 &v);
	explicit Variable(const Math::mat4 &m);
	explicit Variable(const Math::dmat4 &m);
	explicit Variable(const Math::quat &q);
	explicit Variable(const char *s);
	explicit Variable(const UGUID &g);
	explicit Variable(const Palette &p);

	Variable(void *interpreter, const char *type_name, void *object, int append = 0, int manage = 0);
	Variable(void *interpreter, const TypeInfo &type_info, void *object, int append = 0, int manage = 0);
	Variable(void *interpreter, const XmlPtr &xml, int append = 0, int manage = 0);
	Variable(void *interpreter, const GuiPtr &gui, int append = 0, int manage = 0);
	Variable(void *interpreter, const PathPtr &path, int append = 0, int manage = 0);
	Variable(void *interpreter, const MeshPtr &mesh, int append = 0, int manage = 0);
	Variable(void *interpreter, const NodePtr &node, int append = 0, int manage = 0);
	Variable(void *interpreter, const LightPtr &light, int append = 0, int manage = 0);
	Variable(void *interpreter, const DecalPtr &decal, int append = 0, int manage = 0);
	Variable(void *interpreter, const ObjectPtr &object, int append = 0, int manage = 0);
	Variable(void *interpreter, const PlayerPtr &player, int append = 0, int manage = 0);
	Variable(void *interpreter, const ImagePtr &image, int append = 0, int manage = 0);
	Variable(void *interpreter, const StreamPtr &stream, int append = 0, int manage = 0);
	Variable(void *interpreter, const ControlsPtr &controls, int append = 0, int manage = 0);
	Variable(void *interpreter, const MaterialPtr &material, int append = 0, int manage = 0);
	Variable(void *interpreter, const PropertyPtr &property, int append = 0, int manage = 0);
	Variable(void *interpreter, const PropertyParameterPtr &property_parameter, int append = 0, int manage = 0);
	Variable(void *interpreter, const WidgetPtr &widget, int append = 0, int manage = 0);
	Variable(void *interpreter, const CameraPtr &camera, int append = 0, int manage = 0);
	Variable(void *interpreter, const EllipsoidPtr &ellipsoid, int append = 0, int manage = 0);
	Variable(void *interpreter, const BodyPtr &body, int append = 0, int manage = 0);
	Variable(void *interpreter, const ShapePtr &shape, int append = 0, int manage = 0);
	Variable(void *interpreter, const JointPtr &joint, int append = 0, int manage = 0);
	Variable(void *interpreter, const TerrainGlobalLodsPtr &lods, int append = 0, int manage = 0);
	Variable(void *interpreter, const TerrainGlobalLodPtr &lod, int append = 0, int manage = 0);
	Variable(void *interpreter, const TerrainGlobalLodHeightPtr &lod, int append = 0, int manage = 0);
	Variable(void *interpreter, const TerrainDetailPtr &detail, int append = 0, int manage = 0);
	Variable(void *interpreter, const TerrainGlobalDetailPtr &detail, int append = 0, int manage = 0);
	Variable(void *interpreter, const RenderEnvironmentPresetPtr &preset, int append = 0, int manage = 0);
	Variable(void *interpreter, const ParticleModifierScalarPtr &preset, int append = 0, int manage = 0);
	Variable(void *interpreter, const ParticleModifierVectorPtr &preset, int append = 0, int manage = 0);

	~Variable();

	int getType() const;
	String getTypeName() const;
	String getTypeInfo() const;

	void set(const Variable &v);
	const Variable &get() const;

	void setInt(int v);
	int getInt() const;

	UNIGINE_INLINE int isInt() const { return (getType() == INT); }
	UNIGINE_INLINE int isNull() const { return (getType() == INT && getInt() == 0); }

	void setLong(long long v);
	long long getLong() const;
	UNIGINE_INLINE int isLong() const { return (getType() == LONG); }

	void setFloat(float v);
	float getFloat() const;
	UNIGINE_INLINE int isFloat() const { return (getType() == FLOAT); }

	void setDouble(double v);
	double getDouble() const;
	UNIGINE_INLINE int isDouble() const { return (getType() == DOUBLE); }

	void setVec2(const Math::vec2 &v);
	const Math::vec2 &getVec2() const;
	UNIGINE_INLINE int isVec2() const { return (getType() == VEC2); }

	void setVec3(const Math::vec3 &v);
	const Math::vec3 &getVec3() const;
	UNIGINE_INLINE int isVec3() const { return (getType() == VEC3); }

	void setVec4(const Math::vec4 &v);
	const Math::vec4 &getVec4() const;
	UNIGINE_INLINE int isVec4() const { return (getType() == VEC4); }

	void setDVec2(const Math::dvec2 &v);
	const Math::dvec2 &getDVec2() const;
	UNIGINE_INLINE int isDVec2() const { return (getType() == DVEC2); }

	void setDVec3(const Math::dvec3 &v);
	const Math::dvec3 &getDVec3() const;
	UNIGINE_INLINE int isDVec3() const { return (getType() == DVEC3); }

	void setDVec4(const Math::dvec4 &v);
	const Math::dvec4 &getDVec4() const;
	UNIGINE_INLINE int isDVec4() const { return (getType() == DVEC4); }

	void setIVec2(const Math::ivec2 &v);
	const Math::ivec2 &getIVec2() const;
	UNIGINE_INLINE int isIVec2() const { return (getType() == IVEC2); }

	void setIVec3(const Math::ivec3 &v);
	const Math::ivec3 &getIVec3() const;
	UNIGINE_INLINE int isIVec3() const { return (getType() == IVEC3); }

	void setIVec4(const Math::ivec4 &v);
	const Math::ivec4 &getIVec4() const;
	UNIGINE_INLINE int isIVec4() const { return (getType() == IVEC4); }

	void setMat4(const Math::mat4 &m);
	const Math::mat4 &getMat4() const;
	UNIGINE_INLINE int isMat4() const { return (getType() == MAT4); }

	void setDMat4(const Math::dmat4 &m);
	const Math::dmat4 &getDMat4() const;
	UNIGINE_INLINE int isDMat4() const { return (getType() == DMAT4); }

	void setQuat(const Math::quat &q);
	const Math::quat &getQuat() const;
	UNIGINE_INLINE int isQuat() const { return (getType() == QUAT); }

	void setString(const char *s);
	const char *getString() const;
	UNIGINE_INLINE int isString() const { return (getType() == STRING); }

	void setGUID(const UGUID &g);
	const UGUID &getGUID() const;
	UNIGINE_INLINE int isGUID() const { return (getType() == GUID); }

	void setPalette(const Palette &p);
	const Palette &getPalette() const;
	UNIGINE_INLINE int isPalette() const { return (getType() == PALETTE); }

	void setUserClass(int type, int number, int instance);
	int getUserClassType() const;
	int getUserClassNumber() const;
	int getUserClassInstance() const;
	UNIGINE_INLINE int isUserClass() const { return (getType() == USER_CLASS); }

	void setExternClassObject(void *interpreter, const char *type_name, void *object, int append = 0, int manage = 0);
	void setExternClassObject(void *interpreter, const TypeInfo &type_info, void *object, int append = 0, int manage = 0);
	void *getExternClassObject(void *interpreter, const TypeInfo &type_info) const;
	TypeInfo getExternClassType(void *interpreter) const;
	void appendExternClass(void *interpreter) const;
	void manageExternClass(void *interpreter) const;
	void removeExternClass(void *interpreter) const;
	void releaseExternClass(void *interpreter) const;
	UNIGINE_INLINE int isExternClass() const { return (getType() == EXTERN_CLASS); }

	template <class Type>
	int isExternClassObject(void *interpreter) const
	{
		return (isExternClass() && getExternClassType(interpreter) == TypeInfo(TypeID<Type *>()));
	}

	template <class Type>
	void setExternClassObject(void *interpreter, Type *object, int append = 0, int manage = 0)
	{
		setExternClassObject(interpreter, TypeInfo(TypeID<Type *>()), object, append, manage);
	}

	template <class Type>
	Type *getExternClassObject(void *interpreter) const
	{
		return (Type *)getExternClassObject(interpreter, TypeInfo(TypeID<Type *>()));
	}

	template <class Type>
	Type &getExternClassObjectRef(void *interpreter) const
	{
		Type *ptr = getExternClassObject<Type>(interpreter);
		if (!ptr)
			Interpreter::error("Unigine::Variable::getExternClassObject(): object is NULL\n");
		return *ptr;
	}

	void setXml(void *interpreter, const XmlPtr &xml, int append = 0, int manage = 0);
	XmlPtr getXml(void *interpreter) const;
	int isXml(void *interpreter) const;

	void setGui(void *interpreter, const GuiPtr &gui, int append = 0, int manage = 0);
	GuiPtr getGui(void *interpreter) const;
	int isGui(void *interpreter) const;

	void setPath(void *interpreter, const PathPtr &path, int append = 0, int manage = 0);
	PathPtr getPath(void *interpreter) const;
	int isPath(void *interpreter) const;

	void setMesh(void *interpreter, const MeshPtr &mesh, int append = 0, int manage = 0);
	MeshPtr getMesh(void *interpreter) const;
	int isMesh(void *interpreter) const;

	void setNode(void *interpreter, const NodePtr &node, int append = 0, int manage = 0);
	NodePtr getNode(void *interpreter) const;
	int isNode(void *interpreter) const;

	void setLight(void *interpreter, const LightPtr &light, int append = 0, int manage = 0);
	LightPtr getLight(void *interpreter) const;
	int isLight(void *interpreter) const;

	void setDecal(void *interpreter, const DecalPtr &decal, int append = 0, int manage = 0);
	DecalPtr getDecal(void *interpreter) const;
	int isDecal(void *interpreter) const;

	void setObject(void *interpreter, const ObjectPtr &object, int append = 0, int manage = 0);
	ObjectPtr getObject(void *interpreter) const;
	int isObject(void *interpreter) const;

	void setPlayer(void *interpreter, const PlayerPtr &player, int append = 0, int manage = 0);
	PlayerPtr getPlayer(void *interpreter) const;
	int isPlayer(void *interpreter) const;

	void setImage(void *interpreter, const ImagePtr &image, int append = 0, int manage = 0);
	ImagePtr getImage(void *interpreter) const;
	int isImage(void *interpreter) const;

	void setStream(void *interpreter, const StreamPtr &stream, int append = 0, int manage = 0);
	StreamPtr getStream(void *interpreter) const;
	int isStream(void *interpreter) const;

	void setControls(void *interpreter, const ControlsPtr &controls, int append = 0, int manage = 0);
	ControlsPtr getControls(void *interpreter) const;
	int isControls(void *interpreter) const;

	void setMaterial(void *interpreter, const MaterialPtr &material, int append = 0, int manage = 0);
	MaterialPtr getMaterial(void *interpreter) const;
	int isMaterial(void *interpreter) const;

	void setProperty(void *interpreter, const PropertyPtr &property, int append = 0, int manage = 0);
	PropertyPtr getProperty(void *interpreter) const;
	int isProperty(void *interpreter) const;

	void setPropertyParameter(void *interpreter, const PropertyParameterPtr &property_parameters, int append = 0, int manage = 0);
	PropertyParameterPtr getPropertyParameter(void *interpreter) const;
	int isPropertyParameter(void *interpreter) const;

	void setWidget(void *interpreter, const WidgetPtr &widget, int append = 0, int manage = 0);
	WidgetPtr getWidget(void *interpreter) const;
	int isWidget(void *interpreter) const;

	void setCamera(void *interpreter, const CameraPtr &camera, int append = 0, int manage = 0);
	CameraPtr getCamera(void *interpreter) const;
	int isCamera(void *interpreter) const;

	void setEllipsoid(void *interpreter, const EllipsoidPtr &ellipsoid, int append = 0, int manage = 0);
	EllipsoidPtr getEllipsoid(void *interpreter) const;
	int isEllipsoid(void *interpreter) const;

	void setBody(void *interpreter, const BodyPtr &body, int append = 0, int manage = 0);
	BodyPtr getBody(void *interpreter) const;
	int isBody(void *interpreter) const;

	void setShape(void *interpreter, const ShapePtr &shape, int append = 0, int manage = 0);
	ShapePtr getShape(void *interpreter) const;
	int isShape(void *interpreter) const;

	void setJoint(void *interpreter, const JointPtr &joint, int append = 0, int manage = 0);
	JointPtr getJoint(void *interpreter) const;
	int isJoint(void *interpreter) const;

	void setTerrainGlobalLods(void *interpreter, const TerrainGlobalLodsPtr &lods, int append = 0, int manage = 0);
	TerrainGlobalLodsPtr getTerrainGlobalLods(void *interpreter) const;
	int isTerrainGlobalLods(void *interpreter) const;

	void setTerrainGlobalLod(void *interpreter, const TerrainGlobalLodPtr &lod, int append = 0, int manage = 0);
	TerrainGlobalLodPtr getTerrainGlobalLod(void *interpreter) const;
	int isTerrainGlobalLod(void *interpreter) const;

	void setTerrainGlobalLodHeight(void *interpreter, const TerrainGlobalLodHeightPtr &lod, int append = 0, int manage = 0);
	TerrainGlobalLodHeightPtr getTerrainGlobalLodHeight(void *interpreter) const;
	int isTerrainGlobalLodHeight(void *interpreter) const;

	void setTerrainGlobalDetail(void *interpreter, const TerrainGlobalDetailPtr &detail, int append = 0, int manage = 0);
	TerrainGlobalDetailPtr getTerrainGlobalDetail(void *interpreter) const;
	int isTerrainGlobalDetail(void *interpreter) const;

	void setTerrainDetail(void *interpreter, const TerrainDetailPtr &detail, int append = 0, int manage = 0);
	TerrainDetailPtr getTerrainDetail(void *interpreter) const;
	int isTerrainDetail(void *interpreter) const;

	void setRenderEnvironmentPreset(void *interpreter, const RenderEnvironmentPresetPtr &preset, int append = 0, int manage = 0);
	RenderEnvironmentPresetPtr getRenderEnvironmentPreset(void *interpreter) const;
	int isRenderEnvironmentPreset(void *interpreter) const;

	void setParticleModifierScalar(void *interpreter, const ParticleModifierScalarPtr &modifier, int append = 0, int manage = 0);
	ParticleModifierScalarPtr getParticleModifierScalar(void *interpreter) const;
	int isParticleModifierScalar(void *interpreter) const;

	void setParticleModifierVector(void *interpreter, const ParticleModifierVectorPtr &modifier, int append = 0, int manage = 0);
	ParticleModifierVectorPtr getParticleModifierVector(void *interpreter) const;
	int isParticleModifierVector(void *interpreter) const;

	Variable &operator=(const Variable &v);
	Variable operator*(const Variable &v) const;
	Variable operator/(const Variable &v) const;
	Variable operator%(const Variable &v) const;
	Variable operator+(const Variable &v) const;
	Variable operator-(const Variable &v) const;
	Variable operator<<(const Variable &v) const;
	Variable operator>>(const Variable &v) const;
	Variable operator&(const Variable &v) const;
	Variable operator|(const Variable &v) const;
	Variable operator^(const Variable &v) const;

	int operator==(const Variable &v) const;
	int operator!=(const Variable &v) const;
	int operator<=(const Variable &v) const;
	int operator>=(const Variable &v) const;
	int operator<(const Variable &v) const;
	int operator>(const Variable &v) const;
	int operator&&(const Variable &v) const;
	int operator||(const Variable &v) const;

	struct VariableData;
	VariableData *getData() const { return data; }

private:
	friend struct VariableToVariable;

	friend void init_variables();
	friend void shutdown_variables();

	VariableData *data;
};

////////////////////////////////////////////////////////////////////////////////
// Reflection
////////////////////////////////////////////////////////////////////////////////

class UNIGINE_API Reflection
{
public:
	Reflection();
	Reflection(void *interpreter, const Variable &v);
	~Reflection();
	int reflect(void *interpreter, const Variable &v);

	const char *getName() const;
	const char *getBaseName() const;
	const char *getAttribute() const;
	int getNumNameSpaces() const;
	const char *getNameSpaceName(int num) const;
	const char *getNameSpaceAttribute(int num) const;
	int getNumVariables() const;
	const char *getVariableName(int num) const;
	const char *getVariableAttribute(int num) const;
	int getNumArrays() const;
	const char *getArrayName(int num) const;
	const char *getArrayAttribute(int num) const;
	int getNumFunctions() const;
	int getFunctionID(int num) const;
	const char *getFunctionName(int num) const;
	const char *getFunctionType(int num) const;
	const char *getFunctionAttribute(int num) const;
	int getNumFunctionArgs(int num) const;
	int getNumFunctionDefaultArgs(int num) const;
	const char *getFunctionArgName(int num, int arg) const;
	const char *getFunctionArgType(int num, int arg) const;
	int getNumClasses() const;
	const char *getClassName(int num) const;
	const char *getClassAttribute(int num) const;
	int getNumConstants() const;
	const char *getConstantName(int num) const;
	Variable getConstantValue(int num) const;

	// Variable *args
	Variable callExternClassFunction(Variable object, int function, int num_args, const Variable *args);
	Variable callExternClassFunction(Variable object, const char *function, int num_args, const Variable *args);
	Variable callExternFunction(int function, int num_args, const Variable *args);
	Variable callExternFunction(const char *function, int num_args, const Variable *args);

	// Vector<Variable> args
	Variable callExternClassFunction(Variable object, int function, int num_args, const Vector<Variable> &args);
	Variable callExternClassFunction(Variable object, const char *function, int num_args, const Vector<Variable> &args);
	Variable callExternFunction(int function, int num_args, const  Vector<Variable> &args);
	Variable callExternFunction(const char *function, int num_args, const  Vector<Variable> &args);

	// Variable arg
	Variable callExternClassFunction(Variable object, int function);
	Variable callExternClassFunction(Variable object, int function, const Variable &arg0);
	Variable callExternClassFunction(Variable object, int function, const Variable &arg0, const Variable &arg1);
	Variable callExternClassFunction(Variable object, int function, const Variable &arg0, const Variable &arg1, const Variable &arg2);
	Variable callExternClassFunction(Variable object, int function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3);
	Variable callExternClassFunction(Variable object, int function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4);
	Variable callExternClassFunction(Variable object, int function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5);
	Variable callExternClassFunction(Variable object, int function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6);
	Variable callExternClassFunction(Variable object, int function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6, const Variable &arg7);
	Variable callExternClassFunction(Variable object, int function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6, const Variable &arg7, const Variable &arg8);
	Variable callExternClassFunction(Variable object, int function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6, const Variable &arg7, const Variable &arg8, const Variable &arg9);
	Variable callExternClassFunction(Variable object, const char *function);
	Variable callExternClassFunction(Variable object, const char *function, const Variable &arg0);
	Variable callExternClassFunction(Variable object, const char *function, const Variable &arg0, const Variable &arg1);
	Variable callExternClassFunction(Variable object, const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2);
	Variable callExternClassFunction(Variable object, const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3);
	Variable callExternClassFunction(Variable object, const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4);
	Variable callExternClassFunction(Variable object, const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5);
	Variable callExternClassFunction(Variable object, const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6);
	Variable callExternClassFunction(Variable object, const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6, const Variable &arg7);
	Variable callExternClassFunction(Variable object, const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6, const Variable &arg7, const Variable &arg8);
	Variable callExternClassFunction(Variable object, const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6, const Variable &arg7, const Variable &arg8, const Variable &arg9);
	Variable callExternFunction(int function);
	Variable callExternFunction(int function, const Variable &arg0);
	Variable callExternFunction(int function, const Variable &arg0, const Variable &arg1);
	Variable callExternFunction(int function, const Variable &arg0, const Variable &arg1, const Variable &arg2);
	Variable callExternFunction(int function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3);
	Variable callExternFunction(int function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4);
	Variable callExternFunction(int function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5);
	Variable callExternFunction(int function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6);
	Variable callExternFunction(int function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6, const Variable &arg7);
	Variable callExternFunction(int function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6, const Variable &arg7, const Variable &arg8);
	Variable callExternFunction(int function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6, const Variable &arg7, const Variable &arg8, const Variable &arg9);
	Variable callExternFunction(const char *function);
	Variable callExternFunction(const char *function, const Variable &arg0);
	Variable callExternFunction(const char *function, const Variable &arg0, const Variable &arg1);
	Variable callExternFunction(const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2);
	Variable callExternFunction(const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3);
	Variable callExternFunction(const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4);
	Variable callExternFunction(const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5);
	Variable callExternFunction(const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6);
	Variable callExternFunction(const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6, const Variable &arg7);
	Variable callExternFunction(const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6, const Variable &arg7, const Variable &arg8);
	Variable callExternFunction(const char *function, const Variable &arg0, const Variable &arg1, const Variable &arg2, const Variable &arg3, const Variable &arg4, const Variable &arg5, const Variable &arg6, const Variable &arg7, const Variable &arg8, const Variable &arg9);

private:
	void *obj;
};

////////////////////////////////////////////////////////////////////////////////
// Expression
////////////////////////////////////////////////////////////////////////////////

class UNIGINE_API Expression
{
public:
	Expression(void *interpreter, const char *src, int scope = 0);
	~Expression();

	// check expression
	int isCompiled() const;

	// expression name
	void setName(const char *name) const;
	const char *getName() const;

	// functions
	int isFunction(const char *name, int num_args) const;
	int getFunction(const char *name, int num_args) const;

	// variables
	int isVariable(const char *name) const;
	void setVariable(const char *name, const Variable &v) const;
	Variable getVariable(const char *name) const;

	// run expression
	Variable run() const;

	// save/restore state
	int saveState(const StreamPtr &stream) const;
	int restoreState(const StreamPtr &stream);

private:
	void *obj;
};

////////////////////////////////////////////////////////////////////////////////
// VariableToType
////////////////////////////////////////////////////////////////////////////////

template <class Type>
struct VariableToType
{
	VariableToType(void *i, const Variable &v) : value((Type)v.getExternClassObject(i, TypeInfo(TypeID<Type>()))) {}
	Type value;
};

template <class Type>
struct VariableToType<Type *>
{
	VariableToType(void *i, const Variable &v) : value(v.getExternClassObject<Type>(i)) {}
	Type *value;
};

template <class Type>
struct VariableToType<const Type *>
{
	VariableToType(void *i, const Variable &v) : value(v.getExternClassObject<Type>(i)) {}
	const Type *value;
};

template <class Type>
struct VariableToType<Type &>
{
	VariableToType(void *i, const Variable &v) : value(v.getExternClassObjectRef<Type>(i)) {}
	Type &value;
};

template <class Type>
struct VariableToType<const Type &>
{
	VariableToType(void *i, const Variable &v) : value(v.getExternClassObjectRef<Type>(i)) {}
	const Type &value;
};

template <>
struct VariableToType<Variable &>
{
	VariableToType(void *i, Variable &v) : value(v) { UNIGINE_UNUSED(i); }
	Variable &value;
};

template <>
struct VariableToType<const Variable &>
{
	VariableToType(void *i, const Variable &v) : value(v) { UNIGINE_UNUSED(i); }
	const Variable &value;
};

//
#define DECLARE_VARIABLE_TO_TYPE(TYPE) \
template <> \
struct VariableToType<TYPE ## Ptr> \
{ \
	VariableToType(void *i, const Variable &v) : value(v.getExternClassObject<TYPE>(i)) {} \
	TYPE ## Ptr value; \
}; \
template <> \
struct VariableToType<const TYPE ## Ptr&> \
{ \
	VariableToType(void *i, const Variable &v) : value(v.getExternClassObject<TYPE>(i)) {} \
	TYPE ## Ptr value; \
};

DECLARE_VARIABLE_TO_TYPE(Xml)
DECLARE_VARIABLE_TO_TYPE(Gui)
DECLARE_VARIABLE_TO_TYPE(Path)
DECLARE_VARIABLE_TO_TYPE(Mesh)
DECLARE_VARIABLE_TO_TYPE(Node)
DECLARE_VARIABLE_TO_TYPE(Light)
DECLARE_VARIABLE_TO_TYPE(Decal)
DECLARE_VARIABLE_TO_TYPE(Object)
DECLARE_VARIABLE_TO_TYPE(Player)
DECLARE_VARIABLE_TO_TYPE(Image)
DECLARE_VARIABLE_TO_TYPE(Texture)
DECLARE_VARIABLE_TO_TYPE(Stream)
DECLARE_VARIABLE_TO_TYPE(Controls)
DECLARE_VARIABLE_TO_TYPE(Material)
DECLARE_VARIABLE_TO_TYPE(Property)
DECLARE_VARIABLE_TO_TYPE(PropertyParameter)
DECLARE_VARIABLE_TO_TYPE(Widget)
DECLARE_VARIABLE_TO_TYPE(Ellipsoid)
DECLARE_VARIABLE_TO_TYPE(Camera)
DECLARE_VARIABLE_TO_TYPE(TerrainGlobalLods)
DECLARE_VARIABLE_TO_TYPE(TerrainGlobalLod)
DECLARE_VARIABLE_TO_TYPE(TerrainGlobalLodHeight)
DECLARE_VARIABLE_TO_TYPE(TerrainGlobalDetail)
DECLARE_VARIABLE_TO_TYPE(TerrainDetail)
DECLARE_VARIABLE_TO_TYPE(RenderEnvironmentPreset)
DECLARE_VARIABLE_TO_TYPE(ParticleModifierScalar)
DECLARE_VARIABLE_TO_TYPE(ParticleModifierVector)

#undef DECLARE_VARIABLE_TO_TYPE

//
#define DECLARE_VARIABLE_TO_TYPE(TYPE, VALUE_TYPE, OPERATOR, CONVERSION) \
template <> \
struct VariableToType<TYPE> \
{ \
	VariableToType(void *i, const Variable &v) : value( CONVERSION ( v. OPERATOR() ) ) { UNIGINE_UNUSED(i); } \
	VALUE_TYPE value; \
};

DECLARE_VARIABLE_TO_TYPE(Variable, Variable, get, )
DECLARE_VARIABLE_TO_TYPE(bool, bool, getInt, 0 != )
DECLARE_VARIABLE_TO_TYPE(char, char, getInt, (char))
DECLARE_VARIABLE_TO_TYPE(unsigned char, unsigned char, getInt, (unsigned char))
DECLARE_VARIABLE_TO_TYPE(short, short, getInt, (short))
DECLARE_VARIABLE_TO_TYPE(unsigned short, unsigned short, getInt, (unsigned short))
DECLARE_VARIABLE_TO_TYPE(int, int, getInt, )
DECLARE_VARIABLE_TO_TYPE(unsigned int, unsigned int, getInt, (unsigned int))
DECLARE_VARIABLE_TO_TYPE(long int, long int, getLong, (long int))
DECLARE_VARIABLE_TO_TYPE(unsigned long int, unsigned long int, getLong, (unsigned long int))
DECLARE_VARIABLE_TO_TYPE(long long, long long, getLong, )
DECLARE_VARIABLE_TO_TYPE(unsigned long long, unsigned long long, getLong, (unsigned long long))
DECLARE_VARIABLE_TO_TYPE(float, float, getFloat, )
DECLARE_VARIABLE_TO_TYPE(double, double, getDouble, )
DECLARE_VARIABLE_TO_TYPE(Math::vec2, Math::vec2, getVec2, Math::vec2)
DECLARE_VARIABLE_TO_TYPE(const Math::vec2 &, Math::vec2, getVec2, Math::vec2)
DECLARE_VARIABLE_TO_TYPE(Math::vec3, Math::vec3, getVec3, )
DECLARE_VARIABLE_TO_TYPE(const Math::vec3 &, Math::vec3, getVec3, )
DECLARE_VARIABLE_TO_TYPE(Math::vec4, Math::vec4, getVec4, )
DECLARE_VARIABLE_TO_TYPE(const Math::vec4 &, Math::vec4, getVec4, )
DECLARE_VARIABLE_TO_TYPE(Math::dvec2, Math::dvec2, getDVec2, Math::dvec2)
DECLARE_VARIABLE_TO_TYPE(const Math::dvec2 &, Math::dvec2, getDVec2, Math::dvec2)
DECLARE_VARIABLE_TO_TYPE(Math::dvec3, Math::dvec3, getDVec3, )
DECLARE_VARIABLE_TO_TYPE(const Math::dvec3 &, Math::dvec3, getDVec3, )
DECLARE_VARIABLE_TO_TYPE(Math::dvec4, Math::dvec4, getDVec4, )
DECLARE_VARIABLE_TO_TYPE(const Math::dvec4 &, Math::dvec4, getDVec4, )
DECLARE_VARIABLE_TO_TYPE(Math::ivec2, Math::ivec2, getIVec2, )
DECLARE_VARIABLE_TO_TYPE(const Math::ivec2 &, Math::ivec2, getIVec2, )
DECLARE_VARIABLE_TO_TYPE(Math::ivec3, Math::ivec3, getIVec3, )
DECLARE_VARIABLE_TO_TYPE(const Math::ivec3 &, Math::ivec3, getIVec3, )
DECLARE_VARIABLE_TO_TYPE(Math::ivec4, Math::ivec4, getIVec4, )
DECLARE_VARIABLE_TO_TYPE(const Math::ivec4 &, Math::ivec4, getIVec4, )
DECLARE_VARIABLE_TO_TYPE(Math::mat4, Math::mat4, getMat4, )
DECLARE_VARIABLE_TO_TYPE(const Math::mat4 &, Math::mat4, getMat4, )
DECLARE_VARIABLE_TO_TYPE(Math::dmat4, Math::dmat4, getDMat4, )
DECLARE_VARIABLE_TO_TYPE(const Math::dmat4 &, Math::dmat4, getDMat4, )
DECLARE_VARIABLE_TO_TYPE(Math::quat, Math::quat, getQuat, )
DECLARE_VARIABLE_TO_TYPE(const Math::quat &, Math::quat, getQuat, )
DECLARE_VARIABLE_TO_TYPE(String, String, getString, )
DECLARE_VARIABLE_TO_TYPE(const String &, String, getString, )
DECLARE_VARIABLE_TO_TYPE(const char *, const char *, getString, )
DECLARE_VARIABLE_TO_TYPE(UGUID, UGUID, getGUID, )
DECLARE_VARIABLE_TO_TYPE(const UGUID &, UGUID, getGUID, )
DECLARE_VARIABLE_TO_TYPE(Palette, Palette, getPalette, )
DECLARE_VARIABLE_TO_TYPE(const Palette &, Palette, getPalette, )

#undef DECLARE_VARIABLE_TO_TYPE

////////////////////////////////////////////////////////////////////////////////
// TypeToVariable
////////////////////////////////////////////////////////////////////////////////

template <class Type>
struct TypeToVariable
{
	TypeToVariable(void *i, Type t) { value.setExternClassObject(i, TypeInfo(TypeID<Type>()), t); }
	Variable value;
};

template <class Type>
struct TypeToVariable<Type *>
{
	TypeToVariable(void *i, Type *t) { value.setExternClassObject<Type>(i, t); }
	Variable value;
};

template <class Type>
struct TypeToVariable<Type &>
{
	TypeToVariable(void *i, Type &t) { value.setExternClassObject<Type>(i, &t); }
	Variable value;
};

template <class Type>
struct TypeToVariable<const Type *>
{
	TypeToVariable(void *i, const Type *t) { value.setExternClassObject<Type>(i, const_cast<Type *>(t)); }
	Variable value;
};

template <class Type>
struct TypeToVariable<const Type &>
{
	TypeToVariable(void *i, const Type &t) { value.setExternClassObject<Type>(i, const_cast<Type *>(&t)); }
	Variable value;
};

template <>
struct TypeToVariable<Variable &>
{
	TypeToVariable(void *i, Variable &v) : value(v) { UNIGINE_UNUSED(i); }
	Variable &value;
};

template <>
struct TypeToVariable<const Variable &>
{
	TypeToVariable(void *i, const Variable &v) : value(v) { UNIGINE_UNUSED(i); }
	const Variable &value;
};

template <>
struct TypeToVariable<String>
{
	TypeToVariable(void *i, const String &s) { UNIGINE_UNUSED(i); value.setString(s.get()); }
	Variable value;
};

template <>
struct TypeToVariable<const String &>
{
	TypeToVariable(void *i, const String &s) { UNIGINE_UNUSED(i); value.setString(s.get()); }
	Variable value;
};

//
#define DECLARE_TYPE_TO_VARIABLE(TYPE) \
template <> \
struct TypeToVariable<TYPE ## Ptr> \
{ \
	TypeToVariable(void *i, const TYPE ## Ptr &v) { value.setExternClassObject<TYPE>(i, v.get()); } \
	Variable value; \
}; \
template <> \
struct TypeToVariable<const TYPE ## Ptr&> \
{ \
	TypeToVariable(void *i, const TYPE ## Ptr &v) { value.setExternClassObject<TYPE>(i, v.get()); } \
	Variable value; \
};

DECLARE_TYPE_TO_VARIABLE(Xml)
DECLARE_TYPE_TO_VARIABLE(Gui)
DECLARE_TYPE_TO_VARIABLE(Path)
DECLARE_TYPE_TO_VARIABLE(Mesh)
DECLARE_TYPE_TO_VARIABLE(Node)
DECLARE_TYPE_TO_VARIABLE(Light)
DECLARE_TYPE_TO_VARIABLE(Decal)
DECLARE_TYPE_TO_VARIABLE(Object)
DECLARE_TYPE_TO_VARIABLE(Player)
DECLARE_TYPE_TO_VARIABLE(Image)
DECLARE_TYPE_TO_VARIABLE(Texture)
DECLARE_TYPE_TO_VARIABLE(Stream)
DECLARE_TYPE_TO_VARIABLE(Controls)
DECLARE_TYPE_TO_VARIABLE(Material)
DECLARE_TYPE_TO_VARIABLE(Property)
DECLARE_TYPE_TO_VARIABLE(PropertyParameter)
DECLARE_TYPE_TO_VARIABLE(Widget)
DECLARE_TYPE_TO_VARIABLE(Ellipsoid)
DECLARE_TYPE_TO_VARIABLE(Camera)
DECLARE_TYPE_TO_VARIABLE(TerrainGlobalLods)
DECLARE_TYPE_TO_VARIABLE(TerrainGlobalLod)
DECLARE_TYPE_TO_VARIABLE(TerrainGlobalLodHeight)
DECLARE_TYPE_TO_VARIABLE(TerrainGlobalDetail)
DECLARE_TYPE_TO_VARIABLE(TerrainDetail)
DECLARE_TYPE_TO_VARIABLE(RenderEnvironmentPreset)
DECLARE_TYPE_TO_VARIABLE(ParticleModifierScalar)
DECLARE_TYPE_TO_VARIABLE(ParticleModifierVector)

#undef DECLARE_TYPE_TO_VARIABLE

//
#define DECLARE_TYPE_TO_VARIABLE_REF(TYPE, OPERATOR, CONVERSION) \
template <> \
struct TypeToVariable<TYPE> \
{ \
	TypeToVariable(void *i, const TYPE &v) { UNIGINE_UNUSED(i); value. OPERATOR ( CONVERSION ( v ) ); } \
	Variable value; \
};

#define DECLARE_TYPE_TO_VARIABLE_STACK(TYPE, OPERATOR, CONVERSION) \
template <> \
struct TypeToVariable<TYPE> \
{ \
	TypeToVariable(void *i, TYPE v) { UNIGINE_UNUSED(i); value. OPERATOR ( CONVERSION ( v ) ); } \
	Variable value; \
};

DECLARE_TYPE_TO_VARIABLE_REF(Variable, set, )
DECLARE_TYPE_TO_VARIABLE_STACK(bool, setInt, )
DECLARE_TYPE_TO_VARIABLE_STACK(char, setInt, )
DECLARE_TYPE_TO_VARIABLE_STACK(unsigned char, setInt, )
DECLARE_TYPE_TO_VARIABLE_STACK(short, setInt, )
DECLARE_TYPE_TO_VARIABLE_STACK(unsigned short, setInt, )
DECLARE_TYPE_TO_VARIABLE_STACK(int, setInt, )
DECLARE_TYPE_TO_VARIABLE_STACK(unsigned int, setInt, (int))
DECLARE_TYPE_TO_VARIABLE_STACK(long int, setLong, (long long))
DECLARE_TYPE_TO_VARIABLE_STACK(unsigned long int, setLong, (long long))
DECLARE_TYPE_TO_VARIABLE_STACK(long long, setLong, )
DECLARE_TYPE_TO_VARIABLE_STACK(unsigned long long, setLong, (long long))
DECLARE_TYPE_TO_VARIABLE_STACK(float, setFloat, )
DECLARE_TYPE_TO_VARIABLE_STACK(double, setDouble, )
DECLARE_TYPE_TO_VARIABLE_REF(Math::vec2, setVec2, Math::vec2)
DECLARE_TYPE_TO_VARIABLE_STACK(const Math::vec2 &, setVec2, Math::vec2)
DECLARE_TYPE_TO_VARIABLE_REF(Math::vec3, setVec3, )
DECLARE_TYPE_TO_VARIABLE_STACK(const Math::vec3 &, setVec3, )
DECLARE_TYPE_TO_VARIABLE_REF(Math::vec4, setVec4, )
DECLARE_TYPE_TO_VARIABLE_STACK(const Math::vec4 &, setVec4, )
DECLARE_TYPE_TO_VARIABLE_REF(Math::dvec2, setDVec2, Math::dvec2)
DECLARE_TYPE_TO_VARIABLE_STACK(const Math::dvec2 &, setDVec2, Math::dvec2)
DECLARE_TYPE_TO_VARIABLE_REF(Math::dvec3, setDVec3, )
DECLARE_TYPE_TO_VARIABLE_STACK(const Math::dvec3 &, setDVec3, )
DECLARE_TYPE_TO_VARIABLE_REF(Math::dvec4, setDVec4, )
DECLARE_TYPE_TO_VARIABLE_STACK(const Math::dvec4 &, setDVec4, )
DECLARE_TYPE_TO_VARIABLE_REF(Math::ivec2, setIVec2, )
DECLARE_TYPE_TO_VARIABLE_STACK(const Math::ivec2 &, setIVec2, )
DECLARE_TYPE_TO_VARIABLE_REF(Math::ivec3, setIVec3, )
DECLARE_TYPE_TO_VARIABLE_STACK(const Math::ivec3 &, setIVec3, )
DECLARE_TYPE_TO_VARIABLE_REF(Math::ivec4, setIVec4, )
DECLARE_TYPE_TO_VARIABLE_STACK(const Math::ivec4 &, setIVec4, )
DECLARE_TYPE_TO_VARIABLE_REF(Math::mat4, setMat4, )
DECLARE_TYPE_TO_VARIABLE_STACK(const Math::mat4 &, setMat4, )
DECLARE_TYPE_TO_VARIABLE_REF(Math::dmat4, setDMat4, )
DECLARE_TYPE_TO_VARIABLE_STACK(const Math::dmat4 &, setDMat4, )
DECLARE_TYPE_TO_VARIABLE_REF(Math::quat, setQuat, )
DECLARE_TYPE_TO_VARIABLE_STACK(const Math::quat&, setQuat, )
DECLARE_TYPE_TO_VARIABLE_STACK(const char *, setString, )
DECLARE_TYPE_TO_VARIABLE_REF(UGUID, setGUID, )
DECLARE_TYPE_TO_VARIABLE_STACK(const UGUID&, setGUID, )
DECLARE_TYPE_TO_VARIABLE_REF(Palette, setPalette, )
DECLARE_TYPE_TO_VARIABLE_STACK(const Palette&, setPalette, )

#undef DECLARE_TYPE_TO_VARIABLE_REF
#undef DECLARE_TYPE_TO_VARIABLE_STACK

////////////////////////////////////////////////////////////////////////////////
// TypeToName
////////////////////////////////////////////////////////////////////////////////

template <class Type>
struct TypeToName
{
	static const char *name() { return TypeID<Type>().name; }
};

#define DECLARE_TYPE_TO_NAME(TYPE, NAME) \
template <> \
struct TypeToName<TYPE> \
{ \
	static const char *name() { return NAME; } \
};

DECLARE_TYPE_TO_NAME(Variable, "variable")
DECLARE_TYPE_TO_NAME(const Variable &, "variable")
DECLARE_TYPE_TO_NAME(void, "void")
DECLARE_TYPE_TO_NAME(char, "int")
DECLARE_TYPE_TO_NAME(unsigned char, "int")
DECLARE_TYPE_TO_NAME(short, "int")
DECLARE_TYPE_TO_NAME(unsigned short, "int")
DECLARE_TYPE_TO_NAME(int, "int")
DECLARE_TYPE_TO_NAME(unsigned int, "int")
DECLARE_TYPE_TO_NAME(long int, "long")
DECLARE_TYPE_TO_NAME(unsigned long int, "long")
DECLARE_TYPE_TO_NAME(long long, "long")
DECLARE_TYPE_TO_NAME(unsigned long long, "long")
DECLARE_TYPE_TO_NAME(float, "float")
DECLARE_TYPE_TO_NAME(double, "double")
DECLARE_TYPE_TO_NAME(const float*, "float[]")
DECLARE_TYPE_TO_NAME(const double*, "double[]")
DECLARE_TYPE_TO_NAME(Math::vec2, "vec2")
DECLARE_TYPE_TO_NAME(const Math::vec2 &, "vec2")
DECLARE_TYPE_TO_NAME(Math::vec3, "vec3")
DECLARE_TYPE_TO_NAME(const Math::vec3 &, "vec3")
DECLARE_TYPE_TO_NAME(Math::vec4, "vec4")
DECLARE_TYPE_TO_NAME(const Math::vec4 &, "vec4")
DECLARE_TYPE_TO_NAME(Math::dvec2, "dvec2")
DECLARE_TYPE_TO_NAME(const Math::dvec2 &, "dvec2")
DECLARE_TYPE_TO_NAME(Math::dvec3, "dvec3")
DECLARE_TYPE_TO_NAME(const Math::dvec3 &, "dvec3")
DECLARE_TYPE_TO_NAME(Math::dvec4, "dvec4")
DECLARE_TYPE_TO_NAME(const Math::dvec4 &, "dvec4")
DECLARE_TYPE_TO_NAME(Math::ivec2, "ivec2")
DECLARE_TYPE_TO_NAME(const Math::ivec2 &, "ivec2")
DECLARE_TYPE_TO_NAME(Math::ivec3, "ivec3")
DECLARE_TYPE_TO_NAME(const Math::ivec3 &, "ivec3")
DECLARE_TYPE_TO_NAME(Math::ivec4, "ivec4")
DECLARE_TYPE_TO_NAME(const Math::ivec4 &, "ivec4")
DECLARE_TYPE_TO_NAME(Math::mat4, "mat4")
DECLARE_TYPE_TO_NAME(const Math::mat4 &, "mat4")
DECLARE_TYPE_TO_NAME(Math::dmat4, "dmat4")
DECLARE_TYPE_TO_NAME(const Math::dmat4 &, "dmat4")
DECLARE_TYPE_TO_NAME(Math::quat, "quat")
DECLARE_TYPE_TO_NAME(const Math::quat &, "quat")
DECLARE_TYPE_TO_NAME(String, "string")
DECLARE_TYPE_TO_NAME(const String &, "string")
DECLARE_TYPE_TO_NAME(const char *, "string")
DECLARE_TYPE_TO_NAME(UGUID, "guid")
DECLARE_TYPE_TO_NAME(const UGUID &, "guid")
DECLARE_TYPE_TO_NAME(Palette, "palette")
DECLARE_TYPE_TO_NAME(const Palette &, "palette")

#undef DECLARE_TYPE_TO_NAME

////////////////////////////////////////////////////////////////////////////////
// ArrayVector - interface to the UnigineScript vector
////////////////////////////////////////////////////////////////////////////////

class UNIGINE_API ArrayVector
{
public:
	static ArrayVector get(void *interpreter, const Variable &id);

	ArrayVector(const ArrayVector &vector);
	~ArrayVector();

	ArrayVector &operator=(const ArrayVector &vector);
	UNIGINE_INLINE const Variable &operator[](int index) const { return get(index); }

	void set(int index, const Variable &v) const;
	const Variable &get(int index) const;

	int size() const;
	void resize(int size) const;
	void clear() const;

	int find(const Variable &v) const;

	void append(const Variable &v) const;
	void append(int pos, const Variable &v) const;

	void remove(int pos) const;

private:
	friend class Interpreter;

	ArrayVector();

	struct ArrayVectorData;
	ArrayVectorData *data;
};

////////////////////////////////////////////////////////////////////////////////
// ArrayMap - interface to the UnigineScript map
////////////////////////////////////////////////////////////////////////////////

class UNIGINE_API ArrayMap
{
public:
	class UNIGINE_API Iterator
	{
	public:
		Iterator();
		Iterator(const Iterator &it);
		~Iterator();

		Iterator &operator=(const Iterator &it);

		int operator==(const Iterator &it) const;
		int operator!=(const Iterator &it) const;

		const Variable &key() const;
		const Variable &get() const;

		Iterator &operator++();
		Iterator &operator--();
		Iterator operator++(int);
		Iterator operator--(int);

	private:
		friend class ArrayMap;

		struct IteratorData;
		IteratorData *data;
	};

	static ArrayMap get(void *interpreter, const Variable &id);

	ArrayMap(const ArrayMap &map);
	~ArrayMap();

	ArrayMap &operator=(const ArrayMap &map);

	UNIGINE_INLINE const Variable &operator[](const Variable &key) const { return get(key); }

	void set(const Variable &key, const Variable &v) const;
	const Variable &get(const Variable &key) const;

	Iterator begin();
	Iterator back();
	Iterator end();

	int size() const;
	void clear() const;

	Iterator find(const Variable &key) const;

	void append(const Variable &key, const Variable &v) const;

	void remove(const Variable &key) const;

private:
	friend class Interpreter;

	ArrayMap();

	struct ArrayMapData;
	ArrayMapData *data;
};

} // namespace Unigine
