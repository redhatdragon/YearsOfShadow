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
#include "UnigineThread.h"
#include "UnigineMemory.h"

namespace Unigine
{

//////////////////////////////////////////////////////////////////////////
// SystemPool
//////////////////////////////////////////////////////////////////////////

template <class Type>
class SystemPool
{
public:
	static UNIGINE_INLINE void *operator new(size_t size)
	{
		assert(size == sizeof(Type) && "SystemPool::operator new(): bad object size");
		return ::malloc(size);
	}
	static UNIGINE_INLINE void operator delete(void *ptr)
	{
		::free(ptr);
	}
	static UNIGINE_INLINE void operator delete(void *ptr, size_t size)
	{
		assert(size == sizeof(Type) && "SystemPool::operator delete(): bad object size");
		::free(ptr);
	}
};

//////////////////////////////////////////////////////////////////////////
// FixedPool
//////////////////////////////////////////////////////////////////////////

template <class Type, int Number = 128>
class FixedPool
{
public:
	UNIGINE_INLINE FixedPool() {}
	UNIGINE_INLINE ~FixedPool() {}
	static UNIGINE_INLINE void *operator new(size_t size)
	{
		assert(size == sizeof(Type) && "FixedPool::operator new(): bad object size");
		return allocate();
	}
	static UNIGINE_INLINE void operator delete(void *ptr)
	{
		deallocate(ptr);
	}
	static UNIGINE_INLINE void operator delete(void *ptr, size_t size)
	{
		assert(size == sizeof(Type) && "FixedPool::operator delete(): bad object size");
		deallocate(ptr);
	}

	static UNIGINE_INLINE void *operator new[](size_t size)
	{
		return Memory::allocate(size);
	}
	static UNIGINE_INLINE void operator delete[](void *ptr) { Memory::deallocate(ptr); }
	static UNIGINE_INLINE void operator delete[](void *ptr, size_t size) { Memory::deallocate(ptr, size); }

private:
	struct Chunk;

	// base allocators
	static UNIGINE_INLINE void *malloc(size_t size)
	{
		return Memory::allocate(size);
	}
	static UNIGINE_INLINE void free(void *ptr, size_t size)
	{
		Memory::deallocate(ptr, size);
	}

	// fixed allocators
	static void *allocate()
	{
		ScopedLock lock(mutex);

		// find allocated chunk
		if (allocated_chunk == 0 || allocated_chunk->free_blocks == 0)
		{
			for (int i = 0; i < num_chunks; i++)
				if (chunks[i].free_blocks > 0)
				{
					allocated_chunk = &chunks[i];
					break;
				}
		}

		// allocate new chunk
		if (allocated_chunk == 0 || allocated_chunk->free_blocks == 0)
		{
			if (capacity == num_chunks)
			{
				int old_capacity = capacity;
				capacity = capacity + capacity / 2 + 1;
				Chunk *new_chunks = static_cast<Chunk *>(malloc(sizeof(Chunk) * capacity));
				for (int i = 0; i < num_chunks; i++)
					new_chunks[i] = chunks[i];
				free(chunks, sizeof(Chunk) * old_capacity);
				chunks = new_chunks;
			}
			chunks[num_chunks].data = static_cast<unsigned char *>(malloc(sizeof(Type) * Number));
			chunks[num_chunks].data_end = chunks[num_chunks].data + sizeof(Type) * Number;
			chunks[num_chunks].first_block = 0;
			chunks[num_chunks].free_blocks = Number;
			unsigned char *ptr = chunks[num_chunks].data;
			for (unsigned char i = 1; i < Number; i++, ptr += sizeof(Type))
				*ptr = i;
			allocated_chunk = &chunks[num_chunks++];
			deallocated_chunk = 0;
		}

		// allocate memory
		unsigned char *ret = allocated_chunk->data + allocated_chunk->first_block * sizeof(Type);
		allocated_chunk->first_block = *ret;
		allocated_chunk->free_blocks--;
		num_allocations++;

		return ret;
	}

