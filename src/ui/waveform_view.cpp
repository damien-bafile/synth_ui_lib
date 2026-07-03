#include "waveform_view.h"
#include "font.h"

namespace ui {

void WaveformView::rebuildCache(const int16_t* data, uint32_t frames) {
    int n = cols();
    for (int c = 0; c < n; c++) {
        uint32_t s0 = (uint32_t)((uint64_t)c * frames / n);
        uint32_t s1 = (uint32_t)((uint64_t)(c + 1) * frames / n);
        if (s1 <= s0) s1 = s0 + 1;
        if (s1 > frames) s1 = frames;
        uint32_t stride = (s1 - s0) / 24 + 1;
        int16_t mn = 32767, mx = -32768;
        for (uint32_t s = s0; s < s1; s += stride) {
            int16_t v = data[s];
            if (v < mn) mn = v;
            if (v > mx) mx = v;
        }
        colMin_[c] = (int8_t)(mn >> 8);
        colMax_[c] = (int8_t)(mx >> 8);
    }
    cachedData_ = data;
    cachedFrames_ = frames;
}

void WaveformView::draw(Framebuffer& fb, const int16_t* data, uint32_t frames,
                        uint16_t color, const char* emptyText) {
    fb.fillRect(x_, y_, w_, h_, bg_);
    fb.drawRect(x_, y_, w_, h_, frame_);
    int cy = y_ + h_ / 2;
    fb.fillRect(x_ + 2, cy, w_ - 4, 1, frame_);

    if (!data || frames == 0) {
        if (emptyText) {
            int tw = Framebuffer::textWidth(emptyText);
            fb.drawText(x_ + (w_ - tw) / 2, cy - FONT_H / 2, emptyText,
                        TEXT_DIM, bg_);
        }
        cachedData_ = nullptr;
        return;
    }

    if (cachedData_ != data || cachedFrames_ != frames)
        rebuildCache(data, frames);

    int half = (h_ - 6) / 2;
    int n = cols();
    for (int c = 0; c < n; c++) {
        int y0 = cy - (colMax_[c] * half) / 127;
        int y1 = cy - (colMin_[c] * half) / 127;
        int h = y1 - y0 + 1;
        if (h < 1) h = 1;
        fb.fillRect(x_ + 2 + c, y0, 1, h, color);
    }
}

void WaveformView::drawMarker(Framebuffer& fb, float norm, uint16_t color) const {
    if (norm < 0.0f) norm = 0.0f;
    if (norm > 1.0f) norm = 1.0f;
    int px = x_ + 2 + (int)(norm * (cols() - 1));
    fb.fillRect(px, y_ + 2, 1, h_ - 4, color);
}

} // namespace ui
