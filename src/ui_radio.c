#include "ui_radio.h"

void ui_draw_radio(ui_fb_t* fb, int x, int y, int radius, bool selected,
                   const char* label, uint16_t fg, uint16_t bg) {
    int cx = x + radius;
    int cy = y + radius;

    ui_draw_circle(fb, cx, cy, radius, fg);

    if (selected) {
        int ir = radius / 2;
        if (ir < 1) ir = 1;
        ui_fill_circle(fb, cx, cy, ir, fg);
    }

    if (label) {
        int lx = x + radius * 2 + 4;
        int ly = cy - 4;
        ui_draw_text(fb, lx, ly, label, fg, bg);
    }
}

void ui_draw_radio_group(ui_fb_t* fb, int x, int y,
                         const char* labels[], int count, int selected,
                         uint16_t fg, uint16_t bg) {
    int spacing = UI_RADIO_RADIUS * 2 + 8;
    for (int i = 0; i < count; i++) {
        int sy = y + i * spacing;
        ui_draw_radio(fb, x, sy, UI_RADIO_RADIUS,
                      i == selected, labels[i], fg, bg);
    }
}
