#include "textfield.h"
#include "rect.h"
#include "font.h"
#include <cstring>

namespace ui {

TextField::TextField(int x, int y, int w, int h, uint16_t fg, uint16_t bg)
    : x_(x), y_(y), w_(w), h_(h), fg_(fg), bg_(bg) {}

void TextField::draw(Framebuffer& fb, const char* text, int cursorPos, bool focused) {
    int pad = 3;
    int innerW = w_ - pad * 2;
    int innerH = h_ - pad * 2;
    if (innerW < 1 || innerH < 1) return;

    fb.drawRect(x_, y_, w_, h_, fg_);
    fb.fillRect(x_ + 1, y_ + 1, w_ - 2, h_ - 2, BG_SURFACE);

    if (!text) return;

    int maxChars = innerW / 6;
    int textLen = static_cast<int>(std::strlen(text));

    int drawLen = textLen;
    int offset = 0;
    if (drawLen > maxChars) {
        if (cursorPos > maxChars / 2) {
            offset = cursorPos - maxChars / 2;
            if (offset > textLen - maxChars)
                offset = textLen - maxChars;
            if (offset < 0) offset = 0;
        }
        drawLen = maxChars;
    }

    int tx = x_ + pad;
    int ty = y_ + (h_ - FONT_H) / 2;
    for (int i = 0; i < drawLen && (offset + i) < textLen; i++) {
        fb.drawChar(tx + i * FONT_STEP, ty, text[offset + i], fg_, BG_SURFACE);
    }

    if (focused && cursorPos >= offset && cursorPos <= offset + drawLen
        && cursorPos <= textLen) {
        int cx = tx + (cursorPos - offset) * 6;
        fb.fillRect(cx, ty, 1, 7, ACCENT_1);
    }
}

bool TextField::handleTouch(const TouchState& touch) {
    return touch.pressed &&
           Rect{x_, y_, w_, h_}.contains(touch.x, touch.y);
}

} // namespace ui
