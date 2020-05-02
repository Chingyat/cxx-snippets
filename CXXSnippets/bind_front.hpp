#pragma once

#include "invoke.hpp"
#include <tuple>
#include <type_traits>
#include <utility>

namespace cs {

namespace bind_front_impl_ {
template <typename Function, typename Tuple, typename... Args,
          std::size_t... Is>
constexpr auto bind_front_impl(Function &&f, Tuple &&tuple,
                               std::index_sequence<Is...>, Args &&... args) {
  return cs::invoke(f, std::get<Is>(tuple)..., std::forward<Args>(args)...);
  //   return f(std::get<Is>(tuple)..., std::forward<Args>(args)...);
}
} // namespace bind_front_impl_

template <
    typename Function, typename... Args,
    std::enable_if_t<(std::is_copy_constructible<Args>::value && ...), int> = 0>
constexpr auto bind_front(Function &&f, Args &&... args) {
  return [
    f = std::forward<Function>(f),
    args = std::make_tuple(std::forward<Args>(args)...)
  ](auto &&... rest) constexpr mutable {
    return bind_front_impl_::bind_front_impl(
        f, args, std::index_sequence_for<Args...>(),
        std::forward<decltype(rest)>(rest)...);
  };
}
} // namespace cs