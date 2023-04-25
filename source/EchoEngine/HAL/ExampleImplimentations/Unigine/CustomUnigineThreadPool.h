#pragma once
#include <UnigineObjects.h>
#include <vector>

class CustomUnigineThread : public Unigine::Thread {
	void (*foo)(void*);
	void* data;
	bool finished;
	void process() override {
		foo(data);
		finished = true;
		signal();
	}
public:
	void giveTask(void (*_foo)(void*), void* _data) {
		foo = _foo;
		data = _data;
		finished = false;
	}
	bool isFinished() {
		return finished;
	}
};

class CustomUnigineThreadPool {
	std::vector<CustomUnigineThread*> threads;
	uint16_t maxThreadCount;
public:
	void init(uint32_t _maxThreadCount) {
		threads = {};
		threads.reserve(_maxThreadCount);
		maxThreadCount = _maxThreadCount;
	}
	void addTask(void (*_foo)(void*), void* _data) {
		CustomUnigineThread* thread = new CustomUnigineThread();
		thread->giveTask(_foo, _data);
		threads.push_back(thread);
		thread->run();
	}

	bool allTasksFinished() {
		for (uint32_t i = 0; i < threads.size(); i++)
			if (threads[i]->isFinished() == false)
				return false;
		_clear();
		return true;
	}
	uint16_t getFreeThreadCount() {
		return maxThreadCount - threads.size();
	}
private:
	void _clear() {
		for (uint32_t i = 0; i < threads.size(); i++) {
			delete threads[i];
			//threads[i] = nullptr;
		}
		threads.resize(0);
		//threads.clear();
	}
};