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

#include "UnigineCallback.h"
#include "UnigineMathLib.h"
#include "UniginePtr.h"
#include "UnigineXml.h"
#include "UnigineStreams.h"
#include "UnigineJson.h"

namespace Unigine
{


class UNIGINE_API Curve2d : public APIInterface
{
public:

	enum REPEAT_MODE
	{
		REPEAT_MODE_CLAMP = 0,
		REPEAT_MODE_LOOP,
		REPEAT_MODE_PING_PONG,
		NUM_REPEAT_MODES,
	};
	static Ptr<Curve2d> create();
	static Ptr<Curve2d> create(const Ptr<Curve2d> &curve);
	void clear();
	void copy(const Ptr<Curve2d> &curve);
	int getHash() const;
	int getNumKeys() const;
	int addKey(const Math::vec2 &point);
	int addKey(const Math::vec2 &point, const Math::vec2 &left_tangent, const Math::vec2 &right_tangent);
	void removeKey(int index);
	int moveKey(int index, const Math::vec2 &point);
	void sortKeys();
	void setKeyPoint(int index, const Math::vec2 &point);
	void setKeyLeftTangent(int index, const Math::vec2 &point);
	void setKeyRightTangent(int index, const Math::vec2 &point);
	Math::vec2 getKeyPoint(int index) const;
	Math::vec2 getKeyLeftTangent(int index) const;
	Math::vec2 getKeyRightTangent(int index) const;
	bool saveState(const Ptr<Stream> &stream) const;
	bool restoreState(const Ptr<Stream> &stream);
	float evaluate(float time) const;
	bool save(const Ptr<Xml> &xml) const;
	bool load(const Ptr<Xml> &xml);
	bool save(const Ptr<Json> &json) const;
	bool load(const Ptr<Json> &json);
	void *addChangedCallback(CallbackBase *func);
	bool removeChangedCallback(void *id);
	void clearChangedCallbacks();
	void setRepeatModeStart(Curve2d::REPEAT_MODE start);
	Curve2d::REPEAT_MODE getRepeatModeStart() const;
	void setRepeatModeEnd(Curve2d::REPEAT_MODE end);
	Curve2d::REPEAT_MODE getRepeatModeEnd() const;
};
typedef Ptr<Curve2d> Curve2dPtr;

} // namespace Unigine
