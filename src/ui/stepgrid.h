#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "touch.h"
#include "colors.h"

namespace ui {

enum StepGridStyle {
    STEP_GRID_SQUARES,    // Filled square on, outline off
    STEP_GRID_BARS,       // Vertical bar, height = velocity
    STEP_GRID_DOTS,       // Small filled circles
    STEP_GRID_STRIPS,     // Horizontal strips per track
    STEP_GRID_GLOW,       // Sub-step glow with playhead line
};

class StepGrid {
public:
    static constexpr int MAX_STEPS  = 32;
    static constexpr int MAX_TRACKS = 8;
    static constexpr int DEFAULT_STEPS = 16;

    StepGrid(int x, int y, int w, int h,
             int steps = DEFAULT_STEPS,
             int tracks = 4,
             StepGridStyle style = STEP_GRID_SQUARES);

    void draw(Framebuffer& fb,
              const bool pattern[MAX_TRACKS][MAX_STEPS],
              int playStep = -1,
              const uint8_t velocities[MAX_TRACKS][MAX_STEPS] = nullptr);

    bool handleTouch(const TouchState& touch, int& outTrack, int& outStep);

    void setStyle(StepGridStyle style) { style_ = style; }
    void setPosition(int x, int y) { x_ = x; y_ = y; }
    void setColors(const uint16_t trackColors[MAX_TRACKS],
                   uint16_t accent, uint16_t bg);
    void setStepCount(int n) { steps_ = (n <= MAX_STEPS) ? n : MAX_STEPS; }
    void setTrackCount(int n) { tracks_ = (n <= MAX_TRACKS) ? n : MAX_TRACKS; }

    int getX() const { return x_; }
    int getY() const { return y_; }
    int getWidth() const { return w_; }
    int getHeight() const { return h_; }
    int getStepCount() const { return steps_; }
    int getTrackCount() const { return tracks_; }
    StepGridStyle getStyle() const { return style_; }

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

    int cellX(int step) const;
    int cellY(int track) const;
    int cellW() const;
    int cellH() const;

    int x_, y_, w_, h_;
    int steps_, tracks_;
    StepGridStyle style_;
    uint16_t trackColors_[MAX_TRACKS];
    uint16_t accent_;
    uint16_t bg_;
};

} // namespace ui
