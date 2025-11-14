/**
 * Example configuration:
 * @code{.cpp}
 * #define STEPS_PER_MOTOR_REV_MACRO 800
 * #define MOTOR_REVS_PER_PINION_REV_MACRO 100
 * // Common pinions
 * // ATLANTA 24 99 232 (helical): 67.91 mm
 * // ANDANTEX A30-021-300: 48 mm
 * #define PINION_DIAMETER_MM_MACRO 67.91
 * #include "StepsConversions.h"
 * #undef STEPS_PER_MOTOR_REV_MACRO
 * #undef MOTOR_REVS_PER_PINION_REV_MACRO
 * #undef PINION_DIAMETER_MM_MACRO
 * @endcode
 */


#ifndef STEPS_CONVERSIONS_H
#define STEPS_CONVERSIONS_H
#include <RustNumberTypes.h>

#define STEPS_PER_MOTOR_REV_MACRO 800
#define MOTOR_REVS_PER_PINION_REV_MACRO 50
#define PINION_DIAMETER_MM_MACRO 48






static constexpr f64 PI_VAL = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034;


// There are probably better ways to do this and ABS is always a crowded namespace
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

using namespace StepsConversionHelpers;

#ifndef FIXED_BITS_MACRO
static constexpr i64 FIXED_BITS = 30;
#else
static constexpr i64 FIXED_BITS = FIXED_BITS_MACRO;
#endif

static constexpr i64 FIXED_DENOMINATOR = 1ll << FIXED_BITS;
static constexpr i64 HALF_FIXED_DENOMINATOR = 1ll << (FIXED_BITS-1);

#ifndef STEPS_PER_MOTOR_REV_MACRO
#error "Need to assign `STEPS_PER_MOTOR_REV_MACRO` to a float"
#endif
#ifndef MOTOR_REVS_PER_PINION_REV_MACRO
#error "Need to assign `MOTOR_REVS_PER_PINION_REV_MACRO` to a float"
#endif
#ifndef PINION_DIAMETER_MM_MACRO
#error "Need to assign `PINION_DIAMETER_MM_MACRO` to a float"
#endif

static constexpr f64 STEPS_PER_MOTOR_REV = STEPS_PER_MOTOR_REV_MACRO;
static constexpr f64 MOTOR_REVS_PER_PINION_REV = MOTOR_REVS_PER_PINION_REV_MACRO;
// Common pinions
// ATLANTA 24 99 232 (helical): 67.91 mm
// ANDANTEX A30-021-300: 48 mm
static constexpr f64 PINION_DIAMETER_MM = PINION_DIAMETER_MM_MACRO;

static_assert(STEPS_PER_MOTOR_REV > 0.0, "STEPS_PER_MOTOR_REV_MACRO must be > 0");
static_assert(MOTOR_REVS_PER_PINION_REV > 0.0, "MOTOR_REVS_PER_PINION_REV_MACRO must be > 0");
static_assert(PINION_DIAMETER_MM > 0.0, "PINION_DIAMETER_MM_MACRO must be > 0");

static constexpr f64 PINION_CIRCUMFERENCE_MM = PINION_DIAMETER_MM * PI_VAL;
static constexpr f64 MM_PER_PINION_REV = PINION_CIRCUMFERENCE_MM;

static constexpr f64 MM_TRAVEL_PER_MOTOR_REV = MM_PER_PINION_REV / MOTOR_REVS_PER_PINION_REV;
static constexpr f64 INCH_TRAVEL_PER_MOTOR_REV = MM_TRAVEL_PER_MOTOR_REV / 25.4;
static constexpr f64 HUNDREDTH_TRAVEL_PER_MOTOR_REV = INCH_TRAVEL_PER_MOTOR_REV * 100;

static constexpr f64 MM_TRAVEL_PER_STEP = MM_TRAVEL_PER_MOTOR_REV / STEPS_PER_MOTOR_REV;
static constexpr f64 HUNDREDTH_TRAVEL_PER_STEP = HUNDREDTH_TRAVEL_PER_MOTOR_REV / STEPS_PER_MOTOR_REV;

static constexpr f64 STEPS_PER_HUNDREDTH_TRAVEL = STEPS_PER_MOTOR_REV / HUNDREDTH_TRAVEL_PER_MOTOR_REV;
static constexpr f64 STEPS_PER_MM_TRAVEL = STEPS_PER_MOTOR_REV / MM_TRAVEL_PER_MOTOR_REV;

