#include "ui.h"
#include "ui_font_5x7.h"
#include "ui_icons.h"
#include <string.h>

// ---------------------------------------------------------------------------
//  Helpers
// ---------------------------------------------------------------------------

// Write a 16-bit RGB565 color into two consecutive bytes (little-endian).
static inline void pack_rgb565(uint8_t* dst, uint16_t color) {
  dst[0] = color & 0xFF;
  dst[1] = (color >> 8) & 0xFF;
}

// ---------------------------------------------------------------------------
//  Drawing primitives
// ---------------------------------------------------------------------------

// Set a single pixel, clamping to framebuffer bounds.
void ui_set_pixel(ui_fb_t* fb, int x, int y, uint16_t color) {
  if (x < 0 || x >= fb->width || y < 0 || y >= fb->height) return;
  pack_rgb565(fb->data + (y * fb->width + x) * 2, color);
}

// Fill a clipped rectangular region with a solid color.
void ui_fill_rect(ui_fb_t* fb, int x, int y, int w, int h, uint16_t color) {
  int x0 = x < 0 ? 0 : x;
  int y0 = y < 0 ? 0 : y;
  int x1 = x + w > fb->width  ? fb->width  : x + w;
  int y1 = y + h > fb->height ? fb->height : y + h;

  uint8_t lo = color & 0xFF;
  uint8_t hi = (color >> 8) & 0xFF;

  for (int cy = y0; cy < y1; cy++) {
    int fb_row = cy;
    uint8_t* line = fb->data + (fb_row * fb->width + x0) * 2;
    for (int col = x0; col < x1; col++) {
      *line++ = lo;
      *line++ = hi;
    }
  }
}

// Draw a 1-pixel-wide border around a rectangle (uses fill_rect for each edge).
void ui_draw_rect(ui_fb_t* fb, int x, int y, int w, int h, uint16_t color) {
  ui_fill_rect(fb, x, y, w, 1, color);               // top
  ui_fill_rect(fb, x, y + h - 1, w, 1, color);       // bottom
  ui_fill_rect(fb, x, y, 1, h, color);                // left
  ui_fill_rect(fb, x + w - 1, y, 1, h, color);        // right
}

// Fill the entire framebuffer with a single color.
void ui_fill_screen(ui_fb_t* fb, uint16_t color) {
  ui_fill_rect(fb, 0, 0, fb->width, fb->height, color);
}

// ---------------------------------------------------------------------------
//  Text
// ---------------------------------------------------------------------------

// Render a single character from the 5x7 bitmap font, with optional background.
void ui_draw_char(ui_fb_t* fb, int x, int y, char c, uint16_t fg, uint16_t bg) {
  if (c < 32 || c > 127) c = '?';
  int glyph = c - 32;

  for (int col = 0; col < 5; col++) {
    unsigned char bits = font_5x7[glyph * 5 + col];
    for (int row = 0; row < 7; row++) {
      ui_set_pixel(fb, x + col, y + row,
                   (bits >> row) & 1 ? fg : bg);
    }
  }
}

// Render a null-terminated string left-to-right with 6px per character.
void ui_draw_text(ui_fb_t* fb, int x, int y, const char* text,
                  uint16_t fg, uint16_t bg) {
  for (int i = 0; text[i]; i++) {
    ui_draw_char(fb, x + i * 6, y, text[i], fg, bg);
  }
}

// Return the width of a string in pixels (6px per character).
int ui_text_width(const char* text) {
  return (int)strlen(text) * 6;
}

// ---------------------------------------------------------------------------
//  Grid helpers
// ---------------------------------------------------------------------------

// Compute the bounding box (x,y,w,h) of a grid cell with padding.
// Row 0 is the topmost cell; column 0 is the leftmost cell.
void ui_grid_cell(int* ox, int* oy, int* ow, int* oh,
                  int col, int row, int cols, int rows,
                  int fb_w, int fb_h, int padding) {
  int col_w = fb_w / cols;
  int row_h = fb_h / rows;
  *ow = col_w;
  *oh = row_h;
  *ox = col * col_w + padding;
  *oy = row * row_h + padding;
  *ow -= padding * 2;
  *oh -= padding * 2;
}

// ---------------------------------------------------------------------------
//  Synth / FX icons
// ---------------------------------------------------------------------------

// Draw a 5x7 synth icon glyph with foreground and background colors.
void ui_draw_synth_icon(ui_fb_t* fb, int x, int y, SynthIcon icon,
                        uint16_t fg, uint16_t bg) {
  if (icon >= ICON_COUNT) return;
  for (int col = 0; col < 5; col++) {
    unsigned char bits = synth_glyphs[icon * 5 + col];
    for (int row = 0; row < 7; row++) {
      ui_set_pixel(fb, x + col, y + row,
                   (bits >> row) & 1 ? fg : bg);
    }
  }
}

// Draw a 5x7 FX icon glyph with foreground and background colors.
void ui_draw_fx_icon(ui_fb_t* fb, int x, int y, FxIcon icon,
                     uint16_t fg, uint16_t bg) {
  if (icon >= FX_ICON_COUNT) return;
  for (int col = 0; col < 5; col++) {
    unsigned char bits = fx_glyphs[icon * 5 + col];
    for (int row = 0; row < 7; row++) {
      ui_set_pixel(fb, x + col, y + row,
                   (bits >> row) & 1 ? fg : bg);
    }
  }
}
