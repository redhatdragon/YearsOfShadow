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
#include <UnigineTree.h>
#include <UniginePair.h>

namespace Unigine
{

template<typename Key, typename Type, typename Allocator>
struct BiMapData
{
public:

	template<typename TypeKey, typename ... Args>
	BiMapData(TypeKey &&key, BiMapData *p, Args && ... args)
		: parent(p)
		, parent_value(nullptr)
		, right(nullptr)
		, left(nullptr)
		, right_value(nullptr)
		, left_value(nullptr)
		, balance(0)
		, balance_value(0)
		, key(std::forward<TypeKey>(key))
		, data(std::forward<Args>(args)...)
	{
	}

	BiMapData(const BiMapData &o)
		: parent(nullptr)
		, parent_value(nullptr)
		, right(nullptr)
		, left(nullptr)
		, right_value(nullptr)
		, left_value(nullptr)
		, balance(0)
		, balance_value(0)
		, key(o.key)
		, data(o.data)
	{
	}

	BiMapData(BiMapData &&o)
		: parent(nullptr)
		, parent_value(nullptr)
		, right(nullptr)
		, left(nullptr)
		, right_value(nullptr)
		, left_value(nullptr)
		, balance(0)
		, balance_value(0)
		, key(std::move(o.key))
		, data(std::move(o.data))
	{
	}

	BiMapData &operator=(const BiMapData &o) = delete;
	BiMapData &operator=(BiMapData &&o) = delete;

	BiMapData *parent;
	BiMapData *parent_value;

	union
	{
		struct
		{
			BiMapData *right;
			BiMapData *left;
		};
		BiMapData *dirs[2];
	};

	union
	{
		struct
		{
			BiMapData *right_value;
			BiMapData *left_value;
		};
		BiMapData *dirs_value[2];
	};

	char balance;
	char balance_value;

	const Key key;
	const Type data;

	static UNIGINE_INLINE void *operator new(size_t size) { return Allocator::allocate(size); }
	static UNIGINE_INLINE void operator delete(void *ptr) { Allocator::deallocate(ptr); }

	~BiMapData()
	{
		delete left;
		delete right;
	}

};

template<typename Key, typename Type, typename Allocator = TreeAllocator>
class BiMap : public Tree<Key, BiMapData<Key, Type, Allocator>, Allocator>
{
public:
	using Node = typename Unigine::BiMapData<Key, Type, Allocator>;
	using Data = Node;
	using Parent = Tree<Key, Node, Allocator>;
	using Iterator = typename Parent::Iterator;
	using ConstIterator = typename Parent::ConstIterator;

	using iterator = typename Parent::iterator;
	using const_iterator = typename Parent::const_iterator;

public:
	BiMap() : root_value(nullptr)
	{

	}

	BiMap(std::initializer_list<Pair<Key, Type>> list)
		: root_value{nullptr}
	{
		for (const auto &it: list)
			do_append_value(Parent::do_append_node(new Node(it.first, nullptr, it.second)));
	}

	BiMap(const BiMap &o)
		: root_value(nullptr)
	{
		Parent::length = 0;
		Parent::root = nullptr;

		for (const auto &it: o)
			do_append_value(Parent::do_append_node(new Node(it.key, nullptr, it.data)));
	}

	BiMap &operator=(const BiMap &o)
	{
		if (Parent::root == o.root)
			return *this;
		Parent::length = 0;
		delete Parent::root;
		Parent::root = nullptr;
		root_value = nullptr;

		for (const auto &it: o)
			do_append_value(Parent::do_append_node(new Node(it.key, nullptr, it.data)));
		return *this;
	}

	BiMap(BiMap &&o)
	{
		Parent::length = o.length;
		Parent::root = o.root;
		root_value = o.root_value;
		o.length = 0;
		o.root = nullptr;
		o.root_value = nullptr;
	}

	BiMap &operator=(BiMap &&o)
	{
		if (Parent::root == o.root)
			return *this;
		delete Parent::root;
		Parent::root = o.root;
		Parent::length = o.length;
		root_value = o.root_value;
		o.length = 0;
		o.root = nullptr;
		o.root_value = nullptr;
		return *this;
	}

	void swap(BiMap &o)
	{
		if (Parent::root == o.root)
			return;

		int i = Parent::length;
		Parent::length = o.length;
		o.length = i;

		Node *n = Parent::root;
		Parent::root = o.root;
		o.root = n;

		n = root_value;
		root_value = o.root_value;
		o.root_value = n;
	}

