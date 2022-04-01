#include "wheels/matrix.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>

int main() {
    srand(20020405);
    gkxx::Matrix<int> mt(4, 7);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 7; ++j)
            mt[i][j] = rand() % 20;
    print(mt, std::cout, ", ", "\n");
    for (const auto &r : mt)
        for (auto x : r)
            std::cout << x << ' ';
    std::cout << std::endl;
    for (int x : mt.elements())
        std::cout << x << ' ';
    std::cout << std::endl;
    for (gkxx::Matrix<int>::size_type i = 0; i < mt.elements().size(); ++i)
        std::cout << mt.elements()[i] << " ";
    std::cout << std::endl;
    // std::sort(mt.elements().begin(), mt.elements().end());
    // print(mt);
    std::cout << mt.to_latex() << std::endl;
    std::sort(mt.begin(), mt.end());
    print(mt);
    return 0;
}