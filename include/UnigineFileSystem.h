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

#include "UnigineString.h"
#include "UnigineMesh.h"
#include "UnigineImage.h"
#include "UnigineGUID.h"

namespace Unigine
{

// Unigine external package.
class UNIGINE_API Package {
public:
	virtual ~Package() { }
	virtual Package *clone() = 0;
	virtual int getNumFiles() = 0;
	virtual const char *getFileName(int num) = 0;
	virtual int selectFile(const char *name, int &size) = 0;
	virtual int readFile(unsigned char *data, int size) = 0;
};


class UNIGINE_API FileSystemMount : public APIInterface
{
public:

	enum
	{
		ACCESS_READONLY = 0,
		ACCESS_READWRITE,
	};
	int getAccess() const;
	const char *getName() const;
	const char *getDataPath() const;
	const char *getVirtualPath() const;
	const char *getUMountPath() const;
	void getVirtualFiles(Vector<String> &files) const;
	void setExclusiveFilters(Vector<String> &filters);
	void getExclusiveFilters(Vector<String> &filters) const;
	void setIgnoreFilters(Vector<String> &filters);
	void getIgnoreFilters(Vector<String> &filters) const;
	bool isPathFilteredOut(const char *path) const;
};
typedef Ptr<FileSystemMount> FileSystemMountPtr;

struct FilePath
{
	UGUID guid;
	String path;
	String ext;
};
//////////////////////////////////////////////////////////////////////////

class UNIGINE_API FileSystem
{
protected:
	

public:
	static int isInitialized(); 

