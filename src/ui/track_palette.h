#pragma once
#include <cstdint>
#include "colors.h"

namespace ui {

class TrackPalette {
public:
    static uint16_t color(int index) {
        switch (index) {
            case 0: return TRACK_0;
            case 1: return TRACK_1;
            case 2: return TRACK_2;
            case 3: return TRACK_3;
            case 4: return TRACK_4;
            case 5: return TRACK_5;
            case 6: return TRACK_6;
            case 7: return TRACK_7;
            default: return TEXT_DIM;
        }
    }
};

} // namespace ui
