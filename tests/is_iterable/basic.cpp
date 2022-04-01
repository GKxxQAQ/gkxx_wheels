#include <wheels/mpl/is_iterable.hpp>
#include <type_traits>
#include <iostream>
#include <wheels/range.hpp>

int main() {
    using std::boolalpha;
    using std::cout;
    using std::endl;
    cout << boolalpha;
    namespace mpl = gkxx::mpl;
    cout << mpl::is_iterable<int[10]>::value << endl;       // true
    cout << mpl::is_iterable<gkxx::Range>::value << endl;   // true
    cout << mpl::is_iterable<const int *>::value << endl;   // false
    cout << mpl::is_iterable<int[]>::value << endl;         // false
    return 0;
}