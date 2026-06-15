#include "ui_textfield.h"
#include <string.h>

void ui_draw_text_field(ui_fb_t* fb, int x, int y, int w, int h,
                        const char* text, int cursor_pos, bool focused,
                        uint16_t fg, uint16_t bg) {
    int pad = 3;
    int inner_w = w - pad * 2;
    int inner_h = h - pad * 2;
    if (inner_w < 1 || inner_h < 1) return;

    ui_draw_rect(fb, x, y, w, h, fg);
    ui_fill_rect(fb, x + 1, y + 1, w - 2, h - 2, UI_BG_SURFACE);

    if (!text) return;

    int max_chars = inner_w / 6;
    int text_len = (int)strlen(text);

    int draw_len = text_len;
    int offset = 0;
    if (draw_len > max_chars) {
        if (cursor_pos > max_chars / 2) {
            offset = cursor_pos - max_chars / 2;
            if (offset > text_len - max_chars)
                offset = text_len - max_chars;
            if (offset < 0) offset = 0;
        }
        draw_len = max_chars;
    }

    int tx = x + pad;
    int ty = y + (h - 7) / 2;
    for (int i = 0; i < draw_len && (offset + i) < text_len; i++) {
        ui_draw_char(fb, tx + i * 6, ty, text[offset + i], fg, UI_BG_SURFACE);
    }

    if (focused && cursor_pos >= offset && cursor_pos <= offset + draw_len
        && cursor_pos <= text_len) {
        int cx = tx + (cursor_pos - offset) * 6;
        ui_fill_rect(fb, cx, ty, 1, 7, UI_ACCENT_1);
    }
}