	static void deallocate(void *p)
	{
		ScopedLock lock(mutex);

		unsigned char *ptr = static_cast<unsigned char *>(p);

		// find deallocated chunk
		if (deallocated_chunk == 0 || deallocated_chunk->data > ptr || deallocated_chunk->data_end <= ptr)
		{
			Chunk *begin = &chunks[0];
			Chunk *end = &chunks[num_chunks - 1];
			Chunk *left = (deallocated_chunk) ? deallocated_chunk : &chunks[num_chunks / 2];
			Chunk *right = left + 1;
			for (int i = 0; i < num_chunks; i++)
			{
				if (left >= begin)
				{
					if (left->data <= ptr && left->data_end > ptr)
					{
						deallocated_chunk = left;
						break;
					}
					left--;
				}
				if (right <= end)
				{
					if (right->data <= ptr && right->data_end > ptr)
					{
						deallocated_chunk = right;
						break;
					}
					right++;
				}
			}
		}

		assert(deallocated_chunk != 0 && "FixedPool::deallocate(): can't find pointer");
		assert((ptr - deallocated_chunk->data) % sizeof(Type) == 0 && "FixedPool::deallocate(): bad pointer");

		// deallocate memory
		*ptr = deallocated_chunk->first_block;
		deallocated_chunk->first_block = (unsigned char)((ptr - deallocated_chunk->data) / sizeof(Type));
		deallocated_chunk->free_blocks++;

		// deallocate chunks
		if (--num_allocations == 0)
		{
			for (int i = 0; i < num_chunks; i++)
				free(chunks[i].data, sizeof(Type) * Number);
			free(chunks, sizeof(Chunk) * capacity);
			capacity = 0;
			num_chunks = 0;
			chunks = 0;
			allocated_chunk = 0;
			deallocated_chunk = 0;
		}
	}

	struct Chunk
	{
		unsigned char *data;	   // chunk begin
		unsigned char *data_end;   // chunk end
		unsigned char first_block; // first block
		unsigned char free_blocks; // free blocks
	};

	static Mutex mutex;				 // mutex
	static int capacity;			 // chunks capacity
	static int num_chunks;			 // number of chunks
	static int num_allocations;		 // number of allocations
	static Chunk *chunks;			 // chunks
	static Chunk *allocated_chunk;	 // allocated chunk
	static Chunk *deallocated_chunk; // deallocated chunk
};

template <class Type, int Number>
Mutex FixedPool<Type, Number>::mutex;

template <class Type, int Number>
int FixedPool<Type, Number>::capacity = 0;

template <class Type, int Number>
int FixedPool<Type, Number>::num_chunks = 0;

template <class Type, int Number>
int FixedPool<Type, Number>::num_allocations = 0;

template <class Type, int Number>
typename FixedPool<Type, Number>::Chunk *FixedPool<Type, Number>::chunks = 0;

template <class Type, int Number>
typename FixedPool<Type, Number>::Chunk *FixedPool<Type, Number>::allocated_chunk = 0;

template <class Type, int Number>
typename FixedPool<Type, Number>::Chunk *FixedPool<Type, Number>::deallocated_chunk = 0;


//////////////////////////////////////////////////////////////////////////
// InstancePool
//////////////////////////////////////////////////////////////////////////

template <class Type, size_t MemoryLimit = (32 * 1048576)>
class InstancePool
{
public:
	UNIGINE_INLINE InstancePool() {}
	UNIGINE_INLINE ~InstancePool() {}

	static UNIGINE_INLINE void *operator new(size_t size)
	{
		assert(size == sizeof(Type) && "InstancePool::operator new(): bad object size");
		return allocate();
	}
	static UNIGINE_INLINE void operator delete(void *ptr)
	{
		deallocate(ptr);
	}
	static UNIGINE_INLINE void operator delete(void *ptr, size_t size)
	{
		assert(size == sizeof(Type) && "InstancePool::operator delete(): bad object size");
		deallocate(ptr);
	}

	static UNIGINE_INLINE void *operator new[](size_t size)
	{
		return Memory::allocate(size);
	}
	static UNIGINE_INLINE void operator delete[](void *ptr) { Memory::deallocate(ptr); }
	static UNIGINE_INLINE void operator delete[](void *ptr, size_t size) { Memory::deallocate(ptr, size); }

private:
	static void *allocate()
	{
		if (pool_size == 0)
			return Memory::allocate(sizeof(Type));

		{
			ScopedLock lock(mutex);
			if (pool_size != 0)
			{
				pool_size--;
				return pool[pool_size];
			}
		}

		return Memory::allocate(sizeof(Type));
	}
	
	static void deallocate(void *p)
	{
		if (p == nullptr)
			return;
		
		if ((sizeof(Type) * pool_size) >= MemoryLimit)
			return Memory::deallocate(p);

		ScopedLock lock(mutex);

		if (pool_size >= pool_capacity)
		{
			pool_capacity += pool_size / 2 + 1;
			void **new_pool = reinterpret_cast<void **>(Memory::allocate(sizeof(void *) * pool_capacity));

			if (pool_size != 0)
				memcpy(new_pool, pool, pool_size * sizeof(void *));
			if (pool)
				Memory::deallocate(pool);

			pool = new_pool;
		}

		pool[pool_size] = p;
		pool_size++;
	}

