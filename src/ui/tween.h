#pragma once
#include <cstdint>
#include "colors.h" // ui::clamp, ui::RGB565
#include "easing.h"

namespace ui {

// How a Tween behaves after it first reaches its target.
enum class LoopMode : uint8_t {
    ONCE,     // stop at `to` (done() flips true)
    LOOP,     // jump back to `from` and repeat
    PINGPONG, // reverse direction each cycle
};

// ---- stateless value helpers (header-only) ----

inline float lerp(float a, float b, float t) noexcept { return a + (b - a) * t; }

// Interpolate between two RGB565 colors in packed 5-6-5 space. Since both
// endpoints are non-negative per channel, the interpolant stays in range so a
// flat +0.5f rounding is correct. Handy for press-flash / state transitions.
inline uint16_t lerpColor(uint16_t a, uint16_t b, float t) noexcept {
    t = clamp(t, 0.0f, 1.0f);
    const int ar = (a >> 11) & 0x1F, ag = (a >> 5) & 0x3F, ab = a & 0x1F;
    const int br = (b >> 11) & 0x1F, bg = (b >> 5) & 0x3F, bb = b & 0x1F;
    const int r = static_cast<int>(ar + (br - ar) * t + 0.5f);
    const int g = static_cast<int>(ag + (bg - ag) * t + 0.5f);
    const int bl = static_cast<int>(ab + (bb - ab) * t + 0.5f);
    return static_cast<uint16_t>((r << 11) | (g << 5) | bl);
}

// One-pole smoother for continuous signals with no fixed start/end (meter
// ballistics, slider smoothing). Moves `current` a fraction of the way to
// `target` sized by elapsed time; frame-rate independent for small steps.
inline float approach(float current, float target, float ratePerSec,
                      uint32_t dtMs) noexcept {
    if (ratePerSec <= 0.0f || dtMs == 0) return current;
    float k = ratePerSec * (static_cast<float>(dtMs) * 0.001f);
    if (k > 1.0f) k = 1.0f;
    return current + (target - current) * k;
}

// ---- caller-owned one-shot interpolation ----
//
// Store a Tween next to the value it drives, call start() once, then read
// value(nowMs) every frame and feed it into a widget's draw(). All timing is
// on the same uint32_t millisecond clock passed to TouchDispatcher::dispatch().
struct Tween {
    // Begin animating from→to over durationMs, optionally after delayMs.
    void start(float from, float to, uint32_t durationMs, Easing easing,
               uint32_t nowMs, uint32_t delayMs = 0,
               LoopMode loop = LoopMode::ONCE) noexcept {
        from_ = from;
        to_ = to;
        durationMs_ = durationMs;
        easing_ = easing;
        loop_ = loop;
        startMs_ = nowMs + delayMs; // run begins after the delay
    }

    // Redirect to a new target starting from wherever we are right now,
    // reusing the current duration and easing (e.g. a slider chasing input).
    void retarget(float to, uint32_t nowMs) noexcept {
        start(value(nowMs), to, durationMs_, easing_, nowMs, 0, LoopMode::ONCE);
    }

    float value(uint32_t nowMs) const noexcept {
        if (durationMs_ == 0) return to_;
        if (nowMs <= startMs_) return from_; // not started (or still delayed)
        const uint32_t elapsed = nowMs - startMs_;
        float p;
        if (loop_ == LoopMode::ONCE) {
            if (elapsed >= durationMs_) return to_;
            p = static_cast<float>(elapsed) / static_cast<float>(durationMs_);
        } else {
            const uint32_t cycle = elapsed % durationMs_;
            p = static_cast<float>(cycle) / static_cast<float>(durationMs_);
            if (loop_ == LoopMode::PINGPONG && ((elapsed / durationMs_) & 1u)) {
                p = 1.0f - p; // reverse on odd cycles
            }
        }
        return lerp(from_, to_, applyEasing(easing_, p));
    }

    bool done(uint32_t nowMs) const noexcept {
        if (loop_ != LoopMode::ONCE) return false; // loops never finish
        if (durationMs_ == 0) return true;
        return nowMs >= startMs_ + durationMs_;
    }

    bool active(uint32_t nowMs) const noexcept {
        return durationMs_ != 0 && !done(nowMs);
    }

    float target() const noexcept { return to_; }

private:
    float from_ = 0.0f;
    float to_ = 0.0f;
    uint32_t startMs_ = 0;
    uint32_t durationMs_ = 0;
    Easing easing_ = Easing::LINEAR;
    LoopMode loop_ = LoopMode::ONCE;
};

} // namespace ui
