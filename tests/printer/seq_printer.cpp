#include <initializer_list>
#include <iostream>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>
#include <wheels/printer.hpp>
#include <wheels/range.hpp>

int main() {
  using namespace gkxx::printer;
  std::pair<int, char> p(3, 'a');
  std::tuple<int, double, char, std::string> t(42, 3.14, 'c', "hello world");
  std::initializer_list<int> il = {1, 2, 3, 4, 5};
  gkxx::Range r(0, 3);

  std::ostringstream res1;
  Seq_printer(sep = ", ", end = ";\n", stream = res1)(p).print(t).flush(true)(
      il)(r);
  std::cout << std::boolalpha;
  std::cout << (res1.str() == "(3, a);\n(42, 3.14, c, hello world);\n{1, 2, "
                              "3, 4, 5};\n[0, 1, 2];\n")
            << std::endl;
  std::tuple<> empty_tuple;
  std::vector<std::string> v = {"Classic coffee", "Chinese tea", "Soft drink",
                                "Cookies"};

  std::ostringstream res2;
  Seq_printer(sep = ",").stream(res2)(empty_tuple)(v.begin(), v.end());
  std::cout << (res2.str() ==
                "()\n[Classic coffee,Chinese tea,Soft drink,Cookies]\n")
            << std::endl;

  std::ostringstream res3;
  Seq_printer(sep = ", ", left = "tuple<", stream = std::cout, end = ";\n")
      .stream(res3)
      .right('>')(t);
  std::cout << (res3.str() == "tuple<42, 3.14, c, hello world>;\n")
            << std::endl;

  return 0;
}