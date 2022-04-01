#ifndef GKXX_WHEEL_PRINTER_HPP
#define GKXX_WHEEL_PRINTER_HPP

#include <boost/polymorphic_cast.hpp>
#include <initializer_list>
#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <wheels/mpl/all_different.hpp>

namespace gkxx::printer {

template <typename Real_printer>
class __Printer_base;

namespace __arg_modifiers {

  struct sep_arg_tag {};
  struct end_arg_tag {};
  struct stream_arg_tag {};
  struct flush_arg_tag {};

  template <typename Arg_tag>
  class Arg_modifier {};

  template <>
  class Arg_modifier<sep_arg_tag> {
    std::string delim = " ";

   public:
    using value_type = std::string;
    Arg_modifier &operator=(const std::string &sep) {
      delim = sep;
      return *this;
    }
    Arg_modifier &operator=(char sep) {
      delim = std::string(1, sep);
      return *this;
    }
    const value_type &value() const {
      return delim;
    }
  };

  template <>
  class Arg_modifier<end_arg_tag> {
    std::string ending = " ";

   public:
    using value_type = std::string;
    Arg_modifier &operator=(const std::string &end) {
      ending = end;
      return *this;
    }
    Arg_modifier &operator=(char end) {
      ending = std::string(1, end);
      return *this;
    }
    const value_type &value() const {
      return ending;
    }
  };

  template <>
  class Arg_modifier<stream_arg_tag> {
    std::ostream *ostrm = &std::cout;

   public:
    using value_type = std::ostream *;
    Arg_modifier &operator=(std::ostream &os) {
      ostrm = &os;
      return *this;
    }
    const value_type &value() const {
      return ostrm;
    }
  };

  template <>
  class Arg_modifier<flush_arg_tag> {
    bool to_flush = false;

   public:
    using value_type = bool;
    Arg_modifier &operator=(bool flush) {
      to_flush = flush;
      return *this;
    }
    const value_type &value() const {
      return to_flush;
    }
  };

  Arg_modifier<sep_arg_tag> sep;
  Arg_modifier<end_arg_tag> end;
  Arg_modifier<stream_arg_tag> stream;
  Arg_modifier<flush_arg_tag> flush;

  namespace __details {

    template <typename Want>
    inline constexpr const Want *optional_get() {
      return nullptr;
    }

    template <typename Want, typename T1, typename... Others>
    inline constexpr const Want *optional_get(T1 &&first, Others &&...others);

    template <typename Want, bool Same /* false */>
    struct decide {
      template <typename T1, typename... Others>
      constexpr const Want *operator()(T1 &&first, Others &&...others) const {
        return optional_get<Want>(std::forward<Others>(others)...);
      }
    };

    template <typename Want>
    struct decide<Want, true> {
      template <typename T1, typename... Others>
      constexpr const Want *operator()(T1 &&first, Others &&...others) const {
        return &first;
      }
    };

    template <typename Want, typename T1, typename... Others>
    inline constexpr const Want *optional_get(T1 &&first, Others &&...others) {
      return decide<Want, std::is_same_v<Want, std::remove_reference_t<T1>>>()(
          std::forward<T1>(first), std::forward<Others>(others)...);
    }

  } // namespace __details

  template <typename Modifier, typename... Tags>
  inline constexpr auto
  get_value(const typename Modifier::value_type &default_val,
            const Arg_modifier<Tags> &...args) -> const
      typename Modifier::value_type & {
    const Modifier *got = __details::optional_get<const Modifier>(args...);
    return got ? got->value() : default_val;
  }

} // namespace __arg_modifiers

using __arg_modifiers::end;
using __arg_modifiers::flush;
using __arg_modifiers::sep;
using __arg_modifiers::stream;

// CRTP
template <typename Real_printer>
class __Printer_base {
 protected:
  std::ostream *ostrm;
  std::string delim, ending;
  bool to_flush;

  template <typename Arg_tag>
  using arg_modifier = __arg_modifiers::Arg_modifier<Arg_tag>;

  using sep_modifier = arg_modifier<__arg_modifiers::sep_arg_tag>;
  using end_modifier = arg_modifier<__arg_modifiers::end_arg_tag>;
  using stream_modifier = arg_modifier<__arg_modifiers::stream_arg_tag>;
  using flush_modifier = arg_modifier<__arg_modifiers::flush_arg_tag>;

