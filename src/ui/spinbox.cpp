#include "spinbox.h"
#include "font.h"
#include <cstdio>

namespace ui {

Spinbox::Spinbox(int x, int y, int w, int h,
                 float min, float max, float step, float value,
                 uint16_t fg, uint16_t bg) noexcept
    : min_(min), max_(max), step_(step), value_(value), fg_(fg), bg_(bg) {
    setBounds(x, y, w, h);
    setValue(value);
}

void Spinbox::setValue(float v) noexcept {
    value_ = v < min_ ? min_ : (v > max_ ? max_ : v);
}

void Spinbox::draw(Framebuffer& fb, float value) {
    fb.fillRect(x_, y_, w_, h_, bg_);

    minusX0_ = x_;
    minusX1_ = x_ + BTN_W;
    plusX0_ = x_ + w_ - BTN_W;
    plusX1_ = x_ + w_;

    fb.drawRect(minusX0_, y_, BTN_W, h_, fg_);
    fb.drawRect(plusX0_, y_, BTN_W, h_, fg_);

    int ty = y_ + (h_ - FONT_H) / 2;
    fb.drawText(minusX0_ + (BTN_W - FONT_W) / 2, ty, "-", fg_, bg_);
    fb.drawText(plusX0_ + (BTN_W - FONT_W) / 2, ty, "+", fg_, bg_);

    char buf[16];
    std::snprintf(buf, sizeof(buf), "%.2f", value);
    int tw = Framebuffer::textWidth(buf);
    int tx = x_ + (w_ - tw) / 2;
    fb.drawText(tx, ty, buf, fg_, bg_);
}

void Spinbox::onTap(const TouchEvent& event) {
    if (event.x >= minusX0_ && event.x < minusX1_ &&
        event.y >= y_ && event.y < y_ + h_) {
        setValue(value_ - step_);
        changed_ = true;
        return;
    }
    if (event.x >= plusX0_ && event.x < plusX1_ &&
        event.y >= y_ && event.y < y_ + h_) {
        setValue(value_ + step_);
        changed_ = true;
        return;
    }
}

} // namespace ui
