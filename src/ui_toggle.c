#include "ui_toggle.h"

void ui_draw_toggle(ui_fb_t* fb, int x, int y, int w, int h,
                    bool on, uint16_t fg, uint16_t bg) {
    int pad = 2;
    int track_h = h - pad * 2;
    int track_w = w - pad * 2;
    if (track_h < 2 || track_w < 2) return;
    int cy = y + h / 2;
    int thumb_r = track_h / 2;

    if (on) {
        ui_fill_rect(fb, x, y, w, h, fg);
        int tx = x + w - pad - thumb_r;
        ui_fill_circle(fb, tx, cy, thumb_r, bg);
    } else {
        ui_fill_rect(fb, x, y, w, h, bg);
        ui_draw_rect(fb, x, y, w, h, fg);
        int tx = x + pad + thumb_r;
        ui_fill_circle(fb, tx, cy, thumb_r, fg);
    }
}

void ui_draw_toggle_with_label(ui_fb_t* fb, int x, int y, int w, int h,
                               bool on, const char* label,
                               uint16_t fg, uint16_t bg) {
    ui_draw_toggle(fb, x, y, w, h, on, fg, bg);
    if (label) {
        int ly = y + (h - 7) / 2;
        ui_draw_text(fb, x + w + 4, ly, label, fg, bg);
    }
}
