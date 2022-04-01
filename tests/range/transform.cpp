#include "wheels/dynarray.h"
#include "wheels/range.h"
#include <iostream>

int main() {
    using namespace gkxx;
    print(Dynarray<int>(Range(1, 10, 3)));
    return 0;
}