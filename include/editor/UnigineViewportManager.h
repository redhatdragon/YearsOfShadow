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

#include <UniginePlayers.h>
#include <UnigineGui.h>
#include <UnigineViewport.h>
#include <UnigineVector.h>
#include <UnigineNode.h>
#include <UnigineObjects.h>


namespace UnigineEditor
{

struct ViewportWindow;
using ViewportWindowId = ViewportWindow *;
////////////////////////////////////////////////////////////////////////////////
// ViewportManager.
////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Viewport manager class. This class is used to create, manage, and remove all viewports in the Editor.
/// </summary>
class UNIGINE_EDITOR_API ViewportManager final
{
public:
	/// <summary> Creates a new viewport window with the specified name.</summary>
	/// <param name="name">Title of the viewport window.</param>
	/// <returns> ID of the new created viewport.</returns>
	static ViewportWindowId createViewportWindow(const char *name = nullptr);
	/// <summary> Removes the viewport window with the specified ID.</summary>
	/// <param name="viewport_id">ID of the viewport window to be removed.</param>
	/// <returns> <b>true</b> if the viewport window with the specified ID was successfully removed; otherwise, <b>false</b>.</returns>
	static bool removeViewportWindow(ViewportWindowId viewport_id);
	/// <summary> Shows the viewport window with the specified ID.</summary>
	/// <param name="viewport_id">ID of the viewport window to be shown.</param>
	/// <returns> <b>true</b> if the viewport window with the specified ID was successfully shown; otherwise, <b>false</b>.</returns>
	static bool showViewportWindow(ViewportWindowId viewport_id);
	/// <summary> Hides the viewport window with the specified ID.</summary>
	/// <param name="viewport_id">ID of the viewport window to be hidden.</param>
	/// <returns> <b>true</b> if the viewport window with the specified ID was successfully hidden; otherwise, <b>false</b>.</returns>
	static bool hideViewportWindow(ViewportWindowId viewport_id);

	/// <summary> Returns the total number of existing viewport windows.</summary>
	static int getNumViewportWindows();
	/// <summary> Returns the ID of the viewport window by its index if it exists; otherwise, <b>nullptr</b>.</summary>
	/// <param name="index">Index of the viewport window in the list, from <b>0</b> to the total number of viewport windows (see the <see cref="getNumViewportWindows"/>() method).</param>
	static ViewportWindowId getViewportWindowId(int index);

	/// <summary> Returns the ID of the last viewport window hovered by the mouse cursor. Can be used for implementation of Drag&amp;Drop operations</summary>
	static ViewportWindowId getLastHoveredViewportWindow();
	/// <summary> Returns the ID of a viewport window that is currently active.</summary>
	static ViewportWindowId getActiveViewportWindow();

	/// <summary> Returns the name of the viewport window with the specified ID if it exists; otherwise, <b>nullptr</b>.</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	static const char *getViewportWindowName(ViewportWindowId viewport_id);
	/// <summary> Returns the ID of the viewport window with the specified name if it exists; otherwise <b>nullptr</b>.</summary>
	/// <param name="name">Name of the viewport window.</param>
	static ViewportWindowId getViewportWindowId(const char *name);

	/// <summary> Returns the main player (camera) whose current view is rendered to the viewport window with the specified ID if it exists; otherwise, <b>nullptr</b>.</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	static Unigine::PlayerPtr getViewportWindowPlayer(ViewportWindowId viewport_id);
	/// <summary> Sets a new main player (camera) whose current view is to be rendered to the viewport window with the specified ID.</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	/// <param name="player">Player to be used as the main camera for the viewport window with the specified ID.</param>
	static void setViewportWindowPlayer(ViewportWindowId viewport_id, const Unigine::PlayerPtr &player);

	/// <summary> Returns the Engine Viewport instance for the viewport window with the specified ID.</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	static Unigine::ViewportPtr getViewportWindowViewport(ViewportWindowId viewport_id);
	/// <summary> Returns the Gui instance for the viewport window with the specified ID. You can use it to add and manage your UI elements.</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	static Unigine::GuiPtr getViewportWindowGui(ViewportWindowId viewport_id);

	/// <summary> Returns the X coordinate of the mouse position relative to the viewport window with the specified ID. While the mouse cursor is within the window the coordinate is in the [0, getViewportWindowWidth() - 1] range, otherwise the value can be negative or greater than window width (depending on the mouse cursor position relative to the window).</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	static int getViewportWindowMouseX(ViewportWindowId viewport_id);
	/// <summary> Returns the Y coordinate of the mouse position relative to the viewport window with the specified ID. While the mouse cursor is within the window the coordinate is in the [0, getViewportWindowHeight() - 1] range, otherwise the value can be negative or greater than window height (depending on the mouse cursor position relative to the window).</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	static int getViewportWindowMouseY(ViewportWindowId viewport_id);
	/// <summary> Returns the coordinates of the mouse position relative to the viewport window with the specified ID. Coordinates are returned as a two-component vector (X, Y). While the mouse cursor is within the window the coordinates are within the following ranges: X - [0, getViewportWindowWidth() - 1]; Y - [0, getViewportWindowHeight() - 1]. Otherwise the values can be negative or greater than window height and width (depending on the mouse cursor position relative to the window).</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	static Unigine::Math::ivec2 getViewportWindowMousePos(ViewportWindowId viewport_id);

