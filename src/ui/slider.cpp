#include "slider.h"

namespace ui {

Slider::Slider(int x, int y, int w, int h, float min, float max,
               const char* label, uint16_t fg, uint16_t bg, bool vertical)
    : min_(min), max_(max), value_(min),
      label_(label), fg_(fg), bg_(bg), vertical_(vertical) {
    setBounds(x, y, w, h);
}

void Slider::draw(Framebuffer& fb, float value) {
    if (vertical_) {
        if (label_) {
            fb.drawText(x_, y_ + h_ + 4, label_, fg_, bg_);
        }

        int thumbW = w_ + 6;
        int thumbH = 4;
        if (thumbW < 2) thumbW = 2;

        fb.fillRect(x_ + (w_ - 3) / 2, y_, 3, h_, bg_);
        fb.drawRect(x_ + (w_ - 3) / 2, y_, 3, h_, fg_);

        float range = max_ - min_;
        float frac = (range > 0.0f) ? (value - min_) / range : 0.0f;
        if (frac < 0.0f) frac = 0.0f;
        if (frac > 1.0f) frac = 1.0f;
        int fillH = static_cast<int>(h_ * frac);

        int fy = y_ + h_ - fillH;
        if (fillH > 0) {
            fb.fillRect(x_ + (w_ - 3) / 2, fy, 3, fillH, fg_);
        }

        int thumbY = fy - thumbH / 2;
        if (thumbY < y_) thumbY = y_;
        if (thumbY + thumbH > y_ + h_) thumbY = y_ + h_ - thumbH;
        int thumbX = x_ + (w_ - thumbW) / 2;
        fb.fillRect(thumbX, thumbY, thumbW, thumbH, fg_);
    } else {
        if (label_) {
            fb.drawText(x_, y_ - 10, label_, fg_, bg_);
        }

        int thumbW = 4;
        int thumbH = h_ + 6;
        if (thumbH < 2) thumbH = 2;

        fb.fillRect(x_, y_ + (h_ - 3) / 2, w_, 3, bg_);
        fb.drawRect(x_, y_ + (h_ - 3) / 2, w_, 3, fg_);

        float range = max_ - min_;
        float frac = (range > 0.0f) ? (value - min_) / range : 0.0f;
        if (frac < 0.0f) frac = 0.0f;
        if (frac > 1.0f) frac = 1.0f;
        int fillW = static_cast<int>(w_ * frac);

        if (fillW > 0) {
            fb.fillRect(x_, y_ + (h_ - 3) / 2, fillW, 3, fg_);
        }

        int thumbX = x_ + fillW - thumbW / 2;
        if (thumbX < x_) thumbX = x_;
        if (thumbX + thumbW > x_ + w_) thumbX = x_ + w_ - thumbW;
        int thumbY = y_ + (h_ - thumbH) / 2;
        fb.fillRect(thumbX, thumbY, thumbW, thumbH, fg_);
    }
}

void Slider::updateFromTouch(int px, int py) {
    float range = max_ - min_;
    if (range <= 0.0f) return;

    float frac;
    if (vertical_) {
        frac = 1.0f - (float)(py - y()) / height();
    } else {
        frac = (float)(px - x()) / width();
    }
    if (frac < 0.0f) frac = 0.0f;
    if (frac > 1.0f) frac = 1.0f;
    value_ = min_ + frac * range;
    changed_ = true;
}

bool Slider::onTouchBegan(const TouchEvent& event) {
    if (!contains(event.x, event.y)) return false;
    updateFromTouch(event.x, event.y);  // jump to touch position
    return true;
}

void Slider::onDragMoved(const TouchEvent& event, int /*dx*/, int /*dy*/) {
    updateFromTouch(event.x, event.y);
}

} // namespace ui
