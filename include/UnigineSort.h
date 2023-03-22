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


#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif


namespace Unigine
{

namespace
{
template <class Type>
UNIGINE_INLINE Type min(Type v0, Type v1)
{
	return (v0 < v1) ? v0 : v1;
}

template <class Type>
UNIGINE_INLINE Type max(Type v0, Type v1)
{
	return (v0 > v1) ? v0 : v1;
}

template <class Type>
UNIGINE_INLINE Type clamp(Type v, Type v0, Type v1)
{
	if (v < v0)
		return v0;
	if (v1 < v)
		return v1;
	return v;
}

template <class Type>
UNIGINE_INLINE Type lerp(Type v0, Type v1, Type k)
{
	return v0 + (v1 - v0) * k;
}

template <class Type>
UNIGINE_INLINE void swap(Type &v0, Type &v1)
{
	Type temp = v0;
	v0 = v1;
	v1 = temp;
}
}

/// @cond

template <class Type>
struct quick_sort_default_compare
{
	UNIGINE_INLINE int operator() (const Type &t0, const Type &t1) const
	{
		return t0 < t1;
	}
};

template <class Type, class Func>
struct quick_sort_function_compare
{
	Func func;

	UNIGINE_INLINE quick_sort_function_compare(Func f) : func(f) {}

