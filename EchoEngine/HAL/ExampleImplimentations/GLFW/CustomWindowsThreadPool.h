#pragma once
#include <vector>
#include <Windows.h>
#include <iostream>

struct ThreadHeader {
	void (*foo)(void*);
	void* data;
};

class CustomWindowsThread {
	ThreadHeader header;
	HANDLE threadHandle;
	DWORD threadID;

public:
	void init() {
		clear();

		threadHandle = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			threadLoop,				// thread function
			this,					// argument to thread function 
			0,                      // use default creation flags 
			&threadID);				// returns the thread identifier 

		SetThreadPriority(threadHandle, THREAD_PRIORITY_HIGHEST);
	}

	void run() {
		header.foo(header.data);
	}

	void setTask(void (*_foo)(void*), void* _data) {
		header.foo = _foo;
		header.data = _data;
	}
	bool isActive() {
		if (header.foo && header.data)
			return true;
		return false;
	}
	bool isFinished() {
		if (header.foo == nullptr && header.data == nullptr)
			return true;
		return false;
	}

private:
	static DWORD WINAPI threadLoop(void* _customWindowsThread) {
		CustomWindowsThread* thread = (CustomWindowsThread*)_customWindowsThread;

		loopStart:
		if (thread->isActive()) {
			thread->run();
			thread->clear();
		}
		goto loopStart;

		return 0;
	}
	void clear() {
		header.foo = nullptr;
		header.data = nullptr;
	}
};



class CustomWindowsThreadPool {
	static constexpr uint32_t MAX_POSSIBLE_THREADS = 256;
	CustomWindowsThread threads[MAX_POSSIBLE_THREADS];
	uint32_t maxThreadCount;
public:
	void init(uint32_t _maxThreadCount) {
		maxThreadCount = _maxThreadCount;
		for (uint32_t i = 0; i < _maxThreadCount; i++) {
			threads[i].init();
		}
	}
	void addTask(void (*_foo)(void*), void* _data) {
		uint16_t activeThreadCount = 0;
		for (uint32_t i = 0; i < maxThreadCount; i++)
			if (threads[i].isFinished() == false)
				activeThreadCount++;
		if (activeThreadCount >= maxThreadCount)
			throw;
		threads[activeThreadCount].setTask(_foo, _data);
	}

	bool allTasksFinished() {
		for (uint32_t i = 0; i < maxThreadCount; i++)
			if (threads[i].isFinished() == false)
				return false;
		return true;
	}
	uint16_t getFreeThreadCount() {
		uint16_t count = 0;
		for (uint32_t i = 0; i < maxThreadCount; i++)
			if (threads[i].isFinished() == true)
				count++;
		return count;
	}
};



//DWORD WINAPI threadFunctionWrapper(void* _threadHeader) {
//	ThreadHeader* threadHeader = (ThreadHeader*)_threadHeader;
//	threadHeader->foo(threadHeader->data);
//	return 0;
//}

//class CustomWindowsThreadPool {
//	static constexpr uint32_t MAX_POSSIBLE_THREADS = 256;
//	//void* datas[MAX_POSSIBLE_THREADS];
//	ThreadHeader threadHeaders[MAX_POSSIBLE_THREADS];
//	DWORD   threadIDs[MAX_POSSIBLE_THREADS];
//	HANDLE  threadHandles[MAX_POSSIBLE_THREADS];
//	uint32_t maxThreadCount, threadCount;
//public:
//	void init(uint32_t _maxThreadCount) {
//		maxThreadCount = _maxThreadCount;
//		threadCount = 0;
//		if (maxThreadCount > MAX_POSSIBLE_THREADS)
//			throw;
//	}
//	void addTask(void (*_foo)(void*), void* _data) {
//		uint32_t index = threadCount;
//		threadHeaders[index] = { _foo, _data };
//
//		threadHandles[index] = CreateThread(
//			NULL,                   // default security attributes
//			0,                      // use default stack size  
//			threadFunctionWrapper,	// thread function name
//			&threadHeaders[index],	// argument to thread function 
//			0,                      // use default creation flags 
//			&threadIDs[index]);		// returns the thread identifier 
//
//		threadCount++;
//	}
//
//	bool allTasksFinished() {
//		WaitForMultipleObjects(threadCount, threadHandles, TRUE, INFINITE);
//
//		threadCount = 0;
//
//		return true;
//	}
//	uint16_t getFreeThreadCount() {
//		return maxThreadCount - threadCount;
//	}
//private:
//};