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


namespace UnigineEditor
{
namespace Constants
{

////////////////////////////////////////////////////////////////////////////////
// The main QMenuBar menu's keys.
////////////////////////////////////////////////////////////////////////////////
/// <summary> File menu category of the main menu bar.</summary>
extern UNIGINE_EDITOR_API const char MM_FILE[];
/// <summary> Edit menu category of the main menu bar.</summary>
extern UNIGINE_EDITOR_API const char MM_EDIT[];
/// <summary> Create menu category of the main menu bar.</summary>
extern UNIGINE_EDITOR_API const char MM_CREATE[];
/// <summary> Tools menu category of the main menu bar.</summary>
extern UNIGINE_EDITOR_API const char MM_TOOLS[];
/// <summary> Windows menu category of the main menu bar.</summary>
extern UNIGINE_EDITOR_API const char MM_WINDOWS[];
/// <summary> Rendering menu category of the main menu bar.</summary>
extern UNIGINE_EDITOR_API const char MM_RENDERING[];
/// <summary> Help menu category of the main menu bar.</summary>
extern UNIGINE_EDITOR_API const char MM_HELP[];


////////////////////////////////////////////////////////////////////////////////
// The main QToolBar toolbar's keys.
////////////////////////////////////////////////////////////////////////////////
/// <summary> Tools toolbar.</summary>
extern UNIGINE_EDITOR_API const char MT_TOOLS[];


/// <summary> Type of selectable subobjects for various types of nodes.</summary>
enum class SubObjectType
{
	/// <summary> Object's surface (Object).</summary>
	SURFACE,
	/// <summary> Collision shape (Body attached to an Object).</summary>
	SHAPE,
	/// <summary> Physical joint (Body attached to an Object).</summary>
	JOINT,
	/// <summary> Billboard (ObjectBillboards).</summary>
	BILLBOARD,
	/// <summary> Physical force affecting particles (ObjectParticles).</summary>
	FORCE,
	/// <summary> Physical noise affecting particles (ObjectParticles).</summary>
	NOISE,
	/// <summary> Physical deflector affecting particles (ObjectParticles).</summary>
	DEFLECTOR,
	/// <summary> Spline point (WorldSplineGraph).</summary>
	SPLINE_POINT,
	/// <summary> Spline segment (WorldSplineGraph).</summary>
	SPLINE_SEGMENT,
	/// <summary> Spline tangent (WorldSplineGraph).</summary>
	SPLINE_TANGENT,
};

} // namespace Constants
} // namespace UnigineEditor

namespace Unigine
{

template<typename Type>
struct Hasher;

template<>
struct Hasher<::UnigineEditor::Constants::SubObjectType>
{
	using HashType = unsigned int;
	UNIGINE_INLINE static HashType create(::UnigineEditor::Constants::SubObjectType v)
	{
		return static_cast<HashType>(v);
	}
};

} // namespace Unigine