// hundredths to steps
static constexpr f64 HTS_RATIO = STEPS_PER_HUNDREDTH_TRAVEL;
// hundredths to steps fixed fraction
static constexpr i64 FIXED_HTS_RATIO = round_to_nearest(HTS_RATIO * FIXED_DENOMINATOR);
// steps to hundredths
static constexpr f64 STH_RATIO = HUNDREDTH_TRAVEL_PER_STEP;
// steps to hundredths fixed fraction
static constexpr i64 FIXED_STH_RATIO = round_to_nearest(STH_RATIO * FIXED_DENOMINATOR);

/**
 * convert steps at motor to hundredths of an inch of travel
 * @param steps input range [-2^22..2^22]
 */
static constexpr i32 steps_to_hundredths(const i32 steps) {
    return static_cast<i32>(((static_cast<i64>(steps) * FIXED_STH_RATIO)+HALF_FIXED_DENOMINATOR) >> FIXED_BITS);
}

/**
 * convert hundredths of an inch of travel to steps at motor
 * @param hundredths input range [-2^22..2^22]
 */
static constexpr i32 hundredths_to_steps(const i32 hundredths) {
    return static_cast<i32>(((static_cast<i64>(hundredths) * FIXED_HTS_RATIO)+HALF_FIXED_DENOMINATOR) >> FIXED_BITS);
}

/**
 * Self explanatory, only used to verify fast integer math alternative
 */
static constexpr f64 f64_steps_to_hundredths(const f64 steps) {
    return steps * STH_RATIO;
}

/**
 * Self explanatory, only used to verify fast integer math alternative
 */
static constexpr f64 f64_hundredths_to_steps(const f64 hundredths) {
    return hundredths * HTS_RATIO;
}


/**
 * Self explanatory, only used to verify fast integer math alternative
 */
static constexpr i32 slow_steps_to_hundredths(const i32 steps) {
    return static_cast<i32>(round_to_nearest(f64_steps_to_hundredths(steps)));
}
/**
 * Self explanatory, only used to verify fast integer math alternative
 */
static constexpr i32 slow_hundredths_to_steps(const i32 hundredths) {
    return static_cast<i32>(round_to_nearest(f64_hundredths_to_steps(hundredths)));
}

// hundredths per minute to steps per second
static constexpr f64 HPM_TO_SPS_RATIO = HTS_RATIO/60;
// hundredths per minute to steps per second fixed fraction
static constexpr i64 FIXED_HPM_TO_SPS_RATIO = round_to_nearest(HPM_TO_SPS_RATIO * FIXED_DENOMINATOR);
// steps per second to hundredths per minute
static constexpr f64 SPS_TO_HPM_RATIO = 1/HPM_TO_SPS_RATIO;
// steps per second to hundredths per minute fixed fraction
static constexpr i64 FIXED_SPS_TO_HPM_RATIO = round_to_nearest(SPS_TO_HPM_RATIO * FIXED_DENOMINATOR);

/**
 * Steps per second to hundredths per minute
 * @param steps_per_second input range [-2^22..2^22]
 */
static constexpr i32 sps_to_hpm(const i32 steps_per_second) {
    return static_cast<i32>(((static_cast<i64>(steps_per_second) * FIXED_SPS_TO_HPM_RATIO)+HALF_FIXED_DENOMINATOR) >> FIXED_BITS);
}

/**
 * hundredths per minute to steps per second
 * @param hundredths_per_minute input range [-2^22..2^22]
 */
static constexpr i32 hpm_to_sps(const i32 hundredths_per_minute) {
    return static_cast<i32>(((static_cast<i64>(hundredths_per_minute) * FIXED_HPM_TO_SPS_RATIO)+HALF_FIXED_DENOMINATOR) >> FIXED_BITS);
}


static constexpr f64 f64_sps_to_hpm(const f64 steps_per_second) {
    return steps_per_second * SPS_TO_HPM_RATIO;
}

static constexpr f64 f64_hpm_to_sps(const f64 hundredths_per_minute) {
    return hundredths_per_minute * HPM_TO_SPS_RATIO;
}

/**
 * Steps per second to hundredths per minute
 *
 * Slow version, only used to verify fast integer math alternative
 */
