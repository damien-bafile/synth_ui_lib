#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"

namespace ui {

// Small composable badge a screen draws next to whatever control is
// currently MIDI-learn-armed. Not tied to any one widget, not touch-
// interactive, and stateless -- the caller supplies its own millisecond
// clock (same convention as the animation system) so the pulse stays in
// sync with everything else on screen.
class MidiLearnBadge {
public:
    static constexpr int RADIUS = 5;
    static constexpr uint32_t PULSE_PERIOD_MS = 500;

    MidiLearnBadge(uint16_t activeColor = RECORD,
                   uint16_t inactiveColor = TEXT_DIM) noexcept
        : activeColor_(activeColor), inactiveColor_(inactiveColor) {}

    // learning=false draws a dim idle ring. learning=true pulses between
    // active and inactive color on a PULSE_PERIOD_MS cadence and fills
    // the badge with an "L" glyph.
    void draw(Framebuffer& fb, int x, int y, bool learning, uint32_t ticks) const;

    void setColors(uint16_t activeColor, uint16_t inactiveColor) noexcept {
        activeColor_ = activeColor; inactiveColor_ = inactiveColor;
    }

    static bool isPulseBright(uint32_t ticks) noexcept {
        return (ticks % (PULSE_PERIOD_MS * 2)) < PULSE_PERIOD_MS;
    }

private:
    uint16_t activeColor_, inactiveColor_;
};

} // namespace ui
