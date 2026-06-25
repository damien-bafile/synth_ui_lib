#include "xy_pad.h"
#include "touch.h"
#include "colors.h"
#include <cmath>

namespace ui {

XYPad::XYPad(int x, int y, int size, uint16_t fgColor, uint16_t bgColor,
             uint16_t gridColor, bool flipY) noexcept
    : x_(x), y_(y), size_(size), fgColor_(fgColor), bgColor_(bgColor),
      gridColor_(gridColor), lastX_(0.5f), lastY_(0.5f), flipY_(flipY) {}

void XYPad::draw(Framebuffer& fb, float xValue, float yValue, MarkerStyle style,
                 bool showGrid, bool showLabels) const {
  xValue = clamp(xValue, 0.0f, 1.0f);
  yValue = clamp(yValue, 0.0f, 1.0f);

  // Draw background
  fb.fillRect(x_, y_, size_, size_, bgColor_);
  fb.drawRect(x_, y_, size_, size_, fgColor_);

  if (showGrid) {
    drawGrid(fb);
  }

  // Draw axes
  drawAxes(fb);

  // Calculate marker position
  int markerX = x_ + 2 + (int)(xValue * (size_ - 4));
  int markerY = flipY_
    ? y_ + size_ - 2 - (int)(yValue * (size_ - 4))
    : y_ + 2 + (int)(yValue * (size_ - 4));

  // Draw marker
  drawMarker(fb, markerX, markerY, style);

  // Optional axis labels
  if (showLabels) {
    // X axis label (simple visualization)
    int labelX = x_ + 2 + (int)(xValue * (size_ - 4));
    fb.drawLine(labelX, y_ + size_ - 3, labelX, y_ + size_ - 1, fgColor_);

    // Y axis label (simple visualization)
    int labelY = y_ + 2 + (int)(yValue * (size_ - 4));
    fb.drawLine(x_ + size_ - 3, labelY, x_ + size_ - 1, labelY, fgColor_);
  }
}

bool XYPad::handleTouch(const TouchState& touch, float& outX,
                        float& outY) {
  if (touch.x < x_ || touch.x >= x_ + size_ || touch.y < y_ ||
      touch.y >= y_ + size_) {
    return false;
  }

  if (touch.pressed) {
    float normX = (float)(touch.x - x_) / (float)(size_ - 1);
    float normY = (float)(touch.y - y_) / (float)(size_ - 1);

    outX = clamp(normX, 0.0f, 1.0f);
    outY = flipY_ ? 1.0f - clamp(normY, 0.0f, 1.0f) : clamp(normY, 0.0f, 1.0f);

    lastX_ = outX;
    lastY_ = outY;

    return true;
  }

  return false;
}

void XYPad::drawMarker(Framebuffer& fb, int pixelX, int pixelY,
                       MarkerStyle style) const {
  switch (style) {
  case MarkerStyle::CIRCLE: {
    // Draw small circle marker
    int radius = 3;
    fb.drawCircle(pixelX, pixelY, radius, fgColor_);
    fb.drawCircle(pixelX, pixelY, radius - 1, fgColor_);
    break;
  }
  case MarkerStyle::CROSSHAIR: {
    // Draw crosshair
    int hairLength = 4;
    fb.drawLine(pixelX - hairLength, pixelY, pixelX + hairLength, pixelY,
                fgColor_);
    fb.drawLine(pixelX, pixelY - hairLength, pixelX, pixelY + hairLength,
                fgColor_);
    break;
  }
  case MarkerStyle::SQUARE: {
    // Draw square marker
    int halfSize = 3;
    fb.drawRect(pixelX - halfSize, pixelY - halfSize, halfSize * 2 + 1,
                halfSize * 2 + 1, fgColor_);
    break;
  }
  }
}

void XYPad::drawGrid(Framebuffer& fb) const {
  // Vertical grid lines (quarters)
  for (int i = 1; i < 4; i++) {
    int gridX = x_ + (size_ * i) / 4;
    fb.drawLine(gridX, y_ + 1, gridX, y_ + size_ - 2, gridColor_);
  }

  // Horizontal grid lines (quarters)
  for (int i = 1; i < 4; i++) {
    int gridY = y_ + (size_ * i) / 4;
    fb.drawLine(x_ + 1, gridY, x_ + size_ - 2, gridY, gridColor_);
  }
}

void XYPad::drawAxes(Framebuffer& fb) const {
  // Center horizontal line (Y = 0.5)
  int centerY = y_ + size_ / 2;
  fb.drawLine(x_ + 2, centerY, x_ + size_ - 2, centerY, gridColor_);

  // Center vertical line (X = 0.5)
  int centerX = x_ + size_ / 2;
  fb.drawLine(centerX, y_ + 2, centerX, y_ + size_ - 2, gridColor_);
}

} // namespace ui
