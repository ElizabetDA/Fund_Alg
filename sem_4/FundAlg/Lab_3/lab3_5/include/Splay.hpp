#ifndef SPLAY_HPP
#define SPLAY_HPP

#include "DefaultComparator.hpp"
#include <functional>
#include <stack>
#include <cstddef>
#include <utility>

template <
    typename Key,
    typename Value,
    typename Comparator = DefaultComparator<Key>
>
class Splay {
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

    // Развороты
    Node* rightRotate(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        y->right = x;
        return y;
    }
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    // Топ-даун сплей
	// Топ-даун сплей
	Node* splay(Node* root, const Key& key) {
    	if (!root) return nullptr;

    	// фиктивный узел, к нему будем "присоединять" левые и правые поддеревья
    	Node dummy{ Key(), Value() };
    	Node* leftTree  = &dummy;
    	Node* rightTree = &dummy;

    	Node* curr = root;
    	while (true) {
    		if (comp_(key, curr->key)) {
    			// идём в левое поддерево
    			if (!curr->left) break;
    			// двойной поворот вправо?
    			if (comp_(key, curr->left->key)) {
    				curr = rightRotate(curr);
    				if (!curr->left) break;
    			}
    			// присоединили curr в правое "поддерево"
    			rightTree->left = curr;
    			rightTree = curr;
    			curr = curr->left;
    		}
    		else if (comp_(curr->key, key)) {
    			// идём в правое поддерево
    			if (!curr->right) break;
    			// двойной поворот влево?
    			if (comp_(curr->right->key, key)) {
    				curr = leftRotate(curr);
    				if (!curr->right) break;
    			}
    			// присоединили curr в левое "поддерево"
    			leftTree->right = curr;
    			leftTree = curr;
    			curr = curr->right;
    		}
    		else {
    			break;
    		}
    	}

    	// собираем всё обратно:
    	// leftTree.right — узлы < key
    	// rightTree.left — узлы > key
    	leftTree->right  = curr->left;
    	rightTree->left  = curr->right;
    	curr->left       = dummy.right;
    	curr->right      = dummy.left;

    	return curr;
    }

    // Вставка
    Node* insertRec(Node* root, const Key& key, const Value& value, bool& inserted) {
        if (!root) {
            inserted = true;
            return new Node(key, value);
        }
        root = splay(root, key);
        // если ключ уже есть — просто обновляем
        if (!comp_(key, root->key) && !comp_(root->key, key)) {
            root->value = value;
            return root;
        }
        // иначе создаём новый корень
        Node* node = new Node(key, value);
        if (comp_(key, root->key)) {
            node->right = root;
            node->left  = root->left;
            root->left  = nullptr;
        } else {
            node->left  = root;
            node->right = root->right;
            root->right = nullptr;
        }
        inserted = true;
        return node;
    }

    // Удаление
    Node* removeRec(Node* root, const Key& key, bool& removed) {
        if (!root) return nullptr;
        root = splay(root, key);
        if (comp_(key, root->key) || comp_(root->key, key)) {
            // ключ не найден
            return root;
        }
        removed = true;
        Node* temp;
        if (!root->left) {
            temp = root->right;
        } else {
            // сплеем в левой поддереве, чтобы новый корень имел no правого
            temp = splay(root->left, key);
            temp->right = root->right;
        }
        delete root;
        return temp;
    }

    // Inorder-обход
    void inorder(Node* node, const callback_t& cb) const {
        if (!node) return;
        inorder(node->left, cb);
        cb(node->key, node->value);
        inorder(node->right, cb);
    }

    // Удаление всего дерева
    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    Splay() : root_(nullptr), size_(0), comp_() {}
    ~Splay() { clear(); }

    void insert(const Key& key, const Value& value) {
        bool inserted = false;
        root_ = insertRec(root_, key, value, inserted);
        if (inserted) ++size_;
    }

    Value* find(const Key& key) {
        root_ = splay(root_, key);
        if (!root_ || comp_(key, root_->key) || comp_(root_->key, key))
            return nullptr;
        return &root_->value;
    }

    bool contains(const Key& key) const {
    	return const_cast<Splay*>(this)->find(key) != nullptr;
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

    std::size_t size() const {
        return size_;
    }

    void inorder(const callback_t& cb) const {
        inorder(root_, cb);
    }

    // Итератор inorder (как у BST)
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
            if (st_.empty()) {
                current_ = nullptr;
                return;
            }
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

#endif // SPLAY_HPP
