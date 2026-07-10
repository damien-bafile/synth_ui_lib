#include "transport_bar.h"
#include "font.h"
#include <math.h>

namespace ui {

static uint16_t brighten(uint16_t color, uint8_t amount) {
    uint8_t r = (color >> 11) & 0x1F;
    uint8_t g = (color >> 5) & 0x3F;
    uint8_t b = color & 0x1F;
    r = r + ((31 - r) * amount / 255);
    g = g + ((63 - g) * amount / 255);
    b = b + ((31 - b) * amount / 255);
    return (r << 11) | (g << 5) | b;
}

TransportBar::TransportBar(int x, int y, int w, int h)
    : x_(x), y_(y), w_(w), h_(h),
      btns_{{0, 0, 0, 0, nullptr, PLAY, BG_DARK},
            {0, 0, 0, 0, nullptr, TEXT_DIM, BG_DARK},
            {0, 0, 0, 0, nullptr, RECORD, BG_DARK},
            {0, 0, 0, 0, nullptr, ACCENT_3, BG_DARK}} {
    for (int i = 0; i < 4; i++)
        btns_[i].setBounds(btnX(i), btnY(), BTN_W, BTN_H);
}

void TransportBar::setSwatches(const uint16_t* colors, int count) {
    if (count < 0) count = 0;
    if (count > MAX_SWATCHES) count = MAX_SWATCHES;
    for (int i = 0; i < count; i++) swatches_[i] = colors[i];
    swatchCount_ = count;
}

void TransportBar::setNoteActivity(const uint8_t* intensities, int count) {
    if (count < 0) count = 0;
    if (count > MAX_SWATCHES) count = MAX_SWATCHES;
    noteActivityCount_ = count;
    if (intensities) {
        for (int i = 0; i < count; i++) noteActivity_[i] = intensities[i];
    } else {
        noteActivityCount_ = 0;
    }
}

void TransportBar::setBpmIndicator(BpmAnimMode mode, float stepProgress, uint8_t currentStep) {
    bpmAnimMode_ = mode;
    stepProgress_ = stepProgress;
    currentStep_ = currentStep;
}

void TransportBar::draw(Framebuffer& fb, PlayState state, bool recording,
                        const char* leftText, const char* rightText) {
    btns_[BTN_PLAY].setColors(PLAY, BG_DARK);
    btns_[BTN_STOP].setColors(TEXT_DIM, BG_DARK);
    btns_[BTN_REC].setColors(RECORD, BG_DARK);
    btns_[BTN_MENU].setColors(ACCENT_3, BG_DARK);

    fb.fillRect(x_, y_, w_, 1, BG_MID);

    int iy = btnY() + (BTN_H - 7) / 2;

    bool running = (state != PlayState::STOPPED);
    SynthIcon play_icon = (state == PlayState::PAUSED) ? SynthIcon::PAUSE
                                                       : SynthIcon::PLAY;
    uint16_t play_color = (state == PlayState::PAUSED) ? ACCENT_2 : PLAY;
    btns_[BTN_PLAY].draw(fb, running);
    fb.drawSynthIcon(btnX(BTN_PLAY) + (BTN_W - 5) / 2, iy, play_icon,
                     running ? BG_DARK : play_color,
                     running ? play_color : BG_DARK);

    bool stopped = (state == PlayState::STOPPED);
    btns_[BTN_STOP].draw(fb, stopped);
    fb.drawSynthIcon(btnX(BTN_STOP) + (BTN_W - 5) / 2, iy, SynthIcon::STOP,
                     stopped ? BG_DARK : TEXT_DIM,
                     stopped ? TEXT_DIM : BG_DARK);

    btns_[BTN_REC].draw(fb, recording);
    fb.drawSynthIcon(btnX(BTN_REC) + (BTN_W - 5) / 2, iy, SynthIcon::RECORD,
                     recording ? BG_DARK : RECORD,
                     recording ? RECORD : BG_DARK);

    // ── Swatch row with note-activity animation ──────────────────────
    int sy = y_ + (h_ - SWATCH_SIZE) / 2;
    uint8_t pulseAmt = 0;
    if (bpmAnimMode_ == BpmAnimMode::PULSE && stepProgress_ >= 0.0f) {
        float phase = stepProgress_ * 3.14159f * 2.0f;
        pulseAmt = (uint8_t)((sinf(phase) * 0.3f + 0.7f) * 80.0f);
    }
    for (int i = 0; i < swatchCount_; i++) {
        int sx = x_ + SWATCH_X + i * SWATCH_STEP;
        uint16_t base = swatches_[i];
        uint8_t act = (i < noteActivityCount_) ? noteActivity_[i] : 0;

        uint8_t total = act;
        if (pulseAmt > 0 && base != BG_SURFACE && base != TEXT_DIM)
            total = (total > pulseAmt) ? total : pulseAmt;

        if (total > 0) {
            int extra = (int)total * 3 / 255;
            int size = SWATCH_SIZE + extra;
            int ox = sx - extra / 2;
            int oy = sy - extra / 2;
            uint16_t col = brighten(base, total);
            fb.fillRect(ox, oy, size, size, col);
        } else {
            fb.fillRect(sx, sy, SWATCH_SIZE, SWATCH_SIZE, base);
        }
    }

    // ── BPM timing indicator ─────────────────────────────────────────
    if (swatchCount_ > 0 && stepProgress_ >= 0.0f) {
        switch (bpmAnimMode_) {
        case BpmAnimMode::SWEEP_BAR: {
            int sweepLen = swatchCount_ * SWATCH_STEP - SWATCH_STEP + SWATCH_SIZE;
            int barX = x_ + SWATCH_X + (int)(stepProgress_ * sweepLen);
            fb.fillRect(barX, y_ + h_ - 2, 2, 1, ACCENT_2);
            break;
        }
        case BpmAnimMode::PULSE:
            break;
        case BpmAnimMode::WALKING_DOT:
            if (currentStep_ < swatchCount_) {
                int dx = x_ + SWATCH_X + currentStep_ * SWATCH_STEP + SWATCH_STEP / 2 - 1;
                fb.fillRect(dx, y_ + h_ - 4, 3, 3, ACCENT_2);
            }
            break;
        case BpmAnimMode::STEP_PROGRESS:
            for (int i = 0; i < swatchCount_; i++) {
                int sx2 = x_ + SWATCH_X + i * SWATCH_STEP;
                int fillW = (int)(SWATCH_SIZE * stepProgress_);
                if (fillW > 0)
                    fb.fillRect(sx2, y_ + h_ - 2, fillW, 1, ACCENT_2);
            }
            break;
        default:
            break;
        }
    }

    if (menuButton_) {
        btns_[BTN_MENU].draw(fb, false);
        fb.drawSynthIcon(btnX(BTN_MENU) + (BTN_W - 5) / 2, iy,
                         SynthIcon::MENU, ACCENT_3, BG_DARK);
    }

    int ty = textY();
    if (leftText) {
        fb.drawText(x_ + LEFT_TEXT_X, ty, leftText, TEXT_DIM, BG_DARK);
    }
    if (rightText) {
        int tw = Framebuffer::textWidth(rightText);
        int rx = x_ + w_ - tw - 4;
        if (menuButton_) rx -= BTN_W + BTN_GAP + 2;
        fb.drawText(rx, ty, rightText, TEXT_DIM, BG_DARK);
        if (statusDot_) {
            fb.fillRect(rx + tw + 4, ty + 9, 2, 2, statusDotColor_);
        }
    }
}

void TransportBar::addTo(TouchDispatcher& d) {
    for (int i = 0; i < 3; i++) d.add(&btns_[i]);
    if (menuButton_) d.add(&btns_[BTN_MENU]);
}

int TransportBar::tappedButton() {
    for (int i = 0; i < 4; i++)
        if (btns_[i].wasTapped()) return i;
    return -1;
}

} // namespace ui