	UNIGINE_INLINE void clear()
	{
		Parent::length = 0;
		delete Parent::root;
		Parent::root = nullptr;
		root_value = nullptr;
	}

	UNIGINE_INLINE size_t getMemoryUsage() const
	{
		size_t ret = 0;
		ret += sizeof(Parent::length);
		ret += 2 * sizeof(Node *);
		ret += Parent::length * sizeof(Node);
		return ret;
	}

	UNIGINE_INLINE Iterator insert(const Key &key, const Type &value)
	{
		return do_replace(key, value);
	}

	UNIGINE_INLINE Iterator insert(Key &&key, const Type &value)
	{
		return do_replace(std::move(key), value);
	}

	UNIGINE_INLINE Iterator insert(const Key &key, Type &&value)
	{
		return do_replace(key, std::move(value));
	}

	UNIGINE_INLINE Iterator insert(Key &&key, Type &&value)
	{
		return do_replace(std::move(key), std::move(value));
	}

	UNIGINE_INLINE void insert(const BiMap &m)
	{
		for (const auto &it: m)
			do_replace(it.key, it.data);
	}

	UNIGINE_INLINE void insert(BiMap &&m)
	{
		for (const auto &it: m)
			do_replace(std::move(it.key), std::move(it.data));
		m.clear();
	}

	UNIGINE_INLINE Iterator append(const Key &key, const Type &value)
	{
		return do_replace(key, value);
	}

	UNIGINE_INLINE Iterator append(Key &&key, const Type &value)
	{
		return do_replace(std::move(key), value);
	}

	UNIGINE_INLINE Iterator append(const Key &key, Type &&value)
	{
		return do_replace(key, std::move(value));
	}

	UNIGINE_INLINE Iterator append(Key &&key, Type &&value)
	{
		return do_replace(std::move(key), std::move(value));
	}

	UNIGINE_INLINE void append(const BiMap &m)
	{
		for (const auto &it: m)
			do_replace(it.key, it.data);
	}

	UNIGINE_INLINE void append(BiMap &&m)
	{
		for (const auto &it: m)
			do_replace(std::move(it.key), std::move(it.data));
		m.clear();
	}

	template <typename ... Args>
	UNIGINE_INLINE const Type &emplace(const Key &key, Args && ... args)
	{
		return do_replace_emplace(key, std::forward<Args>(args)...)->data;
	}

	template <typename ... Args>
	UNIGINE_INLINE const Type &emplace(Key &&key, Args && ... args)
	{
		return do_replace_emplace(std::move(key), std::forward<Args>(args)...)->data;
	}

	UNIGINE_INLINE Iterator appendFast(const Key &key, const Type &value)
	{
		return do_append_value(Parent::do_append_node(new Node(key, nullptr, value)));
	}

	UNIGINE_INLINE Iterator appendFast(Key &&key, const Type &value)
	{
		return do_append_value(Parent::do_append_node(new Node(std::move(key), nullptr, value)));
	}

	UNIGINE_INLINE Iterator appendFast(const Key &key, Type &&value)
	{
		return do_append_value(Parent::do_append_node(new Node(key, nullptr, std::move(value))));
	}

	UNIGINE_INLINE Iterator appendFast(Key &&key, Type &&value)
	{
		return do_append_value(Parent::do_append_node(new Node(std::move(key), nullptr, std::move(value))));
	}

	UNIGINE_INLINE void appendFast(const BiMap &m)
	{
		for (const auto &it: m)
			appendFast(it.key, it.data);
	}

	UNIGINE_INLINE void appendFast(BiMap &&m)
	{
		for (const auto &it: m)
			appendFast(std::move(it.key), std::move(it.data));
		m.clear();
	}

	template <typename ... Args>
	UNIGINE_INLINE const Type &emplaceFast(const Key &key, Args && ... args)
	{
		return do_append_value(Parent::do_append_node(new Node(key, nullptr, std::forward<Args>(args)...)))->data;
	}

	template <typename ... Args>
	UNIGINE_INLINE const Type &emplaceFast(Key &&key, Args && ... args)
	{
		return do_append_value(Parent::do_append_node(new Node(std::move(key), nullptr, std::forward<Args>(args)...)))->data;
	}

	template <typename T>
	UNIGINE_INLINE bool contains(const T &key) const
	{
		Node *node = Parent::root;
		while (node && (node->key == key) == 0)
			node = node->dirs[key < node->key];
		return node != nullptr;
	}

	template <typename TypeKey>
	UNIGINE_INLINE bool contains(const TypeKey &key, const Type &value) const
	{
		const Node *node = Parent::do_find(key);
		return node != nullptr && node->data == value;
	}

