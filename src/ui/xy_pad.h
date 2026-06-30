#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"

namespace ui {

class XYPad : public Widget {
public:
  enum class MarkerStyle : uint8_t { CIRCLE, CROSSHAIR, SQUARE };

  XYPad(int x, int y, int size, uint16_t fgColor = ACCENT_2,
        uint16_t bgColor = BG_DARK, uint16_t gridColor = BG_MID,
        bool flipY = false) noexcept;

  void draw(Framebuffer& fb, float xValue, float yValue, MarkerStyle style = MarkerStyle::CIRCLE,
            bool showGrid = true, bool showLabels = false) const;

  float getX() const noexcept { return xFrac_; }
  float getY() const noexcept { return yFrac_; }

  void setFlipY(bool flip) { flipY_ = flip; }
  bool flipY() const { return flipY_; }

private:
  int size_;
  uint16_t fgColor_;
  uint16_t bgColor_;
  uint16_t gridColor_;
  float xFrac_ = 0.5f;
  float yFrac_ = 0.5f;
  bool flipY_;

  bool onTouchBegan(const TouchEvent& event) override;
  void onDragMoved(const TouchEvent& event, int dx, int dy) override;

  void drawMarker(Framebuffer& fb, int pixelX, int pixelY,
                  MarkerStyle style) const;
  void drawGrid(Framebuffer& fb) const;
  void drawAxes(Framebuffer& fb) const;
};

} // namespace ui
