#ifndef GKXX_ALGO_CONTEST_IO_UTILITY_HPP
#define GKXX_ALGO_CONTEST_IO_UTILITY_HPP

#include <cctype>
#include <cstdio>
#include <string>
#include <type_traits>
#include <utility>
#include <wheels/mpl/is_iterable.hpp>

namespace gkxx {

namespace io_utility {

  namespace __details {

    template <typename T, bool /* true */>
    struct cat_digit_impl {
      void operator()(T &x, int c) {
        x = (x << 1) + (x << 3) + c - 48;
      }
    };

    template <typename T>
    struct cat_digit_impl<T, false> {
      void operator()(T &x, int c) {
        x = x * 10 + c - 48;
      }
    };

    template <typename T>
    inline void cat_digit(T &x, int c) {
      cat_digit_impl<T, std::is_integral<T>::value>()(x, c);
    }

    inline int skip_spaces() {
      int c = getchar();
      while (isspace(c))
        c = getchar();
      return c;
    }

    // return the length of the number
    template <typename T>
    inline int read_unsigned_int(T &x) {
      x = 0;
      int c = skip_spaces();
      if (!isdigit(c))
        return 0;
      int cnt = 0;
      while (isdigit(c)) {
        cat_digit(x, c);
        c = getchar();
        ++cnt;
      }
      ungetc(c, stdin);
      return cnt;
    }

    template <typename T>
    inline int read_int(T &x) {
      int c = skip_spaces();
      if (!isdigit(c) && c != '-') {
        ungetc(c, stdin);
        return 0;
      }
      x = 0;
      bool neg = false;
      if (c == '-') {
        c = getchar();
        neg = true;
        if (!isdigit(c)) {
          ungetc(c, stdin);
          return 0;
        }
      }
      while (isdigit(c)) {
        cat_digit(x, c);
        c = getchar();
      }
      ungetc(c, stdin);
      if (neg)
        x = -x;
      return 1;
    }

    template <typename T>
    inline int read_float(T &x) {
      if (!read_int(x))
        return 0;
      int c = getchar();
      if (c != '.')
        ungetc(c, stdin);
      else {
        T frac = 0;
        double val = 1;
        int c = getchar();
        if (!isdigit(c)) {
          ungetc(c, stdin);
          return 0;
        }
        while (isdigit(c)) {
          val *= 0.1;
          frac += val * (c - 48);
          c = getchar();
        }
        ungetc(c, stdin);
        if (x < 0)
          x -= frac;
        else
          x += frac;
      }
      return 1;
    }

    template <typename T, bool is_integral>
    struct read_select {
      int operator()(T &x) {
        return read_int(x);
      }
    };

    template <typename T>
    struct read_select<T, false> {
      int operator()(T &x) {
        return read_float(x);
      }
    };

    template <typename T>
    void write_int(T x) {
      if (x < 0) {
        putchar('-');
        x = -x;
      }
      if (x > 9)
        write_int(x / 10);
      putchar(x % 10 + 48);
    }

  } // namespace __details

  template <typename T>
  inline int read_int(T &x) {
    return __details::read_int(x);
  }

  template <typename T>
  inline int read_float(T &x) {
    return __details::read_float(x);
  }

  inline int next_char_skipping_space() {
    return __details::skip_spaces();
  }

  template <typename T>
  inline int read(T &x) {
    return __details::read_select<T, std::is_integral<T>::value>()(x);
  }

  template <typename T, typename... Args>
  inline int read(T &x, Args &...args) {
    int first = __details::read_select<T, std::is_integral<T>::value>()(x);
    int rest = read(args...);
    return first + rest;
  }

  template <typename T>
  inline void write_int(T x) {
    __details::write_int(x);
  }

  namespace __details {

    template <typename T, bool is_iterable /* true */>
    struct single_writer_helper {
      single_writer_helper(char delim, char ending, const T &int_seq) {
        using std::begin;
        using std::empty;
        using std::end;
        if (empty(int_seq))
          return;
        auto it = begin(int_seq);
        write_int(*it++);
        while (it != end(int_seq)) {
          putchar(delim);
          write_int(*it++);
        }
        putchar(ending);
      }
    };

    template <typename T>
    struct single_writer_helper<T, false> {
      single_writer_helper(char delim, char ending, const T &x) {
        write_int(x);
        putchar(ending);
      }
    };

  } // namespace __details

  struct ints_writer {
   private:
    char delim, ending;

   public:
    ints_writer(char sep = ' ', char end = '\n') : delim(sep), ending(end) {}
    template <typename T, typename... Args>
    void operator()(T x, Args... args) const {
      __details::write_int(x);
      putchar(delim);
      this->operator()(args...);
    }
    template <typename T>
    void operator()(const T &x) const {
      __details::single_writer_helper<T, mpl::is_iterable<T>::value>(delim,
                                                                     ending, x);
    }
    template <typename T1, typename T2>
    void operator()(T1 x, T2 y) const {
      __details::write_int(x);
      putchar(delim);
      __details::write_int(y);
      putchar(ending);
    }
    ints_writer &sep(char del) {
      delim = del;
      return *this;
    }
    ints_writer &end(char end) {
      ending = end;
      return *this;
    }
  };

  inline void redirect_in(const char *filename) {
    freopen(filename, "r", stdin);
  }

  inline void redirect_in(const std::string &filename) {
    redirect_in(filename.c_str());
  }

  inline void redirect_out(const char *filename) {
    freopen(filename, "w", stdout);
  }

  inline void redirect_out(const std::string &filename) {
    redirect_out(filename.c_str());
  }

} // namespace io_utility

} // namespace gkxx

#endif // GKXX_ALGO_CONTEST_FAST_IO_H