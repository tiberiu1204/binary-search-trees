#include "scapegoat.h"
#include "stack"
#include <cmath>

template <typename T>
ScapegoatTree<T>::ScapegoatTree(std::vector<T> values, double alpha) : BinarySearchTree<T>(values) {
    if(alpha > 1) this->alpha = 1;
    else if(alpha < .5) this->alpha = .5;
    else this->alpha = alpha;
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
    Node &scapegoat = this->find_scapegoat();
}

template <typename T>
size_t ScapegoatTree<T>::insert_value(const T &value) {
    size_t tree_size = this->size();
    Node &node = this->root();
    size_t height = 0;
    std::stack<size_t> path;

    while(true) {
        height++;
        path.push(node.get_node_index());
        if(value < node.get_value()) {
            if(!node.has_left()) {
                this->emplace(value);
                node.set_left_index(tree_size);
                break;
            }
            node = node.left();
        }
        else if(value > node.get_value()) {
            if(!node.has_right()) {
                this->emplace(value);
                node.set_right_index(tree_size);
                break;
            }
            node = node.right();
        }
        else throw DuplicateElement();
    }

    return node;
}

template <typename T>
ScapegoatTree<T>::Node &ScapegoatTree<T>::find_scapegoat() {
    size_t child_size = 1;
    Node &child = this->back();
    Node &node = child.parent();
    while(true) {
        size_t node_size = child_size + 1;
        size_t sibling_size = 0;
        if(child.has_sibling()) sibling_size = this->size(child.sibling());
        node_size += sibling_size;

        if(child_size > this->alpha * node_size || sibling_size > this->alpha * node_size) {
            return node;
        }
        child = node;
        child_size = node_size;
        node = node.parent();
    }
}

template <typename T>
inline bool ScapegoatTree<T>::is_height_balanced(size_t height) {
    size_t tree_size = this->size();
    return tree_size <= std::floor(std::log(tree_size) / log(1 / this->alpha)) + 1;
}

template <typename T>
ScapegoatTree<T>::Node *ScapegoatTree<T>::lookup(const T &value) const {
    if(this->empty()) return false;
    std::stack<size_t> s;
    s.push(this->root().get_node_index());
    while(!s.empty()) {
        size_t index = s.top();
        s.pop();

        const Node &node = this->at(index);

        if(node.get_value() == value) return &node;
        else if(value < node.get_value()) s.push(node.get_right_index());
        else s.push(node.get_left_index());
    }
    return nullptr;
}

template <typename T>
void ScapegoatTree<T>::remove(const T &value) {

}
