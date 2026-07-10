#pragma once
#include <cstdint>
#include "widget.h"
#include "framebuffer.h"
#include "colors.h"

namespace ui {

// "< PG p/c >" readout with touchable < and > brackets for page nav.
// The brackets are drawn in TEXT (white); the rest in fg (TEXT_DIM).
class PageIndicator : public Widget {
public:
    PageIndicator(int x, int y, int w,
                  uint16_t fg = TEXT_DIM, uint16_t bg = BG_DARK);

    void draw(Framebuffer& fb, int page, int count);

    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }

    // Returns -1 (prev), 0 (none), or 1 (next) since last call.
    int pageDelta() noexcept { int d = delta_; delta_ = 0; return d; }

protected:
    void onTap(const TouchEvent& e) override;

private:
    uint16_t fg_, bg_;
    int delta_ = 0;
    int prev_x0_ = 0, prev_x1_ = 0;
    int next_x0_ = 0, next_x1_ = 0;
};

} // namespace ui
