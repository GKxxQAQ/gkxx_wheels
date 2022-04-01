#include "wheels/matrix.hpp"
#include <iostream>

int main() {
    gkxx::Matrix<int> mt = {{1, 2, 3}, {2, 4, 6}, {3, 6, 9, 12}};
    print(mt);
    gkxx::Matrix<int> emp = {{}, {}, {}, {}};
    std::cout << emp.size() << std::endl;
    return 0;
}