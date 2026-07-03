#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"

namespace ui {

// "< PG p/c >" readout centered within its width.
class PageIndicator {
public:
    PageIndicator(int x, int y, int w,
                  uint16_t fg = TEXT_DIM, uint16_t bg = BG_DARK)
        : x_(x), y_(y), w_(w), fg_(fg), bg_(bg) {}

    void draw(Framebuffer& fb, int page, int count) const;

    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }

private:
    int x_, y_, w_;
    uint16_t fg_, bg_;
};

} // namespace ui
