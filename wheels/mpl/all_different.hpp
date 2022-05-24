#ifndef GKXX_WHEEL_ALL_DIFFERENT_HPP
#define GKXX_WHEEL_ALL_DIFFERENT_HPP

#include <boost/mpl/and.hpp>
#include <type_traits>
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
    : boost::mpl::and_<all_different<T2, Ts...>,
                       all_different<T1, T2>,
                       all_different<T1, Ts...>> {};

template <typename... Ts>
inline constexpr bool all_different_v = all_different<Ts...>::value;

} // namespace gkxx::mpl

#endif // GKXX_WHEEL_ALL_DIFFERENT_HPP