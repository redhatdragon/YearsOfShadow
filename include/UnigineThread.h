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
#include "UnigineVector.h"
#include <UnigineTimer.h>

#ifdef _WIN32
	#include <intrin.h>
#elif _LINUX
	#include <pthread.h>
#else
	#error "Platform unsupported"
#endif

#include <xmmintrin.h>
#include <stdint.h>

namespace Unigine
{

//////////////////////////////////////////////////////////////////////////
/// Atomics and locks.
//////////////////////////////////////////////////////////////////////////

/// Atomic CAS (Compare-And-Swap), 8-bit.
UNIGINE_INLINE bool AtomicCAS(volatile char *ptr, char old_value, char new_value)
{
	assert((((size_t)ptr) % (sizeof(char))) == 0 && "unaligned atomic!");
	#ifdef _WIN32
		return (_InterlockedCompareExchange8(ptr, new_value, old_value) == old_value);
	#elif _LINUX
		return (__sync_val_compare_and_swap(ptr, old_value, new_value) == old_value);
	#endif
}

/// Atomic CAS (Compare-And-Swap), 16-bit.
UNIGINE_INLINE bool AtomicCAS(volatile short *ptr, short old_value, short new_value)
{
	assert((((size_t)ptr) % (sizeof(short))) == 0 && "unaligned atomic!");
	#ifdef _WIN32
		return (_InterlockedCompareExchange16(ptr, new_value, old_value) == old_value);
	#elif _LINUX
		return (__sync_val_compare_and_swap(ptr, old_value, new_value) == old_value);
	#endif
}

/// Atomic CAS (Compare-And-Swap), 32-bit.
UNIGINE_INLINE bool AtomicCAS(volatile int *ptr, int old_value, int new_value)
{
	assert((((size_t)ptr) % (sizeof(int))) == 0 && "unaligned atomic!");
	#ifdef _WIN32
		return (_InterlockedCompareExchange((volatile long *)ptr, new_value, old_value) == old_value);
	#elif _LINUX
		return (__sync_val_compare_and_swap(ptr, old_value, new_value) == old_value);
	#endif
}

/// Atomic CAS (Compare-And-Swap), 64-bit.
UNIGINE_INLINE bool AtomicCAS(volatile long long *ptr, long long old_value, long long new_value)
{
	assert((((size_t)ptr) % (sizeof(int))) == 0 && "unaligned atomic!");
	#ifdef _WIN32
		return (_InterlockedCompareExchange64(ptr, new_value, old_value) == old_value);
	#elif _LINUX
		return (__sync_val_compare_and_swap(ptr, old_value, new_value) == old_value);
	#endif
}
/// Atomic CAS (Compare-And-Swap), pointer.
UNIGINE_INLINE bool AtomicCAS(void *volatile * ptr, void *old_value, void *new_value)
{
	assert((((size_t)ptr) % (sizeof(int))) == 0 && "unaligned atomic!");
	#ifdef _WIN32
		return (_InterlockedCompareExchangePointer(ptr, new_value, old_value) == old_value);
	#elif _LINUX
		return (__sync_val_compare_and_swap(ptr, old_value, new_value) == old_value);
	#endif
}

/// Atomic add, 8-bit.
/// Returns the previous value (just before adding).
UNIGINE_INLINE char AtomicAdd(volatile char *ptr, char value)
{
	assert((((size_t)ptr) % (sizeof(char))) == 0 && "unaligned atomic!");
	#ifdef _WIN32
		return _InterlockedExchangeAdd8(ptr, value);
	#else
		return __sync_fetch_and_add(ptr, value);
	#endif
}

/// Atomic add, 16-bit.
/// Returns the previous value (just before adding).
UNIGINE_INLINE short AtomicAdd(volatile short *ptr, short value)
{
	assert((((size_t)ptr) % (sizeof(short))) == 0 && "unaligned atomic!");
	#ifdef _WIN32
		return _InterlockedExchangeAdd16(ptr, value);
	#else
		return __sync_fetch_and_add(ptr, value);
	#endif
}

/// Atomic add, 32-bit.
/// Returns the previous value (just before adding).
UNIGINE_INLINE int AtomicAdd(volatile int *ptr, int value)
{
	assert((((size_t)ptr) % (sizeof(int))) == 0 && "unaligned atomic!");
	#ifdef _WIN32
		return _InterlockedExchangeAdd((volatile long *)ptr, (long)value);
	#else
		return __sync_fetch_and_add(ptr, value);
	#endif
}

/// Atomic add, 64-bit.
/// Returns the previous value (just before adding).
UNIGINE_INLINE long long AtomicAdd(volatile long long *ptr, long long value)
{
	assert((((size_t)ptr) % (sizeof(int))) == 0 && "unaligned atomic!");
	#ifdef _WIN32
		return _InterlockedExchangeAdd64((volatile long long *)ptr, (long long)value);
	#else
		return __sync_fetch_and_add(ptr, value);
	#endif
}

/// Atomic read, 8-bit.
/// Because simply accessing the variable directly is actually unsafe!
UNIGINE_INLINE char AtomicGet(volatile char *ptr)
{
	return AtomicAdd(ptr, 0);
}

/// Atomic read, 16-bit.
/// Because simply accessing the variable directly is actually unsafe!
UNIGINE_INLINE short AtomicGet(volatile short *ptr)
{
	return AtomicAdd(ptr, 0);
}

/// Atomic read, 32-bit.
/// Because simply accessing the variable directly is actually unsafe!
UNIGINE_INLINE int AtomicGet(volatile int *ptr)
{
	return AtomicAdd(ptr, 0);
}

/// Atomic read, 64-bit.
/// Because simply accessing the variable directly is actually unsafe!
UNIGINE_INLINE long long AtomicGet(volatile long long *ptr)
{
	return AtomicAdd(ptr, 0);
}

/// Atomic set, 8-bit.
/// Returns the previous value.
UNIGINE_INLINE char AtomicSet(volatile char *ptr, char value)
{
	for (;;)
	{
		char old = AtomicAdd(ptr, 0);
		if (AtomicCAS(ptr, old, value))
			return old;
	}
}

/// Atomic set, 16-bit.
/// Returns the previous value.
UNIGINE_INLINE short AtomicSet(volatile short *ptr, short value)
{
	for (;;)
	{
		short old = AtomicAdd(ptr, 0);
		if (AtomicCAS(ptr, old, value))
			return old;
	}
}

/// Atomic set, 32-bit.
/// Returns the previous value.
UNIGINE_INLINE int AtomicSet(volatile int *ptr, int value)
{
	for (;;)
	{
		int old = AtomicAdd(ptr, 0);
		if (AtomicCAS(ptr, old, value))
			return old;
	}
}

/// Atomic set, 64-bit.
/// Returns the previous value.
UNIGINE_INLINE long long AtomicSet(volatile long long *ptr, long long value)
{
	for (;;)
	{
		long long old = AtomicAdd(ptr, 0);
		if (AtomicCAS(ptr, old, value))
			return old;
	}
}

/// Thread wrapper class.
class UNIGINE_API Thread
{
public:
	Thread();
	virtual ~Thread();

