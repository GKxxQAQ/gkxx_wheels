#include "wheels/fill_string.hpp"
#include <iostream>
#include <string>

int main() {
    std::cout << gkxx::fill_string("x == %, y == %", 1, 2) << std::endl;
    std::cout << gkxx::fill_string("x ==%% %, y == %", 1, 2) << std::endl;
    std::cout << gkxx::fill_string("x == %%%%%, y == %", 1, 2) << std::endl;
    std::string s = "x == %, y == %";
    std::cout << gkxx::fill_string(s, 1, 2) << std::endl;
    std::cout << gkxx::fill_string("x == % %, y == %", "wesfsd", 2) << std::endl;
    std::cout << gkxx::fill_string("x == %", 1, 2) << std::endl;
    return 0;
}