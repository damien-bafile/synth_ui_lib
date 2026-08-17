#include "mod_matrix.h"
#include "font.h"

namespace ui {

ModMatrix::ModMatrix(int x, int y, int w, int h,
                     const char* const* sourceLabels, int sourceCount,
                     const char* const* destLabels, int destCount,
                     uint16_t fg, uint16_t bg, uint16_t grid) noexcept
    : sourceLabels_(sourceLabels), sourceCount_(sourceCount),
      destLabels_(destLabels), destCount_(destCount),
      fg_(fg), bg_(bg), grid_(grid) {
    setBounds(x, y, w, h);
    if (sourceCount_ > MAX_SOURCES) sourceCount_ = MAX_SOURCES;
    if (sourceCount_ < 1) sourceCount_ = 1;
    if (destCount_ > MAX_DESTS) destCount_ = MAX_DESTS;
    if (destCount_ < 1) destCount_ = 1;
}

int ModMatrix::cellW() const noexcept {
    int gw = w_ - LABEL_W;
    return gw > 0 ? gw / destCount_ : 1;
}

int ModMatrix::cellH() const noexcept {
    int gh = h_ - LABEL_H;
    return gh > 0 ? gh / sourceCount_ : 1;
}

int ModMatrix::cellX(int col) const noexcept {
    return x_ + LABEL_W + col * cellW();
}

int ModMatrix::cellY(int row) const noexcept {
    return y_ + LABEL_H + row * cellH();
}

float ModMatrix::getDepth(int row, int col) const noexcept {
    return static_cast<float>(getDepthLevel(row, col)) / (DEPTH_LEVELS - 1);
}

int ModMatrix::getDepthLevel(int row, int col) const noexcept {
    if (row < 0 || row >= sourceCount_ || col < 0 || col >= destCount_) return 0;
    return depthLevel_[row][col];
}

void ModMatrix::setDepthLevel(int row, int col, int level) noexcept {
    if (row < 0 || row >= sourceCount_ || col < 0 || col >= destCount_) return;
    if (level < 0) level = 0;
    if (level >= DEPTH_LEVELS) level = DEPTH_LEVELS - 1;
    depthLevel_[row][col] = static_cast<uint8_t>(level);
}

bool ModMatrix::mapTouch(int px, int py, int& row, int& col) const {
    int gx = x_ + LABEL_W;
    int gy = y_ + LABEL_H;
    if (px < gx || px >= x_ + w_) return false;
    if (py < gy || py >= y_ + h_) return false;

    col = (px - gx) / cellW();
    row = (py - gy) / cellH();
    if (col < 0 || col >= destCount_) return false;
    if (row < 0 || row >= sourceCount_) return false;
    return true;
}

bool ModMatrix::onTouchBegan(const TouchEvent& event) {
    int row, col;
    return mapTouch(event.x, event.y, row, col);
}

void ModMatrix::onTap(const TouchEvent& event) {
    int row, col;
    if (!mapTouch(event.x, event.y, row, col)) return;
    depthLevel_[row][col] = (depthLevel_[row][col] + 1) % DEPTH_LEVELS;
    changed_ = true;
    changedRow_ = row;
    changedCol_ = col;
}

void ModMatrix::draw(Framebuffer& fb) const {
    fb.fillRect(x_, y_, w_, h_, bg_);

    for (int r = 0; r < sourceCount_; r++) {
        if (sourceLabels_ && sourceLabels_[r]) {
            int ty = cellY(r) + (cellH() - FONT_H) / 2;
            fb.drawText(x_, ty, sourceLabels_[r], fg_, bg_);
        }
    }
    for (int c = 0; c < destCount_; c++) {
        if (destLabels_ && destLabels_[c]) {
            fb.drawText(cellX(c), y_, destLabels_[c], fg_, bg_);
        }
    }

    int cw = cellW();
    int ch = cellH();
    int minDim = cw < ch ? cw : ch;

    for (int r = 0; r < sourceCount_; r++) {
        for (int c = 0; c < destCount_; c++) {
            int cx = cellX(c);
            int cy = cellY(r);
            fb.drawRect(cx, cy, cw - 1, ch - 1, grid_);

            int level = depthLevel_[r][c];
            if (level > 0) {
                int pad = ((DEPTH_LEVELS - level) * minDim) / (2 * DEPTH_LEVELS);
                int fw = cw - 1 - pad * 2;
                int fh = ch - 1 - pad * 2;
                if (fw > 0 && fh > 0) {
                    fb.fillRect(cx + pad, cy + pad, fw, fh, fg_);
                }
            }
        }
    }
}

} // namespace ui
