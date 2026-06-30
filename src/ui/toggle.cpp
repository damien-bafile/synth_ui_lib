#include "toggle.h"
#include "font.h"
#include <cstring>

namespace ui {

Toggle::Toggle(int x, int y, int w, int h,
               uint16_t fg, uint16_t bg)
    : fg_(fg), bg_(bg) {
    setBounds(x, y, w, h);
}

void Toggle::draw(Framebuffer& fb, bool on) {
    fb.fillRect(x_, y_, w_, h_, bg_);
    fb.drawRect(x_, y_, w_, h_, fg_);

    int knobW = (w_ / 2) - 2;
    if (knobW < 4) knobW = 4;
    int knobH = h_ - 4;
    if (knobH < 4) knobH = 4;

    if (on) {
        fb.fillRect(x_ + w_ - knobW - 2, y_ + 2, knobW, knobH, fg_);
    } else {
        fb.fillRect(x_ + 2, y_ + 2, knobW, knobH, fg_);
    }
}

void Toggle::drawWithLabel(Framebuffer& fb, bool on, const char* label,
                           uint16_t fg, uint16_t bg) {
    draw(fb, on);
    int tw = static_cast<int>(std::strlen(label)) * FONT_W;
    int tx = x_ - tw - 4;
    int ty = y_ + (h_ - FONT_H) / 2;
    if (tx < 0) tx = x_ + w_ + 4;
    fb.drawText(tx, ty, label, fg, bg);
}

void Toggle::onTap(const TouchEvent&) {
    wasTapped_ = true;
}

} // namespace ui
