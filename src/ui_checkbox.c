#include "ui_checkbox.h"

void ui_draw_checkbox(ui_fb_t* fb, int x, int y, int size, bool checked,
                      const char* label, uint16_t fg, uint16_t bg) {
    ui_draw_rect(fb, x, y, size, size, fg);

    if (checked) {
        int s2 = size - 4;
        if (s2 < 2) s2 = 2;
        int ox = x + 2;
        int oy = y + 2;
        for (int i = 0; i < s2; i++) {
            ui_set_pixel(fb, ox + i,           oy + i,            fg);
            ui_set_pixel(fb, ox + s2 - 1 - i,  oy + i,            fg);
        }
    }

    if (label) {
        ui_draw_text(fb, x + size + 4, y + (size - 7) / 2, label, fg, bg);
    }
}
