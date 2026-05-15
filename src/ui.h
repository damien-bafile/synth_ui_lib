#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// --- Framebuffer descriptor ---
// Describes a 320x240 (or similar) RGB565 framebuffer in memory.
typedef struct {
  uint8_t* data;
  int width;
  int height;
} ui_fb_t;

// --- Color packing macro ---
// Pack 8-bit RGB components into a 16-bit RGB565 value.
#define UI_RGB565(r, g, b) \
  (((uint16_t)((r) & 0xF8) << 8) | ((uint16_t)((g) & 0xFC) << 3) | ((uint8_t)(b) >> 3))

// ============================================================
//  Color palette
// ============================================================

// Base
#define UI_BLACK      UI_RGB565(0,   0,   0)
#define UI_WHITE      UI_RGB565(255, 255, 255)
#define UI_RED        UI_RGB565(255, 0,   0)
#define UI_GREEN      UI_RGB565(0,   255, 0)
#define UI_BLUE       UI_RGB565(0,   0,   255)
#define UI_YELLOW     UI_RGB565(255, 255, 0)
#define UI_CYAN       UI_RGB565(0,   255, 255)
#define UI_MAGENTA    UI_RGB565(255, 0,   255)

// Shades
#define UI_GRAY_DARK   UI_RGB565(40,  40,  40)
#define UI_GRAY_MID    UI_RGB565(100, 100, 100)
#define UI_GRAY_LIGHT  UI_RGB565(180, 180, 180)
#define UI_ORANGE      UI_RGB565(255, 140, 0)
#define UI_PURPLE      UI_RGB565(140, 0,   255)
#define UI_TEAL        UI_RGB565(0,   180, 180)
#define UI_PINK        UI_RGB565(255, 100, 180)
#define UI_LIME        UI_RGB565(140, 255, 0)
#define UI_NAVY        UI_RGB565(0,   0,   120)
#define UI_MAROON      UI_RGB565(120, 0,   0)
#define UI_OLIVE       UI_RGB565(100, 120, 0)
#define UI_SILVER      UI_RGB565(200, 200, 210)
#define UI_GOLD        UI_RGB565(255, 200, 40)
#define UI_CORAL       UI_RGB565(255, 80,  80)

// UI Theme (synthwave-inspired)
#define UI_BG_DARK     UI_RGB565(10,  10,  18)
#define UI_BG_MID      UI_RGB565(18,  18,  30)
#define UI_BG_SURFACE  UI_RGB565(28,  28,  48)
#define UI_TEXT        UI_RGB565(220, 220, 240)
#define UI_TEXT_DIM    UI_RGB565(120, 120, 160)
#define UI_ACCENT_1    UI_RGB565(255, 60,  120)
#define UI_ACCENT_2    UI_RGB565(80,  200, 255)
#define UI_ACCENT_3    UI_RGB565(180, 80,  255)
#define UI_ACCENT_4    UI_RGB565(60,  255, 140)
#define UI_HIGHLIGHT   UI_RGB565(255, 200, 60)
#define UI_WARN        UI_RGB565(255, 80,  80)
#define UI_RECORD      UI_RGB565(255, 40,  40)
#define UI_PLAY        UI_RGB565(60,  255, 60)
#define UI_MUTE        UI_RGB565(255, 180, 60)
#define UI_SOLO        UI_RGB565(255, 255, 60)

// Track colors (8 distinct colors)
#define UI_TRACK_0     UI_RGB565(255, 80,  120)
#define UI_TRACK_1     UI_RGB565(80,  200, 255)
#define UI_TRACK_2     UI_RGB565(180, 80,  255)
#define UI_TRACK_3     UI_RGB565(60,  255, 140)
#define UI_TRACK_4     UI_RGB565(255, 200, 60)
#define UI_TRACK_5     UI_RGB565(255, 140, 60)
#define UI_TRACK_6     UI_RGB565(140, 220, 80)
#define UI_TRACK_7     UI_RGB565(200, 100, 220)

// ============================================================
//  Drawing primitives
// ============================================================

// Set a single pixel at (x,y) to the given RGB565 color.
void ui_set_pixel(ui_fb_t* fb, int x, int y, uint16_t color);
// Fill a rectangular area with a solid color.
void ui_fill_rect(ui_fb_t* fb, int x, int y, int w, int h, uint16_t color);
// Draw a 1-pixel-wide rectangular outline.
void ui_draw_rect(ui_fb_t* fb, int x, int y, int w, int h, uint16_t color);
// Fill the entire framebuffer with a single color.
void ui_fill_screen(ui_fb_t* fb, uint16_t color);

// ============================================================
//  Text (5x7 bitmap font)
// ============================================================

// Draw a single 5x7 bitmap character with foreground/background colors.
void ui_draw_char(ui_fb_t* fb, int x, int y, char c, uint16_t fg, uint16_t bg);
// Draw a null-terminated string using the 5x7 font (6px per glyph).
void ui_draw_text(ui_fb_t* fb, int x, int y, const char* text, uint16_t fg, uint16_t bg);
// Return the pixel width of a string when rendered (strlen * 6).
int ui_text_width(const char* text);

// ============================================================
//  Grid helpers
// ============================================================

// Compute pixel coordinates for a grid cell (col,row) within a cols x rows layout.
void ui_grid_cell(int* ox, int* oy, int* ow, int* oh,
                  int col, int row, int cols, int rows,
                  int fb_w, int fb_h, int padding);

#ifdef __cplusplus
}
#endif
