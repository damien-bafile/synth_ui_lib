#include "button.h"
#include "rect.h"
#include "font.h"

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
            int ty = y_ + (h_ - FONT_H) / 2;
            fb.drawText(tx, ty, label_, bg_, fg_);
        }
    } else {
        fb.fillRect(x_, y_, w_, h_, bg_);
        fb.drawRect(x_, y_, w_, h_, fg_);
        if (label_) {
            int tw = Framebuffer::textWidth(label_);
            int tx = x_ + (w_ - tw) / 2;
            int ty = y_ + (h_ - FONT_H) / 2;
            fb.drawText(tx, ty, label_, fg_, bg_);
        }
    }
}

bool Button::handleTouch(const TouchState& touch) {
    return touch.pressed && Rect{x_, y_, w_, h_}.contains(touch.x, touch.y);
}

} // namespace ui
