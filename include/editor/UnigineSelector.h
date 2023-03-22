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


#include <editor/UnigineConstants.h>
#include <editor/UnigineEditorGlobal.h>

#include <UnigineGUID.h>
#include <UnigineHashSet.h>
#include <UnigineNode.h>
#include <UnigineVector.h>


////////////////////////////////////////////////////////////////////////////////
// Forward decl.
////////////////////////////////////////////////////////////////////////////////
namespace UnigineEditor
{
class IndexList;

namespace Internal {
class SelectorGUIDsPrivate;
class SelectorNodesPrivate;
class IndexListPrivate;
}}


namespace UnigineEditor
{

/// <summary> Selector type, defines the type of selected entities. The first 256 types are reserved by UNIGINE, user selector types start from SELECTOR_USER + i (256 and higher).</summary>
enum SelectorType
{
	/// <summary> Nodes selector.</summary>
	SELECTOR_NODES = 0,
	/// <summary> Materials selector.</summary>
	SELECTOR_MATERIALS,
	/// <summary> Properties selector.</summary>
	SELECTOR_PROPERTIES,
	/// <summary> Runtimes selector.</summary>
	SELECTOR_RUNTIMES,
	/// <summary> Beginning of the range of user-defined types. Types are referred to as SELECTOR_USER+0, SELECTOR_USER+1...</summary>
	SELECTOR_USER = 0x0100
};

////////////////////////////////////////////////////////////////////////////////
// Selector.
////////////////////////////////////////////////////////////////////////////////
class UNIGINE_EDITOR_API Selector
{
public:
	Selector();
	virtual ~Selector();
	/// <summary> Returns the current selector type.</summary>
	/// <returns> Current selector type: one of the <see cref="SelectorType"/> enum values.</returns>
	virtual int type() const = 0;
	/// <summary> Checks whether the current selector is equal to the specified one.</summary>
	/// <returns> <b>true</b> if selectors are equal; otherwise, <b>false</b>.</returns>
	virtual bool equal(const Selector *selector) const = 0;
	/// <summary> Clones the current selector.</summary>
	/// <returns> Clone of the current selector.</returns>
	virtual Selector *clone () const = 0;
};

////////////////////////////////////////////////////////////////////////////////
// SelectorGUIDs.
////////////////////////////////////////////////////////////////////////////////
// TODO(SiN_Bizkit: 04/11/19): Choose a clear name.
/// <summary> Selector for GUIDs-based items (materials, properties, runtimes). An item of this type is associated with a file referred to via a GUID.</summary>
class UNIGINE_EDITOR_API SelectorGUIDs : public Selector
{
public:
	/// <summary> Creates a runtimes selector using the specified list of GUIDs.</summary>
	/// <param name="guids"> The list of GUIDs of runtimes.</param>
	/// <returns> SelectorGUIDs containing runtimes with the specified GUIDs.</returns>
	static SelectorGUIDs *createRuntimesSelector  (Unigine::Vector<Unigine::UGUID> guids);
	/// <summary> Creates a materials selector using the specified list of GUIDs.</summary>
	/// <param name="guids"> The list of GUIDs of materials.</param>
	/// <returns> SelectorGUIDs containing materials with the specified GUIDs.</returns>
	static SelectorGUIDs *createMaterialsSelector (Unigine::Vector<Unigine::UGUID> guids);
	/// <summary> Creates a properties selector using the specified list of GUIDs.</summary>
	/// <param name="guids"> The list of GUIDs of properties.</param>
	/// <returns> SelectorGUIDs containing properties with the specified GUIDs.</returns>
	static SelectorGUIDs *createPropertiesSelector(Unigine::Vector<Unigine::UGUID> guids);

	/// <summary> Creates a SelectorGUIDs of the specified type (materials, properties, runtimes) using the specified list of GUIDs.</summary>
	/// <param name="type"> The type of the selector to be created: one of the <see cref="SelectorType"/> enum values.</param>
	/// <param name="guids"> The list of GUIDs of items to be added to the new selector.</param>
	/// <returns> Selector of the specified type containing items with the specified GUIDs.</returns>
	SelectorGUIDs(int type, Unigine::Vector<Unigine::UGUID> guids);
	~SelectorGUIDs() override;

	/// <summary> Returns the current selector type.</summary>
	/// <returns> Current selector type: one of the <see cref="SelectorType"/> enum values.</returns>
	int type() const override;
	/// <summary> Checks whether the selector is equal to the specified target selector.</summary>
	/// <param name="selector"> Target selector.</param>
	/// <returns> <b>true</b> if selectors are equal; otherwise, <b>false</b>.</returns>
	bool equal(const Selector *selector) const override;
	/// <summary> Clones the selector.</summary>
	/// <returns> Clone of the current selector.</returns>
	SelectorGUIDs *clone() const override;

