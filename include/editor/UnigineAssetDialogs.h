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
#include <UnigineGUID.h>
#include <UnigineVector.h>


namespace UnigineEditor
{
/// <summary>This class is used to represent a standard asset dialog of the UnigineEditor that can be used for opening and saving assets.</summary>
class UNIGINE_EDITOR_API AssetDialogs final
{
public:
	/// <summary>Dialog mode.</summary>
	enum class DialogMode
	{
		/// <summary>Modal dialog, which requires the user to respond before continuing the program (the function returns control only when the user closes the dialog).</summary>
		Modal,
		/// <summary>Modeless dialog, which stays on the screen and is available for use at any time but permits other user activities (the function returns control right after creating and displaying the dialog).</summary>
		Modeless
	};

	/// <summary>Structure representing an asset selected in the dialog.</summary>
	struct SelectedAsset
	{
		/// <summary>GUID of the selected asset. Additional information for a given GUID can be obtained via the <a href="https://developer.unigine.com/en/docs/latest/api/library/filesystem/class.filesystem">Unigine::FileSystem</a> class.</summary>
		Unigine::UGUID asset_guid;
		/// <summary>GUID of the selected asset's runtime-file. Additional information for a given GUID can be obtained via the <a href="https://developer.unigine.com/en/docs/latest/api/library/filesystem/class.filesystem">Unigine::FileSystem</a> class.</summary>
		Unigine::UGUID runtime_guid;
	};

	/// <summary>
	/// Opens a dialog enabling the user to select a single asset. When the dialog opens, the specified default path and file filter shall be set displaying the corresponding elements. On selecting an asset and confirming selection by clicking OK, the specified callback function is executed.
	/// </summary>
	/// <param name="callback">Callback function to be fired on selecting an asset and clicking OK. The signature of the callback function is as follows: void (const SelectedAsset &)</param>
	/// <param name="window_title">Title of the dialog window. If not specified, the default title is 'Select Asset'.</param>
	/// <param name="filter">Filter, which is used to display files of required types only. List of file extensions with leading dots and without additional separators, for example: .mesh.prop. If the filter is empty, the dialog displays assets of all types.</param>
	/// <param name="hint_asset_path">Path to an asset to be selected by default when the dialog opens.</param>
	/// <param name="mode">Dialog mode to be set (see <see cref="DialogMode"/>).</param>
	static void browseAsset(Unigine::CallbackBase1<const SelectedAsset &> *callback,
		const char *window_title = nullptr,
		const char *filter = nullptr,
		const char *hint_asset_path = nullptr,
		DialogMode mode = DialogMode::Modal);

	/// <summary>
	/// Opens a dialog enabling the user to select multiple assets. When the dialog opens, the specified default path and file filter shall be set displaying the corresponding elements. On selecting assets and confirming selection by clicking OK, the specified callback function is executed.
	/// </summary>
	/// <param name="callback">Callback function to be fired on selecting assets and clicking OK. The signature of the callback function is as follows: void (const Unigine::Vector<SelectedAsset> &)</param>
	/// <param name="window_title">Title of the dialog window. If not specified, the default title is 'Select Asset'.</param>
	/// <param name="filter">Filter, which is used to display files of required types only. List of file extensions with leading dots and without additional separators, for example: .mesh.prop. If the filter is empty, the dialog displays assets of all types.</param>
	/// <param name="hint_asset_path">Path to an asset to be selected by default when the dialog opens.</param>
	/// <param name="mode">Dialog mode to be set (see <see cref="DialogMode"/>).</param>
	static void browseAssetList(Unigine::CallbackBase1<const Unigine::Vector<SelectedAsset> &> *callback,
		const char *window_title = nullptr,
		const char *filter = nullptr,
		const char *hint_asset_path = nullptr,
		DialogMode mode = DialogMode::Modal);

	/// <summary>
	/// Opens a dialog enabling the user to save an asset to the selected path. When the dialog opens, the specified default path and file filter shall be set displaying the corresponding elements. On selecting assets and confirming selection by clicking OK, the specified callback function is executed.
	/// </summary>
	/// <param name="callback">Callback function to be fired on selecting an asset and clicking OK. The signature of the callback function is as follows: void (const char *)</param>
	/// <param name="window_title">Title of the dialog window. If not specified, the default title is 'Save Asset'.</param>
	/// <param name="filter">Filter, which is used to display files of required types only. List of file extensions with leading dots and without additional separators, for example: .mesh.prop. If the filter is empty, the dialog displays assets of all types.</param>
	/// <param name="dest_path">Destination path to be set by default, when the dialog opens.</param>
	/// <param name="mode">Dialog mode to be set (see <see cref="DialogMode"/>).</param>
	static void saveAsset(Unigine::CallbackBase1<const char *> *callback,
		const char *window_title = nullptr,
		const char *filter = nullptr,
		const char *dest_path = nullptr,
		DialogMode mode = DialogMode::Modal);
};

} // namespace UnigineEditor
