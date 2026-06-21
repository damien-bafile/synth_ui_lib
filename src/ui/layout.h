#pragma once
#include "rect.h"

namespace ui {

class LayoutBuilder {
public:
    LayoutBuilder(int x, int y, int w, int h) noexcept;
    explicit LayoutBuilder(Rect rect) noexcept;

    LayoutBuilder& setPadding(int p) noexcept;
    LayoutBuilder& setPadding(int h, int v) noexcept;
    LayoutBuilder& setPadding(int t, int r, int b, int l) noexcept;

    LayoutBuilder& setSpacing(int gap) noexcept;
    LayoutBuilder& setSpacing(int hgap, int vgap) noexcept;

    Rect next(int h);
    Rect next(int w, int h);
    Rect remaining();
    Rect allRemaining();

    void nextRow();
    void reset();

    Rect cell(int col, int row, int cols, int rows,
              int colspan = 1, int rowspan = 1) const noexcept;

    LayoutBuilder sub(Rect rect) const noexcept;

    Rect contentArea() const noexcept;
    int contentX() const noexcept;
    int contentY() const noexcept;
    int contentW() const noexcept;
    int contentH() const noexcept;
    int cursorX() const noexcept;
    int cursorY() const noexcept;

private:
    int x_, y_, w_, h_;
    int padT_ = 0, padR_ = 0, padB_ = 0, padL_ = 0;
    int hgap_ = 0, vgap_ = 0;
    int cx_, cy_, cw_, ch_;
    int cursorX_, cursorY_;
    int lineMaxH_ = 0;
};

} // namespace ui
