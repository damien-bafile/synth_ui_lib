#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "touch.h"
#include "colors.h"

namespace ui {

class Slider {
public:
    Slider(int x, int y, int w, int h, float min, float max,
           const char* label, uint16_t fg = ACCENT_1, uint16_t bg = BG_DARK,
           bool vertical = false);

    void draw(Framebuffer& fb, float value);
    bool handleTouch(const TouchState& touch);

    void setLabel(const char* label) noexcept { label_ = label; }
    void setPosition(int x, int y) noexcept { x_ = x; y_ = y; }
    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }
    void setRange(float min, float max) noexcept { min_ = min; max_ = max; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }
    const char* getLabel() const noexcept { return label_; }
    bool isVertical() const noexcept { return vertical_; }

private:
    int x_, y_, w_, h_;
    float min_, max_;
    const char* label_;
    uint16_t fg_, bg_;
    bool vertical_;
};

} // namespace ui
