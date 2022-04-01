#include <iostream>
#include <string>
#include <wheels/string_utility.hpp>

int main() {
    using namespace gkxx::__details;
    auto vec = get_kmp_border("cadcacad");
    for (auto x : vec)
        std::cout << x << ' ';
    std::cout << std::endl;
    return 0;
}