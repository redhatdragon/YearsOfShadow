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

// DO NOT EDIT DIRECTLY. This is an auto-generated file. Your changes will be lost.

#pragma once

#include <UnigineCallback.h>
#include <UnigineGUID.h>
#include <UniginePtr.h>

#include "UnigineEditorGlobal.h"
#include "UnigineCollection.h"

namespace UnigineEditor
{

class MountPointParameters;
//////////////////////////////////////////////////////////////////////////

class UNIGINE_EDITOR_API AssetManager
{
protected:
	

public:
	static int isInitialized(); 
	// assets
	static Unigine::UGUID getAssetGUIDFromPath(const char *asset_path);
	static Unigine::String getAssetPathFromGUID(const Unigine::UGUID &asset_guid);
	static bool importAssetSync(const char *asset_path, const Unigine::Ptr<Collection> &import_parameters = {});
	static bool importAssetAsync(const char *asset_path, const Unigine::Ptr<Collection> &import_parameters = {});
	static bool reimportAssetSync(const char *asset_path, const Unigine::Ptr<Collection> &import_parameters = {});
	static bool reimportAssetAsync(const char *asset_path, const Unigine::Ptr<Collection> &import_parameters = {});
	static bool removeAssetSync(const char *asset_path);
	static bool removeAssetAsync(const char *asset_path);
	static bool moveAssetSync(const char *old_asset_path, const char *new_asset_path);
	static bool moveAssetAsync(const char *old_asset_path, const char *new_asset_path);
	static bool renameAssetSync(const char *asset_path, const char *new_asset_name);
	static bool renameAssetAsync(const char *asset_path, const char *new_asset_name);
	static bool copyAssetSync(const char *asset_path, const char *new_asset_path);
	static bool copyAssetAsync(const char *asset_path, const char *new_asset_path);
	static bool isAsset(const char *asset_path);
	static Unigine::Ptr<Collection> getAssetImportParameters(const char *asset_path);
	static bool isAssetWritable(const char *asset_path);
	static Unigine::Vector<Unigine::UGUID> getAssetGUIDs();
	static Unigine::Vector<Unigine::String> getAssetPaths();
	static Unigine::Vector<Unigine::UGUID> getAssetGUIDsForDirectory(const char *directory_path);
	static Unigine::Vector<Unigine::String> getAssetPathsForDirectory(const char *directory_path);
	static Unigine::Vector<Unigine::UGUID> getRuntimeGUIDs(const char *asset_path);
	static Unigine::String getRuntimeAlias(const Unigine::UGUID &runtime_guid);
	static bool isRuntimePrimary(const Unigine::UGUID &runtime_guid);
	// directories
	static bool createDirectory(const char *directory_path);
	static bool removeDirectorySync(const char *directory_path);
	static bool removeDirectoryAsync(const char *directory_path);
	static bool moveDirectorySync(const char *old_directory_path, const char *new_directory_path);
	static bool moveDirectoryAsync(const char *old_directory_path, const char *new_directory_path);
	static bool renameDirectorySync(const char *directory_path, const char *new_directory_name);
	static bool renameDirectoryAsync(const char *directory_path, const char *new_directory_name);
	static bool copyDirectorySync(const char *directory_path, const char *new_directory_path);
	static bool copyDirectoryAsync(const char *directory_path, const char *new_directory_path);
	static bool isDirectory(const char *directory_path);
	static bool isDirectoryWritable(const char *directory_path);
	static Unigine::Vector<Unigine::String> getDirectoryPathsAll();
	static Unigine::Vector<Unigine::String> getDirectoryPaths(const char *directory_path);
	// mount points
	static bool createMountPoint(const char *directory_path, const Unigine::Ptr<MountPointParameters> &mount_creation_parameters);
	static bool removeMountPoint(const char *directory_path);
	static bool isMountPoint(const char *directory_path);
	static Unigine::Ptr<MountPointParameters> getMountPointParameters(const char *directory_path);
	static bool refreshMountPointAsync(const char *directory_path);
	// file system watcher
	static void blockAutoRefresh();
	static void unblockAutoRefresh();
	static bool isAutoRefreshBlocked();
	// asset callbacks
	static void *addAssetAddedCallback(Unigine::CallbackBase1<const char *> *func);
	static bool removeAssetAddedCallback(void *id);
	static void *addAssetBeforeRemoveCallback(Unigine::CallbackBase1<const char *> *func);
	static bool removeAssetBeforeRemoveCallback(void *id);
	static void *addAssetRemovedCallback(Unigine::CallbackBase1<const char *> *func);
	static bool removeAssetRemovedCallback(void *id);
	static void *addAssetChangedCallback(Unigine::CallbackBase1<const char *> *func);
	static bool removeAssetChangedCallback(void *id);
	static void *addAssetMovedCallback(Unigine::CallbackBase2<const char *, const char *> *func);
	static bool removeAssetMovedCallback(void *id);
	// directory callbacks
	static void *addDirectoryAddedCallback(Unigine::CallbackBase1<const char *> *func);
	static bool removeDirectoryAddedCallback(void *id);
	static void *addDirectoryBeforeRemoveCallback(Unigine::CallbackBase1<const char *> *func);
	static bool removeDirectoryBeforeRemoveCallback(void *id);
	static void *addDirectoryRemovedCallback(Unigine::CallbackBase1<const char *> *func);
	static bool removeDirectoryRemovedCallback(void *id);
	static void *addDirectoryMovedCallback(Unigine::CallbackBase2<const char *, const char *> *func);
	static bool removeDirectoryMovedCallback(void *id);
	// process callbacks
	static void *addProcessBeginCallback(Unigine::CallbackBase *func);
	static bool removeProcessBeginCallback(void *id);
	static void *addProcessEndCallback(Unigine::CallbackBase *func);
	static bool removeProcessEndCallback(void *id);
	// utils
	static Unigine::String generateUniquePath(const char *path);
	static bool isExist(const char *path);
};


class UNIGINE_EDITOR_API MountPointParameters : public Unigine::APIInterface
{
public:

	enum ACCESS
	{
		ACCESS_READONLY = 0,
		ACCESS_READWRITE,
	};
	static Unigine::Ptr<MountPointParameters> create();
	void setAccess(MountPointParameters::ACCESS access);
	MountPointParameters::ACCESS getAccess() const;
	void setAbsolutePath(const char *path);
	const char *getAbsolutePath() const;
	void addExclusiveFilter(const char *exclusive_filter);
	Unigine::Vector<Unigine::String> getExclusiveFilters() const;
	void addIgnoreFilter(const char *ignore_filter);
	Unigine::Vector<Unigine::String> getIgnoreFilters() const;
};
typedef Unigine::Ptr<MountPointParameters> MountPointParametersPtr;

} // namespace UnigineEditor
