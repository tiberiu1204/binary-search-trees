#include "bst.h"
#include <stdexcept>

template <typename T>
size_t BinarySearchTree<T>::size() const {
    return this->tree_container.size();
}

template <typename T>
BinarySearchTree<T>::Node &BinarySearchTree<T>::at(size_t index) {
    if(index >= this->size()) throw std::out_of_range(
                "Provided index for 'at' (" +
                std::to_string(index) +
                ") is out of range (" +
                this->size() +
                ")"
        );
    return this->tree_container.at(index);
}

template<typename T>
BinarySearchTree<T>::Node::Node(
        size_t node_index,
        T value, BinarySearchTree *p_bst,
        size_t left_index,
        size_t right_index) :
        node_index(node_index), value(value), left_index(left_index), right_index(right_index), p_bst(p_bst) {
    if(!p_bst) throw std::exception();
}

template<typename T>
size_t BinarySearchTree<T>::Node::get_node_index() const {
    return this->node_index;
}

template<typename T>
void BinarySearchTree<T>::Node::update_indexes(size_t deleted_index) {
    this->node_index--;
    if(left_index > deleted_index) left_index--;
    if(left_index == deleted_index) left_index = 0;
    if(right_index > deleted_index) right_index--;
    if(right_index == deleted_index) right_index = 0;
}

template<typename T>
BinarySearchTree<T>::Node *BinarySearchTree<T>::Node::left() {
    return &this->p_bst->at(this->left_index);
}

template<typename T>
BinarySearchTree<T>::Node *BinarySearchTree<T>::Node::right() {
    return &this->p_bst->at(this->right_index);
}

template<typename T>
void BinarySearchTree<T>::pop(size_t index) {
    if(index >= this->size()) throw std::out_of_range(
            "Provided index for 'pop' (" +
            std::to_string(index) +
            ") is out of range (" +
            this->size() +
            ")"
            );
    for(size_t i = index + 1; i < this->size(); i++) {
        this->at(i).update_indexes(index);
    }
}

template<typename T>
void BinarySearchTree<T>::pop(Node *node) {
    size_t index = node->get_node_index();
    for(size_t i = index + 1; i < this->size(); i++) {
        this->at(i).update_indexes(index);
    }
}

template<typename T>
void BinarySearchTree<T>::push(const Node &node) {
    tree_container.push_back(node);
}

template<typename T>
BinarySearchTree<T>::Node BinarySearchTree<T>::create_node(
        size_t node_index,
        T value, size_t left_index,
        size_t right_index) {
    return Node(node_index, value, this, left_index, right_index);
}

template<typename T>
void BinarySearchTree<T>::emplace(size_t node_index, T value, size_t left_index, size_t right_index) {
    this->tree_container.emplace_back(node_index, value, this, left_index, right_index);
}
