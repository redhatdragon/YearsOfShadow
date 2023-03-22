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

#include <UnigineTree.h>
#include <UniginePair.h>

namespace Unigine
{

template<typename Key, typename Type, typename Allocator>
struct MapData
{
public:
	template<typename TypeKey, typename ... Args>
	MapData(TypeKey &&key, MapData *p, Args && ... args)
		: parent(p)
		, right(nullptr)
		, left(nullptr)
		, balance(0)
		, key(std::forward<TypeKey>(key))
		, data(std::forward<Args>(args)...)
	{}

	MapData(const MapData &o)
		: parent(nullptr)
		, right(nullptr)
		, left(nullptr)
		, balance(0)
		, key(o.key)
		, data(o.data)
	{}

	MapData(MapData &&o)
		: parent(nullptr)
		, right(nullptr)
		, left(nullptr)
		, balance(0)
		, key(std::move(o.key))
		, data(std::move(o.data))
	{}

	MapData &operator=(const MapData &o) = delete;
	MapData &operator=(MapData &&o) = delete;

	MapData *parent;
	union
	{
		struct
		{
			MapData *right;
			MapData *left;
		};
		MapData *dirs[2];
	};
	int balance;

	const Key key;
	Type data;

	static UNIGINE_INLINE void *operator new(size_t size) { return Allocator::allocate(size); }
	static UNIGINE_INLINE void operator delete(void *ptr) { Allocator::deallocate(ptr); }

	~MapData()
	{
		delete left;
		delete right;
	}

};

template <typename Key, typename Type, typename Allocator = TreeAllocator>
class Map : public Tree<Key, MapData<Key, Type, Allocator>, Allocator>
{

public:

	using Node = typename Unigine::MapData<Key, Type, Allocator>;
	using Data = Node;
	using Parent = Tree<Key, Node, Allocator>;
	using Iterator = typename Parent::Iterator;
	using ConstIterator = typename Parent::ConstIterator;

	using iterator = typename Parent::iterator;
	using const_iterator = typename Parent::const_iterator;

	Map() {}

	Map(std::initializer_list<Pair<Key, Type>> list)
	{
		for (const auto &it : list)
			append(it.first, it.second);
	}

	Map(const Map &o)
	{
		Parent::length = 0;
		Parent::root = nullptr;
		Node *dest_parent = nullptr;
		Parent::copy_proc(Parent::root, dest_parent, o.root);
	}

	Map &operator=(const Map &o)
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

	Map(Map &&o)
	{
		Parent::length = o.length;
		Parent::root = o.root;
		o.length = 0;
		o.root = nullptr;
	}

	Map &operator=(Map &&o)
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

	template<typename T>
	UNIGINE_INLINE bool contains(const T &key) const { return Parent::contains(key); }
	template<typename T>
	UNIGINE_INLINE bool contains(const T &key, const Type &value) const
	{
		Node *node = Parent::do_find(key);
		return node != nullptr && node->data == value;
	}

	UNIGINE_INLINE Iterator findData(const Type &t)
	{
		Iterator end_it = Parent::end();
		for (Iterator it = Parent::begin(); it != end_it; ++it)
		{
			if (it->data == t)
				return it;
		}
		return end_it;
	}
	UNIGINE_INLINE ConstIterator findData(const Type &t) const
	{
		ConstIterator end_it = Parent::end();
		for (ConstIterator it = Parent::begin(); it != end_it; ++it)
		{
			if (it->data == t)
				return it;
		}
		return end_it;
	}

	UNIGINE_INLINE Type &operator[](const Key &key) { return get(key); }
	UNIGINE_INLINE Type &operator[](Key &&key) { return get(std::move(key)); }
	UNIGINE_INLINE const Type &operator[](const Key &key) const { return get(key); }

	UNIGINE_INLINE Type &get(const Key &key) { return Parent::do_append(key)->data; }
	UNIGINE_INLINE const Type &get(const Key &key) const
	{
		Node *node = Parent::do_find(key);
		assert(node != nullptr && "Map::operator[] bad key");
		return node->data;
	}

	UNIGINE_INLINE Type &get(Key &&key) { return Parent::do_append(std::move(key))->data; }

	UNIGINE_INLINE Type &append(const Key &key) { return Parent::do_append(key)->data; }
	UNIGINE_INLINE Type &append(Key &&key) { return Parent::do_append(std::move(key))->data; }

	UNIGINE_INLINE Iterator append(const Key &key, const Type &t)
	{
		Node *node = Parent::do_append(key);
		node->data = t;
		return Iterator(node);
	}

