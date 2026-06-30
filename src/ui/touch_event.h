#pragma once
#include <cstdint>

namespace ui {

enum class TouchPhase : uint8_t {
    Began,
    Moved,
    Ended,
    Cancelled
};

struct TouchEvent {
    int x;
    int y;
    TouchPhase phase;
    uint8_t fingerId;
    uint32_t timestamp;
};

} // namespace ui
