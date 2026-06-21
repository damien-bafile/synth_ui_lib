#pragma once

namespace ui {

struct Rect {
    int x;
    int y;
    int w;
    int h;

    bool contains(int px, int py) const {
        return px >= x && px < x + w && py >= y && py < y + h;
    }
};

} // namespace ui
