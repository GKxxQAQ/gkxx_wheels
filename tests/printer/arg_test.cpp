#include <iostream>
#include <string>
#include <wheels/printer.hpp>
#include <sstream>

int main() {
    using namespace gkxx::printer;
    std::ostringstream test1;
    Printer(sep = "  ", end = ".\n", stream = test1,
            flush = true)(2, 3)(4)(4, "helloworld", std::string("string"));
    std::cout << std::boolalpha
              << (test1.str() ==
                  "2  3.\n4.\n4  helloworld  string.\n")
              << std::endl;     // true
    std::ostringstream test2;
    Printer().stream(test2).sep(", ").end(".\n").print("hello", "world");
    std::cout << (test2.str() == "hello, world.\n") << std::endl;   // true
    // The following should cause an error.
    // Printer(sep = "abc", sep = "def")(3, 4);
    return 0;
}