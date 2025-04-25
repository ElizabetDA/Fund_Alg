#pragma once
#include "container.hpp"
#include <algorithm>
#include <stdexcept>
#include <initializer_list>
#include <iterator>
#include <cstddef>

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
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = size_t;

    // Конструкторы
    Array() = default;

    Array(std::initializer_list<T> init) {
        if (init.size() != N) {
            throw std::invalid_argument("Initializer list size mismatch");
        }
        std::copy(init.begin(), init.end(), elements);
    }

    //~Array() override = default;

    Array(const Array& other) {
        std::copy(other.begin(), other.end(), elements);
    }

    Array(Array&& other) noexcept {
        std::move(other.begin(), other.end(), elements);
    }

    // Операторы присваивания
    Array& operator=(const Array& other) {
        if (this != &other) {
            std::copy(other.begin(), other.end(), elements);
        }
        return *this;
    }

    Container<T>& operator=(const Container<T>& other) {
        const Array* p_other = dynamic_cast<const Array*>(&other);
        if (!p_other) throw std::invalid_argument("Type mismatch");
        return *this = *p_other;
    }

    // Доступ к элементам
    T& at(size_t index) {
        if (index >= N) throw std::out_of_range("Index out of range");
        return elements[index];
    }

    const T& at(size_t index) const {
        if (index >= N) throw std::out_of_range("Index out of range");
        return elements[index];
    }

    T& operator[](size_t index) noexcept { return elements[index]; }
    const T& operator[](size_t index) const noexcept { return elements[index]; }

    T& front() noexcept { return elements[0]; }
    const T& front() const noexcept { return elements[0]; }

    T& back() noexcept { return elements[N - 1]; }
    const T& back() const noexcept { return elements[N - 1]; }

    T* data() noexcept { return elements; }
    const T* data() const noexcept { return elements; }

    // Итераторы
    iterator begin() noexcept { return elements; }
    const_iterator begin() const noexcept { return elements; }
    const_iterator cbegin() const noexcept { return elements; }

    iterator end() noexcept { return elements + N; }
    const_iterator end() const noexcept { return elements + N; }
    const_iterator cend() const noexcept { return elements + N; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

    // Размеры
    [[nodiscard]] size_t size() const noexcept { return N; }
    [[nodiscard]] size_t max_size() const noexcept { return N; }
    [[nodiscard]] bool empty() const noexcept { return N == 0; }

    // Утилиты
    void fill(const T& value) { std::fill(begin(), end(), value); }
    void swap(Array& other) noexcept {
        std::swap_ranges(begin(), end(), other.begin());
    }

    // Сравнения
    [[nodiscard]] bool operator==(const Container<T>& other) const {
        const Array* p_other = dynamic_cast<const Array*>(&other);
        return p_other && std::equal(begin(), end(), p_other->begin());
    }

    [[nodiscard]] bool operator!=(const Container<T>& other) const {
        return !(*this == other);
    }

    [[nodiscard]] bool operator==(const Array& other) const {
        return std::equal(begin(), end(), other.begin());
    }

    [[nodiscard]] std::strong_ordering operator<=>(const Array& other) const {
        return std::lexicographical_compare_three_way(
            begin(), end(), other.begin(), other.end()
        );
    }
};

} // namespace my_container