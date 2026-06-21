#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "touch.h"
#include "colors.h"

namespace ui {

class Toggle {
public:
    static constexpr int DEFAULT_W = 30;
    static constexpr int DEFAULT_H = 16;

    Toggle(int x, int y, int w = DEFAULT_W, int h = DEFAULT_H,
           uint16_t fg = ACCENT_2, uint16_t bg = BG_DARK);

    void draw(Framebuffer& fb, bool on);
    void drawWithLabel(Framebuffer& fb, bool on, const char* label,
                       uint16_t fg, uint16_t bg);
    bool handleTouch(const TouchState& touch);

    void setPosition(int x, int y) noexcept { x_ = x; y_ = y; }
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
