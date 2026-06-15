#pragma once
#include "ui.h"

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
//  Synth waveform / parameter icons (5x7 pixel glyphs)
// ---------------------------------------------------------------------------

typedef enum {
    ICON_SINE      = 0,
    ICON_SQUARE    = 1,
    ICON_TRIANGLE  = 2,
    ICON_SAW_RISE  = 3,
    ICON_SAW_FALL  = 4,
    ICON_PULSE     = 5,
    ICON_ADSR      = 6,
    ICON_LPF       = 7,
    ICON_HPF       = 8,
    ICON_BPF       = 9,
    ICON_NOTCH     = 10,
    ICON_VU_METER  = 11,
    ICON_ARPEGGIO  = 12,
    ICON_GLIDE     = 13,
    ICON_NOTE      = 14,
    ICON_PLAY      = 15,
    ICON_PAUSE     = 16,
    ICON_STOP      = 17,
    ICON_RECORD    = 18,
    ICON_COUNT
} SynthIcon;

static const unsigned char synth_glyphs[ICON_COUNT * 5] = {
    0x0C,0x06,0x18,0x30,0x18, /* SINE      */
    0x07,0x07,0x7F,0x70,0x70, /* SQUARE    */
    0x10,0x08,0x04,0x08,0x10, /* TRIANGLE  */
    0x10,0x08,0x04,0x02,0x1F, /* SAW_RISE  */
    0x1F,0x01,0x02,0x04,0x08, /* SAW_FALL  */
    0x0F,0x78,0x70,0x70,0x70, /* PULSE     */
    0x3E,0x02,0x0C,0x08,0x38, /* ADSR      */
    0x04,0x04,0x0C,0x18,0x10, /* LPF       */
    0x10,0x18,0x0C,0x04,0x04, /* HPF       */
    0x04,0x02,0x01,0x02,0x04, /* BPF       */
    0x01,0x02,0x04,0x02,0x01, /* NOTCH     */
    0x10,0x18,0x1C,0x1E,0x1F, /* VU_METER  */
    0x10,0x18,0x0C,0x06,0x02, /* ARPEGGIO  */
    0x10,0x08,0x04,0x02,0x01, /* GLIDE     */
    0x30,0x30,0x1F,0x01,0x02, /* NOTE      */
    0x00,0x7F,0x3E,0x1C,0x08, /* PLAY      */
    0x00,0x7F,0x00,0x7F,0x00, /* PAUSE     */
    0x00,0x7F,0x7F,0x7F,0x00, /* STOP      */
    0x3E,0x7F,0x7F,0x7F,0x3E, /* RECORD    */
};

// ---------------------------------------------------------------------------
//  FX effect icons (5x7 pixel glyphs)
// ---------------------------------------------------------------------------

typedef enum {
    FX_REVERB      =  0,
    FX_DELAY       =  1,
    FX_DISTORTION  =  2,
    FX_OVERDRIVE   =  3,
    FX_BITCRUSH    =  4,
    FX_CHORUS      =  5,
    FX_FLANGER     =  6,
    FX_PHASER      =  7,
    FX_COMPRESSOR  =  8,
    FX_LIMITER     =  9,
    FX_TREMOLO     = 10,
    FX_RINGMOD     = 11,
    FX_GATE        = 12,
    FX_EQ          = 13,
    FX_ICON_COUNT
} FxIcon;

static const unsigned char fx_glyphs[FX_ICON_COUNT * 5] = {
    0x3E,0x00,0x1C,0x00,0x08, /* REVERB     */
    0x3E,0x00,0x3E,0x00,0x3E, /* DELAY      */
    0x06,0x06,0x0C,0x30,0x30, /* DISTORTION */
    0x12,0x21,0x21,0x21,0x12, /* OVERDRIVE  */
    0x06,0x0E,0x18,0x38,0x60, /* BITCRUSH   */
    0x08,0x2A,0x2A,0x2A,0x08, /* CHORUS     */
    0x3F,0x20,0x3F,0x20,0x3F, /* FLANGER    */
    0x24,0x12,0x12,0x12,0x24, /* PHASER     */
    0x06,0x06,0x08,0x10,0x20, /* COMPRESSOR */
    0x1D,0x11,0x1D,0x11,0x1D, /* LIMITER    */
    0x04,0x0E,0x1F,0x0E,0x04, /* TREMOLO    */
    0x1C,0x36,0x22,0x36,0x1C, /* RINGMOD    */
    0x0E,0x0E,0x00,0x0E,0x0E, /* GATE       */
    0x0F,0x08,0x0C,0x08,0x0F, /* EQ         */
};

// Draw a 5x7 synth icon glyph at (x,y) with foreground/background colors.
void ui_draw_synth_icon(ui_fb_t* fb, int x, int y, SynthIcon icon,
                        uint16_t fg, uint16_t bg);

// Draw a 5x7 FX icon glyph at (x,y) with foreground/background colors.
void ui_draw_fx_icon(ui_fb_t* fb, int x, int y, FxIcon icon,
                     uint16_t fg, uint16_t bg);

#ifdef __cplusplus
}
#endif
