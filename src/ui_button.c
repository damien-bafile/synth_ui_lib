#include "ui_button.h"
#include <string.h>

void ui_draw_button(ui_fb_t* fb, int x, int y, int w, int h,
                    const char* label, bool pressed, uint16_t fg, uint16_t bg) {
    if (pressed) {
        ui_fill_rect(fb, x, y, w, h, fg);
        ui_draw_rect(fb, x, y, w, h, bg);
        if (label) {
            int tw = (int)strlen(label) * 6;
            int tx = x + (w - tw) / 2;
            int ty = y + (h - 7) / 2;
            ui_draw_text(fb, tx, ty, label, bg, fg);
        }
    } else {
        ui_fill_rect(fb, x, y, w, h, bg);
        ui_draw_rect(fb, x, y, w, h, fg);
        if (label) {
            int tw = (int)strlen(label) * 6;
            int tx = x + (w - tw) / 2;
            int ty = y + (h - 7) / 2;
            ui_draw_text(fb, tx, ty, label, fg, bg);
        }
    }
}
