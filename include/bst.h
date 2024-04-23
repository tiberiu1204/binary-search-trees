#ifndef BINARY_SEARCH_TREES_BST_H
#define BINARY_SEARCH_TREES_BST_H

#include <vector>

template <typename T>
class BinarySearchTree {
public:
    virtual void insert(T value) = 0;
    virtual void remove(T value) = 0;
    virtual void find(T value) = 0;
    virtual void traverse() = 0;
    [[nodiscard]] size_t size() const;
    virtual ~BinarySearchTree() = default;
protected:
    class Node;
    class Node {
    public:
        Node(size_t node_index, T value, BinarySearchTree *p_bst, size_t left_index = 0, size_t right_index = 0);
        Node *left();
        Node *right();
        void update_indexes(size_t deleted_index);
        [[nodiscard]] size_t get_node_index() const;
    private:
        BinarySearchTree *p_bst;
        size_t node_index;
        size_t left_index;
        size_t right_index;
        T value;
    };
protected:
    Node &at(size_t index);
    void pop(size_t index);
    void pop(Node *node);
    void push(const Node &node);
    Node create_node(size_t node_index, T value, size_t left_index = 0, size_t right_index = 0);
    void emplace(size_t node_index, T value, size_t left_index = 0, size_t right_index = 0);
private:
    std::vector<Node> tree_container;
};

#endif //BINARY_SEARCH_TREES_BST_H
