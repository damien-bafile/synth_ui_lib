#pragma once
#include <cstdint>
#include "easing.h"
#include "tween.h"

namespace ui {

// Fixed-capacity, heap-free animation manager for fire-and-forget and
// sequenced value animations. Unlike a Tween (which the caller stores and
// reads directly), a Timeline owns a pool of animations you address by
// Handle: add() one, tick() the whole pool once per frame with the shared
// millisecond clock, then value(handle) the current scalars into your draws.
//
// Widgets stay stateless: the Timeline produces floats, nothing more. A
// finished (ONCE) animation is reclaimable so a long-running UI can churn
// animations without exhausting the pool. Handles carry a generation tag, so
// reading a handle whose slot has since been reused safely returns 0.
class Timeline {
public:
    static constexpr int MAX_ANIMATIONS = 32; // tune per target
    using Handle = int;
    static constexpr Handle INVALID = -1;

    Timeline() = default;

    // Queue an animation. The clock is captured on the first tick() after this
    // call, so add() needs no timestamp. Returns INVALID if the pool is full.
    Handle add(float from, float to, uint32_t durationMs,
               Easing easing = Easing::CUBIC_OUT, uint32_t delayMs = 0,
               LoopMode loop = LoopMode::ONCE) noexcept;

    // Chain a follow-on that starts from `after`'s final value once it
    // finishes. `after` should be a ONCE animation. Returns INVALID if the
    // pool is full or `after` is not a live handle.
    Handle then(Handle after, float to, uint32_t durationMs,
                Easing easing = Easing::CUBIC_OUT) noexcept;

    void tick(uint32_t nowMs) noexcept; // advance every track once per frame

    float value(Handle h) const noexcept; // current value (0 if handle stale)
    bool active(Handle h) const noexcept;  // delayed, waiting, or running
    void cancel(Handle h) noexcept;        // free the slot immediately
    void clear() noexcept;                 // free all slots
    int activeCount() const noexcept;

private:
    enum class State : uint8_t { IDLE, PENDING, DELAY, WAIT, RUNNING, DONE };

    struct Track {
        State state = State::IDLE;
        float from = 0.0f;
        float to = 0.0f;
        float current = 0.0f;
        uint32_t startMs = 0;    // when RUNNING begins (PENDING resolves this)
        uint32_t durationMs = 0;
        uint32_t delayMs = 0;    // PENDING → DELAY offset
        Easing easing = Easing::LINEAR;
        LoopMode loop = LoopMode::ONCE;
        Handle waitOn = INVALID; // WAIT: parent handle to chain after
        uint16_t generation = 0; // bumped on reuse to invalidate old handles
    };

    static Handle makeHandle(int slot, uint16_t gen) noexcept {
        return static_cast<Handle>((static_cast<uint32_t>(gen) << 16) |
                                   static_cast<uint32_t>(slot & 0xFFFF));
    }
    static int slotOf(Handle h) noexcept { return h & 0xFFFF; }
    static uint16_t genOf(Handle h) noexcept {
        return static_cast<uint16_t>((static_cast<uint32_t>(h) >> 16) & 0xFFFF);
    }

    int findFree() noexcept;             // an IDLE or DONE slot, or -1
    const Track* resolve(Handle h) const noexcept;
    Track* resolve(Handle h) noexcept;
    void advance(Track& t, uint32_t nowMs) noexcept;

    Track tracks_[MAX_ANIMATIONS];
};

} // namespace ui
