#include <wheels/color.hpp>
#include <iostream>
#include <string>

int main() {
  namespace color = gkxx::color;
  std::cout << color::red(1) << "\n";
  int x = 42;
  std::cout << color::blue(x) << "\n";
  const std::string str = "fwefwf";
  std::cout << color::green(str) << "\n";
  std::cout << color::yellow("yello thing") << std::endl;
  return 0;
}