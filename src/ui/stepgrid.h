#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"

namespace ui {

enum class StepGridStyle : uint8_t {
    SQUARES,
    BARS,
    DOTS,
    STRIPS,
    GLOW,
};

class StepGrid : public Widget {
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
              const uint8_t velocities[MAX_TRACKS][MAX_STEPS] = nullptr,
              const char* const notes[MAX_TRACKS][MAX_STEPS] = nullptr);

    int getTouchedTrack() const noexcept { return touchedTrack_; }
    int getTouchedStep() const noexcept { return touchedStep_; }
    bool hasTouch() const noexcept { return touchedTrack_ >= 0; }

    // Origin cell of the current touch (set on Began, unchanged during drag).
    int getTouchStartTrack() const noexcept { return touchStartTrack_; }
    int getTouchStartStep()  const noexcept { return touchStartStep_; }
    // Returns the number of pyThreshold-sized Y increments accumulated since
    // the last call (consumes them). Used by screens for vertical-drag actions.
    int takeDragTicks(int pxThreshold);
    // True once after a tap (touch ended without drag), consumed on read.
    bool wasTapped();
    int dragAccumY() const noexcept { return dragAccumY_; }

    void setStyle(StepGridStyle style) noexcept { style_ = style; }
    void setPosition(int x, int y) noexcept { setBounds(x, y, w_, h_); }
    void setColors(const uint16_t trackColors[MAX_TRACKS],
                   uint16_t accent, uint16_t bg) noexcept;
    void setStepCount(int n) noexcept {
        steps_ = (n < 1) ? 1 : ((n > MAX_STEPS) ? MAX_STEPS : n);
    }
    void setTrackCount(int n) noexcept {
        tracks_ = (n < 1) ? 1 : ((n > MAX_TRACKS) ? MAX_TRACKS : n);
    }
    void setActiveTrack(int track) noexcept;
    int getActiveTrack() const noexcept { return activeTrack_; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }
    int getStepCount() const noexcept { return steps_; }
    int getTrackCount() const noexcept { return tracks_; }
    StepGridStyle getStyle() const noexcept { return style_; }

    int cellX(int step) const noexcept;
    int cellY(int track) const noexcept;
    int cellW() const noexcept;
    int cellH() const noexcept;

private:
    void drawSquares(Framebuffer& fb,
                     const bool pattern[MAX_TRACKS][MAX_STEPS], int playStep,
                     const char* const notes[MAX_TRACKS][MAX_STEPS]);
    void drawBars(Framebuffer& fb,
                  const bool pattern[MAX_TRACKS][MAX_STEPS], int playStep,
                  const uint8_t velocities[MAX_TRACKS][MAX_STEPS],
                  const char* const notes[MAX_TRACKS][MAX_STEPS]);
    void drawDots(Framebuffer& fb,
                  const bool pattern[MAX_TRACKS][MAX_STEPS], int playStep,
                  const char* const notes[MAX_TRACKS][MAX_STEPS]);
    void drawStrips(Framebuffer& fb,
                    const bool pattern[MAX_TRACKS][MAX_STEPS], int playStep,
                    const char* const notes[MAX_TRACKS][MAX_STEPS]);
    void drawGlow(Framebuffer& fb,
                  const bool pattern[MAX_TRACKS][MAX_STEPS], int playStep,
                  const char* const notes[MAX_TRACKS][MAX_STEPS]);

    void drawCellNote(Framebuffer& fb, int cx, int cy, int cw, int ch,
                      const char* text, uint16_t fg, uint16_t bg) const;

    int steps_, tracks_;
    int activeTrack_;
    StepGridStyle style_;
    uint16_t trackColors_[MAX_TRACKS];
    uint16_t accent_;
    uint16_t bg_;
    int touchedTrack_ = -1;
    int touchedStep_ = -1;
    int touchStartTrack_ = -1;
    int touchStartStep_ = -1;
    int dragAccumY_ = 0;
    int lastTouchY_ = 0;
    float smoothPlayStep_ = -1.0f;

    bool mapTouch(int tx, int ty, int& outTrack, int& outStep) const;
    bool onTouchBegan(const TouchEvent& event) override;
    bool onTouchMoved(const TouchEvent& event) override;
    void onTouchEnded(const TouchEvent& event) override;
    void onTap(const TouchEvent& event) override;

    bool tapped_ = false;
};

} // namespace ui
