#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <ui.h>

#define UI_CHECKBOX_SIZE 12

// Draw a checkbox at (x,y) with label to the right.
void ui_draw_checkbox(ui_fb_t* fb, int x, int y, int size, bool checked,
                      const char* label, uint16_t fg, uint16_t bg);

#ifdef __cplusplus
}
#endif