	// Runs the thread.
	// size argument accepts thread stack size in bytes.
	// Returns 1 if the thread was successfully run; otherwise, 0 is returned.
	virtual int run(size_t size = 0x100000);

	// Stops the thread.
	// Returns 1 if the operation was a success; otherwise, 0 is returned.
	int stop();

	// Signals the thread to wake up.
	void signal();

	// Terminates the thread.
	// Returns 1 if the operation was a success; otherwise, 0 is returned.
	int terminate();

	// Checks if the thread is running.
	// Returns 1 if the thread is running; otherwise, 0 is returned.
	int isRunning() const { return running; }

	// Checks if the thread is waiting.
	// Returns 1 if the thread is waiting; otherwise, 0 is returned.
	int isWaiting() const { return is_waiting && (AtomicGet(&need_wait) == 1); }

	// Sets the thread priority.
	// Acceptable priority values are in the range [-3;3].
	// The default thread priority is 0.
	// Returns 1 if the operation was a success; otherwise, 0 is returned.
	int setPriority(int priority);

	// Returns the thread priority in range [-3;3].
	int getPriority() const;

	// Returns a platform specific thread
	auto getThread() const { return thread; }

	// Sets thread's affinity mask
	int setAffinityMask(long long mask);

	// Returns thread's affinity mask
	// returns -1 if mask is not set.
	long long getAffinityMask() const;

	#ifndef _WIN32
		auto getThreadMutex() const { return mutex; }
	#endif

	#ifdef _WIN32
		typedef unsigned long ID;
	#else
		typedef pthread_t ID;
	#endif
	// Returns the current thread identifier.
	static ID getID();

	// Suspends thread execution.
	// Timeout value is provided in milliseconds.
	static void sleep(unsigned int ms);

	static void switchThread();

protected:
	// Thread process function.
	virtual void process() = 0;

	// Puts the thread into the waiting state.
	void wait(int force);

	#ifdef _WIN32
		static unsigned long __stdcall thread_handler(void *data);
		void *thread;
	#else
		static void *thread_handler(void *data);
		pthread_t thread;
		pthread_mutex_t mutex;
	#endif

