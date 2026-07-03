#include "popup_slider.h"
#include <cstdio>

namespace ui {

// Right-side layout: [value][-][+][X] when step buttons are enabled,
// [value][X] otherwise. The slider fills the remaining left span.
static constexpr int BTN_W = 18;

PopupSlider::PopupSlider(int x, int y, int w, int h, float min, float max,
                         const char* label, uint16_t accent)
    : x_(x), y_(y), w_(w), h_(h), label_(label), accent_(accent),
      min_(min), max_(max),
      slider_(x + 6, y + 4, w - 76, h - 8, min, max, nullptr,
              accent, BG_SURFACE, false),
      minusBtn_(x + w - 62, y + 3, BTN_W, h - 6, "-", accent, BG_SURFACE),
      plusBtn_(x + w - 42, y + 3, BTN_W, h - 6, "+", accent, BG_SURFACE),
      closeBtn_(x + w - 22, y + 3, BTN_W, h - 6, "X", RED, BG_SURFACE) {}

void PopupSlider::draw(Framebuffer& fb, float value, const char* valueText) {
    if (!open_) return;
    lastValue_ = value;

    // Re-apply palette colors so a runtime theme switch takes effect.
    slider_.setColors(accent_, BG_SURFACE);
    minusBtn_.setColors(accent_, BG_SURFACE);
    plusBtn_.setColors(accent_, BG_SURFACE);
    closeBtn_.setColors(RED, BG_SURFACE);

    bool steps = (step_ > 0.0f);
    slider_.setBounds(x_ + 6, y_ + 4, w_ - (steps ? 116 : 76), h_ - 8);

    fb.fillRect(x_, y_, w_, h_, BG_SURFACE);
    fb.drawRect(x_, y_, w_, h_, accent_);
    if (label_) fb.drawText(x_, y_ - 10, label_, accent_, BG_DARK);

    slider_.draw(fb, value);

    char buf[16];
    const char* txt = valueText;
    if (!txt) {
        snprintf(buf, sizeof(buf), "%d", (int)(value + 0.5f));
        txt = buf;
    }
    int ty = y_ + (h_ - 8) / 2;
    if (steps) {
        int tw = Framebuffer::textWidth(txt);
        fb.drawText(x_ + w_ - 66 - tw, ty, txt, TEXT, BG_SURFACE);
        minusBtn_.draw(fb, false);
        plusBtn_.draw(fb, false);
    } else {
        fb.drawText(x_ + w_ - 56, ty, txt, TEXT, BG_SURFACE);
    }

    closeBtn_.draw(fb, false);
}

void PopupSlider::addTo(TouchDispatcher& disp) {
    if (!open_) return;
    disp.add(&slider_);
    if (step_ > 0.0f) {
        disp.add(&minusBtn_);
        disp.add(&plusBtn_);
    }
    disp.add(&closeBtn_);
}

bool PopupSlider::valueChanged() {
    if (!open_) return false;
    if (closeBtn_.wasTapped()) open_ = false;

    bool stepped = false;
    if (step_ > 0.0f) {
        float v = lastValue_;
        if (minusBtn_.wasTapped()) { v -= step_; stepped = true; }
        if (plusBtn_.wasTapped())  { v += step_; stepped = true; }
        if (stepped) {
            if (v < min_) v = min_;
            if (v > max_) v = max_;
            slider_.setValue(v);
        }
    }
    return slider_.wasChanged() || stepped;
}

} // namespace ui
