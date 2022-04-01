#include "wheels/dynarray.hpp"
#include <algorithm>
#include <iostream>
#include <string>

using gkxx::Dynarray;

int main() {
    int n = 10;
    Dynarray<int> a(n);
    for (int i = 0; i < n; ++i) {
        a[i] = rand() % 10;
    }
    std::sort(a.begin(), a.end());
    auto it = std::unique(a.begin(), a.end());
    auto cur = a.begin();
    while (cur != it)
        std::cout << *cur++ << ' ';
    std::cout << std::endl;
    return 0;
}