	/// <summary> Returns the list of GUIDs for all selected items.</summary>
	/// <returns> Vector containing GUIDs for all selected items.</returns>
	Unigine::Vector<Unigine::UGUID> guids() const;

	/// <summary> Checks whether the SelectorGUIDs contains the specified GUID.</summary>
	/// <returns> <b>true</b> if the current selection contains the specified GUID; otherwise, <b>false</b>.</returns>
	bool contains(const Unigine::UGUID &guid) const;

	/// <summary> Checks whether the SelectorGUIDs is empty (no GUID-s are currently selected).</summary>
	/// <returns> <b>true</b> if the selector is empty; otherwise, <b>false</b>.</returns>
	bool empty() const;

private:
	::UnigineEditor::Internal::SelectorGUIDsPrivate *d;
};

////////////////////////////////////////////////////////////////////////////////
// SelectorNodes.
////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// This class is used to manage selected nodes along with their subobjects (such as surfaces, shapes, joints, billboards, etc.).
/// </summary>
class UNIGINE_EDITOR_API SelectorNodes : public Selector
{
public:
	/// <summary> Creates a new selection (<see cref="SelectorNodes"/>) combining all objects from the specified list of nodes. All surfaces of these objects shall also be included in the current selection.</summary>
	/// <returns> Selector of the nodes type including all objects from the specified list of nodes, or <b>nullptr</b> if there are no objects in the specified list.</returns>
	static SelectorNodes *createObjectsSelector(const Unigine::Vector<Unigine::NodePtr> &nodes);

	/// <summary> Subitem type. The first 256 types are reserved by UNIGINE, user subitem types start from USER + i (256 and higher).</summary>
	enum SubType
	{
		/// <summary> Subitems of nodes referred to via indices (such as surfaces, shapes, joints, billboards, forces, noises, or deflectors).</summary>
		INDEXED = 0,
		/// <summary> Beginning of the range of user-defined subitem types. Types are referred to as USER+0, USER+1...</summary>
		USER    = 0x0100
	};

	/// <summary> This class represents a list of selected subitems of a selected node. 
	/// When an object is selected, some of its surfaces or collision shapes can also be selected,
	/// the subitems list shall contain all of them.
	/// </summary>
	class UNIGINE_EDITOR_API SubItemList
	{
	public:
		SubItemList();
		virtual ~SubItemList();

		virtual int type() const = 0;
		/// <summary> Checks whether the subitems list is equal to the other list specified.</summary>
		/// <param name="other"> Other subitems list.</param>
		/// <returns> <b>true</b> if subitems lists are equal; otherwise, <b>false</b>.</returns>
		virtual bool equal(SubItemList *other) const = 0;
		/// <summary> Merges the subitems list with the other list specified.</summary>
		/// <param name="other"> Other subitems list.</param>
		virtual void merge(SubItemList *other) = 0;
		/// <summary> Excludes all subitems of the specified type from the list.</summary>
		/// <param name="sub"> Type of subitems to be excluded from the list. One of the <see cref="Constants::SubObjectType"/> enum values.</param>
		virtual void exclude(Constants::SubObjectType sub) = 0;

		virtual SubItemList *clone() const = 0;
	};

	SelectorNodes();
	explicit SelectorNodes(const Unigine::Vector<Unigine::NodePtr> &nodes);
	~SelectorNodes() override;

	/// <summary> Returns the current selector type.</summary>
	/// <returns> Current selector type: one of the <see cref="SelectorType"/> enum values.</returns>
	int type() const override;
	/// <summary> Checks whether the selector is equal to the specified target selector.</summary>
	/// <param name="selector"> Target selector.</param>
	/// <returns> <b>true</b> if selectors are equal; otherwise, <b>false</b>.</returns>
	bool equal(const Selector *selector) const override;
	/// <summary> Clones the current nodes selector.</summary>
	/// <returns> Clone of the current nodes selector.</returns>
	SelectorNodes *clone() const  override;

	/// <summary> Returns the list of all selected nodes.</summary>
	/// <returns> Vector containing all selected.</returns>
	Unigine::Vector<Unigine::NodePtr> getNodes() const;
	/// <summary> Returns the number of selected nodes.</summary>
	/// <returns> Number of selected nodes.</returns>
	int size() const;
	/// <summary> Checks whether the nodes selector is empty (no nodes are currently selected).</summary>
	/// <returns> <b>true</b> if the selector is empty; otherwise, <b>false</b>.</returns>
	bool empty() const;

