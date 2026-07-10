#include "page_indicator.h"
#include <cstdio>
#include <cstring>

namespace ui {

static constexpr int FONT_STEP = 6;

PageIndicator::PageIndicator(int x, int y, int w,
                             uint16_t fg, uint16_t bg)
    : fg_(fg), bg_(bg)
{
    setBounds(x, y, w, 14);
}

void PageIndicator::draw(Framebuffer& fb, int page, int count) {
    char mid[32];
    snprintf(mid, sizeof(mid), " PG %d/%d ", page + 1, count);

    int bw = FONT_STEP;
    int mw = std::strlen(mid) * FONT_STEP;
    int total = bw + mw + bw;

    int tx = x_ + (w_ - total) / 2;
    int ty = y_;

    prev_x0_ = tx;
    prev_x1_ = tx + bw;
    next_x0_ = tx + bw + mw;
    next_x1_ = next_x0_ + bw;

    fb.drawText(tx, ty, "<", TEXT, bg_);
    fb.drawText(tx + bw, ty, mid, fg_, bg_);
    fb.drawText(tx + bw + mw, ty, ">", TEXT, bg_);
}

void PageIndicator::onTap(const TouchEvent& e) {
    if (e.x >= prev_x0_ && e.x < prev_x1_ &&
        e.y >= y_ && e.y < y_ + h_) {
        delta_ = -1;
        return;
    }
    if (e.x >= next_x0_ && e.x < next_x1_ &&
        e.y >= y_ && e.y < y_ + h_) {
        delta_ = 1;
        return;
    }
}

} // namespace ui
