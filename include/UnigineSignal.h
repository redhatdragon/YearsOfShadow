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
#include "UnigineCallback.h"
#include "UnigineThread.h"

// callbacks
#define UNIGINE_CALLBACK_METODS(NAME, SIGNAL)\
	void *set##NAME##Callback(Unigine::CallbackBase *func) { return SIGNAL.internal_clear_and_add(func); }\
	void *add##NAME##Callback(Unigine::CallbackBase *func)\
	{\
		if (!func) return nullptr;\
		return SIGNAL.add(func);\
	}\
	bool remove##NAME##Callback(void *id) { return SIGNAL.remove(id); }\
	void clear##NAME##Callbacks() { SIGNAL.clear(); }

namespace Unigine
{

class Signal
{
public:
	UNIGINE_INLINE Signal() {}
	UNIGINE_INLINE ~Signal()
	{
		assert(reentrancy_stack.empty());
		assert(garbage.empty());

		for (auto c : callbacks)
			delete c;
	}

	UNIGINE_INLINE void *add(CallbackBase *callback)
	{
		if (!callback)
			return nullptr;

		ScopedReentrantLock lock(mutex);
		return do_add(callback);
	}
	UNIGINE_INLINE bool remove(void *id)
	{
		if (!id)
			return false;

		ScopedReentrantLock lock(mutex);
		return do_remove(id);
	}
	UNIGINE_INLINE void clear()
	{
		ScopedReentrantLock lock(mutex);
		do_clear();
	}

	UNIGINE_INLINE bool empty() const { return callbacks.empty(); }
	UNIGINE_INLINE int size() const { return callbacks.size(); }

	template <typename ...Ts>
	UNIGINE_INLINE void invoke(Ts... args)
	{
		if (callbacks.empty())
			return;

		ScopedReentrantLock lock(mutex);

		// No need to fiddle with all that reentrancy stuff
		// when there are simply no callbacks.
		if (callbacks.empty())
			return;

		ReentrancyFrame frame(callbacks);
		reentrancy_stack.append(&frame);

		while (!frame.isDone())
			frame.runNext<Ts...>(std::move(args)...);

		assert(reentrancy_stack.size());
		reentrancy_stack.removeLast();

		if (reentrancy_stack.empty())
			dispose_garbage();
	}

	UNIGINE_INLINE static void swap(Signal &first, Signal &second)
	{
		assert(first.reentrancy_stack.empty());
		assert(first.garbage.empty());

		assert(second.reentrancy_stack.empty());
		assert(second.garbage.empty());

		first.callbacks.swap(second.callbacks);
	}
	UNIGINE_INLINE friend void swap(Signal &first, Signal &second) { Signal::swap(first, second); }


	// internal metod for backward compatibility.
	UNIGINE_INLINE void *internal_clear_and_add(CallbackBase *callback)
	{
		ScopedReentrantLock lock(mutex);
		do_clear();
		if (!callback)
			return nullptr;
		return do_add(callback);
	}

private:
	Signal(const Signal &) = delete;
	Signal &operator=(const Signal &) = delete;
	Signal(Signal &&) = delete;
	Signal &operator=(Signal &&) = delete;

	class ReentrancyFrame
	{
	public:
		UNIGINE_INLINE ReentrancyFrame(Vector<CallbackBase *> list):
			callbacks(std::move(list))
			, current(0)
		{}


		UNIGINE_INLINE bool isDone() const { return current >= callbacks.size(); }

		template <typename ...Ts>
		UNIGINE_INLINE void runNext()
		{
			assert(current < callbacks.size());
			CallbackBase *callback = callbacks[current];
			callback->run();
			++current;
		}

		template <typename ...Ts>
		UNIGINE_INLINE void runNext(Ts... args)
		{
			assert(current < callbacks.size());
			CallbackBase *callback = callbacks[current];
			callback->run<Ts...>(std::move(args)...);
			++current;
		}

		UNIGINE_INLINE void remove(CallbackBase *callback)
		{
			auto it = callbacks.find(callback);
			if (it == callbacks.end())
				return;

			callbacks.erase(it);

			int index = it - callbacks.begin();
			if (index <= current)
				--current;
		}
		UNIGINE_INLINE void clear() { callbacks.clear();}

	private:
		Vector<CallbackBase *> callbacks;
		int current;
	};

	UNIGINE_INLINE void dispose_garbage()
	{
		for (auto c : garbage)
			delete c;
		garbage.clear();
	}

	UNIGINE_INLINE void *do_add(CallbackBase *callback)
	{
		assert(callback);

		// No special handling for reentrancy scenario.
		// When a callback is added in an another callback called by the same signal,
		// it will be triggered only on the next invocation.
		callbacks.append(callback);
		return static_cast<void *>(callback);
	}
	UNIGINE_INLINE bool do_remove(void *id)
	{
		assert(id);

		CallbackBase *callback = static_cast<CallbackBase *>(id);

		auto it = callbacks.find(callback);
		if (it == callbacks.end())
			return false;

		callbacks.erase(it);

		if (reentrancy_stack.size())
		{
			for (auto frame_ptr : reentrancy_stack)
				frame_ptr->remove(callback);

			// The callback may potentially be currently in use,
			// so we can't destroy it right away.
			garbage.append(callback);
		} else
			delete callback;

		return true;
	}
	UNIGINE_INLINE void do_clear()
	{
		if (reentrancy_stack.size())
		{
			for (auto frame_ptr : reentrancy_stack)
				frame_ptr->clear();

			// Callbacks may potentially be currently in use,
			// so we can't destroy them right away.
			garbage.append(callbacks);
		} else
		{
			for (auto c : callbacks)
				delete c;
		}

		callbacks.clear();
	}

	mutable ReentrantMutex mutex;
	Vector<CallbackBase *> callbacks;
	Vector<CallbackBase *> garbage;
	Vector<ReentrancyFrame *> reentrancy_stack;
};

}
