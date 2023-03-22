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

#include "UnigineVector.h"
#include "UnigineString.h"
#include "UnigineMathLib.h"
#include "UniginePtr.h"

namespace Unigine
{


class UNIGINE_API UlonValue : public APIInterface
{
public:
	static Ptr<UlonValue> create();
	static Ptr<UlonValue> create(const char *arg1);
	bool isArray() const;
	const char *getStr() const;
	double getDouble() const;
	float getFloat() const;
	int getInt() const;
	long long getLong() const;
	char getChar() const;
	bool getBool() const;
	Vector<String> getArray() const;
	Math::dvec4 getDVec4() const;
	Math::vec4 getVec4() const;
	Math::ivec4 getIVec4() const;
	void get(Math::dvec4 &ret) const;
	void get(Math::vec4 &ret) const;
	void get(Math::ivec4 &ret) const;
	void setUsage(bool usage);
	bool isUsage() const;
	int read(const char *src);
};
typedef Ptr<UlonValue> UlonValuePtr;


class UNIGINE_API UlonArg : public APIInterface
{
public:
	static Ptr<UlonArg> create();
	String getName() const;
	Ptr<UlonValue> getValue() const;
};
typedef Ptr<UlonArg> UlonArgPtr;


class UNIGINE_API UlonNode : public APIInterface
{
public:
	static Ptr<UlonNode> create();
	bool load(const char *path);
	bool parse(const char *str);
	Vector<Ptr<UlonNode>> getChildren() const;
	const char *getType() const;
	const char *getName() const;
	const char *getCondition() const;
	Ptr<UlonValue> getValue() const;
	Vector<Ptr<UlonArg>> getArgs() const;
	bool isArg(const char *name) const;
	float getArgFloat(const char *name, float ret = 0) const;
	int getArgInt(const char *name, int ret = 0) const;
	long long getArgLong(const char *name, long long ret = 0) const;
	bool getArgBool(const char *name, bool ret = false) const;
	char getArgChar(const char *name, char ret = 0) const;
	const char *getArgStr(const char *name, const char *ret = "") const;
	Vector<String> getArgArray(const char *name) const;
	void printUnusedData(const char *name) const;
	void clearUnusedData() const;
};
typedef Ptr<UlonNode> UlonNodePtr;

} // namespace Unigine
