#pragma once

#include <functional>
#include <type_traits>
#include <utility>

namespace cs {
template <typename T1, typename T2, typename... Ts,
          std::enable_if_t<std::is_member_function_pointer<T1>{} &&
                               std::is_pointer<T2>{},
                           int> = 0>
decltype(auto) invoke(T1 t1, T2 t2, Ts &&... ts) {
  return (t2->*t1)(std::forward<Ts>(ts)...);
}

template <typename T1, typename T2, typename... Ts,
          std::enable_if_t<std::is_member_function_pointer<T1>{} &&
                               std::is_class<std::decay_t<T2>>{},
                           int> = 0>
decltype(auto) invoke(T1 t1, T2 &&t2, Ts &&... ts) {
  return (std::forward<T2>(t2).*t1)(std::forward<Ts>(ts)...);
}

template <typename T1, typename T2, typename... Ts,
          std::enable_if_t<std::is_member_function_pointer<T1>{} &&
                               std::is_class<std::remove_cv_t<T2>>{},
                           int> = 0>
decltype(auto) invoke(T1 t1, std::reference_wrapper<T2> t2, Ts &&... ts) {
  return (t2.get().*t1)(std::forward<Ts>(ts)...);
}

template <
    typename T1, typename T2,
    std::enable_if_t<
        std::is_member_object_pointer<T1>{} && std::is_pointer<T2>{}, int> = 0>
decltype(auto) invoke(T1 t1, T2 t2) {
  return t2->*t1;
}

template <typename T1, typename T2,
          std::enable_if_t<std::is_member_object_pointer<T1>{} &&
                               std::is_class<std::decay_t<T2>>{},
                           int> = 0>
decltype(auto) invoke(T1 t1, T2 &&t2) {
  return std::forward<T2>(t2).*t1;
}

template <typename T1, typename T2,
          std::enable_if_t<std::is_member_object_pointer<T1>{} &&
                               std::is_class<std::remove_cv_t<T2>>{},
                           int> = 0>
decltype(auto) invoke(T1 t1, std::reference_wrapper<T2> t2) {
  return t2.get().*t1;
}

template <typename T1, typename... Ts>
decltype(auto) invoke(T1 &&t1, Ts &&... ts) {
  return std::forward<T1>(t1)(std::forward<Ts>(ts)...);
}

} // namespace cs