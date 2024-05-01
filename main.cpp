#include <iostream>
#include "avl.h"

int main() {
    std::vector<int> v = {2, 4, 1, 9, 0, 5, 10, 11, 15, 8, 20, 16, 50, 23, 12, 43, 60, 70, 123, 534, 124, 6545};
    AVLTree<int> avl(v);
    for (auto elem: avl) {
        std::cout<<elem<<" ";
    }
    std::cout<<"\n";
    avl.remove(6545);
    for (auto elem: avl) {
        std::cout<<elem<<" ";
    }
    std::cout<<"\n";
    avl.remove(0);
    for (auto elem: avl) {
        std::cout<<elem<<" ";
    }
    std::cout<<"\n";
    avl.remove(20);
    for (auto elem: avl) {
        std::cout<<elem<<" ";
    }
    std::cout<<"\n";
    avl.remove(15);
    for (auto elem: avl) {
        std::cout<<elem<<" ";
    }
    std::cout<<"\n";
    avl.remove(11);
    for (auto elem: avl) {
        std::cout<<elem<<" ";
    }
    std::cout<<"\n";
    std::cout << *(avl.find(50) + 2);
}