#ifndef GKXX_WHEEL_OR_HPP
#define GKXX_WHEEL_OR_HPP

#include <type_traits>

namespace gkxx::mpl {

template <bool, bool, bool...>
struct or_;

template <bool X, bool Y>
struct or_<X, Y> : std::true_type {};

template <>
struct or_<false, false> : std::false_type {};

template <bool X, bool Y, bool... Others>
struct or_ : or_<X, or_<Y, Others...>::value> {};

template <bool X, bool Y, bool... Others>
inline constexpr bool or_v = or_<X, Y, Others...>::value;

} // namespace gkxx::mpl

#endif // GKXX_WHEEL_OR_HPP