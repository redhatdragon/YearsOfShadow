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
#include "UnigineBase.h"
#include "UnigineString.h"
#include "UnigineMathLib.h"
#include "UnigineGUID.h"

namespace Unigine
{


class UNIGINE_API Json : public APIInterface
{
public:
	static Ptr<Json> create();
	static Ptr<Json> create(const char *name);
	Ptr<Json> addChild(const Ptr<Json> &json);
	Ptr<Json> addChild(const char *name);
	Ptr<Json> addChild();
	Ptr<Json> addChild(const char *name, int value);
	Ptr<Json> addChild(const char *name, const char *value);
	Ptr<Json> addChild(const char *name, double value);
	void clearChildren();
	void clear();
	void copy(const Ptr<Json> &source);
	Ptr<Json> find(const char *name);
	const char *getTypeName() const;
	bool isArray() const;
	void setArray();
	bool isBool() const;
	void setBool(bool arg1);
	bool getBool() const;
	bool isChild(const char *name) const;
	Ptr<Json> getChild(int num) const;
	Ptr<Json> getChild(const char *name) const;
	bool isNull() const;
	void setNull();
	bool isObject() const;
	void setObject();
	bool isString() const;
	void setString(const char *arg1);
	String getString() const;
	bool isNumber() const;
	void setNumber(double arg1);
	double getNumber() const;
	int getInt() const;
	void setInt(int value);
	int getInt(const char *name) const;
	void setInt(const char *name, int value);
	Math::vec2 getVec2() const;
	void setVec2(const Math::vec2 &value);
	Math::vec3 getVec3() const;
	void setVec3(const Math::vec3 &value);
	Math::vec4 getVec4() const;
	void setVec4(const Math::vec4 &value);
	Math::dvec2 getDVec2() const;
	void setDVec2(const Math::dvec2 &value);
	Math::dvec3 getDVec3() const;
	void setDVec3(const Math::dvec3 &value);
	Math::dvec4 getDVec4() const;
	void setDVec4(const Math::dvec4 &value);
	Math::ivec2 getIVec2() const;
	void setIVec2(const Math::ivec2 &value);
	Math::ivec3 getIVec3() const;
	void setIVec3(const Math::ivec3 &value);
	Math::ivec4 getIVec4() const;
	void setIVec4(const Math::ivec4 &value);
	String getSubTree(const char *name = 0);
	String getFormattedSubTree(const char *name = 0);
	const char *getName() const;
	void setName(const char *name);
	int getNumChildren() const;
	Ptr<Json> getParent();
	int load(const char *path);
	int parse(const char *source);
	Ptr<Json> removeChild(const Ptr<Json> &json);
	Ptr<Json> removeChild(const char *name);
	int save(const char *path) const;
	void read(bool &value) const;
	void read(int &value) const;
	void read(char &value) const;
	void read(double &value) const;
	void read(float &value) const;
	void read(Math::vec2 &value) const;
	void read(Math::vec3 &value) const;
	void read(Math::vec4 &value) const;
	void read(Math::dvec2 &value) const;
	void read(Math::dvec3 &value) const;
	void read(Math::dvec4 &value) const;
	void read(Math::ivec2 &value) const;
	void read(Math::ivec3 &value) const;
	void read(Math::ivec4 &value) const;
	void write(const char *value);
	void write(const UGUID &value);
	void write(bool value);
	void write(int value);
	void write(char value);
	void write(double value);
	void write(float value);
	void write(const Math::vec2 &value);
	void write(const Math::vec3 &value);
	void write(const Math::vec4 &value);
	void write(const Math::dvec2 &value);
	void write(const Math::dvec3 &value);
	void write(const Math::dvec4 &value);
	void write(const Math::ivec2 &value);
	void write(const Math::ivec3 &value);
	void write(const Math::ivec4 &value);
	void read(Vector<bool> &value) const;
	void read(Vector<int> &value) const;
	void read(Vector<char> &value) const;
	void read(Vector<double> &value) const;
	void read(Vector<float> &value) const;
	void read(Vector<String> &value) const;
	void read(Vector<Math::vec2> &value) const;
	void read(Vector<Math::vec3> &value) const;
	void read(Vector<Math::vec4> &value) const;
	void read(Vector<Math::dvec2> &value) const;
	void read(Vector<Math::dvec3> &value) const;
	void read(Vector<Math::dvec4> &value) const;
	void read(Vector<Math::ivec2> &value) const;
	void read(Vector<Math::ivec3> &value) const;
	void read(Vector<Math::ivec4> &value) const;
	void write(const Vector<bool> &value);
	void write(const Vector<int> &value);
	void write(const Vector<char> &value);
	void write(const Vector<double> &value);
	void write(const Vector<float> &value);
	void write(const Vector<String> &value);
	void write(const Vector<Math::vec2> &value);
	void write(const Vector<Math::vec3> &value);
	void write(const Vector<Math::vec4> &value);
	void write(const Vector<Math::dvec2> &value);
	void write(const Vector<Math::dvec3> &value);
	void write(const Vector<Math::dvec4> &value);
	void write(const Vector<Math::ivec2> &value);
	void write(const Vector<Math::ivec3> &value);
	void write(const Vector<Math::ivec4> &value);
	String read(const char *name) const;
	void read(const char *name, UGUID &value) const;
	void read(const char *name, bool &value) const;
	void read(const char *name, int &value) const;
	void read(const char *name, unsigned int &value) const;
	void read(const char *name, char &value) const;
	void read(const char *name, double &value) const;
	void read(const char *name, float &value) const;
	void read(const char *name, Math::vec2 &value) const;
	void read(const char *name, Math::vec3 &value) const;
	void read(const char *name, Math::vec4 &value) const;
	void read(const char *name, Math::dvec2 &value) const;
	void read(const char *name, Math::dvec3 &value) const;
	void read(const char *name, Math::dvec4 &value) const;
	void read(const char *name, Math::ivec2 &value) const;
	void read(const char *name, Math::ivec3 &value) const;
	void read(const char *name, Math::ivec4 &value) const;
	void write(const char *name, const char *value);
	void write(const char *name, const UGUID &value);
	void write(const char *name, bool value);
	void write(const char *name, int value);
	void write(const char *name, unsigned int value);
	void write(const char *name, char value);
	void write(const char *name, double value);
	void write(const char *name, float value);
	void write(const char *name, const Math::vec2 &value);
	void write(const char *name, const Math::vec3 &value);
	void write(const char *name, const Math::vec4 &value);
	void write(const char *name, const Math::dvec2 &value);
	void write(const char *name, const Math::dvec3 &value);
	void write(const char *name, const Math::dvec4 &value);
	void write(const char *name, const Math::ivec2 &value);
	void write(const char *name, const Math::ivec3 &value);
	void write(const char *name, const Math::ivec4 &value);
	void read(const char *name, Vector<int> &value) const;
	void read(const char *name, Vector<char> &value) const;
	void read(const char *name, Vector<double> &value) const;
	void read(const char *name, Vector<float> &value) const;
	void read(const char *name, Vector<String> &value) const;
	void read(const char *name, Vector<Math::vec2> &value) const;
	void read(const char *name, Vector<Math::vec3> &value) const;
	void read(const char *name, Vector<Math::vec4> &value) const;
	void read(const char *name, Vector<Math::dvec2> &value) const;
	void read(const char *name, Vector<Math::dvec3> &value) const;
	void read(const char *name, Vector<Math::dvec4> &value) const;
	void read(const char *name, Vector<Math::ivec2> &value) const;
	void read(const char *name, Vector<Math::ivec3> &value) const;
	void read(const char *name, Vector<Math::ivec4> &value) const;
	void write(const char *name, const Vector<int> &value);
	void write(const char *name, const Vector<char> &value);
	void write(const char *name, const Vector<double> &value);
	void write(const char *name, const Vector<float> &value);
	void write(const char *name, const Vector<String> &value);
	void write(const char *name, const Vector<Math::vec2> &value);
	void write(const char *name, const Vector<Math::vec3> &value);
	void write(const char *name, const Vector<Math::vec4> &value);
	void write(const char *name, const Vector<Math::dvec2> &value);
	void write(const char *name, const Vector<Math::dvec3> &value);
	void write(const char *name, const Vector<Math::dvec4> &value);
	void write(const char *name, const Vector<Math::ivec2> &value);
	void write(const char *name, const Vector<Math::ivec3> &value);
	void write(const char *name, const Vector<Math::ivec4> &value);
};
typedef Ptr<Json> JsonPtr;

} // namespace Unigine
