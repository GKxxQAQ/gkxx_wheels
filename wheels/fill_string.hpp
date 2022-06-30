#ifndef GKXX_WHEEL_FILL_STRING
#define GKXX_WHEEL_FILL_STRING

#include <sstream>
#include <string>

namespace gkxx {

namespace detail {

  using std::to_string;

  template <typename T>
  std::string to_string(const T &t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
  }

  std::string to_string(const char *s) {
    return s;
  }

  std::string to_string(const std::string &s) {
    return s;
  }

} // namespace detail

std::string fill_string(const char *s) {
  return s;
}

template <typename T, typename... Args>
std::string fill_string(const char *str, T &&t, Args &&...args) {
  const char *s = str;
  while (*s && *s != '%')
    ++s;
  if (*s) {
    if (*(s + 1) != '%')
      return std::string(str, s) + detail::to_string(t) +
             fill_string(s + 1, std::forward<Args>(args)...);
    else
      return std::string(str, s + 1) + fill_string(s + 2, std::forward<T>(t),
                                                   std::forward<Args>(args)...);
  } else
    throw std::invalid_argument("Too many arguments in gkxx::fill_string!");
}

template <typename... Args>
std::string fill_string(const std::string &s, Args &&...args) {
  return fill_string(s.c_str(), std::forward<Args>(args)...);
}

} // namespace gkxx

#endif // GKXX_WHEEL_FILL_STRING