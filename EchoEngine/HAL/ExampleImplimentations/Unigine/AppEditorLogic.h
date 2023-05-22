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


#ifndef __APP_EDITOR_LOGIC_H__
#define __APP_EDITOR_LOGIC_H__

#include <UnigineLogic.h>

class AppEditorLogic : public Unigine::EditorLogic
{
public:
	AppEditorLogic();
	virtual ~AppEditorLogic();

	int init() override;

	int update() override;
	int render(const Unigine::EngineWindowPtr &window) override;

	int shutdown() override;

	int worldInit() override;
	int worldShutdown() override;
	int worldSave() override;
};

#endif // __APP_EDITOR_LOGIC_H__
