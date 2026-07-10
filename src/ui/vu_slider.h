#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "touch_event.h"
#include "colors.h"

namespace ui {

// Slider with an integrated VU meter in the track background.
// The VU meter fills with ACCENT_4→WARN→HIGHLIGHT color gradient based on
// meterValue. A draggable thumb over the fill shows the slider (volume)
// setting — two independent values in one compact widget.
// Supports vertical (default) and horizontal orientations.
class VUSlider : public Widget {
public:
    VUSlider(int x, int y, int w, int h,
             uint16_t fg = ACCENT_1, uint16_t bg = BG_DARK,
             bool vertical = true);

    void draw(Framebuffer& fb, float sliderValue, float meterValue);

    float getValue() const noexcept { return value_; }

    void setValue(float v) noexcept {
        value_ = v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v);
    }

    bool wasChanged() noexcept { bool v = changed_; changed_ = false; return v; }

    void setColors(uint16_t fg, uint16_t bg) noexcept { fg_ = fg; bg_ = bg; }

    void setPosition(int x, int y) noexcept { setBounds(x, y, w_, h_); }

private:
    float value_ = 0.0f;
    uint16_t fg_, bg_;
    bool changed_ = false;
    bool vertical_;

    void updateFromTouch(int px, int py);

    bool onTouchBegan(const TouchEvent& event) override;
    void onDragMoved(const TouchEvent& event, int dx, int dy) override;
};

} // namespace ui
