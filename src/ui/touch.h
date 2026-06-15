#pragma once
#include <cstdint>

namespace ui {

struct TouchState {
    int x;
    int y;
    bool pressed;
};

class TouchCalibration {
public:
    TouchCalibration();
    void setIdentity();
    void setMatrix(float a, float b, float c, float d, float e, float f);
    TouchState transform(int rawX, int rawY, bool rawPressed) const;

private:
    float a_, b_, c_, d_, e_, f_;
};

} // namespace ui
