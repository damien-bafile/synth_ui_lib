#include "touch_dispatcher.h"
#include <algorithm>
#include <cstdlib>

namespace ui {

TouchDispatcher::TouchDispatcher() {
    for (auto& t : tracks_) {
        t.active = false;
        t.captured = nullptr;
    }
}

void TouchDispatcher::add(Widget* widget) {
    if (widgetCount_ >= MAX_WIDGETS) return;
    widgets_[widgetCount_++] = widget;
    sortByZOrder();
}

void TouchDispatcher::remove(const Widget* widget) {
    int idx = findWidget(widget);
    if (idx < 0) return;

    // Release any active touches on this widget
    for (auto& track : tracks_) {
        if (track.captured == widget) {
            TouchEvent ev;
            ev.phase = TouchPhase::Cancelled;
            ev.fingerId = 0;
            track.captured->handleTouch(ev);
            track.active = false;
            track.captured = nullptr;
        }
    }

    widgets_[idx] = widgets_[--widgetCount_];
    sortByZOrder();
}

void TouchDispatcher::clear() {
    // Only drop the widget list. In-flight touch tracks are kept so
    // immediate-mode callers (clear + re-add every frame) still deliver
    // Moved/Ended/onTap to the widget captured on Began. Captured widgets
    // must outlive the touch (e.g. static storage).
    widgetCount_ = 0;
}

int TouchDispatcher::findWidget(const Widget* widget) const {
    for (int i = 0; i < widgetCount_; i++) {
        if (widgets_[i] == widget) return i;
    }
    return -1;
}

void TouchDispatcher::sortByZOrder() {
    // Stable insertion sort (descending zOrder): widgets with equal zOrder
    // keep their registration order, which screens rely on for touch
    // priority. Cheaper than <algorithm> sorts for the few widgets we hold.
    for (int i = 1; i < widgetCount_; i++) {
        Widget* w = widgets_[i];
        int j = i - 1;
        while (j >= 0 && widgets_[j]->zOrder() < w->zOrder()) {
            widgets_[j + 1] = widgets_[j];
            j--;
        }
        widgets_[j + 1] = w;
    }
}

void TouchDispatcher::dispatch(const TouchState* touchPoints, int count,
                                uint32_t timestamp) {
    int n = (count > MAX_TOUCH_POINTS) ? MAX_TOUCH_POINTS : count;

    for (int i = 0; i < MAX_TOUCH_POINTS; i++) {
        bool wasPressed = prevPressed_[i];
        bool isPressed  = (i < n) ? touchPoints[i].pressed : false;

        TouchEvent event;
        event.fingerId  = static_cast<uint8_t>(i);
        event.timestamp = timestamp;

        if (!wasPressed && isPressed) {
            event.x     = touchPoints[i].x;
            event.y     = touchPoints[i].y;
            event.phase = TouchPhase::Began;
            deliverBegan(event);
        } else if (wasPressed && isPressed) {
            event.x     = touchPoints[i].x;
            event.y     = touchPoints[i].y;
            event.phase = TouchPhase::Moved;
            if (tracks_[i].active) {
                deliverMoved(event, tracks_[i]);
            }
        } else if (wasPressed && !isPressed) {
            event.x     = prevX_[i];
            event.y     = prevY_[i];
            event.phase = TouchPhase::Ended;
            if (tracks_[i].active) {
                deliverEnded(event, tracks_[i]);
            }
        }

        prevPressed_[i] = isPressed;
        if (isPressed) {
            prevX_[i] = touchPoints[i].x;
            prevY_[i] = touchPoints[i].y;
        }
    }
}

void TouchDispatcher::dispatchEvent(const TouchEvent& event) {
    uint8_t id = event.fingerId;
    if (id >= MAX_TOUCH_POINTS) return;

    switch (event.phase) {
        case TouchPhase::Began:
            deliverBegan(event);
            break;
        case TouchPhase::Moved:
            if (tracks_[id].active) {
                deliverMoved(event, tracks_[id]);
            }
            break;
        case TouchPhase::Ended:
            if (tracks_[id].active) {
                deliverEnded(event, tracks_[id]);
            }
            break;
        case TouchPhase::Cancelled:
            if (tracks_[id].active) {
                deliverCancelled(event, tracks_[id]);
            }
            break;
    }
}

void TouchDispatcher::deliverBegan(const TouchEvent& event) {
    uint8_t id = event.fingerId;
    for (int i = 0; i < widgetCount_; i++) {
        Widget* w = widgets_[i];
        if (!w->visible() || !w->enabled()) continue;
        if (w->handleTouch(event)) {
            TouchTrack& t = tracks_[id];
            t.active     = true;
            t.captured   = w;
            t.startX     = event.x;
            t.startY     = event.y;
            t.lastX      = event.x;
            t.lastY      = event.y;
            t.startTime  = event.timestamp;
            t.dragging   = false;
            t.longPressed = false;
            return;
        }
    }
}

void TouchDispatcher::deliverMoved(const TouchEvent& event, TouchTrack& track) {
    int dx = event.x - track.lastX;
    int dy = event.y - track.lastY;

    track.captured->handleTouch(event);

    if (!track.dragging) {
        int totalDx = event.x - track.startX;
        int totalDy = event.y - track.startY;
        if (std::abs(totalDx) > kDragThreshold ||
            std::abs(totalDy) > kDragThreshold) {
            track.dragging = true;
            track.captured->onDragBegan(event);
        }
    }
    if (track.dragging) {
        track.captured->onDragMoved(event, dx, dy);
    }

    if (!track.longPressed && event.timestamp > 0 &&
        (event.timestamp - track.startTime) >= kLongPressMs) {
        track.longPressed = true;
        track.captured->onLongPress(event);
    }

    track.lastX = event.x;
    track.lastY = event.y;
}

void TouchDispatcher::deliverEnded(const TouchEvent& event, TouchTrack& track) {
    track.captured->handleTouch(event);

    if (!track.dragging) {
        int totalDx = event.x - track.startX;
        int totalDy = event.y - track.startY;
        if (std::abs(totalDx) <= kTapMaxMovement &&
            std::abs(totalDy) <= kTapMaxMovement) {
            uint8_t id = event.fingerId;
            uint32_t dt = event.timestamp - lastTapTime_[id];
            int dx = event.x - lastTapX_[id];
            int dy = event.y - lastTapY_[id];
            bool isDoubleTap = (dt > 0 && dt <= kDoubleTapMs &&
                                std::abs(dx) <= kDoubleTapMaxDist &&
                                std::abs(dy) <= kDoubleTapMaxDist);
            track.captured->onTap(event);
            if (isDoubleTap)
                track.captured->onDoubleTap(event);
            lastTapX_[id] = event.x;
            lastTapY_[id] = event.y;
            lastTapTime_[id] = event.timestamp;
        }
    } else {
        track.captured->onDragEnded(event);
    }

    track.active   = false;
    track.captured = nullptr;
}

void TouchDispatcher::deliverCancelled(const TouchEvent& event,
                                        TouchTrack& track) {
    track.captured->handleTouch(event);
    track.active   = false;
    track.captured = nullptr;
}

} // namespace ui
