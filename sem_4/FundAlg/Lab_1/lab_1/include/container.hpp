#pragma once

#include <cstddef>

template <typename T>
class Container {
public:
	using iterator = T*;
	using const_iterator = const T*;
	Container() = default;
	Container(const Container& container) = default;
	Container& operator=(const Container& container) = default;
	virtual ~Container() = default;

	bool operator==(const Container& other) const = default;
	bool operator!=(const Container& other) const = default;

	virtual size_t size() const = 0;
	virtual size_t max_size() const = 0;
	virtual bool empty() const = 0;
};