#include <iostream>
#include <string>
#include <wheels/string_utility.hpp>

int main() {
    std::cout << gkxx::join(", ", {"hello", "world", "cyqwq", "hahahaha"})
              << std::endl;
    std::cout << gkxx::join(", ", {}) << std::endl;
    std::cout << gkxx::join(',', {"hello", "world", "cyqwq", "hahahaha"})
              << std::endl;
    std::cout << gkxx::join(',', {}) << std::endl;
    return 0;
}