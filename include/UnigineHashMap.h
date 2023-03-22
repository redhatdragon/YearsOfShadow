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
#include <UniginePair.h>
#include "UniginePool.h"

namespace Unigine
{

template<typename Key, typename Type, typename HashType>
struct HashMapData : InstancePool<HashMapData<Key, Type, HashType>>
{
	const HashType hash;
	const Key key;
	Type data;
	HashMapData(HashType h, const Key &k)
		: hash(h)
		, key(k)
		, data{}
	{}
	HashMapData(HashType h, Key &&k)
		: hash(h)
		, key(std::move(k))
		, data{}
	{}
	template<typename ... Args>
	HashMapData(HashType h, const Key &k, Args && ... args)
		: hash(h)
		, key(k)
		, data(std::forward<Args>(args)...)
	{}
	template<typename ... Args>
	HashMapData(HashType h, Key &&k, Args && ... args)
		: hash(h)
		, key(std::move(k))
		, data(std::forward<Args>(args)...)
	{}

};

template <typename Key, typename Type, typename Counter = unsigned int>
class HashMap : public Hash<Key, HashMapData<Key, Type, typename Hasher<Key>::HashType>, typename Hasher<Key>::HashType, Counter>
{
public:

	using HashType = typename Hasher<Key>::HashType;
	using Data = HashMapData<Key, Type, HashType>;
	using Parent = Hash<Key, HashMapData<Key, Type, typename Hasher<Key>::HashType>, typename Hasher<Key>::HashType, Counter>;
	using Iterator = typename Parent::Iterator;
	using ConstIterator = typename Parent::ConstIterator;

	using iterator = typename Parent::iterator;
	using const_iterator = typename Parent::const_iterator;

	HashMap() noexcept
	{
		Parent::data = nullptr;
		Parent::length = 0;
		Parent::capacity = 0;
	}

	~HashMap()
	{
		if (Parent::data == nullptr)
			return;
		for (Counter i = 0; i < Parent::capacity; ++i)
			delete Parent::data[i];
		Memory::deallocate(Parent::data);
	}
	HashMap(std::initializer_list<Pair<Key, Type>> list)
	{
		Parent::data = nullptr;
		Parent::length = 0;
		Parent::capacity = 0;
		Parent::reserve(static_cast<Counter>(list.size()));
		for (const auto &it : list)
			do_emplace(it.first, it.second);
	}
	HashMap(const HashMap &o)
	{
		Parent::data = nullptr;
		Parent::length = 0;
		Parent::capacity = 0;
		Parent::rehash(o.capacity);
		for (const auto &it : o)
			do_emplace_hash(it.hash, it.key, it.data);
	}

	HashMap(HashMap &&o) noexcept
	{
		Parent::length = o.length;
		Parent::capacity = o.capacity;
		Parent::data = o.data;

		o.length = 0;
		o.capacity = 0;
		o.data = nullptr;
	}

	HashMap &operator=(const HashMap &o)
	{
		if (this == &o)
			return *this;
		Parent::clear();
		Parent::reserve(static_cast<Counter>(o.capacity * HASH_LOAD_FACTOR));
		for (const auto &it : o)
			do_emplace_hash(it.hash, it.key, it.data);
		return *this;
	}

	HashMap &operator=(HashMap &&o)
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

	UNIGINE_INLINE Iterator append(const Key &key, const Type &value) { return Iterator(do_emplace(key, value), Parent::data + Parent::capacity); }
	UNIGINE_INLINE Iterator append(const Key &key, Type &&value) { return Iterator(do_emplace(key, std::move(value)), Parent::data + Parent::capacity); }
	UNIGINE_INLINE Iterator append(Key &&key, const Type &value) { return Iterator(do_emplace(std::move(key), value), Parent::data + Parent::capacity); }
	UNIGINE_INLINE Iterator append(Key &&key, Type && value) { return Iterator(do_emplace(std::move(key), std::move(value)), Parent::data + Parent::capacity); }
	UNIGINE_INLINE void append(const HashMap &o)
	{
		for (const auto &it : o)
			do_emplace_hash(it.hash, it.key, it.data);
	}
	UNIGINE_INLINE void append(HashMap &&o)
	{
		for (const auto &it : o)
			do_emplace_hash(it.hash, std::move(it.key), std::move(it.data));
		o.clear();
	}

