#pragma once
#include <vector>
#include <Windows.h>

struct ThreadHeader {
	void (*foo)(void*);
	void* data;
};

class CustomWindowsThread {

};

DWORD WINAPI threadFunctionWrapper(void* _threadHeader) {
	ThreadHeader* threadHeader = (ThreadHeader*)_threadHeader;
	threadHeader->foo(threadHeader->data);
	return 0;
}

class CustomWindowsThreadPool {
	static constexpr uint32_t MAX_POSSIBLE_THREADS = 256;
	//void* datas[MAX_POSSIBLE_THREADS];
	ThreadHeader threadHeaders[MAX_POSSIBLE_THREADS];
	DWORD   threadIDs[MAX_POSSIBLE_THREADS];
	HANDLE  threadHandles[MAX_POSSIBLE_THREADS];
	uint32_t maxThreadCount, threadCount;
public:
	void init(uint32_t _maxThreadCount) {
		maxThreadCount = _maxThreadCount;
		threadCount = 0;
		if (maxThreadCount > MAX_POSSIBLE_THREADS)
			throw;
	}
	void addTask(void (*_foo)(void*), void* _data) {
		uint32_t index = threadCount;
		threadHeaders[index] = { _foo, _data };

		threadHandles[index] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			threadFunctionWrapper,	// thread function name
			&threadHeaders[index],	// argument to thread function 
			0,                      // use default creation flags 
			&threadIDs[index]);		// returns the thread identifier 

		threadCount++;
	}

	bool allTasksFinished() {
		WaitForMultipleObjects(threadCount, threadHandles, TRUE, INFINITE);

		threadCount = 0;

		return true;
	}
	uint16_t getFreeThreadCount() {
		return maxThreadCount - threadCount;
	}
private:
	void _clear() {
		
	}
};