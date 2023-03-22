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


#include <editor/UnigineUndo.h>

#include <UnigineNode.h>
#include <UnigineVector.h>


////////////////////////////////////////////////////////////////////////////////
// Forward decl.
////////////////////////////////////////////////////////////////////////////////
namespace UnigineEditor
{
class Selector;
class SelectionActionPrivate;
class CreateNodesActionPrivate;
class RemoveNodesActionPrivate;
class SetNodeTransformActionPrivate;
class ReparentNodesActionPrivate;
class RenameNodeActionPrivate;
class EnableNodeActionPrivate;
}


namespace UnigineEditor
{

/// <summary>This class is used to represent any user's selection action.</summary>
class UNIGINE_EDITOR_API SelectionAction : public Action
{
public:
	/// <summary>Creates a new selection action for the specified selector.</summary>
	/// <param name="s">Selector defining a new selection.</param>
	explicit SelectionAction(Selector *s);
	~SelectionAction() override;

	/// <summary>Applies the selection action.</summary>
	void apply() override;
	/// <summary>Reverts the selection action.</summary>
	void undo() override;
	/// <summary>Redoes the previously undone selection action (reverses the undo method).</summary>
	void redo() override { apply(); }

	/// <summary>
	/// Indicates whether the action brings any changes to the scene.
	/// Selection actions change nothing and
	/// shouldn't set world`s "dirty state", so 'false' is returned.
	/// </summary>
	bool modifiesWorld() const override { return false; }
	/// <summary>
	/// This method allows action to do some internal cleanup.
	/// Return value indicates whether the action is still sane.
	/// Whenever it returns false undo system will delete invalid action
	/// for good.
	/// </summary>
	bool validate() override;

	/// <summary>Applies a new selection defined by the specified selector.</summary>
	/// <param name="selector">New selection to be applied.</param>
	static void applySelection(Selector *selector);
	/// <summary>Refreshes the current selection.</summary>
	/// <param name="expand">Set <b>true</b> to expand the hierarchy after refreshing selection; <b>false</b>.</param>
	static void refreshSelection(bool expand = false);

public:
	::UnigineEditor::SelectionActionPrivate *const d;
};

/// <summary>This class is used to represent any user's action creating nodes.</summary>
class UNIGINE_EDITOR_API CreateNodesAction : public Action
{
public:
	/// <summary>Creates a new create action for the specified node.</summary>
	/// <param name="node">Node to be created.</param>
	explicit CreateNodesAction(const Unigine::NodePtr &node);
	/// <summary>Creates a new create action for the specified node.</summary>
	/// <param name="nodes">Nodes to be created.</param>
	explicit CreateNodesAction(const Unigine::Vector<Unigine::NodePtr> &nodes);
	~CreateNodesAction() override;

	/// <summary>Applies the create nodes action.</summary>
	void apply() override;
	/// <summary>Reverts the create nodes action.</summary>
	void undo() override;
	/// <summary>Redoes the previously undone create nodes action (reverses the undo method).</summary>
	void redo() override;

	Unigine::Vector<Unigine::NodePtr> getNodes() const;

private:
	::UnigineEditor::CreateNodesActionPrivate *const d;
};

/// <summary>This class is used to represent any user's action removing nodes.</summary>
class UNIGINE_EDITOR_API RemoveNodesAction : public Action
{
public:
	/// <summary>Creates a new remove action for the specified node.</summary>
	/// <param name="node">Node to be removed.</param>
	explicit RemoveNodesAction(const Unigine::NodePtr &node);
	/// <summary>Creates a new remove action for the specified node.</summary>
	/// <param name="nodes">Nodes to be removed.</param>
	explicit RemoveNodesAction(const Unigine::Vector<Unigine::NodePtr> &nodes);
	~RemoveNodesAction() override;

	/// <summary>Applies the remove nodes action.</summary>
	void apply() override;
	/// <summary>Reverts the remove nodes action.</summary>
	void undo() override;
	/// <summary>Redoes the previously undone remove nodes action (reverses the undo method).</summary>
	void redo() override { apply(); }

	/// <summary>
	/// This method allows action to do some internal cleanup.
	/// Return value indicates whether the action is still sane.
	/// Whenever it returns false undo system will delete invalid action
	/// for good.
	/// </summary>
	bool validate() override;

