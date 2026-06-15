#include "ui_slider.h"

void ui_draw_slider_h(ui_fb_t* fb, int x, int y, int w, int h,
                      float value, float min, float max,
                      const char* label, uint16_t fg, uint16_t bg) {
    if (label) {
        ui_draw_text(fb, x, y - 10, label, fg, bg);
    }

    int thumb_w = 4;
    int thumb_h = h + 6;
    if (thumb_h < 2) thumb_h = 2;

    ui_fill_rect(fb, x, y + (h - 3) / 2, w, 3, bg);
    ui_draw_rect(fb, x, y + (h - 3) / 2, w, 3, fg);

    float range = max - min;
    float frac = (range > 0.0f) ? (value - min) / range : 0.0f;
    if (frac < 0.0f) frac = 0.0f;
    if (frac > 1.0f) frac = 1.0f;
    int fill_w = (int)(w * frac);

    if (fill_w > 0) {
        ui_fill_rect(fb, x, y + (h - 3) / 2, fill_w, 3, fg);
    }

    int thumb_x = x + fill_w - thumb_w / 2;
    if (thumb_x < x) thumb_x = x;
    if (thumb_x + thumb_w > x + w) thumb_x = x + w - thumb_w;
    int thumb_y = y + (h - thumb_h) / 2;
    ui_fill_rect(fb, thumb_x, thumb_y, thumb_w, thumb_h, fg);
}

void ui_draw_slider_v(ui_fb_t* fb, int x, int y, int w, int h,
                      float value, float min, float max,
                      const char* label, uint16_t fg, uint16_t bg) {
    if (label) {
        ui_draw_text(fb, x, y + h + 4, label, fg, bg);
    }

    int thumb_w = w + 6;
    int thumb_h = 4;
    if (thumb_w < 2) thumb_w = 2;

    ui_fill_rect(fb, x + (w - 3) / 2, y, 3, h, bg);
    ui_draw_rect(fb, x + (w - 3) / 2, y, 3, h, fg);

    float range = max - min;
    float frac = (range > 0.0f) ? (value - min) / range : 0.0f;
    if (frac < 0.0f) frac = 0.0f;
    if (frac > 1.0f) frac = 1.0f;
    int fill_h = (int)(h * frac);

    int fy = y + h - fill_h;
    if (fill_h > 0) {
        ui_fill_rect(fb, x + (w - 3) / 2, fy, 3, fill_h, fg);
    }

    int thumb_y = fy - thumb_h / 2;
    if (thumb_y < y) thumb_y = y;
    if (thumb_y + thumb_h > y + h) thumb_y = y + h - thumb_h;
    int thumb_x = x + (w - thumb_w) / 2;
    ui_fill_rect(fb, thumb_x, thumb_y, thumb_w, thumb_h, fg);
}
