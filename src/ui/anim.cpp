#include "anim.h"
#include <cmath>

namespace ui {

namespace {
constexpr float kPi = 3.14159265358979323846f;

float easeSineInOut(float t) noexcept { return -(std::cos(kPi * t) - 1.0f) * 0.5f; }

float easeExpoOut(float t) noexcept {
    return t >= 1.0f ? 1.0f : 1.0f - std::pow(2.0f, -10.0f * t);
}

float easeElasticOut(float t) noexcept {
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    const float c4 = (2.0f * kPi) / 3.0f;
    return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
}

float easeBounceOut(float t) noexcept {
    const float n1 = 7.5625f;
    const float d1 = 2.75f;
    if (t < 1.0f / d1) return n1 * t * t;
    if (t < 2.0f / d1) {
        t -= 1.5f / d1;
        return n1 * t * t + 0.75f;
    }
    if (t < 2.5f / d1) {
        t -= 2.25f / d1;
        return n1 * t * t + 0.9375f;
    }
    t -= 2.625f / d1;
    return n1 * t * t + 0.984375f;
}
} // namespace

float applyEasing(Easing e, float t) noexcept {
    t = clamp(t, 0.0f, 1.0f);
    switch (e) {
    case Easing::LINEAR:      return easeLinear(t);
    case Easing::QUAD_IN:     return easeQuadIn(t);
    case Easing::QUAD_OUT:    return easeQuadOut(t);
    case Easing::QUAD_INOUT:  return easeQuadInOut(t);
    case Easing::CUBIC_IN:    return easeCubicIn(t);
    case Easing::CUBIC_OUT:   return easeCubicOut(t);
    case Easing::CUBIC_INOUT: return easeCubicInOut(t);
    case Easing::BACK_OUT:    return easeBackOut(t);
    case Easing::SINE_INOUT:  return easeSineInOut(t);
    case Easing::EXPO_OUT:    return easeExpoOut(t);
    case Easing::ELASTIC_OUT: return easeElasticOut(t);
    case Easing::BOUNCE_OUT:  return easeBounceOut(t);
    }
    return t;
}

// ---- Timeline ----

int Timeline::findFree() noexcept {
    for (int i = 0; i < MAX_ANIMATIONS; ++i) {
        if (tracks_[i].state == State::IDLE || tracks_[i].state == State::DONE) {
            return i;
        }
    }
    return -1;
}

const Timeline::Track* Timeline::resolve(Handle h) const noexcept {
    if (h < 0) return nullptr;
    const int slot = slotOf(h);
    if (slot < 0 || slot >= MAX_ANIMATIONS) return nullptr;
    const Track& t = tracks_[slot];
    if (t.generation != genOf(h) || t.state == State::IDLE) return nullptr;
    return &t;
}

Timeline::Track* Timeline::resolve(Handle h) noexcept {
    return const_cast<Track*>(static_cast<const Timeline*>(this)->resolve(h));
}

Timeline::Handle Timeline::add(float from, float to, uint32_t durationMs,
                               Easing easing, uint32_t delayMs,
                               LoopMode loop) noexcept {
    const int slot = findFree();
    if (slot < 0) return INVALID;
    Track& t = tracks_[slot];
    ++t.generation; // invalidate any handle to the previous occupant
    t.state = State::PENDING;
    t.from = from;
    t.to = to;
    t.current = from;
    t.durationMs = durationMs;
    t.delayMs = delayMs;
    t.easing = easing;
    t.loop = loop;
    t.waitOn = INVALID;
    t.startMs = 0;
    return makeHandle(slot, t.generation);
}

Timeline::Handle Timeline::then(Handle after, float to, uint32_t durationMs,
                                Easing easing) noexcept {
    if (resolve(after) == nullptr) return INVALID;
    const int slot = findFree();
    if (slot < 0) return INVALID;
    Track& t = tracks_[slot];
    ++t.generation;
    t.state = State::WAIT;
    t.from = 0.0f; // filled in from the parent's final value on activation
    t.to = to;
    t.current = 0.0f;
    t.durationMs = durationMs;
    t.delayMs = 0;
    t.easing = easing;
    t.loop = LoopMode::ONCE;
    t.waitOn = after;
    t.startMs = 0;
    return makeHandle(slot, t.generation);
}

void Timeline::advance(Track& t, uint32_t nowMs) noexcept {
    switch (t.state) {
    case State::PENDING:
        t.startMs = nowMs + t.delayMs;
        t.state = t.delayMs > 0 ? State::DELAY : State::RUNNING;
        break;
    case State::WAIT: {
        const Track* parent = resolve(t.waitOn);
        if (parent == nullptr || parent->state == State::DONE) {
            t.from = parent != nullptr ? parent->current : t.from;
            t.current = t.from;
            t.startMs = nowMs;
            t.state = State::RUNNING;
        }
        break;
    }
    default:
        break;
    }

    if (t.state == State::DELAY) {
        if (nowMs >= t.startMs) {
            t.state = State::RUNNING;
        } else {
            t.current = t.from;
            return;
        }
    }

    if (t.state != State::RUNNING) return;

    if (t.durationMs == 0) {
        t.current = t.to;
        t.state = State::DONE;
        return;
    }
    const uint32_t elapsed = nowMs - t.startMs;
    if (t.loop == LoopMode::ONCE && elapsed >= t.durationMs) {
        t.current = t.to;
        t.state = State::DONE;
        return;
    }
    float p;
    if (t.loop == LoopMode::ONCE) {
        p = static_cast<float>(elapsed) / static_cast<float>(t.durationMs);
    } else {
        const uint32_t cycle = elapsed % t.durationMs;
        p = static_cast<float>(cycle) / static_cast<float>(t.durationMs);
        if (t.loop == LoopMode::PINGPONG && ((elapsed / t.durationMs) & 1u)) {
            p = 1.0f - p;
        }
    }
    t.current = lerp(t.from, t.to, applyEasing(t.easing, p));
}

void Timeline::tick(uint32_t nowMs) noexcept {
    for (int i = 0; i < MAX_ANIMATIONS; ++i) {
        Track& t = tracks_[i];
        if (t.state == State::IDLE || t.state == State::DONE) continue;
        advance(t, nowMs);
    }
}

float Timeline::value(Handle h) const noexcept {
    const Track* t = resolve(h);
    return t != nullptr ? t->current : 0.0f;
}

bool Timeline::active(Handle h) const noexcept {
    const Track* t = resolve(h);
    if (t == nullptr) return false;
    return t->state == State::PENDING || t->state == State::DELAY ||
           t->state == State::WAIT || t->state == State::RUNNING;
}

void Timeline::cancel(Handle h) noexcept {
    Track* t = resolve(h);
    if (t != nullptr) t->state = State::IDLE;
}

void Timeline::clear() noexcept {
    for (int i = 0; i < MAX_ANIMATIONS; ++i) tracks_[i].state = State::IDLE;
}

int Timeline::activeCount() const noexcept {
    int n = 0;
    for (int i = 0; i < MAX_ANIMATIONS; ++i) {
        const State s = tracks_[i].state;
        if (s != State::IDLE && s != State::DONE) ++n;
    }
    return n;
}

} // namespace ui
