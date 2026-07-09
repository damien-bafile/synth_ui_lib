#pragma once
#include <cstdint>
#include "colors.h" // ui::clamp

namespace ui {

// Curated easing set — kept small on purpose (embedded flash budget). The
// polynomial curves below are constexpr and defined inline; the trig/exp
// curves (SINE/EXPO/BACK/ELASTIC/BOUNCE) need <cmath>, which is not constexpr
// in C++14, so applyEasing() dispatches to them from easing → anim.cpp.
enum class Easing : uint8_t {
    LINEAR,
    QUAD_IN,
    QUAD_OUT,
    QUAD_INOUT,
    CUBIC_IN,
    CUBIC_OUT,
    CUBIC_INOUT,
    BACK_OUT,    // slight overshoot then settle (polynomial, constexpr)
    SINE_INOUT,  // runtime (anim.cpp)
    EXPO_OUT,    // runtime (anim.cpp)
    ELASTIC_OUT, // runtime (anim.cpp)
    BOUNCE_OUT,  // runtime (anim.cpp)
};

// ---- constexpr polynomial curves. t is expected in [0,1]. ----

constexpr float easeLinear(float t) noexcept { return t; }

constexpr float easeQuadIn(float t) noexcept { return t * t; }
constexpr float easeQuadOut(float t) noexcept { return t * (2.0f - t); }
constexpr float easeQuadInOut(float t) noexcept {
    return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
}

constexpr float easeCubicIn(float t) noexcept { return t * t * t; }
constexpr float easeCubicOut(float t) noexcept {
    const float p = t - 1.0f;
    return 1.0f + p * p * p;
}
constexpr float easeCubicInOut(float t) noexcept {
    const float q = -2.0f * t + 2.0f;
    return t < 0.5f ? 4.0f * t * t * t : 1.0f - (q * q * q) * 0.5f;
}

constexpr float easeBackOut(float t) noexcept {
    const float c1 = 1.70158f;
    const float c3 = c1 + 1.0f;
    const float p = t - 1.0f;
    return 1.0f + c3 * p * p * p + c1 * p * p;
}

// Select a curve by value and evaluate it. t is clamped to [0,1] first so
// callers never have to guard the endpoints. Defined in anim.cpp because it
// reaches the non-constexpr (trig/exp) curves.
float applyEasing(Easing e, float t) noexcept;

} // namespace ui
