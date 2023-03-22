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
#include <UnigineEngine.h>
#include <UnigineInterpreter.h>

namespace Unigine
{
namespace Plugins
{


class SteamLeaderboard
{
protected:
	virtual ~SteamLeaderboard(){}
public:

	enum SORT_METHOD
	{
		SORT_METHOD_NONE,
		SORT_METHOD_ASCENDING,
		SORT_METHOD_DESCENDING,
	};

	enum DISPLAY_TYPE
	{
		DISPLAY_TYPE_NONE,
		DISPLAY_TYPE_NUMERIC,
		DISPLAY_TYPE_TIME_SECONDS,
		DISPLAY_TYPE_TIME_MILLISECONDS,
	};

	enum DATA_REQUEST
	{
		DATA_REQUEST_GLOBAL,
		DATA_REQUEST_GLOBAL_AROUND_USER,
		DATA_REQUEST_FRIENDS,
		DATA_REQUEST_USERS,
	};
	virtual int getID() const = 0;
	virtual const char *getName() const = 0;
	virtual bool isFound() const = 0;
	virtual bool find() = 0;
	virtual bool findOrCreate(SteamLeaderboard::SORT_METHOD sort_method, SteamLeaderboard::DISPLAY_TYPE display_type) = 0;
	virtual bool uploadScore(int score, bool forced = false) = 0;
	virtual bool downloadScores(SteamLeaderboard::DATA_REQUEST request, int num_before, int num_after) = 0;
	virtual bool isUploading() const = 0;
	virtual bool isDownloading() const = 0;
	virtual bool isLastUploadFailed() const = 0;
	virtual bool isLastDownloadFailed() const = 0;
	virtual SteamLeaderboard::DATA_REQUEST getLastDataRequest() const = 0;
	virtual int getEntryCount() const = 0;
	virtual SteamLeaderboard::SORT_METHOD getSortMethod() const = 0;
	virtual SteamLeaderboard::DISPLAY_TYPE getDisplayType() const = 0;
	virtual int getNumEntries() const = 0;
	virtual unsigned long long getEntryUserID(int num) const = 0;
	virtual int getEntryRank(int num) const = 0;
	virtual int getEntryScore(int num) const = 0;
};


class Steam
{
protected:
	virtual ~Steam(){}
public:
	UNIGINE_INLINE static Steam *get() { return Unigine::Engine::get()->getPlugin<Steam>("Steam"); }

	enum OVERLAY_POSITION
	{
		OVERLAY_POSITION_TOP_LEFT,
		OVERLAY_POSITION_TOP_RIGHT,
		OVERLAY_POSITION_BOTTOM_LEFT,
		OVERLAY_POSITION_BOTTOM_RIGHT,
	};

	enum OVERLAY_TO_STORE
	{
		OVERLAY_TO_STORE_FLAG_NONE,
		OVERLAY_TO_STORE_ADD_TO_CART,
		OVERLAY_TO_STORE_ADD_TO_CART_AND_SHOW,
	};

	enum OVERLAY_TO_WEB_PAGE_MODE
	{
		OVERLAY_TO_WEB_PAGE_MODE_DEFAULT,
		OVERLAY_TO_WEB_PAGE_MODE_MODAL,
	};

	enum FRIEND_RELATIONSHIP
	{
		FRIEND_RELATIONSHIP_NONE,
		FRIEND_RELATIONSHIP_BLOCKED,
		FRIEND_RELATIONSHIP_REQUEST_RECIPIENT,
		FRIEND_RELATIONSHIP_FRIEND,
		FRIEND_RELATIONSHIP_REQUEST_INITIATOR,
		FRIEND_RELATIONSHIP_IGNORED,
		FRIEND_RELATIONSHIP_IGNORED_FRIEND,
	};

