#include "wheels/dynarray.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <utility>

using gkxx::Dynarray;

int main() {
    int n = 10, m = 11;
    Dynarray<int> da(n);
    for (int i = 0; i < n; ++i)
        da[i] = rand() % 10;
    Dynarray<int> da2(m);
    for (int i = 0; i < m; ++i)
        da2[i] = rand() % 10;
    print(da);
    print(da2);
    da -= da2;
    print(da);
    return 0;
}