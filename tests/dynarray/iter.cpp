#include "wheels/dynarray.hpp"
#include <iostream>
#include <cstdlib>
#include <algorithm>

int main() {
    srand(20200405);
    using namespace gkxx;
    int n;
    std::cin >> n;
    Dynarray<int> d(n);
    for (auto it = d.begin(); it != d.end(); ++it)
        std::cout << (*it = rand()) << ' ';
    std::cout << '\n';
    std::sort(d.begin(), d.end());
    print(d);
    const Dynarray<int> cd = d;
    for (auto x : cd)
        std::cout << x << ' ';
    std::cout << '\n';
    Dynarray<int>::const_iterator cit = d.begin();
    std::cout << *cit << std::endl;
    while (true) {
        try {
            std::cout << *cit++ << ' ';
        } catch (std::out_of_range o) {
            std::cout << '\n';
            std::cout << o.what() << std::endl;
            break;
        }
    }
    return 0;
}