#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"

namespace ui {

class TextField : public Widget {
public:
    TextField(int x, int y, int w, int h,
              uint16_t fg = TEXT, uint16_t bg = BG_DARK);

    void draw(Framebuffer& fb, const char* text, int cursorPos, bool focused);

    bool wasTapped() noexcept { bool v = wasTapped_; wasTapped_ = false; return v; }

    void setPosition(int x, int y) noexcept { setBounds(x, y, w_, h_); }
    void setSize(int w, int h) noexcept { setBounds(x_, y_, w, h); }
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
