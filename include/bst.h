#ifndef BINARY_SEARCH_TREES_BST_H
#define BINARY_SEARCH_TREES_BST_H

#include <utility>
#include <vector>
#include <stack>

class TreeEmptyException : std::exception {
public:
    explicit TreeEmptyException(std::string instruction = "") : instruction(std::move(instruction)) {
        std::cerr<<"Tree is empty, cannot execute '" + this->instruction + "'\n";
    }
    const char* what() {
        return ("Tree is empty, cannot execute '" + this->instruction + "'").c_str();
    }
private:
    std::string instruction;
};

template <typename T>
class BinarySearchTree {
protected:
    class Node;
public:
    class Iterator;
    using iterator = Iterator;
    BinarySearchTree();
    virtual void insert(const T &value) = 0;
    virtual void remove(const T &value) = 0;
    virtual bool find(const T &value) const = 0;
    [[nodiscard]] size_t size() const;
    virtual ~BinarySearchTree() = default;
    virtual iterator begin();
    virtual iterator end();
public:
    class Iterator {
    public:
        using DataType = T;
        using PointerType = DataType*;
        using RefType = DataType&;

        Iterator &operator++();
        Iterator operator++(int);

        Iterator &operator--();
        Iterator operator--(int);
        Iterator operator+(int n) const;
        Iterator operator-(int n) const;
        Iterator &operator+=(int n);
        Iterator &operator-=(int n);
        RefType operator*() const;
        bool operator==(const Iterator &other) const;
        bool operator!=(const Iterator &other) const;
        bool operator<(const Iterator &other) const;
        bool operator<=(const Iterator &other) const;
        bool operator>(const Iterator &other) const;
        bool operator>=(const Iterator &other) const;

