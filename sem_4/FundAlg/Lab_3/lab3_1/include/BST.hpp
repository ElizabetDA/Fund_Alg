#ifndef BST_HPP
#define BST_HPP

#include "DefaultComparator.hpp"
#include <functional>
#include <stack>
#include <cstddef>

template <
    typename Key,
    typename Value,
    typename Comparator = DefaultComparator<Key>
>
class BST {
public:
    using callback_t = std::function<void(const Key&, const Value&)>;

private:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        Node(const Key& k, const Value& v)
            : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    Node* root_;
    std::size_t size_;
    Comparator comp_;

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    void inorder(Node* node, const callback_t& cb) const {
        if (!node) return;
        inorder(node->left, cb);
        cb(node->key, node->value);
        inorder(node->right, cb);
    }
    void preorder(Node* node, const callback_t& cb) const {
        if (!node) return;
        cb(node->key, node->value);
        preorder(node->left, cb);
        preorder(node->right, cb);
    }
    void postorder(Node* node, const callback_t& cb) const {
        if (!node) return;
        postorder(node->left, cb);
        postorder(node->right, cb);
        cb(node->key, node->value);
    }

    std::pair<Node*, Node*> findNode(const Key& key) const {
        Node* parent = nullptr;
        Node* curr = root_;
        while (curr) {
            if (comp_(key, curr->key)) {
                parent = curr;
                curr = curr->left;
            } else if (comp_(curr->key, key)) {
                parent = curr;
                curr = curr->right;
            } else {
                break;
            }
        }
        return {curr, parent};
    }

public:
    BST() : root_(nullptr), size_(0), comp_() {}
    ~BST() { clear(); }

    void insert(const Key& key, const Value& value) {
        if (!root_) {
            root_ = new Node(key, value);
            size_ = 1;
            return;
        }
        Node* curr = root_;
        Node* parent = nullptr;
        while (curr) {
            parent = curr;
            if (comp_(key, curr->key))
                curr = curr->left;
            else if (comp_(curr->key, key))
                curr = curr->right;
            else {
                curr->value = value;
                return;
            }
        }
        Node* node = new Node(key, value);
        if (comp_(key, parent->key))
            parent->left = node;
        else
            parent->right = node;
        ++size_;
    }

    Value* find(const Key& key) const {
        auto [node, _] = findNode(key);
        return node ? &node->value : nullptr;
    }

    bool contains(const Key& key) const {
        return find(key) != nullptr;
    }

    void remove(const Key& key) {
        auto [node, parent] = findNode(key);
        if (!node) return;

        if (node->left && node->right) {
            Node* succ_parent = node;
            Node* succ = node->right;
            while (succ->left) {
                succ_parent = succ;
                succ = succ->left;
            }
            node->key   = succ->key;
            node->value = succ->value;
            // переназначаем удаляемый узел к successor
            parent = succ_parent;
            node   = succ;
        }

        Node* child = node->left ? node->left : node->right;
        if (!parent) {
            root_ = child;
        } else if (parent->left == node) {
            parent->left = child;
        } else {
            parent->right = child;
        }
        delete node;
        --size_;
    }

    void clear() {
        destroy(root_);
        root_ = nullptr;
        size_ = 0;
    }

    std::size_t size() const { return size_; }

    void inorder(const callback_t& cb) const { inorder(root_, cb); }
    void preorder(const callback_t& cb) const { preorder(root_, cb); }
    void postorder(const callback_t& cb) const { postorder(root_, cb); }

    // Итератор inorder
    class Iterator {
    public:
        struct Proxy {
            Key   first;
            Value second;
        };

        using iterator_category = std::forward_iterator_tag;
        using value_type        = Proxy;
        using reference         = Proxy&;
        using pointer           = Proxy*;

    private:
        Node* current_;
        std::stack<Node*> st_;
        Proxy proxy_;

        void pushLeft(Node* node) {
            while (node) {
                st_.push(node);
                node = node->left;
            }
        }

        void advance() {
            if (st_.empty()) {
                current_ = nullptr;
            } else {
                Node* node = st_.top(); st_.pop();
                current_ = node;
                if (node->right)
                    pushLeft(node->right);
                proxy_.first  = node->key;
                proxy_.second = node->value;
            }
        }

    public:
        Iterator(Node* root) : current_(nullptr) {
            pushLeft(root);
            advance();
        }
        Iterator() : current_(nullptr) {}

        reference operator*() const { return const_cast<reference>(proxy_); }
        pointer   operator->() const { return const_cast<pointer>(&proxy_); }

        Iterator& operator++() {
            advance();
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            advance();
            return tmp;
        }

        bool operator==(const Iterator& o) const { return current_ == o.current_; }
        bool operator!=(const Iterator& o) const { return !(*this == o); }
    };

    Iterator begin() const { return Iterator(root_); }
    Iterator end()   const { return Iterator(); }
};

#endif // BST_HPP
