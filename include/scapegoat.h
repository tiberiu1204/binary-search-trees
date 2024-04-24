#ifndef BINARY_SEARCH_TREES_SCAPEGOAT_H
#define BINARY_SEARCH_TREES_SCAPEGOAT_H

#include "bst.h"
#include <stack>

class DuplicateElement : std::exception {};

template <typename T>
class ScapegoatTree : public BinarySearchTree<T> {
public:
    using iterator = typename ScapegoatTree<T>::iterator;
    using Node = BinarySearchTree<T>::Node;
    explicit ScapegoatTree(double alpha = 0.5);
    explicit ScapegoatTree(std::vector<T> values, double alpha = 0.5);
    void insert(const T &value) override;
    void remove(const T &value) override;
    bool find(const T &value) const override;

private:
    double alpha;
private:
    inline bool is_height_balanced(size_t height);
    size_t insert_value(const T &value);
    Node &find_scapegoat();
    Node *lookup(const T &value) const;
    Node &find_min_in_subtree(const Node &root);
    Node &find_max_in_subtree(const Node &root);

};

#endif //BINARY_SEARCH_TREES_SCAPEGOAT_H