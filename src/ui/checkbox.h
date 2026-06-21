#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "touch.h"
#include "colors.h"

namespace ui {

class Checkbox {
public:
    static constexpr int DEFAULT_SIZE = 12;

    Checkbox(int x, int y, uint16_t fg = TEXT, uint16_t bg = BG_DARK);

    void draw(Framebuffer& fb, bool checked, const char* label);
    bool handleTouch(const TouchState& touch);

    void setPosition(int x, int y) noexcept { x_ = x; y_ = y; }
    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getSize() const noexcept { return size_; }

private:
    int x_, y_;
    int size_;
    uint16_t fg_, bg_;
};

} // namespace ui
