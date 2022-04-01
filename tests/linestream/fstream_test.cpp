#include <wheels/linestream.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

using gkxx::stream_by_line;

int main() {
    std::ifstream file("file.txt");
    std::vector<std::string> v;
    auto lines = stream_by_line(file);
    std::copy(lines.begin(), lines.end(), std::back_inserter(v));
    std::ofstream output("out.txt");
    for (const auto &s : v)
        output << s << std::endl;
    system("diff file.txt out.txt");
    return 0;
}