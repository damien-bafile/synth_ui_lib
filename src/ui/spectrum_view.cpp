#include "spectrum_view.h"

namespace ui {

void SpectrumView::resetPeaks() noexcept {
    for (int i = 0; i < MAX_BINS; i++) peaks_[i] = 0.0f;
}

void SpectrumView::draw(Framebuffer& fb, const float* magnitudes, int count) {
    fb.fillRect(x_, y_, w_, h_, bg_);

    if (count > MAX_BINS) count = MAX_BINS;
    if (count < 1 || !magnitudes) return;

    int binW = w_ / count;
    if (binW < 1) binW = 1;
    int barW = binW > 1 ? binW - 1 : 1;

    for (int i = 0; i < count; i++) {
        float mag = magnitudes[i];
        if (mag < 0.0f) mag = 0.0f;
        if (mag > 1.0f) mag = 1.0f;

        if (mag >= peaks_[i]) {
            peaks_[i] = mag;
        } else {
            peaks_[i] *= 0.9f;
        }

        int barH = static_cast<int>(mag * h_);
        int bx = x_ + i * binW;
        if (barH > 0) {
            fb.fillRect(bx, y_ + h_ - barH, barW, barH, fg_);
        }

        int peakY = y_ + h_ - static_cast<int>(peaks_[i] * h_);
        if (peakY < y_) peakY = y_;
        if (peakY < y_ + h_) {
            fb.fillRect(bx, peakY, barW, 1, fg_);
        }
    }
}

} // namespace ui