	Unigine::Vector<Unigine::NodePtr> getNodes() const;

private:
	::UnigineEditor::RemoveNodesActionPrivate *const d;
};

/// <summary>This class is used to represent any user's action setting node transforms.</summary>
class UNIGINE_EDITOR_API SetNodeTransformAction : public Action
{
public:
	/// <summary>Creates a new set node transform action for the specified node.</summary>
	/// <param name="node">Nodes affected by the set node transform action.</param>
	/// <param name="transform">New transformation to be set for the specified node.</param>
	explicit SetNodeTransformAction(const Unigine::NodePtr &node,
		const Unigine::Math::Mat4 &transform);
	~SetNodeTransformAction() override;

	/// <summary>Applies the set node transform action.</summary>
	void apply() override;
	/// <summary>Reverts the set node transform action.</summary>
	void undo() override;
	/// <summary>Redoes the previously undone set node transform action (reverses the undo method).</summary>
	void redo() override;
	/// <summary>
	/// This method allows action to do some internal cleanup.
	/// Return value indicates whether the action is still sane.
	/// Whenever it returns false undo system will delete invalid action
	/// for good.
	/// </summary>
	bool validate() override;

	Unigine::NodePtr getNode() const;

private:
	::UnigineEditor::SetNodeTransformActionPrivate *const d;
};

/// <summary>This class is used to represent any user's action changing nodes parents.</summary>
class UNIGINE_EDITOR_API ReparentNodesAction final : public Action
{
public:
	/// <summary>Creates a new reparent action for the specified nodes.</summary>
	/// <param name="nodes">Nodes affected by the reparent action.</param>
	/// <param name="new_parent">New parent to be set for the specified nodes.</param>
	/// <param name="new_index">Index of the new parent node's child after which the specified nodes are to be added. The default <b>-1</b> value adds nodes after the last child.</param>
	explicit ReparentNodesAction(const Unigine::Vector<Unigine::NodePtr> &nodes,
		const Unigine::NodePtr &new_parent,
		int new_index = -1);
	~ReparentNodesAction() override;

	/// <summary>Applies the reparent nodes action.</summary>
	void apply() override;
	/// <summary>Reverts the reparent nodes action.</summary>
	void undo() override;
	/// <summary>Redoes the previously undone reparent nodes action (reverses the undo method).</summary>
	void redo() override;

private:
	::UnigineEditor::ReparentNodesActionPrivate *const d;
};

/// <summary>This class is used to represent any user's action changing node's name.</summary>
class UNIGINE_EDITOR_API RenameNodeAction : public Action
{
public:
	/// <summary>Creates a new rename action for the specified node.</summary>
	/// <param name="node">Node affected by the rename action.</param>
	/// <param name="new_name">New name of the node.</param>
	explicit RenameNodeAction(const Unigine::NodePtr &node, const char *new_name);
	~RenameNodeAction() override;

	/// <summary>Applies the rename node action.</summary>
	void apply() override;
	/// <summary>Reverts the rename node action.</summary>
	void undo() override;
	/// <summary>Redoes the previously undone rename node action (reverses the undo method).</summary>
	void redo() override;
	/// <summary>
	/// This method allows action to do some internal cleanup.
	/// Return value indicates whether the action is still sane.
	/// Whenever it returns false undo system will delete invalid action
	/// for good.
	/// </summary>
	bool validate() override;

private:
	::UnigineEditor::RenameNodeActionPrivate *const d;
};

/// <summary>This class is used to represent any user's action changing node's enabled state.</summary>
class UNIGINE_EDITOR_API EnableNodeAction : public Action
{
public:
	/// <summary>Creates a new enable node action for the specified node.</summary>
	/// <param name="node">Node affected by the action.</param>
	/// <param name="enabled"><b>true</b> if the action enables the specified node; otherwise, <b>false</b>.</param>
	explicit EnableNodeAction(const Unigine::NodePtr &node, bool enabled);
	/// <summary>Creates a new enable action for the specified nodes.</summary>
	/// <param name="nodes">Nodes affected by the action.</param>
	/// <param name="enabled"><b>true</b> if the action enables the specified nodes; otherwise, <b>false</b>.</param>
	EnableNodeAction(const Unigine::Vector<Unigine::NodePtr> &nodes, bool enabled);
	~EnableNodeAction() override;

	/// <summary>Applies the enable node action.</summary>
	void apply() override;
	/// <summary>Reverts the enable node action.</summary>
	void undo() override;
	/// <summary>Redoes the previously undone enable node action (reverses the undo method).</summary>
	void redo() override;
	/// <summary>
	/// This method allows action to do some internal cleanup.
	/// Return value indicates whether the action is still sane.
	/// Whenever it returns false undo system will delete invalid action
	/// for good.
	/// </summary>
	bool validate() override;

private:
	::UnigineEditor::EnableNodeActionPrivate *const d;
};

} // namespace UnigineEditor