	UNIGINE_INLINE int operator() (const Type &t0, const Type &t1) const
	{
		return func(t0, t1);
	}
};

//////////////////////////////////////////////////////////////////////////

// integer log2, ie. how many bits we need to store this int
UNIGINE_INLINE int intLog2(unsigned int val)
{
	int bits = 0;
	while (val)
	{
		val >>= 1;
		bits++;
	}
	return bits;
}

// heap sort helper
template <typename Type, typename Data, typename Compare, bool HAS_DATA>
UNIGINE_INLINE void sift_down(Type *values, int first, int last, Compare COMP, Data *data)
{
	for ( ;; )
	{
		int child = first * 2 + 1;
		if (child > last)
			return;

		int child1 = child + 1;
		if (child1 <= last && COMP(values[child], values[child1]))
			child = child1;

		if (COMP(values[child], values[first]))
			return;
		swap(values[child], values[first]);
		if (HAS_DATA)
			swap(data[child], data[first]);
		first = child;
	}
}

// heap sort
template <typename Type, typename Data, typename Compare, bool HAS_DATA>
UNIGINE_INLINE void heap_sort(Type *values, int len, Compare COMP, Data *data)
{
	if (!values || len < 2)
		return;
	if (len == 2)
	{
		if (COMP(values[0], values[1]))
			return;
		swap(values[0], values[1]);
		if (HAS_DATA)
			swap(data[0], data[1]);
		return;
	}

	// build a max-heap, so that the largest element is root
	for (int start = (len - 2) >> 1; start >= 0; start--)
		sift_down<Type, Data, Compare, HAS_DATA>(values, start, len - 1, COMP, data);

	// now keep popping root into the end of array
	for (int end = len - 1; end > 0;)
	{
		swap(values[0], values[end]);
		if (HAS_DATA)
			swap(data[0], data[end]);
		sift_down<Type, Data, Compare, HAS_DATA>(values, 0, --end, COMP, data);
	}
}

// quick sort
template <class Type, typename Data, class Compare, bool HAS_DATA>
UNIGINE_INLINE void quick_sort(Type *values, int len, Compare COMP, Data *data)
{
	if (!values || len < 2)
		return;
	if (len == 2)
	{
		if (COMP(values[0], values[1]))
			return;
		swap(values[0], values[1]);
		if (HAS_DATA)
			swap(data[0], data[1]);
		return;
	}

	// st0 and st1 are stacks with left and right bounds of array-part.
	// They allow us to avoid recursion in quicksort implementation.
	Type *st0[32], *st1[32], *a, *b, *i, *j, x;
	int k = 1;

	const int SMALL_THRESH = 32;
	int depth_limit = intLog2(len) * 3 / 2;

	st0[0] = values;
	st1[0] = values + len - 1;
	while (k)
	{
		k--;
		i = a = st0[k];
		j = b = st1[k];

		// if quicksort fails on this data, switch to heapsort
		if (!k)
			if (!--depth_limit)
				return heap_sort<Type, Data, Compare, HAS_DATA>(a, (int)(b - a + 1), COMP, data + (a - values));

		// for tiny sub-arrays, switch to insertion sort
		if (b - a <= SMALL_THRESH)
		{
			for (i = a + 1; i <= b; i++)
			{
				for (j = i; j > a;)
				{
					Type *j1 = j - 1;
					if (COMP(*j1, *j))
						break;
					swap(*j, *j1);
					if (HAS_DATA)
						swap(data[j - values], data[j1 - values]);
					j = j1;
				}
			}
			continue;
		}

		// ATTENTION! This copy can lead to memleaks if your CopyKey
		// copies something which is not freed by objects destructor.
		x = a[(b - a) >> 1];
		while (a < b)
		{
			while (i <= j)
			{
				while (COMP(*i, x))
					i++;
				while (COMP(x, *j))
					j--;
				if (i <= j)
				{
					swap(*i, *j);
					if (HAS_DATA)
						swap(data[i - values], data[j - values]);
					i++;
					j--;
				}
			}

			// Not so obvious optimization. We put smaller array-parts
			// to the top of stack. That reduces peak stack size.
			if (j - a >= b - i)
			{
				if (a < j)
				{
					st0[k] = a;
					st1[k] = j;
					k++;
				}
				a = i;
			} else
			{
				if (i < b)
				{
					st0[k] = i;
					st1[k] = b;
					k++;
				}
				b = j;
			}
		}
	}
}

/// @endcond

template <class Type>
UNIGINE_INLINE void quickSort(Type *array, int size)
{
	quick_sort_default_compare<Type> compare;
	quick_sort<Type, Type, quick_sort_default_compare<Type>, false>(array, size, compare, NULL);
}

template <class Type, class Compare>
UNIGINE_INLINE void quickSort(Type *array, int size, Compare compare)
{
	quick_sort<Type, Type, Compare, false>(array, size, compare, NULL);
}

template <class Type, class A0, class A1>
UNIGINE_INLINE void quickSort(Type *array, int size, int (*func)(A0,A1))
{
	quick_sort_function_compare<Type, int (*)(A0,A1)> compare(func);
	quick_sort<Type, Type, quick_sort_function_compare<Type,int (*)(A0,A1)>, false>(array, size, compare, NULL);
}

template <class Type, class Data>
UNIGINE_INLINE void quickDoubleSort(Type *array, Data *data, int size)
{
	quick_sort_default_compare<Type> compare;
	quick_sort<Type, Data, quick_sort_default_compare<Type>, true>(array, size, compare, data);
}

template <class Type, class Data, class Compare>
UNIGINE_INLINE void quickDoubleSort(Type *array, Data *data, int size, Compare compare)
{
	quick_sort<Type, Data, Compare, true>(array, size, compare, data);
}

template <class Type, class Data, class A0, class A1>
UNIGINE_INLINE void quickDoubleSort(Type *array, Data *data, int size, int (*func)(A0,A1))
{
	quick_sort_function_compare<Type,int (*)(A0,A1)> compare(func);
	quick_sort<Type, Data, quick_sort_function_compare<Type,int (*)(A0,A1)>, true>(array, size, compare, data);
}

// add quickSort() wrappers to support iterators
template <class Iterator>
UNIGINE_INLINE void quickSort(Iterator begin, Iterator end)
{
	quickSort(&(*begin), end - begin);
}

template <class Iterator, class Compare>
UNIGINE_INLINE void quickSort(Iterator begin, Iterator end, Compare compare)
{
	quickSort(&(*begin), end - begin, compare);
}

template <class Iterator, class A0, class A1>
UNIGINE_INLINE void quickSort(Iterator begin, Iterator end, int (*func)(A0, A1))
{
	quickSort(&(*begin), end - begin, func);
}

//////////////////////////////////////////////////////////////////////////
// quickSort2(), sort using a threeway comparator (that returns -1/0/1)
//////////////////////////////////////////////////////////////////////////

template <class Type, class Compare>
struct ThreewayToLess
{
	const Compare &base_compare;

	UNIGINE_INLINE explicit ThreewayToLess(const Compare &c): base_compare(c)
	{}