	long long affinity_mask;

	volatile bool running{false};
	mutable int priority;

private:

	struct WaitVariable;
	WaitVariable *wait_variable{nullptr};
	mutable volatile int need_wait{false};
	volatile int is_waiting{false};

	volatile int clean_stopped; // set to 1 by thread_handler() on clean exit from process()
};

class BackoffSpinner
{
public:
	void spin()
	{
		if (backoff < 10)
			_mm_pause();
		else if (backoff < 20)
			for (int i = 0; i < 50; ++i) _mm_pause();
		else if (backoff < 22)
			for (int i = 0; i < 100; ++i) _mm_pause();
		else
		{
			Thread::switchThread();
			return;
		}

		++backoff;
	}
private:
	int backoff{0};
};

/// Spinlock, based on atomic CAS.
/// Now with unlocked dirty reads, proper waits, and exponential backoff.
UNIGINE_INLINE void SpinLock(volatile int *ptr, int old_value, int new_value)
{
	const int TRIES = 3;
	BackoffSpinner spinner;
	while (true)
	{
		for (int i = 0; i < TRIES; i++)
		{
			if (*ptr == old_value && AtomicCAS(ptr, old_value, new_value))
				return;
		}
		spinner.spin();
	}
}

/// Waits for, but does not acquire the lock.
UNIGINE_INLINE void WaitLock(volatile int *ptr, int value)
{
	const int TRIES = 3;
	BackoffSpinner spinner;
	while (true)
	{
		for (int i = 0; i < TRIES; i++)
			if (*ptr == value && AtomicCAS(ptr, value, value))
				return;
		spinner.spin();
	}
}

/// Simple mutex, based on atomic CAS.
class UNIGINE_API Mutex
{
public:
	Mutex(): locked(0) {}
	void lock() { SpinLock(&locked, 0, 1); }
	void unlock() { AtomicSet(&locked, 0); }
	bool isLocked() const { return AtomicGet(&locked) != 0; }
	void wait() { WaitLock(&locked, 0); }
private:
	mutable volatile int locked;
};

/// Scoped lock, based on simple mutex.
class ScopedLock
{
public:
	ScopedLock(Mutex &m) : mutex(m) { mutex.lock(); }
	~ScopedLock() { mutex.unlock(); }
private:
	Mutex &mutex;
};

/// Write-preferring readers-writer mutex.
class UNIGINE_API RWMutex
{
public:
	RWMutex(): readers(0), writer(0) {}

	void lockRead()
	{
		BackoffSpinner spinner;
		while (true)
		{
			while (writer)
				spinner.spin();
			AtomicAdd(&readers, 1);
			if (AtomicGet(&writer) == 0)
				break;
			AtomicAdd(&readers, -1);
		}
	}
	void unlockRead() { AtomicAdd(&readers, -1); }

	void lockWrite()
	{
		BackoffSpinner spinner;
		while (true)
		{
			if (writer == 0 && AtomicCAS(&writer, 0, 1))
			{
				while (readers || AtomicGet(&readers))
					spinner.spin();
				break;
			}
			spinner.spin();
		}
	}
	void unlockWrite() { AtomicSet(&writer, 0); }

private:
	mutable volatile int readers;
	mutable volatile int writer;
};

/// Scoped reader lock, based on RW-mutex.
class ScopedReaderLock
{
public:
	ScopedReaderLock(RWMutex &l) : mutex(l) { mutex.lockRead(); }
	~ScopedReaderLock() { mutex.unlockRead(); }
private:
	RWMutex &mutex;
};

/// Scoped writer lock, based on RW-mutex.
class ScopedWriterLock
{
public:
	ScopedWriterLock(RWMutex &l) : mutex(l) { mutex.lockWrite(); }
	~ScopedWriterLock() { mutex.unlockWrite(); }
private:
	RWMutex &mutex;
};

// TODO: implement lock-free specialization for types
// with sizeof(T) <= sizeof(int64), for which we have
// built-in atomic primitives.
template <typename T>
class Atomic
{
public:
	Atomic() = default;
	explicit Atomic(const T &v): value(v) {}

	T load()
	{
		ScopedLock lock(mutex);
		return value;
	}

	void store(const T &v)
	{
		ScopedLock lock(mutex);
		value = v;
	}

private:
	// TODO: implement copy and move related members.
	Atomic(Atomic &&) = delete;
	Atomic &operator=(Atomic &&) = delete;
	Atomic(const Atomic &) = delete;
	Atomic &operator=(const Atomic &) = delete;

private:
	T value;
	Mutex mutex;
};

/// Reentrant mutex.
/// May be locked multiply times by the same thread.
/// Must be unlocked corresponding number of times.
class UNIGINE_API ReentrantMutex
{
public:
	ReentrantMutex() : thread_id(LLONG_MAX), depth(0) {}

