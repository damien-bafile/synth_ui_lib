// Cross-library integration demo: drive synth_ui_lib's animation sources from
// features extracted by synth_dsp_lib. This is the end-to-end pattern the two
// libraries are designed for — DSP reduces raw audio to numbers, the UI turns
// those numbers into motion. Neither library depends on the other; only this
// example (deliberately) pulls in both.
//
// It needs BOTH repos checked out side by side. Compile ad hoc:
//
//   c++ -std=c++14 -Wall -Wextra -Wpedantic -Isrc -I../synth_dsp_lib/src
//       examples/audio_reactive_demo.cpp
//       src/ui/anim.cpp src/ui/colors.cpp src/ui/framebuffer.cpp
//       src/ui/meter_array.cpp
//       ../synth_dsp_lib/src/dsp/fft.cpp ../synth_dsp_lib/src/dsp/window.cpp
//       ../synth_dsp_lib/src/dsp/bands.cpp ../synth_dsp_lib/src/dsp/onset.cpp
//       -o /tmp/audio_reactive_demo
//   (all on one command line) then run /tmp/audio_reactive_demo
//
// No display required: it renders into an in-memory framebuffer and prints a
// per-frame trace of the pipeline, asserting the UI actually reacted.
#include "ui/anim_source.h" // EnvelopeFollower, Trigger, FollowerBank, lerpColor
#include "ui/colors.h"
#include "ui/framebuffer.h"
#include "ui/meter_array.h"

#include "dsp/dsp.h" // rms, RealSpectrum, logBands, OnsetDetector

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

namespace {

constexpr int kFs = 48000;
constexpr int kBlock = 1024; // one FFT frame per audio block (~21 ms)
constexpr int kFrames = 32;
constexpr int kBands = 8;

constexpr int kScreenW = 320;
constexpr int kScreenH = 240;

// Synthesize one audio block for frame f: a tone (with a couple of harmonics
// for spectral spread) whose amplitude swells over the run, plus a broadband
// click every 8th frame to create an onset transient.
void synthBlock(int f, float* out) {
    const float swell = static_cast<float>(f) / static_cast<float>(kFrames - 1);
    const float amp = 0.1f + 0.85f * swell;
    const float f0 = 440.0f;
    static uint32_t rng = 0x1234567u;
    const bool transient = (f % 8) == 4 && f > 0;
    for (int i = 0; i < kBlock; ++i) {
        const float t = static_cast<float>(f * kBlock + i) / kFs;
        float s = amp * (std::sin(2.0f * static_cast<float>(M_PI) * f0 * t) +
                         0.5f * std::sin(2.0f * static_cast<float>(M_PI) * 2 * f0 * t) +
                         0.25f * std::sin(2.0f * static_cast<float>(M_PI) * 5 * f0 * t));
        if (transient && i < 64) { // short noisy click at the block start
            rng = rng * 1664525u + 1013904223u;
            const float n = (static_cast<float>(rng >> 8) / 16777216.0f) * 2.0f - 1.0f;
            s += 0.9f * n;
        }
        out[i] = s;
    }
}

// Count framebuffer pixels that differ from the cleared background — a cheap
// proxy for "something was actually drawn".
long litPixels(const uint8_t* buf, uint8_t ref0, uint8_t ref1) {
    long n = 0;
    for (int p = 0; p < kScreenW * kScreenH; ++p) {
        if (buf[2 * p] != ref0 || buf[2 * p + 1] != ref1) ++n;
    }
    return n;
}

} // namespace