static constexpr i32 slow_sps_to_hpm(const i32 steps_per_second) {
    return static_cast<i32>(round_to_nearest(f64_sps_to_hpm(steps_per_second)));
}
/**
 * hundredths per minute to steps per second
 *
 * Slow version, only used to verify fast integer math alternative
 */
static constexpr i32 slow_hpm_to_sps(const i32 hundredths_per_minute) {
    return static_cast<i32>(round_to_nearest(f64_hpm_to_sps(hundredths_per_minute)));
}






#define TEST_DIST_CONVERSIONS_WITH_VAL(VAL) \
static_assert(ABS(steps_to_hundredths(hundredths_to_steps(VAL)) - VAL) \
    <= static_cast<i32>(round_to_nearest(STH_RATIO/2.0)), "Step conversion failed 1"); \
static_assert(ABS(hundredths_to_steps(steps_to_hundredths(VAL)) - VAL) \
    <= static_cast<i32>(round_to_nearest(HTS_RATIO/2.0)), "Step conversion failed 2"); \
static_assert(ABS(steps_to_hundredths(VAL) - slow_steps_to_hundredths(VAL)) \
    <= 0, "Step conversion failed 3"); \
static_assert(ABS(hundredths_to_steps(VAL) - slow_hundredths_to_steps(VAL)) \
    <= 0, "Step conversion failed 4"); \
static_assert(ABS(f64_steps_to_hundredths(f64_hundredths_to_steps(VAL)) - VAL) \
    <= STH_RATIO/2.0, "Step conversion failed 5"); \
static_assert(ABS(f64_hundredths_to_steps(f64_steps_to_hundredths(VAL)) - VAL) \
    <= HTS_RATIO/2.0, "Step conversion failed 5"); \

#define TEST_SPEED_CONVERSIONS_WITH_VAL(VAL) \
static_assert(ABS(sps_to_hpm(hpm_to_sps(VAL)) - VAL) \
    <= static_cast<i32>(round_to_nearest(SPS_TO_HPM_RATIO/2.0)), "Step conversion failed 1"); \
static_assert(ABS(hpm_to_sps(sps_to_hpm(VAL)) - VAL) \
    <= static_cast<i32>(round_to_nearest(HPM_TO_SPS_RATIO/2.0)), "Step conversion failed 2"); \
static_assert(ABS(sps_to_hpm(VAL) - slow_sps_to_hpm(VAL)) \
    <= 0, "Step conversion failed 3"); \
static_assert(ABS(hpm_to_sps(VAL) - slow_hpm_to_sps(VAL)) \
    <= 0, "Step conversion failed 4"); \
static_assert(ABS(f64_sps_to_hpm(f64_hpm_to_sps(VAL)) - VAL) \
    <= SPS_TO_HPM_RATIO/2.0, "Step conversion failed 1"); \
static_assert(ABS(f64_hpm_to_sps(f64_sps_to_hpm(VAL)) - VAL) \
    <= HPM_TO_SPS_RATIO/2.0, "Step conversion failed 2"); \

TEST_SPEED_CONVERSIONS_WITH_VAL(489000)
TEST_SPEED_CONVERSIONS_WITH_VAL(489)
TEST_SPEED_CONVERSIONS_WITH_VAL(1)
TEST_SPEED_CONVERSIONS_WITH_VAL(0)
TEST_SPEED_CONVERSIONS_WITH_VAL(80000000)
TEST_SPEED_CONVERSIONS_WITH_VAL(-489000)
TEST_SPEED_CONVERSIONS_WITH_VAL(-489)
TEST_SPEED_CONVERSIONS_WITH_VAL(-1)
TEST_SPEED_CONVERSIONS_WITH_VAL(-0)
TEST_SPEED_CONVERSIONS_WITH_VAL(-80000000)

TEST_DIST_CONVERSIONS_WITH_VAL(489000)
TEST_DIST_CONVERSIONS_WITH_VAL(489)
TEST_DIST_CONVERSIONS_WITH_VAL(1)
TEST_DIST_CONVERSIONS_WITH_VAL(0)
TEST_DIST_CONVERSIONS_WITH_VAL(800000)
TEST_DIST_CONVERSIONS_WITH_VAL(-489000)
TEST_DIST_CONVERSIONS_WITH_VAL(-489)
TEST_DIST_CONVERSIONS_WITH_VAL(-1)
TEST_DIST_CONVERSIONS_WITH_VAL(-0)
TEST_DIST_CONVERSIONS_WITH_VAL(-800000)


