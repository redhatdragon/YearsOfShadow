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


#include <editor/UnigineEditorGlobal.h>

#include <UnigineString.h>

#include <QPluginLoader>

////////////////////////////////////////////////////////////////////////////////
// Forward decl.
////////////////////////////////////////////////////////////////////////////////
namespace UnigineEditor
{
class Plugin;
class PluginManager;
class PluginInfoPrivate;
class PluginManagerPrivate;
}


namespace UnigineEditor
{

/// <summary> Defines dependency on a plugin.</summary>
struct UNIGINE_EDITOR_API PluginDependency
{
	/// <summary> Plugin type. Defines whether the plugin is a required or an optional one.</summary>
	enum class Type
	{
		/// <summary> Required UnigineEditor plugin.</summary>
		REQUIRED,
		/// <summary> Optional UnigineEditor plugin.</summary>
		OPTION
	};

	Type type = Type::REQUIRED;
	/// <summary> Plugin name.</summary>
	Unigine::String name;
	/// <summary> Plugin version.</summary>
	Unigine::String version;
};

/// <summary> This class it is responsible for plugin loading/unloading and contains all necessary plugin metadata (name, version, dependencies, etc.).
/// It can also be used to check the current plugin state and get information on errors (if any).
/// </summary>
class UNIGINE_EDITOR_API PluginInfo
{
public:
	/// <summary> Current plugin state.</summary>
	enum class State
	{
		/// <summary> Initial plugin state indicating that initialization either has not yet started or an error has occurred during initialization.</summary>
		INVALID,
		/// <summary> Plugin metadata is read and processed successfully.</summary>
		READ,
		/// <summary> All required dependencies of the plugin are resolved.</summary>
		RESOLVED,
		/// <summary> The plugin's dynamic link library is successfully loaded and processed by the dynamic linker.</summary>
		LOADED,
		/// <summary> The plugin is running - i.e., the UnigineEditor::Plugin::init() method was executed successfully.</summary>
		RUNNING,
		/// <summary> The plugin is stopped - i.e., the UnigineEditor::Plugin::shutdown() method was called. </summary>
		STOPPED,
		/// <summary> The plugin's dynamic link library is successfully unloaded from memory.</summary>
		UNLOADED
	};

	PluginInfo();
	~PluginInfo();

	/// <summary> Returns the name of the UnigineEditor plugin.</summary>
	const char *name() const;
	/// <summary> Returns the version of the UnigineEditor plugin.</summary>
	const char *version() const;
	/// <summary> Returns the vendor of the UnigineEditor plugin.</summary>
	const char *vendor() const;
	/// <summary> Returns the description of the UnigineEditor plugin.</summary>
	const char *description() const;
	/// <summary> Returns version of the plugin considered as compatible.</summary>
	const char *compatibleVersion() const;
	/// <summary> Returns the list of dependencies for the plugin.</summary>
	Unigine::Vector<PluginDependency> dependencies() const;

	/// <summary> Returns an absolute filepath to the corresponding plugin's binary file.</summary>
	const char *absoluteFilePath() const;

	/// <summary> Returns true if a plugin error has occurred.
	/// If the result is positive, you can use the errorString() method to get detailed error information.
	/// </summary>
	bool hasError() const;
	/// <summary> Returns a string describing a plugin error.</summary>
	const char *errorString() const;

	/// <summary> Returns the current plugin state.</summary>
	/// <returns> One of the PluginInfo::State enum values.</returns>
	State state() const;

	/// <summary> Returns true if the plugin can be used to resolve a dependency of the given name and version.</summary>
	/// <param name="name"> Plugin name.</param>
	/// <param name="version"> Plugin version.</param>
	bool isInstanceOf(const char *name, const char *version) const;

	/// <summary> Returns the Plugin interface.</summary>
	Plugin *plugin() const;

private:
	::UnigineEditor::PluginInfoPrivate *const d;

	friend class ::UnigineEditor::PluginManagerPrivate;
};

} // namespace UnigineEditor

Q_DECLARE_METATYPE(::UnigineEditor::PluginInfo *);

namespace Unigine
{

template<typename Type>
struct Hasher;

template<>
struct Hasher<::UnigineEditor::PluginDependency>
{
	using HashType = unsigned int;
	UNIGINE_INLINE static HashType create(const ::UnigineEditor::PluginDependency &v)
	{
		return ::Unigine::Hasher<::Unigine::String>::create(v.name);
	}
};

} // namespace Unigine
