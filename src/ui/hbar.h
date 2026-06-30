#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"

namespace ui {

class HorizontalBar : public Widget {
public:
    HorizontalBar(int x, int y, int w, int h,
                  uint16_t fg = ACCENT_1, uint16_t bg = BG_DARK);

    void draw(Framebuffer& fb, float fraction);

    float getFraction() const noexcept { return fraction_; }

    static void drawCenteredCents(Framebuffer& fb, int x, int y, int w, int h,
                                  float valCents, float maxCents,
                                  uint16_t fill, uint16_t bg);

    void setPosition(int x, int y) noexcept { setBounds(x, y, w_, h_); }
    void setSize(int w, int h) noexcept { setBounds(x_, y_, w, h); }
    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }

private:
    uint16_t fg_, bg_;
    float fraction_ = 0.0f;

    bool onTouchBegan(const TouchEvent& event) override;
    void onDragMoved(const TouchEvent& event, int dx, int dy) override;
};

} // namespace ui