	/// <summary> Extends the current selection by adding the specified node to it.</summary>
	/// <param name="node"> Node to be added to the current selection.</param>
	void extend(const Unigine::NodePtr &node);
	/// <summary> Extends the current selection by adding the specified list of nodes to it.</summary>
	/// <param name="nodes"> Vector containing nodes to be added to the current selection.</param>
	void extend(const Unigine::Vector<Unigine::NodePtr> &nodes);
	/// <summary> Extends the current selection by adding the specified node to it along with the list of selected subitems.</summary>
	/// <param name="node"> Node to be added to the current selection.</param>
	/// <param name="subs"> List of subitems to be added to the current selection.</param>
	void extend(const Unigine::NodePtr &node, SubItemList *subs);

	/// <summary> Excludes the specified node from the current selection.</summary>
	/// <param name="node"> Node to be excluded from the current selection.</param>
	void exclude(const Unigine::NodePtr &node);
	/// <summary> Excludes the specified list of nodes from the current selection.</summary>
	/// <param name="nodes"> Vector containing nodes to be excluded from the current selection.</param>
	void exclude(const Unigine::Vector<Unigine::NodePtr> &nodes);
	/// <summary> Excludes the specified type of subitems from the current selection for all 
	/// selected nodes (e.g., remove all joints from the selection for all selected nodes).
	/// </summary>
	/// <param name="stype"> Type of subitems to be excluded from the current selection for all nodes (<see cref="Constants::SubObjectType"/>).</param>
	void exclude(Constants::SubObjectType stype);

	/// <summary> Checks whether the current selection contains the specified node.</summary>
	/// <param name="node"> Node to be checked.</param>
	/// <returns> <b>true</b> if the current selection contains the specified node; otherwise, <b>false</b>.</returns>
	bool contains(const Unigine::NodePtr &node) const;

	/// <summary> Gets the list of subitems for the selected node with the specified ID.</summary>
	/// <param name="node_id"> ID of the selected node.</param>
	/// <returns> List of subitems for the selected node with the specified ID (if any); otherwise, <b>nullptr</b>.</returns>
	SubItemList *getSubItemList(int node_id) const;
	/// <summary> Gets the list of subitems for the specified selected node.</summary>
	/// <param name="node"> Selected node for which the list of selected subitems is to be obtained.</param>
	/// <returns> List of subitems for the specified selected node (if any); otherwise, <b>nullptr</b>.</returns>
	SubItemList *getSubItemList(const Unigine::NodePtr &node) const;
	/// <summary> Gets the list of subitem indices for the the selected node with the specified ID.
	/// Some subitems (such as surfaces, shapes, joints, billboards, forces, noises, or deflectors) are referred to via indices,
	/// and unlike the <see cref="getSubItemList"/>() which returns the list of subitems,
	/// this method returns the list of indices for them.
	/// </summary>
	/// <param name="node_id"> ID of the selected node for which the list of indices of selected subitems is to be obtained.</param>
	/// <returns> List of indices of selected subitems for the selected node with the specified ID (if any); otherwise, <b>nullptr</b>.</returns>
	IndexList *getIndexList(int node_id) const;
	/// <summary> Gets the list of subitem indices for the specified selected node.
	/// Some subitems (such as surfaces, shapes, joints, billboards, forces, noises, or deflectors) 
	/// are referred to via indices, and unlike the <see cref="getSubItemList"/>() 
	/// which returns the list of subitems, this method returns the list of indices for them.
	/// </summary>
	/// <param name="node"> Selected node for which the list of indices of selected subitems is to be obtained.</param>
	/// <returns> List of indices of selected subitems for the specified selected node (if any); otherwise, <b>nullptr</b>.</returns>
	IndexList *getIndexList(const Unigine::NodePtr &node) const;

	/// <summary> Gets the list of indices of the specified subitem type that are currently selected
	/// for all selected nodes (e.g., if surfaces 0, 1, 2, 3 of the first selected object
	/// and surfaces 1 and 3 of the second one are selected the result will be: {1, 3}). 
	/// Some subitems (such as surfaces, shapes, joints, billboards, forces, noises, or deflectors) 
	/// are referred to via indices, and unlike the <see cref="getSubItemList"/>() 
	/// which returns the list of subitems, this method returns the list of indices for them.
	/// </summary>
	/// <param name="stype"> Selected node for which the list of indices of selected subitems is to be obtained.</param>
	/// <returns> List of indices of selected subitems for the specified selected node (if any); otherwise, <b>nullptr</b>.</returns>
	Unigine::HashSet<int> getIntersectedIndexes(Constants::SubObjectType stype) const;

	/// <summary>
	/// Resets nodes cache used for selection. Storing direct pointers to nodes is unsafe, 
	/// so their IDs are stored instead. To optimize retrieval of nodes by their IDs
	/// the nodes cache is used. This method is used to reset this cache
	/// when it is no longer valid (e.g., in case of loading another world).
	/// </summary>
	void resetCache();

