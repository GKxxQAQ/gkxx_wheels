#include <wheels/algo_contest/io_utility.hpp>
#include <iostream>

int main() {
    using namespace gkxx::io_utility;
    redirect_in("ints.in");
    int x;
    while (read_int(x))
        std::cout << x << std::endl;
    while ((x = getchar()) != EOF)
        std::cout << (char)x << std::endl;
    return 0;
}