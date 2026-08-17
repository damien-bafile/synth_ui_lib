#include "midi_learn_badge.h"
#include "font.h"

namespace ui {

void MidiLearnBadge::draw(Framebuffer& fb, int x, int y, bool learning,
                          uint32_t ticks) const {
    if (!learning) {
        fb.drawCircle(x, y, RADIUS, inactiveColor_);
        return;
    }

    bool bright = isPulseBright(ticks);
    uint16_t fillColor = bright ? activeColor_ : inactiveColor_;
    fb.fillCircle(x, y, RADIUS, fillColor);
    fb.drawCircle(x, y, RADIUS, activeColor_);
    fb.drawChar(x - FONT_W / 2, y - FONT_H / 2, 'L',
               bright ? BG_DARK : activeColor_, fillColor);
}

} // namespace ui
