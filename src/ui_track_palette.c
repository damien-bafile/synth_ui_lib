#include "ui_track_palette.h"

uint16_t ui_track_color(int idx) {
    switch (idx) {
        case 0: return UI_TRACK_0;
        case 1: return UI_TRACK_1;
        case 2: return UI_TRACK_2;
        case 3: return UI_TRACK_3;
        case 4: return UI_TRACK_4;
        case 5: return UI_TRACK_5;
        case 6: return UI_TRACK_6;
        case 7: return UI_TRACK_7;
        default: return UI_TEXT_DIM;
    }
}
