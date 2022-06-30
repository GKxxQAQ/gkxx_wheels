#ifndef GKXX_WHEEL_COLOR_HPP
#define GKXX_WHEEL_COLOR_HPP

#include <string>
#include <string_view>

#define ANSI_COLOR_BLACK "\x1b[30m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_COLOR_RESET "\x1b[0m"

namespace gkxx::color {

template <typename Streamable>
class Colored {
  Streamable &&thing;
  std::string_view color;

 public:
  constexpr Colored(Streamable &&x, const char *const c)
      : thing(std::forward<Streamable>(x)), color(c) {}

  std::ostream &print(std::ostream &os) const {
    return os << color << thing << ANSI_COLOR_RESET;
  }
};

template <typename Streamable>
inline std::ostream &operator<<(std::ostream &os,
                                const Colored<Streamable> &colored) {
  return colored.print(os);
}

template <typename Streamable>
inline Colored<Streamable> black(Streamable &&x) {
  return {std::forward<Streamable>(x), ANSI_COLOR_BLACK};
}

template <typename Streamable>
inline Colored<Streamable> red(Streamable &&x) {
  return {std::forward<Streamable>(x), ANSI_COLOR_RED};
}

template <typename Streamable>
inline Colored<Streamable> green(Streamable &&x) {
  return {std::forward<Streamable>(x), ANSI_COLOR_GREEN};
}

template <typename Streamable>
inline Colored<Streamable> yellow(Streamable &&x) {
  return {std::forward<Streamable>(x), ANSI_COLOR_YELLOW};
}

template <typename Streamable>
inline Colored<Streamable> blue(Streamable &&x) {
  return {std::forward<Streamable>(x), ANSI_COLOR_BLUE};
}

template <typename Streamable>
inline Colored<Streamable> magenta(Streamable &&x) {
  return {std::forward<Streamable>(x), ANSI_COLOR_MAGENTA};
}

template <typename Streamable>
inline Colored<Streamable> cyan(Streamable &&x) {
  return {std::forward<Streamable>(x), ANSI_COLOR_CYAN};
}

template <typename Streamable>
inline Colored<Streamable> white(Streamable &&x) {
  return {std::forward<Streamable>(x), ANSI_COLOR_WHITE};
}

} // namespace gkxx::color

#endif // GKXX_WHEEL_COLOR_HPP