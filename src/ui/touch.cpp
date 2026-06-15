#include "touch.h"

namespace ui {

TouchCalibration::TouchCalibration()
    : a_(1.0f), b_(0.0f), c_(0.0f), d_(0.0f), e_(1.0f), f_(0.0f) {}

void TouchCalibration::setIdentity() {
    a_ = 1.0f; b_ = 0.0f; c_ = 0.0f;
    d_ = 0.0f; e_ = 1.0f; f_ = 0.0f;
}

void TouchCalibration::setMatrix(float a, float b, float c, float d, float e, float f) {
    a_ = a; b_ = b; c_ = c;
    d_ = d; e_ = e; f_ = f;
}

TouchState TouchCalibration::transform(int rawX, int rawY, bool rawPressed) const {
    TouchState state;
    state.x = static_cast<int>(a_ * rawX + b_ * rawY + c_);
    state.y = static_cast<int>(d_ * rawX + e_ * rawY + f_);
    state.pressed = rawPressed;
    return state;
}

} // namespace ui
