#include <stdint.h>
#include <intrin.h>

inline int64_t getCPUTicks() {
	//_asm rdtsc
	return __rdtsc();
}

void profilerLog(const char* name, uint64_t ticksToAdd);
void profilerDisplayLogs();

#define profileFunctionWithReturn(functionCall, returnValue) \
	{ \
	uint64_t _ticks = getCPUTicks(); \
	returnValue = functionCall; \
	_ticks = getCPUTicks()-_ticks; \
	profilerLog(#functionCall, _ticks); \
	}
#define profileFunction(functionCall) \
	{ \
	uint64_t _ticks = getCPUTicks(); \
	functionCall; \
	_ticks = getCPUTicks()-_ticks; \
	profilerLog(#functionCall, _ticks); \
	}

void _profileLinesStart(const char* file, const char* function, int line);
void _profileLinesEnd(const char* file, const char* function, int line);
// TODO: Lol lets see how hard this one fails.
// TODO: Need to find proper names for the _defines
#define _S1(x) #x
#define _S2(x) S1(x)
#define _LOCATION __FILE__ " : " __LINE__
#define profileLinesStart() \
	_profileLinesStart(__FILE__, __FUNCTION__, __LINE__);
#define profileLinesEnd() \
	_profileLinesEnd(__FILE__, __FUNCTION__, __LINE__);