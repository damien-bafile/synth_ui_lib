#include "colors.h"

namespace ui {

// Active palette, constant-initialized to Catppuccin Mocha (dark).
uint16_t GRAY_DARK   = RGB565(69,  71,  90);   // surface1
uint16_t GRAY_MID    = RGB565(108, 112, 134);  // overlay0
uint16_t GRAY_LIGHT  = RGB565(166, 173, 200);  // subtext0
uint16_t LAVENDER    = RGB565(180, 190, 254);
uint16_t BG_DARK     = RGB565(17,  17,  27);   // crust
uint16_t BG_MID      = RGB565(24,  24,  37);   // mantle
uint16_t BG_SURFACE  = RGB565(30,  30,  46);   // base
uint16_t TEXT        = RGB565(205, 214, 244);
uint16_t TEXT_DIM    = RGB565(127, 132, 156);  // overlay1
uint16_t ACCENT_1    = RGB565(243, 139, 168);  // red
uint16_t ACCENT_2    = RGB565(116, 199, 236);  // sapphire
uint16_t ACCENT_3    = RGB565(203, 166, 247);  // mauve
uint16_t ACCENT_4    = RGB565(166, 227, 161);  // green
uint16_t HIGHLIGHT   = RGB565(249, 226, 175);  // yellow
uint16_t HEADING     = RGB565(249, 226, 175);  // yellow
uint16_t WARN        = RGB565(235, 160, 172);  // maroon
uint16_t RECORD      = RGB565(243, 139, 168);  // red
uint16_t PLAY        = RGB565(166, 227, 161);  // green
uint16_t MUTE        = RGB565(250, 179, 135);  // peach
uint16_t SOLO        = RGB565(249, 226, 175);  // yellow
uint16_t TRACK_0     = RGB565(243, 139, 168);  // red
uint16_t TRACK_1     = RGB565(250, 179, 135);  // peach
uint16_t TRACK_2     = RGB565(249, 226, 175);  // yellow
uint16_t TRACK_3     = RGB565(166, 227, 161);  // green
uint16_t TRACK_4     = RGB565(148, 226, 213);  // teal
uint16_t TRACK_5     = RGB565(137, 220, 235);  // sky
uint16_t TRACK_6     = RGB565(203, 166, 247);  // mauve
uint16_t TRACK_7     = RGB565(245, 194, 231);  // pink
uint16_t TRACK_COLORS[8] = {
    TRACK_0, TRACK_1, TRACK_2, TRACK_3,
    TRACK_4, TRACK_5, TRACK_6, TRACK_7
};

static Theme g_theme = Theme::MOCHA;

struct Palette {
    uint16_t gray_dark, gray_mid, gray_light, lavender;
    uint16_t bg_dark, bg_mid, bg_surface, text, text_dim;
    uint16_t accent1, accent2, accent3, accent4;
    uint16_t highlight, heading, warn, record, play, mute, solo;
    uint16_t tracks[8];
};

static const Palette k_mocha = {
    RGB565(69, 71, 90), RGB565(108, 112, 134), RGB565(166, 173, 200),
    RGB565(180, 190, 254),
    RGB565(17, 17, 27), RGB565(24, 24, 37), RGB565(30, 30, 46),
    RGB565(205, 214, 244), RGB565(127, 132, 156),
    RGB565(243, 139, 168), RGB565(116, 199, 236),
    RGB565(203, 166, 247), RGB565(166, 227, 161),
    RGB565(249, 226, 175), RGB565(249, 226, 175), RGB565(235, 160, 172),
    RGB565(243, 139, 168), RGB565(166, 227, 161), RGB565(250, 179, 135),
    RGB565(249, 226, 175),
    { RGB565(243, 139, 168), RGB565(250, 179, 135), RGB565(249, 226, 175),
      RGB565(166, 227, 161), RGB565(148, 226, 213), RGB565(137, 220, 235),
      RGB565(203, 166, 247), RGB565(245, 194, 231) },
};

// Catppuccin Latte: light backgrounds, dark text, Latte accent variants.
static const Palette k_latte = {
    RGB565(188, 192, 204),  // surface1
    RGB565(156, 160, 176),  // overlay0
    RGB565(108, 111, 133),  // subtext0
    RGB565(114, 135, 253),  // lavender
    RGB565(220, 224, 232),  // crust
    RGB565(230, 233, 239),  // mantle
    RGB565(239, 241, 245),  // base
    RGB565(76,  79,  105),  // text
    RGB565(140, 143, 161),  // overlay1
    RGB565(210, 15,  57),   // red
    RGB565(32,  159, 181),  // sapphire
    RGB565(136, 57,  239),  // mauve
    RGB565(64,  160, 43),   // green
    RGB565(223, 142, 29),   // yellow
    RGB565(223, 142, 29),   // yellow
    RGB565(230, 69,  83),   // maroon
    RGB565(210, 15,  57),   // red
    RGB565(64,  160, 43),   // green
    RGB565(254, 100, 11),   // peach
    RGB565(223, 142, 29),   // yellow
    { RGB565(210, 15, 57),  RGB565(254, 100, 11), RGB565(223, 142, 29),
      RGB565(64, 160, 43),  RGB565(23, 146, 153), RGB565(4, 165, 229),
      RGB565(136, 57, 239), RGB565(234, 118, 203) },
};

void setTheme(Theme t) {
    const Palette& p = (t == Theme::LATTE) ? k_latte : k_mocha;
    g_theme = t;

    GRAY_DARK = p.gray_dark;  GRAY_MID = p.gray_mid;
    GRAY_LIGHT = p.gray_light; LAVENDER = p.lavender;
    BG_DARK = p.bg_dark;      BG_MID = p.bg_mid;
    BG_SURFACE = p.bg_surface;
    TEXT = p.text;            TEXT_DIM = p.text_dim;
    ACCENT_1 = p.accent1;     ACCENT_2 = p.accent2;
    ACCENT_3 = p.accent3;     ACCENT_4 = p.accent4;
    HIGHLIGHT = p.highlight;  HEADING = p.heading;
    WARN = p.warn;            RECORD = p.record;
    PLAY = p.play;            MUTE = p.mute;
    SOLO = p.solo;
    TRACK_0 = p.tracks[0]; TRACK_1 = p.tracks[1];
    TRACK_2 = p.tracks[2]; TRACK_3 = p.tracks[3];
    TRACK_4 = p.tracks[4]; TRACK_5 = p.tracks[5];
    TRACK_6 = p.tracks[6]; TRACK_7 = p.tracks[7];
    for (int i = 0; i < 8; i++) TRACK_COLORS[i] = p.tracks[i];
}

Theme currentTheme() {
    return g_theme;
}

} // namespace ui
