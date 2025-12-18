
#define STEPS_PER_MOTOR_REV_MACRO 800
#define MOTOR_REVS_PER_PINION_REV_MACRO 50
#define PINION_DIAMETER_MM_MACRO 48
#include <ConstexprStepsConversions.h>

#include <RuntimeStepsConversions.h>

#include <unity.h>
#include <RustNumberTypes.h>
#include <stdio.h>


void constexpr_tests(){
    // These tests are performed statically in the actual header, but I want my IDE to be able to do some analysis
    #define TEST_DIST_CONVERSIONS_WITH_VAL(VAL) \
    TEST_ASSERT_MESSAGE(StepsConversionHelpers::ABS( \
        ConstexprStepsConversion::steps_to_hundredths(ConstexprStepsConversion::hundredths_to_steps(VAL)) - VAL) \
        <= static_cast<i32>(StepsConversionHelpers::round_to_nearest(ConstexprStepsConversion::STH_RATIO/2.0)), \
        "Step distance conversion failed 1"); \
    TEST_ASSERT_MESSAGE(StepsConversionHelpers::ABS( \
        ConstexprStepsConversion::hundredths_to_steps(ConstexprStepsConversion::steps_to_hundredths(VAL)) - VAL) \
        <= static_cast<i32>(StepsConversionHelpers::round_to_nearest(ConstexprStepsConversion::HTS_RATIO/2.0)), \
        "Step distance conversion failed 2"); \
    TEST_ASSERT_MESSAGE(StepsConversionHelpers::ABS( \
        ConstexprStepsConversion::steps_to_hundredths(VAL) - ConstexprStepsConversion::slow_steps_to_hundredths(VAL)) \
        <= 0, "Step distance conversion failed 3"); \
    TEST_ASSERT_MESSAGE(StepsConversionHelpers::ABS( \
        ConstexprStepsConversion::hundredths_to_steps(VAL) - ConstexprStepsConversion::slow_hundredths_to_steps(VAL)) \
        <= 0, "Step distance conversion failed 4"); \
    TEST_ASSERT_MESSAGE(StepsConversionHelpers::ABS( \
        ConstexprStepsConversion::f64_steps_to_hundredths(ConstexprStepsConversion::f64_hundredths_to_steps(VAL)) - VAL) \
        <= ConstexprStepsConversion::STH_RATIO/2.0, \
        "Step distance conversion failed 5"); \
    TEST_ASSERT_MESSAGE(StepsConversionHelpers::ABS( \
        ConstexprStepsConversion::f64_hundredths_to_steps(ConstexprStepsConversion::f64_steps_to_hundredths(VAL)) - VAL) \
        <= ConstexprStepsConversion::HTS_RATIO/2.0, \
        "Step distance conversion failed 5"); \

    #define TEST_SPEED_CONVERSIONS_WITH_VAL(VAL) \
    TEST_ASSERT_MESSAGE(StepsConversionHelpers::ABS( \
        ConstexprStepsConversion::sps_to_hpm(ConstexprStepsConversion::hpm_to_sps(VAL)) - VAL) \
        <= static_cast<i32>(StepsConversionHelpers::round_to_nearest(ConstexprStepsConversion::SPS_TO_HPM_RATIO/2.0)), \
        "Step speed conversion failed 1"); \
    TEST_ASSERT_MESSAGE(StepsConversionHelpers::ABS( \
        ConstexprStepsConversion::hpm_to_sps(ConstexprStepsConversion::sps_to_hpm(VAL)) - VAL) \
        <= static_cast<i32>(StepsConversionHelpers::round_to_nearest(ConstexprStepsConversion::HPM_TO_SPS_RATIO/2.0)), \
        "Step speed conversion failed 2"); \
    TEST_ASSERT_MESSAGE(StepsConversionHelpers::ABS( \
        ConstexprStepsConversion::sps_to_hpm(VAL) - ConstexprStepsConversion::slow_sps_to_hpm(VAL)) \
        <= 0, "Step speed conversion failed 3"); \
    TEST_ASSERT_MESSAGE(StepsConversionHelpers::ABS( \
        ConstexprStepsConversion::hpm_to_sps(VAL) - ConstexprStepsConversion::slow_hpm_to_sps(VAL)) \
        <= 0, "Step speed conversion failed 4"); \
    TEST_ASSERT_MESSAGE(StepsConversionHelpers::ABS( \
        ConstexprStepsConversion::f64_sps_to_hpm(ConstexprStepsConversion::f64_hpm_to_sps(VAL)) - VAL) \
        <= ConstexprStepsConversion::SPS_TO_HPM_RATIO/2.0, \
        "Step speed conversion failed 1"); \
    TEST_ASSERT_MESSAGE(StepsConversionHelpers::ABS( \
        ConstexprStepsConversion::f64_hpm_to_sps(ConstexprStepsConversion::f64_sps_to_hpm(VAL)) - VAL) \
        <= ConstexprStepsConversion::HPM_TO_SPS_RATIO/2.0, \
        "Step speed conversion failed 2"); \

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

    TEST_DIST_CONVERSIONS_WITH_VAL(2)
    TEST_DIST_CONVERSIONS_WITH_VAL(3)
    TEST_DIST_CONVERSIONS_WITH_VAL(5)
    TEST_DIST_CONVERSIONS_WITH_VAL(7)
    TEST_DIST_CONVERSIONS_WITH_VAL(11)
    TEST_DIST_CONVERSIONS_WITH_VAL(97)

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

    TEST_DIST_CONVERSIONS_WITH_VAL(67)
    TEST_DIST_CONVERSIONS_WITH_VAL(68)
    TEST_DIST_CONVERSIONS_WITH_VAL(134)
    TEST_DIST_CONVERSIONS_WITH_VAL(135)

    TEST_DIST_CONVERSIONS_WITH_VAL(1000)
    TEST_DIST_CONVERSIONS_WITH_VAL(10000)
    TEST_DIST_CONVERSIONS_WITH_VAL(100000)
    TEST_DIST_CONVERSIONS_WITH_VAL(1000000)

    TEST_SPEED_CONVERSIONS_WITH_VAL((1<<24))
    TEST_SPEED_CONVERSIONS_WITH_VAL(-(1<<24))

    TEST_SPEED_CONVERSIONS_WITH_VAL(2)
    TEST_SPEED_CONVERSIONS_WITH_VAL(3)
    TEST_SPEED_CONVERSIONS_WITH_VAL(5)
    TEST_SPEED_CONVERSIONS_WITH_VAL(7)
    TEST_SPEED_CONVERSIONS_WITH_VAL(11)
    TEST_SPEED_CONVERSIONS_WITH_VAL(97)

    TEST_SPEED_CONVERSIONS_WITH_VAL(2)
    TEST_SPEED_CONVERSIONS_WITH_VAL(4)
    TEST_SPEED_CONVERSIONS_WITH_VAL(16)
    TEST_SPEED_CONVERSIONS_WITH_VAL(64)
    TEST_SPEED_CONVERSIONS_WITH_VAL(256)
    TEST_SPEED_CONVERSIONS_WITH_VAL(1024)
    TEST_SPEED_CONVERSIONS_WITH_VAL(4096)

    TEST_SPEED_CONVERSIONS_WITH_VAL(1)
    TEST_SPEED_CONVERSIONS_WITH_VAL(60)
    TEST_SPEED_CONVERSIONS_WITH_VAL(120)
    TEST_SPEED_CONVERSIONS_WITH_VAL(600)

    TEST_SPEED_CONVERSIONS_WITH_VAL(3000)
    TEST_SPEED_CONVERSIONS_WITH_VAL(6000)
    TEST_SPEED_CONVERSIONS_WITH_VAL(12000)
    TEST_SPEED_CONVERSIONS_WITH_VAL(60000)
    TEST_SPEED_CONVERSIONS_WITH_VAL(300000)
}


