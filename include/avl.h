//
// Created by Voicu Ioan Vladut on 30.04.2024.
//

#ifndef BINARY_SEARCH_TREES_AVL_H
#define BINARY_SEARCH_TREES_AVL_H


#include "bst.h"
#include <iostream>
#include <unordered_map>

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
    std::unordered_map<int, int> heights;
private:
    int balance_factor(Node &node);

    void left_rotate(Node &node);

    void right_rotate(Node &node);

    void balance(Node &node);

    void update_heights(Node &node);
};

template<typename T>
void AVLTree<T>::update_heights(AVLTree::Node &node)
{
    heights[node.get_node_index()] = std::max(node.has_left() ? heights[node.get_left_index()] : 0,
                                              node.has_right() ? heights[node.get_right_index()] : 0) + 1;
}

template<typename T>
void AVLTree<T>::balance(AVLTree::Node &node)
{
    Node *node_ptr = &node;
    while (node_ptr->get_parent_index() > 0) {
        update_heights(*node_ptr);
        if (balance_factor(*node_ptr) >= 2 and balance_factor(node_ptr->left()) >= 0)   // left - left
            right_rotate(*node_ptr);
        else if (balance_factor(*node_ptr) >= 2) {  // left - right
            left_rotate(node_ptr->left());
            right_rotate(*node_ptr);
        } else if (balance_factor(*node_ptr) <= -2 and balance_factor(node_ptr->right()) <= 0)  // right - right
            left_rotate(*node_ptr);
        else if (balance_factor(*node_ptr) <= -2) {  // right - left
            right_rotate(node_ptr->right());
            left_rotate(*node_ptr);
        }
        node_ptr = &node_ptr->parent();
    }
}

template<typename T>
void AVLTree<T>::left_rotate(AVLTree::Node &node)
{
    Node &right_child = node.right();
    node.set_right_index(right_child.get_left_index());
    right_child.left().set_parent_index(node.get_node_index());
    right_child.set_left_index(node.get_node_index());
    right_child.set_parent_index(node.get_parent_index());
    if (node.is_left_sibling()) right_child.parent().set_left_index(right_child.get_node_index());
    else right_child.parent().set_right_index(right_child.get_node_index());
    node.set_parent_index(right_child.get_node_index());

    update_heights(node);
    update_heights(right_child);
}

template<typename T>
void AVLTree<T>::right_rotate(AVLTree::Node &node)
{
    Node &left_child = node.left();
    node.set_left_index(left_child.get_right_index());
    left_child.right().set_parent_index(node.get_node_index());
    left_child.set_right_index(node.get_node_index());
    left_child.set_parent_index(node.get_parent_index());
    if (node.is_left_sibling()) left_child.parent().set_left_index(left_child.get_node_index());
    else left_child.parent().set_right_index(left_child.get_node_index());
    node.set_parent_index(left_child.get_node_index());

    update_heights(node);
    update_heights(left_child);
}

template<typename T>
int AVLTree<T>::balance_factor(AVLTree::Node &node)
{
    return (node.has_left() ? heights[node.left().get_node_index()] : 0) -
           (node.has_right() ? heights[node.right().get_node_index()] : 0);
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
        Node &parent = node.parent();
        this->pop(node);
        balance(parent);
    } else if (!node.has_left()) {
        node.set_value(node.right().get_value());
        this->pop(node.right());
        node.set_right_index(0);
        balance(node);
    } else if (!node.has_right()) {
        node.set_value(node.left().get_value());
        this->pop(node.left());
        node.set_left_index(0);
        balance(node);
    } else {
        it++;
        Node &next = it.get_node();
        node.set_value(next.get_value());
        if (next.is_left_sibling()) next.parent().set_left_index(0);
        else next.parent().set_right_index(0);
        this->pop(next);
        balance(node);
    }

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
        } else throw DuplicateElement();
    }
    Node &new_node = this->back();
    heights[new_node.get_node_index()] = 1;
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