	// `expand` flag is a shitty solution to pass a param to the `WorldModel.`. Refactor this.
	[[deprecated]]
	void setNeedExpand(bool value);
	[[deprecated]]
	bool isNeedExpand() const;

private:
	::UnigineEditor::Internal::SelectorNodesPrivate *d;
};

/// <summary> This class represents a list of indices of selected subitems of a selected node. 
/// When an object is selected, some of its surfaces or collision shapes can also be selected,
/// the index list shall contain indices for all of them grouped by type.
/// </summary>
class UNIGINE_EDITOR_API IndexList : public SelectorNodes::SubItemList
{
public:
	IndexList();
	~IndexList() override;

	/// <summary> Extends the list by adding an item of the specified type
	/// with the specified index (e.g., you can add the third surface).
	/// </summary>
	/// <param name="stype"> Type of the item to be added. One of the <see cref="Constants::SubObjectType"/> enum values.</param>
	/// <param name="index"> Index of the item to be added.</param>
	void extend(Constants::SubObjectType stype, int index);
	/// <summary> Extends the list by adding a range of items of the specified type having indices
	/// in the specified range (e.g., you can add surfaces with indices from 2 to 6).</summary>
	/// <param name="stype"> Type of the item to be added. One of the <see cref="Constants::SubObjectType"/> enum values.</param>
	/// <param name="from"> Index of the first item of the range to be added.</param>
	/// <param name="to"> Index of the last item of the range to be added.</param>
	void extend(Constants::SubObjectType stype, int from, int to);

	/// <summary> Returns the type of the index list.</summary>
	/// <returns> SubType::INDEXED.</returns>
	int type() const override;
	/// <summary> Checks whether the index list is equal to the other list specified.</summary>
	/// <param name="other"> Other subitems list.</param>
	/// <returns> <b>true</b> if lists are equal; otherwise, <b>false</b>.</returns>
	bool equal(SubItemList *other) const override;
	/// <summary> Merges the index list with the other list specified.</summary>
	/// <param name="other"> Other subitems list.</param>
	void merge(SubItemList *other) override;

	/// <summary> Excludes all indices of subitems of the specified type from the list
	/// (e.g., remove all joints).
	/// </summary>
	/// <param name="stype"> Type of subitems to be excluded from the list (<see cref="Constants::SubObjectType"/>).</param>
	void exclude(Constants::SubObjectType stype) override;
	/// <summary> Excludes the specified index of a subitem of the specified type from the list
	/// (e.g., remove third joint).
	/// </summary>
	/// <param name="stype"> Type of subitems to be excluded from the list (<see cref="Constants::SubObjectType"/>).</param>
	/// <param name="index"> Item index.</param>
	void exclude(Constants::SubObjectType stype, int index);

	/// <summary> Checks whether the index list contains the subitem of the specified type
	/// having the specified index (e.g., you can check whether the second shape is selected).</summary>
	/// <param name="stype"> Subitem type. One of the <see cref="Constants::SubObjectType"/> enum values.</param>
	/// <param name="index"> Subitem index.</param>
	/// <returns> <b>true</b> if the list contains the subitem of the specified type having the specified index; otherwise, <b>false</b>.</returns>
	bool contains(Constants::SubObjectType stype, int index) const;

	/// <summary> Returns all indices of subitems of the specified type stored in the list (e.g., you can get all surfaces).</summary>
	/// <param name="stype"> Subitem type. One of the <see cref="Constants::SubObjectType"/> enum values.</param>
	/// <returns> A set of indices of all list subitems of the specified type (if any); otherwise, <b>nullptr</b>.</returns>
	Unigine::HashSet<int> getSubObjectIndexes(Constants::SubObjectType stype) const;
	/// <summary> Checks whether the index list does not contain any indices of subitems of the specified type.</summary>
	/// <returns> <b>true</b> if the list does not contain any indices of subitems of the specified type; otherwise, <b>false</b>.</returns>
	bool empty(Constants::SubObjectType stype) const;
	/// <summary> Returns the number of indices of subitems of the specified type.</summary>
	/// <param name="stype"> Subitem type. One of the <see cref="Constants::SubObjectType"/> enum values.</param>
	/// <returns> Number of indices of subitems of the specified type.</returns>
	int size(Constants::SubObjectType stype) const;

	/// <summary> Clones the index list.</summary>
	/// <returns> Clone of the index list.</returns>
	IndexList *clone() const override;

private:
	::UnigineEditor::Internal::IndexListPrivate *d;
};

} // namespace UnigineEditor
