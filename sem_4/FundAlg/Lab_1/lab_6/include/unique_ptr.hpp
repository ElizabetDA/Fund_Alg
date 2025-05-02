#pragma once
#include <cstddef>
#include <utility>
#include <type_traits>

namespace my_smart_ptr {

template <typename T>
class UniquePtr {
private:
    T* ptr;

public:
    // Конструкторы
    UniquePtr() noexcept : ptr(nullptr) {}
    explicit UniquePtr(T* p) noexcept : ptr(p) {}

    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    // Запрещаем копирование
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // Перемещающее присваивание
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    ~UniquePtr() {
        delete ptr;
    }

    // Методы доступа
    T* get() const noexcept { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const noexcept { return ptr; }
    explicit operator bool() const noexcept { return ptr != nullptr; }

    // Модификаторы
    void reset(T* p = nullptr) noexcept {
        delete ptr;
        ptr = p;
    }

    T* release() noexcept {
        T* old = ptr;
        ptr = nullptr;
        return old;
    }

    void swap(UniquePtr& other) noexcept {
        std::swap(ptr, other.ptr);
    }
};

// Специализация для массивов
template <typename T>
class UniquePtr<T[]> {
private:
    T* ptr;

public:
    UniquePtr() noexcept : ptr(nullptr) {}
    explicit UniquePtr(T* p) noexcept : ptr(p) {}

    UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete[] ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    ~UniquePtr() {
        delete[] ptr;
    }

    T* get() const noexcept { return ptr; }
    T& operator[](std::size_t index) const { return ptr[index]; }
    explicit operator bool() const noexcept { return ptr != nullptr; }

    void reset(T* p = nullptr) noexcept {
        delete[] ptr;
        ptr = p;
    }

    T* release() noexcept {
        T* old = ptr;
        ptr = nullptr;
        return old;
    }

    void swap(UniquePtr& other) noexcept {
        std::swap(ptr, other.ptr);
    }
};

} // namespace my_smart_ptr