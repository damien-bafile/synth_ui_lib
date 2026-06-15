#include "radio.h"

namespace ui {

Radio::Radio(int x, int y, uint16_t fg, uint16_t bg)
    : x_(x), y_(y), radius_(DEFAULT_RADIUS), fg_(fg), bg_(bg) {}

void Radio::draw(Framebuffer& fb, bool selected, const char* label) {
    int cx = x_ + radius_;
    int cy = y_ + radius_;

    fb.drawCircle(cx, cy, radius_, fg_);

    if (selected) {
        int ir = radius_ / 2;
        if (ir < 1) ir = 1;
        fb.fillCircle(cx, cy, ir, fg_);
    }

    if (label) {
        int lx = x_ + radius_ * 2 + 4;
        int ly = cy - 4;
        fb.drawText(lx, ly, label, fg_, bg_);
    }
}

bool Radio::handleTouch(const TouchState& touch) {
    int cx = x_ + radius_;
    int cy = y_ + radius_;
    int dx = touch.x - cx;
    int dy = touch.y - cy;
    return touch.pressed && (dx * dx + dy * dy <= radius_ * radius_);
}

void Radio::drawGroup(Framebuffer& fb, int x, int y,
                      const char* labels[], int count, int selected,
                      uint16_t fg, uint16_t bg) {
    int spacing = DEFAULT_RADIUS * 2 + 8;
    for (int i = 0; i < count; i++) {
        int sy = y + i * spacing;
        fb.drawCircle(x + DEFAULT_RADIUS, sy + DEFAULT_RADIUS, DEFAULT_RADIUS, fg);
        if (i == selected) {
            fb.fillCircle(x + DEFAULT_RADIUS, sy + DEFAULT_RADIUS, DEFAULT_RADIUS / 2, fg);
        }
        if (labels[i]) {
            int lx = x + DEFAULT_RADIUS * 2 + 4;
            int ly = sy + DEFAULT_RADIUS - 4;
            fb.drawText(lx, ly, labels[i], fg, bg);
        }
    }
}

} // namespace ui
