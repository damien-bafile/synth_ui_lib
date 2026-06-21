#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"

namespace ui {

class HorizontalBar {
public:
    HorizontalBar(int x, int y, int w, int h,
                  uint16_t fg = ACCENT_1, uint16_t bg = BG_DARK);

    void draw(Framebuffer& fb, float fraction);

    static void drawCenteredCents(Framebuffer& fb, int x, int y, int w, int h,
                                  float valCents, float maxCents,
                                  uint16_t fill, uint16_t bg);

    void setPosition(int x, int y) noexcept { x_ = x; y_ = y; }
    void setSize(int w, int h) noexcept { w_ = w; h_ = h; }
    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }

private:
    int x_, y_, w_, h_;
    uint16_t fg_, bg_;
};

} // namespace ui
