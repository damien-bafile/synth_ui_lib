#include "button.h"

namespace ui {

Button::Button(int x, int y, int w, int h, const char* label,
               uint16_t fg, uint16_t bg)
    : x_(x), y_(y), w_(w), h_(h), label_(label), fg_(fg), bg_(bg) {}

void Button::draw(Framebuffer& fb, bool pressed) {
    if (pressed) {
        fb.fillRect(x_, y_, w_, h_, fg_);
        fb.drawRect(x_, y_, w_, h_, bg_);
        if (label_) {
            int tw = Framebuffer::textWidth(label_);
            int tx = x_ + (w_ - tw) / 2;
            int ty = y_ + (h_ - 7) / 2;
            fb.drawText(tx, ty, label_, bg_, fg_);
        }
    } else {
        fb.fillRect(x_, y_, w_, h_, bg_);
        fb.drawRect(x_, y_, w_, h_, fg_);
        if (label_) {
            int tw = Framebuffer::textWidth(label_);
            int tx = x_ + (w_ - tw) / 2;
            int ty = y_ + (h_ - 7) / 2;
            fb.drawText(tx, ty, label_, fg_, bg_);
        }
    }
}

bool Button::handleTouch(const TouchState& touch) {
    return touch.pressed &&
           touch.x >= x_ && touch.x < x_ + w_ &&
           touch.y >= y_ && touch.y < y_ + h_;
}

} // namespace ui
