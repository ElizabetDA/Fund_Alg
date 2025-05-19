#ifndef MYMAP_HPP
#define MYMAP_HPP

#include <cstddef>
#include "DefaultComparator.hpp"

template <
	typename Key,
	typename Value,
	template<typename, typename, typename> class TreeType,
	typename Comparator = DefaultComparator<Key>
>
class MyMap {
private:
	TreeType<Key, Value, Comparator> tree_;

public:
	void insert(const Key& k, const Value& v) { tree_.insert(k, v); }
	Value* find(const Key& k)                  { return tree_.find(k); }
	bool contains(const Key& k) const          { return tree_.contains(k); }
	void remove(const Key& k)                  { tree_.remove(k); }
	void clear()                               { tree_.clear(); }
	std::size_t size() const                   { return tree_.size(); }

	auto begin()       { return tree_.begin(); }
	auto end()         { return tree_.end();   }
	auto begin() const { return tree_.begin(); }
	auto end()   const { return tree_.end();   }

	template<typename F>
	void inorder(F f) const  { tree_.inorder(f); }
	template<typename F>
	void preorder(F f) const { tree_.preorder(f); }
	template<typename F>
	void postorder(F f) const{ tree_.postorder(f); }
};

#endif // MYMAP_HPP
