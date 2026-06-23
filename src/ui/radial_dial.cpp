#include "radial_dial.h"
#include "font.h"
#include <cmath>
#include <cstdio>
#include <cstring>

namespace ui {

RadialDial::RadialDial(int x, int y, int radius,
                       float min, float max, float value, int steps,
                       uint16_t fg, uint16_t bg, uint16_t track) noexcept
    : x_(x), y_(y), radius_(radius),
      min_(min), max_(max), value_(value),
      steps_(steps > 1 ? steps : 1),
      fg_(fg), bg_(bg), track_(track),
      arcThickness_(3),
      tickLength_(5),
      tickDirection_(TickDirection::Outward),
      fontScale_(1),
      startAngle_(135.0f),
      sweep_(270.0f),
      suffix_(nullptr), showSign_(false),
      wasPressed_(false), isDragging_(false),
      touchStartX_(0), touchStartY_(0), lastX_(0), lastY_(0) {}

void RadialDial::setColors(uint16_t fg, uint16_t bg, uint16_t track) noexcept {
    fg_ = fg;
    bg_ = bg;
    track_ = track;
}

void RadialDial::setArcThickness(int thickness) noexcept {
    arcThickness_ = thickness < 1 ? 1 : thickness;
    if (arcThickness_ >= radius_) arcThickness_ = radius_ - 1;
    if (arcThickness_ < 1) arcThickness_ = 1;
}

void RadialDial::setTickLength(int length) noexcept {
    tickLength_ = length < 0 ? 0 : length;
}

void RadialDial::setTickDirection(TickDirection dir) noexcept {
    tickDirection_ = dir;
}

void RadialDial::setFontScale(int scale) noexcept {
    fontScale_ = scale < 1 ? 1 : scale;
}

void RadialDial::setAngles(float startAngle, float sweep) noexcept {
    startAngle_ = startAngle;
    if (sweep > 360.0f) sweep = 360.0f;
    if (sweep < -360.0f) sweep = -360.0f;
    if (sweep == 0.0f) sweep = 1.0f;
    sweep_ = sweep;
}

void RadialDial::setLabel(const char* label, uint16_t color,
                          LabelPos pos, int offset) noexcept {
    label_ = label;
    labelColor_ = color;
    labelPos_ = pos;
    labelOffset_ = offset;
}

void RadialDial::clearLabel() noexcept {
    label_ = nullptr;
    labelPos_ = LABEL_NONE;
}

void RadialDial::draw(Framebuffer& fb, float value, bool active) const {
    value = ui::clamp(value, min_, max_);

    // Circular face
    fb.fillCircle(x_, y_, radius_, bg_);
    fb.drawCircle(x_, y_, radius_, track_);

    // Track arc (unfilled background)
    const float endAngle = startAngle_ + sweep_;
    const int innerR = radius_ - arcThickness_;
    drawArc(fb, startAngle_, endAngle, innerR, radius_, track_);

    // Filled value arc
    const float valueAngle = valueToAngle(value);
    uint16_t arcColor = active ? fg_ : track_;
    drawArc(fb, startAngle_, valueAngle, innerR, radius_, arcColor);

    // Step ticks
    drawTicks(fb);

    // Center value
    drawCenterValue(fb, value, active);

    // Optional label
    if (label_ && labelPos_ != LABEL_NONE) {
        int lx = x_, ly = y_;
        int tw = static_cast<int>(std::strlen(label_)) * FONT_W * fontScale_;
        switch (labelPos_) {
            case LABEL_BELOW:
                lx = x_ - tw / 2;
                ly = y_ + radius_ + labelOffset_;
                break;
            case LABEL_ABOVE:
                lx = x_ - tw / 2;
                ly = y_ - radius_ - labelOffset_ - FONT_H * fontScale_;
                break;
            case LABEL_RIGHT:
                lx = x_ + radius_ + labelOffset_;
                ly = y_ - (FONT_H * fontScale_) / 2;
                break;
            case LABEL_LEFT:
                lx = x_ - radius_ - labelOffset_ - tw;
                ly = y_ - (FONT_H * fontScale_) / 2;
                break;
            default: break;
        }
        drawScaledText(fb, lx, ly, label_, labelColor_, bg_, fontScale_);
    }
}

bool RadialDial::handleTouch(const TouchState& touch, float& outValue,
                             bool& outToggled) {
    outToggled = false;
    bool changed = false;

    if (touch.pressed) {
        if (!wasPressed_) {
            // Touch down
            wasPressed_ = true;
            touchStartX_ = touch.x;
            touchStartY_ = touch.y;
            lastX_ = touch.x;
            lastY_ = touch.y;
            isDragging_ = false;
        } else {
            // Touch continue
            int dx = touch.x - touchStartX_;
            int dy = touch.y - touchStartY_;
            if (!isDragging_ &&
                (std::abs(dx) > kDragThresholdPx ||
                 std::abs(dy) > kDragThresholdPx)) {
                isDragging_ = true;
            }

            if (isDragging_) {
                float range = max_ - min_;
                // Dragging the full diameter covers the full range.
                float sensitivity = (range > 0.0f)
                                        ? range / (2.0f * radius_)
                                        : 0.0f;
                float delta = -static_cast<float>(touch.y - lastY_) * sensitivity;
                float newValue = ui::clamp(value_ + delta, min_, max_);
                newValue = snap(newValue);
                if (newValue != value_) {
                    value_ = newValue;
                    changed = true;
                }
            }

            lastX_ = touch.x;
            lastY_ = touch.y;
        }
    } else {
        if (wasPressed_) {
            // Touch release
            int dx = lastX_ - touchStartX_;
            int dy = lastY_ - touchStartY_;
            if (!isDragging_ &&
                std::abs(dx) <= kDragThresholdPx &&
                std::abs(dy) <= kDragThresholdPx &&
                isInside(touchStartX_, touchStartY_)) {
                outToggled = true;
                changed = true;
            }
            wasPressed_ = false;
            isDragging_ = false;
        }
    }

    outValue = value_;
    return changed;
}

float RadialDial::snap(float v) const {
    if (steps_ <= 1) return v;
    float range = max_ - min_;
    if (range <= 0.0f) return min_;
    float stepSize = range / (steps_ - 1);
    int step = static_cast<int>(std::round((v - min_) / stepSize));
    if (step < 0) step = 0;
    if (step >= steps_) step = steps_ - 1;
    return min_ + step * stepSize;
}

float RadialDial::valueToAngle(float value) const {
    float range = max_ - min_;
    float frac = (range > 0.0f) ? (value - min_) / range : 0.0f;
    return startAngle_ + ui::clamp(frac, 0.0f, 1.0f) * sweep_;
}

bool RadialDial::isInside(int x, int y) const {
    int dx = x - x_;
    int dy = y - y_;
    return (dx * dx + dy * dy) <= (radius_ * radius_);
}

void RadialDial::drawArc(Framebuffer& fb, float startAngle, float endAngle,
                         int innerR, int outerR, uint16_t color) const {
    if (innerR < 0) innerR = 0;
    if (outerR <= innerR) return;

    float sweep = endAngle - startAngle;
    if (sweep == 0.0f) return;

    float direction = (sweep > 0.0f) ? 1.0f : -1.0f;
    int segments = static_cast<int>(std::ceil(std::abs(sweep)));

    for (int i = 0; i <= segments; ++i) {
        float a = startAngle + i * direction;
        // Clamp final iteration to the exact end angle to avoid over/under-draw.
        if (i == segments) a = endAngle;

        float rad = a * kPi / 180.0f;
        float c = std::cos(rad);
        float s = std::sin(rad);

        int xi = x_ + static_cast<int>(innerR * c);
        int yi = y_ + static_cast<int>(innerR * s);
        int xo = x_ + static_cast<int>(outerR * c);
        int yo = y_ + static_cast<int>(outerR * s);

        fb.drawLine(xi, yi, xo, yo, color);
    }
}

void RadialDial::drawTicks(Framebuffer& fb) const {
    if (tickDirection_ == TickDirection::Off || tickLength_ <= 0) return;
    if (steps_ <= 1 || steps_ > 64) return;

    float range = max_ - min_;
    if (range <= 0.0f) return;

    const int gap = 2;
    int innerR = 0;
    int outerR = 0;

    switch (tickDirection_) {
        case TickDirection::Outward:
            innerR = radius_ + gap;
            outerR = radius_ + gap + tickLength_;
            break;
        case TickDirection::Inward:
            outerR = radius_ - gap;
            innerR = radius_ - gap - tickLength_;
            if (innerR < 0) innerR = 0;
            break;
        case TickDirection::Both:
            innerR = radius_ - gap - tickLength_;
            if (innerR < 0) innerR = 0;
            outerR = radius_ + gap + tickLength_;
            break;
        default:
            return;
    }

    for (int i = 0; i < steps_; ++i) {
        float value = min_ + (i * range) / (steps_ - 1);
        float angle = valueToAngle(value);
        float rad = angle * kPi / 180.0f;
        float c = std::cos(rad);
        float s = std::sin(rad);

        int xi = x_ + static_cast<int>(innerR * c);
        int yi = y_ + static_cast<int>(innerR * s);
        int xo = x_ + static_cast<int>(outerR * c);
        int yo = y_ + static_cast<int>(outerR * s);

        fb.drawLine(xi, yi, xo, yo, track_);
    }
}

void RadialDial::drawCenterValue(Framebuffer& fb, float value, bool active) const {
    char buf[16];
    int pos = 0;
    float rounded = std::round(value);
    if (showSign_ && value > 0.0f) {
        buf[pos++] = '+';
    }
    if (std::abs(rounded - value) < 0.001f) {
        pos += std::snprintf(buf + pos, sizeof(buf) - pos, "%d", static_cast<int>(rounded));
    } else {
        pos += std::snprintf(buf + pos, sizeof(buf) - pos, "%.1f", value);
    }
    if (suffix_) {
        std::strncat(buf, suffix_, sizeof(buf) - std::strlen(buf) - 1);
    }

    int textW = static_cast<int>(std::strlen(buf)) * FONT_W * fontScale_;
    int textH = FONT_H * fontScale_;
    int tx = x_ - textW / 2;
    int ty = y_ - textH / 2;

    uint16_t fg = active ? fg_ : track_;
    drawScaledText(fb, tx, ty, buf, fg, bg_, fontScale_);
}

void RadialDial::drawScaledChar(Framebuffer& fb, int x, int y, char c,
                                uint16_t fg, uint16_t bg, int scale) {
    if (c < 32 || c > 127) c = '?';
    int glyph = c - 32;

    for (int col = 0; col < FONT_W; ++col) {
        unsigned char bits = font_5x7[glyph * FONT_W + col];
        for (int row = 0; row < FONT_H; ++row) {
            uint16_t color = (bits >> row) & 1 ? fg : bg;
            fb.fillRect(x + col * scale, y + row * scale, scale, scale, color);
        }
    }
}

void RadialDial::drawScaledText(Framebuffer& fb, int x, int y, const char* text,
                                uint16_t fg, uint16_t bg, int scale) {
    for (int i = 0; text[i]; ++i) {
        drawScaledChar(fb, x + i * FONT_W * scale, y, text[i], fg, bg, scale);
    }
}

} // namespace ui
