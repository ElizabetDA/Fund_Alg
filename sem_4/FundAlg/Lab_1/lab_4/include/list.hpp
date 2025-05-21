#pragma once
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <algorithm>
#include "container.hpp"

namespace my_container {

template <typename T>
class List : public Container<T> {
private:
    struct Node {
        T value;
        Node* prev;
        Node* next;

        Node(const T& val) : value(val), prev(nullptr), next(nullptr) {}
        Node(T&& val) : value(std::move(val)), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t count;

public:
    List() : head(nullptr), tail(nullptr), count(0) {}

    List(const List& other) : head(nullptr), tail(nullptr), count(0) {
        for (Node* curr = other.head; curr != nullptr; curr = curr->next)
            push_back(curr->value);
    }

    List(List&& other) noexcept
        : head(other.head), tail(other.tail), count(other.count) {
        other.head = nullptr;
        other.tail = nullptr;
        other.count = 0;
    }

    List(std::initializer_list<T> init) : head(nullptr), tail(nullptr), count(0) {
        for (const auto& item : init)
            push_back(item);
    }

    ~List() {
        clear();
    }

    List& operator=(const List& other) {
        if (this != &other) {
            clear();
            for (Node* curr = other.head; curr != nullptr; curr = curr->next)
                push_back(curr->value);
        }
        return *this;
    }

    List& operator=(List&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            count = other.count;

            other.head = nullptr;
            other.tail = nullptr;
            other.count = 0;
        }
        return *this;
    }

    void clear() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        count = 0;
    }

    void push_back(const T& value) {
        Node* node = new (std::nothrow) Node(value);
        if (!node) throw std::bad_alloc();
        node->prev = tail;
        if (tail) tail->next = node;
        else head = node;
        tail = node;
        ++count;
    }

    void push_back(T&& value) {
        Node* node = new (std::nothrow) Node(std::move(value));
        if (!node) throw std::bad_alloc();
        node->prev = tail;
        if (tail) tail->next = node;
        else head = node;
        tail = node;
        ++count;
    }

    void push_front(const T& value) {
        Node* node = new (std::nothrow) Node(value);
        if (!node) throw std::bad_alloc();
        node->next = head;
        if (head) head->prev = node;
        else tail = node;
        head = node;
        ++count;
    }

    void push_front(T&& value) {
        Node* node = new (std::nothrow) Node(std::move(value));
        if (!node) throw std::bad_alloc();
        node->next = head;
        if (head) head->prev = node;
        else tail = node;
        head = node;
        ++count;
    }

    void pop_back() {
        if (!tail) throw std::out_of_range("List is empty");
        Node* to_delete = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete to_delete;
        --count;
    }

    void pop_front() {
        if (!head) throw std::out_of_range("List is empty");
        Node* to_delete = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete to_delete;
        --count;
    }

    bool empty() const noexcept {
        return count == 0;
    }

    size_t size() const noexcept {
        return count;
    }

	size_t max_size() const override {
    	return static_cast<size_t>(-1) / sizeof(Node);
    }

    T& front() {
        if (!head) throw std::out_of_range("List is empty");
        return head->value;
    }

    const T& front() const {
        if (!head) throw std::out_of_range("List is empty");
        return head->value;
    }

    T& back() {
        if (!tail) throw std::out_of_range("List is empty");
        return tail->value;
    }

    const T& back() const {
        if (!tail) throw std::out_of_range("List is empty");
        return tail->value;
    }

    class Iterator {
        Node* current;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit Iterator(Node* node) : current(node) {}
        reference operator*() const { return current->value; }
        pointer operator->() const { return &(current->value); }
        Iterator& operator++() { current = current->next; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        Iterator& operator--() { current = current->prev; return *this; }
        Iterator operator--(int) { Iterator tmp = *this; --(*this); return tmp; }
        bool operator==(const Iterator& other) const { return current == other.current; }
        bool operator!=(const Iterator& other) const { return current != other.current; }

        friend class List;
    };

    class ConstIterator {
        const Node* current;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        explicit ConstIterator(const Node* node) : current(node) {}
        reference operator*() const { return current->value; }
        pointer operator->() const { return &(current->value); }
        ConstIterator& operator++() { current = current->next; return *this; }
        ConstIterator operator++(int) { ConstIterator tmp = *this; ++(*this); return tmp; }
        ConstIterator& operator--() { current = current->prev; return *this; }
        ConstIterator operator--(int) { ConstIterator tmp = *this; --(*this); return tmp; }
        bool operator==(const ConstIterator& other) const { return current == other.current; }
        bool operator!=(const ConstIterator& other) const { return current != other.current; }
    };

    using iterator = Iterator;
    using const_iterator = ConstIterator;

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
    const_iterator begin() const { return const_iterator(head); }
    const_iterator end() const { return const_iterator(nullptr); }
    const_iterator cbegin() const { return const_iterator(head); }
    const_iterator cend() const { return const_iterator(nullptr); }

    iterator insert(iterator pos, const T& value) {
        if (pos == begin()) {
            push_front(value);
            return begin();
        } else if (pos == end()) {
            push_back(value);
            return iterator(tail);
        } else {
            Node* node = new (std::nothrow) Node(value);
            if (!node) throw std::bad_alloc();
            Node* current = pos.current;
            Node* prev = current->prev;

            prev->next = node;
            node->prev = prev;
            node->next = current;
            current->prev = node;

            ++count;
            return iterator(node);
        }
    }

    iterator erase(iterator pos) {
        if (pos == end()) throw std::out_of_range("Cannot erase end");
        Node* current = pos.current;
        Node* prev = current->prev;
        Node* next = current->next;

        if (prev) prev->next = next;
        else head = next;

        if (next) next->prev = prev;
        else tail = prev;

        delete current;
        --count;
        return iterator(next);
    }

    void resize(size_t new_size, const T& value = T()) {
        while (count > new_size) pop_back();
        while (count < new_size) push_back(value);
    }

    void swap(List& other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(count, other.count);
    }

    bool operator==(const List& other) const {
        if (count != other.count) return false;
        auto it1 = begin(), it2 = other.begin();
        while (it1 != end()) {
            if (!(*it1 == *it2)) return false;
            ++it1; ++it2;
        }
        return true;
    }

    bool operator!=(const List& other) const { return !(*this == other); }
    bool operator<(const List& other) const {
        return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
    }
    bool operator<=(const List& other) const { return !(other < *this); }
    bool operator>(const List& other) const { return other < *this; }
    bool operator>=(const List& other) const { return !(*this < other); }
};

} // namespace my_container