/*
 * Copyright (c) 2022, Kim Nilsson <kim@wayoftao.net>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef DETAIL_CONJUNCTION_HH
#define DETAIL_CONJUNCTION_HH

#include <type_traits>

namespace nilssonk {

namespace detail {

template<class...>
struct conjunction : std::true_type {
};

template<class T>
struct conjunction<T> : T {
};

template<class T, class... Ts>
struct conjunction<T, Ts...>
    : std::conditional_t<T::value, conjunction<Ts...>, T> {
};

} // namespace detail

} // namespace nilssonk

#endif
