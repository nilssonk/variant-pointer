#include "variant_pointer.hh"

#include <catch2/catch_test_macros.hpp>
#include <variant>

TEST_CASE("variant_pointer is constructed", "[construct]")
{
    using IntOrFloatPtr = nilssonk::variant_pointer<int, float>;

    {
        float const         x{1.0f};
        IntOrFloatPtr const y{&x};

        auto * const maybe_int = y.try_get<int>();
        REQUIRE(maybe_int == nullptr);

        auto * const maybe_float = y.try_get<float>();
        REQUIRE(maybe_float == &x);
    }

    {
        int const           x{1};
        IntOrFloatPtr const y{&x};

        auto * const maybe_int = y.try_get<int>();
        REQUIRE(maybe_int == &x);

        auto * const maybe_float = y.try_get<float>();
        REQUIRE(maybe_float == nullptr);
    }
}