TEST_DIST_CONVERSIONS_WITH_VAL((1<<22))
TEST_DIST_CONVERSIONS_WITH_VAL(-(1<<22))

// Prime numbers
TEST_DIST_CONVERSIONS_WITH_VAL(2)
TEST_DIST_CONVERSIONS_WITH_VAL(3)
TEST_DIST_CONVERSIONS_WITH_VAL(5)
TEST_DIST_CONVERSIONS_WITH_VAL(7)
TEST_DIST_CONVERSIONS_WITH_VAL(11)
TEST_DIST_CONVERSIONS_WITH_VAL(97)

// Powers of 2 (important for binary operations)
TEST_DIST_CONVERSIONS_WITH_VAL(2)
TEST_DIST_CONVERSIONS_WITH_VAL(4)
TEST_DIST_CONVERSIONS_WITH_VAL(8)
TEST_DIST_CONVERSIONS_WITH_VAL(16)
TEST_DIST_CONVERSIONS_WITH_VAL(32)
TEST_DIST_CONVERSIONS_WITH_VAL(64)
TEST_DIST_CONVERSIONS_WITH_VAL(128)
TEST_DIST_CONVERSIONS_WITH_VAL(256)
TEST_DIST_CONVERSIONS_WITH_VAL(512)
TEST_DIST_CONVERSIONS_WITH_VAL(1024)
TEST_DIST_CONVERSIONS_WITH_VAL(2048)
TEST_DIST_CONVERSIONS_WITH_VAL(4096)
TEST_DIST_CONVERSIONS_WITH_VAL(8192)

// Values around HTS_RATIO
TEST_DIST_CONVERSIONS_WITH_VAL(67)
TEST_DIST_CONVERSIONS_WITH_VAL(68)
TEST_DIST_CONVERSIONS_WITH_VAL(134)
TEST_DIST_CONVERSIONS_WITH_VAL(135)

// Common values in industrial settings
TEST_DIST_CONVERSIONS_WITH_VAL(1000)
TEST_DIST_CONVERSIONS_WITH_VAL(10000)
TEST_DIST_CONVERSIONS_WITH_VAL(100000)
TEST_DIST_CONVERSIONS_WITH_VAL(1000000)

// Similar tests for speed conversions
TEST_SPEED_CONVERSIONS_WITH_VAL((1<<24))
TEST_SPEED_CONVERSIONS_WITH_VAL(-(1<<24))

// Prime numbers for speed
TEST_SPEED_CONVERSIONS_WITH_VAL(2)
TEST_SPEED_CONVERSIONS_WITH_VAL(3)
TEST_SPEED_CONVERSIONS_WITH_VAL(5)
TEST_SPEED_CONVERSIONS_WITH_VAL(7)
TEST_SPEED_CONVERSIONS_WITH_VAL(11)
TEST_SPEED_CONVERSIONS_WITH_VAL(97)

// Powers of 2 for speed
TEST_SPEED_CONVERSIONS_WITH_VAL(2)
TEST_SPEED_CONVERSIONS_WITH_VAL(4)
TEST_SPEED_CONVERSIONS_WITH_VAL(16)
TEST_SPEED_CONVERSIONS_WITH_VAL(64)
TEST_SPEED_CONVERSIONS_WITH_VAL(256)
TEST_SPEED_CONVERSIONS_WITH_VAL(1024)
TEST_SPEED_CONVERSIONS_WITH_VAL(4096)

// Values around HPM_TO_SPS_RATIO
TEST_SPEED_CONVERSIONS_WITH_VAL(1)
TEST_SPEED_CONVERSIONS_WITH_VAL(60)
TEST_SPEED_CONVERSIONS_WITH_VAL(120)
TEST_SPEED_CONVERSIONS_WITH_VAL(600)

// Typical industrial speeds
TEST_SPEED_CONVERSIONS_WITH_VAL(3000)
TEST_SPEED_CONVERSIONS_WITH_VAL(6000)
TEST_SPEED_CONVERSIONS_WITH_VAL(12000)
TEST_SPEED_CONVERSIONS_WITH_VAL(60000)
TEST_SPEED_CONVERSIONS_WITH_VAL(300000)

#undef TEST_SPEED_CONVERSIONS_WITH_VAL
#undef TEST_DIST_CONVERSIONS_WITH_VAL

#endif //STEPS_CONVERSIONS_H
