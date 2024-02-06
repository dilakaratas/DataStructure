#ifndef HOMEWORK3_BST_H
#define HOMEWORK3_BST_H

#include <iostream>

using namespace std;

template <class Key, class Value>
class BSTNode {
public:
    Key key;
    Value value;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const Key& k, const Value& v)
            : key(k), value(v), left(nullptr), right(nullptr) {}
};

template <class Key, class Value>
class BinarySearchTree {
private:
    BSTNode<Key, Value>* root;
    BSTNode<Key, Value>* insertRecursive(BSTNode<Key, Value>* node, Key key, Value value);
    BSTNode<Key, Value>* removeRecursive(BSTNode<Key, Value>* node, Key key);
    BSTNode<Key, Value>* searchRecursive(BSTNode<Key, Value>* node, Key key);
    void destroyTree(BSTNode<Key, Value>* node);

public:
    BinarySearchTree();
    ~BinarySearchTree();
    BSTNode<Key, Value>* getRoot();
    void insert(const Key& key, const Value& value);
    void remove(const Key& key);
    Value search(const Key& key);
};

//constructor
template <class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() : root(nullptr) {}

//destructor
template <class Key, class Value>
BinarySearchTree<Key, Value>::~BinarySearchTree() {
    destroyTree(root);
}

//get root
template <class Key, class Value>
BSTNode<Key, Value>* BinarySearchTree<Key, Value>::getRoot() {
    return root;
}


template <class Key, class Value>
void BinarySearchTree<Key, Value>::insert(const Key& key, const Value& value) {
    root = insertRecursive(root, key, value);
}

template <class Key, class Value>
void BinarySearchTree<Key, Value>::remove(const Key& key) {
    root = removeRecursive(root, key);
}

template <class Key, class Value>
BSTNode<Key, Value>* BinarySearchTree<Key, Value>::insertRecursive(BSTNode<Key, Value>* node, Key key, Value value) {
    if (node == nullptr) {
        return new BSTNode<Key, Value>(key, value);
    }

    if (key < node->key) { // if smaller
        node->left = insertRecursive(node->left, key, value);
    }
    else if (key > node->key) { // if larger
        node->right = insertRecursive(node->right, key, value);
    }
    else {
        node->value->Update(value);
    }

    return node;
}
//same logic
template <class Key, class Value>
BSTNode<Key, Value>* BinarySearchTree<Key, Value>::removeRecursive(BSTNode<Key, Value>* node, Key key) {
    if (node == nullptr) {
        return node;
    }

    if (key < node->key) {
        node->left = removeRecursive(node->left, key);
    }
    else if (key > node->key) {
        node->right = removeRecursive(node->right, key);
    }
    else {
        if (node->left == nullptr) {
            BSTNode<Key, Value>* temp = node->right;
            delete node;
            return temp;
        }
        else if (node->right == nullptr) {
            BSTNode<Key, Value>* temp = node->left;
            delete node;
            return temp;
        }


        BSTNode<Key, Value>* temp = node->right;
        while (temp && temp->left != nullptr) {
            temp = temp->left;
        }

        node->key = temp->key;
        node->value = temp->value;


        node->right = removeRecursive(node->right, temp->key);
    }
    return node;
}

template <class Key, class Value>
Value BinarySearchTree<Key, Value>::search(const Key& key) {
    BSTNode<Key, Value>* node = searchRecursive(root, key);
    if (node != nullptr)
        return node->value;
    return nullptr;
}

template <class Key, class Value>
BSTNode<Key, Value>* BinarySearchTree<Key, Value>::searchRecursive(BSTNode<Key, Value>* node, Key key) {
    if (node == nullptr) return nullptr;
    if (key < node->key) return searchRecursive(node->left, key);
    if (key > node->key) return searchRecursive(node->right, key);
    return node;
}

// deleting left right and node
template <class Key, class Value>
void BinarySearchTree<Key, Value>::destroyTree(BSTNode<Key, Value>* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

#endif //HOMEWORK3_BST_H
