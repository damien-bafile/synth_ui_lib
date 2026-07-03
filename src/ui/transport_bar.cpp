#include "transport_bar.h"
#include "font.h"

namespace ui {

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

void TransportBar::draw(Framebuffer& fb, PlayState state, bool recording,
                        const char* leftText, const char* rightText) {
    // Re-apply palette colors so a runtime theme switch takes effect.
    btns_[BTN_PLAY].setColors(PLAY, BG_DARK);
    btns_[BTN_STOP].setColors(TEXT_DIM, BG_DARK);
    btns_[BTN_REC].setColors(RECORD, BG_DARK);
    btns_[BTN_MENU].setColors(ACCENT_3, BG_DARK);

    // 1px divider along the top edge
    fb.fillRect(x_, y_, w_, 1, BG_MID);

    int iy = btnY() + (BTN_H - 7) / 2;

    // Play/pause: pressed while transport is running
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

    // Swatch row, vertically centered
    int sy = y_ + (h_ - SWATCH_SIZE) / 2;
    for (int i = 0; i < swatchCount_; i++) {
        fb.fillRect(x_ + SWATCH_X + i * SWATCH_STEP, sy,
                    SWATCH_SIZE, SWATCH_SIZE, swatches_[i]);
    }

    if (menuButton_) {
        btns_[BTN_MENU].draw(fb, false);
        fb.drawSynthIcon(btnX(BTN_MENU) + (BTN_W - 5) / 2, iy,
                         SynthIcon::MENU, ACCENT_3, BG_DARK);
    }

    int ty = textY();
    if (leftText) {
        fb.drawText(x_ + LEFT_TEXT_X, ty, leftText, TEXT_DIM, BG_DARK);
        if (statusDot_) {
            int dx = x_ + LEFT_TEXT_X + Framebuffer::textWidth(leftText) + 4;
            fb.fillRect(dx, ty + 9, 2, 2, statusDotColor_);
        }
    }
    if (rightText) {
        int tw = Framebuffer::textWidth(rightText);
        int rx = x_ + w_ - tw - 4;
        if (menuButton_) rx -= BTN_W + BTN_GAP + 2;
        fb.drawText(rx, ty, rightText, TEXT_DIM, BG_DARK);
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
