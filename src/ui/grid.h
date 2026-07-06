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
    // Lay out cell (col,row) of a cols×rows grid within the origin rect
    // (ox,oy,ow,oh), inset by `padding` on every side.
    static GridCell cell(int ox, int oy, int ow, int oh,
                         int col, int row, int cols, int rows,
                         int padding) {
        int colW = ow / cols;
        int rowH = oh / rows;
        return {
            ox + col * colW + padding,
            oy + row * rowH + padding,
            colW - padding * 2,
            rowH - padding * 2
        };
    }
};

} // namespace ui
