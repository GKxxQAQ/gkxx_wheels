#ifndef GKXX_WHEEL_DIFFERENT_FROM_HPP
#define GKXX_WHEEL_DIFFERENT_FROM_HPP

#include <type_traits>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

namespace gkxx::mpl {

template <typename T, typename... Ts>
struct different_from {};

template <typename T>
struct different_from<T> : std::true_type {};

template <typename T, typename From>
struct different_from<T, From> : boost::mpl::not_<std::is_same<T, From>> {};

template <typename T, typename From, typename... More>
struct different_from<T, From, More...>
    : boost::mpl::and_<different_from<T, From>, different_from<T, More...>> {
};

template <typename T, typename... Ts>
inline constexpr bool different_from_v = different_from<T, Ts...>::value;

} // namespace gkxx::mpl

#endif // GKXX_WHEEL_DIFFERENT_FROM_HPP