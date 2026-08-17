#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"

namespace ui {

// Modulation routing grid: sourceCount rows x destCount columns, each cell
// a stepped depth (OFF/LOW/MED/HIGH) that cycles on tap. Owns its depth
// state internally (unlike StepGrid, which is handed an external pattern
// each frame) -- depths are inherently the matrix's own small, bounded
// state, closer to how Slider owns its value_. Cell hit-testing follows
// StepGrid's mapTouch/cellX/cellY approach.
class ModMatrix : public Widget {
public:
    static constexpr int MAX_SOURCES  = 8;
    static constexpr int MAX_DESTS    = 8;
    static constexpr int DEPTH_LEVELS = 4;  // OFF, LOW, MED, HIGH
    static constexpr int LABEL_W = 30;
    static constexpr int LABEL_H = 12;

    ModMatrix(int x, int y, int w, int h,
              const char* const* sourceLabels, int sourceCount,
              const char* const* destLabels, int destCount,
              uint16_t fg = ACCENT_1, uint16_t bg = BG_DARK,
              uint16_t grid = BG_MID) noexcept;

    void draw(Framebuffer& fb) const;

    // Depth as a fraction in [0,1]; level*1.0/(DEPTH_LEVELS-1).
    float getDepth(int row, int col) const noexcept;
    int getDepthLevel(int row, int col) const noexcept;
    void setDepthLevel(int row, int col, int level) noexcept;

    // Reports the most recently tapped cell (cycled depth). Clears on read.
    bool wasCellChanged() noexcept { bool v = changed_; changed_ = false; return v; }
    void getChangedCell(int& row, int& col) const noexcept {
        row = changedRow_; col = changedCol_;
    }

    int getSourceCount() const noexcept { return sourceCount_; }
    int getDestCount() const noexcept { return destCount_; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }

    int cellX(int col) const noexcept;
    int cellY(int row) const noexcept;
    int cellW() const noexcept;
    int cellH() const noexcept;

private:
    bool mapTouch(int px, int py, int& row, int& col) const;

    const char* const* sourceLabels_;
    int sourceCount_;
    const char* const* destLabels_;
    int destCount_;
    uint8_t depthLevel_[MAX_SOURCES][MAX_DESTS] = {};
    uint16_t fg_, bg_, grid_;

    bool changed_ = false;
    int changedRow_ = -1, changedCol_ = -1;

    bool onTouchBegan(const TouchEvent& event) override;
    void onTap(const TouchEvent& event) override;
};

} // namespace ui
