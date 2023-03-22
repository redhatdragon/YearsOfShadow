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
#include "UnigineGUID.h"
#include "UnigineStreams.h"

namespace Unigine
{


class UNIGINE_API Xml : public APIInterface
{
public:
	static Ptr<Xml> create();
	static Ptr<Xml> create(const char *name, const char *args = 0);
	static Ptr<Xml> create(const Ptr<Xml> &xml);
	bool parse(const char *src);
	bool load(const char *name, bool skip_errors = false);
	bool save(const char *name, bool binary = false, const char *indent = "\t") const;
	bool load(Ptr<Stream> &stream, size_t read_size, bool binary = false, bool skip_errors = false);
	bool save(Ptr<Stream> &stream, bool binary = false, const char *indent = "\t") const;
	const char *getLoadPath() const;
	void clear();
	void setName(const char *name);
	const char *getName() const;
	Ptr<Xml> getParent() const;
	void clearChildren();
	bool isChild(const char *name) const;
	int findChild(const char *name) const;
	int getNumChildren() const;
	Ptr<Xml> getChild(int num) const;
	Ptr<Xml> getChild(const char *name) const;
	Ptr<Xml> addChild(const Ptr<Xml> &xml) const;
	Ptr<Xml> addChild(const char *name, const char *args = 0) const;
	Ptr<Xml> removeChild(const Ptr<Xml> &xml);
	Ptr<Xml> find(const char *path) const;
	String getSubTree(const char *path = 0) const;
	String getFormattedSubTree(const char *path = 0, const char *indent = "\t") const;
	void printUnusedData(const char *name) const;
	void clearUnusedData();
	bool isArg(const char *name) const;
	int getNumArgs() const;
	int findArg(const char *name) const;
	void setArgName(int num, const char *name);
	const char *getArgName(int num) const;
	void setArgValue(int num, const char *value);
	const char *getArgValue(int num) const;
	bool setArg(const char *name, const char *value);
	const char *getArg(const char *name) const;
	int removeArg(const char *name);
	bool setBoolArg(const char *name, bool value);
	bool getBoolArg(const char *name) const;
	bool getBoolArg(const char *name, bool value) const;
	bool setIntArg(const char *name, int value, int radix = 10);
	int getIntArg(const char *name) const;
	int getIntArg(const char *name, int value) const;
	bool setIntArrayArg(const char *name, const int *src, int src_size, int radix = 10);
	bool getIntArrayArg(const char *name, int *dest, int dest_size) const;
	bool setFloatArg(const char *name, float value);
	float getFloatArg(const char *name) const;
	float getFloatArg(const char *name, float value) const;
	bool setFloatArrayArg(const char *name, const float *src, int src_size);
	bool getFloatArrayArg(const char *name, float *dest, int dest_size) const;
	bool setDoubleArg(const char *name, double value);
	double getDoubleArg(const char *name) const;
	double getDoubleArg(const char *name, double value) const;
	bool setDoubleArrayArg(const char *name, const double *src, int src_size);
	bool getDoubleArrayArg(const char *name, double *dest, int dest_size) const;
	bool setVec2Arg(const char *name, const Math::vec2 &value);
	Math::vec2 getVec2Arg(const char *name) const;
	bool setVec3Arg(const char *name, const Math::vec3 &value);
	Math::vec3 getVec3Arg(const char *name) const;
	bool setVec4Arg(const char *name, const Math::vec4 &value);
	Math::vec4 getVec4Arg(const char *name) const;
	bool setDVec2Arg(const char *name, const Math::dvec2 &value);
	Math::dvec2 getDVec2Arg(const char *name) const;
	bool setDVec3Arg(const char *name, const Math::dvec3 &value);
	Math::dvec3 getDVec3Arg(const char *name) const;
	bool setDVec4Arg(const char *name, const Math::dvec4 &value);
	Math::dvec4 getDVec4Arg(const char *name) const;
	bool setIVec2Arg(const char *name, const Math::ivec2 &value);
	Math::ivec2 getIVec2Arg(const char *name) const;
	bool setIVec3Arg(const char *name, const Math::ivec3 &value);
	Math::ivec3 getIVec3Arg(const char *name) const;
	bool setIVec4Arg(const char *name, const Math::ivec4 &value);
	Math::ivec4 getIVec4Arg(const char *name) const;
	bool setMat4Arg(const char *name, const Math::mat4 &value);
	Math::mat4 getMat4Arg(const char *name) const;
	bool setDMat4Arg(const char *name, const Math::dmat4 &value);
	Math::dmat4 getDMat4Arg(const char *name) const;
	bool setQuatArg(const char *name, const Math::quat &value);
	Math::quat getQuatArg(const char *name) const;
	bool setPaletteArg(const char *name, const Palette &value);
	Palette getPaletteArg(const char *name) const;
	bool setStringArrayArg(const char *name, const Vector<String> &src);
	bool getStringArrayArg(const char *name, Vector<String> &dest) const;
	void setData(const char *data);
	const char *getData() const;
	bool setRawData(const char *data);
	bool setXmlData(const char *data);
	const char *getChildData(const char *name) const;
	void setBoolData(bool data);
	bool getBoolData() const;
	bool setIntData(int value, int radix = 10);
	int getIntData() const;
	bool setIntArrayData(const int *src, int src_size, int radix = 10);
	bool getIntArrayData(int *dest, int dest_size) const;
	void setLongData(long long data);
	long long getLongData() const;
	void setFloatData(float data);
	float getFloatData() const;
	bool setFloatArrayData(const float *src, int src_size);
	bool getFloatArrayData(float *dest, int dest_size) const;
	void setDoubleData(double data);
	double getDoubleData() const;
	bool setDoubleArrayData(const double *src, int src_size);
	bool getDoubleArrayData(double *dest, int dest_size) const;
	void setVec2Data(const Math::vec2 &data);
	Math::vec2 getVec2Data() const;
	void setVec3Data(const Math::vec3 &data);
	Math::vec3 getVec3Data() const;
	void setVec4Data(const Math::vec4 &data);
	Math::vec4 getVec4Data() const;
	void setDVec2Data(const Math::dvec2 &data);
	Math::dvec2 getDVec2Data() const;
	void setDVec3Data(const Math::dvec3 &data);
	Math::dvec3 getDVec3Data() const;
	void setDVec4Data(const Math::dvec4 &data);
	Math::dvec4 getDVec4Data() const;
	void setIVec2Data(const Math::ivec2 &data);
	Math::ivec2 getIVec2Data() const;
	void setIVec3Data(const Math::ivec3 &data);
	Math::ivec3 getIVec3Data() const;
	void setIVec4Data(const Math::ivec4 &data);
	Math::ivec4 getIVec4Data() const;
	void setMat4Data(const Math::mat4 &data);
	Math::mat4 getMat4Data() const;
	void setDMat4Data(const Math::dmat4 &data);
	Math::dmat4 getDMat4Data() const;
	void setQuatData(const Math::quat &data);
	Math::quat getQuatData() const;
	void setPaletteData(const Palette &data);
	Palette getPaletteData() const;
	bool setStringArrayData(const Vector<String> &src, const char *delimiter = ",");
	bool getStringArrayData(Vector<String> &dest, const char *delimiter = ",") const;
	bool setData(const UGUID &guid);
	bool setData(bool value);
	bool setData(int value, int radix = 10);
	bool setData(long long value);
	bool setData(float value);
	bool setData(double value);
	bool setData(const Math::vec2 &value);
	bool setData(const Math::vec3 &value);
	bool setData(const Math::vec4 &value);
	bool setData(const Math::dvec2 &value);
	bool setData(const Math::dvec3 &value);
	bool setData(const Math::dvec4 &value);
	bool setData(const Math::ivec3 &value);
	bool setData(const Math::ivec2 &value);
	bool setData(const Math::ivec4 &value);
	bool setData(const Math::mat4 &value);
	bool setData(const Math::dmat4 &value);
	bool setData(const Math::quat &value);
	bool setData(const Palette &value);
	void readChildData(const char *name, bool &value) const;
	void readChildData(const char *name, int &value) const;
	void readChildData(const char *name, char &value) const;
	void readChildData(const char *name, long long &value) const;
	void readChildData(const char *name, float &value) const;
	void readChildData(const char *name, double &value) const;
	void readChildData(const char *name, Math::vec2 &value) const;
	void readChildData(const char *name, Math::vec3 &value) const;
	void readChildData(const char *name, Math::vec4 &value) const;
	void readChildData(const char *name, Math::dvec2 &value) const;
	void readChildData(const char *name, Math::dvec3 &value) const;
	void readChildData(const char *name, Math::dvec4 &value) const;
	void readChildData(const char *name, Math::ivec2 &value) const;
	void readChildData(const char *name, Math::ivec3 &value) const;
	void readChildData(const char *name, Math::ivec4 &value) const;
	void readChildData(const char *name, Palette &value) const;
	void readArg(const char *name, bool &value) const;
	void readArg(const char *name, int &value) const;
	void readArg(const char *name, char &value) const;
	void readArg(const char *name, float &value) const;
	void readArg(const char *name, double &value) const;
	void readArg(const char *name, Math::vec2 &value) const;
	void readArg(const char *name, Math::vec3 &value) const;
	void readArg(const char *name, Math::vec4 &value) const;
	void readArg(const char *name, Math::dvec2 &value) const;
	void readArg(const char *name, Math::dvec3 &value) const;
	void readArg(const char *name, Math::dvec4 &value) const;
	void readArg(const char *name, Math::ivec2 &value) const;
	void readArg(const char *name, Math::ivec3 &value) const;
	void readArg(const char *name, Math::ivec4 &value) const;
	void readArg(const char *name, Palette &value) const;
	void copy(const Ptr<Xml> &source);
	static String symbols(const char *arg1);
	static String encode(const char *arg1);
	static String decode(const char *arg1);
};
typedef Ptr<Xml> XmlPtr;

} // namespace Unigine
