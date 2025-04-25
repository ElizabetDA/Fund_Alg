
#pragma once

#include <algorithm>
#include <compare>
#include <iostream>
#include <iterator>

#include "container.hpp"

namespace my_container {

template <typename T, size_t N>
class Array final : public Container<T> {
   private:
	T elements[N]{};

   public:
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;
	using iterator = T*;
	using const_iterator = const T*;
	using difference_type = ptrdiff_t;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	Array() = default;
	explicit Array(std::initializer_list<T> init) ;
	~Array() = default;
	Array(const Array<T, N>& other);
	Array(Array<T, N>&& other) noexcept;
	Array<T, N>& operator=(const Array<T, N>& other);
	Array<T, N>& operator=(Array<T, N>&& other) noexcept;

	T& at(size_t index);
	const T& at(size_t index) const;
	T& operator[](size_t index);
	const T& operator[](size_t index) const;
	T& front();
	const T& front() const;
	T& back();
	const T& back() const;
	T* data() noexcept;
	const T* data() const noexcept;

	iterator begin() noexcept;
	const_iterator cbegin() const noexcept;
	iterator end() noexcept;
	const_iterator cend() const noexcept;

	reverse_iterator rbegin() noexcept;
	const_reverse_iterator crbegin() const noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator crend() const noexcept;

	bool empty() const noexcept override;
	size_t size() const noexcept override;
	size_t max_size() const noexcept override;

	void fill(const T& value);
	void swap(Array& other) noexcept;

	bool operator==(const Array<T, N>& rhs) const;
	std::strong_ordering operator<=>(const Array<T, N>& rhs) const;
};


template <typename T, size_t N>
Array<T, N>::Array(std::initializer_list<T> init) {
	size_t copy_size = std::min(init.size(), N);
	std::copy_n(init.begin(), copy_size, elements);
}

template <typename T, size_t N>
Array<T, N>::Array(const Array<T, N>& other) {
	std::copy(other.cbegin(), other.cend(), this->begin());
}

template <typename T, size_t N>
Array<T, N>::Array(Array<T, N>&& other) noexcept {
	std::move(other.begin(), other.end(), this->begin());
}

template <typename T, size_t N>
Array<T, N>& Array<T, N>::operator=(const Array<T, N>& other) {
	if (this != &other) {
		std::copy(other.cbegin(), other.cend(), this->begin());
	}
	return *this;
}

template <typename T, size_t N>
Array<T, N>& Array<T, N>::operator=(Array<T, N>&& other)  noexcept {
	if (this != &other) {
		std::move(other.cbegin(), other.cend(), this->begin());
	}
	return *this;
}

template <typename T, size_t N>
T& Array<T, N>::at(size_t index) {
	if (index >= N) {
		throw std::out_of_range("Index out of range");
	}
	return elements[index];
}

template <typename T, size_t N>
const T& Array<T, N>::at(size_t index) const {
	if (index >= N) {
		throw std::out_of_range("Index out of range");
	}
	return elements[index];
}

template <typename T, size_t N>
T& Array<T, N>::operator[](size_t index) {
	return elements[index];
}

template <typename T, size_t N>
const T& Array<T, N>::operator[](size_t index) const {
	return elements[index];
}

template <typename T, size_t N>
T& Array<T, N>::front() {
	return elements[0];
}

template <typename T, size_t N>
const T& Array<T, N>::front() const {
	return elements[0];
}

template <typename T, size_t N>
T& Array<T, N>::back() {
	return elements[N - 1];
}

template <typename T, size_t N>
const T& Array<T, N>::back() const {
	return elements[N - 1];
}

template <typename T, size_t N>
T* Array<T, N>::data() noexcept {
	return elements;
}

template <typename T, size_t N>
const T* Array<T, N>::data() const noexcept {
	return elements;
}

template <typename T, size_t N>
typename Array<T, N>::iterator Array<T, N>::begin() noexcept {
	return elements;
}

template <typename T, size_t N>
typename Array<T, N>::const_iterator Array<T, N>::cbegin() const noexcept {
	return elements;
}

template <typename T, size_t N>
typename Array<T, N>::iterator Array<T, N>::end() noexcept {
	return elements + N;
}

template <typename T, size_t N>
typename Array<T, N>::const_iterator Array<T, N>::cend() const noexcept {
	return elements + N;
}

template <typename T, size_t N>
typename Array<T, N>::reverse_iterator Array<T, N>::rbegin() noexcept {
	return reverse_iterator(end());
}

template <typename T, size_t N>
typename Array<T, N>::const_reverse_iterator Array<T, N>::crbegin() const noexcept {
	return const_reverse_iterator(cend());
}

template <typename T, size_t N>
typename Array<T, N>::reverse_iterator Array<T, N>::rend() noexcept {
	return reverse_iterator(begin());
}

template <typename T, size_t N>
typename Array<T, N>::const_reverse_iterator Array<T, N>::crend() const noexcept {
	return const_reverse_iterator(cbegin());
}

template <typename T, size_t N>
bool Array<T, N>::empty() const noexcept {
	return size() == 0;
}

template <typename T, size_t N>
size_t Array<T, N>::size() const noexcept {
	return N;
}

template <typename T, size_t N>
size_t Array<T, N>::max_size() const noexcept {
	return N;
}

template <typename T, size_t N>
void Array<T, N>::fill(const T& value) {
	std::fill(begin(), end(), value);
}

template <typename T, size_t N>
void Array<T, N>::swap(Array& other) noexcept {
	for (size_t i = 0; i < N; ++i) {
		std::swap(elements[i], other.elements[i]);
	}
}

template <typename T, size_t N>
bool Array<T, N>::operator==(const Array<T, N>& rhs) const {
	return (*this <=> rhs) == std::strong_ordering::equal;
}

template <typename T, size_t N>
std::strong_ordering Array<T, N>::operator<=>(const Array<T, N>& rhs) const {
	return std::lexicographical_compare_three_way(cbegin(), cend(), rhs.cbegin(), rhs.cend());
}

}  // namespace my_container