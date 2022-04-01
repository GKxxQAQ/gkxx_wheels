#ifndef GKXX_WHEEL_AND_HPP
#define GKXX_WHEEL_AND_HPP

#include <type_traits>

namespace gkxx::mpl {

template <bool, bool, bool...>
struct and_;

template <bool X, bool Y>
struct and_<X, Y> : std::false_type {};

template <>
struct and_<true, true> : std::true_type {};

template <bool X, bool Y, bool... Others>
struct and_ : and_<X, and_<Y, Others...>::value> {};

template <bool X, bool Y, bool... Others>
inline constexpr bool and_v = and_<X, Y, Others...>::value;

} // namespace gkxx::mpl

#endif // GKXX_WHEEL_AND_HPP