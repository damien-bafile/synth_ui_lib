#include "ui_hbar.h"
#include <math.h>

void draw_hbar(ui_fb_t* fb, int x, int y, int w, int h,
               float val, uint16_t fill, uint16_t bg) {
    int fw = (int)(w * val);
    if (fw < 0) fw = 0;
    if (fw > w) fw = w;
    if (fw > 0) ui_fill_rect(fb, x, y, fw, h, fill);
    if (fw < w) ui_fill_rect(fb, x + fw, y, w - fw, h, bg);
}

void draw_centered_hbar_cents(ui_fb_t* fb, int x, int y, int w, int h,
                              float val_cents, float max_cents,
                              uint16_t fill, uint16_t bg) {
    ui_fill_rect(fb, x, y, w, h, bg);

    int half = w / 2;
    if (half < 1) return;

    if (max_cents <= 0.0f) return;
    float mag = val_cents;
    if (mag < -max_cents) mag = -max_cents;
    if (mag >  max_cents) mag =  max_cents;

    if (mag < 0.0f) {
        int fw = (int)(((-mag) / max_cents) * half);
        if (fw > half) fw = half;
        if (fw > 0) ui_fill_rect(fb, x + half - fw, y, fw, h, fill);
    } else if (mag > 0.0f) {
        int fw = (int)((mag / max_cents) * half);
        if (fw > half) fw = half;
        if (fw > 0) ui_fill_rect(fb, x + half, y, fw, h, fill);
    }

    ui_fill_rect(fb, x + half, y, 1, h, fill);
}
