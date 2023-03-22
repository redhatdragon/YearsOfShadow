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

#include "UnigineBase.h"
#include "UnigineString.h"
#include "UnigineCallback.h"

// Unigine compilation flags
enum
{
	UNIGINE_COMPILATION_FLAG_DOUBLE = 1 << 0,
	UNIGINE_COMPILATION_FLAG_DEBUG = 1 << 1,
};

#ifdef UNIGINE_DOUBLE
	#ifdef NDEBUG
		#define UNIGINE_COMPILATION_FLAGS (UNIGINE_COMPILATION_FLAG_DOUBLE)
	#else
		#define UNIGINE_COMPILATION_FLAGS (UNIGINE_COMPILATION_FLAG_DOUBLE | UNIGINE_COMPILATION_FLAG_DEBUG)
	#endif
#else
	#ifdef NDEBUG
		#define UNIGINE_COMPILATION_FLAGS (0)
	#else
		#define UNIGINE_COMPILATION_FLAGS (UNIGINE_COMPILATION_FLAG_DEBUG)
	#endif
#endif

namespace Unigine
{

class CustomSystemProxy;
class Engine;
class SystemLogic;
class WorldLogic;
class EditorLogic;
class Plugin;
class Variable;

/// Free functions for internal usage.
UNIGINE_API Engine* Engine_internal_init(int compilation_flags, CustomSystemProxy *system_proxy, const char* window_title, const char* window_icon_path, const char* app_path, const char* home_path, const char* project, const char* password, int argc, char **argv);
UNIGINE_API Engine* Engine_internal_init(int compilation_flags, CustomSystemProxy *system_proxy, const char* window_title, const char* window_icon_path, const char* app_path, const char* home_path, const char* project, const char* password, int argc, wchar_t **argv);

class Engine
{
public:
	enum BUILD_CONFIG
	{
		BUILD_CONFIG_DEBUG = 0,
		BUILD_CONFIG_DEVELOPMENT,
		BUILD_CONFIG_RELEASE,
	};

	enum PRECISION
	{
		PRECISION_FLOAT = 0,
		PRECISION_DOUBLE,
	};

	enum VIDEO_CONTEXT
	{
		VIDEO_CONTEXT_DEBUG = 1 << 0,
		VIDEO_CONTEXT_BREAK_ON_ERROR = 1 << 1,
		VIDEO_CONTEXT_GPU_SIDE_VALIDATION = 1 << 2,
		VIDEO_CONTEXT_QUAD_BUFFER_CONTEXT = 1 << 3,
	};

	enum CALLBACK_INDEX
	{
		// Engine::update()
		CALLBACK_BEGIN_UPDATE = 0,
			CALLBACK_BEGIN_PROPERTIES_UPDATE,
			  CALLBACK_END_PROPERTIES_UPDATE,
			CALLBACK_BEGIN_INPUT_UPDATE,
			  CALLBACK_END_INPUT_UPDATE,
			CALLBACK_BEGIN_CONTROLS_UPDATE,
			  CALLBACK_END_CONTROLS_UPDATE,
			CALLBACK_BEGIN_WORLD_MANAGER_UPDATE,
			  CALLBACK_END_WORLD_MANAGER_UPDATE,
			CALLBACK_BEGIN_SOUND_MANAGER_UPDATE,
			  CALLBACK_END_SOUND_MANAGER_UPDATE,
			CALLBACK_BEGIN_RENDER_MANAGER_UPDATE,
			  CALLBACK_END_RENDER_MANAGER_UPDATE,
			CALLBACK_BEGIN_LANDSCAPE_UPDATE,
			  CALLBACK_END_LANDSCAPE_UPDATE,
			CALLBACK_BEGIN_LANDSCAPE_ASYNC_UPDATE,
			  CALLBACK_END_LANDSCAPE_ASYNC_UPDATE,
			CALLBACK_BEGIN_GAME_UPDATE,
			  CALLBACK_END_GAME_UPDATE,
			CALLBACK_BEGIN_RENDER_UPDATE,
			  CALLBACK_END_RENDER_UPDATE,
			CALLBACK_BEGIN_EXPRESSION_UPDATE,
			  CALLBACK_END_EXPRESSION_UPDATE,
			CALLBACK_BEGIN_SOUNDS_UPDATE,
			  CALLBACK_END_SOUNDS_UPDATE,

