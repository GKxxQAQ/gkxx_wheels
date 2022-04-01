#ifndef GKXX_WHEEL_DEBUG_HELPER
#define GKXX_WHEEL_DEBUG_HELPER

#include <fstream>
#include <iostream>

namespace gkxx {

class Dbstream {
  std::ostream *stream;
  bool newed;

 public:
  Dbstream(std::ostream &os) : stream(&os), newed(false) {}
  Dbstream(const std::string &filename,
           std::ofstream::openmode mode = std::ofstream::out)
      : stream(new std::ofstream(filename, mode)), newed(true) {}
  // Also a default constructor
  Dbstream(std::ofstream::openmode mode = std::ofstream::out)
      : stream(new std::ofstream("debug_helper.out", mode)), newed(true) {}
  ~Dbstream() {
    if (newed)
      delete stream;
  }
  template <typename T>
  void print_var(const std::string &name, const T &x,
                 const std::string &end = "\n") {
    *stream << "\n"
            << "Dbstream print_var output: " << name << " == " << x << end;
  }
  template <typename InputIter>
  void print_list(const std::string &name, InputIter first, InputIter last,
                  const std::string &sep = ", ",
                  const std::string &end = "\n") {
    *stream << "\n"
            << "Dbstream print_list output: " << name << ": " << std::endl;
    while (first + 1 != last)
      *stream << *first++ << sep;
    *stream << *first << end;
  }
  template <typename T>
  Dbstream &operator<<(const T &x) {
    *stream << x;
    return *this;
  }
  Dbstream &operator<<(std::ostream &(*pf)(std::ostream &)) {
    pf(*stream);
    return *this;
  }
};

} // namespace gkxx

#endif // GKXX_WHEEL_DEBUG_HELPER