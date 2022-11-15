/*
 * Copyright (c) 2022, Kim Nilsson <kim@wayoftao.net>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef DETAIL_DISJUNCTION_HH
#define DETAIL_DISJUNCTION_HH

#include <type_traits>

namespace nilssonk {

namespace detail {

template<class...>
struct disjunction : std::false_type {
};

template<class T>
struct disjunction<T> : T {
};

template<class T, class... Ts>
struct disjunction<T, Ts...>
    : std::conditional_t<T::value, T, disjunction<Ts...>> {
};

} // namespace detail

} // namespace nilssonk

#endif
