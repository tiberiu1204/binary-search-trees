#include "bst.h"
#include <stdexcept>
#include <stack>

template <typename T>
size_t BinarySearchTree<T>::size() const {
    return this->tree_container.size() - 1;
}

template <typename T>
BinarySearchTree<T>::Node &BinarySearchTree<T>::at(size_t index) const {
    if(index > this->size()) throw std::out_of_range(
                "Provided index for 'at' (" +
                std::to_string(index) +
                ") is out of range (" +
                this->size() +
                ")"
        );
    return this->tree_container.at(index + 1);
}

template <typename T>
BinarySearchTree<T>::BinarySearchTree(std::vector<T> values) {
    this->emplace({}, 0, 1);
    for(const auto &value : values) {
        this->insert(value);
    }
}

template <typename T>
BinarySearchTree<T>::BinarySearchTree() {
    this->emplace({}, 0, 1);
}

template <typename T>
size_t BinarySearchTree<T>::size(const Node &node) const {
    return this->size(node.get_node_index());
}

template <typename T>
size_t BinarySearchTree<T>::size(size_t index) const {
    std::stack<size_t> s;
    s.push(index);
    size_t result = 0;
    while(!s.empty()) {
        result++;
        index = s.top();
        s.pop();
        Node &node = this->at(index);
        if(node.has_left()) s.push(node.get_left_index());
        if(node.has_right()) s.push(node.get_right_index());
    }
    return result;
}

template<typename T>
BinarySearchTree<T>::Node::Node(
        size_t node_index,
        const T &value,
        BinarySearchTree *p_bst,
        size_t parent_index,
        size_t left_index,
        size_t right_index)
        :
        node_index(node_index),
        parent_index(parent_index),
        value(value),
        left_index(left_index),
        right_index(right_index),
        p_bst(p_bst) {
    if(!p_bst) throw std::exception();
}

template<typename T>
size_t BinarySearchTree<T>::Node::get_node_index() const {
    return this->node_index;
}

template<typename T>
size_t BinarySearchTree<T>::Node::get_parent_index() const {
    return this->parent_index;
}

template<typename T>
size_t BinarySearchTree<T>::Node::get_left_index() const {
    return this->left_index;
}

template<typename T>
size_t BinarySearchTree<T>::Node::get_right_index() const {
    return this->right_index;
}

