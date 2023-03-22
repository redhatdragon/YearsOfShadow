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

#include <UnigineCallback.h>

#include <QWidget>


////////////////////////////////////////////////////////////////////////////////
// Forward decl.
////////////////////////////////////////////////////////////////////////////////
namespace UnigineEditor { struct RenderWindowPrivate; }

namespace UnigineEditor
{

////////////////////////////////////////////////////////////////////////////////
// RenderWindow.
////////////////////////////////////////////////////////////////////////////////
/// <summary> This class is used to represent a window widget to which an image
/// can be rendered by the Engine. For example, to render node or texture
/// previews, frame sequences, etc.
/// </summary>
class UNIGINE_EDITOR_API RenderWindow : public QWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(RenderWindow)

public:
	/// <summary> Rendering policy for the window that defines when it should be
	/// rendered depending on its visibility and focus status.
	/// </summary>
	enum RENDERING_POLICY
	{
		/// <summary> The window is rendered only when visible.</summary>
		RENDERING_POLICY_WINDOW_VISIBLE = 0,
		/// <summary> The window is rendered only when it is visible and focused.</summary>
		RENDERING_POLICY_WINDOW_VISIBLE_AND_FOCUSED,
		/// <summary> The window is always rendered regardless of its visibility and focus status.</summary>
		RENDERING_POLICY_ALWAYS,
	};

public:
	explicit RenderWindow(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	~RenderWindow() override;

	struct Callback;
	using CallbackId = Callback *;
	/// <summary> Adds a callback to be fired for the Render Window
	/// on Engine::update(). As an alternative you can inherit your
	/// class from the RenderWindow and override
	/// the <see cref="onUpdate"/>() method.
	/// </summary>
	/// <param name="update_func">Callback function.</param>
	CallbackId addUpdateCallback(Unigine::CallbackBase *update_func);
	/// <summary> Adds a callback to be fired for the Render Window
	/// on Engine::render(). As an alternative you can inherit your
	/// class from the RenderWindow and override
	/// the <see cref="onRender"/>() method.
	/// </summary>
	/// <param name="render_func">Callback function.</param>
	CallbackId addRenderCallback(Unigine::CallbackBase *render_func);
	/// <summary> Adds a callback to be fired for the Render Window
	/// on Engine::swap(). As an alternative you can inherit your
	/// class from the RenderWindow and override
	/// the <see cref="onSwap"/>() method.
	/// </summary>
	/// <param name="swap_func">Callback function.</param>
	CallbackId addSwapCallback(Unigine::CallbackBase *swap_func);
	/// <summary> Adds a callback to be fired when the focus
	/// of the Render Window changes.
	/// </summary>
	/// <param name="focus_changed_func">Callback function.</param>
	CallbackId addFocusChangedCallback(Unigine::CallbackBase *focus_changed_func);
	/// <summary> Adds a callback to be fired when the focus of
	/// the Render Window becomes focused. As an alternative you
	/// can inherit your class from the RenderWindow and override
	/// the <see cref="focusInEvent"/>() method.
	/// </summary>
	/// <param name="focus_in_func">Callback function.</param>
	CallbackId addFocusInChangedCallback(Unigine::CallbackBase *focus_in_func);
	/// <summary> Adds a callback to be fired when the Render Window
	/// loses focus. As an alternative you can inherit your class
	/// from the RenderWindow and override
	/// the <see cref="focusOutEvent"/>() method.</summary>
	/// <param name="focus_out_func">Callback function.</param>
	CallbackId addFocusOutChangedCallback(Unigine::CallbackBase *focus_out_func);
	/// <summary> Adds a callback to be fired when something is
	/// dragged into the Render Window. The callback function
	/// receives a QDragEnterEvent instance. For more details please
	/// refer to Qt documentation. As an alternative you can inherit
	/// your class from the RenderWindow and override
	/// the <see cref="dragEnterEvent"/>() method.
	/// </summary>
	/// <param name="drag_enter_func">Callback function.</param>
	CallbackId addDragEnterCallback(Unigine::CallbackBase1<QDragEnterEvent *> *drag_enter_func);
	/// <summary> Adds a callback to be fired when something is
	/// dragged within the Render Window. The callback function
	/// receives a QDragMoveEvent instance. For more details
	/// please refer to Qt documentation. As an alternative you
	/// can inherit your class from the RenderWindow and override
	/// the <see cref="dragMoveEvent"/>() method.
	/// </summary>
	/// <param name="drag_move_func">Callback function.</param>
	CallbackId addDragMoveCallback(Unigine::CallbackBase1<QDragMoveEvent *> *drag_move_func);
	/// <summary> Adds a callback to be fired when something is
	/// dragged out of the Render Window. The callback function
	/// receives a QDragLeaveEvent instance. For more details
	/// please refer to Qt documentation. As an alternative you
	/// can inherit your class from the RenderWindow and override
	/// the <see cref="dragLeaveEvent"/>() method.
	/// </summary>
	/// <param name="drag_leave_func">Callback function.</param>
	CallbackId addDragLeaveCallback(Unigine::CallbackBase1<QDragLeaveEvent *> *drag_leave_func);
	/// <summary>Adds a callback to be fired when the user drops
	/// something within the Render Window. The callback function
	/// receives a QDropEvent instance, that contains a proposed
	/// action, available from proposedAction(), for the widget
	/// to either accept or ignore. If the action can be handled
	/// by the widget, you should call the acceptProposedAction()
	/// function. Since the proposed action can be a combination
	/// of Qt::DropAction values, it may be useful to either
	/// select one of these values as a default action or ask
	/// the user to select their preferred action.
	/// If the proposed drop action is not suitable, perhaps
	/// because your custom widget does not support that action,
	/// you can replace it with any of the possible drop actions
	/// by calling setDropAction() with your preferred action.
	/// If you set a value that is not present in the bitwise OR
	/// combination of values returned by possibleActions(), the
	/// default copy action will be used. Once a replacement drop
	/// action has been set, call accept() instead of
	/// acceptProposedAction() to complete the drop operation.
	/// For more details please refer to Qt documentation.
	/// As an alternative you can inherit your class from the
	/// RenderWindow and override
	/// the <see cref="dropEvent"/>() method.
	/// </summary>
	/// <param name="drop_func">Callback function.</param>
	CallbackId addDropCallback(Unigine::CallbackBase1<QDropEvent *> *drop_func);
	/// <summary>Removes a callback with the specified ID.</summary>
	/// <param name="callback_id">Callback function ID.</param>
	/// <returns> <b>true</b> if the callback with the specified ID is removed successfully; otherwise, <b>false</b>.</returns>
	bool removeCallback(CallbackId callback_id);
	/// <summary>Removes all callbacks.</summary>
	void clearCallbacks();

	/// <summary>Sets rendering policy for the Render Window.
	/// This policy defines when the window should be rendered
	/// depending on its visibility and focus status.
	/// </summary>
	/// <param name="rendering_policy">New rendering policy to be set.</param>
	void setRenderingPolicy(RENDERING_POLICY rendering_policy);
	/// <summary>Returns the current rendering policy for the
	/// Render Window. This policy defines when the window should be
	/// rendered depending on its visibility and focus status
	/// (See <see cref="RENDERING_POLICY"/>).
	/// </summary>
	RENDERING_POLICY getRenderingPolicy() const;

	QPaintEngine *paintEngine() const final override { return nullptr; }

signals:
	/// <summary> This signal is emitted when the focus has changed
	/// because the user pressed the tab-key, clicked into a widget
	/// or changed the active window.
	/// </summary>
	void focusChanged();

protected:
	/// <summary>You can inherit your class from RenderWindow and
	/// override this method to define your custom actions to be
	/// performed on Engine::update(). As an alternative to
	/// overriding this method you can use
	/// the Update callback (<see cref="addUpdateCallback"/>).
	/// </summary>
	virtual void onUpdate() {}
	/// <summary>You can inherit your class from RenderWindow and
	/// override this method to define your custom actions to be
	/// performed on Engine::render(). As an alternative to
	/// overriding this method you can use
	/// the Render callback (<see cref="addRenderCallback"/>).
	/// </summary>
	virtual void onRender() {}
	/// <summary>You can inherit your class from RenderWindow and
	/// override this method to define your custom actions to be
	/// performed on Engine::swap(). As an alternative to
	/// overriding this method you can use
	/// the Swap callback (<see cref="addSwapCallback"/>).
	/// </summary>
	virtual void onSwap() {}

	/// <summary>You can inherit your class from RenderWindow and
	/// override this method to define your custom actions to be
	/// performed on receiving various events (QEvent). As an
	/// alternative to overriding this method you can use
	/// the Update callback (<see cref="addUpdateCallback"/>).
	/// </summary>
	bool event(QEvent *event) override;
	void paintEvent(QPaintEvent *) final override {}
	/// <summary>You can inherit your class from RenderWindow and
	/// override this method to define actions to be performed on
	/// receiving a resize event (QResizeEvent). Resize events are
	/// sent to widgets that have been resized. For more details
	/// please refer to Qt documentation. As an alternative to
	/// overriding this method you can use
	/// the Update callback (<see cref="addUpdateCallback"/>).
	/// </summary>
	void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
	/// <summary>You can inherit your class from RenderWindow and
	/// override this method to define actions to be performed on
	/// getting focus, i.e., receiving the corresponding focus
	/// event (QFocusEvent). Focus events are sent to widgets
	/// when the keyboard input focus changes. Focus events occur
	/// due to mouse actions, key presses (such as Tab or Backtab),
	/// the window system, popup menus, keyboard shortcuts, or other
	/// application-specific reasons. The reason for a particular
	/// focus event is returned by reason() in the appropriate event
	/// handler. For more details please refer to Qt documentation.
	/// As an alternative to overriding this method you can use
	/// the FocusIn callback (<see cref="addFocusInChangedCallback"/>).
	/// </summary>
	void focusInEvent(QFocusEvent *event) override;
	/// <summary>You can inherit your class from RenderWindow and
	/// override this method to define actions to be performed on
	/// losing focus, i.e., receiving the corresponding
	/// focus event (QFocusEvent). Focus events are sent to widgets
	/// when the keyboard input focus changes. Focus events occur
	/// due to mouse actions, key presses (such as Tab or Backtab),
	/// the window system, popup menus, keyboard shortcuts, or other
	/// application-specific reasons. The reason for a particular
	/// focus event is returned by reason() in the appropriate event
	/// handler. For more details please refer to Qt documentation.
	/// As an alternative to overriding this method you can use
	/// the FocusOut callback (<see cref="addFocusOutChangedCallback"/>).
	/// </summary>
	void focusOutEvent(QFocusEvent *event) override;
	/// <summary>You can inherit your class from RenderWindow and
	/// override this method to define actions to be performed
	/// when a drag-and-drop action enters
	/// the Render Window (QDragEnterEvent). The drag enter event
	/// is always immediately followed by a drag move event.
	/// For more details please refer to Qt documentation. As an
	/// alternative to overriding this method you can use
	/// the <see cref="addDragEnterCallback"/> callback.
	/// </summary>
	void dragEnterEvent(QDragEnterEvent *event) override;
	/// <summary>You can inherit your class from RenderWindow and
	/// override this method to define actions to be performed when
	/// a drag-and-drop action is in progress (QDragMoveEvent).
	/// The drag move event always immediately follows a drag enter
	/// event. For more details please refer to Qt documentation.
	/// As an alternative to overriding this method you can use
	/// the <see cref="addDragMoveCallback"/> callback.
	/// </summary>
	void dragMoveEvent(QDragMoveEvent *event) override;
	/// <summary>You can inherit your class from RenderWindow and
	/// override this method to define actions to be performed when
	/// a drag-and-drop action leaves the Render Window (QDragLeaveEvent).
	/// For more details please refer to Qt documentation. As an
	/// alternative to overriding this method you can use
	/// the <see cref="addDragLeaveCallback"/> callback.
	/// </summary>
	void dragLeaveEvent(QDragLeaveEvent *event) override;
	/// <summary>You can inherit your class from RenderWindow and
	/// override this method to define actions to be performed when
	/// a drag-and-drop action is completed (QDropEvent). For more
	/// details please refer to Qt documentation. As an alternative
	/// to overriding this method you can use
	/// the <see cref="addDropCallback"/> callback.
	/// </summary>
	void dropEvent(QDropEvent *event) override;

protected:
	bool is_rendering() const;

private:
	void do_update();
	void do_render();
	void do_swap();

private:
	friend struct ::UnigineEditor::RenderWindowPrivate;
	::UnigineEditor::RenderWindowPrivate *d;
};

} // namespace UnigineEditor
