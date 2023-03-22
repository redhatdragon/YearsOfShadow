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

#include "UnigineTree.h"

namespace Unigine
{
	
template<typename Key, typename Allocator>
struct SetData
{
public:
	SetData(const Key &key, SetData *p)
		: parent(p)
		, right(nullptr)
		, left(nullptr)
		, balance(0)
		, key(key)
	{}

	SetData(Key &&key, SetData *p)
		: parent(p)
		, right(nullptr)
		, left(nullptr)
		, balance(0)
		, key(std::move(key))
	{}

	SetData(const SetData &o)
		: parent(nullptr)
		, right(nullptr)
		, left(nullptr)
		, balance(0)
		, key(o.key)
	{}

	SetData(SetData &&o)
		: parent(nullptr)
		, left(nullptr)
		, right(nullptr)
		, balance(0)
		, key(std::move(o.key))
	{}

	SetData &operator=(const SetData &o) = delete;
	SetData &operator=(SetData &&o) = delete;

	SetData *parent;
	union
	{
		struct
		{
			SetData *right;
			SetData *left;
		};
		SetData *dirs[2];
	};
	int balance;

	const Key key;

	static UNIGINE_INLINE void *operator new(size_t size) { return Allocator::allocate(size); }
	static UNIGINE_INLINE void operator delete(void *ptr) { Allocator::deallocate(ptr); }

	~SetData()
	{
		delete left;
		delete right;
	}

};

template <typename Key, typename Allocator = TreeAllocator>
class Set : public Tree<Key, SetData<Key, Allocator>, Allocator>
{
public:

	using Node = typename Unigine::SetData<Key, Allocator>;
	using Data = Node;
	using Parent = Tree<Key, Node, Allocator>;
	using Iterator = typename Parent::Iterator;
	using ConstIterator = typename Parent::ConstIterator;

	using iterator = typename Parent::iterator;
	using const_iterator = typename Parent::const_iterator;

	Set() {}

	Set(const Set &o)
	{
		Parent::length = 0;
		Parent::root = nullptr;
		Node *dest_parent = nullptr;
		Parent::copy_proc(Parent::root, dest_parent, o.root);
	}

	Set &operator=(const Set &o)
	{
		if (Parent::root == o.root)
			return *this;
		Parent::length = 0;
		delete Parent::root;
		Parent::root = nullptr;
		Node *dest_parent = nullptr;
		Parent::copy_proc(Parent::root, dest_parent, o.root);
		return *this;
	}

	Set(Set &&o)
	{
		Parent::length = o.length;
		Parent::root = o.root;
		o.length = 0;
		o.root = nullptr;
	}

	Set(std::initializer_list<Key> list)
	{
		Parent::length = 0;
		Parent::root = nullptr;
		for (const auto &v : list)
			append(v);
	}

	Set &operator=(Set &&o)
	{
		if (Parent::root == o.root)
			return *this;
		delete Parent::root;
		Parent::root = o.root;
		Parent::length = o.length;
		o.root = nullptr;
		o.length = 0;
		return *this;
	}

	UNIGINE_INLINE Iterator append(const Key &key) { return Iterator(Parent::do_append(key)); }
	UNIGINE_INLINE Iterator append(Key &&key) { return Iterator(Parent::do_append(std::move(key))); }

	UNIGINE_INLINE void append(const Set &s)
	{
		ConstIterator end_it = s.end();
		for (ConstIterator it = s.begin(); it != end_it; ++it)
			append(it->key);
	}

	UNIGINE_INLINE void append(Set &&s)
	{
		Iterator end_it = s.end();
		for (Iterator it = s.begin(); it != end_it; ++it)
			append(std::move(it->key));
		s.clear();
	}

	UNIGINE_INLINE void insert(const Key &key) { Parent::do_append(key); }
	UNIGINE_INLINE void insert(Key &&key) { Parent::do_append(std::move(key)); }
	UNIGINE_INLINE void insert(const Set &o) { append(o); }
	UNIGINE_INLINE void insert(Set &&o) { append(std::move(o)); }

	UNIGINE_INLINE bool operator==(const Set &o) const
	{
		if (Parent::length != o.length)
			return false;
		auto it0 = Parent::cbegin();
		auto it0_end = Parent::cend();

		auto it1 = o.cbegin();
		auto it1_end = o.cend();

		for (;it0 != it0_end && it1 != it1_end; ++it0, ++it1)
		{
			if (it0->key != it1->key)
				return false;
		}

		return true;
	}

	UNIGINE_INLINE bool operator!=(const Set &o) const { return !(*this == o); }
};

}
