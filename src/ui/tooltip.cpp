#include "tooltip.h"

namespace ui {

void Tooltip::paintTrampoline(Framebuffer& fb, void* user) {
    static_cast<Tooltip*>(user)->paint(fb);
}

void Tooltip::paint(Framebuffer& fb) {
    if (!visible_ || !text_) return;

    int w = getWidth();
    int h = getHeight();
    fb.fillRect(x_, y_, w, h, bg_);
    fb.drawRect(x_, y_, w, h, border_);
    fb.drawText(x_ + PADDING, y_ + PADDING, text_, fg_, bg_);
}

void Tooltip::draw(Framebuffer& fb) {
    if (!visible_) return;
    fb.enqueueOverlay(x_, y_, getWidth(), getHeight(),
                      &Tooltip::paintTrampoline, this,
                      reinterpret_cast<uintptr_t>(this));
}

} // namespace ui
