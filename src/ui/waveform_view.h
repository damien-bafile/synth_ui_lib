#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"

namespace ui {

// Sample-data display: one min/max column per pixel, computed once per
// buffer and cached (rebuilt when the data pointer or frame count
// changes). Shows a centered placeholder text while no data is set.
// drawMarker() overlays a vertical playhead at a normalized position.
class WaveformView {
public:
    static constexpr int MAX_COLS = 320;

    WaveformView(int x, int y, int w, int h,
                 uint16_t bg = BG_SURFACE, uint16_t frame = GRAY_DARK)
        : x_(x), y_(y), w_(w), h_(h), bg_(bg), frame_(frame) {}

    void draw(Framebuffer& fb, const int16_t* data, uint32_t frames,
              uint16_t color, const char* emptyText);

    void drawMarker(Framebuffer& fb, float norm, uint16_t color) const;

    void setColors(uint16_t bg, uint16_t frame) noexcept {
        bg_ = bg; frame_ = frame;
    }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }

private:
    int cols() const noexcept {
        int c = w_ - 4;
        return c > MAX_COLS ? MAX_COLS : c;
    }
    void rebuildCache(const int16_t* data, uint32_t frames);

    int x_, y_, w_, h_;
    uint16_t bg_, frame_;
    int8_t colMin_[MAX_COLS] = {};
    int8_t colMax_[MAX_COLS] = {};
    const int16_t* cachedData_ = nullptr;
    uint32_t cachedFrames_ = 0;
};

} // namespace ui
