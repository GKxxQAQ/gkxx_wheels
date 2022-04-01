#include <wheels/debug_helper.hpp>
#include <iostream>
#include <string>
#include <vector>

int main() {
    // std::cout << gkxx::fill_string("hello", 1, 3.14, 'c') << std::endl;
    gkxx::Dbstream dbs;
    int x = 3, y = 5;
    dbs.print_var("x", x, "");
    dbs.print_var("y", y);
    dbs << std::endl;
    dbs << "x == " << x << ", y == " << y << std::endl;
    std::vector<int> v;
    for (int i = 0; i < 10; ++i)
        v.push_back(i);
    dbs.print_list("v", v.begin(), v.end(), "--", "lalala\n");
    return 0;
}