template<typename T>
T BinarySearchTree<T>::Node::get_value() const {
    return this->value;
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
void BinarySearchTree<T>::Node::set_left_index(size_t index) {
    this->left_index = index;
}

template<typename T>
void BinarySearchTree<T>::Node::set_parent_index(size_t index) {
    this->parent_index = index;
}

template<typename T>
void BinarySearchTree<T>::Node::set_right_index(size_t index) {
    this->right_index = index;
}

template<typename T>
BinarySearchTree<T>::Node &BinarySearchTree<T>::Node::left() const {
    return this->p_bst->at(this->left_index);
}

template<typename T>
bool BinarySearchTree<T>::Node::has_left() const {
    return this->left_index != 0;
}

template<typename T>
bool BinarySearchTree<T>::Node::has_right() const {
    return this->right_index != 0;
}

template<typename T>
void BinarySearchTree<T>::Node::insert_child(const Node &child, bool left) {
    child.parent_index = this->node_index;
    if(left) this->left_index = child.node_index;
    else this->right_index = child.node_index;
}

template<typename T>
BinarySearchTree<T>::Node &BinarySearchTree<T>::Node::parent() const {
    return this->p_bst->at(this->parent_index);
}

template<typename T>
bool BinarySearchTree<T>::Node::is_left_sibling() const {
    return this->parent().left().node_index == this->node_index;
}

template<typename T>
bool BinarySearchTree<T>::Node::is_right_sibling() const {
    return this->parent().right().node_index == this->node_index;
}

template<typename T>
BinarySearchTree<T>::Node &BinarySearchTree<T>::Node::right() const {
    return this->p_bst->at(this->right_index);
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
    this->tree_container.erase(this->tree_container.begin() + index);
}

template<typename T>
void BinarySearchTree<T>::pop(const Node &node) {
    size_t index = node->get_node_index();
    for(size_t i = index + 1; i < this->size(); i++) {
        this->at(i).update_indexes(index);
    }
    this->tree_container.erase(this->tree_container.begin() + index);
}

template<typename T>
void BinarySearchTree<T>::push(const Node &node) {
    tree_container.push_back(node);
}

template<typename T>
BinarySearchTree<T>::Node BinarySearchTree<T>::create_node(
        size_t node_index,
        const T &value,
        size_t parent_index,
        size_t left_index,
        size_t right_index) const {
    return Node(node_index, value, this, left_index, right_index);
}

template <typename T>
void BinarySearchTree<T>::emplace(const T &value, size_t parent_index, size_t left_index, size_t right_index) {
    this->tree_container.emplace_back(this->size(), parent_index, value, this, left_index, right_index);
}

template <typename T>
BinarySearchTree<T>::iterator::iterator(const BinarySearchTree<T>::Node &node) : ptr(&node.value), node(node) {}

template <typename T>
BinarySearchTree<T>::Node &BinarySearchTree<T>::iterator::find_next_node() {
    Node &node_it = this->node;
    if(node_it.has_right()) {
        while(node.has_left()) node_it = node_it.left();
    }
    else {
        while(!node_it.parent().is_left_sibling()) {
            node_it = node_it.parent();
            if(node_it.node_index == 0) break;
        }
    }
    return node_it;
}

template <typename T>
BinarySearchTree<T>::iterator &BinarySearchTree<T>::iterator::operator++() {
    this->node = this->find_next_node();
    return *this;
}


template <typename T>
BinarySearchTree<T>::iterator BinarySearchTree<T>::iterator::operator++(int) const {
    iterator temp = *this;
    this->node = this->find_next_node();
    return temp;
}

template <typename T>
BinarySearchTree<T>::iterator::RefType BinarySearchTree<T>::iterator::operator*() const {
    return *this->ptr;
}

template <typename T>
bool BinarySearchTree<T>::iterator::operator==(const iterator &other) const {
    return this->node.node_index == other.node.node_index;
}

template <typename T>
bool BinarySearchTree<T>::iterator::operator!=(const iterator &other) const {
    return this->node.node_index != other.node.node_index;
}

template <typename T>
BinarySearchTree<T>::iterator BinarySearchTree<T>::begin() const {
    return iterator(this->find_min());
}

template <typename T>
BinarySearchTree<T>::iterator BinarySearchTree<T>::end() const {
    return iterator(this->root().parent());
}

template <typename T>
BinarySearchTree<T>::Node &BinarySearchTree<T>::find_min() {
    if(this->size() == 0) throw std::exception();
    Node &node = this->at(0);
    while(true) {
        if(!node.has_left()) return node;
        node = node.left();
    }
}

template <typename T>
BinarySearchTree<T>::Node &BinarySearchTree<T>::find_max() {
    if(this->size() == 0) throw std::exception();
    Node &node = this->at(0);
    while(true) {
        if(!node.has_right()) return node;
        node = node.right();
    }
}

template <typename T>
BinarySearchTree<T>::Node &BinarySearchTree<T>::root() const {
    return this->at(1);
}

template <typename T>
BinarySearchTree<T>::Node &BinarySearchTree<T>::back() const {
    return this->at(this->size());
}

template <typename T>
bool BinarySearchTree<T>::empty() const {
    return this->size() == 0;
}

template <typename T>
bool BinarySearchTree<T>::Node::has_sibling() const {
    if(this->is_left_sibling()) return this->parent().right_index != 0;
    else return this->parent().left_index != 0;
}

template <typename T>
BinarySearchTree<T>::Node &BinarySearchTree<T>::Node::sibling() const {
    if(this->is_left_sibling()) return this->parent().right();
    else return this->parent().left();
}
