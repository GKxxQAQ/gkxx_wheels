#include <iostream>
#include <string>
#include <wheels/string_utility.hpp>

int main() {
    using namespace gkxx;
    auto res1 = split_by_kmp("hello, world, hahaha, cyqwq", ", ");
    for (const auto &s : res1)
        std::cout << s << ' ';
    std::cout << std::endl;
    auto res2 = split_by_kmp(", ", ", ");
    std::cout << "res2.size() == " << res2.size() << std::endl;
    std::cout << std::boolalpha << (res2.front() == "" && res2.back() == "")
              << std::endl;
    auto res3 = split_by_kmp("hahaha, cyqwq, ", ", ");
    std::cout << "res3.size() == " << res3.size() << std::endl;
    for (const auto &s : res3)
        std::cout << s << ' ';
    std::cout << std::endl;
    auto res4 = split_by_kmp("hahaha,,,haha", ",,");
    for (const auto &s : res4)
        std::cout << s << ' ';
    std::cout << std::endl;

    // auto res5 = split_by_kmp("fweefsdfew", 'f');
    return 0;
}