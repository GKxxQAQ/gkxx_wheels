#include "wheels/dynarray.hpp"
#include <iostream>

int main() {
    using gkxx::Dynarray;
    using gkxx::Range;
    Dynarray<int> d = Range(1, 10);
    print(d);
    print(d(0, 5, 2));
    return 0;
}