#ifndef GKXX_WHEEL_IS_ITERABLE_HPP
#define GKXX_WHEEL_IS_ITERABLE_HPP

#include <iterator>
#include <type_traits>
#include <utility>

namespace gkxx::mpl {

// https://stackoverflow.com/questions/13830158/check-if-a-variable-type-is-iterable
// Some modifications have been made to it.

namespace __details {

  using std::begin;
  using std::end;

  template <typename T>
  auto is_iterable_impl(int)
      -> decltype(begin(std::declval<T &>()) != end(std::declval<T &>()),
                  void(),
                  ++std::declval<decltype(begin(std::declval<T &>())) &>(),
                  void(*begin(std::declval<T &>())), std::true_type{});

  template <typename T>
  std::false_type is_iterable_impl(double);

} // namespace __details

template <typename T>
struct is_iterable : public decltype(__details::is_iterable_impl<T>(0)) {};

template <typename T>
inline constexpr bool is_iterable_v = is_iterable<T>::value;

} // namespace gkxx::mpl

#endif // GKXX_WHEEL_IS_ITERABLE_HPP