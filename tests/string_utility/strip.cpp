#include <iostream>
#include <string>
#include <vector>
#include <wheels/string_utility.hpp>

int main() {
    using gkxx::strip;
    std::string str("  abcdefghi   \n");
    std::cout << strip("  abcdefghi   \n", " \n") << std::endl;
    std::cout << strip("  abcdefghi   \n") << std::endl;
    std::cout << strip(str, {' ', '\n', 'a', 'h', 'i'})
              << std::endl;
    std::cout << strip("\t", "\t\n") << std::endl;
    return 0;
}