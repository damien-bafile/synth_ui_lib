#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <ui.h>

// Draw a horizontal slider at (x,y) with track (w,h).
// `value` is in range [min, max]. `label` is drawn above.
void ui_draw_slider_h(ui_fb_t* fb, int x, int y, int w, int h,
                      float value, float min, float max,
                      const char* label, uint16_t fg, uint16_t bg);

// Draw a vertical slider at (x,y) with track (w,h).
void ui_draw_slider_v(ui_fb_t* fb, int x, int y, int w, int h,
                      float value, float min, float max,
                      const char* label, uint16_t fg, uint16_t bg);

#ifdef __cplusplus
}
#endif
