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
};
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
