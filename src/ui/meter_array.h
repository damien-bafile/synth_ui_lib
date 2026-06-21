#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"

namespace ui {

class MeterArray {
public:
  enum class MeterStyle : uint8_t { VERTICAL, HORIZONTAL, CIRCULAR };

  MeterArray(int x, int y, int width, int height, int numChannels = 2,
             MeterStyle style = MeterStyle::VERTICAL, uint16_t fgColor = ACCENT_1,
             uint16_t bgColor = BG_DARK) noexcept;

  void draw(Framebuffer& fb, const float* levels, const float* peakLevels = nullptr,
            bool showLabels = false) const;

  static uint16_t getLevelColor(float level);

private:
  int x_, y_, width_, height_;
  int numChannels_;
  MeterStyle style_;
  uint16_t fgColor_;
  uint16_t bgColor_;

  void drawVerticalMeters(Framebuffer& fb, const float* levels,
                          const float* peakLevels, bool showLabels) const;
  void drawHorizontalMeters(Framebuffer& fb, const float* levels,
                            const float* peakLevels, bool showLabels) const;
  void drawCircularMeters(Framebuffer& fb, const float* levels,
                          const float* peakLevels, bool showLabels) const;
};

} // namespace ui