			CALLBACK_BEGIN_PLUGINS_UPDATE,
			  CALLBACK_END_PLUGINS_UPDATE,
			CALLBACK_BEGIN_EDITOR_UPDATE,
			  CALLBACK_END_EDITOR_UPDATE,
			CALLBACK_BEGIN_SYSTEM_SCRIPT_UPDATE,
			  CALLBACK_END_SYSTEM_SCRIPT_UPDATE,
			CALLBACK_BEGIN_SYSTEM_LOGIC_UPDATE,
			  CALLBACK_END_SYSTEM_LOGIC_UPDATE,
			CALLBACK_BEGIN_WORLD_UPDATE,
			  CALLBACK_END_WORLD_UPDATE,
			CALLBACK_BEGIN_WORLD_POST_UPDATE,
			  CALLBACK_END_WORLD_POST_UPDATE,
			CALLBACK_BEGIN_SYSTEM_SCRIPT_POST_UPDATE,
			  CALLBACK_END_SYSTEM_SCRIPT_POST_UPDATE,
			CALLBACK_BEGIN_SYSTEM_LOGIC_POST_UPDATE,
			  CALLBACK_END_SYSTEM_LOGIC_POST_UPDATE,
			CALLBACK_BEGIN_EDITOR_POST_UPDATE,
			  CALLBACK_END_EDITOR_POST_UPDATE,
			CALLBACK_BEGIN_PLUGINS_POST_UPDATE,
			  CALLBACK_END_PLUGINS_POST_UPDATE,

			CALLBACK_BEGIN_SPATIAL_UPDATE,
			  CALLBACK_END_SPATIAL_UPDATE,
			CALLBACK_BEGIN_ASYNC_TASKS_UPDATE,
			  CALLBACK_END_ASYNC_TASKS_UPDATE,
			CALLBACK_BEGIN_FILESYSTEM_UPDATE,
			  CALLBACK_END_FILESYSTEM_UPDATE,
			CALLBACK_BEGIN_PHYSICS,
			CALLBACK_BEGIN_PATHFINDING,
		CALLBACK_END_UPDATE,

		// Engine::render()
		CALLBACK_BEGIN_RENDER,
			CALLBACK_BEGIN_EDITOR_RENDER,
			  CALLBACK_END_EDITOR_RENDER,
			CALLBACK_BEGIN_PLUGINS_RENDER,
			  CALLBACK_END_PLUGINS_RENDER,
			CALLBACK_BEGIN_RENDER_WORLD,
			  CALLBACK_END_RENDER_WORLD,
			CALLBACK_BEGIN_PLUGINS_GUI,
			  CALLBACK_END_PLUGINS_GUI,
			CALLBACK_BEGIN_GUI_RENDER,
			  CALLBACK_END_GUI_RENDER,
			CALLBACK_BEGIN_POST_RENDER,
			  CALLBACK_END_POST_RENDER,
		CALLBACK_END_RENDER,

		// Engine::swap()
		CALLBACK_BEGIN_SWAP,
			CALLBACK_END_PHYSICS,
			CALLBACK_END_PATHFINDING,
			CALLBACK_BEGIN_WORLD_SWAP,
			  CALLBACK_END_WORLD_SWAP,
			CALLBACK_BEGIN_PLUGINS_SWAP,
			  CALLBACK_END_PLUGINS_SWAP,
			CALLBACK_BEGIN_DELETE_OBJECTS,
			  CALLBACK_END_DELETE_OBJECTS,
		CALLBACK_END_SWAP,

		// Engine::focus()
		CALLBACK_FOCUS_GAINED,
		CALLBACK_FOCUS_LOST,

