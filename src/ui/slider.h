#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "touch_event.h"
#include "colors.h"

namespace ui {

class Slider : public Widget {
public:
    Slider(int x, int y, int w, int h, float min, float max,
           const char* label, uint16_t fg = ACCENT_1, uint16_t bg = BG_DARK,
           bool vertical = false);

    void draw(Framebuffer& fb, float value);

    float getValue() const noexcept { return value_; }

    // Returns true once after the user touched/dragged the slider, then clears
    bool wasChanged() noexcept { bool v = changed_; changed_ = false; return v; }

    void setLabel(const char* label) noexcept { label_ = label; }
    void setPosition(int x, int y) noexcept { setBounds(x, y, w_, h_); }
    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }
    void setRange(float min, float max) noexcept { min_ = min; max_ = max; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }
    const char* getLabel() const noexcept { return label_; }
    bool isVertical() const noexcept { return vertical_; }

private:
    float min_, max_;
    float value_;
    const char* label_;
    uint16_t fg_, bg_;
    bool vertical_;
    bool changed_ = false;

    void updateFromTouch(int px, int py);

    bool onTouchBegan(const TouchEvent& event) override;
    void onDragMoved(const TouchEvent& event, int dx, int dy) override;
};

} // namespace ui
