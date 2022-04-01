#include "wheels/range.h"
#include <iostream>

int main() {
    using gkxx::Range;
    for (int x : Range(10, 20, 4))
        std::cout << x << ' ';
    std::cout << std::endl;
    return 0;
}