 public:
  template <typename... Tags>
  __Printer_base(const arg_modifier<Tags> &...args)
      : ostrm(__arg_modifiers::get_value<stream_modifier>(&std::cout, args...)),
        delim(__arg_modifiers::get_value<sep_modifier>(" ", args...)),
        ending(__arg_modifiers::get_value<end_modifier>("\n", args...)),
        to_flush(__arg_modifiers::get_value<flush_modifier>(false, args...)) {
    static_assert(mpl::all_different_v<Tags...>, "Printer arguments repeated!");
  }

  virtual ~__Printer_base() = default;

  // The following member functions use static_cast to do downcasting.
  // It is ensured that Real_printer is derived from __Printer_base,
  // and therefore the downcasting is always safe.

  Real_printer &sep(const std::string &sep_) {
    delim = sep_;
    return boost::polymorphic_downcast<Real_printer &>(*this);
  }
  Real_printer &sep(char sep_) {
    delim = std::string(1, sep_);
    return boost::polymorphic_downcast<Real_printer &>(*this);
  }

  Real_printer &end(const std::string &end_) {
    ending = end_;
    return boost::polymorphic_downcast<Real_printer &>(*this);
  }
  Real_printer &end(char end_) {
    ending = end_;
    return boost::polymorphic_downcast<Real_printer &>(*this);
  }

  Real_printer &stream(std::ostream &os) {
    ostrm = &os;
    return boost::polymorphic_downcast<Real_printer &>(*this);
  }

  Real_printer &flush(bool flush_) {
    to_flush = flush_;
    return boost::polymorphic_downcast<Real_printer &>(*this);
  }
};

class Printer : public __Printer_base<Printer> {
  using __Printer_base::__Printer_base; // public

 private:
  template <typename T>
  void print_without_flush(const T &x) const {
    *ostrm << x << ending;
  }
  template <typename T1, typename T2>
  void print_without_flush(const T1 &x, const T2 &y) const {
    *ostrm << x << delim << y << ending;
  }
  template <typename T1, typename T2, typename... More>
  void print_without_flush(const T1 &x, const T2 &y,
                           const More &...more) const {
    *ostrm << x << delim << y << delim;
    print_without_flush(more...);
  }

 public:
  template <typename... Ts>
  const Printer &print(const Ts &...things) const {
    print_without_flush(things...);
    if (to_flush)
      *ostrm << std::flush;
    return *this;
  }
  template <typename... Ts>
  Printer &print(const Ts &...things) {
    return const_cast<Printer &>(
        static_cast<const Printer *>(this)->print(things...));
  }
  template <typename... Ts>
  const Printer &operator()(const Ts &...things) const {
    return print(things...);
  }
  template <typename... Ts>
  Printer &operator()(const Ts &...things) {
    return print(things...);
  }
};

// sequence printer

namespace __tuple_helper {

  template <std::size_t cur, typename... Ts>
  struct print_tuple {
    void operator()(const std::tuple<Ts...> &t, std::ostream &os,
                    const std::string &delim) const {
      print_tuple<cur - 1, Ts...>()(t, os, delim);
      os << delim << std::get<cur>(t);
    }
  };

  template <typename... Ts>
  struct print_tuple<0ul, Ts...> {
    void operator()(const std::tuple<Ts...> &t, std::ostream &os,
                    const std::string &) const {
      os << std::get<0>(t);
    }
  };

  template <std::size_t cur>
  struct print_tuple<cur> {
    void operator()(const std::tuple<> &, std::ostream &,
                    const std::string &) const {}
  };

} // namespace __tuple_helper

class Seq_printer;

namespace __arg_modifiers {

  struct left_arg_tag {};
  struct right_arg_tag {};

  template <>
  class Arg_modifier<left_arg_tag> {
    friend class ::gkxx::printer::Seq_printer;

    std::optional<std::string> left_brac;

   public:
    using value_type = std::optional<std::string>;
    Arg_modifier &operator=(const std::string &left) {
      left_brac.emplace(left);
      return *this;
    }
    Arg_modifier &operator=(char left) {
      left_brac.emplace(1, left);
      return *this;
    }
    const value_type &value() const {
      return left_brac;
    }
  };

  template <>
  class Arg_modifier<right_arg_tag> {
    friend class ::gkxx::printer::Seq_printer;

    std::optional<std::string> right_brac;

   public:
    using value_type = std::optional<std::string>;
    Arg_modifier &operator=(const std::string &right) {
      right_brac.emplace(right);
      return *this;
    }
    Arg_modifier &operator=(char right) {
      right_brac.emplace(1, right);
      return *this;
    }
    const value_type &value() const {
      return right_brac;
    }
  };

