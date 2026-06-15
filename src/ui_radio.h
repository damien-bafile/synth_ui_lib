#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <ui.h>

#define UI_RADIO_RADIUS 7

// Draw a radio button at (x,y) with label to the right.
void ui_draw_radio(ui_fb_t* fb, int x, int y, int radius, bool selected,
                   const char* label, uint16_t fg, uint16_t bg);

// Draw a vertical list of radio options, `selected` is the active index.
void ui_draw_radio_group(ui_fb_t* fb, int x, int y,
                         const char* labels[], int count, int selected,
                         uint16_t fg, uint16_t bg);

#ifdef __cplusplus
}
#endif
