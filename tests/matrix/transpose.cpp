#include "wheels/matrix.hpp"
#include <iostream>

int main() {
    gkxx::Matrix<double> md = {
        {1, 2, 3}, {3, 6, 9, 7, 5}, {5, 12, 18}, {40, 70, 86, 32}};
    print(md);
    std::cout << std::endl;
    print(transpose(md));
    std::cout << std::endl;
    print(transpose(transpose(md)));
    std::cout << std::endl;
    return 0;
}