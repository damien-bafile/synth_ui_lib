#include "label.h"
#include "font.h"
#include <cstring>

namespace ui {

static constexpr int VERTICAL_STEP = 10;

void Label::draw(Framebuffer& fb, const char* text) const {
    if (!text) return;

    if (align_ == Align::VERTICAL) {
        int len = (int)strlen(text);
        int startY = y_ + (h_ > 0 ? (h_ - len * VERTICAL_STEP) / 2 : 0);
        for (int i = 0; i < len; i++) {
            fb.drawChar(x_, startY + i * VERTICAL_STEP, text[i], fg_, bg_);
        }
        return;
    }

    int tx = x_;
    if (w_ > 0) {
        int tw = Framebuffer::textWidth(text);
        if (align_ == Align::CENTER)      tx = x_ + (w_ - tw) / 2;
        else if (align_ == Align::RIGHT)  tx = x_ + w_ - tw;
    }
    int ty = y_ + (h_ > 0 ? (h_ - FONT_H) / 2 : 0);
    fb.drawText(tx, ty, text, fg_, bg_);
}

} // namespace ui
