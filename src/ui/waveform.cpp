#include "waveform.h"
#include "colors.h"
#include <cmath>

namespace ui {

Waveform::Waveform(int x, int y, int width, int height, uint16_t fgColor,
                   uint16_t bgColor)
    : x_(x), y_(y), width_(width), height_(height), fgColor_(fgColor),
      bgColor_(bgColor) {}

void Waveform::draw(Framebuffer& fb, WaveformType waveType, float amplitude,
                    float frequency, bool showGrid) const {
  // Draw background
  fb.fillRect(x_, y_, width_, height_, bgColor_);
  fb.drawRect(x_, y_, width_, height_, fgColor_);

  // Center line
  int centerY = y_ + height_ / 2;
  fb.drawLine(x_ + 1, centerY, x_ + width_ - 1, centerY, BG_MID);

  if (showGrid) {
    drawGrid(fb, centerY);
  }

  // Clamp amplitude
  amplitude = amplitude < 0.0f ? 0.0f : (amplitude > 1.0f ? 1.0f : amplitude);

  switch (waveType) {
  case SINE:
    drawSine(fb, amplitude, centerY);
    break;
  case SQUARE:
    drawSquare(fb, amplitude, centerY);
    break;
  case TRIANGLE:
    drawTriangle(fb, amplitude, centerY);
    break;
  case SAWTOOTH:
    drawSawtooth(fb, amplitude, centerY);
    break;
  case CUSTOM:
    // Custom waveform handled by drawCustom()
    break;
  }
}

void Waveform::drawCustom(Framebuffer& fb, const float* samples,
                          int numSamples, bool showGrid) const {
  // Draw background
  fb.fillRect(x_, y_, width_, height_, bgColor_);
  fb.drawRect(x_, y_, width_, height_, fgColor_);

  int centerY = y_ + height_ / 2;
  fb.drawLine(x_ + 1, centerY, x_ + width_ - 1, centerY, BG_MID);

  if (showGrid) {
    drawGrid(fb, centerY);
  }

  if (numSamples < 2)
    return;

  // Draw waveform from samples
  int lastPixelX = x_ + 1;
  int lastPixelY = centerY;

  for (int i = 0; i < numSamples - 1; i++) {
    int pixelX = x_ + 1 + (i * (width_ - 2)) / (numSamples - 1);
    float sampleValue = samples[i];
    // Clamp sample value
    sampleValue = sampleValue < -1.0f ? -1.0f
                  : (sampleValue > 1.0f ? 1.0f : sampleValue);
    int pixelY = centerY - (int)(sampleValue * (height_ / 2 - 2));

    fb.drawLine(lastPixelX, lastPixelY, pixelX, pixelY, fgColor_);

    lastPixelX = pixelX;
    lastPixelY = pixelY;
  }
}

void Waveform::drawSine(Framebuffer& fb, float amplitude, int centerY) const {
  int lastX = x_ + 1;
  int lastY = centerY;

  for (int i = 0; i < width_ - 2; i++) {
    float phase = (i / (float)(width_ - 2)) * 2.0f * 3.14159f;
    float sineValue = std::sin(phase) * amplitude;
    int pixelX = x_ + 1 + i;
    int pixelY = centerY - (int)(sineValue * (height_ / 2 - 2));

    if (i > 0) {
      fb.drawLine(lastX, lastY, pixelX, pixelY, fgColor_);
    }
    lastX = pixelX;
    lastY = pixelY;
  }
}

void Waveform::drawSquare(Framebuffer& fb, float amplitude, int centerY) const {
  int amplitude_px = (int)(amplitude * (height_ / 2 - 2));

  // First half: high
  int midX = x_ + 1 + (width_ - 2) / 2;
  fb.drawLine(x_ + 1, centerY - amplitude_px, midX, centerY - amplitude_px,
              fgColor_);

  // Second half: low
  fb.drawLine(midX, centerY + amplitude_px, x_ + width_ - 1,
              centerY + amplitude_px, fgColor_);

  // Vertical transitions
  fb.drawLine(x_ + 1, centerY - amplitude_px, x_ + 1, centerY + amplitude_px,
              fgColor_);
  fb.drawLine(midX, centerY - amplitude_px, midX, centerY + amplitude_px,
              fgColor_);
  fb.drawLine(x_ + width_ - 1, centerY + amplitude_px, x_ + width_ - 1,
              centerY - amplitude_px, fgColor_);
}

void Waveform::drawTriangle(Framebuffer& fb, float amplitude,
                            int centerY) const {
  int amplitude_px = (int)(amplitude * (height_ / 2 - 2));
  int midX = x_ + 1 + (width_ - 2) / 2;
  int quarterX = x_ + 1 + (width_ - 2) / 4;
  int threeQuarterX = x_ + 1 + 3 * (width_ - 2) / 4;

  fb.drawLine(x_ + 1, centerY, quarterX, centerY - amplitude_px, fgColor_);
  fb.drawLine(quarterX, centerY - amplitude_px, threeQuarterX,
              centerY + amplitude_px, fgColor_);
  fb.drawLine(threeQuarterX, centerY + amplitude_px, x_ + width_ - 1, centerY,
              fgColor_);
}

void Waveform::drawSawtooth(Framebuffer& fb, float amplitude,
                            int centerY) const {
  int amplitude_px = (int)(amplitude * (height_ / 2 - 2));
  int midX = x_ + 1 + (width_ - 2) / 2;

  // Ramp up
  fb.drawLine(x_ + 1, centerY + amplitude_px, midX, centerY - amplitude_px,
              fgColor_);
  // Sharp drop
  fb.drawLine(midX, centerY - amplitude_px, midX, centerY + amplitude_px,
              fgColor_);
  // Ramp down to end
  fb.drawLine(midX, centerY + amplitude_px, x_ + width_ - 1, centerY - amplitude_px,
              fgColor_);
}

void Waveform::drawGrid(Framebuffer& fb, int centerY) const {
  // Horizontal grid lines (quartiles)
  int quarterHeight = height_ / 4;
  for (int i = 1; i < 4; i++) {
    int gridY = y_ + quarterHeight * i;
    fb.drawLine(x_ + 2, gridY, x_ + width_ - 2, gridY, BG_MID);
  }

  // Vertical grid line (center)
  int centerX = x_ + width_ / 2;
  fb.drawLine(centerX, y_ + 2, centerX, y_ + height_ - 2, BG_MID);
}

} // namespace ui
