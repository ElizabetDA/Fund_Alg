#ifndef AVL_HPP
#define AVL_HPP

#include "DefaultComparator.hpp"
#include <functional>
#include <stack>
#include <cstddef>
#include <algorithm>

template <
    typename Key,
    typename Value,
    typename Comparator = DefaultComparator<Key>
>
class AVL {
public:
    using callback_t = std::function<void(const Key&, const Value&)>;

private:
    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        int height;
        Node(const Key& k, const Value& v)
            : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root_;
    std::size_t size_;
    Comparator comp_;

    int height(Node* n) const { return n ? n->height : 0; }
    int balanceFactor(Node* n) const { return n ? height(n->left) - height(n->right) : 0; }
    void updateHeight(Node* n) {
        n->height = 1 + std::max(height(n->left), height(n->right));
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* rebalance(Node* node) {
        updateHeight(node);
        int bf = balanceFactor(node);
        if (bf > 1) {
            if (balanceFactor(node->left) < 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (bf < -1) {
            if (balanceFactor(node->right) > 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }

    Node* insertRec(Node* node, const Key& key, const Value& value, bool& inserted) {
        if (!node) {
            inserted = true;
            return new Node(key, value);
        }
        if (comp_(key, node->key))
            node->left  = insertRec(node->left, key, value, inserted);
        else if (comp_(node->key, key))
            node->right = insertRec(node->right, key, value, inserted);
        else {
            node->value = value;
            return node;
        }
        return rebalance(node);
    }

    Node* findMin(Node* node) const {
        return node->left ? findMin(node->left) : node;
    }

    Node* removeRec(Node* node, const Key& key, bool& removed) {
        if (!node) return nullptr;
        if (comp_(key, node->key))
            node->left = removeRec(node->left, key, removed);
        else if (comp_(node->key, key))
            node->right = removeRec(node->right, key, removed);
        else {
            removed = true;
            if (!node->left || !node->right) {
                Node* tmp = node->left ? node->left : node->right;
                delete node;
                return tmp;
            } else {
                Node* succ = findMin(node->right);
                node->key = succ->key;
                node->value = succ->value;
                node->right = removeRec(node->right, succ->key, removed);
            }
        }
        return node ? rebalance(node) : nullptr;
    }

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    // Обходы
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

    Node* findNode(Node* node, const Key& key) const {
        if (!node) return nullptr;
        if (comp_(key, node->key)) return findNode(node->left, key);
        if (comp_(node->key, key)) return findNode(node->right, key);
        return node;
    }

public:
    AVL() : root_(nullptr), size_(0), comp_() {}
    ~AVL() { clear(); }

    void insert(const Key& key, const Value& value) {
        bool inserted = false;
        root_ = insertRec(root_, key, value, inserted);
        if (inserted) ++size_;
    }

    bool contains(const Key& key) const {
        return findNode(root_, key) != nullptr;
    }

    Value* find(const Key& key) const {
        Node* p = findNode(root_, key);
        return p ? &p->value : nullptr;
    }

    void remove(const Key& key) {
        bool removed = false;
        root_ = removeRec(root_, key, removed);
        if (removed) --size_;
    }

    void clear() {
        destroy(root_);
        root_ = nullptr;
        size_ = 0;
    }

    std::size_t size() const { return size_; }

    void inorder(const callback_t& cb)  const { inorder(root_, cb); }
    void preorder(const callback_t& cb) const { preorder(root_, cb); }
    void postorder(const callback_t& cb) const { postorder(root_, cb); }

    // Итератор inorder (аналогично BST::Iterator, см. BST.hpp)
    class Iterator {
    public:
        struct Proxy { Key first; Value second; };
        using iterator_category = std::forward_iterator_tag;
        using value_type        = Proxy;
        using reference         = Proxy&;
        using pointer           = Proxy*;
    private:
        std::stack<Node*> st_;
        Node* current_;
        Proxy proxy_;
        void pushLeft(Node* n) {
            while (n) { st_.push(n); n = n->left; }
        }
        void advance() {
            if (st_.empty()) { current_ = nullptr; return; }
            Node* n = st_.top(); st_.pop();
            current_ = n;
            if (n->right) pushLeft(n->right);
            proxy_.first = n->key;
            proxy_.second = n->value;
        }
    public:
        Iterator(Node* root) : current_(nullptr) {
            pushLeft(root);
            advance();
        }
        Iterator() : current_(nullptr) {}

        reference operator*() const { return const_cast<reference>(proxy_); }
        pointer   operator->() const { return const_cast<pointer>(&proxy_); }
        Iterator& operator++() { advance(); return *this; }
        Iterator  operator++(int){ Iterator tmp=*this; advance(); return tmp; }
        bool operator==(const Iterator& o) const { return current_==o.current_; }
        bool operator!=(const Iterator& o) const { return !(*this==o); }
    };

    Iterator begin() const { return Iterator(root_); }
    Iterator end()   const { return Iterator(); }
};

#endif // AVL_HPP
