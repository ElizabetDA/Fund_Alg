#ifndef REDBLACK_HPP
#define REDBLACK_HPP

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
class RedBlack {
public:
    using callback_t = std::function<void(const Key&, const Value&)>;

private:
    enum Color { RED, BLACK };

    struct Node {
        Key key;
        Value value;
        Node* left;
        Node* right;
        Color color;
        Node(const Key& k, const Value& v)
            : key(k), value(v), left(nullptr), right(nullptr), color(RED) {}
    };

    Node* root_;
    std::size_t size_;
    Comparator comp_;

    static bool isRed(Node* x) {
        return x && x->color == RED;
    }

    Node* rotateLeft(Node* h) {
        Node* x = h->right;
        h->right = x->left;
        x->left = h;
        x->color = h->color;
        h->color = RED;
        return x;
    }

    Node* rotateRight(Node* h) {
        Node* x = h->left;
        h->left = x->right;
        x->right = h;
        x->color = h->color;
        h->color = RED;
        return x;
    }

    void flipColors(Node* h) {
        h->color = (h->color == RED ? BLACK : RED);
        if (h->left)  h->left->color  = (h->left->color  == RED ? BLACK : RED);
        if (h->right) h->right->color = (h->right->color == RED ? BLACK : RED);
    }

    Node* fixUp(Node* h) {
        if (isRed(h->right) && !isRed(h->left))       h = rotateLeft(h);
        if (isRed(h->left) && isRed(h->left->left))   h = rotateRight(h);
        if (isRed(h->left) && isRed(h->right))        flipColors(h);
        return h;
    }

    Node* moveRedLeft(Node* h) {
        flipColors(h);
        if (isRed(h->right->left)) {
            h->right = rotateRight(h->right);
            h = rotateLeft(h);
            flipColors(h);
        }
        return h;
    }

    Node* moveRedRight(Node* h) {
        flipColors(h);
        if (isRed(h->left->left)) {
            h = rotateRight(h);
            flipColors(h);
        }
        return h;
    }

    Node* put(Node* h, const Key& key, const Value& val, bool& inserted) {
        if (!h) {
            inserted = true;
            return new Node(key, val);
        }
        if (comp_(key, h->key))
            h->left  = put(h->left,  key, val, inserted);
        else if (comp_(h->key, key))
            h->right = put(h->right, key, val, inserted);
        else {
            h->value = val;
        }
        h = fixUp(h);
        return h;
    }

    Node* minNode(Node* h) const {
        return h->left ? minNode(h->left) : h;
    }

	Node* deleteMin(Node* h) {
    	if (!h->left) {
    		Node* right = h->right;
    		delete h;
    		return right;
    	}
    	if (!isRed(h->left) && !isRed(h->left->left))
    		h = moveRedLeft(h);
    	h->left = deleteMin(h->left);
    	return fixUp(h);
    }

    Node* deleteRec(Node* h, const Key& key, bool& removed) {
        if (!h) return nullptr;
        if (comp_(key, h->key)) {
            if (h->left && !isRed(h->left) && !isRed(h->left->left))
                h = moveRedLeft(h);
            h->left = deleteRec(h->left, key, removed);
        } else {
            if (isRed(h->left))
                h = rotateRight(h);
            if (!comp_(key, h->key) && !h->right) {
                removed = true;
                delete h;
                return nullptr;
            }
            if (h->right && !isRed(h->right) && !isRed(h->right->left))
                h = moveRedRight(h);
            if (!comp_(key, h->key)) {
                removed = true;
                Node* m = minNode(h->right);
                h->key = m->key;
                h->value = m->value;
                h->right = deleteMin(h->right);
            } else {
                h->right = deleteRec(h->right, key, removed);
            }
        }
        return fixUp(h);
    }

    void destroy(Node* h) {
        if (!h) return;
        destroy(h->left);
        destroy(h->right);
        delete h;
    }

    // обходы
    void inorder(Node* h, const callback_t& cb) const {
        if (!h) return;
        inorder(h->left, cb);
        cb(h->key, h->value);
        inorder(h->right, cb);
    }
    void preorder(Node* h, const callback_t& cb) const {
        if (!h) return;
        cb(h->key, h->value);
        preorder(h->left, cb);
        preorder(h->right, cb);
    }
    void postorder(Node* h, const callback_t& cb) const {
        if (!h) return;
        postorder(h->left, cb);
        postorder(h->right, cb);
        cb(h->key, h->value);
    }

public:
    RedBlack() : root_(nullptr), size_(0), comp_() {}
    ~RedBlack() { clear(); }

    void insert(const Key& k, const Value& v) {
        bool ins = false;
        root_ = put(root_, k, v, ins);
        if (root_) root_->color = BLACK;
        if (ins) ++size_;
    }

    Value* find(const Key& key) const {
        Node* h = root_;
        while (h) {
            if (comp_(key, h->key))      h = h->left;
            else if (comp_(h->key, key)) h = h->right;
            else                          return &h->value;
        }
        return nullptr;
    }

    bool contains(const Key& key) const {
        return find(key) != nullptr;
    }

    void remove(const Key& key) {
        if (!contains(key)) return;
        if (root_ && !isRed(root_->left) && !isRed(root_->right))
            root_->color = RED;
        bool rem = false;
        root_ = deleteRec(root_, key, rem);
        if (root_) root_->color = BLACK;
        if (rem) --size_;
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

    // итератор inorder
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
            proxy_.first  = n->key;
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

        Iterator& operator++()   { advance(); return *this; }
        Iterator  operator++(int){ Iterator tmp = *this; advance(); return tmp; }

        bool operator==(const Iterator& o) const { return current_ == o.current_; }
        bool operator!=(const Iterator& o) const { return !(*this == o); }
    };

    Iterator begin() const { return Iterator(root_); }
    Iterator end()   const { return Iterator(); }
};

#endif // REDBLACK_HPP