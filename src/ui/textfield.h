#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "touch.h"
#include "colors.h"

namespace ui {

class TextField {
public:
    TextField(int x, int y, int w, int h,
              uint16_t fg = TEXT, uint16_t bg = BG_DARK);

    void draw(Framebuffer& fb, const char* text, int cursorPos, bool focused);
    bool handleTouch(const TouchState& touch);

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
