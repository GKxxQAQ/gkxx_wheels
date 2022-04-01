#include <initializer_list>
#include <iostream>

template <typename T>
void fun(std::initializer_list<T> il) {
    std::cout << "initlist" << std::endl;
}

template <typename T>
void fun(const T &x) {
    std::cout << "normal" << std::endl;
}

int main() {
    fun({1, 2, 3});
    return 0;
}