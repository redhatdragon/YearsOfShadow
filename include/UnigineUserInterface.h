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

#include "UnigineGui.h"
#include "UnigineWidgets.h"
#include "UnigineCallback.h"

namespace Unigine
{


class UNIGINE_API UserInterface : public APIInterface
{
public:
	static Ptr<UserInterface> create(const Ptr<Gui> &gui, const char *name, const char *prefix = 0);
	void setGui(const Ptr<Gui> &gui);
	Ptr<Gui> getGui() const;
	void setLifetime(Widget::LIFETIME lifetime);
	Widget::LIFETIME getLifetime() const;
	void updateWidgets() const;
	int getNumWidgets() const;
	int findWidget(const char *name) const;
	Ptr<Widget> getWidget(int num) const;
	const char *getWidgetName(int num) const;
	const char *getWidgetNext(int num) const;
	int getWidgetExport(int num) const;
	int getNumCallbacks(int num) const;
	int getCallback(int num, int callback) const;
	const char *getCallbackName(int num, int callback) const;
	Ptr<Widget> getCallbackWidgetData(int num, int callback) const;
	const char *getCallbackStringData(int num, int callback) const;
	const char *getCallbackVariableData(int num, int callback) const;
	const char *getCallbackInstanceData(int num, int callback) const;
	void *addCallback(const char *name, Gui::CALLBACK_INDEX callback, CallbackBase *func);
	void *addCallback(const char *name, Gui::CALLBACK_INDEX callback, CallbackBase1< Ptr<Widget> > *func);
	void *addCallback(const char *name, Gui::CALLBACK_INDEX callback, CallbackBase2< Ptr<Widget>, Ptr<Widget> > *func);
	void *addCallback(const char *name, Gui::CALLBACK_INDEX callback, CallbackBase3< Ptr<Widget>, Ptr<Widget>, int > *func);
	bool removeCallback(const char *name, Gui::CALLBACK_INDEX callback, void *id);
	void clearCallbacks(const char *name, Gui::CALLBACK_INDEX callback);
};
typedef Ptr<UserInterface> UserInterfacePtr;

} // namespace Unigine