	/// <summary> Returns the current width (in pixels) of the viewport window with the specified ID.</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	static int getViewportWindowWidth(ViewportWindowId viewport_id);
	/// <summary> Returns the current height (in pixels) of the viewport window with the specified ID.</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	static int getViewportWindowHeight(ViewportWindowId viewport_id);
	/// <summary> Returns the current size of the viewport window with the specified ID. The value is returned as a two-component vector (Width, Height).</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	static Unigine::Math::ivec2 getViewportWindowSize(ViewportWindowId viewport_id);

	/// <summary> Sets a value indicating if rectangular selection for viewport windows is enabled.</summary>
	/// <param name="enable"> <b>true</b> to enable rectangular selection for viewport windows; <b>false</b> - to disable it.</param>
	static void setEnabledRectangleSelection(bool enable);
	/// <summary> Returns a value indicating if rectangular selection for viewport windows is enabled.</summary>
	/// <returns> <b>true</b> if rectangular selection for viewport windows is enabled; otherwise, <b>false</b>.</returns>
	static bool isEnabledRectangleSelection();

	/// <summary> Sets the camera focus for the viewport window with the specified ID on the specified node.</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	/// <param name="node">Node to be focused on.</param>
	static void focusOnNode(ViewportWindowId viewport_id, const Unigine::NodePtr &node);
	/// <summary> Sets the camera focus for the viewport window with the specified ID on the specified nodes (the viewport will be adjusted to display all the specified nodes).</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	/// <param name="nodes">Vector containing nodes to be focused on.</param>
	static void focusOnNodeList(ViewportWindowId viewport_id,
		const Unigine::Vector<Unigine::NodePtr> &nodes);
	/// <summary> Sets the camera focus for the viewport window with the specified ID on the specified surface of the specified object (the viewport will be adjusted to display the specified surface).</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	/// <param name="object">Object to be focused on.</param>
	/// <param name="surface">The number of the surface of the specified object to be focused on.</param>
	static void focusOnSurface(ViewportWindowId viewport_id, const Unigine::ObjectPtr &object,
		int surface);
	/// <summary> Sets the camera focus for the viewport window with the specified ID on the specified surfaces of the specified object (the viewport will be adjusted to display all the specified surfaces).</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	/// <param name="object">Object to be focused on.</param>
	/// <param name="surfaces">Vector containing the numbers of surfaces of the specified object to be focused on.</param>
	static void focusOnSurfaceList(ViewportWindowId viewport_id, const Unigine::ObjectPtr &object,
		const Unigine::Vector<int> &surfaces);
	/// <summary> Sets the camera focus for the viewport window with the specified ID on an imaginary sphere with the specified center point and radius.</summary>
	/// <param name="viewport_id">ID of the viewport window.</param>
	/// <param name="center">Coordinates of the center of an imaginary sphere to be focused on.</param>
	/// <param name="radius">Radius of an imaginary sphere to be focused on.</param>
	static void focusOnCenter(ViewportWindowId viewport_id, const Unigine::Math::Vec3 &center,
		float radius);

	/// <summary> Renders an effect of wireframe warming, a temporary wireframe fading in and out for the current selection (such effect can be seen when selecting a node or a group of nodes in the Editor).</summary>
	static void forceWireframeWarming();

	/// <summary> Activates placement mode for the specified node. The node is to be placed at the location within the viewport where the left mouse button is clicked. If the Esc key is pressed before clicking within the viewport the current node placement operation will be cancelled. This behavior can be observed in the Editor, when the user creates a node via the <b>Create</b> menu. This function will automatically create all necessary Undo Actions (for adding nodes to the world and for selecting them).</summary>
	/// <param name="node">The node to be placed at the point within the viewport clicked by the user.</param>
	static void placeNode(const Unigine::NodePtr &node);
	/// <summary> Activates placement mode for the specified nodes. The nodes are to be placed at the location within the viewport where the left mouse button is clicked. If the Esc key is pressed before clicking within the viewport the current node placement operation will be cancelled. This behavior can be observed in the Editor, when the user creates a node via the <b>Create</b> menu. This function will automatically create all necessary Undo Actions (for adding nodes to the world and for selecting them).</summary>
	/// <param name="nodes">Vector containing nodes to be placed at the point within the viewport clicked by the user.</param>
	static void placeNodeList(const Unigine::Vector<Unigine::NodePtr> &nodes);
};

} // namespace UnigineEditor
