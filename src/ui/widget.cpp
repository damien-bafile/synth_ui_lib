#include "widget.h"

namespace ui {

void Widget::setBounds(int x, int y, int w, int h) noexcept {
    x_ = x; y_ = y; w_ = w; h_ = h;
}

bool Widget::contains(int px, int py) const noexcept {
    return px >= x_ && px < x_ + w_ && py >= y_ && py < y_ + h_;
}

bool Widget::handleTouch(const TouchEvent& event) {
    switch (event.phase) {
        case TouchPhase::Began:
            return onTouchBegan(event);
        case TouchPhase::Moved:
            onTouchMoved(event);
            return true;
        case TouchPhase::Ended:
            onTouchEnded(event);
            return true;
        case TouchPhase::Cancelled:
            onTouchCancelled(event);
            return true;
    }
    return false;
}

bool Widget::onTouchBegan(const TouchEvent& event) {
    return contains(event.x, event.y);
}

bool Widget::onTouchMoved(const TouchEvent&) {
    return true;
}

void Widget::onTouchEnded(const TouchEvent&) {}
void Widget::onTouchCancelled(const TouchEvent&) {}

void Widget::onTap(const TouchEvent&) {}
void Widget::onLongPress(const TouchEvent&) {}
void Widget::onDragBegan(const TouchEvent&) {}
void Widget::onDragMoved(const TouchEvent&, int, int) {}
void Widget::onDragEnded(const TouchEvent&) {}

} // namespace ui
