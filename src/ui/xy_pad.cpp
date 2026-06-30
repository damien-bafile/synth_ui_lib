#include "xy_pad.h"
#include "colors.h"
#include <cmath>

namespace ui {

XYPad::XYPad(int x, int y, int size, uint16_t fgColor, uint16_t bgColor,
             uint16_t gridColor, bool flipY) noexcept
    : size_(size), fgColor_(fgColor), bgColor_(bgColor),
      gridColor_(gridColor), flipY_(flipY) {
    setBounds(x, y, size, size);
}

void XYPad::draw(Framebuffer& fb, float xValue, float yValue, MarkerStyle style,
                 bool showGrid, bool showLabels) const {
  xValue = clamp(xValue, 0.0f, 1.0f);
  yValue = clamp(yValue, 0.0f, 1.0f);

  fb.fillRect(x_, y_, size_, size_, bgColor_);
  fb.drawRect(x_, y_, size_, size_, fgColor_);

  if (showGrid) drawGrid(fb);

  drawAxes(fb);

  int markerX = x_ + 2 + (int)(xValue * (size_ - 4));
  int markerY = flipY_
    ? y_ + size_ - 2 - (int)(yValue * (size_ - 4))
    : y_ + 2 + (int)(yValue * (size_ - 4));

  drawMarker(fb, markerX, markerY, style);

  if (showLabels) {
    int labelX = x_ + 2 + (int)(xValue * (size_ - 4));
    fb.drawLine(labelX, y_ + size_ - 3, labelX, y_ + size_ - 1, fgColor_);
    int labelY = y_ + 2 + (int)(yValue * (size_ - 4));
    fb.drawLine(x_ + size_ - 3, labelY, x_ + size_ - 1, labelY, fgColor_);
  }
}

bool XYPad::onTouchBegan(const TouchEvent& event) {
  return contains(event.x, event.y);
}

void XYPad::onDragMoved(const TouchEvent& event, int /*dx*/, int /*dy*/) {
  float normX = (float)(event.x - x_) / (float)(size_ - 1);
  float normY = (float)(event.y - y_) / (float)(size_ - 1);
  xFrac_ = clamp(normX, 0.0f, 1.0f);
  yFrac_ = flipY_ ? 1.0f - clamp(normY, 0.0f, 1.0f) : clamp(normY, 0.0f, 1.0f);
}

void XYPad::drawMarker(Framebuffer& fb, int pixelX, int pixelY,
                       MarkerStyle style) const {
  switch (style) {
  case MarkerStyle::CIRCLE: {
    int radius = 3;
    fb.drawCircle(pixelX, pixelY, radius, fgColor_);
    fb.drawCircle(pixelX, pixelY, radius - 1, fgColor_);
    break;
  }
  case MarkerStyle::CROSSHAIR: {
    int hairLength = 4;
    fb.drawLine(pixelX - hairLength, pixelY, pixelX + hairLength, pixelY, fgColor_);
    fb.drawLine(pixelX, pixelY - hairLength, pixelX, pixelY + hairLength, fgColor_);
    break;
  }
  case MarkerStyle::SQUARE: {
    int halfSize = 3;
    fb.drawRect(pixelX - halfSize, pixelY - halfSize, halfSize * 2 + 1,
                halfSize * 2 + 1, fgColor_);
    break;
  }
  }
}

void XYPad::drawGrid(Framebuffer& fb) const {
  for (int i = 1; i < 4; i++) {
    int gridX = x_ + (size_ * i) / 4;
    fb.drawLine(gridX, y_ + 1, gridX, y_ + size_ - 2, gridColor_);
  }
  for (int i = 1; i < 4; i++) {
    int gridY = y_ + (size_ * i) / 4;
    fb.drawLine(x_ + 1, gridY, x_ + size_ - 2, gridY, gridColor_);
  }
}

void XYPad::drawAxes(Framebuffer& fb) const {
  int centerY = y_ + size_ / 2;
  fb.drawLine(x_ + 2, centerY, x_ + size_ - 2, centerY, gridColor_);
  int centerX = x_ + size_ / 2;
  fb.drawLine(centerX, y_ + 2, centerX, y_ + size_ - 2, gridColor_);
}

} // namespace ui
