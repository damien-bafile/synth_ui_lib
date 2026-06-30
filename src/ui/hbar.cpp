#include "hbar.h"

namespace ui {

HorizontalBar::HorizontalBar(int x, int y, int w, int h,
                             uint16_t fg, uint16_t bg)
    : fg_(fg), bg_(bg) {
    setBounds(x, y, w, h);
}

void HorizontalBar::draw(Framebuffer& fb, float fraction) {
    if (fraction < 0.0f) fraction = 0.0f;
    if (fraction > 1.0f) fraction = 1.0f;

    fb.fillRect(x_, y_, w_, h_, bg_);
    fb.drawRect(x_, y_, w_, h_, fg_);

    int fillW = static_cast<int>(w_ * fraction);
    if (fillW > 0) {
        fb.fillRect(x_ + 1, y_ + 1, fillW - 2, h_ - 2, fg_);
    }
}

void HorizontalBar::drawCenteredCents(Framebuffer& fb, int x, int y, int w, int h,
                                       float valCents, float maxCents,
                                       uint16_t fill, uint16_t bg) {
    if (maxCents <= 0.0f) maxCents = 1.0f;
    float frac = valCents / maxCents;
    if (frac < -1.0f) frac = -1.0f;
    if (frac > 1.0f) frac = 1.0f;

    fb.fillRect(x, y, w, h, bg);
    int center = x + w / 2;
    int barW = static_cast<int>((w / 2) * frac);

    if (barW > 0) {
        fb.fillRect(center, y + 1, barW, h - 2, fill);
    } else if (barW < 0) {
        fb.fillRect(center + barW, y + 1, -barW, h - 2, fill);
    }
}

bool HorizontalBar::onTouchBegan(const TouchEvent& event) {
    return contains(event.x, event.y);
}

void HorizontalBar::onDragMoved(const TouchEvent& event, int /*dx*/, int /*dy*/) {
    float frac = (float)(event.x - x()) / width();
    if (frac < 0.0f) frac = 0.0f;
    if (frac > 1.0f) frac = 1.0f;
    fraction_ = frac;
}

} // namespace ui
