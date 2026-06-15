#include "ui_panel.h"

void ui_draw_panel_with_title(ui_fb_t* fb, int x, int y, int w, int h,
                              const char* title, uint16_t title_color) {
    ui_draw_rect(fb, x, y, w, h, UI_BG_MID);
    ui_fill_rect(fb, x + 1, y + 1, w - 2, UI_PANEL_TITLE_H, UI_BG_SURFACE);
    ui_draw_text(fb, x + 6, y + 3, title, title_color, UI_BG_SURFACE);
}