	static Mutex mutex;

	static void **pool;
	static volatile size_t pool_size;
	static size_t pool_capacity;
};

template <class Type, size_t MemoryLimit>
Mutex InstancePool<Type, MemoryLimit>::mutex;

template <class Type, size_t MemoryLimit>
void **InstancePool<Type, MemoryLimit>::pool = nullptr;

template <class Type, size_t MemoryLimit>
volatile size_t InstancePool<Type, MemoryLimit>::pool_size = 0;

template <class Type, size_t MemoryLimit>
size_t InstancePool<Type, MemoryLimit>::pool_capacity = 0;

//////////////////////////////////////////////////////////////////////////
// LocalPool
//////////////////////////////////////////////////////////////////////////

template <class Type, int Number = 256>
class LocalPool
{
public:
	UNIGINE_INLINE LocalPool() {}
	UNIGINE_INLINE ~LocalPool() {}
	static UNIGINE_INLINE void *operator new(size_t size)
	{
		assert(size == sizeof(Type) && "LocalPool::operator new(): bad object size");
		return allocate(size);
	}
	static UNIGINE_INLINE void operator delete(void *ptr)
	{
		deallocate(ptr);
	}
	static UNIGINE_INLINE void operator delete(void *ptr, size_t size)
	{
		assert(size == sizeof(Type) && "LocalPool::operator delete(): bad object size");
		deallocate(ptr);
	}

	static UNIGINE_INLINE void *operator new[](size_t size)
	{
		#if defined(_LINUX) && defined(__ARM_EABI__)
			if (sizeof(size_t) == 4 && !IS_ALIGNED16(size))
				return static_cast<size_t *>(allocate(size + sizeof(size_t) * 2)) + 2;
		#endif
		return allocate(size);
	}
	static UNIGINE_INLINE void operator delete[](void *ptr)
	{
		#if defined(_LINUX) && defined(__ARM_EABI__)
			if (sizeof(size_t) == 4 && !IS_ALIGNED16(ptr))
				ptr = static_cast<size_t *>(ptr) - 2;
		#endif
		deallocate(ptr);
	}
	static UNIGINE_INLINE void operator delete[](void *ptr, size_t size)
	{
		#if defined(_LINUX) && defined(__ARM_EABI__)
			if (sizeof(size_t) == 4 && !IS_ALIGNED16(ptr))
				ptr = static_cast<size_t *>(ptr) - 2;
		#endif
		deallocate(ptr);
	}

	// heap allocators
	static void *allocate(size_t size)
	{
		ScopedLock lock(mutex);

		// external allocation
		if (size > sizeof(Type) * Number)
			return malloc(size);

		// allocate memory
		if (allocated_pool && allocated_pool->size >= size)
		{
			void *ptr = allocate(allocated_pool, size);
			if (ptr)
				return ptr;
		}

		// find allocated pool
		for (int i = 0; i < num_pools; i++)
		{
			if (pools[i].size >= size)
			{
				void *ptr = allocate(&pools[i], size);
				if (ptr)
				{
					allocated_pool = &pools[i];
					return ptr;
				}
			}
		}

		// allocate new pool
		if (capacity == num_pools)
		{
			int old_capacity = capacity;
			capacity = capacity + capacity / 2 + 1;
			Pool *new_pools = static_cast<Pool *>(malloc(sizeof(Pool) * capacity));
			for (int i = 0; i < num_pools; i++)
				new_pools[i] = pools[i];
			free(pools, sizeof(Pool) * old_capacity);
			pools = new_pools;
		}
		pools[num_pools].size = sizeof(Type) * Number;
		pools[num_pools].data = static_cast<unsigned char *>(malloc(sizeof(Type) * Number));
		pools[num_pools].chunk = new Chunk(pools[num_pools].data, sizeof(Type) * Number, 0, 0);
		allocated_pool = &pools[num_pools++];
		deallocated_pool = 0;

		// allocate memory
		return allocate(allocated_pool, size);
	}

