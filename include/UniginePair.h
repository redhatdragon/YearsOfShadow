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

#include <UnigineBase.h>
#include <utility>

namespace Unigine
{

template<typename TypeFirst, typename TypeSecond>
class Pair
{

public:
	TypeFirst first;
	TypeSecond second;

	Pair() {}

	template<typename OtherFirst, typename OtherSecond>
	Pair(const Pair<OtherFirst, OtherSecond> &o)
		: first(o.first)
		, second(o.second)
	{}

	template<typename OtherFirst, typename OtherSecond>
	Pair(Pair<OtherFirst, OtherSecond> &&o)
		: first(std::forward<OtherFirst>(o.first))
		, second(std::forward<OtherSecond>(o.second))
	{}

	Pair(const TypeFirst &f, const TypeSecond &s)
		: first(f)
		, second(s)
	{}

	template<typename OtherFirst, typename OtherSecond>
	Pair(OtherFirst &&f, OtherSecond &&s)
		: first(std::forward<OtherFirst>(f))
		, second(std::forward<OtherSecond>(s))
	{}

	template<typename OtherFirst, typename OtherSecond>
	Pair &operator=(Pair<OtherFirst, OtherSecond> &&o)
	{
		first = std::forward<OtherFirst>(o.first);
		second = std::forward<OtherSecond>(o.second);
		return *this;
	}

	void swap(Pair &o)
	{
		std::swap(first, o.first);
		std::swap(second, o.second);
	}

	template<typename OtherFirst, typename OtherSecond>
	UNIGINE_INLINE bool operator==(const Pair<OtherFirst, OtherSecond> &o) const
	{
		return first == o.first && second == o.second;
	}

	template<typename OtherFirst, typename OtherSecond>
	UNIGINE_INLINE bool operator!=(const Pair<OtherFirst, OtherSecond> &o) const
	{
		return first != o.first || second != o.second;
	}

	template<typename OtherFirst, typename OtherSecond>
	UNIGINE_INLINE bool operator<(const Pair<OtherFirst, OtherSecond> &o) const
	{
		return first < o.first || (!(o.first < first) && second < o.second);
	}

	template<typename OtherFirst, typename OtherSecond>
	UNIGINE_INLINE bool operator>(const Pair<OtherFirst, OtherSecond> &o) const { return o < *this; }

	template<typename OtherFirst, typename OtherSecond>
	UNIGINE_INLINE bool operator<=(const Pair<OtherFirst, OtherSecond> &o) const { return !(o < *this); }

	template<typename OtherFirst, typename OtherSecond>
	UNIGINE_INLINE bool operator>=(const Pair<OtherFirst, OtherSecond> &o) const { return !(*this < o); }

};

template <typename TypeFirst, typename TypeSecond>
Pair <TypeFirst, TypeSecond> MakePair(TypeFirst &&f, TypeSecond &&s)
{
	return Pair<TypeFirst, TypeSecond>(std::forward<TypeFirst>(f), std::forward<TypeSecond>(s));
}

} // namespace Unigine
