#include "meter_array.h"
#include <algorithm>
#include <cmath>

namespace ui {

MeterArray::MeterArray(int x, int y, int width, int height, int numChannels,
                       MeterStyle style, uint16_t fgColor, uint16_t bgColor) noexcept
    : x_(x), y_(y), width_(width), height_(height), numChannels_(numChannels),
      style_(style), fgColor_(fgColor), bgColor_(bgColor) {}

void MeterArray::draw(Framebuffer& fb, const float* levels,
                      const float* peakLevels, bool showLabels) const {
  // Draw background
  fb.fillRect(x_, y_, width_, height_, bgColor_);
  fb.drawRect(x_, y_, width_, height_, fgColor_);

  switch (style_) {
  case MeterStyle::VERTICAL:
    drawVerticalMeters(fb, levels, peakLevels, showLabels);
    break;
  case MeterStyle::HORIZONTAL:
    drawHorizontalMeters(fb, levels, peakLevels, showLabels);
    break;
  case MeterStyle::CIRCULAR:
    drawCircularMeters(fb, levels, peakLevels, showLabels);
    break;
  }
}

uint16_t MeterArray::getLevelColor(float level) {
  // Green -> Yellow -> Red gradient
  if (level < 0.5f) {
    return TEXT;
  } else if (level < 0.8f) {
    return WARN;
  } else {
    return HIGHLIGHT;
  }
}

void MeterArray::drawVerticalMeters(Framebuffer& fb, const float* levels,
                                    const float* peakLevels,
                                    bool showLabels) const {
  if (numChannels_ < 1)
    return;

  int meterWidth = (width_ - 2 - (numChannels_ - 1)) / numChannels_;
  if (meterWidth < 4)
    meterWidth = 4;

  int labelHeight = showLabels ? 10 : 0;
  int meterHeight = height_ - 2 - labelHeight;

  for (int ch = 0; ch < numChannels_; ch++) {
    int meterX = x_ + 1 + ch * (meterWidth + 1);
    int meterY = y_ + 1;

    float level = clamp(levels[ch], 0.0f, 1.0f);

    // Draw meter background
    fb.fillRect(meterX, meterY, meterWidth, meterHeight, BG_DARK);

    // Draw level fill
    int fillHeight = (int)(level * meterHeight);
    int fillY = meterY + meterHeight - fillHeight;
    uint16_t levelColor = getLevelColor(level);
    fb.fillRect(meterX, fillY, meterWidth, fillHeight, levelColor);

    // Draw peak indicator if provided
    if (peakLevels) {
      float peakLevel = clamp(peakLevels[ch], 0.0f, 1.0f);
      int peakY = meterY + meterHeight - (int)(peakLevel * meterHeight);
      fb.drawLine(meterX, peakY, meterX + meterWidth - 1, peakY, ACCENT_1);
    }
  }
}

void MeterArray::drawHorizontalMeters(Framebuffer& fb, const float* levels,
                                      const float* peakLevels,
                                      bool showLabels) const {
  if (numChannels_ < 1)
    return;

  int meterHeight = (height_ - 2 - (numChannels_ - 1)) / numChannels_;
  if (meterHeight < 4)
    meterHeight = 4;

  int labelWidth = showLabels ? 16 : 0;
  int meterWidth = width_ - 2 - labelWidth;

  for (int ch = 0; ch < numChannels_; ch++) {
    int meterX = x_ + 1 + labelWidth;
    int meterY = y_ + 1 + ch * (meterHeight + 1);

    float level = clamp(levels[ch], 0.0f, 1.0f);

    // Draw meter background
    fb.fillRect(meterX, meterY, meterWidth, meterHeight, BG_DARK);

    // Draw level fill
    int fillWidth = (int)(level * meterWidth);
    uint16_t levelColor = getLevelColor(level);
    fb.fillRect(meterX, meterY, fillWidth, meterHeight, levelColor);

    // Draw peak indicator if provided
    if (peakLevels) {
      float peakLevel = clamp(peakLevels[ch], 0.0f, 1.0f);
      int peakX = meterX + (int)(peakLevel * meterWidth);
      fb.drawLine(peakX, meterY, peakX, meterY + meterHeight - 1, ACCENT_1);
    }
  }
}
void MeterArray::drawCircularMeters(Framebuffer& fb, const float* levels,
                                    const float* /*peakLevels*/,
                                    bool /*showLabels*/) const {
  if (numChannels_ < 1)
    return;

  // Calculate layout for circular gauges
  int gaugeRadius = std::min((width_ - 4) / (numChannels_ * 2), (height_ - 4) / 2);
  if (gaugeRadius < 8)
    gaugeRadius = 8;

  for (int ch = 0; ch < numChannels_; ch++) {
    int centerX = x_ + 2 + gaugeRadius + ch * (gaugeRadius * 2 + 2);
    int centerY = y_ + height_ / 2;

    float level = clamp(levels[ch], 0.0f, 1.0f);

    // Draw gauge background circle
    fb.drawCircle(centerX, centerY, gaugeRadius, BG_MID);

    // Draw level arc (simplified: just draw a partial circle/line)
    uint16_t levelColor = getLevelColor(level);
    int arcRadius = (int)(level * gaugeRadius);
    if (arcRadius > 0) {
      fb.drawCircle(centerX, centerY, arcRadius, levelColor);
    }
  }
}

} // namespace ui
