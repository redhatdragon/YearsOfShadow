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

#include "UnigineEngine.h"
#include "UnigineXml.h"
#include "UnigineStreams.h"
#include "UnigineWindowManager.h"

namespace Unigine
{

// Unigine Plugin allows loading a custom library dynamically at Unigine runtime.
class Plugin
{
public:
	virtual ~Plugin() { }

	// Must return the UNIGINE compilation flags for which the plugin is built (please, see UNIGINE_COMPILATION_FLAG_).
	virtual int getCompilationFlags() const { return UNIGINE_COMPILATION_FLAGS; }

	// May return the plugin name.
	virtual const char *get_name() { return nullptr; }

	// May return the plugin data.
	virtual void *get_data() { return nullptr; }

	// Must return the plugin execution order.
	virtual int get_order() { return 0; }

	// The engine calls this function on plugin initialization.
	// Should return 1 on success, or 0 if an error occurred.
	virtual int init() { return 1; }

	// The engine calls this function on plugin shutdown.
	// Should return 1 on success, or 0 if an error occurred.
	virtual int shutdown() { return 1; }

	// The engine calls this function before updating each render frame.
	virtual void update() { }

	// The engine calls this function after updating each render frame.
	virtual void postUpdate() { }

	// The engine calls this function before rendering each render frame.
	virtual void render(const EngineWindowPtr &window) { }

	// The engine calls this function before each GUI render frame.
	virtual void gui(EngineWindowPtr &window) { }

	// The engine calls this function before updating each physics frame.
	virtual void updatePhysics() { }

	// The engine calls this function before swapping each render frame.
	virtual void swap() { }

	// The engine calls this function on world loading.
	// Should return 1 on success, or 0 if an error occurred.
	virtual int loadWorld(const XmlPtr &xml) { UNIGINE_UNUSED(xml); return 1; }

	// The engine calls this function on world saving.
	// Shoukld return 1 on success, or 0 if an error occurred.
	virtual int saveWorld(const XmlPtr &xml) { UNIGINE_UNUSED(xml); return 1; }

	// The engine calls this function on world saving.
	// Should return 1 on success, or 0 if an error occurred.
	virtual int saveState(const StreamPtr &stream) { UNIGINE_UNUSED(stream); return 1; }

	// The engine calls this function on world restoring.
	// Should return 1 on success, or 0 if an error occurred.
	virtual int restoreState(const StreamPtr &stream) { UNIGINE_UNUSED(stream); return 1; }
};

} // namespace Unigine
