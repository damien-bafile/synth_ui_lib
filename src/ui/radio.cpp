#include "radio.h"
#include "font.h"

namespace ui {

Radio::Radio(int x, int y, uint16_t fg, uint16_t bg)
    : radius_(DEFAULT_RADIUS), fg_(fg), bg_(bg) {
    setBounds(x - radius_, y - radius_, radius_ * 2, radius_ * 2);
}

void Radio::setPosition(int x, int y) noexcept {
    setBounds(x - radius_, y - radius_, radius_ * 2, radius_ * 2);
}

bool Radio::isInside(int px, int py) const noexcept {
    int dx = px - (x_ + radius_);
    int dy = py - (y_ + radius_);
    return (dx * dx + dy * dy) <= (radius_ * radius_);
}

bool Radio::onTouchBegan(const TouchEvent& event) {
    return isInside(event.x, event.y);
}

void Radio::onTap(const TouchEvent&) {
    wasTapped_ = true;
}

void Radio::draw(Framebuffer& fb, bool selected, const char* label) {
    int cx = x_ + radius_;
    int cy = y_ + radius_;
    fb.drawCircle(cx, cy, radius_, fg_);
    if (selected) {
        fb.fillCircle(cx, cy, radius_ - 3, fg_);
    }
    if (label) {
        fb.drawText(cx + radius_ + 4, cy - FONT_H / 2, label, fg_, bg_);
    }
}

void Radio::drawGroup(Framebuffer& fb, int x, int y,
                       const char* labels[], int count, int selected,
                       uint16_t fg, uint16_t bg) {
    for (int i = 0; i < count; i++) {
        Radio r(x, y + i * (DEFAULT_RADIUS * 2 + 4), fg, bg);
        r.draw(fb, i == selected, labels[i]);
    }
}

} // namespace ui
