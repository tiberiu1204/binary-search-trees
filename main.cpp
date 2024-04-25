#include <iostream>
#include "scapegoat.h"

int main() {
    std::vector<int> v = {2, 4, 1, 9, 0, 5, 10, 11, 15, 8, 20, 16, 50, 23, 12, 43, 60, 70, 123, 534, 124, 6545}; // Test fail 1
    std::vector<int> v1 = {2, 4, 1, 9, 0, 5, 10, 11, 15, 8, 20, 16, 50, 23, 12}; // Test fail 2
    ScapegoatTree<int> scapegoat_tree(v, 0.5);
    for(auto elem : scapegoat_tree) {
        std::cout<<elem<<" ";
    }
}