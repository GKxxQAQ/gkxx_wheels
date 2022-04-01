#include <wheels/mpl/and.hpp>
#include <iostream>

int main() {
    using namespace std;
    using namespace gkxx::mpl;

    cout << boolalpha;
    cout << and_<true, false>::value << endl;               // false
    cout << and_<true, true>::value << endl;                // true
    cout << and_<true, false, false, true>::value << endl;  // false
    cout << and_<true, true, true, true>::value << endl;    // true
    return 0;
}