		NUM_CALLBACKS,
	};

	/// Additional parameters to initialize a new Engine instance
	struct InitParameters
	{
		CustomSystemProxy *system_proxy = nullptr;
		const char *window_title = nullptr;
		const char *window_icon_path = nullptr;
		const char *app_path = nullptr;
		const char *home_path = nullptr;
		const char *project = nullptr;
		const char *password = nullptr;
	};

	/// Initializes a new Engine instance.
	static UNIGINE_INLINE Engine *init(int argc, char **argv)
	{
		const InitParameters init_parameters;
		return init(init_parameters, argc, argv);
	}
	/// Initializes a new Engine instance.
	static UNIGINE_INLINE Engine *init(int argc, wchar_t **argv)
	{
		const InitParameters init_parameters;
		return init(init_parameters, argc, argv);
	}

	/// Initializes a new Engine instance.
	static UNIGINE_INLINE Engine *init(const InitParameters &init_parameters, int argc, char **argv)
	{
		const InitParameters &p = init_parameters;
		return Engine_internal_init(UNIGINE_COMPILATION_FLAGS, p.system_proxy, p.window_title, p.window_icon_path, p.app_path, p.home_path, p.project, p.password, argc, argv);
	}
	/// Initializes a new Engine instance.
	static UNIGINE_INLINE Engine *init(const InitParameters &init_parameters, int argc, wchar_t **argv)
	{
		const InitParameters &p = init_parameters;
		return Engine_internal_init(UNIGINE_COMPILATION_FLAGS, p.system_proxy, p.window_title, p.window_icon_path, p.app_path, p.home_path, p.project, p.password, argc, argv);
	}

	/// Deletes the pointer to the existing Engine instance.
	static UNIGINE_API void shutdown();

	/// Returns a pointer to the existing Engine instance.
	static UNIGINE_API Engine *get();

	/// Returns true if the Engine is initialized; otherwise, false.
	static UNIGINE_API bool isInitialized();

	/// Returns Unigine Engine Version.
	static UNIGINE_API const char *getVersion();

	/// Returns Unigine Engine Build Configuration.
	static UNIGINE_API BUILD_CONFIG getBuildConfiguration();

	/// Returns Unigine Engine precision type.
	static UNIGINE_API PRECISION getPrecision();

	/// Returns true if Engine was built as an evaluation version; otherwise false.
	static UNIGINE_API bool isEvaluation();

	/// Returns the list of features (like OpenGL, Direct3D, Microprofile, Geodetic, etc.) as a string 
	static UNIGINE_API const char *getFeatures();

	/// Returns the number of command line arguments.
	virtual int getNumArgs() const = 0;

	/// Returns a command line argument by its index.
	virtual const char *getArg(int num) const = 0;

	/// Returns a command line argument by its index converted to an integer value.
	virtual int getArgi(int num) const = 0;

	/// Returns a command line argument by its index converted to a floating point value.
	virtual float getArgf(int num) const = 0;

	/// Returns video context flags
	virtual int getVideoContextFlags() const = 0;

	/// Returns the number of loaded plugins.
	virtual int getNumPlugins() const = 0;

	/// Finds an index of a loaded plugin by its name.
	virtual int findPlugin(const char *name) const = 0;

	/// Returns the name of the loaded plugin by its index.
	virtual const char *getPluginName(int num) const = 0;

	/// Returns the pointer to the main interface of the loaded plugin by its name.
	template <class SingletonClass> SingletonClass *getPlugin(const char *name)
	{
		int index = Engine::get()->findPlugin(name);
		if (index == -1)
			return nullptr;

		return static_cast<SingletonClass*>(Engine::get()->getPluginData(index));
	}

	/// Returns the instance of the loaded plugin by its index.
	virtual Plugin *getPluginInterface(int num) const = 0;

	/// Returns the data of the loaded plugin by its index.
	virtual void *getPluginData(int num) const = 0;

	/// Returns the execution order of the loaded plugin by its index.
	virtual int getPluginOrder(int num) const = 0;

