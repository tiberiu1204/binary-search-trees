#ifndef BINARY_SEARCH_TREES_SCAPEGOAT_H
#define BINARY_SEARCH_TREES_SCAPEGOAT_H

#include "bst.h"
#include <stack>
#include <cmath>
#include <tuple>

class DuplicateElement : std::exception {};

template <typename T>
class ScapegoatTree : public BinarySearchTree<T> {
public:
    using iterator = typename ScapegoatTree<T>::iterator;
    explicit ScapegoatTree(double alpha = 0.5);
    explicit ScapegoatTree(std::vector<T> values, double alpha = 0.5);
    void insert(const T &value) override;
    void remove(const T &value) override;
    bool find(const T &value) const override;

private:
    using Node = typename BinarySearchTree<T>::Node;
    using restricted_iterator = typename BinarySearchTree<T>::restricted_iterator;
    double alpha;
private:
    inline bool is_height_balanced(size_t height);
    size_t insert_value(const T &value);
    Node &find_scapegoat();
    const Node *lookup(const T &value) const;
    Node &find_min_in_subtree(Node &root);
    Node &find_max_in_subtree(Node &root);
    void rebuild_subtree(Node &root);
};

template <typename T>
ScapegoatTree<T>::ScapegoatTree(std::vector<T> values, double alpha) {
    if(alpha > 1) this->alpha = 1;
    else if(alpha < .5) this->alpha = .5;
    else this->alpha = alpha;
    for(auto value : values) {
        this->insert(value);
    }
}

template <typename T>
ScapegoatTree<T>::ScapegoatTree(double alpha) {
    if(alpha > 1) this->alpha = 1;
    else if(alpha < .5) this->alpha = .5;
    else this->alpha = alpha;
}

template <typename T>
bool ScapegoatTree<T>::find(const T &value) const {
    return this->lookup(value) != nullptr;
}

template <typename T>
void ScapegoatTree<T>::insert(const T &value) {
    if(this->empty()) {
        this->emplace(value);
        return;
    }

    size_t height = this->insert_value(value);
    if(this->is_height_balanced(height)) return;

    this->rebuild_subtree(this->find_scapegoat());
}

template <typename T>
void ScapegoatTree<T>::rebuild_subtree(Node &root) {
    Node &root_parent = root.parent();
    bool is_root_left_sibling = root.is_left_sibling();

    std::vector<Node *> sorted_nodes;
    std::vector<T> sorted_values;

    auto begin = restricted_iterator(this->find_min_in_subtree(root));
    auto end = restricted_iterator(this->find_max_in_subtree(root)) + 1;
    for(auto it = begin; it != end; it++) {
        sorted_nodes.push_back(&it.get_node());
        sorted_values.push_back(*it);
    }

    std::stack<std::tuple<size_t, size_t, size_t, bool>> s; // left, right, parent_index, is_left_sibling
    s.push(std::make_tuple(0, sorted_nodes.size() - 1, root_parent.get_node_index(), is_root_left_sibling));
    size_t index = 0;
    while(!s.empty()) {
        std::tuple<size_t, size_t, size_t, bool> top = s.top();
        s.pop();

        size_t left_index = std::get<0>(top);
        size_t right_index = std::get<1>(top);
        size_t parent_index = std::get<2>(top);
        bool is_left_sibling = std::get<3>(top);

        if(left_index > right_index) continue;
        size_t mid = (left_index + right_index) / 2;

        Node &new_root = *sorted_nodes.at(index);

        new_root.set_value(sorted_values.at(mid));
        new_root.set_parent_index(parent_index);
        new_root.set_right_index(0);
        new_root.set_left_index(0);
        if(is_left_sibling) new_root.parent().set_left_index(new_root.get_node_index());
        else new_root.parent().set_right_index(new_root.get_node_index());

        s.push(std::make_tuple(mid + 1, right_index, new_root.get_node_index(), false));
        if(mid > 0) s.push(std::make_tuple(left_index, mid - 1, new_root.get_node_index(), true));
        index++;
    }
}


template <typename T>
typename ScapegoatTree<T>::Node &ScapegoatTree<T>::find_min_in_subtree(Node &root) {
    Node *node = &root;
    while(node->has_left()) node = &node->left();
    return *node;
}

template <typename T>
typename ScapegoatTree<T>::Node &ScapegoatTree<T>::find_max_in_subtree(Node &root) {
    Node *node = &root;
    while(node->has_right()) node = &node->right();
    return *node;
}

template <typename T>
size_t ScapegoatTree<T>::insert_value(const T &value) {
    Node *node = &this->root();
    size_t height = 0;

    while(true) {
        height++;
        if(value < node->get_value()) {
            if(!node->has_left()) {
                node->set_left_index(this->next_index());
                this->emplace(value, node->get_node_index());
                break;
            }
            node = &node->left();
        }
        else if(value > node->get_value()) {
            if(!node->has_right()) {
                node->set_right_index(this->next_index());
                this->emplace(value, node->get_node_index());
                break;
            }
            node = &node->right();
        }
        else throw DuplicateElement();
    }

    return height;
}

template <typename T>
typename ScapegoatTree<T>::Node &ScapegoatTree<T>::find_scapegoat() {
    size_t child_size = 1;
    Node *child = &this->back();
    Node *node = &child->parent();
    while(true) {
        size_t node_size = child_size + 1;
        size_t sibling_size = 0;
        if(child->has_sibling()) sibling_size = this->size(child->sibling());
        node_size += sibling_size;

        if(child_size > this->alpha * node_size || sibling_size > this->alpha * node_size) {
            return *node;
        }
        child = node;
        child_size = node_size;
        node = &node->parent();
    }
}

template <typename T>
inline bool ScapegoatTree<T>::is_height_balanced(size_t height) {
    size_t tree_size = this->size();
    double log_one_over_alpha = std::log(tree_size) / std::log(1 / this->alpha);
    int result = static_cast<int>(std::floor(log_one_over_alpha)) + 1;
    return height <= result;
}

template <typename T>
const typename ScapegoatTree<T>::Node *ScapegoatTree<T>::lookup(const T &value) const {
    if(this->empty()) return nullptr;
    std::stack<size_t> s;
    s.push(this->root().get_node_index());
    while(!s.empty()) {
        size_t index = s.top();
        s.pop();

        const Node *node = &this->at(index);

        if(node->get_value() == value) return node;
        else if(value < node->get_value()) s.push(node->get_right_index());
        else s.push(node->get_left_index());
    }
    return nullptr;
}

template <typename T>
void ScapegoatTree<T>::remove(const T &value) {
    Node &node = this->find_node_by_value(value);
    if(!node.has_left() && !node.has_right()) this->pop(node);
}

template class ScapegoatTree<int>;
template class ScapegoatTree<float>;
template class ScapegoatTree<double>;
template class ScapegoatTree<unsigned int>;
template class ScapegoatTree<unsigned long long>;
template class ScapegoatTree<long long>;
template class ScapegoatTree<short>;
template class ScapegoatTree<char>;
template class ScapegoatTree<unsigned char>;

#endif //BINARY_SEARCH_TREES_SCAPEGOAT_H