        explicit Iterator(Node &node);
    protected:
        Node *node;
        PointerType ptr;
    private:
        Node &find_next_node();
        Node &find_prev_node();
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
        const Node &left() const;
        Node &left();
        const Node &right() const;
        Node &right();
        const Node &parent() const;
        Node &parent();
        const Node &sibling() const;
        Node &sibling();
        void insert_child(Node &child, bool left);
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
        void set_value(const T &index);
        [[nodiscard]] T get_value() const;
        [[nodiscard]] bool is_dummy_node() const;
    private:
        BinarySearchTree *p_bst;
        size_t node_index;
        size_t left_index;
        size_t right_index;
        size_t parent_index;
        T value;
    };
protected:
    class RestrictedIterator : public Iterator {
    public:
        explicit RestrictedIterator(Node &node);
        Node &get_node();
    };
    using restricted_iterator = RestrictedIterator;
protected:
    [[nodiscard]] size_t next_index() const;
    const Node &at(size_t index) const;
    Node &at(size_t index);
    const Node &back() const;
    Node &back();
    const Node &find_node_by_value(const T &value) const;
    Node &find_node_by_value(const T &value);
    [[nodiscard]] bool empty() const;
    size_t size(const Node &node) const;
    [[nodiscard]] size_t size(size_t index) const;
    const Node &root() const;
    Node &root();
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

template<typename T>
const BinarySearchTree<T>::Node &BinarySearchTree<T>::find_node_by_value(const T &value) const {
    const Node *node = &this->root();
    while(node->get_value() != value) {
        if(node->get_value() > value) node = &node->left();
        else node = &node->right();
        if(node->is_dummy_node()) return *node;
    }
    return *node;
}

template<typename T>
BinarySearchTree<T>::Node &BinarySearchTree<T>::find_node_by_value(const T &value) {
    return const_cast<Node &>(this->find_node_by_value(value));
}

template<typename T>
size_t BinarySearchTree<T>::next_index() const {
    return this->size() + 1;
}

template<typename T>
typename BinarySearchTree<T>::Node &BinarySearchTree<T>::at(size_t index) {
    if(index > this->size()) throw std::out_of_range(
                "Provided index for 'at' (" +
                std::to_string(index) +
                ") is out of range (" +
                std::to_string(this->size()) +
                ")"
        );
    return this->tree_container.at(index);
}

template <typename T>
size_t BinarySearchTree<T>::size() const {
    return this->tree_container.size() - 1;
}

template <typename T>
const typename BinarySearchTree<T>::Node &BinarySearchTree<T>::at(size_t index) const {
    if(index > this->size()) throw std::out_of_range(
                "Provided index for 'at' (" +
                std::to_string(index) +
                ") is out of range (" +
                std::to_string(this->size()) +
                ")"
        );
    return this->tree_container.at(index);
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
        const Node *node = &this->at(index);
        if(node->has_left()) s.push(node->get_left_index());
        if(node->has_right()) s.push(node->get_right_index());
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
const typename BinarySearchTree<T>::Node &BinarySearchTree<T>::Node::left() const {
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
void BinarySearchTree<T>::Node::insert_child(Node &child, bool left) {
    child.parent_index = this->node_index;
    if(left) this->left_index = child.node_index;
    else this->right_index = child.node_index;
}

template<typename T>
const typename BinarySearchTree<T>::Node &BinarySearchTree<T>::Node::parent() const {
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
const typename BinarySearchTree<T>::Node &BinarySearchTree<T>::Node::right() const {
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
    size_t index = node.get_node_index();
    for(size_t i = index + 1; i < this->size(); i++) {
        this->at(i).update_indexes(index);
    }
    this->tree_container.erase(this->tree_container.begin() + index);
}

template<typename T>
void BinarySearchTree<T>::push(const Node &node) {
    tree_container.push_back(node);
    if(this->size() == 0) {
        this->tree_container.at(0).insert_child(this->back(), true);
    }
}

template<typename T>
typename BinarySearchTree<T>::Node BinarySearchTree<T>::create_node(
        size_t node_index,
        const T &value,
        size_t parent_index,
        size_t left_index,
        size_t right_index) const {
    return Node(node_index, value, parent_index, this, left_index, right_index);
}

template <typename T>
void BinarySearchTree<T>::emplace(const T &value, size_t parent_index, size_t left_index, size_t right_index) {
    size_t node_index = 0;
    if(!this->tree_container.empty()) node_index = this->next_index();
    this->tree_container.emplace_back(node_index, value, this, parent_index, left_index, right_index);
}

template <typename T>
BinarySearchTree<T>::Iterator::Iterator(BinarySearchTree<T>::Node &node) : ptr(&node.value), node(&node) {}

template <typename T>
typename BinarySearchTree<T>::Node &BinarySearchTree<T>::iterator::find_next_node() {
    Node *node_it = this->node;
    if(node_it->has_right()) {
        node_it = &node_it->right();
        while(node_it->has_left()) node_it = &node_it->left();
        return *node_it;
    }

    Node *temp = node_it;
    node_it = &node_it->parent();
    if(temp->is_left_sibling() || node_it->node_index == 0) return *node_it;

    while(!node_it->is_left_sibling()) {
        node_it = &node_it->parent();
    }

    return node_it->parent();
}

template <typename T>
typename BinarySearchTree<T>::Node &BinarySearchTree<T>::iterator::find_prev_node() {
    Node *node_it = this->node;
    if(node_it->has_left()) {
        node_it = &node_it->left();
        while(node_it->has_right()) node_it = &node_it->right();
        return *node_it;
    }

    Node *temp = node_it;
    node_it = &node_it->parent();
    if(node_it->get_node_index() == 0) return *node_it;
    if(temp->is_right_sibling()) return *node_it;

    while(!node_it->is_right_sibling()) {
        node_it = &node_it->parent();
        if(node_it->get_node_index() == 0) return *node_it;
    }
    node_it = &node_it->parent();
    if(node_it->get_node_index() == 0) return *node_it;

    return *node_it;
}

template <typename T>
typename BinarySearchTree<T>::iterator &BinarySearchTree<T>::iterator::operator++(){
    this->node = &this->find_next_node();
    this->ptr = &this->node->value;
    return *this;
}


template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::iterator::operator++(int) {
    iterator temp = *this;
    this->node = &this->find_next_node();
    this->ptr = &this->node->value;
    return temp;
}

template <typename T>
typename BinarySearchTree<T>::iterator::RefType BinarySearchTree<T>::iterator::operator*() const {
    return *this->ptr;
}

template <typename T>
bool BinarySearchTree<T>::iterator::operator==(const iterator &other) const {
    return this->node->node_index == other.node->node_index;
}

template <typename T>
bool BinarySearchTree<T>::iterator::operator!=(const iterator &other) const {
    return this->node->node_index != other.node->node_index;
}

template <typename T>
typename BinarySearchTree<T>::iterator &BinarySearchTree<T>::iterator::operator--() {
    this->node = &this->find_prev_node();
    this->ptr = &this->node->value;
    return *this;
}

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::iterator::operator--(int) {
    iterator temp = *this;
    this->node = &this->find_prev_node();
    this->ptr = &this->node->value;
    return temp;
}

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::iterator::operator+(int n) const {
    iterator temp = *this;
    int sign = n > 0 ? 1 : -1;
    n = n > 0 ? n : -n;
    while(n--) {
        if(sign > 0) ++temp;
        else --temp;
    }
    return temp;
}

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::iterator::operator-(int n) const {
    return *this + -n;
}

template <typename T>
typename BinarySearchTree<T>::iterator &BinarySearchTree<T>::iterator::operator+=(int n) {
    *this = *this + n;
    return *this;
}

template <typename T>
typename BinarySearchTree<T>::iterator &BinarySearchTree<T>::iterator::operator-=(int n) {
    return *this += -n;
}

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::begin() {
    return iterator(this->find_min());
}

template <typename T>
typename BinarySearchTree<T>::iterator BinarySearchTree<T>::end() {
    return iterator(this->root().parent());
}

template <typename T>
typename BinarySearchTree<T>::Node &BinarySearchTree<T>::find_min() {
    if(this->size() == 0) throw std::exception();
    Node *node = &this->at(0);
    while(true) {
        if(!node->has_left()) return *node;
        node = &node->left();
    }
}

template <typename T>
typename BinarySearchTree<T>::Node &BinarySearchTree<T>::find_max() {
    if(this->size() == 0) throw std::exception();
    Node *node = this->at(0);
    while(true) {
        if(!node->has_right()) return *node;
        node = node->right();
    }
}

template <typename T>
const typename BinarySearchTree<T>::Node &BinarySearchTree<T>::root() const {
    if(this->empty()) throw TreeEmptyException("root");
    return this->at(1);
}

template <typename T>
typename BinarySearchTree<T>::Node &BinarySearchTree<T>::root() {
    if(this->empty()) throw TreeEmptyException("root");
    return this->at(1);
}

template <typename T>
const typename BinarySearchTree<T>::Node &BinarySearchTree<T>::back() const {
    if(this->empty()) throw TreeEmptyException("back");
    return this->at(this->size());
}

template <typename T>
typename BinarySearchTree<T>::Node &BinarySearchTree<T>::back() {
    if(this->empty()) throw TreeEmptyException("back");
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
const typename BinarySearchTree<T>::Node &BinarySearchTree<T>::Node::sibling() const {
    if(this->is_left_sibling()) return this->parent().right();
    else return this->parent().left();
}

template <typename T>
typename BinarySearchTree<T>::Node &BinarySearchTree<T>::RestrictedIterator::get_node() {
    return *this->node;
}

template <typename T>
BinarySearchTree<T>::RestrictedIterator::RestrictedIterator(Node &node) : Iterator(node) {}

template <typename T>
void BinarySearchTree<T>::Node::set_value(const T &index) {
    this->value = index;
}

template <typename T>
typename BinarySearchTree<T>::Node &BinarySearchTree<T>::Node::left() {
    return this->p_bst->at(this->left_index);
}

template <typename T>
typename BinarySearchTree<T>::Node &BinarySearchTree<T>::Node::right() {
    return this->p_bst->at(this->right_index);
}

template <typename T>
typename BinarySearchTree<T>::Node &BinarySearchTree<T>::Node::parent() {
    return this->p_bst->at(this->parent_index);
}

template <typename T>
typename BinarySearchTree<T>::Node &BinarySearchTree<T>::Node::sibling() {
    if(this->is_left_sibling()) return this->parent().right();
    else return this->parent().left();
}

template <typename T>
bool BinarySearchTree<T>::iterator::operator<(const Iterator &other) const {
    if(other.node->is_dummy_node() && !this->node->is_dummy_node()) return true;
    return *this->ptr < *other.ptr;
}

template <typename T>
bool BinarySearchTree<T>::iterator::operator<=(const Iterator &other) const {
    if(this->node->is_dummy_node() && other.node->is_dummy_node()) return false;
    return *this->ptr <= *other.ptr;
}

template <typename T>
bool BinarySearchTree<T>::iterator::operator>(const Iterator &other) const {
    return *this->ptr > *other.ptr;
}

template <typename T>
bool BinarySearchTree<T>::iterator::operator>=(const Iterator &other) const {
    if(this->node->is_dummy_node()) return false;
    return *this->ptr >= *other.ptr;
}

template <typename T>
bool BinarySearchTree<T>::Node::is_dummy_node() const {
    return this->node_index == 0;
}

#endif //BINARY_SEARCH_TREES_BST_H
