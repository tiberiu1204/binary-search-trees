//
// Created by Voicu Ioan Vladut on 30.04.2024.
//

#ifndef BINARY_SEARCH_TREES_AVL_H
#define BINARY_SEARCH_TREES_AVL_H


#include "bst.h"
#include <iostream>

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
    int left_subtree_height(Node &node);

    int right_subtree_height(Node &node);

    int balance_factor(Node &node);

    Node &left_rotate(Node &node);

    Node &right_rotate(Node &node);

    void balance(Node &node);
};

template<typename T>
void AVLTree<T>::balance(AVLTree::Node &node)
{
    Node *node_ptr = &node;
    while (node_ptr->get_parent_index() > 0) {
        if (balance_factor(*node_ptr) >= 2 and balance_factor(node_ptr->left()) >= 0)   // left - left
            *node_ptr = right_rotate(*node_ptr);

        else if (balance_factor(*node_ptr) >= 2) {  // left - right
            node_ptr->left() = left_rotate(node_ptr->left());
            *node_ptr = right_rotate(*node_ptr);
        } else if (balance_factor(*node_ptr) <= -2 and balance_factor(node_ptr->right()) <= 0)  // right - right
            *node_ptr = left_rotate(*node_ptr);

        else if (balance_factor(*node_ptr) <= -2) {  // right - left
            node_ptr->right() = right_rotate(node_ptr->right());
            *node_ptr = left_rotate(*node_ptr);
        } else node_ptr = &node_ptr->parent();
    }
}

template<typename T>
AVLTree<T>::Node &AVLTree<T>::left_rotate(AVLTree::Node &node)
{
    Node &right_child = node.right();
    node.set_right_index(right_child.get_left_index());
    right_child.set_left_index(node.get_node_index());
    return right_child;
}

template<typename T>
AVLTree<T>::Node &AVLTree<T>::right_rotate(AVLTree::Node &node)
{
    Node &left_child = node.left();
    node.set_left_index(left_child.get_right_index());
    left_child.set_right_index(node.get_node_index());
    return left_child;
}

template<typename T>
int AVLTree<T>::left_subtree_height(AVLTree::Node &node)
{
    Node &left = node.left();
    if (!node.has_left()) return 0;
    else {
        return std::max(left_subtree_height(left), right_subtree_height(left)) + 1;
    }
}

template<typename T>
int AVLTree<T>::right_subtree_height(AVLTree::Node &node)
{
    Node &right = node.right();
    if (!node.has_right()) return 0;
    else {
        return std::max(left_subtree_height(right), right_subtree_height(right)) + 1;
    }
}

template<typename T>
int AVLTree<T>::balance_factor(AVLTree::Node &node)
{
    return left_subtree_height(node) - right_subtree_height(node);
}

template<typename T>
void AVLTree<T>::remove(const T &value)
{
    restricted_iterator it = this->lookup(value);
    if (it == this->end()) return;
    Node &node = it.get_node();
    if (!node.has_left() && !node.has_right()) {
        if (node.is_left_sibling()) node.parent().set_left_index(0);
        else node.parent().set_right_index(0);
        this->pop(node);
    } else if (!node.has_left()) {
        node.set_value(node.right().get_value());
        this->pop(node.right());
        node.set_right_index(0);
    } else if (!node.has_right()) {
        node.set_value(node.left().get_value());
        this->pop(node.left());
        node.set_left_index(0);
    } else {
        it++;
        Node &next = it.get_node();
        node.set_value(next.get_value());
        if (next.is_left_sibling()) next.parent().set_left_index(0);
        else next.parent().set_right_index(0);
        this->pop(next);
    }
    balance(node);
}

template<typename T>
AVLTree<T>::iterator AVLTree<T>::insert(const T &value)
{
    if (this->empty()) {
        this->emplace(value);
        return this->begin();
    }

    Node *node = &this->root();

    while (true) {
        if (value < node->get_value()) {
            if (!node->has_left()) {
                node->set_left_index(this->next_index());
                this->emplace(value, node->get_node_index());
                break;
            }
            node = &node->left();
        } else if (value > node->get_value()) {
            if (!node->has_right()) {
                node->set_right_index(this->next_index());
                this->emplace(value, node->get_node_index());
                break;
            }
            node = &node->right();
        }
    }
    Node &new_node = this->back();
    balance(new_node);
    return this->find(value);
}

template<typename T>
AVLTree<T>::AVLTree(std::vector<T> values)
{
    for(auto value : values) {
        this->insert(value);
    }
}

template
class AVLTree<int>;

template
class AVLTree<float>;

template
class AVLTree<double>;

template
class AVLTree<unsigned int>;

template
class AVLTree<unsigned long long>;

template
class AVLTree<long long>;

template
class AVLTree<short>;

template
class AVLTree<char>;

template
class AVLTree<unsigned char>;


#endif //BINARY_SEARCH_TREES_AVL_H
