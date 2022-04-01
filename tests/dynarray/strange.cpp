#include "wheels/dynarray.hpp"
#include <iostream>

int main() {
    gkxx::Dynarray<int> da(0);
    int *p = new int[0]();
    int a[0];
    std::cout << (p == nullptr) << std::endl;
    std::cout << *p << std::endl;
    std::cout << a[2] << std::endl;
    delete[] p;
    return 0;
}