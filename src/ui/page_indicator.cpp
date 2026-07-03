#include "page_indicator.h"
#include <cstdio>

namespace ui {

void PageIndicator::draw(Framebuffer& fb, int page, int count) const {
    char buf[32];
    snprintf(buf, sizeof(buf), "< PG %d/%d >", page + 1, count);
    int tw = Framebuffer::textWidth(buf);
    fb.drawText(x_ + (w_ - tw) / 2, y_, buf, fg_, bg_);
}

} // namespace ui
