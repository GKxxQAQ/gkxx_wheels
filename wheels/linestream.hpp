#ifndef GKXX_WHEEL_LINESTREAM_HPP
#define GKXX_WHEEL_LINESTREAM_HPP

#include <iostream>
#include <iterator>
#include <string>

namespace gkxx {

class __Line_stream_iterator
    : public std::iterator<std::input_iterator_tag, std::string, std::ptrdiff_t,
                           const std::string *, const std::string &> {
 private:
  std::istream *stream;
  char delim;
  std::string current_line;
  bool is_ok;

 public:
  explicit __Line_stream_iterator(std::istream *s, char del)
      : stream(s), delim(del), current_line(), is_ok(true) {
    read();
  }
  __Line_stream_iterator()
      : stream(nullptr), delim('\n'), current_line(), is_ok(false) {}

  __Line_stream_iterator(const __Line_stream_iterator &) = default;
  ~__Line_stream_iterator() = default;
  __Line_stream_iterator &operator=(const __Line_stream_iterator &) = default;

 private:
  using Self = __Line_stream_iterator;
  void read() {
    if (!stream)
      return;
    is_ok = static_cast<bool>(*stream);
    if (is_ok) {
      std::getline(*stream, current_line, delim);
      current_line += delim;
    }
    is_ok = static_cast<bool>(*stream);
  }

 public:
  const std::string &operator*() const {
    return current_line;
  }
  const std::string *operator->() const {
    return std::addressof(this->operator*());
  }
  Self &operator++() {
    read();
    return *this;
  }
  Self operator++(int) {
    auto tmp = *this;
    read();
    return tmp;
  }

  friend bool operator==(const __Line_stream_iterator &lhs,
                         const __Line_stream_iterator &rhs) {
    return lhs.is_ok == rhs.is_ok && (!lhs.is_ok || lhs.stream == rhs.stream);
  }
  friend bool operator!=(const __Line_stream_iterator &lhs,
                         const __Line_stream_iterator &rhs) {
    return !(lhs == rhs);
  }
};

class __Line_stream_helper {
  friend __Line_stream_helper stream_by_line(std::istream &, char);

 private:
  std::istream *stream;
  char delim;
  explicit __Line_stream_helper(std::istream &is, char del)
      : stream(std::addressof(is)), delim(del) {}

 public:
  using iterator = __Line_stream_iterator;
  using const_iterator = iterator;
  iterator begin() const {
    return iterator(stream, delim);
  }
  iterator end() const {
    return iterator();
  }
  const_iterator cbegin() const {
    return begin();
  }
  const_iterator cend() const {
    return end();
  }
};

inline __Line_stream_helper stream_by_line(std::istream &is,
                                           char delim = '\n') {
  return __Line_stream_helper(is, delim);
}

} // namespace gkxx

#endif // GKXX_WHEEL_LINESTREAM_HPP