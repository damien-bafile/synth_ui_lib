#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <ui.h>

#define UI_PANEL_TITLE_H 18

// Draw a bordered panel with a title bar at (x,y) of (w,h). The title bar is
// 18px tall and the title text is drawn in the given color.
void ui_draw_panel_with_title(ui_fb_t* fb, int x, int y, int w, int h,
                              const char* title, uint16_t title_color);

#ifdef __cplusplus
}
#endif
