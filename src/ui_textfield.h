#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <ui.h>

// Draw a text field at (x,y) of (w,h) with an optional cursor at `cursor_pos`.
// When `focused`, draws a blinking-style cursor indicator.
// Text is clipped to fit within the field minus 3px padding on each side.
void ui_draw_text_field(ui_fb_t* fb, int x, int y, int w, int h,
                        const char* text, int cursor_pos, bool focused,
                        uint16_t fg, uint16_t bg);

#ifdef __cplusplus
}
#endif