	/// Adds a plugin to Engine runtime by using a name of this plugin.
	virtual bool addPlugin(const char *name) = 0;

	/// Adds a plugin instance to Engine runtime.
	virtual bool addPlugin(Plugin *plugin) = 0;

	/// Removes a plugin instance from Engine runtime.
	virtual bool removePlugin(Plugin *plugin) = 0;

	/// Returns the number of registered SystemLogic instances.
	virtual int getNumSystemLogics() const = 0;

	/// Returns the registered SystemLogic instance by its number.
	virtual SystemLogic *getSystemLogic(int num) const = 0;

	/// Adds a SystemLogic instance to Engine runtime.
	virtual bool addSystemLogic(SystemLogic *logic) = 0;

	/// Removes a SystemLogic instance from Engine runtime.
	virtual bool removeSystemLogic(SystemLogic *logic) = 0;

	/// Returns the number of registered WorldLogic instances.
	virtual int getNumWorldLogics() const = 0;

	/// Returns the registered WorldLogic instance by its number.
	virtual WorldLogic *getWorldLogic(int num) const = 0;

	/// Adds a WorldLogic instance in Engine runtime.
	virtual bool addWorldLogic(WorldLogic *logic) = 0;

	/// Removes a WorldLogic instance from Engine runtime.
	virtual bool removeWorldLogic(WorldLogic *logic) = 0;

	/// Returns the number of registered EditorLogic instances.
	virtual int getNumEditorLogics() const = 0;

	/// Returns the registered EditorLogic instance.
	virtual EditorLogic *getEditorLogic(int num) const = 0;

	/// Adds an EditorLogic instance to Engine runtime.
	virtual bool addEditorLogic(EditorLogic *logic) = 0;

	/// Removes an EditorLogic instance from Engine runtime.
	virtual bool removeEditorLogic(EditorLogic *logic) = 0;

	/// Returns a path to the binary executable file.
	virtual const char *getAppPath() const = 0;

	/// Returns a path to the user's home directory.
	virtual const char *getHomePath() const = 0;

	/// Returns a path where a default configuration file, save files, and screenshots will be stored.
	virtual const char *getSavePath() const = 0;

	/// Returns a path where cache files will be stored.
	virtual const char *getCachePath() const = 0;

	/// Returns a path to the Engine's dynamic library file.
	virtual const char *getLibraryModulePath() const = 0;

	/// Returns a path to data directory.
	virtual const char *getDataPath() const = 0;

	/// Returns a number of plugin directories (provided with -plugin_path command line argument).
	virtual int getNumPluginPaths() const = 0;

	/// Returns a path to plugin directory by its index.
	virtual const char *getPluginPath(int num) const = 0;

	/// Returns a path to the system script (provided with -system_script command line argument).
	virtual const char *getSystemScript() const = 0;

	/// Returns a path to the editor script (provided with -editor_script command line argument).
	virtual const char *getEditorScript() const = 0;

	/// Returns a path to the system script cache (provided with -system_cache command line argument).
	virtual const char *getSystemCache() const = 0;

	/// Returns a path to the editor script cache (provided with -editor_cache command line argument).
	virtual const char *getEditorCache() const = 0;

	/// Returns a graphics API used for rendering (provided with -video_app command line argument).
	virtual const char *getVideoApp() const = 0;

	/// Returns a sound API used for rendering (provided with -sound_app command line argument).
	virtual const char *getSoundApp() const = 0;

	/// Returns Unigine Script defines specified at startup (provided with -extern_define command line argument).
	virtual const char *getExternDefine() const = 0;

	/// Sets the value indicating whether an application windows is updated when all window is hidden or out of focus.
	virtual void setBackgroundUpdate(bool enabled) = 0;

	/// Returns a value indicating whether an application windows is updated when all window is hidden or out of focus.
	virtual bool isBackgroundUpdate() const = 0;

	/// Return the active state of engine
	virtual bool isActive() const = 0;
	