	enum FRIEND_FLAG
	{
		FRIEND_FLAG_NONE = 0,
		FRIEND_FLAG_BLOCKED = 1,
		FRIEND_FLAG_FRIENDSHIP_REQUESTED = 2,
		FRIEND_FLAG_IMMEDIATE = 4,
		FRIEND_FLAG_CLAN_MEMBER = 8,
		FRIEND_FLAG_ON_GAME_SERVER = 16,
		FRIEND_FLAG_REQUESTING_FRIENDSHIP = 128,
		FRIEND_FLAG_REQUESTING_INFO = 256,
		FRIEND_FLAG_IGNORED = 512,
		FRIEND_FLAG_IGNORED_FRIEND = 1024,
		FRIEND_FLAG_ALL = 65535,
	};

	enum PERSONA_STATE
	{
		PERSONA_STATE_OFFLINE,
		PERSONA_STATE_ONLINE,
		PERSONA_STATE_BUSY,
		PERSONA_STATE_AWAY,
		PERSONA_STATE_SNOOZE,
		PERSONA_STATE_LOOKING_TO_TRADE,
		PERSONA_STATE_LOOKING_TO_PLAY,
	};
	virtual int getAppID() const = 0;
	virtual bool isSubscribed() const = 0;
	virtual bool isLowViolence() const = 0;
	virtual bool isCybercafe() const = 0;
	virtual bool isVACBanned() const = 0;
	virtual const char *getCurrentGameLanguage() const = 0;
	virtual const char *getAvailableGameLanguages() const = 0;
	virtual const char *getUserDataFolder() const = 0;
	virtual void showOverlay(const char *dialog) = 0;
	virtual void showOverlayToWebPage(const char *url, Steam::OVERLAY_TO_WEB_PAGE_MODE mode) = 0;
	virtual void showOverlayToUser(const char *dialog, unsigned long long steam_id) = 0;
	virtual void showOverlayToStore(int app_id, Steam::OVERLAY_TO_STORE store_flag) = 0;
	virtual void showOverlayInviteDialog(unsigned long long steam_id_lobby) = 0;
	virtual void setOverlayNotificationPosition(Steam::OVERLAY_POSITION position) = 0;
	virtual bool isOverlayShown() const = 0;
	virtual unsigned long long getMyUserID() const = 0;
	virtual const char *getMyName() const = 0;
	virtual Steam::PERSONA_STATE getMyState() const = 0;
	virtual const char *getUserName(unsigned long long steam_id_friend) = 0;
	virtual int getNumFriends(int friend_flags) = 0;
	virtual unsigned long long getFriendByIndex(int ifriend, int ifriend_flags) = 0;
	virtual Steam::FRIEND_RELATIONSHIP getUserRelationship(unsigned long long steam_id_friend) = 0;
	virtual Steam::PERSONA_STATE getUserPersonaState(unsigned long long steam_id_friend) = 0;
	virtual Variable  getUserAvatarSmall(unsigned long long id) = 0;
	virtual Variable  getUserAvatarMedium(unsigned long long id) = 0;
	virtual Variable  getUserAvatarLarge(unsigned long long id) = 0;

	enum CALLBACK_INDEX
	{
		CALLBACK_OVERLAY_SHOWN,
		CALLBACK_LEADERBOARD_FOUND,
		CALLBACK_LEADERBOARD_SCORES_UPLOADED,
		CALLBACK_LEADERBOARD_SCORES_DOWNLOADED,
	};
	virtual void *addCallback(Steam::CALLBACK_INDEX callback, CallbackBase1<unsigned char> *func) = 0;
	virtual void *addCallback(Steam::CALLBACK_INDEX callback, CallbackBase1<int> *func) = 0;
	virtual void *addCallback(Steam::CALLBACK_INDEX callback, CallbackBase3<int, int, unsigned char> *func) = 0;
	virtual void *addCallback(Steam::CALLBACK_INDEX callback, CallbackBase3<int, int, int> *func) = 0;
	virtual bool removeCallback(Steam::CALLBACK_INDEX callback, void *id) = 0;
	virtual void clearCallbacks(Steam::CALLBACK_INDEX callback) = 0;
	virtual SteamLeaderboard * createLeaderboard(const char *name) = 0;
	virtual SteamLeaderboard * getLeaderboard(int id) = 0;
	virtual void deleteLeaderboard(SteamLeaderboard *leaderboard) = 0;
};

} // namespace Plugins
} // namespace Unigine
