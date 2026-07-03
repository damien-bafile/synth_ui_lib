#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "slider.h"
#include "button.h"
#include "touch_dispatcher.h"
#include "colors.h"

namespace ui {

// A small hovering window with one drag slider, a value readout and an X
// close button. Generic: construct with bounds and a range, toggle it open,
// draw it with the model's current value each frame, then poll
// valueChanged() and read value(). The X button closes it automatically.
class PopupSlider {
public:
    PopupSlider(int x, int y, int w, int h, float min, float max,
                const char* label = nullptr, uint16_t accent = ACCENT_2);

    void open() noexcept { open_ = true; }
    void close() noexcept { open_ = false; }
    void toggle() noexcept { open_ = !open_; }
    bool isOpen() const noexcept { return open_; }

    // Draws panel + slider + readout + X (no-op while closed).
    // `valueText` overrides the default integer readout.
    void draw(Framebuffer& fb, float value, const char* valueText = nullptr);

    // Registers the popup's widgets while open; call after draw each frame.
    void addTo(TouchDispatcher& disp);

    // Poll once per frame: handles the X button (closes the popup) and the
    // -/+ step buttons; returns true if the user changed the value.
    bool valueChanged();
    float value() const noexcept { return slider_.getValue(); }

    // Show -/+ buttons right of the readout; each tap nudges the value by
    // ±step (clamped to the range). 0 hides them (default).
    void setStepButtons(float step) noexcept { step_ = step; }

private:
    int x_, y_, w_, h_;
    const char* label_;
    uint16_t accent_;
    float min_, max_;
    float step_ = 0.0f;
    float lastValue_ = 0.0f;  // model value from the latest draw()
    Slider slider_;
    Button minusBtn_, plusBtn_;
    Button closeBtn_;
    bool open_ = false;
};

} // namespace ui
