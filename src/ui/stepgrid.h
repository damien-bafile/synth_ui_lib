#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "touch.h"
#include "colors.h"

namespace ui {

enum class StepGridStyle : uint8_t {
    SQUARES,    // Filled square on, outline off
    BARS,       // Vertical bar, height = velocity
    DOTS,       // Small filled circles
    STRIPS,     // Horizontal strips per track
    GLOW,       // Sub-step glow with playhead line
};

class StepGrid {
public:
    static constexpr int MAX_STEPS  = 32;
    static constexpr int MAX_TRACKS = 8;
    static constexpr int DEFAULT_STEPS = 16;

    StepGrid(int x, int y, int w, int h,
             int steps = DEFAULT_STEPS,
             int tracks = 4,
             StepGridStyle style = StepGridStyle::SQUARES);

    void draw(Framebuffer& fb,
              const bool pattern[MAX_TRACKS][MAX_STEPS],
              int playStep = -1,
              const uint8_t velocities[MAX_TRACKS][MAX_STEPS] = nullptr);

    bool handleTouch(const TouchState& touch, int& outTrack, int& outStep);

    void setStyle(StepGridStyle style) noexcept { style_ = style; }
    void setPosition(int x, int y) noexcept { x_ = x; y_ = y; }
    void setColors(const uint16_t trackColors[MAX_TRACKS],
                   uint16_t accent, uint16_t bg) noexcept;
    void setStepCount(int n) noexcept { steps_ = (n <= MAX_STEPS) ? n : MAX_STEPS; }
    void setTrackCount(int n) noexcept { tracks_ = (n <= MAX_TRACKS) ? n : MAX_TRACKS; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }
    int getStepCount() const noexcept { return steps_; }
    int getTrackCount() const noexcept { return tracks_; }
    StepGridStyle getStyle() const noexcept { return style_; }

private:
    void drawSquares(Framebuffer& fb,
                     const bool pattern[MAX_TRACKS][MAX_STEPS], int playStep);
    void drawBars(Framebuffer& fb,
                  const bool pattern[MAX_TRACKS][MAX_STEPS], int playStep,
                  const uint8_t velocities[MAX_TRACKS][MAX_STEPS]);
    void drawDots(Framebuffer& fb,
                  const bool pattern[MAX_TRACKS][MAX_STEPS], int playStep);
    void drawStrips(Framebuffer& fb,
                    const bool pattern[MAX_TRACKS][MAX_STEPS], int playStep);
    void drawGlow(Framebuffer& fb,
                  const bool pattern[MAX_TRACKS][MAX_STEPS], int playStep);

    int cellX(int step) const noexcept;
    int cellY(int track) const noexcept;
    int cellW() const noexcept;
    int cellH() const noexcept;

    int x_, y_, w_, h_;
    int steps_, tracks_;
    StepGridStyle style_;
    uint16_t trackColors_[MAX_TRACKS];
    uint16_t accent_;
    uint16_t bg_;
};

} // namespace ui
