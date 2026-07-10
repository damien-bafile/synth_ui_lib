#include "vu_slider.h"

namespace ui {

VUSlider::VUSlider(int x, int y, int w, int h,
                   uint16_t fg, uint16_t bg,
                   bool vertical)
    : fg_(fg), bg_(bg), vertical_(vertical)
{
    setBounds(x, y, w, h);
}

void VUSlider::draw(Framebuffer& fb, float sliderValue, float meterValue) {
    if (sliderValue < 0.0f) sliderValue = 0.0f;
    if (sliderValue > 1.0f) sliderValue = 1.0f;
    if (meterValue < 0.0f)  meterValue = 0.0f;
    if (meterValue > 1.0f)  meterValue = 1.0f;

    fb.fillRect(x_, y_, w_, h_, bg_);
    fb.drawRect(x_, y_, w_, h_, fg_);

    // VU gradient bands: green → yellow → red
    struct { float lo, hi; uint16_t color; } bands[3] = {
        {0.0f, 0.5f, ACCENT_4},
        {0.5f, 0.8f, WARN},
        {0.8f, 1.0f, HIGHLIGHT},
    };

    if (vertical_) {
        int ch = h_ - 2;
        int by = y_ + 1 + ch;

        for (int i = 0; i < 3; i++) {
            if (meterValue <= bands[i].lo) break;
            float top = meterValue < bands[i].hi ? meterValue : bands[i].hi;
            int bandH = (int)((top - bands[i].lo) * ch);
            if (bandH > 0) {
                int bandTop = by - (int)(top * ch);
                fb.fillRect(x_ + 1, bandTop, w_ - 2, bandH, bands[i].color);
            }
        }

        int thumbH = 6;
        int thumbW = w_ - 4;
        int thumbY = by - (int)(sliderValue * ch) - thumbH / 2;
        if (thumbY < y_ + 1) thumbY = y_ + 1;
        if (thumbY + thumbH > y_ + h_ - 1) thumbY = y_ + h_ - 1 - thumbH;
        fb.fillRect(x_ + 2, thumbY, thumbW, thumbH, fg_);
    } else {
        int cw = w_ - 2;

        for (int i = 0; i < 3; i++) {
            if (meterValue <= bands[i].lo) break;
            float right = meterValue < bands[i].hi ? meterValue : bands[i].hi;
            int bandW = (int)((right - bands[i].lo) * cw);
            if (bandW > 0) {
                int bandLeft = x_ + 1 + (int)(bands[i].lo * cw);
                fb.fillRect(bandLeft, y_ + 1, bandW, h_ - 2, bands[i].color);
            }
        }

        int thumbW = 4;
        int thumbH = h_ - 4;
        int thumbX = x_ + 2 + (int)(sliderValue * cw) - thumbW / 2;
        if (thumbX < x_ + 1) thumbX = x_ + 1;
        if (thumbX + thumbW > x_ + w_ - 1) thumbX = x_ + w_ - 1 - thumbW;
        fb.fillRect(thumbX, y_ + 2, thumbW, thumbH, fg_);
    }
}

void VUSlider::updateFromTouch(int px, int py) {
    float frac;
    if (vertical_) {
        frac = 1.0f - (float)(py - y()) / height();
    } else {
        frac = (float)(px - x()) / width();
    }
    if (frac < 0.0f) frac = 0.0f;
    if (frac > 1.0f) frac = 1.0f;
    value_ = frac;
    changed_ = true;
}

bool VUSlider::onTouchBegan(const TouchEvent& event) {
    if (!contains(event.x, event.y)) return false;
    updateFromTouch(event.x, event.y);
    return true;
}

void VUSlider::onDragMoved(const TouchEvent& event, int /*dx*/, int /*dy*/) {
    updateFromTouch(event.x, event.y);
}

} // namespace ui
