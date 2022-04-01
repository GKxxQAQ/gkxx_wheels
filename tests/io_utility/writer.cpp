#include <wheels/algo_contest/io_utility.hpp>
#include <vector>
#include <iterator>

int main() {
    using namespace gkxx::io_utility;
    redirect_out("writer.out");
    int data[] = {2357, 11131719, -232, 0, -343, 18};
    std::vector<int> v(std::begin(data), std::end(data));
    ints_writer().sep(',').end('\n')(v);
    ints_writer().sep(',').end('\n')(data);
    ints_writer()(2357, 11131719, -232, 0, -343, 18);
    ints_writer()(30);
    return 0;
}