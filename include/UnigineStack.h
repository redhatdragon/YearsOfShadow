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

struct StackErrorHandlerEmpty
{
	inline static void readingOutOfRange() { }
	inline static void resizingOutOfRange() { }
	inline static void overflow() { }
	inline static void underflow() { }
};

template <class Type, int Capacity, typename Counter = short, typename ErrorHandler = StackErrorHandlerEmpty>
class Stack
{
public:
	inline Stack() : depth(-1) {}
	inline ~Stack() {}

	inline Type &get(int index)
	{
		if (index > depth)
			ErrorHandler::readingOutOfRange();

		#ifndef NDEBUG
			assert(index <= depth && index >= 0 && "Stack::get(): bad index");
		#endif
		return stack[depth - index];
	}
	inline const Type &get(int index) const
	{
		if (index > depth)
			ErrorHandler::readingOutOfRange();

		#ifndef NDEBUG
			assert(index <= depth && index >= 0 && "Stack::get(): bad index");
		#endif
		return stack[depth - index];
	}

	inline Type *get() { return stack; }
	inline const Type *get() const { return stack; }
	inline Counter size() const { return depth + 1; }
	inline bool empty() const { return depth == -1; }
	inline int space() const { return Capacity; }
	inline void resize(Counter size)
	{
		if (size > Capacity)
			ErrorHandler::resizingOutOfRange();

		#ifndef NDEBUG
			assert(size <= Capacity && size >= 0 && "Stack::resize(): bad size");
		#endif
		depth = size - 1;
	}
	inline void append(int num)
	{
		if (depth + num + 1 >= Capacity)
			ErrorHandler::overflow();

		#ifndef NDEBUG
			assert(depth + num + 1 < Capacity && "Stack::append(): stack overflow");
		#endif
		depth += num;
	}
	inline void remove(Counter num)
	{
		if (depth - num + 1 < 0)
			ErrorHandler::underflow();

		#ifndef NDEBUG
			assert(depth - num + 1 >= 0 && "Stack::remove(): stack underflow");
		#endif
		depth -= num;
	}
	inline void remove()
	{
		if (depth < 0)
			ErrorHandler::underflow();

		#ifndef NDEBUG
			assert(depth >= 0 && "Stack::remove(): stack underflow");
		#endif
		depth--;
	}
	inline int find(const Type &value) const
	{
		for (int i = depth; i >= 0; i--)
		{
			if (stack[i] == value)
				return i;
		}
		return -1;
	}

	inline void clear() { depth = -1; }

	inline Type &top()
	{
		if (depth < 0)
			ErrorHandler::underflow();

		#ifndef NDEBUG
			assert(depth >= 0 && "Stack::top(): stack underflow");
		#endif
		return stack[depth];
	}
	inline const Type &top() const
	{
		if (depth < 0)
			ErrorHandler::underflow();

		#ifndef NDEBUG
			assert(depth >= 0 && "Stack::top(): stack underflow");
		#endif
		return stack[depth];
	}
	inline Type &pop()
	{
		if (depth < 0)
			ErrorHandler::underflow();

		#ifndef NDEBUG
			assert(depth >= 0 && "Stack::pop(): stack underflow");
		#endif
		return stack[depth--];
	}
	inline void push(const Type &t)
	{
		if (depth + 1 >= Capacity)
			ErrorHandler::overflow();

		#ifndef NDEBUG
			assert(depth + 1 < Capacity && "Stack::push(): stack overflow");
		#endif
		stack[++depth] = t;
	}
	inline Type &push()
	{
		if (depth + 1 >= Capacity)
			ErrorHandler::overflow();

		#ifndef NDEBUG
			assert(depth + 1 < Capacity && "Stack::push(): stack overflow");
		#endif
		return stack[++depth];
	}

private:
	Counter depth;
	Type stack[Capacity];
};

}