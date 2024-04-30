#include <iostream>
#include "scapegoat.h"

int main() {
    std::vector<int> v = {2, 4, 1, 9, 0, 5, 10, 11, 15, 8, 20, 16, 50, 23, 12, 43, 60, 70, 123, 534, 124, 6545};
    ScapegoatTree<int> scapegoat_tree(v, 0.5);
    for(auto elem : scapegoat_tree) {
        std::cout<<elem<<" ";
    }
    std::cout<<"\n";
    scapegoat_tree.remove(6545);
    for(auto elem : scapegoat_tree) {
        std::cout<<elem<<" ";
    }
    std::cout<<"\n";
    scapegoat_tree.remove(0);
    for(auto elem : scapegoat_tree) {
        std::cout<<elem<<" ";
    }
    std::cout<<"\n";
    scapegoat_tree.remove(20);
    for(auto elem : scapegoat_tree) {
        std::cout<<elem<<" ";
    }
    std::cout<<"\n";
    scapegoat_tree.remove(15);
    for(auto elem : scapegoat_tree) {
        std::cout<<elem<<" ";
    }
    std::cout<<"\n";
    scapegoat_tree.remove(11);
    for(auto elem : scapegoat_tree) {
        std::cout<<elem<<" ";
    }
    std::cout<<"\n";
    std::cout<<*(scapegoat_tree.find(50) + 2);
}