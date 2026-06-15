#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <ui.h>

// Look up a track color by index (0..7). Returns UI_TEXT_DIM for out-of-range.
uint16_t ui_track_color(int idx);

#ifdef __cplusplus
}
#endif
