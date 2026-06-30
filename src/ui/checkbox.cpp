#include "checkbox.h"
#include "font.h"

namespace ui {

Checkbox::Checkbox(int x, int y, uint16_t fg, uint16_t bg)
    : size_(DEFAULT_SIZE), fg_(fg), bg_(bg) {
    setBounds(x, y, size_, size_);
}

void Checkbox::draw(Framebuffer& fb, bool checked, const char* label) {
    fb.drawRect(x_, y_, size_, size_, fg_);
    if (checked) {
        fb.fillRect(x_ + 2, y_ + 2, size_ - 4, size_ - 4, fg_);
    }
    if (label) {
        fb.drawText(x_ + size_ + 4, y_, label, fg_, bg_);
    }
}

void Checkbox::onTap(const TouchEvent&) {
    wasTapped_ = true;
}

} // namespace ui
