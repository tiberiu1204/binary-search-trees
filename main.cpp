#include <iostream>
#include <chrono>
#include <numeric>
#include <random>
#include "avl.h"
#include "scapegoat.h"

std::vector<std::vector<int>> prepare_vectors(const std::vector<int> &sizes)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<std::vector<int>> vectors;
    for (auto &size: sizes) {
        vectors.emplace_back(size);
        auto &vector = vectors.back();
        std::iota(std::begin(vector), std::end(vector), 0);
        std::shuffle(std::begin(vector), std::end(vector), g);
    }
    return vectors;
}

void test_trees(const std::vector<std::vector<int>> &vectors)
{
    using namespace std::chrono;

    for (const auto &vector: vectors) {
        auto start = high_resolution_clock::now();
        AVLTree<int> avl_tree(vector);
        auto end = high_resolution_clock::now();
        auto time = duration_cast<milliseconds>(end - start);
        std::cout << "AVL tree insertion time for " << vector.size() << " elements: " << time << std::endl;

        start = high_resolution_clock::now();
        ScapegoatTree<int> sg_tree(vector);
        end = high_resolution_clock::now();
        time = duration_cast<milliseconds>(end - start);
        std::cout << "Scapegoat tree insertion time for " << vector.size() << " elements: " << time << std::endl;

        size_t qty = vector.size() / 5;

        start = high_resolution_clock::now();
        for (size_t i = vector.size() - 1; i >= vector.size() - qty - 1; --i) {
            avl_tree.find(vector.at(i));
        }
        end = high_resolution_clock::now();
        time = duration_cast<milliseconds>(end - start);
        std::cout << "AVL tree look-up time for " << vector.size() << " elements, " << qty << " look-ups: " << time
                  << std::endl;

        start = high_resolution_clock::now();
        for (size_t i = vector.size() - 1; i >= vector.size() - qty - 1; --i) {
            sg_tree.find(vector.at(i));
        }
        end = high_resolution_clock::now();
        time = duration_cast<milliseconds>(end - start);
        std::cout << "Scapegoat tree look-up time for " << vector.size() << " elements, " << qty << " look-ups: "
                  << time << std::endl;

        start = high_resolution_clock::now();
        for (size_t i = vector.size() - 1; i >= vector.size() - qty - 1; --i) {
            avl_tree.remove(vector.at(i));
        }
        end = high_resolution_clock::now();
        time = duration_cast<milliseconds>(end - start);
        std::cout << "AVL tree removal time for " << vector.size() << " elements, " << qty << " removals: " << time
                  << std::endl;

        start = high_resolution_clock::now();
        for (size_t i = vector.size() - 1; i >= vector.size() - qty - 1; --i) {
            sg_tree.remove(vector.at(i));
        }
        end = high_resolution_clock::now();
        time = duration_cast<milliseconds>(end - start);
        std::cout << "Scapegoat tree removal time for " << vector.size() << " elements, " << qty << " removals: "
                  << time << std::endl;
    }
}

int main() {
    std::vector<int> sizes = {10000};
    auto test_vectors = prepare_vectors(sizes);
    test_trees(test_vectors);
}