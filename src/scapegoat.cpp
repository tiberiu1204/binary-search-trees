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
bool ScapegoatTree<T>::find(const T &value) {
    return this->lookup(value) != nullptr;
}

template <typename T>
void ScapegoatTree<T>::insert(const T &value) {
    if(this->size() == 0) {
        this->emplace(0, value);
        return;
    }
    size_t tree_size = this->size();
    Node &node = this->at(0);
    size_t height = 0;
    while(true) {
        height++;
        if(value < node.get_value()) {
            if(!node.has_left()) {
                this->emplace(tree_size, value);
                node.set_left_index(tree_size);
                break;
            }
            node = node.left();
        }
        else if(value > node.get_value()) {
            if(!node.has_right()) {
                this->emplace(tree_size, value);
                node.set_right_index(tree_size);
                break;
            }
            node = node.right();
        }
        else throw DuplicateElement();
    }
}

template <typename T>
inline bool ScapegoatTree<T>::is_height_balanced(size_t height) {
    size_t tree_size = this->size();
    return tree_size <= std::floor(std::log(tree_size) / log(1 / this->alpha));
}

template <typename T>
ScapegoatTree<T>::Node *ScapegoatTree<T>::lookup(const T &value) {
    if(this -> size() == 0) return false;
    std::stack<size_t> s;
    s.push(0);
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
