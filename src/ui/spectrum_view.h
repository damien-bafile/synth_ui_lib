#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"

namespace ui {

// Spectrum/FFT magnitude display: one vertical bar per bin, plus a
// per-bin peak-hold line that decays over successive draw() calls (same
// technique as VUSlider's peak indicator). Draw-only, not touch-interactive
// -- the display sibling of WaveformView for frequency-domain data.
class SpectrumView {
public:
    static constexpr int MAX_BINS = 32;

    SpectrumView(int x, int y, int w, int h,
                 uint16_t fg = ACCENT_1, uint16_t bg = BG_DARK) noexcept
        : x_(x), y_(y), w_(w), h_(h), fg_(fg), bg_(bg) {}

    // magnitudes are expected normalized to [0,1]; count is clamped to
    // MAX_BINS. Peak-hold state persists across calls and decays when a
    // bin's magnitude drops below its held peak.
    void draw(Framebuffer& fb, const float* magnitudes, int count);

    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }
    void resetPeaks() noexcept;

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }

private:
    int x_, y_, w_, h_;
    uint16_t fg_, bg_;
    float peaks_[MAX_BINS] = {};
};

} // namespace ui
