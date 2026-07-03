#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"

namespace ui {

// 1px rectangular outline (row selection / highlight frame).
class Frame {
public:
    Frame(int x = 0, int y = 0, int w = 0, int h = 0,
          uint16_t color = ACCENT_2)
        : x_(x), y_(y), w_(w), h_(h), color_(color) {}

    void draw(Framebuffer& fb) const { fb.drawRect(x_, y_, w_, h_, color_); }

    void setBounds(int x, int y, int w, int h) noexcept {
        x_ = x; y_ = y; w_ = w; h_ = h;
    }
    void setColor(uint16_t c) noexcept { color_ = c; }

private:
    int x_, y_, w_, h_;
    uint16_t color_;
};

} // namespace ui