void runtime_tests() {
    const auto steps_conversion = RuntimeStepsConversion(ConstexprStepsConversion::STH_RATIO);

#define COMPARE_WITH_CONSTEXPR(VAL) \
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(ConstexprStepsConversion::steps_to_hundredths(VAL), steps_conversion.steps_to_hundredths(VAL), "steps_to_hundredths failed"); \
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(ConstexprStepsConversion::hundredths_to_steps(VAL), steps_conversion.hundredths_to_steps(VAL), "hundredths_to_steps failed"); \
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(ConstexprStepsConversion::sps_to_hpm(VAL), steps_conversion.sps_to_hpm(VAL), "sps_to_hpm failed"); \
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(ConstexprStepsConversion::hpm_to_sps(VAL), steps_conversion.hpm_to_sps(VAL), "hpm_to_sps failed"); \
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(ConstexprStepsConversion::f64_steps_to_hundredths(VAL), steps_conversion.f64_steps_to_hundredths(VAL), "f64_steps_to_hundredths failed"); \
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(ConstexprStepsConversion::f64_hundredths_to_steps(VAL), steps_conversion.f64_hundredths_to_steps(VAL), "f64_hundredths_to_steps failed"); \
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(ConstexprStepsConversion::f64_sps_to_hpm(VAL), steps_conversion.f64_sps_to_hpm(VAL), "f64_sps_to_hpm failed"); \
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(ConstexprStepsConversion::f64_hpm_to_sps(VAL), steps_conversion.f64_hpm_to_sps(VAL), "f64_hpm_to_sps failed");

    COMPARE_WITH_CONSTEXPR(489000)
    COMPARE_WITH_CONSTEXPR(489)
    COMPARE_WITH_CONSTEXPR(1)
    COMPARE_WITH_CONSTEXPR(0)
    COMPARE_WITH_CONSTEXPR(800000)
    COMPARE_WITH_CONSTEXPR(-489000)
    COMPARE_WITH_CONSTEXPR(-489)
    COMPARE_WITH_CONSTEXPR(-1)
    COMPARE_WITH_CONSTEXPR(-0)
    COMPARE_WITH_CONSTEXPR(-800000)


    COMPARE_WITH_CONSTEXPR((1<<22))
    COMPARE_WITH_CONSTEXPR(-(1<<22))

    COMPARE_WITH_CONSTEXPR(2)
    COMPARE_WITH_CONSTEXPR(3)
    COMPARE_WITH_CONSTEXPR(5)
    COMPARE_WITH_CONSTEXPR(7)
    COMPARE_WITH_CONSTEXPR(11)
    COMPARE_WITH_CONSTEXPR(97)

    COMPARE_WITH_CONSTEXPR(2)
    COMPARE_WITH_CONSTEXPR(4)
    COMPARE_WITH_CONSTEXPR(8)
    COMPARE_WITH_CONSTEXPR(16)
    COMPARE_WITH_CONSTEXPR(32)
    COMPARE_WITH_CONSTEXPR(64)
    COMPARE_WITH_CONSTEXPR(128)
    COMPARE_WITH_CONSTEXPR(256)
    COMPARE_WITH_CONSTEXPR(512)
    COMPARE_WITH_CONSTEXPR(1024)
    COMPARE_WITH_CONSTEXPR(2048)
    COMPARE_WITH_CONSTEXPR(4096)
    COMPARE_WITH_CONSTEXPR(8192)

    COMPARE_WITH_CONSTEXPR(67)
    COMPARE_WITH_CONSTEXPR(68)
    COMPARE_WITH_CONSTEXPR(134)
    COMPARE_WITH_CONSTEXPR(135)

    COMPARE_WITH_CONSTEXPR(1000)
    COMPARE_WITH_CONSTEXPR(10000)
    COMPARE_WITH_CONSTEXPR(100000)
    COMPARE_WITH_CONSTEXPR(1000000)
}


void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}


int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(constexpr_tests);
    RUN_TEST(runtime_tests);
    return UNITY_END();
}