	void lock();
	void unlock();
	bool isLocked() const { return mutex.isLocked(); }
	void wait() { mutex.wait(); }

private:
	
	volatile long long thread_id;
	volatile int depth;
	Mutex mutex;
};

/// Reentrant scoped lock, based on reentrant mutex.
class ScopedReentrantLock
{
public:
	ScopedReentrantLock(ReentrantMutex &m): mutex(m)
	{
		mutex.lock();
	}

	~ScopedReentrantLock() { mutex.unlock(); }
private:
	ReentrantMutex &mutex;
};

////////////////////////////////////////////////////////////////////////////////
/// CPUShader
////////////////////////////////////////////////////////////////////////////////

class UNIGINE_API CPUShader;

class UNIGINE_API PoolCPUShaders
{
public:
	enum
	{
		MAX_THREADS = 64
	};

	// check CPU runtime
	static int isInitialized();

	// get CPU parameters
	static int getNumSyncThreads();
	static int getNumAsyncThreads();

	// check CPU status
	static int isRunning();
	static void wait();

public:  // only internal methods

	static int internal_init();
	static int internal_shutdown();

private:
	friend CPUShader;

	struct CPUThreadShader
	{
		constexpr CPUThreadShader(): id(-1), shader(nullptr) {}
		constexpr CPUThreadShader(int id_, CPUShader *shader_): id(id_), shader(shader_) {}

		int id{-1};
		CPUShader *shader{nullptr};
	};
	struct CPUThreadQueue;
	class CPUThread;

	static void run_sync(CPUShader *shader);
	static void run_async(CPUShader *shader);
	static void signal_queue(CPUThreadQueue *queue);
	static void internal_dec_active_threads();

	static CPUThread *sync_threads[MAX_THREADS];
	static CPUThread *async_threads[MAX_THREADS];

	static CPUThreadQueue *queue_sync;
	static CPUThreadQueue *queue_async;

	static int is_initialized;
	static int num_sync_threads;
	static int num_async_threads;
};

class UNIGINE_API CPUShader
{
	friend class PoolCPUShaders;
public:
	CPUShader() {};
	virtual ~CPUShader() { wait(); };

	void runSync(int threads_count = -1);
	void runAsync(int threads_count = -1);

	void wait();

	int isRunning() const { return AtomicGet(&num_active_threads) != 0; }
	int getNumThreads() { return num_threads; }

	virtual void process(int thread_num, int threads_count) = 0;

private:
	void internal_dec_active_threads();
	void set_current_queue(PoolCPUShaders::CPUThreadQueue *queue) { current_queue = queue; }

private:
	mutable volatile int num_active_threads{ 0 };
	int num_threads{ 0 };
	PoolCPUShaders::CPUThreadQueue* current_queue{nullptr};
};

template<typename State, typename Process, typename Destroy>
class CPUShaderCallable: public CPUShader
{
public:
	CPUShaderCallable(Process func_process_, Destroy func_destroy_)
		: func_process(func_process_)
		, func_destroy(func_destroy_)
		, state()
	{}
	~CPUShaderCallable() override { func_destroy(state); }
	void process(int thread_num, int threads_count) override { func_process(this, thread_num, threads_count); }

private:
	Process func_process;
	Destroy func_destroy;
	State state;
};

template<typename Process>
class CPUShaderCallableStateless: public CPUShader
{
public:
	CPUShaderCallableStateless(Process func_process_)
		: func_process(func_process_)
	{}
	void process(int thread_num, int threads_count) override
	{
		func_process(this, thread_num, threads_count);
	}

private:
	Process func_process;
};

template <typename State, typename Process, typename Destroy>
CPUShader *makeCPUShader(Process func_process, Destroy func_destroy)
{
	return new CPUShaderCallable<State, Process, Destroy>(std::move(func_process), std::move(func_destroy));
}

template <typename State, typename Process>
CPUShader *makeCPUShader(Process func_process)
{
	const auto dummy_destroy = [](const State &) {};
	return new CPUShaderCallable<State, Process, decltype(dummy_destroy)>(std::move(func_process), dummy_destroy);
}

template <typename Process>
CPUShader *makeCPUShaderStateless(Process func_process)
{
	return new CPUShaderCallableStateless<Process>(std::move(func_process));
}

} /* namespace Unigine */
