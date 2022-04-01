#ifndef GKXX_WHEEL_ALL_DIFFERENT_HPP
#define GKXX_WHEEL_ALL_DIFFERENT_HPP

#include <type_traits>
#include <wheels/mpl/and.hpp>
#include <wheels/mpl/different_from.hpp>

namespace gkxx::mpl {

template <typename... Ts>
struct all_different {};

template <>
struct all_different<> : std::true_type {};

template <typename T>
struct all_different<T> : std::true_type {};

template <typename T1, typename T2>
struct all_different<T1, T2> : different_from<T1, T2> {};

template <typename T1, typename T2, typename... Ts>
struct all_different<T1, T2, Ts...>
    : and_<all_different<T2, Ts...>::value, all_different<T1, T2>::value,
           all_different<T1, Ts...>::value> {};

template <typename... Ts>
inline constexpr bool all_different_v = all_different<Ts...>::value;

} // namespace gkxx::mpl

#endif // GKXX_WHEEL_ALL_DIFFERENT_HPP