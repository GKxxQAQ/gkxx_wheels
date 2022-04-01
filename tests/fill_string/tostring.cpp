#include "wheels/fill_string.hpp"
#include <string>
#include <iostream>

struct Point {
    double x, y;
    Point(double a, double b) : x(a), y(b) {}
    Point() : x(0), y(0) {}
};

inline std::ostream &operator<<(std::ostream &os, const Point &p) {
    os << p.x << ' ' << p.y;
    return os;
}

int main() {
    using gkxx::fill_string;
    using std::cout;
    using std::endl;
    Point p;
    std::string s = "hello";
    cout << fill_string("% % % %", "1234", p, s, 42) << endl;
}