	/// Return the focus state of engine
	virtual bool isFocus() const = 0;

	/// Returns true when the Engine is quitting; otherwise, false.
	virtual bool isQuit() const = 0;

	/// Engine iterate function. This function must be called every frame.
	virtual void iterate() = 0;

	/// Engine main loop. Replaces while (isQuit() == 0) { iterate(); } commands.
	virtual void main(SystemLogic *system = nullptr, WorldLogic *world = nullptr, EditorLogic *editor = nullptr) = 0;

	/// The engine requests to exit the application.
	virtual void quit() = 0;

	/// Stops the FPS counter.
	virtual void stopFps() = 0;

	/// Starts the FPS counter if it was stopped.
	virtual void startFps() = 0;

	/// Returns the engine FPS counter value.
	virtual float getFps() const = 0;

	/// Returns an inverse value of engine FPS counter.
	virtual float getIFps() const = 0;

	/// Returns the minimum FPS counter value for the last 600 frames.
	virtual float getStatisticsFpsMin() const = 0;

	/// Returns the average FPS counter value for the last 600 frames.
	virtual float getStatisticsFpsAvg() const = 0;

	/// Returns the maximum FPS counter value for the last 600 frames.
	virtual float getStatisticsFpsMax() const = 0;

	/// Returns true when the function is called from the system script; otherwise, false.
	virtual bool isSystemInterpreter() const = 0;

	/// Returns an instance of the system interpreter.
	virtual void *getSystemInterpreter() const = 0;

	/// Returns true if the system script variable exists; otherwise, false.
	virtual bool isSystemVariable(const char *name) const = 0;

	/// Sets a system script variable by a name.
	virtual void setSystemVariable(const char *name, const Variable &v) = 0;

	/// Returns the system script variable by its name.
	virtual const Variable &getSystemVariable(const char *name) const = 0;

	/// Returns true if the system script function exists; otherwise, false.
	virtual bool isSystemFunction(const char *name, int num_args) const = 0;

	/// Returns system script function identifier.
	virtual int getSystemFunction(const char *name, int num_args) const = 0;

	/// Runs system script function by its name.
	virtual const Variable &runSystemFunction(const Variable &name) = 0;

	/// Runs the system script function by its name. The target function can receive up to 8 arguments.
	virtual const Variable &runSystemFunction(const Variable &name, const Variable *args, int num_args) = 0;

	/// Runs system script function by its name.
	virtual const Variable &runSystemFunction(const Variable &name, const Variable &a0) = 0;

	/// Runs system script function by its name.
	virtual const Variable &runSystemFunction(const Variable &name, const Variable &a0, const Variable &a1) = 0;

	/// Runs system script function by its name.
	virtual const Variable &runSystemFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2) = 0;

