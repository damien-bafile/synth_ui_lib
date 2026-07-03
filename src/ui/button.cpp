#include "button.h"
#include "font.h"

namespace ui {

Button::Button(int x, int y, int w, int h, const char* label,
               uint16_t fg, uint16_t bg)
    : label_(label), fg_(fg), bg_(bg) {
    setBounds(x, y, w, h);
}

void Button::drawIcon(Framebuffer& fb, int ix, int iy, uint16_t fg, uint16_t bg) {
    if (sicon_ != SynthIcon::NONE) fb.drawSynthIcon(ix, iy, sicon_, fg, bg);
    else if (ficon_ != FxIcon::ICON_COUNT) fb.drawFxIcon(ix, iy, ficon_, fg, bg);
}

void Button::draw(Framebuffer& fb, bool pressed) {
    uint16_t fill = pressed ? fg_ : bg_;
    uint16_t ink  = pressed ? bg_ : fg_;

    fb.fillRect(x_, y_, w_, h_, fill);
    if (border_) fb.drawRect(x_, y_, w_, h_, ink);

    uint16_t iconInk = (iconColor_ != 0 && !pressed) ? iconColor_ : ink;
    int iy = y_ + (h_ - 7) / 2;
    int ty = y_ + (h_ - FONT_H) / 2;

    if (hasIcon() && label_) {
        // Icon at the left edge, label following, both left-aligned.
        drawIcon(fb, x_ + 8, iy, iconInk, fill);
        fb.drawText(x_ + 18, ty, label_, ink, fill);
    } else if (hasIcon()) {
        drawIcon(fb, x_ + (w_ - 5) / 2, iy, iconInk, fill);
    } else if (label_) {
        int tw = Framebuffer::textWidth(label_);
        fb.drawText(x_ + (w_ - tw) / 2, ty, label_, ink, fill);
    }
}

void Button::onTap(const TouchEvent&) {
    wasTapped_ = true;
}

} // namespace ui
