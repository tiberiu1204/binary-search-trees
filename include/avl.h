//
// Created by Voicu Ioan Vladut on 30.04.2024.
//

#ifndef BINARY_SEARCH_TREES_AVL_H
#define BINARY_SEARCH_TREES_AVL_H


#include "bst.h"

template <typename T>
class AVLTree : public BinarySearchTree<T> {
public:
    using iterator = typename AVLTree<T>::iterator;
    AVLTree() = default;
    explicit AVLTree(std::vector<T> values);
    iterator insert(const T &value) override;
    void remove(const T &value) override;
private:
    using Node = typename BinarySearchTree<T>::Node;
    using restricted_iterator = typename BinarySearchTree<T>::restricted_iterator;
private:
    int left_subtree_height(Node *node);
    int right_subtree_height(Node *node);
    int balance_factor(Node *node);
    Node *left_rotete(Node *node);
    Node *right_rotate(Node *node);
};

template<typename T>
AVLTree<T>::Node *AVLTree<T>::left_rotete(AVLTree::Node *node)
{
    Node *right = node->right();
    node->right = right->left();
    right->left = node;
    return right;
}

template<typename T>
AVLTree<T>::Node *AVLTree<T>::right_rotate(AVLTree::Node *node)
{
    Node *left = node->left();
    node->left = left->right();
    left->right = node;
    return left;
}

template<typename T>
int AVLTree<T>::left_subtree_height(AVLTree::Node *node)
{
    Node &left = node->left();
    if (!node->has_left()) return 0;
    else {
        return std::max(left_subtree_height(left), right_subtree_height(left)) + 1;
    }
}

template<typename T>
int AVLTree<T>::right_subtree_height(AVLTree::Node *node)
{
    Node &right = node->right();
    if (!node->has_left()) return 0;
    else {
        return std::max(left_subtree_height(right), right_subtree_height(right)) + 1;
    }
}

template<typename T>
int AVLTree<T>::balance_factor(AVLTree::Node *node)
{
    return left_subtree_height(node) - right_subtree_height(node);
}

template<typename T>
void AVLTree<T>::remove(const T &value)
{
}

template<typename T>
AVLTree<T>::iterator AVLTree<T>::insert(const T &value)
{
}

template<typename T>
AVLTree<T>::AVLTree(std::vector<T> values)
{
    for(auto value : values) {
        this->insert(value);
    }
}


#endif //BINARY_SEARCH_TREES_AVL_H
