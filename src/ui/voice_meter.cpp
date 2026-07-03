#include "voice_meter.h"

namespace ui {

void VoiceMeter::draw(Framebuffer& fb, const bool* active, int count) const {
    for (int i = 0; i < count; i++) {
        int bx = x_ + i * pitch_;
        bool on = active[i];
        fb.fillRect(bx, y_, boxW_, boxH_, on ? onColor_ : offColor_);
        if (border_ == BorderStyle::ALL ||
            (border_ == BorderStyle::ACTIVE_ONLY && on)) {
            fb.drawRect(bx, y_, boxW_, boxH_, borderColor_);
        }
    }
}

} // namespace ui