	static void deallocate(void *ptr)
	{
		ScopedLock lock(mutex);

		// find deallocated pool
		if (deallocated_pool == 0 || is_allocator(deallocated_pool, ptr) == 0)
		{
			for (int i = 0; i < num_pools; i++)
			{
				if (is_allocator(&pools[i], ptr))
				{
					deallocated_pool = &pools[i];
					break;
				}
			}
		}

		// deallocate memory
		if (deallocated_pool && is_allocator(deallocated_pool, ptr))
		{
			deallocate(deallocated_pool, ptr);
			if (deallocated_pool->size == sizeof(Type) * Number)
			{
				delete deallocated_pool->chunk;
				free(deallocated_pool->data, sizeof(Type) * Number);
				int num = (int)(deallocated_pool - pools);
				pools[num] = pools[--num_pools];
				allocated_pool = 0;
				deallocated_pool = 0;
				if (num_pools == 0)
				{
					free(pools, sizeof(Pool) * capacity);
					pools = 0;
					capacity = 0;
				}
			}
			return;
		}

		// external allocation
		free(ptr);
	}

private:
	struct Chunk;
	struct Pool;

	// base allocators
	static UNIGINE_INLINE void *malloc(size_t size) { return Memory::allocate(size); }
	static UNIGINE_INLINE void free(void *ptr, size_t size) { Memory::deallocate(ptr, size); }
	static UNIGINE_INLINE void free(void *ptr) { Memory::deallocate(ptr); }

	static void *allocate(Pool *pool, size_t size)
	{
		// find best fit chunk
		Chunk *chunk = 0;
		Chunk *it = pool->chunk;
		while (it != 0)
		{
			if (it->free && it->size >= size)
			{
				if (chunk == 0 || chunk->size > it->size)
					chunk = it;
			}
			it = it->next;
		}

		// allocate memory
		if (chunk)
		{
			chunk->free = 0;
			if (chunk->size > size)
			{
				new Chunk(chunk->data + size, chunk->size - size, chunk, chunk->next);
				chunk->size = size;
			}
			pool->size -= size;
			return chunk->data;
		}

		return 0;
	}

	static void deallocate(Pool *pool, void *ptr)
	{
		// deallocate chunk
		Chunk *it = pool->chunk;
		while (it != 0)
		{
			if (it->data == ptr)
			{
				it->free = 1;
				pool->size += it->size;
				if (it->prev && it->prev->free)
				{
					Chunk *old_prev = it->prev;
					it->data = it->prev->data;
					it->size += it->prev->size;
					it->prev = it->prev->prev;
					if (it->prev)
						it->prev->next = it;
					if (pool->chunk == old_prev)
						pool->chunk = it;
					delete old_prev;
				}
				if (it->next && it->next->free)
				{
					Chunk *old_next = it->next;
					it->size += it->next->size;
					it->next = it->next->next;
					if (it->next)
						it->next->prev = it;
					delete old_next;
				}
				return;
			}
			it = it->next;
		}

		assert(0 && "LocalPool::deallocate(): bad pointer");
	}

	static UNIGINE_INLINE int is_allocator(Pool *pool, void *ptr)
	{
		return (pool->data <= ptr && pool->data + sizeof(Type) * Number > ptr);
	}

	struct Chunk : public FixedPool<Chunk>
	{
		UNIGINE_INLINE Chunk(unsigned char *d, size_t s, Chunk *p, Chunk *n)
			: data(d)
			, size(s)
			, prev(p)
			, next(n)
			, free(1)
		{
			if (prev)
				prev->next = this;
			if (next)
				next->prev = this;
		}
		unsigned char *data; // chunk data
		size_t size;		 // chunk size
		Chunk *prev;		 // previuos chunk
		Chunk *next;		 // next chunk
		int free;			 // chunk is free
	};

	struct Pool
	{
		unsigned char *data; // pool data
		size_t size;		 // pool size
		Chunk *chunk;		 // root chunk
	};

	static Mutex mutex;			   // lock
	static int capacity;		   // pools capacity
	static int num_pools;		   // number of pools
	static Pool *pools;			   // pools
	static Pool *allocated_pool;   // allocated pool
	static Pool *deallocated_pool; // deallocated pool
};

template <class Type, int Number>
Mutex LocalPool<Type, Number>::mutex;

template <class Type, int Number>
int LocalPool<Type, Number>::capacity = 0;

template <class Type, int Number>
int LocalPool<Type, Number>::num_pools = 0;

template <class Type, int Number>
typename LocalPool<Type, Number>::Pool *LocalPool<Type, Number>::pools = 0;

template <class Type, int Number>
typename LocalPool<Type, Number>::Pool *LocalPool<Type, Number>::allocated_pool = 0;

template <class Type, int Number>
typename LocalPool<Type, Number>::Pool *LocalPool<Type, Number>::deallocated_pool = 0;

}