	UNIGINE_INLINE int operator()(const Type &t0, const Type &t1) const
	{
		return base_compare(t0, t1) < 0;
	}
};

template <class Type, class Compare>
UNIGINE_INLINE void quickSort2(Type *array, int size, Compare compare)
{
	ThreewayToLess<Type, Compare> compare2(compare);
	quickSort(array, size, compare2);
}

template <class Type, class Data, class Compare>
UNIGINE_INLINE void quickDoubleSort2(Type *array, Data *data, int size, Compare compare)
{
	ThreewayToLess<Type, Compare> compare2(compare);
	quickDoubleSort(array, data, size, compare2);
}

//////////////////////////////////////////////////////////////////////////
// quickOptimize2(), optimizes dupes, and builds an index buffer
//////////////////////////////////////////////////////////////////////////

template <class Type, class Compare>
UNIGINE_INLINE int quickOptimize2(Type *data, int *indices, int size, Compare compare)
{
	// check size
	if (size == 0)
		return 0;

	// data indices
	int *data_indices = new int[size];
	for (int i = 0; i < size; i++)
		data_indices[i] = i;

	// sort indexed data
	quickDoubleSort2(data, data_indices, size, compare);

	// optimize data
	int optimized_size = 0;
	indices[data_indices[0]] = optimized_size++;
	for (int i = 1; i < size; i++)
	{
		indices[data_indices[i]] = -1;
		for (int j = optimized_size - 1; j >= 0 && j >= optimized_size - 16; j--)
		{
			if (compare(data[i], data[j]) == 0)
			{
				indices[data_indices[i]] = j;
				break;
			}
		}
		if (indices[data_indices[i]] == -1)
		{
			data[optimized_size] = data[i];
			indices[data_indices[i]] = optimized_size++;
		}
	}
	delete[] data_indices;

	return optimized_size;
}


//////////////////////////////////////////////////////////////////////////
// radixSort32(), fast sort by 32-bit key
//////////////////////////////////////////////////////////////////////////

template <class Type>
UNIGINE_INLINE const Type *radixSort32(Type *array, int size)
{
	int indices_0[256];
	int indices_1[256];
	int indices_2[256];
	int indices_3[256];
	for (int i = 0; i < 256; i++)
	{
		indices_0[i] = 0;
		indices_1[i] = 0;
		indices_2[i] = 0;
		indices_3[i] = 0;
	}
	for (int i = 0; i < size; i++)
	{
		unsigned int hash = array[i].hash;
		indices_0[(hash >> 0) & 0xff]++;
		indices_1[(hash >> 8) & 0xff]++;
		indices_2[(hash >> 16) & 0xff]++;
		indices_3[(hash >> 24) & 0xff]++;
	}
	int offset_0 = size;
	int offset_1 = 0;
	int offset_2 = size;
	int offset_3 = 0;
	for (int i = 0; i < 256; i++)
	{
		int index_0 = indices_0[i];
		int index_1 = indices_1[i];
		int index_2 = indices_2[i];
		int index_3 = indices_3[i];
		indices_0[i] = offset_0;
		indices_1[i] = offset_1;
		indices_2[i] = offset_2;
		indices_3[i] = offset_3;
		offset_0 += index_0;
		offset_1 += index_1;
		offset_2 += index_2;
		offset_3 += index_3;
	}
	for (int i = 0; i < size; i++)
	{
		const Type &t = array[i];
		array[indices_0[(t.hash >> 0) & 0xff]++] = t;
	}
	for (int i = 0; i < size; i++)
	{
		const Type &t = array[size + i];
		array[indices_1[(t.hash >> 8) & 0xff]++] = t;
	}
	for (int i = 0; i < size; i++)
	{
		const Type &t = array[i];
		array[indices_2[(t.hash >> 16) & 0xff]++] = t;
	}
	for (int i = 0; i < size; i++)
	{
		const Type &t = array[size + i];
		array[indices_3[(t.hash >> 24) & 0xff]++] = t;
	}
	return array;
}

template <class Type>
struct radix_sort_default_compare;

template <>
struct radix_sort_default_compare<int>
{
	typedef unsigned int Hash;
	UNIGINE_INLINE Hash operator()(int i) const
	{
		return i ^ 0x80000000;
	}
};

template <>
struct radix_sort_default_compare<unsigned int>
{
	typedef unsigned int Hash;
	UNIGINE_INLINE Hash operator()(unsigned int i) const
	{
		return i;
	}
};

template <>
struct radix_sort_default_compare<float>
{
	typedef unsigned int Hash;
	UNIGINE_INLINE Hash operator()(float f) const
	{
		union
		{
			float f;
			unsigned int i;
		} hash = {f};
		if (hash.i & 0x80000000)
			return hash.i ^ 0xffffffff;
		return hash.i ^ 0x80000000;
	}
};

} // namespace Unigine