  Arg_modifier<left_arg_tag> left;
  Arg_modifier<right_arg_tag> right;

} // namespace __arg_modifiers

using __arg_modifiers::left;
using __arg_modifiers::right;

class Seq_printer : public __Printer_base<Seq_printer> {
 private:
  std::optional<std::string> left_brac, right_brac;

  using left_modifier = arg_modifier<__arg_modifiers::left_arg_tag>;
  using right_modifier = arg_modifier<__arg_modifiers::right_arg_tag>;

  template <typename Input_iter>
  void print_iter_range(Input_iter begin_, Input_iter end_) const {
    if (begin_ != end_) {
      *ostrm << *begin_++;
      while (begin_ != end_)
        *ostrm << delim << *begin_++;
    }
  }
  template <typename T1, typename T2>
  void do_print(const std::pair<T1, T2> &p) const {
    *ostrm << left_brac.value_or("(") << p.first << delim << p.second
           << right_brac.value_or(")") << ending;
  }
  template <typename... Ts>
  void do_print(const std::tuple<Ts...> &t) const {
    using helper = __tuple_helper::print_tuple<sizeof...(Ts) - 1, Ts...>;
    *ostrm << left_brac.value_or("(");
    helper()(t, *ostrm, delim);
    *ostrm << right_brac.value_or(")") << ending;
  }
  template <typename T>
  void do_print(std::initializer_list<T> il) const {
    *ostrm << left_brac.value_or("{");
    print_iter_range(il.begin(), il.end());
    *ostrm << right_brac.value_or("}") << ending;
  }
  template <typename Seq>
  void do_print(const Seq &seq) const {
    *ostrm << left_brac.value_or("[");
    print_iter_range(std::begin(seq), std::end(seq));
    *ostrm << right_brac.value_or("]") << ending;
  }

 public:
  template <typename Seq>
  const Seq_printer &print(const Seq &seq) const {
    do_print(seq);
    if (to_flush)
      *ostrm << std::flush;
    return *this;
  }
  template <typename Seq>
  Seq_printer &print(const Seq &seq) {
    return const_cast<Seq_printer &>(
        static_cast<const Seq_printer *>(this)->print(seq));
  }
  template <typename Seq>
  const Seq_printer &operator()(const Seq &seq) const {
    return print(seq);
  }
  template <typename Seq>
  Seq_printer &operator()(const Seq &seq) {
    return print(seq);
  }
  template <typename Input_iter>
  const Seq_printer &print(Input_iter begin_, Input_iter end_) const {
    *ostrm << left_brac.value_or("[");
    print_iter_range(begin_, end_);
    *ostrm << right_brac.value_or("]") << ending;
    if (to_flush)
      *ostrm << std::flush;
    return *this;
  }
  template <typename Input_iter>
  Seq_printer &print(Input_iter begin_, Input_iter end_) {
    return const_cast<Seq_printer &>(
        static_cast<const Seq_printer *>(this)->print(begin_, end_));
  }
  template <typename Input_iter>
  const Seq_printer &operator()(Input_iter begin_, Input_iter end_) const {
    return print(begin_, end_);
  }
  template <typename Input_iter>
  Seq_printer &operator()(Input_iter begin_, Input_iter end_) {
    return print(begin_, end_);
  }

 public:
  template <typename... Tags>
  Seq_printer(const arg_modifier<Tags> &...args)
      : __Printer_base(args...),
        left_brac(
            __arg_modifiers::get_value<left_modifier>(std::nullopt, args...)),
        right_brac(__arg_modifiers::get_value<right_modifier>(std::nullopt,
                                                              args...)) {}

  // In particular, call to left/right without arguments provided will reset
  // left_brac/right_brac to default values.
  Seq_printer &left(const std::string &left_) {
    left_brac.emplace(left_);
    return *this;
  }
  Seq_printer &left(char left_) {
    left_brac.emplace(1, left_);
    return *this;
  }
  Seq_printer &left() {
    left_brac.reset();
    return *this;
  }
  Seq_printer &right(const std::string &right_) {
    right_brac.emplace(right_);
    return *this;
  }
  Seq_printer &right(char right_) {
    right_brac.emplace(1, right_);
    return *this;
  }
  Seq_printer &right() {
    right_brac.reset();
    return *this;
  }
};

} // namespace gkxx::printer

#endif // GKXX_WHEEL_PRINTER_HPP