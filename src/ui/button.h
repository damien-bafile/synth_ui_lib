#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "touch.h"
#include "colors.h"
#include <cstring>

namespace ui {

class Button {
public:
    Button(int x, int y, int w, int h, const char* label,
           uint16_t fg = TEXT, uint16_t bg = BG_SURFACE);

    void draw(Framebuffer& fb, bool pressed);
    bool handleTouch(const TouchState& touch);

    void setLabel(const char* label) { label_ = label; }
    void setPosition(int x, int y) { x_ = x; y_ = y; }
    void setColors(uint16_t fg, uint16_t bg) { fg_ = fg; bg_ = bg; }

    int getX() const { return x_; }
    int getY() const { return y_; }
    int getWidth() const { return w_; }
    int getHeight() const { return h_; }
    const char* getLabel() const { return label_; }

private:
    int x_, y_, w_, h_;
    const char* label_;
    uint16_t fg_, bg_;
};

} // namespace ui
