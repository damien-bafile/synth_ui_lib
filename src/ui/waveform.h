#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"

namespace ui {

class Waveform {
public:
  enum class WaveformType : uint8_t {
    SINE,
    SQUARE,
    TRIANGLE,
    SAWTOOTH,
    CUSTOM
  };

  Waveform(int x, int y, int width, int height,
           uint16_t fgColor = ACCENT_1,
           uint16_t bgColor = BG_DARK) noexcept;

  void draw(Framebuffer& fb, WaveformType waveType, float amplitude,
            float frequency = 1.0f, bool showGrid = true) const;

  void drawCustom(Framebuffer& fb, const float* samples, int numSamples,
                  bool showGrid = true) const;

private:
  int x_, y_, width_, height_;
  uint16_t fgColor_, bgColor_;

  void drawSine(Framebuffer& fb, float amplitude, int centerY) const;
  void drawSquare(Framebuffer& fb, float amplitude, int centerY) const;
  void drawTriangle(Framebuffer& fb, float amplitude, int centerY) const;
  void drawSawtooth(Framebuffer& fb, float amplitude, int centerY) const;
  void drawGrid(Framebuffer& fb, int centerY) const;
};

} // namespace ui
