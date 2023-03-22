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


#include <editor/UnigineRenderWindow.h>

#include <UnigineGui.h>


////////////////////////////////////////////////////////////////////////////////
// Forward decl.
////////////////////////////////////////////////////////////////////////////////
namespace UnigineEditor { struct EngineGuiWindowPrivate; }


namespace UnigineEditor
{
////////////////////////////////////////////////////////////////////////////////
// EngineGuiWindow.
////////////////////////////////////////////////////////////////////////////////
/// <summary>This class is used to represent a window widget to which an image
/// can be rendered by the Engine with inputs handling by the Engine GUI. For
/// example, to create a curve or a node graph editor of your own. By default
/// EngineGuiWindow creates a Controls exclusive context to process mouse and
/// keyboard events for this window exclusively, without sending them to the
/// common Editor's Controls handling system.
/// </summary>
class UNIGINE_EDITOR_API EngineGuiWindow : public RenderWindow
{
	Q_OBJECT
	Q_DISABLE_COPY(EngineGuiWindow)

public:
	explicit EngineGuiWindow(QWidget *parent = nullptr);
	~EngineGuiWindow() override;

	/// <summary>Returns the current Engine GUI for the window. You can use it to manage UI and user inputs.</summary>
	Unigine::GuiPtr getGui() const;

	/// <summary>Returns the ID of the Controls exclusive context for the Engine
	/// GUI window. By default EngineGuiWindow creates a Controls exclusive context
	/// to process mouse and keyboard events for this window exclusively, without
	/// sending them to the common Editor's Controls handling system.</summary>
	const char *getControlsExclusiveContextId() const;
	/// <summary>Creates a new Controls exclusive context for the Engine GUI window.
	/// This context is used to process mouse and keyboard events for this window
	/// exclusively, without sending them to the common Editor's Controls handling
	/// system.
	/// </summary>
	/// <returns> <b>true</b> if the context is created successfully; otherwise, <b>false</b>.</returns>
	bool createControlsExclusiveContext(const char *context_id);
	/// <summary>Removes the Controls exclusive context from the window. This
	/// context is used to process mouse and keyboard events for this window
	/// exclusively, without sending them to the common Editor's Controls
	/// handling system.
	/// </summary>
	/// <returns> <b>true</b> if the context is removed successfully; otherwise, <b>false</b>.</returns>
	bool removeControlsExclusiveContext();
	/// <summary>Returns a value indicating if the window has its own Controls
	/// exclusive context. This context is used to process mouse and keyboard
	/// events for this window exclusively, without sending them to the common
	/// Editor's Controls handling system.
	/// </summary>
	/// <returns> <b>true</b> if the window has its own Controls exclusive context; otherwise, <b>false</b>.</returns>
	bool hasControlsExclusiveContext() const;

protected:
	/// <summary>You can inherit your class from EngineGuiWindow and override
	/// this method to define your custom actions
	/// to be performed on Engine::update().
	/// </summary>
	void onUpdate() override;
	/// <summary>You can inherit your class from EngineGuiWindow and override
	/// this method to define your custom actions
	/// to be performed on Engine::render().
	/// </summary>
	void onRender() override;

	void focusOutEvent(QFocusEvent *event) override;
	void focusInEvent(QFocusEvent *event) override;

private:
	void update_gui();

	::UnigineEditor::EngineGuiWindowPrivate *const d;
};

} // namespace UnigineEditor
