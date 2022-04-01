#ifndef GKXX_WHEEL_NOT_HPP
#define GKXX_WHEEL_NOT_HPP

#include <type_traits>

namespace gkxx::mpl {

template <bool>
struct not_ : std::false_type {};

template <>
struct not_<false> : std::true_type {};

template <bool B>
inline constexpr bool not_v = not_<B>::value;

} // namespace gkxx::mpl

#endif // GKXX_WHEEL_NOT_HPP