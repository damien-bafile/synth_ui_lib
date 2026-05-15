#include "ui.h"
#include "ui_font_5x7.h"
#include <string.h>

// ---------------------------------------------------------------------------
//  Helpers
// ---------------------------------------------------------------------------

static inline void pack_rgb565(uint8_t* dst, uint16_t color) {
  dst[0] = color & 0xFF;
  dst[1] = (color >> 8) & 0xFF;
}

// ---------------------------------------------------------------------------
//  Drawing primitives
// ---------------------------------------------------------------------------

void ui_set_pixel(ui_fb_t* fb, int x, int y, uint16_t color) {
  if (x < 0 || x >= fb->width || y < 0 || y >= fb->height) return;
  pack_rgb565(fb->data + ((fb->height - 1 - y) * fb->width + x) * 2, color);
}

void ui_fill_rect(ui_fb_t* fb, int x, int y, int w, int h, uint16_t color) {
  int x0 = x < 0 ? 0 : x;
  int y0 = y < 0 ? 0 : y;
  int x1 = x + w > fb->width  ? fb->width  : x + w;
  int y1 = y + h > fb->height ? fb->height : y + h;

  uint8_t lo = color & 0xFF;
  uint8_t hi = (color >> 8) & 0xFF;

  for (int cy = y0; cy < y1; cy++) {
    int fb_row = fb->height - 1 - cy;
    uint8_t* line = fb->data + (fb_row * fb->width + x0) * 2;
    for (int col = x0; col < x1; col++) {
      *line++ = lo;
      *line++ = hi;
    }
  }
}

void ui_draw_rect(ui_fb_t* fb, int x, int y, int w, int h, uint16_t color) {
  ui_fill_rect(fb, x, y, w, 1, color);               // top
  ui_fill_rect(fb, x, y + h - 1, w, 1, color);       // bottom
  ui_fill_rect(fb, x, y, 1, h, color);                // left
  ui_fill_rect(fb, x + w - 1, y, 1, h, color);        // right
}

void ui_fill_screen(ui_fb_t* fb, uint16_t color) {
  ui_fill_rect(fb, 0, 0, fb->width, fb->height, color);
}

// ---------------------------------------------------------------------------
//  Text
// ---------------------------------------------------------------------------

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

void ui_draw_text(ui_fb_t* fb, int x, int y, const char* text,
                  uint16_t fg, uint16_t bg) {
  for (int i = 0; text[i]; i++) {
    ui_draw_char(fb, x + i * 6, y, text[i], fg, bg);
  }
}

int ui_text_width(const char* text) {
  return (int)strlen(text) * 6;
}

// ---------------------------------------------------------------------------
//  Grid helpers
// ---------------------------------------------------------------------------

void ui_grid_cell(int* ox, int* oy, int* ow, int* oh,
                  int col, int row, int cols, int rows,
                  int fb_w, int fb_h, int padding) {
  int col_w = fb_w / cols;
  int row_h = fb_h / rows;
  *ow = col_w;
  *oh = row_h;
  *ox = col * col_w + padding;
  *oy = (rows - 1 - row) * row_h + padding;
  *ow -= padding * 2;
  *oh -= padding * 2;
}
