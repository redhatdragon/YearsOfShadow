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

#include <UniginePtr.h>
#include <UnigineString.h>
#include <UnigineVector.h>

#include "UnigineEditorGlobal.h"

namespace UnigineEditor
{


class UNIGINE_EDITOR_API Collection : public Unigine::APIInterface
{
public:

	enum TYPE
	{
		TYPE_NONE = 0,
		TYPE_BOOL,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_STRING,
	};
	static Unigine::Ptr<Collection> create();
	void clear();
	bool isEmpty() const;
	bool hasValue(const char *value_name) const;
	Unigine::Vector<Unigine::String> getNames() const;
	bool remove(const char *value_name);
	Collection::TYPE getType(const char *value_name) const;
	void setBool(const char *value_name, bool value);
	bool getBool(const char *value_name, bool default_value = false) const;
	void setInt(const char *value_name, int value);
	int getInt(const char *value_name, int default_value = 0) const;
	void setFloat(const char *value_name, float value);
	float getFloat(const char *value_name, float default_value = 0) const;
	void setString(const char *value_name, const char *value);
	Unigine::String getString(const char *value_name, const char *default_value = nullptr) const;
};
typedef Unigine::Ptr<Collection> CollectionPtr;

} // namespace UnigineEditor
