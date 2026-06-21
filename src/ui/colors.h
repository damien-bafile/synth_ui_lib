#pragma once
#include <cstdint>

namespace ui {

constexpr uint16_t RGB565(uint8_t r, uint8_t g, uint8_t b) {
    return (static_cast<uint16_t>(r & 0xF8) << 8) |
           (static_cast<uint16_t>(g & 0xFC) << 3) |
           (static_cast<uint8_t>(b) >> 3);
}

constexpr float clamp(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

// Base
constexpr uint16_t BLACK      = RGB565(0,   0,   0);
constexpr uint16_t WHITE      = RGB565(255, 255, 255);
constexpr uint16_t RED        = RGB565(255, 0,   0);
constexpr uint16_t GREEN      = RGB565(0,   255, 0);
constexpr uint16_t BLUE       = RGB565(0,   0,   255);
constexpr uint16_t YELLOW     = RGB565(255, 255, 0);
constexpr uint16_t CYAN       = RGB565(0,   255, 255);
constexpr uint16_t MAGENTA    = RGB565(255, 0,   255);

// Shades
constexpr uint16_t GRAY_DARK   = RGB565(40,  40,  40);
constexpr uint16_t GRAY_MID    = RGB565(100, 100, 100);
constexpr uint16_t GRAY_LIGHT  = RGB565(180, 180, 180);
constexpr uint16_t ORANGE      = RGB565(255, 140, 0);
constexpr uint16_t PURPLE      = RGB565(140, 0,   255);
constexpr uint16_t TEAL        = RGB565(0,   180, 180);
constexpr uint16_t PINK        = RGB565(255, 100, 180);
constexpr uint16_t LIME        = RGB565(140, 255, 0);
constexpr uint16_t NAVY        = RGB565(0,   0,   120);
constexpr uint16_t MAROON      = RGB565(120, 0,   0);
constexpr uint16_t OLIVE       = RGB565(100, 120, 0);
constexpr uint16_t SILVER      = RGB565(200, 200, 210);
constexpr uint16_t GOLD        = RGB565(255, 200, 40);
constexpr uint16_t CORAL       = RGB565(255, 80,  80);

// UI Theme (Catppuccin Mocha)
constexpr uint16_t BG_DARK     = RGB565(17,  17,  27);   // crust
constexpr uint16_t BG_MID      = RGB565(24,  24,  37);   // mantle
constexpr uint16_t BG_SURFACE  = RGB565(30,  30,  46);   // base
constexpr uint16_t TEXT        = RGB565(205, 214, 244);  // text
constexpr uint16_t TEXT_DIM    = RGB565(127, 132, 156);  // overlay1
constexpr uint16_t ACCENT_1    = RGB565(243, 139, 168);  // red
constexpr uint16_t ACCENT_2    = RGB565(116, 199, 236);  // sapphire
constexpr uint16_t ACCENT_3    = RGB565(203, 166, 247);  // mauve
constexpr uint16_t ACCENT_4    = RGB565(166, 227, 161);  // green
constexpr uint16_t HIGHLIGHT   = RGB565(249, 226, 175);  // yellow
constexpr uint16_t HEADING     = RGB565(249, 226, 175);  // yellow
constexpr uint16_t WARN        = RGB565(235, 160, 172);  // maroon
constexpr uint16_t RECORD      = RGB565(243, 139, 168);  // red
constexpr uint16_t PLAY        = RGB565(166, 227, 161);  // green
constexpr uint16_t MUTE        = RGB565(250, 179, 135);  // peach
constexpr uint16_t SOLO        = RGB565(249, 226, 175);  // yellow

// Track colors (8 Catppuccin Mocha accent colors)
constexpr uint16_t TRACK_0     = RGB565(243, 139, 168);  // red
constexpr uint16_t TRACK_1     = RGB565(250, 179, 135);  // peach
constexpr uint16_t TRACK_2     = RGB565(249, 226, 175);  // yellow
constexpr uint16_t TRACK_3     = RGB565(166, 227, 161);  // green
constexpr uint16_t TRACK_4     = RGB565(148, 226, 213);  // teal
constexpr uint16_t TRACK_5     = RGB565(137, 220, 235);  // sky
constexpr uint16_t TRACK_6     = RGB565(203, 166, 247);  // mauve
constexpr uint16_t TRACK_7     = RGB565(245, 194, 231);  // pink

} // namespace ui