	/// Runs system script function by its name.
	virtual const Variable &runSystemFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3) = 0;

	/// Runs system script function by its name.
	virtual const Variable &runSystemFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4) = 0;

	/// Runs system script function by its name.
	virtual const Variable &runSystemFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5) = 0;

	/// Runs system script function by its name.
	virtual const Variable &runSystemFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6) = 0;

	/// Runs system script function by its name.
	virtual const Variable &runSystemFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6, const Variable &a7) = 0;

	/// Runs system script function by its identifier.
	virtual const Variable &runSystemFunction(int id) = 0;

	/// Runs the system script function by its identifier. The target function can receive up to 8 arguments.
	virtual const Variable &runSystemFunction(int id, const Variable *args, int num_args) = 0;

	/// Runs system script function by its identifier.
	virtual const Variable &runSystemFunction(int id, const Variable &a0) = 0;

	/// Runs system script function by its identifier.
	virtual const Variable &runSystemFunction(int id, const Variable &a0, const Variable &a1) = 0;

	/// Runs system script function by its identifier.
	virtual const Variable &runSystemFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2) = 0;

	/// Runs system script function by its identifier.
	virtual const Variable &runSystemFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3) = 0;

	/// Runs system script function by its identifier.
	virtual const Variable &runSystemFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4) = 0;

	/// Runs system script function by its identifier.
	virtual const Variable &runSystemFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5) = 0;

	/// Runs system script function by its identifier.
	virtual const Variable &runSystemFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6) = 0;

	/// Runs system script function by its identifier.
	virtual const Variable &runSystemFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6, const Variable &a7) = 0;

	/// Returns true if the world script is loaded; otherwise, false.
	virtual bool isWorldLoaded() const = 0;

	/// Returns true if the function is called from the world script; otherwise, false.
	virtual bool isWorldInterpreter() const = 0;

	/// Returns the world interpreter instance.
	virtual void *getWorldInterpreter() const = 0;

	/// Returns true if the world script variable exists; otherwise, false.
	virtual bool isWorldVariable(const char *name) const = 0;

	/// Sets a world script variable by its name.
	virtual void setWorldVariable(const char *name, const Variable &v) = 0;

	/// Returns a world script variable by its name.
	virtual const Variable &getWorldVariable(const char *name) const = 0;

	/// Returns true if the world script function exists; otherwise, false.
	virtual bool isWorldFunction(const char *name, int num_args) const = 0;

	/// Returns the world function identifier.
	virtual int getWorldFunction(const char *name, int num_args) const = 0;

	/// Runs the world script function by its name.
	virtual const Variable &runWorldFunction(const Variable &name) = 0;

	/// Runs the world script function by its name. The target function can receive up to 8 arguments.
	virtual const Variable &runWorldFunction(const Variable &name, const Variable *args, int num_args) = 0;

	/// Runs the world script function by its name.
	virtual const Variable &runWorldFunction(const Variable &name, const Variable &a0) = 0;

	/// Runs the world script function by its name.
	virtual const Variable &runWorldFunction(const Variable &name, const Variable &a0, const Variable &a1) = 0;

	/// Runs the world script function by its name.
	virtual const Variable &runWorldFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2) = 0;

	/// Runs the world script function by its name.
	virtual const Variable &runWorldFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3) = 0;

	/// Runs the world script function by its name.
	virtual const Variable &runWorldFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4) = 0;

	/// Runs the world script function by its name.
	virtual const Variable &runWorldFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5) = 0;

	/// Runs the world script function by its name.
	virtual const Variable &runWorldFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6) = 0;

	/// Runs the world script function by its name.
	virtual const Variable &runWorldFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6, const Variable &a7) = 0;

	/// Runs the world script function by its identifier.
	virtual const Variable &runWorldFunction(int id) = 0;

	/// Runs the world script function by its identifier. The target function can receive up to 8 arguments.
	virtual const Variable &runWorldFunction(int id, const Variable *args, int num_args) = 0;

	/// Runs the world script function by its identifier.
	virtual const Variable &runWorldFunction(int id, const Variable &a0) = 0;

	/// Runs the world script function by its identifier.
	virtual const Variable &runWorldFunction(int id, const Variable &a0, const Variable &a1) = 0;

	/// Runs the world script function by its identifier.
	virtual const Variable &runWorldFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2) = 0;

	/// Runs the world script function by its identifier.
	virtual const Variable &runWorldFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3) = 0;

	/// Runs the world script function by its identifier.
	virtual const Variable &runWorldFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4) = 0;

	/// Runs the world script function by its identifier.
	virtual const Variable &runWorldFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5) = 0;

	/// Runs the world script function by its identifier.
	virtual const Variable &runWorldFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6) = 0;

	/// Runs the world script function by its identifier.
	virtual const Variable &runWorldFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6, const Variable &a7) = 0;

	/// Returns true if the editor script is loaded; otherwise, false.
	virtual bool isEditorLoaded() const = 0;

	/// Returns true if the function is called from the editor script; otherwise, false.
	virtual bool isEditorInterpreter() const = 0;

	/// Returns the editor interpreter instance.
	virtual void *getEditorInterpreter() const = 0;

	/// Returns true if the editor script variable exists; otherwise, false.
	virtual bool isEditorVariable(const char *name) const = 0;

	/// Sets the editor script variable by its name.
	virtual void setEditorVariable(const char *name, const Variable &v) = 0;

	/// Returns the editor script variable by its name.
	virtual const Variable &getEditorVariable(const char *name) const = 0;

	/// Returns true if the editor script function exists; otherwise, false.
	virtual bool isEditorFunction(const char *name, int num_args) const = 0;

	/// Returns the editor function identifier.
	virtual int getEditorFunction(const char *name, int num_args) const = 0;

	/// Runs the editor script function by its name.
	virtual const Variable &runEditorFunction(const Variable &name) = 0;

	/// Runs the editor script function by its name. The target function can receive up to 8 arguments.
	virtual const Variable &runEditorFunction(const Variable &name, const Variable *args, int num_args) = 0;

	/// Runs the editor script function by its name.
	virtual const Variable &runEditorFunction(const Variable &name, const Variable &a0) = 0;

	/// Runs the editor script function by its name.
	virtual const Variable &runEditorFunction(const Variable &name, const Variable &a0, const Variable &a1) = 0;

	/// Runs the editor script function by its name.
	virtual const Variable &runEditorFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2) = 0;

	/// Runs the editor script function by its name.
	virtual const Variable &runEditorFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3) = 0;

	/// Runs the editor script function by its name.
	virtual const Variable &runEditorFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4) = 0;

	/// Runs the editor script function by its name.
	virtual const Variable &runEditorFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5) = 0;

	/// Runs the editor script function by its name.
	virtual const Variable &runEditorFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6) = 0;

	/// Runs the editor script function by its name.
	virtual const Variable &runEditorFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6, const Variable &a7) = 0;

	/// Runs the editor script function by its identifier.
	virtual const Variable &runEditorFunction(int id) = 0;

	/// Runs the editor script function by its identifier. The target function can receive up to 8 arguments.
	virtual const Variable &runEditorFunction(int id, const Variable *args, int num_args) = 0;

	/// Runs the editor script function by its identifier.
	virtual const Variable &runEditorFunction(int id, const Variable &a0) = 0;

	/// Runs the editor script function by its identifier.
	virtual const Variable &runEditorFunction(int id, const Variable &a0, const Variable &a1) = 0;

	/// Runs the editor script function by its identifier.
	virtual const Variable &runEditorFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2) = 0;

	/// Runs the editor script function by its identifier.
	virtual const Variable &runEditorFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3) = 0;

	/// Runs the editor script function by its identifier.
	virtual const Variable &runEditorFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4) = 0;

	/// Runs the editor script function by its identifier.
	virtual const Variable &runEditorFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5) = 0;

	/// Runs the editor script function by its identifier.
	virtual const Variable &runEditorFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6) = 0;

	/// Runs the editor script function by its identifier.
	virtual const Variable &runEditorFunction(int id, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6, const Variable &a7) = 0;

	/// Returns a total accumulative time spent on preparing Engine frames (includes update, render and swap, present).
	virtual float getTotalTime() const = 0;

	/// Returns a accumulative time spent on preparing Engine frames (includes update, render and swap).
	virtual float getTotalCPUTime() const = 0;

	/// Returns a total accumulative time spent on updating the scene.
	virtual float getUpdateTime() const = 0;

	/// Returns a total accumulative time spent on rendering the scene.
	virtual float getRenderTime() const = 0;

	/// Returns a total accumulative time spent on waiting for the gpu.
	virtual float getPresentTime() const = 0;

	/// Returns a total accumulative time spent on rendering the interface.
	virtual float getInterfaceTime() const = 0;

	/// Returns current Engine frame
	virtual long long getFrame() const = 0;

	/// Returns true if the function from the main Engine thread; otherwise, false.
	virtual bool isMainThread() const = 0;

	/// Callbacks
	virtual void *addCallback(CALLBACK_INDEX callback, CallbackBase *func) = 0;
	virtual bool removeCallback(CALLBACK_INDEX callback, void *id) = 0;
	virtual void clearCallbacks(CALLBACK_INDEX callback) = 0;

