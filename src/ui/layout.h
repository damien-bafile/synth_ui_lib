#pragma once
#include "rect.h"

namespace ui {

class LayoutBuilder {
public:
    LayoutBuilder(int x, int y, int w, int h);
    explicit LayoutBuilder(Rect rect);

    LayoutBuilder& setPadding(int p);
    LayoutBuilder& setPadding(int h, int v);
    LayoutBuilder& setPadding(int t, int r, int b, int l);

    LayoutBuilder& setSpacing(int gap);
    LayoutBuilder& setSpacing(int hgap, int vgap);

    Rect next(int h);
    Rect next(int w, int h);
    Rect remaining();
    Rect allRemaining();

    void nextRow();
    void reset();

    Rect cell(int col, int row, int cols, int rows,
              int colspan = 1, int rowspan = 1) const;

    LayoutBuilder sub(Rect rect) const;

    Rect contentArea() const;
    int contentX() const;
    int contentY() const;
    int contentW() const;
    int contentH() const;
    int cursorX() const;
    int cursorY() const;

private:
    int x_, y_, w_, h_;
    int padT_ = 0, padR_ = 0, padB_ = 0, padL_ = 0;
    int hgap_ = 0, vgap_ = 0;
    int cx_, cy_, cw_, ch_;
    int cursorX_, cursorY_;
    int lineMaxH_ = 0;
};

} // namespace ui
