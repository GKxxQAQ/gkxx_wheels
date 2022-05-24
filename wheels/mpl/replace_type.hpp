#ifndef GKXX_WHEEL_REPLACE_TYPE_HPP
#define GKXX_WHEEL_REPLACE_TYPE_HPP

namespace gkxx::mpl {

template <typename C, typename X, typename Y>
struct replace_type;

template <typename X, typename Y>
struct replace_type<X, X, Y> {
  using type = Y;
};

namespace detail {

  template <typename C, typename X, typename Y>
  struct replace_type_impl {
    using type = C;
  };

  template <typename C, typename X, typename Y>
  struct replace_type_impl<C *, X, Y> {
    using type = typename replace_type<C, X, Y>::type *;
  };

  template <typename C, typename X, typename Y>
  struct replace_type_impl<C &, X, Y> {
    using type = typename replace_type<C, X, Y>::type &;
  };

  template <typename C, typename X, typename Y>
  struct replace_type_impl<C &&, X, Y> {
    using type = typename replace_type<C, X, Y>::type &&;
  };

  template <typename C, typename X, typename Y>
  struct replace_type_impl<C const, X, Y> {
    using type = typename replace_type<C, X, Y>::type const;
  };

  template <typename C, typename X, typename Y>
  struct replace_type_impl<C volatile, X, Y> {
    using type = typename replace_type<C, X, Y>::type volatile;
  };

  template <typename C, unsigned N, typename X, typename Y>
  struct replace_type_impl<C[N], X, Y> {
    using type = typename replace_type<C, X, Y>::type[N];
  };

  template <typename C, typename X, typename Y>
  struct replace_type_impl<C[], X, Y> {
    using type = typename replace_type<C, X, Y>::type[];
  };

  template <typename C, typename Class, typename X, typename Y>
  struct replace_type_impl<C Class::*, X, Y> {
    using type = typename replace_type<C, X, Y>::type Class::*;
  };

  template <typename C, typename X, typename Y, typename... Args>
  struct replace_type_impl<C(Args...), X, Y> {
    using type = typename replace_type<C, X, Y>::type(
        typename replace_type<Args, X, Y>::type...);
  };

} // namespace detail

template <typename C, typename X, typename Y>
struct replace_type : detail::replace_type_impl<C, X, Y> {};

template <typename C, typename X, typename Y>
using replace_type_t = typename replace_type<C, X, Y>::type;

} // namespace gkxx::mpl

#endif // GKXX_WHEEL_REPLACE_TYPE_HPP