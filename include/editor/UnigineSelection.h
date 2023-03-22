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
class Selector;
class SelectorNodes;
class SelectorGUIDs;
}


namespace UnigineEditor
{
////////////////////////////////////////////////////////////////////////////////
// Selection.
////////////////////////////////////////////////////////////////////////////////
class UNIGINE_EDITOR_API Selection: public QObject
{
	Q_OBJECT
public:
	/// <summary> Returns the instance of the Selection class. This method is used to subscribe to the <see cref="changed"/> signal.</summary>
	/// <returns> Selection class instance.</returns>
	/// <example> <b>Example:</b>
	/// <code>
	/// connect(Selection::instance(), &Selection::changed
	///   , this, &ParametersWindow::selection_changed);
	/// </code>
	/// </example>
	static Selection *instance();

	/// <summary> Clears the current selection. If current selection was not empty the <see cref="changed"/> signal is emitted.</summary>
	static void clear();
	/// <summary> Returns the current selection.</summary>
	static Selector *getSelector();
	/// <summary> Sets the specified selection as the current one. If current selection has changed the <see cref="changed"/> signal is emitted.</summary>
	static void setSelector(Selector *selector);
	/// <summary> Copies the current selection. Current selection obtained via the <see cref="getSelector"/>() method is not managed by the user, so it can change at any time. You can use this method to store a copy of the current selection in order to perform undo/redo operations.</summary>
	static Selector *copySelector();

	/// <summary> Tries to filter out nodes from the current selection as a nodes selector (<see cref="SelectorType"/>). The result shall combine only selected nodes (if any).</summary>
	/// <returns> Selector of the nodes type for currently selected nodes, or <b>nullptr</b> if there are no nodes currently selected.</returns>
	static SelectorNodes *getSelectorNodes();
	/// <summary> Tries to filter out runtimes from the current selection as a runtimes selector (<see cref="SelectorType"/>). The result shall combine only selected runtimes (if any).</summary>
	/// <returns> Selector of the runtimes type for currently selected runtimes, or <b>nullptr</b> if there are no runtimes currently selected.</returns>
	static SelectorGUIDs *getSelectorRuntimes();
	/// <summary> Tries to filter out materials from the current selection as a materials selector (<see cref="SelectorType"/>). The result shall combine only selected materials (if any).</summary>
	/// <returns> Selector of the materials type for currently selected materials, or <b>nullptr</b> if there are no materials currently selected.</returns>
	static SelectorGUIDs *getSelectorMaterials();
	/// <summary> Tries to filter out properties from the current selection as a properties selector (<see cref="SelectorType"/>). The result shall combine only selected properties (if any).</summary>
	/// <returns> Selector of the properties type for currently selected properties, or <b>nullptr</b> if there are no properties currently selected.</returns>
	static SelectorGUIDs *getSelectorProperties();

	/// <summary> Copies all currently selected nodes to a separate list.</summary>
	/// <returns> Pointer to the Selector containing copies of currently selected nodes.</returns>
	static SelectorNodes *copySelectorNodes();
	/// <summary> Copies all runtimes for all currently selected nodes to a separate list.</summary>
	/// <returns> Pointer to the Selector containing copies of all runtimes for all currently selected nodes.</returns>
	static SelectorGUIDs *copySelectorRuntimes();
	/// <summary> Copies all currently selected materials to a separate list.</summary>
	/// <returns> Pointer to the Selector containing copies of currently selected materials.</returns>
	static SelectorGUIDs *copySelectorMaterials();
	/// <summary> Copies all currently selected properties to a separate list.</summary>
	/// <returns> Pointer to the Selector containing copies of currently selected properties.</returns>
	static SelectorGUIDs *copySelectorProperties();

signals:
	/// <summary> This signal is emitted when the current selection has changed.</summary>
	void changed();

private:
	explicit Selection(QObject *parent);
	~Selection() override;

	friend class ::UnigineEditor::Presenter;
};

} // namespace UnigineEditor
