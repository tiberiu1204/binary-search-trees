#include "scapegoat.h"
#include "stack"

template <typename T>
ScapegoatTree<T>::ScapegoatTree(std::vector<T> values) : BinarySearchTree<T>(values) {}

template <typename T>
bool ScapegoatTree<T>::find(T value) {
    if(this -> size() == 0) return false;
    std::stack<size_t> s;
    s.push(0);
    while(!s.empty()) {
        size_t index = s.top();
        s.pop();

        const Node &node = this->at(index);

        if(node.get == value) return true;
        else if(value < node.get_value()) s.push(node.get_right_index());
        else s.push(node.get_left_index());
    }
    return false;
}

template <typename T>
void ScapegoatTree<T>::insert(T value) {

}

template <typename T>
void ScapegoatTree<T>::remove(T value) {

}
