#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"

namespace ui {

// Positioned text. Horizontal styles align within the label width and
// center vertically within the height (h = 0 draws a bare text row at y).
// VERTICAL stacks one character per row (10px pitch), centered in h —
// used for rotated-style axis labels.
class Label {
public:
    enum class Align : uint8_t { LEFT, CENTER, RIGHT, VERTICAL };

    Label(int x, int y, int w = 0, int h = 0,
          uint16_t fg = TEXT, uint16_t bg = BG_DARK,
          Align align = Align::LEFT)
        : x_(x), y_(y), w_(w), h_(h), fg_(fg), bg_(bg), align_(align) {}

    void draw(Framebuffer& fb, const char* text) const;

    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }
    void setBounds(int x, int y, int w, int h) noexcept {
        x_ = x; y_ = y; w_ = w; h_ = h;
    }
    void setAlign(Align a) noexcept { align_ = a; }

private:
    int x_, y_, w_, h_;
    uint16_t fg_, bg_;
    Align align_;
};

} // namespace ui
