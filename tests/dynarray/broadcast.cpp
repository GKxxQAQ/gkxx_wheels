#include "wheels/dynarray.hpp"
#include <iostream>

int main() {
    using namespace gkxx;
    using namespace std;
    Dynarray<int> a = {1, 2, 3, 4, 5};
    // Dynarray<int> b = {4, 5, 6, 3, 2};
    int b = 3;
    print(a + b);
    print(a - b);
    print(a * b);
    print(a / b);
    print(a % b);
    print(a << b);
    print(a >> b);
    print(a & b);
    print(a | b);
    print(a ^ b);
    print(-a);
    print(~a);
    return 0;
}