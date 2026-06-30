#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"

namespace ui {

class Radio : public Widget {
public:
    static constexpr int DEFAULT_RADIUS = 7;

    Radio(int x, int y, uint16_t fg = TEXT, uint16_t bg = BG_DARK);

    void draw(Framebuffer& fb, bool selected, const char* label);
    static void drawGroup(Framebuffer& fb, int x, int y,
                          const char* labels[], int count, int selected,
                          uint16_t fg, uint16_t bg);

    bool wasTapped() noexcept { bool v = wasTapped_; wasTapped_ = false; return v; }

    void setPosition(int x, int y) noexcept;
    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getRadius() const noexcept { return radius_; }

private:
    int radius_;
    uint16_t fg_, bg_;
    bool wasTapped_ = false;

    bool onTouchBegan(const TouchEvent& event) override;
    void onTap(const TouchEvent& event) override;

    bool isInside(int px, int py) const noexcept;
};

} // namespace ui
