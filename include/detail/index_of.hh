/*
 * Copyright (c) 2022, Kim Nilsson <kim@wayoftao.net>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef DETAIL_INDEX_OF_HH
#define DETAIL_INDEX_OF_HH

#include <type_traits>

namespace nilssonk {

namespace detail {

template<class T, class... Us>
struct index_of;

template<class T, class... Us>
struct index_of<T, T, Us...> : std::integral_constant<unsigned long, 0> {
};

template<class T, class U, class... Us>
struct index_of<T, U, Us...>
    : std::integral_constant<unsigned long, 1 + index_of<T, Us...>::value> {
};

} // namespace detail

} // namespace nilssonk

#endif