	enum CALLBACK_INDEX
	{
		CALLBACK_FILE_ADDED = 0,
		CALLBACK_FILE_REMOVED,
		CALLBACK_FILE_CHANGED,
		CALLBACK_FILES_ADDED,
		CALLBACK_FILES_REMOVED,
		CALLBACK_FILES_CHANGED,
		NUM_CALLBACKS,
	};
	static Ptr<FileSystemMount> getMount(const char *path);
	static Ptr<FileSystemMount> getMount(const UGUID &guid);
	static void getMounts(Vector<Ptr<FileSystemMount>> &container);
	static Ptr<FileSystemMount> getRootMount();
	static Ptr<FileSystemMount> createMount(const char *absolute_path, const char *virtual_path, int access);
	static Ptr<FileSystemMount> addMount(const char *umount_path);
	static bool saveMountFile(const char *umount_path);
	static bool removeMount(const char *path);
	static void clearMounts();
	static bool loadPackage(const char *path);
	static bool loadPackage(const char *path, const char *extension);
	static bool removePackage(const char *path);
	static void getSupportedPackagesExtensions(Vector<String> &extensions);
	static void getPackageVirtualFiles(const char *path, Vector<String> &files);
	static void getPackageVirtualFiles(const char *path, const char *extension, Vector<String> &files);
	static void preloadExternPackage(Package *package);
	static void preloadExternPackage(const char *path, Package *package);
	static void clearPreloadedExternPackages();
	static bool addExternPackage(Package *package);
	static bool addExternPackage(const char *path, Package *package);
	static void getVirtualFiles(Vector<String> &files);
	static void getVirtualFiles(Vector<UGUID> &files);
	static bool isVirtualFile(const char *path);
	static bool isVirtualFile(const UGUID &guid);
	static bool addVirtualFile(const char *path, const UGUID &guid, bool must_exist = false);
	static UGUID addVirtualFile(const char *path, bool must_exist = false);
	static bool renameVirtualFile(const char *path, const char *new_path);
	static bool renameVirtualFile(const UGUID &guid, const char *new_path);
	static bool renameVirtualFile(const char *path, const char *new_path, const UGUID &new_guid);
	static bool renameVirtualFile(const UGUID &guid, const char *new_path, const UGUID &new_guid);
	static bool removeVirtualFile(const char *path);
	static bool removeVirtualFile(const UGUID &guid);
	static bool changeVirtualFile(const char *path);
	static bool changeVirtualFile(const UGUID &guid);
	static void removeNonExistingVirtualFiles();
	static bool isBlobFile(const char *path);
	static bool isBlobFile(const UGUID &guid);
	static bool addBlobFile(const char *path);
	static bool addBlobFile(const UGUID &guid);
	static bool removeBlobFile(const char *path);
	static bool removeBlobFile(const UGUID &guid);
	static bool isCacheFile(const char *path);
	static bool isCacheFile(const UGUID &guid);
	static bool addCacheFile(const char *path);
	static bool addCacheFile(const UGUID &guid);
	static bool removeCacheFile(const char *path);
	static bool removeCacheFile(const UGUID &guid);
	static bool isDiskFile(const char *path);
	static bool isDiskFile(const UGUID &guid);
	static bool isPackageFile(const char *path);
	static bool isPackageFile(const UGUID &guid);
	static long long getFileSize(const char *path);
	static long long getFileSize(const UGUID &guid);
	static String resolvePartialVirtualPath(const char *path);
	static String getVirtualPath(const char *path);
	static String getVirtualPath(const UGUID &guid);
	static String getAbsolutePath(const char *path);
	static String getAbsolutePath(const UGUID &guid);
	static String getPackageVirtualPath(const char *path);
	static String getPackageVirtualPath(const UGUID &guid);
	static String getPackageAbsolutePath(const char *path);
	static String getPackageAbsolutePath(const UGUID &guid);
	static String getExtension(const char *path);
	static String getExtension(const UGUID &guid);
	static bool isFileExist(const char *path);
	static bool isFileExist(const UGUID &guid);
	static bool isGUIDPath(const char *path);
	static bool isAssetPath(const char *path);
	static long long getMTime(const char *path);
	static long long getMTime(const UGUID &guid);
	static bool loadGUIDs(const char *path);
	static bool saveGUIDs(const char *path, bool binary = false);
	static UGUID generateGUID();
	static bool setGUID(const char *path, const UGUID &guid);
	static UGUID getGUID(const char *path);
	static int getNumModifiers();
	static String getModifier(int num);
	static void addModifier(const char *name);
	static void removeModifier(const char *name);
	static void clearModifiers();
	static void *addCallback(FileSystem::CALLBACK_INDEX callback, CallbackBase1<const Vector<FilePath> &> *func);
	static void *addCallback(FileSystem::CALLBACK_INDEX callback, CallbackBase2<const char *, UGUID> *func);
	static bool removeCallback(FileSystem::CALLBACK_INDEX callback, void *id);
	static void clearCallbacks(FileSystem::CALLBACK_INDEX callback);
};

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API FileSystemAssets
{
protected:
	

public:
	static int isInitialized(); 
	static UGUID addRuntime(const UGUID &asset_guid, const char *alias, int primary = 0);
	static UGUID addRuntime(const char *asset_path, const char *alias, int primary = 0);
	static bool addRuntime(const UGUID &asset_guid, const char *alias, const UGUID &runtime_guid, int primary = 0);
	static bool addRuntime(const char *asset_path, const char *alias, const UGUID &runtime_guid, int primary = 0);
	static bool copyRuntime(const UGUID &asset_guid, int index, const UGUID &new_asset_guid, const UGUID &new_runtime_guid);
	static bool copyRuntime(const char *asset_path, int index, const UGUID &new_asset_guid, const UGUID &new_runtime_guid);
	static bool copyRuntime(const UGUID &asset_guid, int index, const char *new_asset_path, const UGUID &new_runtime_guid);
	static bool copyRuntime(const char *asset_path, int index, const char *new_asset_path, const UGUID &new_runtime_guid);
	static UGUID copyRuntime(const UGUID &asset_guid, int index, const UGUID &new_asset_guid);
	static UGUID copyRuntime(const char *asset_path, int index, const UGUID &new_asset_guid);
	static UGUID copyRuntime(const UGUID &asset_guid, int index, const char *new_asset_path);
	static UGUID copyRuntime(const char *asset_path, int index, const char *new_asset_path);
	static bool copyRuntime(const UGUID &runtime_guid, const UGUID &new_asset_guid, const UGUID &new_runtime_guid);
	static bool copyRuntime(const char *runtime_path, const UGUID &new_asset_guid, const UGUID &new_runtime_guid);
	static bool copyRuntime(const UGUID &runtime_guid, const char *new_asset_path, const UGUID &new_runtime_guid);
	static bool copyRuntime(const char *runtime_path, const char *new_asset_path, const UGUID &new_runtime_guid);
	static UGUID copyRuntime(const UGUID &runtime_guid, const UGUID &new_asset_guid);
	static UGUID copyRuntime(const char *runtime_path, const UGUID &new_asset_guid);
	static UGUID copyRuntime(const UGUID &runtime_guid, const char *new_asset_path);
	static UGUID copyRuntime(const char *runtime_path, const char *new_asset_path);
	static bool removeRuntime(const UGUID &asset_guid, int index);
	static bool removeRuntime(const char *asset_path, int index);
	static bool removeRuntime(const UGUID &runtime_guid);
	static bool removeRuntime(const char *runtime_path);
	static bool clearRuntimes(const UGUID &asset_guid);
	static bool clearRuntimes(const char *asset_path);
	static int getNumRuntimes(const UGUID &asset_path);
	static int getNumRuntimes(const char *asset_path);
	static bool setRuntimeGUID(const UGUID &asset_guid, int index, const UGUID &new_runtime_guid);
	static bool setRuntimeGUID(const char *asset_path, int index, const UGUID &new_runtime_guid);
	static bool setRuntimeGUID(const UGUID &runtime_guid, const UGUID &new_runtime_guid);
	static bool setRuntimeGUID(const char *runtime_path, const UGUID &new_runtime_guid);
	static UGUID getRuntimeGUID(const UGUID &asset_guid, int index);
	static UGUID getRuntimeGUID(const char *asset_path, int index);
	static bool setRuntimeAlias(const UGUID &asset_guid, int index, const char *new_alias);
	static bool setRuntimeAlias(const char *asset_path, int index, const char *new_alias);
	static bool setRuntimeAlias(const UGUID &runtime_guid, const char *new_alias);
	static bool setRuntimeAlias(const char *runtime_path, const char *new_alias);
	static String getRuntimeAlias(const UGUID &asset_guid, int index);
	static String getRuntimeAlias(const char *asset_path, int index);
	static String getRuntimeAlias(const UGUID &runtime_guid);
	static String getRuntimeAlias(const char *runtime_path);
	static bool setRuntimePrimary(const UGUID &asset_guid, int index, int primary);
	static bool setRuntimePrimary(const char *asset_path, int index, int primary);
	static bool setRuntimePrimary(const UGUID &runtime_guid, int primary);
	static bool setRuntimePrimary(const char *runtime_path, int primary);
	static bool setRuntimeParameter(const UGUID &asset_guid, int index, const char *parameter_name, const char *new_parameter_value);
	static bool setRuntimeParameter(const char *asset_path, int index, const char *parameter_name, const char *new_parameter_value);
	static bool setRuntimeParameter(const UGUID &runtime_guid, const char *parameter_name, const char *new_parameter_value);
	static bool setRuntimeParameter(const char *runtime_path, const char *parameter_name, const char *new_parameter_value);
	static String getRuntimeParameter(const UGUID &asset_guid, int index, const char *parameter_name);
	static String getRuntimeParameter(const char *asset_path, int index, const char *parameter_name);
	static String getRuntimeParameter(const UGUID &runtime_guid, const char *parameter_name);
	static String getRuntimeParameter(const char *runtime_path, const char *parameter_name);
	static bool hasRuntimeParameter(const UGUID &asset_guid, int index, const char *parameter_name);
	static bool hasRuntimeParameter(const char *asset_path, int index, const char *parameter_name);
	static bool hasRuntimeParameter(const UGUID &runtime_guid, const char *parameter_name);
	static bool hasRuntimeParameter(const char *runtime_path, const char *parameter_name);
	static bool removeRuntimeParameter(const UGUID &asset_guid, int index, const char *parameter_name);
	static bool removeRuntimeParameter(const char *asset_path, int index, const char *parameter_name);
	static bool removeRuntimeParameter(const UGUID &runtime_guid, const char *parameter_name);
	static bool removeRuntimeParameter(const char *runtime_path, const char *parameter_name);
	static bool isMeta(const UGUID &any_guid);
	static bool isRuntime(const UGUID &any_guid);
	static bool isRuntime(const char *path);
	static bool isRuntimePath(const char *path);
	static bool isAsset(const UGUID &any_guid);
	static bool isAsset(const char *path);
	static bool isPrimary(const UGUID &any_guid);
	static bool isPrimary(const char *path);
	static UGUID getAssetGUID(const UGUID &any_guid);
	static UGUID getAssetGUID(const char *path);
	static UGUID getPrimaryGUID(const UGUID &any_guid);
	static UGUID getPrimaryGUID(const char *path);
	static UGUID resolveRuntimeAlias(const char *path);
	static UGUID resolve(const UGUID &any_guid);
	static UGUID resolvePrimary(const UGUID &any_guid);
	static UGUID resolveAsset(const UGUID &any_guid);
};

} // namespace Unigine