	UNIGINE_INLINE Iterator append(Key &&key, const Type &t)
	{
		Node *node = Parent::do_append(std::move(key));
		node->data = t;
		return Iterator(node);
	}

	UNIGINE_INLINE Iterator append(const Key &key, Type &&t)
	{
		Node *node = Parent::do_append(std::move(key));
		node->data =  std::move(t);
		return Iterator(node);
	}

	UNIGINE_INLINE Iterator append(Key &&key, Type &&t)
	{
		Node *node = Parent::do_append(std::move(key));
		node->data = std::move(t);
		return Iterator(node);
	}

	UNIGINE_INLINE void append(const Map &m)
	{
		ConstIterator end_it = m.end();
		for (ConstIterator it = m.begin(); it != end_it; ++it)
			append(it->key, it->data);
	}

	UNIGINE_INLINE void append(Map &&m)
	{
		Iterator end_it = m.end();
		for (Iterator it = m.begin(); it != end_it; ++it)
			append(std::move(it->key), std::move(it->data));
		m.clear();
	}

	UNIGINE_INLINE Type &insert(const Key &key) { return Parent::do_append(key)->data; }
	UNIGINE_INLINE Type &insert(Key &&key) { return Parent::do_append(std::move(key))->data; }

	UNIGINE_INLINE Iterator insert(const Key &key, const Type &t)
	{
		Node *node = Parent::do_append(key);
		node->data = t;
		return Iterator(node);
	}

	UNIGINE_INLINE Iterator insert(Key &&key, const Type &t)
	{
		Node *node = Parent::do_append(std::move(key));
		node->data = t;
		return Iterator(node);
	}

	UNIGINE_INLINE Iterator insert(const Key &key, Type &&t)
	{
		Node *node = Parent::do_append(std::move(key));
		node->data =  std::move(t);
		return Iterator(node);
	}

	UNIGINE_INLINE Iterator insert(Key &&key, Type &&t)
	{
		Node *node = Parent::do_append(std::move(key));
		node->data = std::move(t);
		return Iterator(node);
	}

	UNIGINE_INLINE void insert(const Map &o) { append(o); }
	UNIGINE_INLINE void insert(Map &&o) { append(std::move(o)); }

	template <typename ... Args>
	UNIGINE_INLINE Type &emplace(const Key &key, Args && ... args)
	{
		Node *node = Parent::do_append(std::move(key));
		node->data = Type(std::forward<Args>(args)...);
		return node->data;
	}

	template <typename ... Args>
	UNIGINE_INLINE Type &emplace(Key &&key, Args && ... args)
	{
		Node *node = Parent::do_append(key);
		node->data = Type(std::forward<Args>(args)...);
		return node->data;
	}

	UNIGINE_INLINE Type take(const Key &key)
	{
		Type ret{};
		take(key, ret);
		return ret;
	}

	UNIGINE_INLINE bool take(const Key &key, Type &ret)
	{
		Node *node = Parent::do_remove(key);
		if (node == nullptr)
			return false;
		ret = std::move(node->data);
		delete node;
		return true;
	}

	UNIGINE_INLINE Type value(const Key &key) const
	{
		Node *node = Parent::do_find(key);
		return node == nullptr ? Type{} : node->data;
	}

	UNIGINE_INLINE Type value(const Key &key, const Type &def) const
	{
		Node *node = Parent::do_find(key);
		return node == nullptr ? def : node->data;
	}
	UNIGINE_INLINE const Type &valueRef(const Key &key, const Type &def) const
	{
		Node *node = Parent::do_find(key);
		return node == nullptr ? def : node->data;
	}

	UNIGINE_INLINE Vector<Type> values() const
	{
		Vector<Type> values;
		getValues(values);
		return values;
	}

	UNIGINE_INLINE void getValues(Vector<Type> &values) const
	{
		values.allocate(values.size() + Parent::length);
		ConstIterator end_it = Parent::end();
		for (ConstIterator it = Parent::begin(); it != end_it; ++it)
			values.appendFast(it->data);
	}

	UNIGINE_INLINE void getPairs(Vector<Pair<Key, Type>> &pairs) const
	{
		pairs.allocate(pairs.size() + Parent::length);
		ConstIterator end_it = Parent::end();
		for (ConstIterator it = Parent::begin(); it != end_it; ++it)
			pairs.appendFast(MakePair(it->key, it->data));
	}

	UNIGINE_INLINE bool operator==(const Map &o) const
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
			if (it0->data != it1->data)
				return false;
		}

		return true;
	}

	UNIGINE_INLINE bool operator!=(const Map &o) const { return !(*this == o); }

};

}
