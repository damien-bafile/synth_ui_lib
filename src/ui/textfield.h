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

    void setPosition(int x, int y) { x_ = x; y_ = y; }
    void setSize(int w, int h) { w_ = w; h_ = h; }
    void setColors(uint16_t fg, uint16_t bg) { fg_ = fg; bg_ = bg; }

    int getX() const { return x_; }
    int getY() const { return y_; }
    int getWidth() const { return w_; }
    int getHeight() const { return h_; }

private:
    int x_, y_, w_, h_;
    uint16_t fg_, bg_;
};

} // namespace ui
