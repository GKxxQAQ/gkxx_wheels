#include <wheels/linestream.hpp>
#include <iostream>
#include <string>

using gkxx::stream_by_line;

int main() {
    for (const auto &s : stream_by_line(std::cin))
        std::cout << s << std::endl;
    return 0;
}