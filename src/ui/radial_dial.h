#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"

namespace ui {

class RadialDial : public Widget {
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

    RadialDial(int x, int y, int radius,
               float min, float max, float value, int steps,
               uint16_t fg = ACCENT_1,
               uint16_t bg = BG_DARK,
               uint16_t track = BG_MID) noexcept;

    void draw(Framebuffer& fb, float value, bool active = true) const;
    void setBounds(int x, int y, int w, int h) noexcept;

    float getValue() const noexcept { return value_; }
    bool wasToggled() noexcept { bool v = wasToggled_; wasToggled_ = false; return v; }

    void setRange(float min, float max) noexcept { min_ = min; max_ = max; }
    void setSteps(int steps) noexcept { steps_ = steps > 1 ? steps : 1; }
    void setColors(uint16_t fg, uint16_t bg, uint16_t track) noexcept;
    void setValue(float value) noexcept { value_ = snap(ui::clamp(value, min_, max_)); }

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

    void setLabel(const char* label, uint16_t color = TEXT_DIM,
                  LabelPos pos = LABEL_BELOW, int offset = 2) noexcept;
    void clearLabel() noexcept;
    void setDisplaySuffix(const char* suffix) noexcept { suffix_ = suffix; }
    void setShowSign(bool show) noexcept { showSign_ = show; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getRadius() const noexcept { return radius_; }
    float getMin() const noexcept { return min_; }
    float getMax() const noexcept { return max_; }
    int getSteps() const noexcept { return steps_; }

private:
    int radius_;
    float min_, max_, value_;
    int steps_;
    uint16_t fg_, bg_, track_;

    int arcThickness_;
    int tickLength_;
    TickDirection tickDirection_;
    int fontScale_;
    float startAngle_;
    float sweep_;

    const char* label_ = nullptr;
    uint16_t labelColor_ = TEXT_DIM;
    LabelPos labelPos_ = LABEL_NONE;
    int labelOffset_ = 2;

    const char* suffix_ = nullptr;
    bool showSign_ = false;

    bool wasToggled_ = false;
    float dragStartValue_ = 0.0f;
    int dragStartY_ = 0;

    static constexpr float kPi = 3.14159265f;

    float snap(float v) const;
    float valueToAngle(float value) const;
    bool isInside(int x, int y) const;

    bool onTouchBegan(const TouchEvent& event) override;
    void onDragMoved(const TouchEvent& event, int dx, int dy) override;
    void onTap(const TouchEvent& event) override;
    void onDragEnded(const TouchEvent& event) override;

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
