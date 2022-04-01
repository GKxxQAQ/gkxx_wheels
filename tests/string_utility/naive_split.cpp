#include <iostream>
#include <string>
#include <wheels/string_utility.hpp>

int main() {
    using namespace gkxx;
    auto res1 = split("hello, world, hahaha, cyqwq", ", ");
    for (const auto &s : res1)
        std::cout << s << ' ';
    std::cout << std::endl;
    auto res2 = split(", ", ", ");
    std::cout << "res2.size() == " << res2.size() << std::endl;
    std::cout << std::boolalpha << (res2.front() == "" && res2.back() == "")
              << std::endl;
    auto res3 = split("hahaha, cyqwq, ", ", ");
    std::cout << "res3.size() == " << res3.size() << std::endl;
    for (const auto &s : res3)
        std::cout << s << ' ';
    std::cout << std::endl;
    // hello world hahaha cyqwq 
    // res2.size() == 2
    // true
    // res3.size() == 3
    // hahaha cyqwq
    return 0;
}