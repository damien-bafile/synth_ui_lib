#include "checkbox.h"

namespace ui {

Checkbox::Checkbox(int x, int y, uint16_t fg, uint16_t bg)
    : x_(x), y_(y), size_(DEFAULT_SIZE), fg_(fg), bg_(bg) {}

void Checkbox::draw(Framebuffer& fb, bool checked, const char* label) {
    fb.drawRect(x_, y_, size_, size_, fg_);

    if (checked) {
        int s2 = size_ - 4;
        if (s2 < 2) s2 = 2;
        int ox = x_ + 2;
        int oy = y_ + 2;
        for (int i = 0; i < s2; i++) {
            fb.setPixel(ox + i,          oy + i,           fg_);
            fb.setPixel(ox + s2 - 1 - i, oy + i,           fg_);
        }
    }

    if (label) {
        fb.drawText(x_ + size_ + 4, y_ + (size_ - 7) / 2, label, fg_, bg_);
    }
}

bool Checkbox::handleTouch(const TouchState& touch) {
    return touch.pressed &&
           touch.x >= x_ && touch.x < x_ + size_ &&
           touch.y >= y_ && touch.y < y_ + size_;
}

} // namespace ui
