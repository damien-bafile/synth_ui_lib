#include "infobar.h"
#include "font.h"

namespace ui {

InfoBar::InfoBar(int x, int y, int w, int h, uint16_t fg, uint16_t bg)
    : x_(x), y_(y), w_(w), h_(h), fg_(fg), bg_(bg) {}

void InfoBar::draw(Framebuffer& fb, const Segment* segments, int count) {
    fb.fillRect(x_, y_, w_, h_, bg_);
    int total = 0;
    for (int i = 0; i < count; i++) {
        if (segments[i].text) total += Framebuffer::textWidth(segments[i].text);
    }
    int tx = x_ + (w_ - total) / 2;
    int ty = y_ + (h_ - FONT_H) / 2;
    for (int i = 0; i < count; i++) {
        if (!segments[i].text) continue;
        fb.drawText(tx, ty, segments[i].text, segments[i].color, bg_);
        tx += Framebuffer::textWidth(segments[i].text);
    }
}

void InfoBar::draw(Framebuffer& fb, const char* left, const char* right) {
    fb.fillRect(x_, y_, w_, h_, bg_);
    int ty = y_ + (h_ - FONT_H) / 2;
    if (left) {
        fb.drawText(x_ + 4, ty, left, fg_, bg_);
    }
    if (right) {
        int tw = Framebuffer::textWidth(right);
        fb.drawText(x_ + w_ - tw - 4, ty, right, fg_, bg_);
    }
}

} // namespace ui
