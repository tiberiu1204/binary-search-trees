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
        ScapegoatTree<int> sg_tree(vector, .5);
        end = high_resolution_clock::now();
        time = duration_cast<milliseconds>(end - start);
        std::cout << "Scapegoat tree insertion time for " << vector.size() << " elements: " << time << std::endl;

        size_t qty = vector.size() / 5;

        start = high_resolution_clock::now();
        for (size_t i = vector.size() - 1; i >= vector.size() - qty - 1; --i) {
            if(*avl_tree.find(vector.at(i)) != vector.at(i)) throw std::exception();
        }
        end = high_resolution_clock::now();
        time = duration_cast<milliseconds>(end - start);
        std::cout << "AVL tree look-up time for " << vector.size() << " elements, " << qty << " look-ups: " << time
                  << std::endl;

        start = high_resolution_clock::now();
        for (size_t i = vector.size() - 1; i >= vector.size() - qty - 1; --i) {
            if(*sg_tree.find(vector.at(i)) != vector.at(i)) throw std::exception();
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
        std::cout<<"\n\n";
    }
}

int main() {
    std::vector<int> sizes = {1000, 10000, 100000, 500000, 1000000, 10000000};
    auto test_vectors = prepare_vectors(sizes);
    test_trees(test_vectors);

    /*
     * Descoperiri:
     * - Scapegoat tree cu alpha 0.5 este mai bun decat AVL tree la toate capitolele pe test seturi <= 500000
     * - AVL tree intrece Scapegoat la find() pe test caseuri cu >= 500000 elemente. Insa este de aprox. 1.5 - 2 ori mai
     * incet la inserare si stergere decat un Scapegoat tree cu alpha 0.5
     * - Un scapegoat tree cu alpha > 0.5 poate ajunge la timpi de inserare < 700ms pentru 1000000 elemente, in timp ce AVL
     * tree scoate o medie de 3.1 secunde.
     * - Concluzii: AVL tree trebuie folosit atunci cand avem mai multa nevoie de a cauta elemente dintr-un set de date
     * care nu se schimba deloc sau aproape deloc, si are cel putin 500000 de elemente, in timp ce Scapegoat tree
     * este preferat in toate cazurile in care avem data seturi < 500000 de elemente sau data seturi dinamice, in care
     * avem nevoie de multe operatii de inserare si stergere.
     */
}