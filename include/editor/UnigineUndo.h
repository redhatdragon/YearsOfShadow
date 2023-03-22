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

#include <QObject>


////////////////////////////////////////////////////////////////////////////////
// Forward decl.
////////////////////////////////////////////////////////////////////////////////
namespace UnigineEditor
{
class Presenter;
class Action;
}


namespace UnigineEditor
{
/// <summary>
/// Undo manager class. This class is used to manage undo/redo operations for user actions in the Editor.
/// </summary>
class UNIGINE_EDITOR_API Undo : public QObject
{
	Q_OBJECT
public:
	/// <summary> Returns the instance of the Undo class. This method is used to subscribe to signals.</summary>
	/// <returns> Undo class instance.</returns>
	/// <example> <b>Example:</b>
	/// <code>
	/// connect(UnigineEditor::Undo::instance(), &UnigineEditor::Undo::worldModified
	///   , this, &SomeClass::world_modified);
	/// </code>
	/// </example>
	static Undo *instance();

	/// <summary>Applies the specified action.</summary>
	/// <param name="action">Action to be applied.</param>
	static void apply(Action *action);
	/// <summary>Pushes the specified action to the transaction stack.</summary>
	/// <param name="action">Action to be pushed.</param>
	static void push(Action *action);
	/// <summary>Reverts the last action in the stack.</summary>
	static void undo();
	/// <summary>Redoes the last action that was undone (reverses the undo method).</summary>
	static void redo();

	/// <summary>Clears the undo/redo stack.</summary>
	static void reset();
	/// <summary>
	/// Begins a transaction (i.e. a sequence of actions applied to be applied 
	/// or reverted as a whole). Use <see cref="commit"/>() to apply all actions
	/// of the current transaction, or <see cref="rollback"/>() to revert them.
	/// </summary>
	static void begin();
	/// <summary> 
	/// Commits the last transaction (i.e. a sequence of actions
	/// applied since the last call of the <see cref="begin"/>() method).
	/// </summary>
	static void commit();
	/// <summary> 
	/// Reverts the last transaction (i.e. a sequence of actions applied 
	/// since the last call of the <see cref="begin"/>() method).
	/// </summary>
	static void rollback();

signals:
	/// <summary> This signal is emitted when an action brings any changes to the scene.</summary>
	void worldModified();
	/// <summary>This signal is emitted when an action has been applied, undone, redone, or pushed to transaction stack.</summary>
	void activity();

private:
	explicit Undo(QObject *parent);
	~Undo() override;

	friend class ::UnigineEditor::Presenter;
};

/// <summary>This basic class is used to represent any possible user action.</summary>
class UNIGINE_EDITOR_API Action
{
public:
	Action();
	virtual ~Action();
	/// <summary>Applies the action.</summary>
	virtual void apply() = 0;
	/// <summary>Reverts the action.</summary>
	virtual void undo()  = 0;
	/// <summary>Redoes the previously undone action (reverses the undo method).</summary>
	virtual void redo()  = 0;

	/// <summary>
	/// This method allows action to do some internal cleanup.
	/// Return value indicates whether the action is still sane.
	/// Whenever it returns false asset system will delete invalid action
	/// for good.
	/// </summary>
	virtual bool validate() { return true; }
	/// <summary>
	/// Indicates whether the action brings any changes to the scene.
	/// Actions like selection, for example, change nothing and
	/// shouldn't set world`s "dirty state".
	/// For such actions this method would return 'false'.
	/// </summary>
	virtual bool modifiesWorld() const { return true; }
};

} // namespace UnigineEditor
