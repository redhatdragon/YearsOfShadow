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
#include "UnigineNodes.h"
#include "UnigineVector.h"
#include "UnigineGUID.h"
#include "UnigineMaterial.h"
#include "UnigineWindowManager.h"

namespace Unigine
{

// Unigine SystemLogic allow user to write application logic in C++ in the same manner as in UnigineScript.
// Execution sequence and method names are the same as in system script.
class SystemLogic
{
public:
	virtual ~SystemLogic() {}

	// Engine calls this function on initialization.
	// Similar to system script's init function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int init() { return 1; }

	// Engine calls this function on shutdown.
	// Similar to system script's shutdown function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int shutdown() { return 1; }

	// Engine calls this function before updating each render frame.
	// Similar to system script's update function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int update() { return 1; }

	// Engine calls this function after updating each render frame.
	// Similar to system script's postUpdate function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int postUpdate() { return 1; }
};

// Unigine WorldLogic allow user to write application logic in C++ in the same manner as in UnigineScript.
// Execution sequence and method names are the same as in world script.
class WorldLogic
{
public:
	virtual ~WorldLogic() {}

	// Engine calls this function on world initialization.
	// Similar to world script's init function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int init() { return 1; }

	// Engine calls this function on world shutdown.
	// Similar to world script's shutdown function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int shutdown() { return 1; }

	// Engine calls this function "size" times before updating each render frame.
	// size - count of threads
	// id - thread number
	// Non blocking function, works like physics thread (from start of frame to the swap() function)
	virtual void updateAsyncThread(int id, int size) { UNIGINE_UNUSED(id); UNIGINE_UNUSED(size); }

	// Engine calls this function "size" times before updating each render frame.
	// size - count of threads
	// id - thread number
	// Main thread is blocked until all updateSyncThread is executed
	virtual void updateSyncThread(int id, int size) { UNIGINE_UNUSED(id); UNIGINE_UNUSED(size); }

	// Engine calls this function before updating each render frame.
	// Similar to world script's update function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int update() { return 1; }

	// Engine calls this function after updating each render frame.
	// Similar to world script's postUpdate function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int postUpdate() { return 1; }

	// Engine calls this function before updating each physics frame.
	// Similar to world script's updatePhysics function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int updatePhysics() { return 1; }

	// Engine calls this function before swap buffers every frame
	// (at the moment all threads have already finished their work).
	// Returns 1 on success, or 0 if an error occurred.
	virtual int swap() { return 1; }
	
	// Override this function if application needs to serialize user data to a file.
	// Engine calls this function when the world is saving its state (i.e. state_save is called).
	// Similar to world script's save function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int save(const StreamPtr &stream) { UNIGINE_UNUSED(stream); return 1; }

	// Override this function if application needs to restore user data from a file.
	// Engine calls this function when the world is restoring its state (i.e. state_restore is called).
	// Similar to world script's restore function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int restore(const StreamPtr &stream) { UNIGINE_UNUSED(stream); return 1; }
};

// Unigine EditorLogic allow user to write application logic in C++ in the same manner as in UnigineScript.
// Execution sequence and method names are the same as in editor script.
class EditorLogic
{
public:
	virtual ~EditorLogic() {}

	// Engine calls this function on world initialization.
	// Similar to world script's init function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int init() { return 1; }

	// Engine calls this function on editor shutdown.
	// Similar to editor script's shutdown function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int shutdown() { return 1; }

	// If editor is loaded, engine calls this function before updating each render frame.
	// Similar to editor script's update function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int update() { return 1; }

	// If editor is loaded, engine calls this function after updating each render frame.
	// Similar to editor script's postUpdate function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int postUpdate() { return 1; }

	// If editor is loaded, engine calls this function before rendering each render frame.
	// Similar to editor script's render function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int render(const EngineWindowPtr &window) { UNIGINE_UNUSED(window); return 1; }

	// If editor is loaded, engine calls this function on world initialization.
	// Similar to editor script's worldInit function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int worldInit() { return 1; }

	// If editor is loaded, engine calls this function on world shutdown.
	// Similar to editor script's worldShutdown function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int worldShutdown() { return 1; }

	// If editor is loaded, engine calls this function on world save.
	// Similar to editor script's worldSave function.
	// Returns 1 on success, or 0 if an error occurred.
	virtual int worldSave() { return 1; }

	// Node-related signals.
	virtual void clear() {}
	virtual void nodeShowInEditorChanged(const NodePtr &node) { UNIGINE_UNUSED(node); }
	virtual void nodeReparented(const NodePtr &node) { UNIGINE_UNUSED(node); }
	virtual void nodeReordered(const NodePtr &node) { UNIGINE_UNUSED(node); }
	virtual void nodeRenamed(const NodePtr &node, const char *old_name) { UNIGINE_UNUSED(node); UNIGINE_UNUSED(old_name); }

	// Material-related signals.
	virtual void materialAdded(const UGUID &guid) { UNIGINE_UNUSED(guid); }
	virtual void materialRemoved(const UGUID &guid) { UNIGINE_UNUSED(guid); }
	virtual void materialChanged(const UGUID &guid) { UNIGINE_UNUSED(guid); }
	virtual void materialReparented(const UGUID &guid, const UGUID &old_parent, const UGUID &new_parent)
	{
		UNIGINE_UNUSED(guid);
		UNIGINE_UNUSED(old_parent);
		UNIGINE_UNUSED(new_parent);
	}

	virtual void materialCompileShadersBegin() {}
	virtual void materialCompileShadersEnd() {}
	virtual void materialCompileShaders(const MaterialPtr &material, int num) { UNIGINE_UNUSED(material); UNIGINE_UNUSED(num); }

	// Properties` related signals.
	virtual void propertyAdded(const UGUID &guid) { UNIGINE_UNUSED(guid); }
	virtual void propertyRemoved(const UGUID &guid) { UNIGINE_UNUSED(guid); }
	virtual void propertyMoved(const UGUID &guid) { UNIGINE_UNUSED(guid); }
	virtual void propertyChanged(const UGUID &guid) { UNIGINE_UNUSED(guid); }
	virtual void propertyReplaced(const UGUID &guid, const UGUID &new_guid) { UNIGINE_UNUSED(guid); UNIGINE_UNUSED(new_guid);}
	virtual void propertyReparented(const UGUID &guid, const UGUID &old_parent, const UGUID &new_parent)
	{
		UNIGINE_UNUSED(guid);
		UNIGINE_UNUSED(old_parent);
		UNIGINE_UNUSED(new_parent);
	}
};

} // namespace Unigine
