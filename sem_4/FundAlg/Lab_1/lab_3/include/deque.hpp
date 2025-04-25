#pragma once

#include "list.hpp"
#include <stdexcept>
#include <initializer_list>

namespace my_container {

template <typename T>
class Deque : public List<T> {
public:
	// Конструктор по умолчанию
	Deque() = default;

	// Конструктор копирования
	Deque(const Deque& other) : List<T>(other) {}

	// Конструктор перемещения
	Deque(Deque&& other) noexcept : List<T>(std::move(other)) {}

	// Конструктор со списком инициализации
	Deque(std::initializer_list<T> init) : List<T>(init) {}

	// Деструктор
	~Deque() override = default;

	// Оператор копирующего присваивания
	Deque& operator=(const Deque& other) {
		List<T>::operator=(other);
		return *this;
	}

	// Оператор перемещающего присваивания
	Deque& operator=(Deque&& other) noexcept {
		List<T>::operator=(std::move(other));
		return *this;
	}

	// Метод at с проверкой границ
	T& at(size_t index) {
		if (index >= this->size()) {
			throw std::out_of_range("Index out of range");
		}
		auto it = this->begin();
		std::advance(it, index);
		return *it;
	}

	const T& at(size_t index) const {
		if (index >= this->size()) {
			throw std::out_of_range("Index out of range");
		}
		auto it = this->cbegin();
		std::advance(it, index);
		return *it;
	}

	// Оператор [] без проверки границ
	T& operator[](size_t index) {
		auto it = this->begin();
		std::advance(it, index);
		return *it;
	}

	const T& operator[](size_t index) const {
		auto it = this->cbegin();
		std::advance(it, index);
		return *it;
	}
};

} // namespace my_container
