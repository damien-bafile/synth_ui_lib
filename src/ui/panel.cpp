#include "panel.h"

namespace ui {

Panel::Panel(int x, int y, int w, int h, const char* title, uint16_t titleColor)
    : x_(x), y_(y), w_(w), h_(h), title_(title), titleColor_(titleColor) {}

void Panel::draw(Framebuffer& fb) {
    fb.drawRect(x_, y_, w_, h_, BG_MID);
    fb.fillRect(x_ + 1, y_ + 1, w_ - 2, TITLE_H, BG_SURFACE);
    fb.drawText(x_ + 6, y_ + 3, title_, titleColor_, BG_SURFACE);
}

} // namespace ui
