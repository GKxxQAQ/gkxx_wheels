#include <wheels/mpl/count_param.hpp>
#include <iostream>

int main() {
    using std::cout;
    using std::endl;
    using namespace gkxx::mpl;
    cout << count_param<int, int, int>::value << endl;
    cout << count_param<int, int>::value << endl;
    cout << count_param<int>::value << endl;
    cout << count_param<>::value << endl;
    return 0;
}