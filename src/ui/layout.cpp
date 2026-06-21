#include "layout.h"
#include <algorithm>

namespace ui {

LayoutBuilder::LayoutBuilder(int x, int y, int w, int h) noexcept
    : x_(x), y_(y), w_(w), h_(h),
      cx_(x), cy_(y), cw_(w), ch_(h),
      cursorX_(x), cursorY_(y) {}

LayoutBuilder::LayoutBuilder(Rect rect) noexcept
    : LayoutBuilder(rect.x, rect.y, rect.w, rect.h) {}

LayoutBuilder& LayoutBuilder::setPadding(int p) noexcept {
    padT_ = padR_ = padB_ = padL_ = p;
    cx_ = x_ + padL_; cy_ = y_ + padT_;
    cw_ = w_ - padL_ - padR_;
    ch_ = h_ - padT_ - padB_;
    cursorX_ = cx_; cursorY_ = cy_;
    return *this;
}

LayoutBuilder& LayoutBuilder::setPadding(int h, int v) noexcept {
    return setPadding(v, h, v, h);
}

LayoutBuilder& LayoutBuilder::setPadding(int t, int r, int b, int l) noexcept {
    padT_ = t; padR_ = r; padB_ = b; padL_ = l;
    cx_ = x_ + padL_; cy_ = y_ + padT_;
    cw_ = w_ - padL_ - padR_;
    ch_ = h_ - padT_ - padB_;
    cursorX_ = cx_; cursorY_ = cy_;
    return *this;
}

LayoutBuilder& LayoutBuilder::setSpacing(int gap) noexcept {
    hgap_ = vgap_ = gap;
    return *this;
}

LayoutBuilder& LayoutBuilder::setSpacing(int hgap, int vgap) noexcept {
    hgap_ = hgap; vgap_ = vgap;
    return *this;
}

Rect LayoutBuilder::next(int h) {
    int availW = cx_ + cw_ - cursorX_;
    if (availW < 0) availW = 0;
    Rect r = {cursorX_, cursorY_, availW, h};
    lineMaxH_ = std::max(lineMaxH_, h);
    cursorY_ += lineMaxH_ + vgap_;
    cursorX_ = cx_;
    lineMaxH_ = 0;
    return r;
}

Rect LayoutBuilder::next(int w, int h) {
    int availW = cx_ + cw_ - cursorX_;
    if (w > availW) w = availW;
    Rect r = {cursorX_, cursorY_, w, h};
    lineMaxH_ = std::max(lineMaxH_, h);
    cursorX_ += w + hgap_;
    return r;
}

Rect LayoutBuilder::remaining() {
    int w = cx_ + cw_ - cursorX_;
    if (w < 0) w = 0;
    Rect r = {cursorX_, cursorY_, w, lineMaxH_ ? lineMaxH_ : 1};
    cursorX_ = cx_ + cw_;
    return r;
}

Rect LayoutBuilder::allRemaining() {
    int w = cx_ + cw_ - cursorX_;
    int h = cy_ + ch_ - cursorY_;
    if (w < 0) w = 0;
    if (h < 0) h = 0;
    return {cursorX_, cursorY_, w, h};
}

void LayoutBuilder::nextRow() {
    if (cursorX_ == cx_) return;
    cursorY_ += lineMaxH_ + vgap_;
    cursorX_ = cx_;
    lineMaxH_ = 0;
}

void LayoutBuilder::reset() {
    cursorX_ = cx_;
    cursorY_ = cy_;
    lineMaxH_ = 0;
}

Rect LayoutBuilder::cell(int col, int row, int cols, int rows,
                          int colspan, int rowspan) const noexcept {
    if (cols < 1) cols = 1;
    if (rows < 1) rows = 1;
    int cellW = cw_ / cols;
    int cellH = ch_ / rows;
    return {
        cx_ + col * cellW + hgap_ / 2,
        cy_ + row * cellH + vgap_ / 2,
        cellW * colspan - hgap_,
        cellH * rowspan - vgap_
    };
}

LayoutBuilder LayoutBuilder::sub(Rect rect) const noexcept {
    return LayoutBuilder(rect);
}

Rect LayoutBuilder::contentArea() const noexcept { return {cx_, cy_, cw_, ch_}; }
int LayoutBuilder::contentX() const noexcept { return cx_; }
int LayoutBuilder::contentY() const noexcept { return cy_; }
int LayoutBuilder::contentW() const noexcept { return cw_; }
int LayoutBuilder::contentH() const noexcept { return ch_; }
int LayoutBuilder::cursorX() const noexcept { return cursorX_; }
int LayoutBuilder::cursorY() const noexcept { return cursorY_; }

} // namespace ui
