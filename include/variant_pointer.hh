/*
 * Copyright (c) 2022, Kim Nilsson <kim@wayoftao.net>
 *
 * SPDX-License-Identifier: MIT
 */

#include <cstdint>
#include <type_traits>

namespace nilssonk {

namespace detail {

template<typename T, typename... Us>
struct index_of;

template<typename T, typename... Us>
struct index_of<T, T, Us...> : std::integral_constant<unsigned long, 0> {
};

template<typename T, typename U, typename... Us>
struct index_of<T, U, Us...>
    : std::integral_constant<unsigned long, 1 + index_of<T, Us...>::value> {
};

} // namespace detail

template<typename... Ts>
class variant_pointer {
    uintptr_t value_;

    static constexpr unsigned long required_alignment = 1
                                                        << (sizeof...(Ts) - 1);

    static_assert(
        std::conjunction_v<
            std::bool_constant<(alignof(Ts) > required_alignment)>...>,
        "Minimum pointer alignment must be sufficiently large to store "
        "target discriminant");

public:
    template<typename T,
             typename = std::enable_if_t<std::disjunction_v<
                 std::is_same<std::remove_const_t<T>, Ts>...>>>
    explicit variant_pointer(T * x) noexcept
        : value_{reinterpret_cast<uintptr_t>(x) |
                 detail::index_of<std::remove_const_t<T>, Ts...>::value}
    {
    }

    explicit variant_pointer(uintptr_t x) noexcept : value_{x} {}

    operator uintptr_t() noexcept
    {
        return value_;
    }

    template<typename T>
    [[nodiscard]] T *
    try_get() const noexcept
    {
        using RawT = std::remove_const_t<T>;

        auto const sought_index = detail::index_of<RawT, Ts...>::value;
        auto const free_mask = required_alignment - 1;
        auto const storage_part = value_ & free_mask;
        auto const pointer_part = value_ & ~free_mask;
        if (storage_part == sought_index) {
            return reinterpret_cast<T *>(pointer_part);
        }

        return nullptr;
    }
};

} // namespace nilssonk