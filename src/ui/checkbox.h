#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"

namespace ui {

class Checkbox : public Widget {
public:
    static constexpr int DEFAULT_SIZE = 12;

    Checkbox(int x, int y, uint16_t fg = TEXT, uint16_t bg = BG_DARK);

    void draw(Framebuffer& fb, bool checked, const char* label);

    bool wasTapped() noexcept { bool v = wasTapped_; wasTapped_ = false; return v; }

    void setPosition(int x, int y) noexcept { setBounds(x, y, size_, size_); }
    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getSize() const noexcept { return size_; }

private:
    int size_;
    uint16_t fg_, bg_;
    bool wasTapped_ = false;

    void onTap(const TouchEvent& event) override;
};

} // namespace ui
