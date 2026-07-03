#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"

namespace ui {

// Row of per-voice activity boxes (filled while the voice sounds).
// Border style is configurable: a border on active boxes only (synth
// footer look) or on every box (sample screen look).
class VoiceMeter {
public:
    enum class BorderStyle : uint8_t { NONE, ACTIVE_ONLY, ALL };

    VoiceMeter(int x, int y, int boxW, int boxH, int pitch,
               uint16_t onColor = PLAY, uint16_t offColor = BG_SURFACE,
               BorderStyle border = BorderStyle::NONE,
               uint16_t borderColor = WHITE)
        : x_(x), y_(y), boxW_(boxW), boxH_(boxH), pitch_(pitch),
          onColor_(onColor), offColor_(offColor),
          border_(border), borderColor_(borderColor) {}

    void draw(Framebuffer& fb, const bool* active, int count) const;

    void setPosition(int x, int y) noexcept { x_ = x; y_ = y; }
    void setColors(uint16_t on, uint16_t off, uint16_t border) noexcept {
        onColor_ = on; offColor_ = off; borderColor_ = border;
    }

private:
    int x_, y_, boxW_, boxH_, pitch_;
    uint16_t onColor_, offColor_;
    BorderStyle border_;
    uint16_t borderColor_;
};

} // namespace ui
