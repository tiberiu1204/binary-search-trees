#ifndef BINARY_SEARCH_TREES_SCAPEGOAT_H
#define BINARY_SEARCH_TREES_SCAPEGOAT_H

#include "bst.h"

class DuplicateElement : std::exception {};

template <typename T>
class ScapegoatTree : public BinarySearchTree<T> {
public:
    ScapegoatTree() = default;
    explicit ScapegoatTree(std::vector<T> values);
    void insert(T value) override;
    void remove(T value) override;
    bool find(T value) override;

private:
    using Node = BinarySearchTree<T>::Node;
};

#endif //BINARY_SEARCH_TREES_SCAPEGOAT_H