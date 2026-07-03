#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"

namespace ui {

// Single-row status strip: filled bar with vertically centered text,
// left-aligned plus an optional right-aligned field.
class InfoBar {
public:
    static constexpr int DEFAULT_H = 14;

    // One piece of the bar text with its own color (e.g. a value readout
    // tinted to match the control it reports on).
    struct Segment {
        const char* text;
        uint16_t color;
    };

    InfoBar(int x, int y, int w, int h = DEFAULT_H,
            uint16_t fg = TEXT, uint16_t bg = BG_SURFACE);

    void draw(Framebuffer& fb, const char* left, const char* right = nullptr);

    // Draw colored segments as one run of text, centered in the bar.
    void draw(Framebuffer& fb, const Segment* segments, int count);

    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }
    void setBounds(int x, int y, int w, int h) noexcept {
        x_ = x; y_ = y; w_ = w; h_ = h;
    }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }

private:
    int x_, y_, w_, h_;
    uint16_t fg_, bg_;
};

} // namespace ui
