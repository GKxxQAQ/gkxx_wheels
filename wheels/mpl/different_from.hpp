#ifndef GKXX_WHEEL_DIFFERENT_FROM_HPP
#define GKXX_WHEEL_DIFFERENT_FROM_HPP

#include <type_traits>
#include <wheels/mpl/and.hpp>
#include <wheels/mpl/not.hpp>

namespace gkxx::mpl {

template <typename T, typename... Ts>
struct different_from {};

template <typename T>
struct different_from<T> : std::true_type {};

template <typename T, typename From>
struct different_from<T, From> : not_<std::is_same_v<T, From>> {};

template <typename T, typename From, typename... More>
struct different_from<T, From, More...>
    : and_<different_from<T, From>::value, different_from<T, More...>::value> {
};

template <typename T, typename... Ts>
inline constexpr bool different_from_v = different_from<T, Ts...>::value;

} // namespace gkxx::mpl

#endif // GKXX_WHEEL_DIFFERENT_FROM_HPP