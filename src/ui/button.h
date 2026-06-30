#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"

namespace ui {

class Button : public Widget {
public:
    Button(int x, int y, int w, int h, const char* label,
           uint16_t fg = TEXT, uint16_t bg = BG_SURFACE);

    void draw(Framebuffer& fb, bool pressed);

    // Returns true once per tap, then clears
    bool wasTapped() noexcept { bool v = wasTapped_; wasTapped_ = false; return v; }

    void setLabel(const char* label) noexcept { label_ = label; }
    void setPosition(int x, int y) noexcept { setBounds(x, y, w_, h_); }
    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }
    const char* getLabel() const noexcept { return label_; }

private:
    const char* label_;
    uint16_t fg_, bg_;
    bool wasTapped_ = false;

    void onTap(const TouchEvent& event) override;
};

} // namespace ui