protected:
	virtual ~Engine() { }
};

// Unigine Engine smart pointer
class EnginePtr
{
public:
	/// Initializes the pointer to a new Engine instance.
	UNIGINE_INLINE EnginePtr() : engine(0), owner(0) { init(); }

	/// Initializes the pointer to an existing Engine instance.
	UNIGINE_INLINE EnginePtr(const EnginePtr &ptr)
		: engine(ptr.engine), owner(0)
	{}

	/// Initializes the pointer to a new Engine instance.
	UNIGINE_INLINE EnginePtr(int argc, char **argv)
		: engine(0), owner(0)
	{
		const Engine::InitParameters init_parameters;
		init(init_parameters, argc, argv);
	}
	/// Initializes the pointer to a new Engine instance.
	UNIGINE_INLINE EnginePtr(int argc, wchar_t **argv)
		: engine(0), owner(0)
	{
		const Engine::InitParameters init_parameters;
		init(init_parameters, argc, argv);
	}

	/// Initializes the pointer to a new Engine instance.
	UNIGINE_INLINE EnginePtr(const Engine::InitParameters &init_parameters, int argc, char **argv)
		: engine(0), owner(0)
	{
		init(init_parameters, argc, argv);
	}
	/// Initializes the pointer to a new Engine instance.
	UNIGINE_INLINE EnginePtr(const Engine::InitParameters &init_parameters, int argc, wchar_t **argv)
		: engine(0), owner(0)
	{
		init(init_parameters, argc, argv);
	}

