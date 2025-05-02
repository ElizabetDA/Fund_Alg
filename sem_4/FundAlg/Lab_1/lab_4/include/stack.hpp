#pragma once
#include "deque.hpp"
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace my_container {

template <typename T>
class Stack {
private:
    Deque<T> deque;

public:
    // Конструктор по умолчанию
    Stack() = default;

    // Конструктор копирования
    Stack(const Stack& other) = default;

    // Конструктор перемещения
    Stack(Stack&& other) noexcept = default;

    // Конструктор со списком инициализации
    Stack(std::initializer_list<T> init) {
        for (const auto& val : init) {
            deque.push_back(val);
        }
    }

    // Деструктор
    ~Stack() = default;

    // Оператор копирующего присваивания
    Stack& operator=(const Stack& other) = default;

    // Оператор перемещающего присваивания
    Stack& operator=(Stack&& other) noexcept = default;

    // Метод доступа к верхнему элементу
    T& top() {
        if (empty()) throw std::out_of_range("Stack is empty");
        return deque.back();
    }

    const T& top() const {
        if (empty()) throw std::out_of_range("Stack is empty");
        return deque.back();
    }

    // Емкость
    bool empty() const noexcept {
        return deque.empty();
    }

    size_t size() const noexcept {
        return deque.size();
    }

    // Модификаторы
    void push(const T& value) {
        deque.push_back(value);
    }

    void push(T&& value) {
        deque.push_back(std::move(value));
    }

    void pop() {
        if (empty()) throw std::out_of_range("Pop from empty stack");
        deque.pop_back();
    }

    void swap(Stack& other) noexcept {
        deque.swap(other.deque);
    }

    // Операторы сравнения
    bool operator==(const Stack& other) const { return deque == other.deque; }
    bool operator!=(const Stack& other) const { return deque != other.deque; }
    bool operator<(const Stack& other) const { return deque < other.deque; }
    bool operator<=(const Stack& other) const { return deque <= other.deque; }
    bool operator>(const Stack& other) const { return deque > other.deque; }
    bool operator>=(const Stack& other) const { return deque >= other.deque; }
};

} // namespace my_container