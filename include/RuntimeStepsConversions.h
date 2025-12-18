//
// Created by pfornage on 12/16/25.
//

#ifndef RUNTIME_STEPS_CONVERSIONS_H
#define RUNTIME_STEPS_CONVERSIONS_H

#include <cmath>
#include <RustNumberTypes.h>
#include <StepsConversionCommon.h>


struct RuntimeStepsConversion {
    static constexpr f64 INCHES_PER_MM = 1.0 / 25.4;
    static constexpr f64 HUNDREDTHS_PER_INCH = 100.0;
    static constexpr f64 SECONDS_PER_MINUTE = 60.0;
    static constexpr f64 HALF = 0.5;

    // Inputs (caller-owned lifecycle)
    f64 steps_per_motor_rev = 0.0;
    f64 motor_revs_per_pinion_rev = 0.0;
    f64 pinion_diameter_mm = 0.0;

    // Precomputed factors (caller must keep fresh by calling recalculate())
    f64 sth_ratio = 0.0;         // steps -> hundredths
    f64 hts_ratio = 0.0;         // hundredths -> steps
    f64 hpm_to_sps_ratio = 0.0;  // hundredths/min -> steps/sec
    f64 sps_to_hpm_ratio = 0.0;  // steps/sec -> hundredths/min

    RuntimeStepsConversion() = delete;

    RuntimeStepsConversion(const f64 steps_per_motor_rev_,
                           const f64 motor_revs_per_pinion_rev_,
                           const f64 pinion_diameter_mm_) {
        steps_per_motor_rev = steps_per_motor_rev_;
        motor_revs_per_pinion_rev = motor_revs_per_pinion_rev_;
        pinion_diameter_mm = pinion_diameter_mm_;
        recalculate();
    }

    // Contract: you (the caller) may update any combination of public inputs,
    // then MUST call recalculate() before using conversions.
    bool recalculate() {
        if (!(steps_per_motor_rev > 0.0) ||
            !(motor_revs_per_pinion_rev > 0.0) ||
            !(pinion_diameter_mm > 0.0)) {
            sth_ratio = 0.0;
            hts_ratio = 0.0;
            hpm_to_sps_ratio = 0.0;
            sps_to_hpm_ratio = 0.0;
            return false;
        }

        const f64 pinion_circumference_mm = pinion_diameter_mm * PI_VAL;
        const f64 mm_per_motor_rev = pinion_circumference_mm / motor_revs_per_pinion_rev;
        const f64 inches_per_motor_rev = mm_per_motor_rev * INCHES_PER_MM;
        const f64 hundredths_per_motor_rev = inches_per_motor_rev * HUNDREDTHS_PER_INCH;

        const f64 hundredths_per_step = hundredths_per_motor_rev / steps_per_motor_rev;
        const f64 steps_per_hundredth = steps_per_motor_rev / hundredths_per_motor_rev;

        sth_ratio = hundredths_per_step;  // steps -> hundredths
        hts_ratio = steps_per_hundredth;  // hundredths -> steps

        hpm_to_sps_ratio = (steps_per_hundredth / SECONDS_PER_MINUTE); // hpm -> sps
        sps_to_hpm_ratio = (SECONDS_PER_MINUTE / steps_per_hundredth); // sps -> hpm
        return true;
    }

    // Distance conversions (fast path: cast -> multiply -> round -> cast back)
    [[nodiscard]] i32 steps_to_hundredths(const i32 steps) const {
        return round_to_i32(static_cast<f64>(steps) * sth_ratio);
    }

    [[nodiscard]] i32 hundredths_to_steps(const i32 hundredths) const {
        return round_to_i32(static_cast<f64>(hundredths) * hts_ratio);
    }

    // Speed conversions (hundredths per minute <-> steps per second)
    [[nodiscard]] i32 sps_to_hpm(const i32 steps_per_second) const {
        return round_to_i32(static_cast<f64>(steps_per_second) * sps_to_hpm_ratio);
    }

    [[nodiscard]] i32 hpm_to_sps(const i32 hundredths_per_minute) const {
        return round_to_i32(static_cast<f64>(hundredths_per_minute) * hpm_to_sps_ratio);
    }

    // Optional f64 conversions (no rounding)
    [[nodiscard]] f64 f64_steps_to_hundredths(const f64 steps) const { return steps * sth_ratio; }
    [[nodiscard]] f64 f64_hundredths_to_steps(const f64 hundredths) const { return hundredths * hts_ratio; }
    [[nodiscard]] f64 f64_sps_to_hpm(const f64 steps_per_second) const { return steps_per_second * sps_to_hpm_ratio; }
    [[nodiscard]] f64 f64_hpm_to_sps(const f64 hundredths_per_minute) const { return hundredths_per_minute * hpm_to_sps_ratio; }

    static i32 round_to_i32(const f64 x) {
        return static_cast<i32>(std::lround(x));
    }
};

#endif