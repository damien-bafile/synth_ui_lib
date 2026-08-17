#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"

namespace ui {

// Stepped numeric field: "[-] 12.50 [+]" with tappable minus/plus zones.
// Hit-rects are recomputed on every draw() call and used by onTap() --
// same "cache the rect from the last draw, hit-test against it later"
// technique as PageIndicator's prev_x0_/next_x0_.
class Spinbox : public Widget {
public:
    static constexpr int BTN_W = 12;

    Spinbox(int x, int y, int w, int h,
            float min, float max, float step, float value = 0.0f,
            uint16_t fg = TEXT, uint16_t bg = BG_DARK) noexcept;

    void draw(Framebuffer& fb, float value);

    float getValue() const noexcept { return value_; }
    void setValue(float v) noexcept;

    // Returns true once after the value changed via a tap, then clears.
    bool wasChanged() noexcept { bool v = changed_; changed_ = false; return v; }

    void setRange(float min, float max) noexcept { min_ = min; max_ = max; }
    void setStep(float step) noexcept { step_ = step; }
    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }

    float getMin() const noexcept { return min_; }
    float getMax() const noexcept { return max_; }
    float getStep() const noexcept { return step_; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }

private:
    float min_, max_, step_, value_;
    uint16_t fg_, bg_;
    bool changed_ = false;

    int minusX0_ = 0, minusX1_ = 0;
    int plusX0_ = 0, plusX1_ = 0;

    void onTap(const TouchEvent& event) override;
};

} // namespace ui
