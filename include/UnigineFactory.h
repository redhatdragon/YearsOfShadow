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

#include <UnigineMap.h>

namespace Unigine
{

template <typename Key, typename Base, typename ... Args>
class Factory
{
public:

	using FuncPtr = Base * (*)(Args && ...);

	Factory() {}
	~Factory() {}

	template <typename T>
	void append(const Key &key)
	{
		assert(!creators.contains(key) && "Factory::append() id already contains\n");
		creators.append(key, &create_child<T>);
	}

	bool contains(const Key &key) const { return creators.contains(key); }

	Base *create(const Key &key, Args ... args) const
	{
		auto it = creators.find(key);
		if (it == creators.end())
			return nullptr;
		return it->data(std::forward<Args>(args)...);
	}

	bool remove(const Key &key) { return creators.remove(key); }
	void clear() { creators.clear(); }

	void getKeys(Vector<Key> &keys) const { creators.getKeys(keys); }
	Vector<Key> keys() const { return creators.keys(); }

private:
	Map<Key, FuncPtr> creators;

	template <typename Child>
	static Base *create_child(Args && ... args) { return new Child(std::forward<Args>(args)...); }

};

} //namespace Unigine
