#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"
#include "icons.h"

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

    // Optional icon. With a label the icon sits at the left edge and the
    // label follows left-aligned; alone it is centered. iconColor picks a
    // dedicated tint (0 = use fg/bg like the label).
    void setSynthIcon(SynthIcon icon, uint16_t iconColor = 0) noexcept {
        sicon_ = icon; ficon_ = FxIcon::ICON_COUNT; iconColor_ = iconColor;
    }
    void setFxIcon(FxIcon icon, uint16_t iconColor = 0) noexcept {
        ficon_ = icon; sicon_ = SynthIcon::NONE; iconColor_ = iconColor;
    }
    void clearIcon() noexcept {
        sicon_ = SynthIcon::NONE; ficon_ = FxIcon::ICON_COUNT;
    }

    // Skip the 1px outline (flat chip look).
    void setDrawBorder(bool on) noexcept { border_ = on; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }
    const char* getLabel() const noexcept { return label_; }

private:
    bool hasIcon() const noexcept {
        return sicon_ != SynthIcon::NONE || ficon_ != FxIcon::ICON_COUNT;
    }
    void drawIcon(Framebuffer& fb, int ix, int iy, uint16_t fg, uint16_t bg);

    const char* label_;
    uint16_t fg_, bg_;
    SynthIcon sicon_ = SynthIcon::NONE;
    FxIcon ficon_ = FxIcon::ICON_COUNT;
    uint16_t iconColor_ = 0;
    bool border_ = true;
    bool wasTapped_ = false;

    void onTap(const TouchEvent& event) override;
};

} // namespace ui
