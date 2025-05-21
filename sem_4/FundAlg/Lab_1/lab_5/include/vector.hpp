#pragma once

#include "container.hpp"
#include <cstddef>
#include <stdexcept>
#include <initializer_list>
#include <utility>
#include <memory>
#include <algorithm>
#include <limits>


namespace my_container {

template <typename T>
class Vector : public Container<T> {
private:
    T* buffer;
    size_t sz;
    size_t cap;

    void check_and_grow() {
        if (sz >= cap) reserve(cap == 0 ? 1 : cap * 2);
    }

public:
    // Конструкторы
    Vector() : buffer(nullptr), sz(0), cap(0) {}

    Vector(size_t count, const T& value = T()) : buffer(new T[count]), sz(count), cap(count) {
        std::fill(buffer, buffer + count, value);
    }

    Vector(std::initializer_list<T> init) : Vector(init.size()) {
        std::copy(init.begin(), init.end(), buffer);
    }

    Vector(const Vector& other) : Vector(other.sz) {
        std::copy(other.buffer, other.buffer + other.sz, buffer);
    }

    Vector(Vector&& other) noexcept : buffer(other.buffer), sz(other.sz), cap(other.cap) {
        other.buffer = nullptr;
        other.sz = other.cap = 0;
    }

    ~Vector() {
        delete[] buffer;
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector tmp(other);
            swap(tmp);
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] buffer;
            buffer = other.buffer;
            sz = other.sz;
            cap = other.cap;
            other.buffer = nullptr;
            other.sz = other.cap = 0;
        }
        return *this;
    }

    // Методы доступа
    T& operator[](size_t index) { return buffer[index]; }
    const T& operator[](size_t index) const { return buffer[index]; }

    T& at(size_t index) {
        if (index >= sz) throw std::out_of_range("Index out of range");
        return buffer[index];
    }

    const T& at(size_t index) const {
        if (index >= sz) throw std::out_of_range("Index out of range");
        return buffer[index];
    }

    T& front() { return buffer[0]; }
    const T& front() const { return buffer[0]; }

    T& back() { return buffer[sz - 1]; }
    const T& back() const { return buffer[sz - 1]; }

    T* data() noexcept { return buffer; }
    const T* data() const noexcept { return buffer; }

    // Ёмкость
    bool empty() const noexcept { return sz == 0; }
    size_t size() const noexcept override { return sz; }
    size_t capacity() const noexcept { return cap; }
	size_t max_size() const override {
    	return std::numeric_limits<size_t>::max() / sizeof(T);
    }


    void reserve(size_t new_cap) {
        if (new_cap > cap) {
            T* new_buf = new T[new_cap];
            std::move(buffer, buffer + sz, new_buf);
            delete[] buffer;
            buffer = new_buf;
            cap = new_cap;
        }
    }

    void shrink_to_fit() {
        if (sz < cap) {
            T* new_buf = new T[sz];
            std::copy(buffer, buffer + sz, new_buf);
            delete[] buffer;
            buffer = new_buf;
            cap = sz;
        }
    }

    // Модификаторы
    void clear() {
        delete[] buffer;
        buffer = nullptr;
        sz = cap = 0;
    }

    void push_back(const T& value) {
        check_and_grow();
        buffer[sz++] = value;
    }

    void push_back(T&& value) {
        check_and_grow();
        buffer[sz++] = std::move(value);
    }

    void pop_back() {
        if (empty()) throw std::out_of_range("Pop from empty vector");
        --sz;
    }

    void insert(size_t index, const T& value) {
        if (index > sz) throw std::out_of_range("Insert index out of range");
        check_and_grow();
        for (size_t i = sz; i > index; --i) {
            buffer[i] = std::move(buffer[i - 1]);
        }
        buffer[index] = value;
        ++sz;
    }

    void erase(size_t index) {
        if (index >= sz) throw std::out_of_range("Erase index out of range");
        for (size_t i = index; i < sz - 1; ++i) {
            buffer[i] = std::move(buffer[i + 1]);
        }
        --sz;
    }

    void resize(size_t count, const T& value = T()) {
        if (count < sz) {
            sz = count;
        } else {
            reserve(count);
            for (size_t i = sz; i < count; ++i) {
                buffer[i] = value;
            }
            sz = count;
        }
    }

    void swap(Vector& other) noexcept {
        std::swap(buffer, other.buffer);
        std::swap(sz, other.sz);
        std::swap(cap, other.cap);
    }

    // Операторы сравнения
    bool operator==(const Vector& other) const {
        if (sz != other.sz) return false;
        for (size_t i = 0; i < sz; ++i) {
            if (!(buffer[i] == other.buffer[i])) return false;
        }
        return true;
    }

    bool operator!=(const Vector& other) const {
        return !(*this == other);
    }

    bool operator<(const Vector& other) const {
        return std::lexicographical_compare(buffer, buffer + sz, other.buffer, other.buffer + other.sz);
    }

    bool operator<=(const Vector& other) const {
        return !(other < *this);
    }

    bool operator>(const Vector& other) const {
        return other < *this;
    }

    bool operator>=(const Vector& other) const {
        return !(*this < other);
    }
};

} // namespace my_container
