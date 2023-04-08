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


#include <UnigineEngine.h>

#include "AppEditorLogic_impl.h"
#include "AppSystemLogic_impl.h"
#include "AppWorldLogic_impl.h"

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	// UnigineLogic
	AppSystemLogic system_logic;
	AppWorldLogic world_logic;
	AppEditorLogic editor_logic;

	// init engine
	Unigine::EnginePtr engine(argc, argv);

	EE_init();

	// enter main loop
	engine->main(&system_logic, &world_logic, &editor_logic);

	return 0;
}

#ifdef _WIN32
#include <Windows.h>
extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
extern "C" __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif
