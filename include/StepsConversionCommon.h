//
// Created by pfornage on 12/16/25.
//

#ifndef STEPS_CONVERSION_COMMON_H
#define STEPS_CONVERSION_COMMON_H
#include <RustNumberTypes.h>

static constexpr f64 PI_VAL = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034;

namespace StepsConversionHelpers{
    static constexpr u32 ABS(const i32 a) {
        return (a > 0) ? a : -a;
    }
    static constexpr f64 ABS(const f64 a) {
        return (a > 0) ? a : -a;
    }
    static constexpr f64 HALF = 0.5;
    static constexpr i64 round_to_nearest(const f64 x) {
        return x < 0 ? static_cast<i64>(x - HALF) : static_cast<i64>(x + HALF);
    }

    static_assert(round_to_nearest(0.2) == 0, "round error");
    static_assert(round_to_nearest(0.5) == 1, "round error");
    static_assert(round_to_nearest(0.51) == 1, "round error");
    static_assert(round_to_nearest(51) == 51, "round error");
    static_assert(round_to_nearest(-0.2) == -0, "round error");
    static_assert(round_to_nearest(-0.5) == -1, "round error");
    static_assert(round_to_nearest(-0.51) == -1, "round error");
    static_assert(round_to_nearest(-51) == -51, "round error");
}

#endif