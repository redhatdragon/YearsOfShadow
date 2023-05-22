#pragma once
//#include <UnigineObjects.h>
#include <UnigineEngine.h>
#include <UnigineLogic.h>
#include <UnigineThread.h>
#include <vector>

#include "miniSleep.h"

class CustomUnigineThread : public Unigine::Thread {
	void (*foo)(void*);
	void* data;
	//mutable Unigine::Mutex lock;
protected:
	void process() override {
		while (isRunning()) {
			if (isActive() == false) {
				miniSleep(1.0f);
				continue;
			}
			foo(data);
			foo = nullptr; data = nullptr;
		}
	}
public:
	void init() {
		foo = nullptr; data = nullptr;
	}
	void giveTask(void (*_foo)(void*), void* _data) {
		foo = _foo;
		data = _data;
	}
	bool isActive() {
		if (foo && data)
			return true;
		return false;
	}
};

class CustomUnigineThreadPool {
	CustomUnigineThread* threads[256];
	uint16_t maxThreadCount;
public:
	void init(uint16_t _maxThreadCount) {
		maxThreadCount = _maxThreadCount;
		for (uint32_t i = 0; i < _maxThreadCount; i++) {
			threads[i] = new CustomUnigineThread();
			threads[i]->init();
			threads[i]->setPriority(3);
			threads[i]->run();
		}
	}
	void addTask(void (*_foo)(void*), void* _data) {
		for (uint32_t i = 0; i < maxThreadCount; i++) {
			if (threads[i]->isActive() == false) {
				threads[i]->giveTask(_foo, _data);
				return;
			}
		}
		throw;
	}

	bool allTasksFinished() {
		for (uint32_t i = 0; i < maxThreadCount; i++)
			if (threads[i]->isActive())
				return false;
		return true;
	}
	uint16_t getFreeThreadCount() {
		uint32_t count = 0;
		for (uint32_t i = 0; i < maxThreadCount; i++)
			if (threads[i]->isActive() == false)
				count++;
		return count;
	}
private:
};