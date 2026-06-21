#include "xy_pad.h"
#include "touch.h"
#include <cmath>

namespace ui {

XYPad::XYPad(int x, int y, int size, uint16_t fgColor, uint16_t bgColor,
             uint16_t gridColor)
    : x_(x), y_(y), size_(size), fgColor_(fgColor), bgColor_(bgColor),
      gridColor_(gridColor), lastX_(0.5f), lastY_(0.5f) {}

void XYPad::draw(Framebuffer& fb, float xValue, float yValue, MarkerStyle style,
                 bool showGrid, bool showLabels) const {
  // Clamp values
  xValue = xValue < 0.0f ? 0.0f : (xValue > 1.0f ? 1.0f : xValue);
  yValue = yValue < 0.0f ? 0.0f : (yValue > 1.0f ? 1.0f : yValue);

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
  int markerY = y_ + 2 + (int)(yValue * (size_ - 4));

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

bool XYPad::handleTouchInput(const TouchState& touch, float* outX,
                             float* outY) {
  // Check if touch is within bounds
  if (touch.x < x_ || touch.x >= x_ + size_ || touch.y < y_ ||
      touch.y >= y_ + size_) {
    return false;
  }

  if (touch.pressed) {
    // Map pixel coordinates to 0.0-1.0 range
    float normX = (float)(touch.x - x_) / (float)(size_ - 1);
    float normY = (float)(touch.y - y_) / (float)(size_ - 1);

    // Clamp to valid range
    normX = normX < 0.0f ? 0.0f : (normX > 1.0f ? 1.0f : normX);
    normY = normY < 0.0f ? 0.0f : (normY > 1.0f ? 1.0f : normY);

    if (outX)
      *outX = normX;
    if (outY)
      *outY = normY;

    lastX_ = normX;
    lastY_ = normY;

    return true;
  }

  return false;
}

void XYPad::drawMarker(Framebuffer& fb, int pixelX, int pixelY,
                       MarkerStyle style) const {
  switch (style) {
  case CIRCLE: {
    // Draw small circle marker
    int radius = 3;
    fb.drawCircle(pixelX, pixelY, radius, fgColor_);
    fb.drawCircle(pixelX, pixelY, radius - 1, fgColor_);
    break;
  }
  case CROSSHAIR: {
    // Draw crosshair
    int hairLength = 4;
    fb.drawLine(pixelX - hairLength, pixelY, pixelX + hairLength, pixelY,
                fgColor_);
    fb.drawLine(pixelX, pixelY - hairLength, pixelX, pixelY + hairLength,
                fgColor_);
    break;
  }
  case SQUARE: {
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
