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

#include <UnigineMemory.h>

#include <algorithm>
#include <iterator>
#include <limits>

//special for std::numeric_limits
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace Unigine {


struct VectorAllocator
{
	static char *allocate(size_t size) { return (char *)Memory::allocate(size); }
	static void deallocate(char *ptr) { Memory::deallocate(ptr); }
};

template <typename Type, typename Counter = int, typename Allocator = VectorAllocator>
class Vector
{
	template<typename OtherType, typename OtherCounter, typename OtherAllocator>
	friend class Vector;
public:

	template <typename IteratorType>
	class IteratorTemplate
	{
	private:

		friend class Vector<Type, Counter, Allocator>;

		UNIGINE_INLINE IteratorTemplate(IteratorType *ptr)
			: ptr(ptr)
		{}

	public:

		UNIGINE_INLINE IteratorTemplate()
			: ptr(nullptr)
		{}
		UNIGINE_INLINE IteratorTemplate(const IteratorTemplate &it)
			: ptr(it.ptr)
		{}

		UNIGINE_INLINE IteratorTemplate &operator=(const IteratorTemplate &it) { ptr = it.ptr; return *this; }

		UNIGINE_INLINE IteratorType &operator*() const { return *ptr; }
		UNIGINE_INLINE IteratorType *operator->() const { return ptr; }
		UNIGINE_INLINE IteratorType &get() const { return *ptr; }

		UNIGINE_INLINE IteratorTemplate &operator++() { ptr++; return *this; }
		UNIGINE_INLINE IteratorTemplate operator++(int) { IteratorTemplate ret = *this; ptr++; return ret; }
		UNIGINE_INLINE IteratorTemplate &operator--() { ptr--; return *this; }
		UNIGINE_INLINE IteratorTemplate operator--(int) { IteratorTemplate ret = *this;	ptr--; return ret; }

		UNIGINE_INLINE IteratorTemplate operator+(size_t n) const { return IteratorTemplate(ptr + n); }
		UNIGINE_INLINE IteratorTemplate operator-(size_t n) const { return IteratorTemplate(ptr - n); }

		UNIGINE_INLINE IteratorTemplate &operator+=(size_t n) { ptr += n; return *this; }
		UNIGINE_INLINE IteratorTemplate &operator-=(size_t n) { ptr -= n; return *this; }

		template<typename T>
		UNIGINE_INLINE Counter operator-(const IteratorTemplate<T> &it) const { return Counter(ptr - it.ptr); }

		template<typename T>
		UNIGINE_INLINE bool operator==(const IteratorTemplate<T> &it) const { return ptr == it.ptr; }
		template<typename T>
		UNIGINE_INLINE bool operator!=(const IteratorTemplate<T> &it) const { return ptr != it.ptr; }
		template<typename T>
		UNIGINE_INLINE bool operator<(const IteratorTemplate<T> &it) const { return ptr < it.ptr; }
		template<typename T>
		UNIGINE_INLINE bool operator>(const IteratorTemplate<T> &it) const { return ptr > it.ptr; }
		template<typename T>
		UNIGINE_INLINE bool operator<=(const IteratorTemplate<T> &it) const { return ptr <= it.ptr; }
		template<typename T>
		UNIGINE_INLINE bool operator>=(const IteratorTemplate<T> &it) const { return ptr >= it.ptr; }

		using iterator_category = std::random_access_iterator_tag;
		using difference_type = Counter;
		using value_type = IteratorType;
		using pointer = IteratorType *;
		using reference = IteratorType &;

	private:
		IteratorType *ptr;
	};

	template <typename IteratorType>
	class ReverseIteratorTemplate
	{
	private:

		friend class Vector<Type, Counter, Allocator>;

		UNIGINE_INLINE ReverseIteratorTemplate(IteratorType *ptr)
			: ptr(ptr)
		{}

	public:

		UNIGINE_INLINE ReverseIteratorTemplate()
			: ptr(nullptr)
		{}
		UNIGINE_INLINE ReverseIteratorTemplate(const ReverseIteratorTemplate &it)
			: ptr(it.ptr)
		{}

		UNIGINE_INLINE ReverseIteratorTemplate &operator=(const ReverseIteratorTemplate &it) { ptr = it.ptr; return *this; }

		UNIGINE_INLINE IteratorType &operator*() const { return *ptr; }
		UNIGINE_INLINE IteratorType *operator->() const { return ptr; }
		UNIGINE_INLINE IteratorType &get() const { return *ptr; }

		UNIGINE_INLINE ReverseIteratorTemplate &operator++() { ptr--; return *this; }
		UNIGINE_INLINE ReverseIteratorTemplate operator++(int) { ReverseIteratorTemplate ret = *this; ptr--; return ret; }
		UNIGINE_INLINE ReverseIteratorTemplate &operator--() { ptr++; return *this; }
		UNIGINE_INLINE ReverseIteratorTemplate operator--(int) { ReverseIteratorTemplate ret = *this;	ptr++; return ret; }

		UNIGINE_INLINE ReverseIteratorTemplate operator+(size_t n) const { return ReverseIteratorTemplate(ptr - n); }
		UNIGINE_INLINE ReverseIteratorTemplate operator-(size_t n) const { return ReverseIteratorTemplate(ptr + n); }

		UNIGINE_INLINE ReverseIteratorTemplate &operator+=(size_t n) { ptr -= n; return *this; }
		UNIGINE_INLINE ReverseIteratorTemplate &operator-=(size_t n) { ptr += n; return *this; }

		template<typename T>
		UNIGINE_INLINE Counter operator-(const ReverseIteratorTemplate<T> &it) const { return Counter(it.ptr - ptr); }

		template<typename T>
		UNIGINE_INLINE int operator==(const ReverseIteratorTemplate<T> &it) const { return ptr == it.ptr; }
		template<typename T>
		UNIGINE_INLINE int operator!=(const ReverseIteratorTemplate<T> &it) const { return ptr != it.ptr; }
		template<typename T>
		UNIGINE_INLINE bool operator<(const ReverseIteratorTemplate<T> &it) const { return it.ptr < ptr; }
		template<typename T>
		UNIGINE_INLINE bool operator>(const ReverseIteratorTemplate<T> &it) const { return it.ptr > ptr; }
		template<typename T>
		UNIGINE_INLINE bool operator<=(const ReverseIteratorTemplate<T> &it) const { return it.ptr <= ptr; }
		template<typename T>
		UNIGINE_INLINE bool operator>=(const ReverseIteratorTemplate<T> &it) const { return it.ptr >= ptr; }

		using iterator_category = std::random_access_iterator_tag;
		using difference_type = Counter;
		using value_type = IteratorType;
		using pointer = IteratorType *;
		using reference = IteratorType &;

	private:
		IteratorType *ptr;
	};

	using Iterator = IteratorTemplate<Type>;
	using ConstIterator = IteratorTemplate<const Type>;

	using ReverseIterator = ReverseIteratorTemplate<Type>;
	using ConstReverseIterator = ReverseIteratorTemplate<const Type>;

	using value_type = Type;
	using size_type  = size_t;

	using iterator = IteratorTemplate<Type>;
	using const_iterator = IteratorTemplate<const Type>;

	using reverse_iterator = ReverseIteratorTemplate<Type>;
	using const_reverse_iterator = ReverseIteratorTemplate<const Type>;

public:

	Vector()
		: data(nullptr)
		, length(0)
		, capacity(0)
	{}

	Vector(std::initializer_list<Type> list)
		: Vector()
	{
		allocate(list.size());
		copy(reinterpret_cast<Type *>(data), list.begin(), Counter(list.size()));
		length = Counter(list.size());
	}

	Vector(const Vector &o)
		: Vector()
	{
		append(o);
	}

	Vector(Vector &&o)
	{
		if (o.is_dynamic())
		{
			length = o.length;
			capacity = o.capacity;
			data = o.data;

			o.length = 0;
			o.capacity = 0;
			o.data = nullptr;
		} else
		{
			length = 0;
			capacity = 0;
			data = nullptr;
			allocate(o.length);
			move(reinterpret_cast<Type *>(data), reinterpret_cast<Type *>(o.data), o.length);
			length = o.length;
			o.clear();
		}
	}

	template<typename C, typename A>
	Vector(const Vector<Type, C, A> &o)
		: Vector()
	{
		append(o);
	}

	explicit Vector(size_t size)
		: Vector()
	{
		resize(size);
	}

	Vector(const Type &t, size_t size)
		: Vector()
	{
		resize(t, size);
	}

	Vector(const Type *v, size_t size)
		: Vector()
	{
		append(v, size);
	}

	~Vector()
	{
		destruct();
		dealloc(data);
	}

	Vector &operator=(const Vector &v)
	{
		if (this == &v)
			return *this;
		clear();
		allocate(v.length);
		copy(reinterpret_cast<Type *>(data), reinterpret_cast<const Type *>(v.data), v.length);
		length = v.length;
		return *this;
	}

	Vector &operator=(Vector &&v)
	{
		if (this == &v)
			return *this;
		if (v.is_dynamic())
		{
			destruct();
			dealloc(data);
			length = v.length;
			capacity = v.capacity;
			data = v.data;

			v.length = 0;
			v.capacity = 0;
			v.data = nullptr;
			return *this;
		}

		clear();
		allocate(v.length);
		move(reinterpret_cast<Type *>(data), reinterpret_cast<Type *>(v.data), v.length);
		length = v.length;
		v.clear();
		return *this;
	}

	template<typename C>
	Vector &operator=(const Vector<Type, C> &v)
	{
		if (get() == v.get())
			return *this;
		clear();
		allocate(v.length);
		copy(reinterpret_cast<Type *>(data), reinterpret_cast<const Type *>(v.data), v.length);
		length = v.length;
		return *this;
	}

	UNIGINE_INLINE Vector &operator+=(const Type &v)
	{
		append(v);
		return *this;
	}

	UNIGINE_INLINE Vector &operator+=(Type &&v)
	{
		append(std::move(v));
		return *this;
	}

	template<typename C, typename A>
	UNIGINE_INLINE Vector operator+(const Vector<Type, C, A> &v) const
	{
		Vector result;
		result.allocate(size() + v.size());
		result.appendFast(*this);
		result.appendFast(v);
		return result;
	}

	template<typename C, typename A>
	UNIGINE_INLINE Vector &operator+=(const Vector<Type, C, A> &v)
	{
		append(v);
		return *this;
	}

	template<typename C, typename A>
	UNIGINE_INLINE Vector &operator+=(Vector<Type, C, A> &&v)
	{
		append(std::move(v));
		return *this;
	}

	UNIGINE_INLINE void append(const Type &v)
	{
		return (length + 1 > capacity)
				? realloc(v)
				: construct(data, length++, v);
	}

	UNIGINE_INLINE void append(Type &&v)
	{
		return (length + 1 > capacity)
				? realloc(std::move(v))
				: construct(data, length++, std::move(v));
	}

	UNIGINE_INLINE void appendUnique(const Type &v) { if (!contains(v)) append(v); }

	UNIGINE_INLINE void push_back(const Type &v) { append(v); }
	UNIGINE_INLINE void push_back(Type &&v) { append(std::move(v)); }

	template <typename ... Args>
	UNIGINE_INLINE Type &emplace_back(Args && ... args)
	{
		return (length + 1 > capacity)
				? realloc_emplace(std::forward<Args>(args)...)
				: construct_emplace(data, length++, std::forward<Args>(args)...);
	}

	template<typename C, typename A>
	UNIGINE_INLINE void append(const Vector<Type, C, A> &v)
	{
		assert(getMaxSize() >= size_t(length) + v.size() && "Vector::append: Counter is small");
		if (length + v.size() > capacity)
			reserve(length + v.size());
		copy(reinterpret_cast<Type *>(data) + length, v.get(), Counter(v.size()));
		length += Counter(v.size());
	}

	template<typename C, typename A>
	UNIGINE_INLINE void append(Vector<Type, C, A> &&v)
	{
		assert(getMaxSize() >= size_t(length) + v.size() && "Vector::append: Counter is small");
		if (length + v.size() > capacity)
			reserve(length + v.size());
		move(reinterpret_cast<Type *>(data) + length, v.get(), Counter(v.size()));
		length += Counter(v.size());
		v.clear();
	}

	UNIGINE_INLINE Type &append()
	{
		if (length + 1 > capacity)
			realloc();
		return construct(data, length++);
	}

	UNIGINE_INLINE void appendFast(const Type &v)
	{
		assert(capacity >= (length + 1) && "Vector::appendFast()");
		construct(data, length++, v);
	}

	UNIGINE_INLINE void appendFast(Type &&v)
	{
		assert(capacity >= (length + 1) && "Vector::appendFast()");
		construct(data, length++, std::move(v));
	}

	template<typename C, typename A>
	UNIGINE_INLINE void appendFast(const Vector<Type, C, A> &v)
	{
		assert(capacity >= (length + v.size()) && "Vector::appendFast()");
		copy(reinterpret_cast<Type *>(data) + length, v.get(), v.size());
		length += v.size();
	}

	template<typename C, typename A>
	UNIGINE_INLINE void appendFast(Vector<Type, C, A> &&v)
	{
		assert(capacity >= (length + v.size()) && "Vector::appendFast()");
		move(reinterpret_cast<Type *>(data) + length, v.get(), v.size());
		length += v.size();
		v.clear();
	}

	template <typename ... Args>
	UNIGINE_INLINE Type &emplaceFast(Args && ... args)
	{
		return construct_emplace(data, length++, std::forward<Args>(args)...);
	}

	UNIGINE_INLINE Type &appendFast()
	{
		assert(capacity >= (length + 1) && "Vector::appendFast()");
		return construct(data, length++);
	}

	void append(size_t pos, const Type &v)
	{
		assert(pos <= size_t(length) && "Vector::append(): bad position");
		if (length + 1 > capacity)
		{
			capacity = grow_to(length + 1);
			char *new_data = alloc(capacity * sizeof(Type));
			if (data)
			{
				construct(new_data, Counter(pos), v);
				move(reinterpret_cast<Type *>(new_data), reinterpret_cast<Type *>(data), Counter(pos));
				move(reinterpret_cast<Type *>(new_data) + pos + 1, reinterpret_cast<Type *>(data) + pos, length - Counter(pos));
				destruct();
				dealloc(data);
			} else
				construct(new_data, 0, v);

			data = new_data;
			length++;
			return;
		}

		construct(data, length++);
		insert_internal(Counter(pos));
		*(reinterpret_cast<Type *>(data) + pos) = v;
	}

	template<typename C, typename A>
	void append(size_t pos, const Vector<Type, C, A> &v)
	{
		assert(pos <= size_t(length) && "Vector::append(): bad position");
		if (length + v.length > capacity)
		{
			capacity = grow_to(length + v.length);
			char *new_data = alloc(capacity * sizeof(Type));
			if (data)
			{
				move(reinterpret_cast<Type *>(new_data), reinterpret_cast<Type *>(data), Counter(pos));
				copy(reinterpret_cast<Type *>(new_data) + pos, v.get(), v.length);
				move(reinterpret_cast<Type *>(new_data) + pos + v.length, reinterpret_cast<Type *>(data) + pos, length - Counter(pos));
				destruct();
				dealloc(data);
			} else
				copy(reinterpret_cast<Type *>(new_data), v.get(), Counter(v.size()));

			data = new_data;
			length += v.length;
			return;
		}

		Counter construct_own_tail_size = length - Counter(pos);
		if (construct_own_tail_size > v.length)
			construct_own_tail_size = v.length;

		for (Counter i = 0; i < construct_own_tail_size; ++i)
		{
			construct(data, length + v.length - i - 1,
				std::move(*(reinterpret_cast<Type *>(data) + length - i - 1)));
		}

		Counter construct_tail_size = v.length - construct_own_tail_size;
		for (Counter i = 0; i < construct_tail_size; ++i)
		{
			construct(data, length + i,
				*(reinterpret_cast<Type *>(v.data) + i + construct_own_tail_size));
		}

		for (Counter i = length - v.length - 1; i >= Counter(pos); --i)
		{
			*(reinterpret_cast<Type *>(data) + i + v.length) =
				std::move(*(reinterpret_cast<Type *>(data) + i));
		}

		copy(reinterpret_cast<Type *>(data) + pos, v.get(), v.length - construct_tail_size);

		length += v.size();
	}

	template<typename C, typename A>
	void append(size_t pos, Vector<Type, C, A> &&v)
	{
		assert(pos <= size_t(length) && "Vector::append(): bad position");
		if (length + v.length > capacity)
		{
			capacity = grow_to(length + v.length);
			char *new_data = alloc(capacity * sizeof(Type));
			if (data)
			{
				move(reinterpret_cast<Type *>(new_data), reinterpret_cast<Type *>(data), Counter(pos));
				move(reinterpret_cast<Type *>(new_data) + pos, v.get(), v.length);
				move(reinterpret_cast<Type *>(new_data) + pos + v.length, reinterpret_cast<Type *>(data) + pos, length - Counter(pos));
				destruct();
				dealloc(data);
			} else
				move(reinterpret_cast<Type *>(new_data), v.get(), v.length);

			data = new_data;
			length += v.length;
			v.clear();
			return;
		}

		Counter construct_own_tail_size = std::min(v.length, length - Counter(pos));
		for (Counter i = 0; i < construct_own_tail_size; ++i)
		{
			construct(data, length + v.length - i - 1,
				std::move(*(reinterpret_cast<Type *>(data) + length - i - 1)));
		}

		Counter construct_tail_size = v.length - construct_own_tail_size;
		for (Counter i = 0; i < construct_tail_size; ++i)
		{
			construct(data, length + i,
				std::move(*(reinterpret_cast<Type *>(v.data) + i + construct_own_tail_size)));
		}

		for (Counter i = length - v.length - 1; i >= Counter(pos); --i)
		{
			*(reinterpret_cast<Type *>(data) + i + v.length) =
				std::move(*(reinterpret_cast<Type *>(data) + i));
		}

		move(reinterpret_cast<Type *>(data) + pos, v.get(), v.length - construct_tail_size);

		length += v.size();
		v.clear();
	}

	void append(size_t pos, Type &&v)
	{
		assert(pos <= size_t(length) && "Vector::append(): bad position");
		if (length + 1 > capacity)
		{
			capacity = grow_to(length + 1);
			char *new_data = alloc(capacity * sizeof(Type));
			if (data)
			{
				construct(new_data, Counter(pos), std::move(v));
				move(reinterpret_cast<Type *>(new_data), reinterpret_cast<Type *>(data), Counter(pos));
				move(reinterpret_cast<Type *>(new_data) + pos + 1, reinterpret_cast<Type *>(data) + pos, length - Counter(pos));
				destruct();
				dealloc(data);
			} else
				construct(new_data, 0, std::move(v));

			data = new_data;
			length++;
			return;
		}

		construct(data, length++);
		insert_internal(Counter(pos));
		*(reinterpret_cast<Type *>(data) + pos) = std::move(v);
	}

	template<typename C, typename A>
	UNIGINE_INLINE void prepend(const Vector<Type, C, A> &v)
	{
		append(0, v);
	}

	template<typename C, typename A>
	UNIGINE_INLINE void prepend(Vector<Type, C, A> &&v)
	{
		append(0, std::move(v));
	}

	UNIGINE_INLINE void prepend(const Type &v)
	{
		append(0, v);
	}

	UNIGINE_INLINE void prepend(Type &&v)
	{
		append(0, std::move(v));
	}

	template<typename ... Args>
	Type &emplace(size_t pos, Args && ... args)
	{
		assert(pos <= size_t(length) && "Vector::emplace(): bad position");
		if (length + 1 > capacity)
		{
			capacity = grow_to(length + 1);
			char *new_data = alloc(capacity * sizeof(Type));
			if (data)
			{
				construct_emplace(new_data, Counter(pos), std::forward<Args>(args)...);
				move(reinterpret_cast<Type *>(new_data), reinterpret_cast<Type *>(data), Counter(pos));
				move(reinterpret_cast<Type *>(new_data) + pos + 1, reinterpret_cast<Type *>(data) + pos, length - Counter(pos));
				destruct();
				dealloc(data);
			} else
				construct_emplace(new_data, 0, std::forward<Args>(args)...);

			data = new_data;
			length++;
			return *(reinterpret_cast<Type *>(data) + pos);
		}

		Type value(std::forward<Args>(args)...);
		emplaceFast(value);
		insert_internal(Counter(pos));
		set_emplace(data, Counter(pos), std::move(value));
		return *(reinterpret_cast<Type *>(data) + pos);
	}

	UNIGINE_INLINE void insert(size_t pos, const Type &v) { append(pos, v); }
	UNIGINE_INLINE void insert(size_t pos, Type &&v) { append(pos, std::move(v)); }
	template<typename C, typename A>
	UNIGINE_INLINE void insert(size_t pos, const Vector<Type, C, A> &v) { append(pos, v); }
	template<typename C, typename A>
	UNIGINE_INLINE void insert(size_t pos, Vector<Type, C, A> &&v) { append(pos, std::move(v)); }
	template<typename IteratorType>
	UNIGINE_INLINE void insert(const IteratorTemplate<IteratorType> &it, const Type &v) { append(size_t(it.ptr - get()), v); }
	template<typename IteratorType>
	UNIGINE_INLINE void insert(const IteratorTemplate<IteratorType> &it, Type &&v) { append(size_t(it.ptr - get()), std::move(v)); }
	UNIGINE_INLINE void push_front(const Type &v) { append(0, v); }
	UNIGINE_INLINE void push_front(Type &&v) { append(0, std::move(v)); }

	UNIGINE_INLINE void move(size_t from, size_t to)
	{
		assert(from < size_t(length) && "Vector::move(): bad from");
		assert(to < size_t(length) && "Vector::move(): bad to");

		if (from == to)
			return;
		Type v(std::move(*(reinterpret_cast<Type *>(data) + from)));
		shift(Counter(from), Counter(to));
		*(reinterpret_cast<Type *>(data) + to) = std::move(v);
	}

	UNIGINE_INLINE void append(const Type *v, size_t size)
	{
		assert(getMaxSize() >= size_t(length) + size && "Vector::append: Counter is small");
		if (length + size > size_t(capacity))
			reserve(length + size);
		copy(reinterpret_cast<Type *>(data) + length, v, Counter(size));
		length += Counter(size);
	}

	UNIGINE_INLINE void remove(size_t pos, size_t size = 1)
	{
		assert(pos < size_t(length) && "Vector::remove(): bad position");
		assert(pos + size <= size_t(length) && "Vector::remove(): bad size");
		remove_internal(Counter(pos), Counter(size));
		length -= Counter(size);
	}

	UNIGINE_INLINE void removeFast(size_t pos)
	{
		assert(pos < size_t(length) && "Vector::removeFast(): bad position");
		*(reinterpret_cast<Type*>(data) + pos) = std::move(*(reinterpret_cast<Type *>(data) + length - 1));
		destruct(--length);
	}

	UNIGINE_INLINE void remove()
	{
		assert(length > 0 && "Vector::remove(): bad length");
		destruct(--length);
	}

	UNIGINE_INLINE bool removeOne(const Type &v)
	{
		for (Counter i = 0; i < length; ++i)
		{
			if (v != *(reinterpret_cast<Type *>(data) + i))
				continue;
			remove(i);
			return true;
		}
		return false;
	}
	
	UNIGINE_INLINE bool replaceOne(const Type &old_value, const Type &new_value)
	{
		for (Counter i = 0; i < length; ++i)
		{
			if (old_value != *(reinterpret_cast<Type *>(data) + i))
				continue;
			set(i, new_value);
			return true;
		}
		return false;
	}
	
	UNIGINE_INLINE void replace(const Type &old_value, const Type &new_value)
	{
		for (Counter i = 0; i < length; ++i)
		{
			if (old_value == *(reinterpret_cast<Type *>(data) + i))
				set(i, new_value);
		}
	}

	UNIGINE_INLINE bool removeOneFast(const Type &v)
	{
		for (Counter i = 0; i < length; ++i)
		{
			if (v != *(reinterpret_cast<Type *>(data) + i))
				continue;
			removeFast(i);
			return true;
		}
		return false;
	}

	UNIGINE_INLINE void removeAll(const Type &v)
	{
		for (Counter i = 0; i < length; ++i)
		{
			if (v != *(reinterpret_cast<Type *>(data) + i))
				continue;
			remove(i);
		}
	}

	UNIGINE_INLINE void removeAllFast(const Type &v)
	{
		for (Counter i = 0; i < length; ++i)
		{
			if (v != *(reinterpret_cast<Type *>(data) + i))
				continue;
			removeFast(i);
			i--;
		}
	}

	UNIGINE_INLINE void removeLast() { remove(); }
	UNIGINE_INLINE void removeFirst() { remove(0); }

	template<typename IteratorType>
	UNIGINE_INLINE void remove(const IteratorTemplate<IteratorType> &it) { remove(size_t(it.ptr - get())); }
	template<typename IteratorType>
	UNIGINE_INLINE void removeFast(const IteratorTemplate<IteratorType> &it) { removeFast(size_t(it.ptr - get())); }
	template<typename IteratorType>
	UNIGINE_INLINE Iterator erase(const IteratorTemplate<IteratorType> &it)
	{
		size_t index = it.ptr - get();
		remove(index);
		return Iterator(get() + index);
	}
	template<typename IteratorType0, typename IteratorType1>
	UNIGINE_INLINE Iterator erase(const IteratorTemplate<IteratorType0> &it, const IteratorTemplate<IteratorType1> &end_it)
	{
		size_t index = it.ptr - get();
		if (end_it.ptr != it.ptr)
			remove(index, size_t(end_it.ptr - it.ptr));
		return Iterator(get() + index);
	}

	UNIGINE_INLINE Type takeFirst()
	{
		assert(length > 0 && "Vector::takeFirst(): bad length");
		Type ret = std::move(first());
		removeFirst();
		return ret;
	}

	UNIGINE_INLINE Type takeLast()
	{
		assert(length > 0 && "Vector::takeLast(): bad length");
		Type ret = std::move(last());
		removeLast();
		return ret;
	}

	UNIGINE_INLINE Type takeAt(size_t index)
	{
		assert(index < size_t(length) && "Vector::takeAt: bad index");
		Type ret = std::move(*(reinterpret_cast<Type *>(data) + index));
		remove(index);
		return ret;
	}

	UNIGINE_INLINE Type takeAtFast(size_t index)
	{
		assert(index < size_t(length) && "Vector::takeAtFast: bad index");
		Type ret = std::move(*(reinterpret_cast<Type *>(data) + index));
		removeFast(index);
		return ret;
	}

	UNIGINE_INLINE void allocate(size_t size)
	{
		assert(getMaxSize() >= size && "Vector::allocate: Counter is small");
		if (size <= size_t(capacity))
			return;
		capacity = Counter(size);
		char *new_data = alloc(capacity * sizeof(Type));
		if (data)
		{
			move(reinterpret_cast<Type *>(new_data), reinterpret_cast<Type *>(data), length);
			destruct();
			dealloc(data);
		}
		data = new_data;
	}

	UNIGINE_INLINE void reserve(size_t size)
	{
		assert(getMaxSize() >= size && "Vector::reserve: Counter is small");
		if (size <= size_t(capacity))
			return;
		capacity = grow_to(size);
		char *new_data = alloc(capacity * sizeof(Type));
		if (data)
		{
			move(reinterpret_cast<Type *>(new_data), reinterpret_cast<Type *>(data), length);
			destruct();
			dealloc(data);
		}
		data = new_data;
	}

	UNIGINE_INLINE void resize(size_t size)
	{
		assert(getMaxSize() >= size && "Vector::resize: Counter is small");
		if (size_t(length) == size)
			return;
		allocate(size);
		destruct(Counter(size), length);
		length = expand(Counter(size));
	}

	UNIGINE_INLINE void resize(const Type &value, size_t size)
	{
		assert(getMaxSize() >= size && "Vector::resize: Counter is small");
		for (Counter i = 0; i < length && i < Counter(size); ++i)
			*(reinterpret_cast<Type *>(data) + i) = value;

		if (size_t(length) == size)
			return;
		allocate(size);
		destruct(Counter(size), length);
		length = expand(Counter(size), value);
	}

	void shrink()
	{
		if (length == capacity)
			return;
		capacity = length;
		char *new_data = capacity > 0 ? alloc(capacity * sizeof(Type)) : nullptr;
		if (data)
		{
			move(reinterpret_cast<Type *>(new_data), reinterpret_cast<Type *>(data), length);
			destruct();
			dealloc(data);
		}
		data = new_data;
	}

	UNIGINE_INLINE bool isValidNum(int num) const { return num >= 0 && num < length; }
	UNIGINE_INLINE bool empty() const { return length == 0; }
	UNIGINE_INLINE Counter size() const { return length; }
	UNIGINE_INLINE Counter space() const { return capacity; }
	UNIGINE_INLINE size_t getMaxSize() const { return std::numeric_limits<Counter>::max(); }
	UNIGINE_INLINE size_t getMemoryUsage() const
	{
		size_t ret = 0;
		ret += sizeof(length);
		ret += sizeof(capacity);
		ret += sizeof(data);
		ret += capacity * sizeof(Type);
		return ret;
	}

	UNIGINE_INLINE void clear()
	{
		destruct();
		length = 0;
	}

	UNIGINE_INLINE void destroy()
	{
		destruct();
		dealloc(data);
		data = nullptr;
		capacity = 0;
		length = 0;
	}

	void swap(Vector &v)
	{
		if (this == &v)
			return;
		if (is_dynamic() && v.is_dynamic())
		{
			Counter l = length;
			length = v.length;
			v.length = l;
			Counter c = capacity;
			capacity = v.capacity;
			v.capacity = c;
			char *d = data;
			data = v.data;
			v.data = d;
		} else
		{
			Vector temp = std::move(*this);
			*this = std::move(v);
			v = std::move(temp);
		}
	}

	void swap(int num_0, int num_1)
	{
		std::swap(get(num_0), get(num_1));
	}

	template <typename T>
	Iterator find(const T &t)
	{
		for (Counter i = 0; i < length; ++i)
		{
			if (reinterpret_cast<Type *>(data)[i] == t)
				return Iterator(get() + i);
		}
		return end();
	}

	template <typename T>
	ConstIterator find(const T &t) const
	{
		for (Counter i = 0; i < length; ++i)
		{
			if (reinterpret_cast<Type *>(data)[i] == t)
				return ConstIterator(get() + i);
		}
		return end();
	}

	template <typename T>
	Counter findIndex(const T &t) const
	{
		for (Counter i = 0; i < length; ++i)
		{
			if (reinterpret_cast<Type *>(data)[i] == t)
				return i;
		}
		return -1;
	}

	template <typename T>
	Counter leftIndex(const T &t) const
	{
		if (length == 0 || t < first())
			return -1;
		if (last() < t)
			return length - 1;
		Counter left = 0;
		Counter right = length - 1;
		while (left <= right)
		{
			Counter middle = (left + right) >> 1;
			if (reinterpret_cast<Type *>(data)[middle] < t)
				left = middle + 1;
			else if (t < reinterpret_cast<Type *>(data)[middle])
				right = middle - 1;
			else
				return middle - 1;
		}
		return left - 1;
	}

	template <typename T>
	Counter rightIndex(const T &t) const
	{
		if (length == 0 || last() < t)
			return -1;
		if (t < first())
			return 0;
		Counter left = 0;
		Counter right = length - 1;
		while (left <= right)
		{
			Counter middle = (left + right) >> 1;
			if (reinterpret_cast<Type *>(data)[middle] < t)
				left = middle + 1;
			else if (t < reinterpret_cast<Type *>(data)[middle])
				right = middle - 1;
			else
				return middle;
		}
		return right + 1;
	}

	template<typename T>
	UNIGINE_INLINE bool contains(const T &t) const { return findIndex(t) != -1; }

	UNIGINE_INLINE bool operator==(const Vector &v) const
	{
		if (length != v.length)
			return false;
		if (length == 0)
			return true;
		return compare_internal(v.data);
	}

	UNIGINE_INLINE bool operator!=(const Vector &v) const { return !(*this == v); }

	UNIGINE_INLINE Type &at(size_t index)
	{
		assert(index < size_t(length) && "Vector::at(): bad index");
		return *(reinterpret_cast<Type*>(data) + index);
	}

	UNIGINE_INLINE const Type &at(size_t index) const
	{
		assert(index < size_t(length) && "Vector::at(): bad index");
		return *(reinterpret_cast<const Type *>(data) + index);
	}

	UNIGINE_INLINE Type &get(size_t index)
	{
		assert(index < size_t(length) && "Vector::get(): bad index");
		return *(reinterpret_cast<Type *>(data) + index);
	}

	UNIGINE_INLINE const Type &get(size_t index) const
	{
		assert(index < size_t(length) && "Vector::get(): bad index");
		return*(reinterpret_cast<const Type *>(data) + index);
	}

	UNIGINE_INLINE void set(size_t index, const Type &value)
	{
		assert(index < size_t(length) && "Vector::set(): bad index");
		reinterpret_cast<Type *>(data)[index] = value;
	}

	UNIGINE_INLINE void set(size_t index, Type &&value)
	{
		assert(index < size_t(length) && "Vector::set(): bad index");
		reinterpret_cast<Type *>(data)[index] = std::move(value);
	}

	UNIGINE_INLINE Type &operator[](size_t index)
	{
		assert(index < size_t(length) && "Vector::operator[]: bad index");
		return *(reinterpret_cast<Type *>(data) + index);
	}

	UNIGINE_INLINE const Type &operator[](size_t index) const
	{
		assert(index < size_t(length) && "Vector::operator[]: bad index");
		return *(reinterpret_cast<const Type*>(data) + index);
	}

	UNIGINE_INLINE Type value(size_t index) const { return (index >= size_t(length)) ? Type{} : *(reinterpret_cast<Type*>(data) + index); }
	UNIGINE_INLINE Type value(size_t index, const Type &def) const { return (index >= size_t(length)) ? def : *(reinterpret_cast<Type*>(data) + index); }
	UNIGINE_INLINE const Type &valueRef(size_t index, const Type &def) const { return (index >= size_t(length)) ? def : *(reinterpret_cast<Type*>(data) + index); }

	UNIGINE_INLINE Type *get() { return reinterpret_cast<Type *>(data); }
	UNIGINE_INLINE const Type *get() const { return reinterpret_cast<const Type *>(data); }

	UNIGINE_INLINE Iterator begin() { return Iterator(get()); }
	UNIGINE_INLINE ConstIterator begin() const { return ConstIterator(get()); }
	UNIGINE_INLINE ConstIterator cbegin() const { return ConstIterator(get()); }
	UNIGINE_INLINE ReverseIterator rbegin() const { return ReverseIterator(reinterpret_cast<Type *>(data) + length - 1); }
	UNIGINE_INLINE ConstReverseIterator crbegin() const { return ConstReverseIterator(reinterpret_cast<const Type *>(data) + length - 1); }

	UNIGINE_INLINE Iterator end() { return Iterator(reinterpret_cast<Type *>(data) + length); }
	UNIGINE_INLINE ConstIterator end() const { return ConstIterator(reinterpret_cast<const Type *>(data) + length); }
	UNIGINE_INLINE ConstIterator cend() const { return ConstIterator(reinterpret_cast<const Type *>(data) + length); }
	UNIGINE_INLINE ReverseIterator rend() const { return ReverseIterator(reinterpret_cast<Type *>(data) - 1); }
	UNIGINE_INLINE ConstReverseIterator crend() const { return ConstReverseIterator(reinterpret_cast<const Type *>(data) - 1); }

	UNIGINE_INLINE Iterator front() { return Iterator(reinterpret_cast<Type *>(data)); }
	UNIGINE_INLINE ConstIterator front() const { return ConstIterator(reinterpret_cast<const Type *>(data)); }

	UNIGINE_INLINE Iterator back() { return Iterator(reinterpret_cast<Type *>(data) + length - 1); }
	UNIGINE_INLINE ConstIterator back() const { return ConstIterator(reinterpret_cast<const Type *>(data) + length - 1); }

	UNIGINE_INLINE Type &first() { return *reinterpret_cast<Type *>(data); }
	UNIGINE_INLINE const Type &first() const { return *reinterpret_cast<const Type *>(data); }

	UNIGINE_INLINE Type &last() { return *(reinterpret_cast<Type *>(data) + length - 1); }
	UNIGINE_INLINE const Type &last() const { return *(reinterpret_cast<const Type *>(data) + length - 1); }

	UNIGINE_INLINE Type &random() { return *(reinterpret_cast<Type *>(data) + std::rand() % length); }
	UNIGINE_INLINE const Type &random() const { return *(reinterpret_cast<const Type *>(data) + std::rand() % length); }

	UNIGINE_INLINE Counter randomIndex() const { return (length == 0) ? 0 : (std::rand() % length); }

private:

	template <typename, int, typename>
	friend class VectorStack;

	explicit Vector(char *memory, Counter size, Counter space)
		: data(memory)
		, length(size)
		, capacity(space)
	{}

	UNIGINE_INLINE Counter grow_to(size_t new_length)
	{
		assert(size_t(capacity) < getMaxSize() && "Vector::grow_to: capacity limit");
		new_length += new_length / 2 + 1;
		return Counter(new_length > getMaxSize() ? getMaxSize() : new_length);
	}

	template<typename T>
	struct GetAlign
	{
		T t0;
		char t2;
		T t3;
	};

	UNIGINE_INLINE bool is_dynamic() const {
		std::ptrdiff_t diff = data - ((const char *)&data + sizeof(Vector));
		return diff < 0 || diff > std::ptrdiff_t(sizeof(GetAlign<Type>) - 2 * sizeof(Type));
	}

	void realloc()
	{
		capacity = grow_to(length + 1);
		char *new_data = alloc(capacity * sizeof(Type));
		if (data)
		{
			move(reinterpret_cast<Type *>(new_data), reinterpret_cast<Type *>(data), length);
			destruct();
			dealloc(data);
		}

		data = new_data;
	}

	template<typename T>
	void realloc(T &&v)
	{
		capacity = grow_to(length + 1);
		char *new_data = alloc(capacity * sizeof(Type));
		if (data)
		{
			construct(new_data, length, std::forward<T>(v));
			move(reinterpret_cast<Type *>(new_data), reinterpret_cast<Type *>(data), length);
			destruct();
			dealloc(data);
			data = new_data;
			++length;
			return;
		}

		data = new_data;
		construct(data, length++, std::forward<T>(v));
	}

	template <typename ... Args>
	Type &realloc_emplace(Args && ... args)
	{
		capacity = grow_to(length + 1);
		char *new_data = alloc(capacity * sizeof(Type));
		if (data)
		{
			Type &ret = construct_emplace(new_data, length, std::forward<Args>(args)...);
			move(reinterpret_cast<Type *>(new_data), reinterpret_cast<Type *>(data), length);
			destruct();
			dealloc(data);
			data = new_data;
			++length;
			return ret;
		}

		data = new_data;
		return construct_emplace(data, length++, std::forward<Args>(args)...);
	}

	UNIGINE_INLINE char *alloc(size_t size) { return (char *)Allocator::allocate(size); }
	UNIGINE_INLINE void dealloc(char *ptr)
	{
		if (is_dynamic())
			Allocator::deallocate(ptr);
	}

	template<typename T, int>
	struct VectorHelper
	{

		UNIGINE_INLINE static Type &construct(char *data, Counter pos) { return *new (reinterpret_cast<Type *>(data) + pos) Type(); }
		UNIGINE_INLINE static void construct(char *data, Counter pos, const Type &v) { new (reinterpret_cast<Type *>(data) + pos) Type(v); }
		UNIGINE_INLINE static void construct(char *data, Counter pos, Type &&v) { new (reinterpret_cast<Type *>(data) + pos) Type(std::move(v)); }

		template<typename ... Args>
		UNIGINE_INLINE static Type &construct_emplace(char *data, Counter pos, Args && ... args)
		{
			return *new (reinterpret_cast<Type *>(data) + pos) Type(std::forward<Args>(args)...);
		}

		template<typename ... Args>
		UNIGINE_INLINE static void set_emplace(char *data, Counter pos, Args && ... args)
		{
			*(reinterpret_cast<Type *>(data) + pos) = std::move(Type(std::forward<Args>(args)...));
		}

		UNIGINE_INLINE static Counter expand(char *data, Counter length, Counter new_length)
		{
			for (Counter i = length; i < new_length; ++i)
				new (reinterpret_cast<Type *>(data) + i) Type();
			return new_length;
		}

		UNIGINE_INLINE static Counter expand(char *data, Counter length, Counter new_length, const Type &v)
		{
			for (Counter i = length; i < new_length; ++i)
				new (reinterpret_cast<Type *>(data) + i) Type(v);
			return new_length;
		}

		UNIGINE_INLINE static void insert(char *data, Counter pos, Counter length)
		{
			for (Counter i = length - 1; i > pos; --i)
				*(reinterpret_cast<Type *>(data) + i) = std::move(*(reinterpret_cast<Type *>(data) + i - 1));
		}

		UNIGINE_INLINE static void copy(Type *dst, const Type *src, Counter length)
		{
			for (Counter i = 0; i < length; ++i)
				new (dst + i) Type(*(src + i));
		}

		UNIGINE_INLINE static void move(Type *dst, Type *src, Counter length)
		{
			for (Counter i = 0; i < length; ++i)
				new (dst + i) Type(std::move(*(src + i)));
		}
		UNIGINE_INLINE static void shift(char *data, Counter from, Counter to)
		{
			if (to > from)
			{
				for (Counter i = from; i < to; ++i)
					*(reinterpret_cast<Type *>(data) + i) = std::move(*(reinterpret_cast<Type *>(data) + i + 1));
			} else
			{
				for (Counter i = from; i > to; --i)
					*(reinterpret_cast<Type *>(data) + i) = std::move(*(reinterpret_cast<Type *>(data) + i - 1));
			}
		}
		UNIGINE_INLINE static void remove(char *data, Counter length, Counter pos, Counter size)
		{
			for (Counter i = pos; i < length - size; ++i)
				*(reinterpret_cast<Type *>(data) + i) = std::move(*(reinterpret_cast<Type *>(data) + i + size));
			destruct(data, length - size, length);
		}

		UNIGINE_INLINE static bool compare(char *data0, char *data1, Counter length)
		{
			for (Counter i = 0; i < length; ++i)
			{
				if (*(reinterpret_cast<Type *>(data0) + i) == *(reinterpret_cast<Type *>(data1) + i))
					continue;
				return false;
			}
			return true;
		}

		UNIGINE_INLINE static void destruct(char *data, Counter length)
		{
			for (Counter i = 0; i < length; ++i)
				(reinterpret_cast<Type*>(data) + i)->~Type();
		}
		UNIGINE_INLINE static void destructOne(char *data, Counter pos){ (reinterpret_cast<Type*>(data) + pos)->~Type(); }
		UNIGINE_INLINE static void destruct(char *data, Counter pos, Counter size)
		{
			for (Counter i = pos; i < size; ++i)
				(reinterpret_cast<Type *>(data) + i)->~Type();
		}
	};

	template<typename T>
	struct VectorHelper<T, 1> {

		UNIGINE_INLINE static Type &construct(char *data, Counter pos) { return *(reinterpret_cast<Type *>(data) + pos); }
		UNIGINE_INLINE static void construct(char *data, Counter pos, const Type &v) { *(reinterpret_cast<Type *>(data) + pos) = v; }
		UNIGINE_INLINE static void construct(char *data, Counter pos, Type &&v) { *(reinterpret_cast<Type *>(data) + pos) = v; }

		template<typename ... Args>
		UNIGINE_INLINE static Type &construct_emplace(char *data, Counter pos, Args && ... args)
		{
			return *(reinterpret_cast<Type *>(data) + pos) = { std::forward<Args>(args)... };
		}

		template<typename ... Args>
		UNIGINE_INLINE static void set_emplace(char *data, Counter pos, Args && ... args)
		{
			*(reinterpret_cast<Type *>(data) + pos) = { std::forward<Args>(args)... };
		}

		UNIGINE_INLINE static Counter expand(char *, Counter, Counter new_length) { return new_length; }

		UNIGINE_INLINE static Counter expand(char *data, Counter length, Counter new_length, const Type &v)
		{
			for (Counter i = length; i < new_length; ++i)
				*(reinterpret_cast<Type *>(data) + i) = v;
			return new_length;
		}

		UNIGINE_INLINE static void insert(char *data, Counter pos, Counter length)
		{
			memmove(data + (pos + 1) * sizeof(Type), data + pos * sizeof(Type), (length - pos - 1) * sizeof(Type));
		}

		UNIGINE_INLINE static void copy(Type *dst, const Type *src, Counter length)
		{
			if (length)
				memcpy(dst, src, length * sizeof(Type));
		}

		UNIGINE_INLINE static void move(Type *dst, Type *src, Counter length)
		{
			if (length)
				memcpy(dst, src, length * sizeof(Type));
		}

		UNIGINE_INLINE static void shift(char *data, Counter from, Counter to)
		{
			if (to > from)
				memmove(data + from * sizeof(Type), data + (from + 1) * sizeof(Type), (to - from) * sizeof(Type));
			else
				memmove(data + (to + 1) * sizeof(Type), data + to * sizeof(Type), (from - to) * sizeof(Type));

		}
		UNIGINE_INLINE static void remove(char *data, Counter length, Counter pos, Counter size)
		{
			memmove(data + pos * sizeof(Type), data + (pos + size) * sizeof(Type), (length - pos - size) * sizeof(Type));
		}

		UNIGINE_INLINE static bool compare(char *data0,char *data1, Counter length)
		{
			return memcmp(data0, data1, length * sizeof(Type)) == 0;
		}

		UNIGINE_INLINE static void destruct(char *, Counter ) {}
		UNIGINE_INLINE static void destructOne(char *, Counter ) {}
		UNIGINE_INLINE static void destruct(char *, Counter, Counter) {}
	};

	UNIGINE_INLINE Type &construct(char *d, Counter pos) { return VectorHelper<Type, __is_pod(Type)>::construct(d, pos); }
	UNIGINE_INLINE void construct(char *d, Counter pos, const Type &v) { VectorHelper<Type, __is_pod(Type)>::construct(d, pos, v); }
	UNIGINE_INLINE void construct(char *d, Counter pos, Type &&v) { VectorHelper<Type, __is_pod(Type)>::construct(d, pos, std::move(v)); }

	template<typename ... Args>
	UNIGINE_INLINE Type &construct_emplace(char *d, Counter pos, Args && ... args) { return VectorHelper<Type, __is_pod(Type)>::construct_emplace(d, pos, std::forward<Args>(args)...); }

	template<typename ... Args>
	UNIGINE_INLINE void set_emplace(char *d, Counter pos, Args && ... args) { VectorHelper<Type, __is_pod(Type)>::set_emplace(d, pos, std::forward<Args>(args)...); }

	UNIGINE_INLINE Counter expand(Counter new_length) { return VectorHelper<Type, __is_pod(Type)>::expand(data, length, new_length); }
	UNIGINE_INLINE Counter expand(Counter new_length, const Type &v) { return VectorHelper<Type, __is_pod(Type)>::expand(data, length, new_length, v); }

	UNIGINE_INLINE void destruct() { VectorHelper<Type, __is_pod(Type)>::destruct(data, length); }
	UNIGINE_INLINE void destruct(Counter pos) { VectorHelper<Type, __is_pod(Type)>::destructOne(data, pos); }
	UNIGINE_INLINE void destruct(Counter pos, Counter size) { VectorHelper<Type, __is_pod(Type)>::destruct(data, pos, size); }

	UNIGINE_INLINE void insert_internal(Counter pos) { VectorHelper<Type, __is_pod(Type)>::insert(data, pos, length); }
	UNIGINE_INLINE void shift(Counter from, Counter to) { VectorHelper<Type, __is_pod(Type)>::shift(data, from, to); }
	UNIGINE_INLINE void remove_internal(Counter pos, Counter size) { VectorHelper<Type, __is_pod(Type)>::remove(data, length, pos, size); }

	UNIGINE_INLINE bool compare_internal(char *odata) const { return VectorHelper<Type, __is_pod(Type)>::compare(data, odata, length); }

	UNIGINE_INLINE static void copy(Type *dst, const Type *src, Counter length) { VectorHelper<Type, __is_pod(Type)>::copy(dst, src, length); }
	UNIGINE_INLINE static void move(Type *dst, Type *src, Counter length) { VectorHelper<Type, __is_pod(Type)>::move(dst, src, length); }

	char *data;
	Counter length;
	Counter capacity;
};

template<typename Type, int Capacity = 128, typename Counter = int>
class VectorStack : public Vector<Type, Counter>
{
public:

	using Parent = Vector<Type, Counter>;

	VectorStack()
		: Parent(stack_data.data, 0, Capacity)
	{}

	VectorStack(std::initializer_list<Type> list)
		: Parent(stack_data.data, 0, Capacity)
	{
		Parent::allocate(list.size());
		Parent::copy(reinterpret_cast<Type *>(Parent::data), list.begin(), Counter(list.size()));
		Parent::length = Counter(list.size());
	}

	explicit VectorStack(const VectorStack &o)
		: Parent(stack_data.data, 0, Capacity)
	{
		Parent::append(o);
	}

	VectorStack(VectorStack &&o)
	{
		do_move_construct(std::move(o));
	}

	template<int OtherCapacity, typename OtherCounter>
	explicit VectorStack(const VectorStack<Type, OtherCapacity, OtherCounter> &o)
		: Parent(stack_data.data, 0, Capacity)
	{
		Parent::append(o);
	}

	template<int OtherCapacity, typename OtherCounter>
	VectorStack(VectorStack<Type, OtherCapacity, OtherCounter> &&o)
	{
		do_move_construct(std::move(o));
	}

	template<typename OtherCounter>
	VectorStack(const Vector<Type, OtherCounter> &o)
		: Parent(stack_data.data, 0, Capacity)
	{
		Parent::append(o);
	}

	template<typename OtherCounter>
	VectorStack(Vector<Type, OtherCounter> &&o)
		: Parent(stack_data.data, 0, Capacity)
	{
		Parent::append(std::move(o));
	}

	explicit VectorStack(size_t size)
		: Parent(stack_data.data, 0, Capacity)
	{
		Parent::resize(size);
	}

	VectorStack(const Type &t, size_t size)
		: Parent(stack_data.data, 0, Capacity)
	{
		Parent::resize(t, size);
	}

	VectorStack(const Type *v, size_t size)
		: Parent(stack_data.data, 0, Capacity)
	{
		Parent::append(v, size);
	}

	~VectorStack() {}

	VectorStack &operator=(const VectorStack &v)
	{
		if (this == &v)
			return *this;
		return do_copy_assign(v);
	}

	VectorStack &operator=(VectorStack &&v)
	{
		if (this == &v)
			return *this;
		return do_move_assign(std::move(v));
	}


	template<int OtherCapacity, typename OtherCounter>
	VectorStack &operator=(const VectorStack<Type, OtherCapacity, OtherCounter> &v)
	{
		// Don't need this check because object of different type cannot have same address.
		//if (this == &v)
		//	return *this;
		return do_copy_assign(v);
	}

	template<int OtherCapacity, typename OtherCounter>
	VectorStack &operator=(VectorStack<Type, OtherCapacity, OtherCounter> &&v)
	{
		// Don't need this check because object of different type cannot have same address.
		//if (this == &v)
		//	return *this;
		return do_move_assign(std::move(v));
	}

	template<typename OtherCounter>
	VectorStack &operator=(const Vector<Type, OtherCounter> &v)
	{
		if (this == &v)
			return *this;
		Parent::clear();
		Parent::allocate(v.size());
		Parent::copy(reinterpret_cast<Type *>(Parent::data), reinterpret_cast<const Type *>(v.get()), v.size());
		Parent::length = v.length;
		return *this;
	}

	template<typename OtherCounter>
	VectorStack &operator=(Vector<Type, OtherCounter> &&v)
	{
		if (this == &v)
			return *this;
		Parent::clear();
		Parent::allocate(v.size());
		Parent::move(reinterpret_cast<Type *>(Parent::data), reinterpret_cast<Type *>(v.get()), v.size());
		Parent::length = v.length;
		v.clear();
		return *this;
	}

	UNIGINE_INLINE size_t getMemoryUsage() const
	{
		size_t ret = 0;
		ret += sizeof(Parent::length);
		ret += sizeof(Parent::capacity);
		ret += sizeof(Parent::data);
		ret += sizeof(stack_data);
		if (Parent::is_dynamic())
			ret += Parent::capacity * sizeof(Type);
		return ret;
	}

	UNIGINE_INLINE void destroy()
	{
		Parent::destroy();
		Parent::capacity = Capacity;
		Parent::data = stack_data.data;
	}

private:

	template<int OtherCapacity, typename OtherCounter>
	void do_move_construct(VectorStack<Type, OtherCapacity, OtherCounter> &&o)
	{
		if (o.is_dynamic())
		{
			Parent::length = o.length;
			Parent::capacity = o.capacity;
			Parent::data = o.data;

			o.length = 0;
			o.capacity = 0;
			o.data = nullptr;
			o.destroy();
		} else
		{
			Parent::length = 0;
			Parent::capacity = Capacity;
			Parent::data = stack_data.data;
			Parent::allocate(o.length);
			Parent::move(reinterpret_cast<Type *>(Parent::data), reinterpret_cast<Type *>(o.data), o.length);
			Parent::length = o.length;
			o.clear();
		}
	}

	template<int OtherCapacity, typename OtherCounter>
	VectorStack &do_copy_assign(const VectorStack<Type, OtherCapacity, OtherCounter> &v)
	{
		Parent::clear();
		Parent::allocate(v.size());
		Parent::copy(reinterpret_cast<Type *>(Parent::data), reinterpret_cast<const Type *>(v.get()), v.size());
		Parent::length = v.length;
		return *this;
	}

	template<int OtherCapacity, typename OtherCounter>
	VectorStack &do_move_assign(VectorStack<Type, OtherCapacity, OtherCounter> &&v)
	{
		if (v.is_dynamic())
		{
			Parent::destruct();
			Parent::dealloc(Parent::data);

			Parent::length = v.length;
			Parent::capacity = v.capacity;
			Parent::data = v.data;

			v.length = 0;
			v.capacity = 0;
			v.data = nullptr;
			v.destroy();
		} else
		{
			Parent::clear();
			Parent::allocate(v.length);
			Parent::move(reinterpret_cast<Type *>(Parent::data), reinterpret_cast<Type *>(v.data), v.length);
			Parent::length = v.length;
			v.clear();
		}
		return *this;
	}

	template<typename T>
	struct GetAlign
	{
		T t0;
		char t2;
		T t3;
	};

	struct DisableCopyMove
	{
		DisableCopyMove() = default;

		DisableCopyMove(const DisableCopyMove &) = delete;
		DisableCopyMove(DisableCopyMove &&) = delete;

		DisableCopyMove &operator=(const DisableCopyMove &) = delete;
		DisableCopyMove &operator=(DisableCopyMove &&) = delete;
	};

	template<typename T, int C, int>
	struct Data : private DisableCopyMove { char data[C * sizeof(T)]; };

	template<typename T, int C>
	struct alignas(4) Data<T, C, 4> : private DisableCopyMove { char data[C * sizeof(T)]; };

	template<typename T, int C>
	struct alignas(8) Data<T, C, 8> : private DisableCopyMove { char data[C * sizeof(T)]; };

	template<typename T, int C>
	struct alignas(16) Data<T, C, 16> : private DisableCopyMove { char data[C * sizeof(T)]; };

	template<typename T, int C>
	struct alignas(128) Data<T, C, 128> : private DisableCopyMove { char data[C * sizeof(T)]; };

	Data<Type, Capacity, sizeof(GetAlign<Type>) - 2 * sizeof(Type)> stack_data;

};

}
