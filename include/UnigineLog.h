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

#include "UnigineBase.h"


namespace Unigine
{
namespace Math
{
struct vec4;
}
}

namespace Unigine
{

template <class A0>
class CallbackBase1;
using LogCallback = CallbackBase1<const char *>;

class Log
{
public:
	enum CALLBACK_INDEX
	{
		CALLBACK_MESSAGE,
		CALLBACK_WARNING,
		CALLBACK_ERROR,
		CALLBACK_FATAL,
		CALLBACK_COUNT
	};

	// Prints a message to the console and the log file.
	static UNIGINE_API void message(const char *format, ...) UNIGINE_PRINTF(1, 2);
	static UNIGINE_API void message(const Unigine::Math::vec4 &color, const char *format, ...);

	// Prints a warning to the console and the log file.
	static UNIGINE_API void warning(const char *format, ...) UNIGINE_PRINTF(1, 2);

	// Prints an error message to the console and the log file.
	static UNIGINE_API void error(const char *format, ...) UNIGINE_PRINTF(1, 2);

	// Prints a fatal error message to the log file and immediately quit the engine.
	static UNIGINE_API void fatal(const char *format, ...) UNIGINE_PRINTF(1, 2);


	// Add callback
	static UNIGINE_API void *addCallback(CALLBACK_INDEX callback, LogCallback *func);
	static UNIGINE_API bool removeCallback(CALLBACK_INDEX callback, void* id);
	static UNIGINE_API void clearCallbacks(CALLBACK_INDEX callback);
	static UNIGINE_API bool hasCallbacks(CALLBACK_INDEX callback);
};

} // namespace Unigine
