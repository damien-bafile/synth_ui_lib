#include "hbar.h"
#include "rect.h"

namespace ui {

HorizontalBar::HorizontalBar(int x, int y, int w, int h, uint16_t fg, uint16_t bg)
    : x_(x), y_(y), w_(w), h_(h), fg_(fg), bg_(bg) {}

bool HorizontalBar::handleTouch(const TouchState& touch, float& outFraction) const {
    if (!touch.pressed) return false;
    if (!Rect{x_, y_, w_, h_}.contains(touch.x, touch.y)) return false;
    outFraction = (float)(touch.x - x_) / (float)w_;
    if (outFraction < 0.0f) outFraction = 0.0f;
    if (outFraction > 1.0f) outFraction = 1.0f;
    return true;
}

void HorizontalBar::draw(Framebuffer& fb, float fraction) {
    int fw = static_cast<int>(w_ * fraction);
    if (fw < 0) fw = 0;
    if (fw > w_) fw = w_;
    if (fw > 0) fb.fillRect(x_, y_, fw, h_, fg_);
    if (fw < w_) fb.fillRect(x_ + fw, y_, w_ - fw, h_, bg_);
}

void HorizontalBar::drawCenteredCents(Framebuffer& fb, int x, int y, int w, int h,
                                      float valCents, float maxCents,
                                      uint16_t fill, uint16_t bg) {
    fb.fillRect(x, y, w, h, bg);

    int half = w / 2;
    if (half < 1) return;
    if (maxCents <= 0.0f) return;

    float mag = valCents;
    if (mag < -maxCents) mag = -maxCents;
    if (mag >  maxCents) mag =  maxCents;

    if (mag < 0.0f) {
        int fw = static_cast<int>(((-mag) / maxCents) * half);
        if (fw > half) fw = half;
        if (fw > 0) fb.fillRect(x + half - fw, y, fw, h, fill);
    } else if (mag > 0.0f) {
        int fw = static_cast<int>((mag / maxCents) * half);
        if (fw > half) fw = half;
        if (fw > 0) fb.fillRect(x + half, y, fw, h, fill);
    }

    fb.fillRect(x + half, y, 1, h, fill);
}

} // namespace ui