int main() {
    using namespace ui;

    // --- synth_dsp_lib: feature extractors ---
    dsp::RealSpectrum spec(kBlock);
    dsp::OnsetDetector onset(spec.bins());

    // --- synth_ui_lib: animation sources fed by those features ---
    EnvelopeFollower vol;
    vol.configure(/*attackPerSec=*/1000.0f, /*releasePerSec=*/6.0f);

    FollowerBank<kBands> spectrumBank;
    spectrumBank.configure(/*attack=*/1000.0f, /*release=*/9.0f);

    Trigger beat;
    // pulse 1→0 over 90 ms on a normalized onset crossing, 150 ms refractory
    // (shorter than the gap between beats, so each flash reads as distinct)
    beat.configure(/*on=*/0.5f, /*off=*/0.3f, /*from=*/1.0f, /*to=*/0.0f,
                   /*durationMs=*/90, Easing::CUBIC_OUT, /*refractory=*/150);

    // --- UI render target + a meter for the spectrum bands ---
    static uint8_t fbData[kScreenW * kScreenH * 2];
    Framebuffer fb(fbData, kScreenW, kScreenH);
    MeterArray meters(20, 20, 280, 180, kBands, MeterArray::MeterStyle::VERTICAL,
                      ACCENT_1, BG_DARK);

    float block[kBlock];
    float bands[kBands];
    float levels[kBands];

    // Onset strength has a positive floor while the signal swells, so threshold
    // it adaptively: a beat is flux rising *above* a moving average of itself.
    // This is the caller's job — the UI Trigger just wants a clean 0..1 signal.
    float onsetEma = 0.0f;
    float excessPeak = 1e-6f; // normalizer for the above-average excess
    float maxLevel = 0.0f;
    int beatFires = 0;
    long lastLit = 0;

    std::printf("frame   rms   level  onset*  beat  band0 band3 band7  lit\n");
    for (int f = 0; f < kFrames; ++f) {
        const uint32_t now = static_cast<uint32_t>(f) * 21; // ~21 ms per block

        // DSP: raw audio -> features
        synthBlock(f, block);
        const float rms = dsp::rms(block, kBlock);
        spec.compute(block);
        dsp::logBands(spec.magnitude(), spec.bins(), bands, kBands);
        const float onsetRaw = onset.process(spec.magnitude());
        const float excess = onsetRaw - onsetEma;       // spike above the floor
        onsetEma = 0.80f * onsetEma + 0.20f * onsetRaw; // update floor after
        const float pos = excess > 0.0f ? excess : 0.0f;
        if (pos > excessPeak) excessPeak = pos;
        const float onsetN = excessPeak > 0.0f ? pos / excessPeak : 0.0f;

        // UI: features -> animated values
        const float level = vol.push(rms, now);
        spectrumBank.push(bands, kBands, now);
        spectrumBank.read(levels, kBands);
        if (beat.process(onsetN, now)) ++beatFires;

        // Render: meter shows the smoothed spectrum; background flashes on beat.
        const uint16_t flashBg = lerpColor(BG_DARK, ACCENT_2, beat.value(now));
        fb.fillScreen(flashBg);
        const uint8_t ref0 = fbData[0], ref1 = fbData[1];
        meters.draw(fb, levels, nullptr, false);
        lastLit = litPixels(fbData, ref0, ref1);

        if (level > maxLevel) maxLevel = level;
        std::printf("%4d  %5.2f  %5.2f  %5.2f  %4s  %5.2f %5.2f %5.2f  %ld\n", f,
                    rms, level, onsetN, beat.active(now) ? "on" : "-", levels[0],
                    levels[3], levels[7], lastLit);
    }

    // The pipeline is "working" if the level tracked the swell, the meter drew
    // something, and at least one transient fired the beat trigger.
    bool ok = true;
    if (maxLevel < 0.3f) { std::printf("FAIL: level never rose with the swell\n"); ok = false; }
    if (beatFires < 1) { std::printf("FAIL: no beat trigger fired on transients\n"); ok = false; }
    if (lastLit <= 0) { std::printf("FAIL: meter drew nothing into the framebuffer\n"); ok = false; }

    if (ok) {
        std::printf("\nOK: DSP features drove the UI (peak level %.2f, %d beat flashes)\n",
                    maxLevel, beatFires);
        return 0;
    }
    return EXIT_FAILURE;
}
