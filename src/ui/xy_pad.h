#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"
#include "touch.h"

namespace ui {

class XYPad {
public:
  enum MarkerStyle { CIRCLE, CROSSHAIR, SQUARE };

  XYPad(int x, int y, int size, uint16_t fgColor = ACCENT_2,
        uint16_t bgColor = BG_DARK, uint16_t gridColor = BG_MID);

  void draw(Framebuffer& fb, float xValue, float yValue, MarkerStyle style = CIRCLE,
            bool showGrid = true, bool showLabels = false) const;

  bool handleTouchInput(const TouchState& touch, float* outX, float* outY);

private:
  int x_, y_, size_;
  uint16_t fgColor_;
  uint16_t bgColor_;
  uint16_t gridColor_;
  float lastX_;
  float lastY_;

  void drawMarker(Framebuffer& fb, int pixelX, int pixelY,
                  MarkerStyle style) const;
  void drawGrid(Framebuffer& fb) const;
  void drawAxes(Framebuffer& fb) const;
};

} // namespace ui
