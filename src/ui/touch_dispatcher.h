#pragma once
#include "touch_event.h"
#include "touch.h"
#include "widget.h"
#include <cstdint>

namespace ui {

class TouchDispatcher {
public:
    static constexpr int MAX_WIDGETS = 32;
    static constexpr int MAX_TOUCH_POINTS = 2;

    static constexpr int kDragThreshold = 4;
    static constexpr uint32_t kLongPressMs = 800;
    static constexpr int kTapMaxMovement = 8;

    TouchDispatcher();

    void add(Widget* widget);
    void remove(const Widget* widget);
    void clear();
    int count() const { return widgetCount_; }

    // Convert TouchState[] to TouchEvents and route
    void dispatch(const TouchState* touchPoints, int count,
                  uint32_t timestamp = 0);

    // Direct TouchEvent injection
    void dispatchEvent(const TouchEvent& event);

private:
    struct TouchTrack {
        bool active = false;
        Widget* captured = nullptr;
        int startX = 0, startY = 0;
        int lastX = 0, lastY = 0;
        uint32_t startTime = 0;
        bool dragging = false;
        bool longPressed = false;
    };

    Widget* widgets_[MAX_WIDGETS];
    int widgetCount_ = 0;
    bool prevPressed_[MAX_TOUCH_POINTS] = {};
    int prevX_[MAX_TOUCH_POINTS] = {};
    int prevY_[MAX_TOUCH_POINTS] = {};
    TouchTrack tracks_[MAX_TOUCH_POINTS];

    int findWidget(const Widget* widget) const;
    void sortByZOrder();

    void deliverBegan(const TouchEvent& event);
    void deliverMoved(const TouchEvent& event, TouchTrack& track);
    void deliverEnded(const TouchEvent& event, TouchTrack& track);
    void deliverCancelled(const TouchEvent& event, TouchTrack& track);
};

} // namespace ui
