#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"
#include "touch.h"

namespace ui {

// Circular parameter dial with a solid filled arc, step ticks, and a
// large centered value. Value is changed by dragging up/down; a short
// tap toggles an active/on state.
class RadialDial {
public:
    enum class TickDirection : uint8_t {
        Off,
        Outward,
        Inward,
        Both
    };

    enum LabelPos : uint8_t {
        LABEL_NONE,
        LABEL_BELOW,
        LABEL_ABOVE,
        LABEL_RIGHT,
        LABEL_LEFT,
    };

    // steps <= 1 disables snapping (continuous).
    RadialDial(int x, int y, int radius,
               float min, float max, float value, int steps,
               uint16_t fg = ACCENT_1,
               uint16_t bg = BG_DARK,
               uint16_t track = BG_MID) noexcept;

    void draw(Framebuffer& fb, float value, bool active = true) const;

    // Call every frame with current touch state.
    // outValue  - snapped current value
    // outToggled - true if a tap occurred (caller should flip active state)
    // Returns true if value changed or a toggle was triggered.
    bool handleTouch(const TouchState& touch, float& outValue, bool& outToggled);

    void setRange(float min, float max) noexcept { min_ = min; max_ = max; }
    void setSteps(int steps) noexcept { steps_ = steps > 1 ? steps : 1; }
    void setColors(uint16_t fg, uint16_t bg, uint16_t track) noexcept;
    void setValue(float value) noexcept { value_ = snap(ui::clamp(value, min_, max_)); }

    // Geometry / styling configuration
    void setArcThickness(int thickness) noexcept;
    void setTickLength(int length) noexcept;
    void setTickDirection(TickDirection dir) noexcept;
    void setFontScale(int scale) noexcept;
    void setAngles(float startAngle, float sweep) noexcept;

    int getArcThickness() const noexcept { return arcThickness_; }
    int getTickLength() const noexcept { return tickLength_; }
    TickDirection getTickDirection() const noexcept { return tickDirection_; }
    int getFontScale() const noexcept { return fontScale_; }
    float getStartAngle() const noexcept { return startAngle_; }
    float getSweep() const noexcept { return sweep_; }

    // Label / display helpers
    void setLabel(const char* label, uint16_t color = TEXT_DIM,
                  LabelPos pos = LABEL_BELOW, int offset = 2) noexcept;
    void clearLabel() noexcept;
    void setDisplaySuffix(const char* suffix) noexcept { suffix_ = suffix; }
    void setShowSign(bool show) noexcept { showSign_ = show; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getRadius() const noexcept { return radius_; }
    float getValue() const noexcept { return value_; }
    float getMin() const noexcept { return min_; }
    float getMax() const noexcept { return max_; }
    int getSteps() const noexcept { return steps_; }

private:
    int x_, y_, radius_;
    float min_, max_, value_;
    int steps_;
    uint16_t fg_, bg_, track_;

    // Geometry / styling (all user-configurable)
    int arcThickness_;
    int tickLength_;
    TickDirection tickDirection_;
    int fontScale_;
    float startAngle_;
    float sweep_;

    // Label
    const char* label_ = nullptr;
    uint16_t labelColor_ = TEXT_DIM;
    LabelPos labelPos_ = LABEL_NONE;
    int labelOffset_ = 2;

    // Display
    const char* suffix_ = nullptr;
    bool showSign_ = false;

    // Touch tracking
    bool wasPressed_;
    bool isDragging_;
    int touchStartX_, touchStartY_;
    int lastX_, lastY_;

    // Constants
    static constexpr float kPi = 3.14159265f;
    static constexpr int kDragThresholdPx = 4;

    float snap(float v) const;
    float valueToAngle(float value) const;
    bool isInside(int x, int y) const;

    void drawArc(Framebuffer& fb, float startAngle, float endAngle,
                 int innerR, int outerR, uint16_t color) const;
    void drawTicks(Framebuffer& fb) const;
    void drawCenterValue(Framebuffer& fb, float value, bool active) const;

    static void drawScaledChar(Framebuffer& fb, int x, int y, char c,
                               uint16_t fg, uint16_t bg, int scale);
    static void drawScaledText(Framebuffer& fb, int x, int y, const char* text,
                               uint16_t fg, uint16_t bg, int scale);
};

} // namespace ui
