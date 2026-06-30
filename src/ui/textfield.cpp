#include "textfield.h"
#include "font.h"
#include <cstring>

namespace ui {

TextField::TextField(int x, int y, int w, int h,
                     uint16_t fg, uint16_t bg)
    : fg_(fg), bg_(bg) {
    setBounds(x, y, w, h);
}

void TextField::draw(Framebuffer& fb, const char* text, int cursorPos, bool focused) {
    fb.fillRect(x_, y_, w_, h_, bg_);
    fb.drawRect(x_, y_, w_, h_, focused ? fg_ : GRAY_DARK);

    if (!text) return;

    int textLen = static_cast<int>(std::strlen(text));
    int maxVisible = w_ / FONT_STEP;
    if (maxVisible < 1) maxVisible = 1;

    int offset = 0;
    if (cursorPos > maxVisible - 1) {
        offset = cursorPos - maxVisible + 1;
    }
    int drawLen = textLen - offset;
    if (drawLen > maxVisible) drawLen = maxVisible;

    int tx = x_ + 2;
    int ty = y_ + (h_ - FONT_H) / 2;

    for (int i = 0; i < drawLen; i++) {
        fb.drawChar(tx + i * FONT_STEP, ty, text[offset + i], fg_, BG_SURFACE);
    }

    if (focused && cursorPos >= offset && cursorPos <= offset + drawLen
        && cursorPos <= textLen) {
        int cx = tx + (cursorPos - offset) * 6;
        fb.fillRect(cx, ty, 1, 7, ACCENT_1);
    }
}

void TextField::onTap(const TouchEvent&) {
    wasTapped_ = true;
}

} // namespace ui
