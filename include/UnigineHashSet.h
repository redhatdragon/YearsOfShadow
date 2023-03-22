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

#include <UnigineHash.h>

namespace Unigine
{

template<typename Key, typename HashType>
struct HashSetData
{
	const HashType hash;
	const Key key;
	HashSetData(HashType h, const Key &k)
		: hash(h)
		, key(k)
	{}
	HashSetData(HashType h, Key &&k)
		: hash(h)
		, key(std::move(k))
	{}

	static UNIGINE_INLINE void *operator new(size_t size) { return Memory::allocate(size); }
	static UNIGINE_INLINE void operator delete(void *ptr) { Memory::deallocate(ptr); }
	static UNIGINE_INLINE void operator delete(void *ptr, size_t size) { Memory::deallocate(ptr,size); }

};

template <typename Key, typename Counter = unsigned int>
class HashSet : public Hash<Key, HashSetData<Key, typename Hasher<Key>::HashType>, typename Hasher<Key>::HashType, Counter>
{
public:

	using HashType = typename Hasher<Key>::HashType;
	using Parent = Hash<Key, HashSetData<Key, typename Hasher<Key>::HashType>, typename Hasher<Key>::HashType, Counter>;
	using Iterator = typename Parent::Iterator;
	using ConstIterator = typename Parent::ConstIterator;

    // STL compatibility
	using iterator = typename Parent::iterator;
	using const_iterator = typename Parent::const_iterator;
    using value_type = Key;

	HashSet() noexcept
	{
		Parent::data = nullptr;
		Parent::length = 0;
		Parent::capacity = 0;
	}

	~HashSet()
	{
		if (Parent::data == nullptr)
			return;
		for (Counter i = 0; i < Parent::capacity; ++i)
			delete Parent::data[i];

		Memory::deallocate(Parent::data);
	}

	HashSet(const HashSet &o)
	{
		Parent::data = nullptr;
		Parent::length = 0;
		Parent::capacity = 0;
		Parent::rehash(o.capacity);
		for (const auto &it : o)
			Parent::do_append(it.hash, it.key);
	}

	HashSet(HashSet &&o) noexcept
	{
		Parent::length = o.length;
		Parent::capacity = o.capacity;
		Parent::data = o.data;

		o.length = 0;
		o.capacity = 0;
		o.data = nullptr;
	}

	HashSet(std::initializer_list<Key> list)
	{
		Parent::data = nullptr;
		Parent::length = 0;
		Parent::capacity = 0;
		for (const auto &v : list)
			append(v);
	}

	HashSet &operator=(const HashSet &o)
	{
		if (this == &o)
			return *this;
		Parent::clear();
		Parent::reserve(static_cast<Counter>(o.capacity * HASH_LOAD_FACTOR));
		for (const auto &it : o)
			Parent::do_append(it.hash, it.key);
		return *this;
	}

	HashSet &operator=(HashSet &&o)
	{
		if (this == &o)
			return *this;
		for (Counter i = 0; i < Parent::capacity; ++i)
			delete Parent::data[i];

		Memory::deallocate(Parent::data);
		Parent::length = o.length;
		Parent::capacity = o.capacity;
		Parent::data = o.data;

		o.length = 0;
		o.capacity = 0;
		o.data = nullptr;
		return *this;
	}

	UNIGINE_INLINE void append(const Key &key) { Parent::do_append(key); }
	UNIGINE_INLINE void append(Key &&key) { Parent::do_append(std::move(key)); }

	UNIGINE_INLINE void append(const HashSet &o)
	{
		if (&o == this)
			return;

		for (const auto &it : o)
			Parent::do_append(it.hash, it.key);
	}

	UNIGINE_INLINE void append(HashSet &&o)
	{
		if (&o == this)
			return;

		for (const auto &it : o)
			Parent::do_append(it.hash, std::move(it.key));
		o.clear();
	}

	using Parent::remove;

	UNIGINE_INLINE void remove(const HashSet &o)
	{
		if (&o == this)
			Parent::clear();
		else
		{
			for (Counter i = 0; i < o.capacity; ++i)
			{
				if (o.data[i] == nullptr)
					continue;
				Parent::do_remove(o.data[i]->hash, o.data[i]->key);
			}
		}
	}

	UNIGINE_INLINE void insert(const Key &key) { Parent::do_append(key); }
	UNIGINE_INLINE void insert(Key &&key) { Parent::do_append(std::move(key)); }
	UNIGINE_INLINE void insert(const HashSet &o) { append(o); }
	UNIGINE_INLINE void insert(HashSet &&o) { append(std::move(o)); }

	UNIGINE_INLINE void subtract(const HashSet &o) { remove(o); }

	UNIGINE_INLINE bool operator==(const HashSet &o) const noexcept
	{
		if (&o == this)
			return true;

		if (Parent::length != o.length)
			return false;

		for (Counter i = 0; i < Parent::capacity; ++i)
		{
			if (Parent::data[i] == nullptr)
				continue;

			auto other_data = o.do_find(Parent::data[i]->key);
			if (other_data == nullptr)
				return false;
		}

		return true;
	}

	UNIGINE_INLINE bool operator!=(const HashSet &o) const noexcept { return !(*this == o); }

	UNIGINE_INLINE HashSet &operator+=(const Key &key) { insert(key); return *this; }
	UNIGINE_INLINE HashSet &operator+=(const HashSet &o) { insert(o); return *this; }
	UNIGINE_INLINE HashSet &operator-=(const Key &key) { Parent::remove(key); return *this; }
	UNIGINE_INLINE HashSet &operator-=(const HashSet &o) { remove(o); return *this; }

	UNIGINE_INLINE static HashSet fromKeys(const Key *keys, size_t size)
	{
		HashSet result;
		result.reserve(static_cast<Counter>(size));
		for (size_t i = 0; i < size; ++i)
			result.append(keys[i]);
		return result;
	}

	UNIGINE_INLINE static HashSet fromKeys(const Vector<Key> &keys)
	{
		HashSet result;
		result.reserve(keys.size());
		for (size_t i = 0, count = keys.size(); i < count; ++i)
			result.append(keys[i]);
		return result;
	}

	UNIGINE_INLINE static HashSet fromKeys(Vector<Key> &&keys)
	{
		HashSet result;
		result.reserve(keys.size());
		for (size_t i = 0, count = keys.size(); i < count; ++i)
			result.append(std::move(keys[i]));
		keys.clear();
		return result;
	}
};

} // namespace Unigine
