#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"

namespace ui {

// Horizontal or vertical rule (1px by default).
class Divider {
public:
    Divider(int x, int y, int len, bool vertical = false,
            uint16_t color = GRAY_DARK, int thickness = 1)
        : x_(x), y_(y), len_(len), thickness_(thickness),
          vertical_(vertical), color_(color) {}

    void draw(Framebuffer& fb) const {
        if (vertical_) fb.fillRect(x_, y_, thickness_, len_, color_);
        else           fb.fillRect(x_, y_, len_, thickness_, color_);
    }

    void setBounds(int x, int y, int len) noexcept { x_ = x; y_ = y; len_ = len; }
    void setColor(uint16_t c) noexcept { color_ = c; }

private:
    int x_, y_, len_, thickness_;
    bool vertical_;
    uint16_t color_;
};

} // namespace ui
