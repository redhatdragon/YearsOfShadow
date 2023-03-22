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

#include "UnigineBase.h"
#include "UniginePtr.h"
#include "UnigineString.h"

namespace Unigine
{


class UNIGINE_API RegExp : public APIInterface
{
public:
	static Ptr<RegExp> create();
	static Ptr<RegExp> create(const char *pattern);
	int compile(const char *pattern);
	const char *getGroup(int number) const;
	int getNumGroups() const;
	int isCompiled() const;
	int match(const char *str) const;
	String replace(const char *str, const char *after);
	int search(const char *str) const;
};
typedef Ptr<RegExp> RegExpPtr;

} // namespace Unigine