	template<typename TypeKey, typename TypeValue>
	Iterator replace(TypeKey &&key, TypeValue &&value)
	{
		return do_replace(std::forward<TypeKey>(key), std::forward<TypeValue>(value));
	}
	template<typename TypeValue, typename TypeKey>
	Iterator replaceData(TypeKey &&key, TypeValue &&value)
	{
		return do_replace_data(std::forward<TypeKey>(key), std::forward<TypeValue>(value));
	}

	UNIGINE_INLINE const Type &operator[](const Key &key) const { return get(key); }
	UNIGINE_INLINE const Type &get(const Key &key) const
	{
		Node *node = Parent::do_find(key);
		assert(node != nullptr && "BiMap::operator[] bad key");
		return node->data;
	}
	UNIGINE_INLINE const Key &getKey(const Type &value) const
	{
		Node *node = do_find_value(value);
		assert(node != nullptr && "BiMap::getKey bad value");
		return node->key;
	}

	template<typename T>
	UNIGINE_INLINE Iterator findData(const T &v) { return do_find_value(v); }

	template<typename T>
	UNIGINE_INLINE ConstIterator findData(const T &v) const { return do_find_value(v); }

	template<typename T>
	UNIGINE_INLINE bool containsData(const T &v) const { return do_find_value(v) != nullptr; }

	UNIGINE_INLINE bool remove(const Key &key)
	{
		Node *node = do_remove_value(Parent::do_remove(key));
		bool ret = node != nullptr;
		delete node;
		return ret;
	}

	UNIGINE_INLINE bool remove(const Iterator &it)
	{
		Node *node = do_remove_value(Parent::do_remove(it.get()));
		bool ret = node != nullptr;
		delete node;
		return ret;
	}
	UNIGINE_INLINE bool remove(const ConstIterator &it)
	{
		Node *node = do_remove_value(Parent::do_remove(it.get()));
		bool ret = node != nullptr;
		delete node;
		return ret;
	}

	UNIGINE_INLINE bool erase(const Key &key) { return remove(key); }
	UNIGINE_INLINE Iterator erase(const Iterator &it)
	{
		auto ret = it; ++ret;
		remove(it);
		return ret;
	}
	UNIGINE_INLINE ConstIterator erase(const ConstIterator &it)
	{
		auto ret = it; ++ret;
		remove(it);
		return ret;
	}

	UNIGINE_INLINE bool removeData(const Type &value)
	{
		Node *node = do_remove_value(Parent::do_remove(do_find_value(value)));
		bool ret = node != nullptr;
		delete node;
		return ret;
	}

	UNIGINE_INLINE Type take(const Key &key)
	{
		Type ret;
		take(key, ret);
		return ret;
	}

	UNIGINE_INLINE bool take(const Key &key, Type &ret)
	{
		Node *node = do_remove_value(Parent::do_remove(key));
		if (node == nullptr)
			return false;
		ret = std::move(node->data);
		delete node;
		return true;
	}

	UNIGINE_INLINE Key takeData(const Type &value)
	{
		Key ret;
		takeData(value, ret);
		return ret;
	}

