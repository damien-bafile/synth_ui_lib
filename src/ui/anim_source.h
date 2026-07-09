#pragma once
#include <cstdint>
#include "colors.h" // ui::clamp
#include "easing.h" // Easing, applyEasing
#include "tween.h"  // lerp, approach, Tween

namespace ui {

// Data-driven animation sources. The library stays signal-agnostic: the caller
// extracts a feature (audio RMS/peak/FFT band, video brightness/motion, MIDI
// velocity, ...) as a plain float and push()es it here. These sources smooth,
// map, and trigger animations from that stream. Nothing here parses audio or
// video — the same code drives any data. All timing uses the caller's uint32_t
// millisecond clock, exactly like Tween/Timeline.

// ---- range mapping (glue) ----
//
// Remap x from [inLo,inHi] to [outLo,outHi] through an easing curve; the
// normalized position is clamped to [0,1] first so out-of-range inputs saturate
// instead of extrapolating. A zero-width input range returns outLo. Turns a raw
// feature (dB, 0..255 brightness) into a UI value or a color-lerp t.
inline float mapRange(float x, float inLo, float inHi, float outLo, float outHi,
                      Easing easing = Easing::LINEAR) noexcept {
    if (inHi == inLo) return outLo;
    const float t = clamp((x - inLo) / (inHi - inLo), 0.0f, 1.0f);
    return lerp(outLo, outHi, applyEasing(easing, t));
}

// ---- continuous: EnvelopeFollower ----
//
// Attack/release ballistics over a pushed level: chases the input quickly on
// the way up and slowly on the way down (VU/meter feel). Built on approach(),
// so rates are "fraction toward target per second" — larger is snappier, and a
// very large attack rate yields instant peak tracking (approach caps its step
// at 1.0). Feed one value per block/frame with a real timestamp; for
// sub-millisecond audio-rate data, accumulate peak/RMS yourself and push per
// block (this does not buffer audio).
struct EnvelopeFollower {
    void configure(float attackPerSec, float releasePerSec) noexcept {
        attack_ = attackPerSec;
        release_ = releasePerSec;
    }

    float push(float input, uint32_t nowMs) noexcept {
        if (!started_) { // snap on first sample — no spurious ramp from 0
            value_ = input;
            lastMs_ = nowMs;
            started_ = true;
            return value_;
        }
        const uint32_t dt = nowMs - lastMs_;
        lastMs_ = nowMs;
        const float rate = input > value_ ? attack_ : release_;
        value_ = approach(value_, input, rate, dt);
        return value_;
    }

    float value() const noexcept { return value_; }

    void reset(float v = 0.0f) noexcept {
        value_ = v;
        started_ = false;
    }

private:
    float value_ = 0.0f;
    float attack_ = 1000.0f; // near-instant by default
    float release_ = 6.0f;
    uint32_t lastMs_ = 0;
    bool started_ = false;
};

// ---- event: Trigger ----
//
// Threshold/rising-edge detector that fires a bound Tween pulse. Answers "what
// launches a discrete animation": a beat/onset/gate crossing, or a manual
// event via fire(). Hysteresis (arm clears on fire, re-arms only once the
// signal drops below offLevel) plus a refractory window prevent machine-gun
// retriggers on a noisy signal.
struct Trigger {
    void configure(float onLevel, float offLevel, float from, float to,
                   uint32_t durationMs, Easing easing = Easing::CUBIC_OUT,
                   uint32_t refractoryMs = 0) noexcept {
        onLevel_ = onLevel;
        offLevel_ = offLevel;
        from_ = from;
        to_ = to;
        durationMs_ = durationMs;
        easing_ = easing;
        refractoryMs_ = refractoryMs;
    }

    // Feed the signal each frame/block. Returns true only on the frame it fires.
    bool process(float signal, uint32_t nowMs) noexcept {
        bool fired = false;
        if (armed_ && signal >= onLevel_ &&
            (!hasFired_ || nowMs - lastFireMs_ >= refractoryMs_)) {
            fire(nowMs);
            armed_ = false;
            fired = true;
        } else if (!armed_ && signal <= offLevel_) {
            armed_ = true; // hysteresis re-arm
        }
        return fired;
    }

    // Launch the pulse directly, bypassing the threshold (MIDI note-on, tap,
    // scene change — any external event). Does not touch arm/refractory state.
    void fire(uint32_t nowMs) noexcept {
        pulse_.start(from_, to_, durationMs_, easing_, nowMs);
        lastFireMs_ = nowMs;
        hasFired_ = true;
    }

    float value(uint32_t nowMs) const noexcept { return pulse_.value(nowMs); }
    bool active(uint32_t nowMs) const noexcept { return pulse_.active(nowMs); }

private:
    Tween pulse_;
    float onLevel_ = 0.7f;
    float offLevel_ = 0.5f;
    float from_ = 1.0f;
    float to_ = 0.0f;
    uint32_t durationMs_ = 200;
    uint32_t refractoryMs_ = 0;
    uint32_t lastFireMs_ = 0;
    Easing easing_ = Easing::CUBIC_OUT;
    bool armed_ = true;
    bool hasFired_ = false;
};

// ---- multi-channel: FollowerBank ----
//
// Fixed-size, heap-free bank of EnvelopeFollowers. Drives the array-taking
// widgets directly: push an FFT's band energies (audio) or a region grid's
// brightness (video), then read() into a MeterArray/VoiceMeter levels buffer.
template <int N>
struct FollowerBank {
    static constexpr int SIZE = N;

    void configure(float attackPerSec, float releasePerSec) noexcept {
        for (int i = 0; i < N; ++i) ch_[i].configure(attackPerSec, releasePerSec);
    }

    void push(const float* inputs, int count, uint32_t nowMs) noexcept {
        const int n = count < N ? count : N;
        for (int i = 0; i < n; ++i) ch_[i].push(inputs[i], nowMs);
    }

    float value(int i) const noexcept {
        return (i >= 0 && i < N) ? ch_[i].value() : 0.0f;
    }

    void read(float* out, int count) const noexcept {
        const int n = count < N ? count : N;
        for (int i = 0; i < n; ++i) out[i] = ch_[i].value();
    }

    void reset(float v = 0.0f) noexcept {
        for (int i = 0; i < N; ++i) ch_[i].reset(v);
    }

private:
    EnvelopeFollower ch_[N];
};

} // namespace ui
