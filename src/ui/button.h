#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "touch.h"
#include "colors.h"

namespace ui {

class Button {
public:
    Button(int x, int y, int w, int h, const char* label,
           uint16_t fg = TEXT, uint16_t bg = BG_SURFACE);

    void draw(Framebuffer& fb, bool pressed);
    bool handleTouch(const TouchState& touch);

    void setLabel(const char* label) noexcept { label_ = label; }
    void setPosition(int x, int y) noexcept { x_ = x; y_ = y; }
    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }
    const char* getLabel() const noexcept { return label_; }

private:
    int x_, y_, w_, h_;
    const char* label_;
    uint16_t fg_, bg_;
};

} // namespace ui
