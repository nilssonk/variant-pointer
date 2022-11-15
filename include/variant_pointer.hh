/*
 * Copyright (c) 2022, Kim Nilsson <kim@wayoftao.net>
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef VARIANT_POINTER_HH
#define VARIANT_POINTER_HH

#if !defined(__cplusplus) || __cplusplus < 201402L
#error "This library requires at least C++14 to be used."
#endif

#include "detail/conjunction.hh"
#include "detail/disjunction.hh"
#include "detail/index_of.hh"

#include <cstdint>
#include <type_traits>

namespace nilssonk {

template<class... Ts>
class variant_pointer {
    const uintptr_t value_;

    static constexpr unsigned long kRequiredAlignment = 1
                                                        << (sizeof...(Ts) - 1);

    template<class T>
    struct alignment_ok
        : std::integral_constant<bool, (alignof(T) > kRequiredAlignment)> {
    };

    template<class T>
    struct type_in_pack : detail::disjunction<std::is_same<T, Ts>...> {
    };

    static_assert(
        detail::conjunction<alignment_ok<Ts>...>::value,
        "Minimum pointer alignment must be sufficiently large to store "
        "target discriminant");

public:
    template<class T,
             class = std::enable_if<type_in_pack<std::remove_const_t<T>>::value>>
    explicit variant_pointer(T * ptr_value) noexcept
        : value_{reinterpret_cast<uintptr_t>(ptr_value) |
                 detail::index_of<std::remove_const_t<T>, Ts...>::value}
    {
    }

    explicit variant_pointer(uintptr_t value) noexcept : value_{value} {}

    explicit operator uintptr_t() const noexcept
    {
        return value_;
    }

    template<class T>
#if __cplusplus >= 201703L
    [[nodiscard]]
#endif
    T *
    try_get() const noexcept
    {
        using RawT = std::remove_const_t<T>;

        auto const sought_index = detail::index_of<RawT, Ts...>::value;
        auto const free_mask = kRequiredAlignment - 1;
        auto const storage_part = value_ & free_mask;
        auto const pointer_part = value_ & ~free_mask;
        if (storage_part == sought_index) {
            return reinterpret_cast<T *>(pointer_part);
        }

        return nullptr;
    }
};

} // namespace nilssonk

#endif
