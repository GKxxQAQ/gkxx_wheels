#ifndef GKXX_WHEEL_LINESTREAM_HPP
#define GKXX_WHEEL_LINESTREAM_HPP

#include <iostream>
#include <iterator>
#include <string>

namespace gkxx {

namespace detail {

  class Line_stream_iterator {
   public:
    using value_type = std::string;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type *;
    using reference = const value_type &;
    using iterator_category = std::input_iterator_tag;

   private:
    std::istream *stream;
    char delim;
    std::string current_line;
    bool is_ok;

   public:
    explicit Line_stream_iterator(std::istream *s, char del)
        : stream(s), delim(del), current_line(), is_ok(true) {
      read();
    }
    Line_stream_iterator()
        : stream(nullptr), delim('\n'), current_line(), is_ok(false) {}

    Line_stream_iterator(const Line_stream_iterator &) = default;
    ~Line_stream_iterator() = default;
    Line_stream_iterator &operator=(const Line_stream_iterator &) = default;

   private:
    using Self = Line_stream_iterator;
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

    friend bool operator==(const Line_stream_iterator &lhs,
                           const Line_stream_iterator &rhs) {
      return lhs.is_ok == rhs.is_ok && (!lhs.is_ok || lhs.stream == rhs.stream);
    }
    friend bool operator!=(const Line_stream_iterator &lhs,
                           const Line_stream_iterator &rhs) {
      return !(lhs == rhs);
    }
  };

  class Line_stream_helper {
    friend Line_stream_helper stream_by_line(std::istream &, char);

   private:
    std::istream *stream;
    char delim;
    explicit Line_stream_helper(std::istream &is, char del)
        : stream(std::addressof(is)), delim(del) {}

   public:
    using iterator = Line_stream_iterator;
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

  inline Line_stream_helper stream_by_line(std::istream &is, char delim = '\n') {
    return Line_stream_helper(is, delim);
  }

} // namespace detail

using detail::stream_by_line;

} // namespace gkxx

#endif // GKXX_WHEEL_LINESTREAM_HPP