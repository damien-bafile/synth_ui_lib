#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <ui.h>

#define UI_TOGGLE_W 30
#define UI_TOGGLE_H 16

// Draw a pill-shaped toggle switch at (x,y) of (w,h).
void ui_draw_toggle(ui_fb_t* fb, int x, int y, int w, int h,
                    bool on, uint16_t fg, uint16_t bg);

// Draw a toggle switch with a label to the right.
void ui_draw_toggle_with_label(ui_fb_t* fb, int x, int y, int w, int h,
                               bool on, const char* label,
                               uint16_t fg, uint16_t bg);

#ifdef __cplusplus
}
#endif
