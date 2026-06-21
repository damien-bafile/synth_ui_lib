#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "touch.h"
#include "colors.h"

namespace ui {

class Radio {
public:
    static constexpr int DEFAULT_RADIUS = 7;

    Radio(int x, int y, uint16_t fg = TEXT, uint16_t bg = BG_DARK);

    void draw(Framebuffer& fb, bool selected, const char* label);
    bool handleTouch(const TouchState& touch);

    static void drawGroup(Framebuffer& fb, int x, int y,
                          const char* labels[], int count, int selected,
                          uint16_t fg, uint16_t bg);

    void setPosition(int x, int y) noexcept { x_ = x; y_ = y; }
    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getRadius() const noexcept { return radius_; }

private:
    int x_, y_;
    int radius_;
    uint16_t fg_, bg_;
};

} // namespace ui
