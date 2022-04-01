#include <wheels/printer.hpp>
#include <iostream>

int main() {
    using namespace gkxx::printer;
    Printer(sep = ",")(2, 3);
    return 0;
}