	UNIGINE_INLINE Iterator insert(const Key &key, const Type &value) { return Iterator(do_emplace(key, value), Parent::data + Parent::capacity); }
	UNIGINE_INLINE Iterator insert(const Key &key, Type &&value) { return Iterator(do_emplace(key, std::move(value)), Parent::data + Parent::capacity); }
	UNIGINE_INLINE Iterator insert(Key &&key, const Type &value) { return Iterator(do_emplace(std::move(key), value), Parent::data + Parent::capacity); }
	UNIGINE_INLINE Iterator insert(Key &&key, Type && value) { return Iterator(do_emplace(std::move(key), std::move(value)), Parent::data + Parent::capacity); }
	UNIGINE_INLINE void insert(const HashMap &o) { append(o); }
	UNIGINE_INLINE void insert(HashMap &&o) { append(std::move(o)); }

	UNIGINE_INLINE Type &append(const Key &key) { return Parent::do_append(key)->data; }
	UNIGINE_INLINE Type &append(Key &&key) { return Parent::do_append(std::move(key))->data; }

	UNIGINE_INLINE Type &insert(const Key &key) { return Parent::do_append(key)->data; }
	UNIGINE_INLINE Type &insert(Key &&key) { return Parent::do_append(std::move(key))->data; }

	template <typename ... Args>
	UNIGINE_INLINE Type &emplace(const Key &key, Args && ... args) { return (*do_emplace(key, std::forward<Args>(args)...))->data; }
	template <typename ... Args>
	UNIGINE_INLINE Type &emplace(Key &&key, Args && ... args) { return (*do_emplace(std::move(key), std::forward<Args>(args)...))->data; }

	UNIGINE_INLINE Type take(const Key &key, const Type &def) { return do_take(Hasher<Key>::create(key), key, def); }
	UNIGINE_INLINE Type take(const Key &key) { return do_take(Hasher<Key>::create(key), key); }
	UNIGINE_INLINE Type take(const Iterator &it) { return do_take(it->hash, it->key); }
	UNIGINE_INLINE Type take(const ConstIterator &it) { return do_take(it->hash, it->key); }

	UNIGINE_INLINE Type &operator[](const Key &key) noexcept { return get(key); }
	UNIGINE_INLINE Type &operator[](Key &&key) noexcept { return get(std::move(key)); }
	UNIGINE_INLINE const Type &operator[](const Key &key) const noexcept { return get(key); }

	UNIGINE_INLINE Type &get(Key &&key) noexcept { return Parent::do_append(std::move(key))->data; }
	UNIGINE_INLINE Type &get(const Key &key) noexcept { return Parent::do_append(key)->data; }

	template <typename T>
	UNIGINE_INLINE const Type &get(const T &key) const noexcept
	{
		const Data * const *d = Parent::do_find(key);
		assert(d != nullptr && "Hash::get() const : bad key.");
		return (*d)->data;
	}
	
	template <typename T>
	UNIGINE_INLINE const Type &get(const T &key, const Type &value) const noexcept
	{
		const Data *const *d = Parent::do_find(key);
		if (!d)
			return value;
		return (*d)->data;
	}

	using Parent::contains;

	template <typename T>
	UNIGINE_INLINE bool contains(const T &key, const Type &value) const noexcept
	{
		const Data * const *d = Parent::do_find(key);
		return d != nullptr && (*d)->data == value;
	}

	template<typename T>
	UNIGINE_INLINE Iterator findData(const T &t) noexcept
	{
		Iterator end_it = Parent::end();
		for (Iterator it = Parent::begin(); it != end_it; ++it)
			if (it->data == t)
				return it;
		return end_it;
	}

	template<typename T>
	UNIGINE_INLINE ConstIterator findData(const T &t) const noexcept
	{
		ConstIterator end_it = Parent::end();
		for (ConstIterator it = Parent::begin(); it != end_it; ++it)
			if (it->data == t)
				return it;
		return end_it;
	}

	template<typename T>
	UNIGINE_INLINE Type value(const T &key) const noexcept
	{
		const Data * const *d = Parent::do_find(key);
		return d == nullptr ? Type{} : (*d)->data;
	}

	template<typename T>
	UNIGINE_INLINE Type value(const T &key, const Type &def) const noexcept
	{
		const Data * const *d = Parent::do_find(key);
		return d == nullptr ? def : (*d)->data;
	}

	template<typename T>
	UNIGINE_INLINE const Type &valueRef(const T &key, const Type &def) const noexcept
	{
		const Data * const *d = Parent::do_find(key);
		return d == nullptr ? def : (*d)->data;
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
		for (Counter i = 0; i < Parent::capacity; ++i)
		{
			if (Parent::data[i] == nullptr)
				continue;
			values.appendFast(Parent::data[i]->data);
		}
	}

