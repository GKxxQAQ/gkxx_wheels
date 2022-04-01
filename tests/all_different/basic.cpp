#include <wheels/mpl/all_different.hpp>
#include <type_traits>
#include <iostream>

int main() {
    using gkxx::mpl::all_different;
    using std::boolalpha;
    using std::cout;
    using std::endl;
    cout << boolalpha;
    cout << all_different<int, int>::value << endl;         // false
    cout << all_different<int, char>::value << endl;        // true
    cout << all_different<int, char, int>::value << endl;   // false
    cout << all_different<int, int, char>::value << endl;   // false
    return 0;
}