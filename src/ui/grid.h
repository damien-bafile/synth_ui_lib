#pragma once
#include <cstdint>

namespace ui {

struct GridCell {
    int x;
    int y;
    int w;
    int h;
};

class Grid {
public:
    static GridCell cell(int ox, int oy, int ow, int oh,
                         int col, int row, int cols, int rows,
                         int fbW, int fbH, int padding) {
        int colW = fbW / cols;
        int rowH = fbH / rows;
        return {
            col * colW + padding,
            row * rowH + padding,
            colW - padding * 2,
            rowH - padding * 2
        };
    }
};

} // namespace ui