	UNIGINE_INLINE void getPairs(Vector<Pair<Key, Type>> &pairs) const
	{
		pairs.allocate(pairs.size() + Parent::length);
		for (Counter i = 0; i < Parent::capacity; ++i)
		{
			if (Parent::data[i] == nullptr)
				continue;
			pairs.appendFast(MakePair(Parent::data[i]->key, Parent::data[i]->data));
		}
	}

	UNIGINE_INLINE bool operator==(const HashMap &o) const noexcept
	{
		if (Parent::length != o.length)
			return false;

		for (Counter i = 0; i < Parent::capacity; ++i)
		{
			if (Parent::data[i] == nullptr)
				continue;

			auto other_data = o.do_find(Parent::data[i]->key);
			if (other_data == nullptr)
				return false;
			if ((*other_data)->data != Parent::data[i]->data)
				return false;
		}

		return true;
	}

	UNIGINE_INLINE bool operator!=(const HashMap &o) const noexcept { return !(*this == o); }

private:

	template<typename ... Args>
	Data **do_emplace_hash(HashType hash, const Key &key, Args && ... args)
	{
		if (Parent::capacity == 0)
			Parent::realloc();
		Counter index = hash & (Parent::capacity - 1);
		while (Parent::data[index])
		{
			if (Parent::data[index]->hash == hash && Parent::data[index]->key == key)
			{
				Parent::data[index]->data = Type(std::forward<Args>(args)...);
				return Parent::data + index;
			}
			index = (index + 1) & (Parent::capacity - 1);
		}

		Data *d = new Data(hash, key, std::forward<Args>(args)...);
		Parent::data[index] = d;
		++Parent::length;
		if (Parent::is_need_realloc())
			Parent::realloc(&index);

		return Parent::data + index;
	}

	template<typename ... Args>
	UNIGINE_INLINE Data **do_emplace(const Key &key, Args && ... args)
	{
		return do_emplace_hash(Hasher<Key>::create(key), key, std::forward<Args>(args)...);
	}

	template<typename ... Args>
	Data **do_emplace_hash(HashType hash, Key &&key, Args && ... args)
	{
		if (Parent::capacity == 0)
			Parent::realloc();
		Counter index = hash & (Parent::capacity - 1);
		while (Parent::data[index])
		{
			if (Parent::data[index]->hash == hash && Parent::data[index]->key == key)
			{
				Parent::data[index]->data = Type(std::forward<Args>(args)...);
				return Parent::data + index;
			}
			index = (index + 1) & (Parent::capacity - 1);
		}

		Data *d = new Data(hash, std::move(key), std::forward<Args>(args)...);
		Parent::data[index] = d;
		++Parent::length;
		if (Parent::is_need_realloc())
			Parent::realloc(&index);
		return Parent::data + index;
	}

	template<typename ... Args>
	UNIGINE_INLINE Data **do_emplace(Key &&key, Args && ... args)
	{
		return do_emplace_hash(Hasher<Key>::create(key), std::move(key), std::forward<Args>(args)...);
	}

	Type do_take(HashType hash, const Key &key, Type def)
	{
		if (Parent::length == 0)
			return def;
		Counter index = hash & (Parent::capacity - 1);

		while (Parent::data[index])
		{
			if (Parent::data[index]->hash == hash && Parent::data[index]->key == key)
				break;
			index = (index + 1) & (Parent::capacity - 1);
		}

		if (Parent::data[index] == nullptr)
			return def;

		Type ret = std::move(Parent::data[index]->data);
		delete Parent::data[index];
		Parent::data[index] = nullptr;

		--Parent::length;
		Parent::rehash_data(index);
		return ret;
	}

	Type do_take(HashType hash, const Key &key)
	{
		if (Parent::length == 0)
			return Type();
		Counter index = hash & (Parent::capacity - 1);

		while (Parent::data[index])
		{
			if (Parent::data[index]->hash == hash && Parent::data[index]->key == key)
				break;
			index = (index + 1) & (Parent::capacity - 1);
		}

		if (Parent::data[index] == nullptr)
			return Type();

		Type ret = std::move(Parent::data[index]->data);
		delete Parent::data[index];
		Parent::data[index] = nullptr;

		--Parent::length;
		Parent::rehash_data(index);
		return ret;
	}

};

} // namespace Unigine
