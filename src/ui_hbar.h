#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <ui.h>

// Draw a horizontal progress bar at (x,y) of size (w,h) filled to `val` ∈ [0,1].
void draw_hbar(ui_fb_t* fb, int x, int y, int w, int h,
               float val, uint16_t fill, uint16_t bg);

// Draw a horizontal bar centered at 0 that fills OUTWARD from the centerline.
// `val_cents` is a signed value in cents, with the resting state at 0. The
// bar grows toward the left edge as val_cents goes negative, and toward the
// right edge as val_cents goes positive. `max_cents` is the absolute value at
// which a half-bar is fully filled. Useful for pitch-bend / wheel displays
// where the resting state is centered and bending is symmetric in perception.
//
// Example: pitch bend multiplier p, max ±2 semitones:
//   cents = log2(p) * 1200        // signed
//   draw_centered_hbar_cents(..., cents, 200, ...);
void draw_centered_hbar_cents(ui_fb_t* fb, int x, int y, int w, int h,
                              float val_cents, float max_cents,
                              uint16_t fill, uint16_t bg);

#ifdef __cplusplus
}
#endif
