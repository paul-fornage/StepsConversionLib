//
// Created by pfornage on 12/16/25.
//

#ifndef RUNTIME_STEPS_CONVERSIONS_H
#define RUNTIME_STEPS_CONVERSIONS_H

#include <cmath>
#include <RustNumberTypes.h>
#include <StepsConversionCommon.h>


struct RuntimeStepsConversion {

    static constexpr f64 HUNDREDTHS_PER_INCH = 100.0;
    static constexpr f64 SECONDS_PER_MINUTE = 60.0;


    // Precomputed factors (caller must keep fresh by calling recalculate())
    f64 sth_ratio = 0.0;         // steps -> hundredths
    f64 hts_ratio = 0.0;         // hundredths -> steps
    f64 hpm_to_sps_ratio = 0.0;  // hundredths/min -> steps/sec
    f64 sps_to_hpm_ratio = 0.0;  // steps/sec -> hundredths/min

    RuntimeStepsConversion() = delete;

    explicit RuntimeStepsConversion(const f64 sth_ratio_) {
        recalculate(sth_ratio_);
    }

    // Contract: you (the caller) may update any combination of public inputs,
    // then MUST call recalculate() before using conversions.
    void recalculate(const f64 sth_ratio_) {
        sth_ratio = sth_ratio_;  // steps -> hundredths
        hts_ratio = 1.0/sth_ratio;  // hundredths -> steps

        hpm_to_sps_ratio = (hts_ratio / SECONDS_PER_MINUTE); // hpm -> sps
        sps_to_hpm_ratio = (sth_ratio * SECONDS_PER_MINUTE); // sps -> hpm
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