#include "toggle.h"

namespace ui {

Toggle::Toggle(int x, int y, int w, int h, uint16_t fg, uint16_t bg)
    : x_(x), y_(y), w_(w), h_(h), fg_(fg), bg_(bg) {}

void Toggle::draw(Framebuffer& fb, bool on) {
    int pad = 2;
    int trackH = h_ - pad * 2;
    int trackW = w_ - pad * 2;
    if (trackH < 2 || trackW < 2) return;
    int cy = y_ + h_ / 2;
    int thumbR = trackH / 2;

    if (on) {
        fb.fillRect(x_, y_, w_, h_, fg_);
        int tx = x_ + w_ - pad - thumbR;
        fb.fillCircle(tx, cy, thumbR, bg_);
    } else {
        fb.fillRect(x_, y_, w_, h_, bg_);
        fb.drawRect(x_, y_, w_, h_, fg_);
        int tx = x_ + pad + thumbR;
        fb.fillCircle(tx, cy, thumbR, fg_);
    }
}

void Toggle::drawWithLabel(Framebuffer& fb, bool on, const char* label,
                           uint16_t fg, uint16_t bg) {
    draw(fb, on);
    if (label) {
        int ly = y_ + (h_ - 7) / 2;
        fb.drawText(x_ + w_ + 4, ly, label, fg, bg);
    }
}

bool Toggle::handleTouch(const TouchState& touch) {
    return touch.pressed &&
           touch.x >= x_ && touch.x < x_ + w_ &&
           touch.y >= y_ && touch.y < y_ + h_;
}

} // namespace ui
