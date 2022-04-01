#include <iostream>
#include <string>
#include <wheels/string_utility.hpp>

int main() {
    using namespace gkxx;
    using namespace std;
    cout << boolalpha;
    cout << (replace_by_kmp("abcabcdabc", "ab", "TSQ") == "TSQcTSQcdTSQc")
         << endl;
    cout << (replace_by_kmp("fwfsdfew,,,fweffe", ",,", "T") ==
             "fwfsdfewT,fweffe")
         << endl;
    cout << (replace_by_kmp("wefwfwe,,,fwwfewfew,,,fwffew,,,zvdvsd", ",,,",
                            ";;;",
                            2) == "wefwfwe;;;fwwfewfew;;;fwffew,,,zvdvsd")
         << endl;
    return 0;
}