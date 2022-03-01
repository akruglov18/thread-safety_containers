#pragma once

#include <atomic>
#include <type_traits>
#include <utility>

template <class T>
struct is_atomic :
        std::integral_constant<bool, std::is_same<bool, typename std::remove_cv<T>::type>::value ||
                               std::is_same<int, typename std::remove_cv<T>::type>::value> {};