	UNIGINE_INLINE bool takeData(const Type &value, Key &ret)
	{
		Node *node = do_remove_value(Parent::do_remove(do_find_value(value)));
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

	UNIGINE_INLINE Key key(const Type &value) const
	{
		Node *node = do_find_value(value);
		return node == nullptr ? Key() : node->key;
	}
	UNIGINE_INLINE Key key(const Type &value, const Key &def) const
	{
		Node *node = do_find_value(value);
		return node == nullptr ? def : node->key;
	}
	UNIGINE_INLINE const Key &keyRef(const Type &value, const Key &def) const
	{
		Node *node = do_find_value(value);
		return node == nullptr ? def : node->key;
	}

	UNIGINE_INLINE Vector<Type> values() const
	{
		Vector<Type> values;
		getValues(values);
		return std::move(values);
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

	UNIGINE_INLINE bool operator==(const BiMap &o) const
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

	UNIGINE_INLINE bool operator!=(const BiMap &o) const { return !(*this == o); }

private:

	template<typename T>
	UNIGINE_INLINE Node *do_find_value(const T &value) const
	{
		Node *node = root_value;
		while (node && (node->data == value) == 0)
			node = node->dirs_value[value < node->data];
		return node;
	}

	Node *&do_find_value_node(const Type &value, Node *&parent)
	{
		if (root_value == nullptr)
			return root_value;

		Node *node = root_value;
		int dir = -1;
		while (node && (node->data == value) == 0)
		{
			parent = node;
			node = node->dirs_value[dir = (value < node->data)];
		}
		return node == root_value ? root_value : parent->dirs_value[dir];
	}

	template<typename TypeKey, typename TypeValue>
	Node *do_replace(TypeKey &&key, TypeValue &&value)
	{
		Node *parent = nullptr;
		Node *&finded_node = Parent::do_find_node(key, parent);
		if (finded_node == nullptr)
		{
			++Parent::length;
			Node *node = new Node(std::forward<TypeKey>(key), parent, std::forward<TypeValue>(value));
			finded_node = node;
			if (parent)
				Parent::rebalance_insert(node);
			do_append_value(node);
			return node;
		}

		if (finded_node->data != value)
		{
			do_remove_value(finded_node);
			finded_node->balance_value = 0;
			Type *v = const_cast<Type *>(&(finded_node->data));
			*v = std::forward<TypeValue>(value);
			do_append_value(finded_node);
		}
		return finded_node;
	}

	template<typename TypeKey, typename TypeValue>
	Node *do_replace_data(TypeKey &&key, TypeValue &&value)
	{
		Node *parent_value = nullptr;
		Node *&finded_node = do_find_value_node(value, parent_value);
		if (finded_node == nullptr)
		{
			Node *node = new Node(std::forward<TypeKey>(key), nullptr, std::forward<TypeValue>(value));
			finded_node = node;
			node->parent_value = parent_value;
			if (parent_value)
				rebalance_insert_value(node);

			Parent::do_append_node(node);
			return node;
		}

		if (finded_node->key != key)
		{
			Parent::do_remove(finded_node);
			finded_node->balance = 0;
			Key *k = const_cast<Key *>(&(finded_node->key));
			*k = std::forward<TypeKey>(key);
			Parent::do_append_node(finded_node);
		}
		return finded_node;
	}

	template<typename TypeKey, typename ... Args>
	Node *do_replace_emplace(TypeKey &&key, Args && ... args)
	{
		Node *parent = nullptr;
		Node *&finded_node = Parent::do_find_node(key, parent);
		if (finded_node == nullptr)
		{
			++Parent::length;
			Node *node = new Node(std::forward<TypeKey>(key), parent, std::forward<Args>(args)...);
			finded_node = node;
			if (parent)
				Parent::rebalance_insert(node);
			do_append_value(node);
			return node;
		}

		Type value(std::forward<Args>(args)...);
		if (finded_node->data != value)
		{
			do_remove_value(finded_node);
			finded_node->balance_value = 0;
			Type *v = const_cast<Type *>(&(finded_node->data));
			*v = std::move(value);
			do_append_value(finded_node);
		}
		return finded_node;
	}

	Node *do_append_value(Node *data)
	{
		Node *parent = nullptr;
		Node *&finded_node = do_find_value_node(data->data, parent);
		if (finded_node == nullptr)
		{
			data->parent_value = parent;
			finded_node = data;
			if (parent)
				rebalance_insert_value(data);
			return data;
		}
		assert(0 && "BiMap::do_append_value: value already exists");
		delete data;
		return nullptr;
	}

	Node *do_remove_value(Node *node)
	{
		if (root_value == nullptr)
			return nullptr;
		if (node == nullptr)
			return nullptr;

		Node *parent = node->parent_value;
		Node *target = node->parent_value;
		Node *child;
		bool left = false;
		if (node->left_value == nullptr)
			child = node->right_value;
		else if (node->right_value == nullptr)
			child = node->left_value;
		else
		{
			target = node->right_value;
			while (target->left_value)
				target = target->left_value;

			if (parent)
				parent->dirs_value[parent->left_value == node] = target;
			else
				root_value = target;

			child = target->right_value;
			Node *targe_parent = target->parent_value;
			target->parent_value = parent;
			target->left_value = node->left_value;
			node->left_value->parent_value = target;
			target->balance_value = node->balance_value;

			left = targe_parent != node;
			if (left)
			{
				if (child)
					child->parent_value = targe_parent;
				targe_parent->left_value = child;
				target->right_value = node->right_value;
				node->right_value->parent_value = target;
				target = targe_parent;
			}

			if (root_value)
				rebalance_remove_value(target, left);
			node->parent_value = node->left_value = node->right_value = nullptr;
			return node;
		}

		if (parent)
		{
			left = parent->left_value == node;
			parent->dirs_value[left] = child;
		} else
			root_value = child;

		if (child)
			child->parent_value = parent;

		if (root_value)
			rebalance_remove_value(target, left);
		node->parent_value = node->left_value = node->right_value = nullptr;
		return node;
	}

	void rotate_left_value(Node *n)
	{
		Node *parent = n->parent_value;
		Node *right = n->right_value;
		n->right_value = right->left_value;
		if (n->right_value)
			n->right_value->parent_value = n;

		n->parent_value = right;
		right->left_value = n;
		right->parent_value = parent;
		if (parent == nullptr)
		{
			root_value = right;
			return;
		}
		parent->dirs_value[parent->left_value == n] = right;
	}

	void rotate_right_value(Node *n)
	{
		Node *parent = n->parent_value;
		Node *left = n->left_value;
		n->left_value = left->right_value;
		if (n->left_value)
			n->left_value->parent_value = n;

		n->parent_value = left;
		left->right_value = n;
		left->parent_value = parent;
		if (parent == nullptr)
		{
			root_value = left;
			return;
		}
		parent->dirs_value[parent->left_value == n] = left;
	}

	void rotate_left_right_value(Node *parent)
	{
		rotate_left_value(parent->left_value);
		rotate_right_value(parent);
	}

	void rotate_right_left_value(Node *parent)
	{
		rotate_right_value(parent->right_value);
		rotate_left_value(parent);
	}

	void rebalance_insert_value(Node *node)
	{
		Node *parent = node;
		do
		{
			node = parent;
			parent = parent->parent_value;
			if (parent->left_value == node)
			{
				parent->balance_value--;
				if (parent->balance_value == 0)
					return;
				if (parent->balance_value == -1)
					continue;

				if (node->balance_value == -1)
				{
					node->balance_value = parent->balance_value = 0;
					rotate_right_value(parent);
					return;
				}

				Node *child = node->right_value;
				parent->balance_value = node->balance_value = 0;
				if (child->balance_value == -1) parent->balance_value = 1;
				else if (child->balance_value == 1) node->balance_value = -1;
				child->balance_value = 0;

				rotate_left_right_value(parent);
				return;
			} else
			{
				parent->balance_value++;
				if (parent->balance_value == 0)
					return;
				if (parent->balance_value == 1)
					continue;

				if (node->balance_value == 1)
				{
					node->balance_value = parent->balance_value = 0;
					rotate_left_value(parent);
					return;
				}

				Node *child = node->left_value;
				parent->balance_value = node->balance_value = 0;
				if (child->balance_value == 1) parent->balance_value = -1;
				else if (child->balance_value == -1) node->balance_value = 1;
				child->balance_value = 0;

				rotate_right_left_value(parent);
				return;
			}
		} while (parent->parent_value);
	}

	void rebalance_remove_value(Node *node, bool left)
	{
		for (Node *parent; node; node = parent)
		{
			parent = node->parent_value;
			if (left)
			{
				node->balance_value++;
				if (node->balance_value == 1)
					break;
				if (parent && parent->right_value == node)
					left = false;
				if (node->balance_value == 0)
					continue;

				Node *child = node->right_value;

				if (child->balance_value == 0)
				{
					node->balance_value = 1;
					child->balance_value = -1;
					rotate_left_value(node);
					break;
				}
				if (child->balance_value == 1)
				{
					node->balance_value = child->balance_value = 0;
					rotate_left_value(node);
					continue;
				}

				Node *child_left = child->left_value;
				node->balance_value = child->balance_value = 0;
				if (child_left->balance_value == 1)
					node->balance_value = -1;
				else if (child_left->balance_value == -1)
					child->balance_value = 1;

				child_left->balance_value = 0;

				rotate_right_left_value(node);
			} else
			{
				node->balance_value--;
				if (node->balance_value == -1)
					break;
				if (parent && parent->left_value == node)
					left = true;
				if (node->balance_value == 0)
					continue;

				Node *child = node->left_value;
				if (child->balance_value == 0)
				{
					node->balance_value = -1;
					child->balance_value = 1;
					rotate_right_value(node);
					break;
				}

				if (child->balance_value == -1)
				{
					node->balance_value = child->balance_value = 0;
					rotate_right_value(node);
					continue;
				}

				Node *child_right = child->right_value;
				node->balance_value = child->balance_value = 0;
				if (child_right->balance_value == -1)
					node->balance_value = 1;
				else if (child_right->balance_value == 1)
					child->balance_value = -1;

				child_right->balance_value = 0;

				rotate_left_right_value(node);
			}
		}
	}

private:
	Node *root_value;

};

}
