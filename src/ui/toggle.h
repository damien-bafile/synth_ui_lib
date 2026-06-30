#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"

namespace ui {

class Toggle : public Widget {
public:
    static constexpr int DEFAULT_W = 30;
    static constexpr int DEFAULT_H = 16;

    Toggle(int x, int y, int w = DEFAULT_W, int h = DEFAULT_H,
           uint16_t fg = ACCENT_2, uint16_t bg = BG_DARK);

    void draw(Framebuffer& fb, bool on);
    void drawWithLabel(Framebuffer& fb, bool on, const char* label,
                       uint16_t fg, uint16_t bg);

    bool wasTapped() noexcept { bool v = wasTapped_; wasTapped_ = false; return v; }

    void setPosition(int x, int y) noexcept { setBounds(x, y, w_, h_); }
    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }

private:
    uint16_t fg_, bg_;
    bool wasTapped_ = false;

    void onTap(const TouchEvent& event) override;
};

} // namespace ui
