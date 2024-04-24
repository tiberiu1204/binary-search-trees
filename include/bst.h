#ifndef BINARY_SEARCH_TREES_BST_H
#define BINARY_SEARCH_TREES_BST_H

#include <vector>

template <typename T>
class BinarySearchTree {
protected:
    class Node;
public:
    class iterator;
    BinarySearchTree();
    explicit BinarySearchTree(std::vector<T> values);
    virtual void insert(const T &value) = 0;
    virtual void remove(const T &value) = 0;
    virtual bool find(const T &value) const = 0;
    [[nodiscard]] size_t size() const;
    virtual ~BinarySearchTree() = default;
    virtual iterator begin() const;
    virtual iterator end() const;
public:
    class iterator {
    public:
        using DataType = T;
        using PointerType = DataType*;
        using RefType = DataType&;

        iterator &operator++();
        iterator operator++(int) const;
        RefType operator*() const;
        bool operator==(const iterator &other) const;
        bool operator!=(const iterator &other) const;

        explicit iterator(const Node &node);
    private:
        const Node &node;
        PointerType ptr;
    private:
        Node &find_next_node();
    };
protected:
    class Node {
    public:
        friend iterator;
        Node(
                size_t node_index,
                const T &value,
                BinarySearchTree *p_bst,
                size_t parent_index = 0,
                size_t left_index = 0,
                size_t right_index = 0);
        Node &left() const;
        Node &right() const;
        Node &parent() const;
        Node &sibling() const;
        void insert_child(const Node &child, bool left);
        [[nodiscard]] bool is_left_sibling() const;
        [[nodiscard]] bool is_right_sibling() const;
        [[nodiscard]] bool has_left() const;
        [[nodiscard]] bool has_right() const;
        [[nodiscard]] bool has_sibling() const;
        void update_indexes(size_t deleted_index);
        [[nodiscard]] size_t get_node_index() const;
        [[nodiscard]] size_t get_left_index() const;
        [[nodiscard]] size_t get_right_index() const;
        [[nodiscard]] size_t get_parent_index() const;
        void set_left_index(size_t index);
        void set_right_index(size_t index);
        void set_parent_index(size_t index);
        [[nodiscard]] T get_value() const;
    private:
        BinarySearchTree *p_bst;
        size_t node_index;
        size_t left_index;
        size_t right_index;
        size_t parent_index;
        T value;
    };
protected:
    Node &at(size_t index) const;
    Node &back() const;
    bool empty() const;
    size_t size(const Node &node) const;
    [[nodiscard]] size_t size(size_t index) const;
    Node &root() const;
    void pop(size_t index);
    void pop(const Node &node);
    void push(const Node &node);
    Node create_node(
            size_t node_index,
            const T &value,
            size_t parent_index = 0,
            size_t left_index = 0,
            size_t right_index = 0) const;
    void emplace(const T &value, size_t parent_index = 0, size_t left_index = 0, size_t right_index = 0);
private:
    std::vector<Node> tree_container;
private:
    Node &find_min();
    Node &find_max();
};

#endif //BINARY_SEARCH_TREES_BST_H
