#include <iostream>
#include <typeinfo>

class Derived;

class Base {
public:
    void fun() const {
        std::cout << (typeid(this) == typeid(const Base *)) << std::endl;
        std::cout << (typeid(this) == typeid(const Derived *)) << std::endl;
    }
    virtual ~Base() = default;
};

class Derived : public Base {};

int main() {
    std::cout << std::boolalpha;
    Derived d;
    Base *bp = &d;
    bp->fun();
    return 0;
}