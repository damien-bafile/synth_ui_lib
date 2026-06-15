#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <ui.h>

// Draw a button at (x,y) of (w,h) with centered text.
// When `pressed`, colors are inverted.
void ui_draw_button(ui_fb_t* fb, int x, int y, int w, int h,
                    const char* label, bool pressed, uint16_t fg, uint16_t bg);

#ifdef __cplusplus
}
#endif