	UNIGINE_INLINE ~EnginePtr()
	{
		if (isOwner())
			shutdown();
	}

	/// Sets the owner flag for the pointer. Engine instance will be destroyed along with the pointer.
	UNIGINE_INLINE void grab() { owner = 1; }

	/// Releases the owner flag for the pointer. Engine instance will stay alive upon the pointer destruction.
	UNIGINE_INLINE void release() { owner = 0; }

	/// Returns the owner flag indicating whether Engine instance will be destroyed along with the pointer or not.
	UNIGINE_INLINE int isOwner() const { return owner; }

	/// Initializes the pointer to a new Engine instance.
	UNIGINE_INLINE void init()
	{
		if (Engine::isInitialized())
			engine = Engine::get();
	}

	/// Initializes the pointer to a new Engine instance.
	UNIGINE_INLINE void init(const Engine::InitParameters &init_parameters, int argc, char **argv)
	{
		UNIGINE_ASSERT(!engine && "Unigine::EnginePtr::init(): Engine is already initialized");
		engine = Engine::init(init_parameters, argc, argv);
		owner = 1;
	}

	/// Initializes the pointer to a new Engine instance.
	UNIGINE_INLINE void init(const Engine::InitParameters &init_parameters, int argc, wchar_t **argv)
	{
		UNIGINE_ASSERT(!engine && "Unigine::EnginePtr::init(): Engine is already initialized");
		engine = Engine::init(init_parameters, argc, argv);
		owner = 1;
	}

	/// Destroys the existing Engine instance. The pointer must be an owner of the Engine instance.
	UNIGINE_INLINE void shutdown()
	{
		UNIGINE_ASSERT(owner && "Unigine::EnginePtr::shutdown(): is not an owner");
		Engine::shutdown();
		engine = nullptr;
		owner = 0;
	}

	UNIGINE_INLINE EnginePtr &operator=(const EnginePtr &ptr)
	{
		engine = ptr.engine;
		owner = 0;
		return *this;
	}

	UNIGINE_INLINE Engine *operator->() const
	{
		UNIGINE_ASSERT(engine && "Unigine::EnginePtr::operator->(): pointer is NULL");
		return engine;
	}

	/// Return a raw pointer to the Engine instance.
	UNIGINE_INLINE Engine *get() const { return engine;	}

private:
	Engine *engine;
	int owner;
};

} // namespace Unigine
