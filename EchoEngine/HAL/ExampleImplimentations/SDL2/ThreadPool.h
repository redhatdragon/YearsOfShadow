#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL.h>
#include <memory.h>

struct ThreadHeader {
	void (*func)(void*);
	void* param;
	bool inUse;
	bool terminate;
};

struct ThreadPool {
	struct SDL_Thread** threads;
	struct ThreadHeader* threadHeader;
	uint16_t threadCount;
};

int threadChamber(struct ThreadHeader* threadHeader) {
loop:
	if (threadHeader->func) {
		threadHeader->func(threadHeader->param);
		threadHeader->func = NULL;
		threadHeader->param = NULL;
		//SDL_Delay(1);
		threadHeader->inUse = false;
		goto loop;
	}
	if (threadHeader->terminate)
		goto endLoop;
	goto loop;
	SDL_Delay(1);
endLoop:
	return 0;
}

struct ThreadPool* ThreadPoolCreate(uint16_t maxThreadCount) {
	struct ThreadPool* self = (struct ThreadPool*)malloc(sizeof(struct ThreadPool));
	self->threads = (struct SDL_Thread**)malloc(sizeof(struct SDL_Thread*) * maxThreadCount);
	self->threadHeader = (struct ThreadHeader*)malloc(sizeof(struct ThreadHeader) * maxThreadCount);
	//memset(self->threadHeader, 0, sizeof(struct ThreadHeader) * maxThreadCount);  //Causes some kind of compile error
	for (uint16_t i = 0; i < maxThreadCount; i++) {
		//self->threadHeader[i].func = self->threadHeader[i].inUse = self->threadHeader[i].param = self->threadHeader[i].terminate = NULL;
		memset(&self->threadHeader[i], 0, sizeof(struct ThreadHeader));
		//memset(self, 0, sizeof(struct ThreadPool));  //?
		self->threads[i] = SDL_CreateThread((SDL_ThreadFunction)threadChamber, NULL, &self->threadHeader[i]);
		//SDL_DetachThread(self->threads[i]);
	}
	self->threadCount = maxThreadCount;
	return self;
}
void ThreadPoolDestroy(struct ThreadPool* self) {
	free(self->threads);
	for (uint16_t i = 0; i < self->threadCount; i++)
		self->threadHeader[i].terminate = true;
	for (uint16_t i = 0; i < self->threadCount; i++) {
		int status;
		SDL_WaitThread(self->threads[i], &status);
	}
	free(self->threadHeader);
}
uint16_t ThreadPoolGetFreeThreadCount(struct ThreadPool* self) {
	uint16_t retValue = 0;
	for (uint16_t i = 0; i < self->threadCount; i++) 
		if (self->threadHeader[i].inUse == false)
			retValue++;
	return retValue;
}
bool ThreadPoolHasFreeThread(struct ThreadPool* self) {
	for (uint16_t i = 0; i < self->threadCount; i++)
		if (self->threadHeader[i].inUse == false)
			return true;
	return false;
}
bool ThreadPoolAllThreadsFree(struct ThreadPool* self) {
	return self->threadCount == ThreadPoolGetFreeThreadCount(self);
}
bool ThreadPoolGiveTask(struct ThreadPool* self, void(*func)(void*), void* param) {
	for (uint16_t i = 0; i < self->threadCount; i++)
		if (self->threadHeader[i].inUse == false) {
			self->threadHeader[i].inUse = true;
			self->threadHeader[i].param = param;
			//SDL_Delay(1);  //might be needed, or we might need mutexes to deal with out of order execution
			self->threadHeader[i].func = func;
			return true;
		}
	return false;
}

#ifdef __cplusplus
}
#endif