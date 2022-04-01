#include <wheels/algo_contest/io_utility.hpp>
#include <iostream>

int main() {
    using namespace gkxx::io_utility;
    redirect_in("floats.in");
    double x;
    while (read_float(x))
        printf("%.16lf\n", x);
    while ((x = getchar()) != EOF)
        std::cout << (char)x << std::endl;
    return 0;
}