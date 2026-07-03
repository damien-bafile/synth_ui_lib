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

// ── Runtime theme palette ───────────────────────────────────────────
// Mutable so the theme can switch at runtime; constant-initialized to
// Catppuccin Mocha (dark). setTheme(Theme::LATTE) swaps in Catppuccin
// Latte (light). Widgets that copy these values at construction should
// re-apply them at draw time to pick up a theme change.
enum class Theme : uint8_t { MOCHA, LATTE };
void setTheme(Theme t);
Theme currentTheme();

extern uint16_t GRAY_DARK;    // surface1
extern uint16_t GRAY_MID;     // overlay0
extern uint16_t GRAY_LIGHT;   // subtext0
extern uint16_t LAVENDER;
extern uint16_t BG_DARK;      // crust
extern uint16_t BG_MID;       // mantle
extern uint16_t BG_SURFACE;   // base
extern uint16_t TEXT;
extern uint16_t TEXT_DIM;     // overlay1
extern uint16_t ACCENT_1;     // red
extern uint16_t ACCENT_2;     // sapphire
extern uint16_t ACCENT_3;     // mauve
extern uint16_t ACCENT_4;     // green
extern uint16_t HIGHLIGHT;    // yellow
extern uint16_t HEADING;      // yellow
extern uint16_t WARN;         // maroon
extern uint16_t RECORD;       // red
extern uint16_t PLAY;         // green
extern uint16_t MUTE;         // peach
extern uint16_t SOLO;         // yellow

// Track colors (8 accent colors of the active theme)
extern uint16_t TRACK_0, TRACK_1, TRACK_2, TRACK_3;
extern uint16_t TRACK_4, TRACK_5, TRACK_6, TRACK_7;
extern uint16_t TRACK_COLORS[8];

} // namespace ui
