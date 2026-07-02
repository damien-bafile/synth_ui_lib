#pragma once
#include "touch_event.h"
#include "rect.h"
#include <cstdint>

namespace ui {

class Widget {
public:
    Widget() = default;
    virtual ~Widget() = default;

    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;

    // Geometry
    void setBounds(int x, int y, int w, int h) noexcept;
    Rect bounds() const noexcept { return {x_, y_, w_, h_}; }
    bool contains(int px, int py) const noexcept;

    int x() const noexcept { return x_; }
    int y() const noexcept { return y_; }
    int width() const noexcept { return w_; }
    int height() const noexcept { return h_; }

    // State
    void setVisible(bool v) noexcept { visible_ = v; }
    bool visible() const noexcept { return visible_; }
    void setEnabled(bool e) noexcept { enabled_ = e; }
    bool enabled() const noexcept { return enabled_; }
    void setZOrder(int z) noexcept { zOrder_ = z; }
    int zOrder() const noexcept { return zOrder_; }

    // Touch: return true if event was consumed
    // Default dispatches to phase-specific callbacks
    virtual bool handleTouch(const TouchEvent& event);

    // Phase callbacks (override these)
    // onTouchBegan: return true to capture this touch
    virtual bool onTouchBegan(const TouchEvent& event);
    virtual bool onTouchMoved(const TouchEvent& event);
    virtual void onTouchEnded(const TouchEvent& event);
    virtual void onTouchCancelled(const TouchEvent& event);

    // Gesture callbacks (override these)
    virtual void onTap(const TouchEvent& event);
    virtual void onDoubleTap(const TouchEvent& event);
    virtual void onLongPress(const TouchEvent& event);
    virtual void onDragBegan(const TouchEvent& event);
    virtual void onDragMoved(const TouchEvent& event, int dx, int dy);
    virtual void onDragEnded(const TouchEvent& event);

protected:
    int x_ = 0, y_ = 0, w_ = 0, h_ = 0;
    bool visible_ = true;
    bool enabled_ = true;
    int zOrder_ = 0;
};

} // namespace ui
