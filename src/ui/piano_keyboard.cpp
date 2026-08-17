#include "piano_keyboard.h"

namespace ui {

PianoKeyboard::PianoKeyboard(int x, int y, int w, int h,
                             int startNote, int numKeys,
                             uint16_t whiteColor, uint16_t blackColor,
                             uint16_t activeColor) noexcept
    : startNote_(startNote), numKeys_(numKeys),
      whiteColor_(whiteColor), blackColor_(blackColor), activeColor_(activeColor) {
    setBounds(x, y, w, h);
    if (numKeys_ > MAX_KEYS) numKeys_ = MAX_KEYS;
    if (numKeys_ < 1) numKeys_ = 1;
}

void PianoKeyboard::setRange(int startNote, int numKeys) noexcept {
    startNote_ = startNote;
    numKeys_ = (numKeys > MAX_KEYS) ? MAX_KEYS : ((numKeys < 1) ? 1 : numKeys);
}

bool PianoKeyboard::isBlackKey(int note) noexcept {
    int pc = note % 12;
    if (pc < 0) pc += 12;
    return pc == 1 || pc == 3 || pc == 6 || pc == 8 || pc == 10;
}

int PianoKeyboard::layoutKeys(int keyX[MAX_KEYS], int keyW[MAX_KEYS],
                              bool isBlack[MAX_KEYS]) const {
    int numWhite = 0;
    for (int i = 0; i < numKeys_; i++) {
        isBlack[i] = isBlackKey(startNote_ + i);
        if (!isBlack[i]) numWhite++;
    }
    int whiteKeyW = (numWhite > 0) ? (w_ / numWhite) : w_;
    if (whiteKeyW < 1) whiteKeyW = 1;
    int blackKeyW = (whiteKeyW * 3) / 5;
    if (blackKeyW < 1) blackKeyW = 1;

    int whiteIndex = 0;
    for (int i = 0; i < numKeys_; i++) {
        if (isBlack[i]) {
            keyX[i] = x_ + whiteIndex * whiteKeyW - blackKeyW / 2;
            keyW[i] = blackKeyW;
        } else {
            keyX[i] = x_ + whiteIndex * whiteKeyW;
            keyW[i] = whiteKeyW;
            whiteIndex++;
        }
    }
    return whiteKeyW;
}

int PianoKeyboard::hitTest(int px, int py) const {
    if (py < y_ || py >= y_ + h_) return -1;

    int keyX[MAX_KEYS], keyW[MAX_KEYS];
    bool isBlack[MAX_KEYS];
    layoutKeys(keyX, keyW, isBlack);
    int blackKeyH = (h_ * 3) / 5;

    // Black keys are drawn on top; check them first.
    for (int i = 0; i < numKeys_; i++) {
        if (!isBlack[i]) continue;
        if (py < y_ + blackKeyH && px >= keyX[i] && px < keyX[i] + keyW[i]) {
            return startNote_ + i;
        }
    }
    for (int i = 0; i < numKeys_; i++) {
        if (isBlack[i]) continue;
        if (px >= keyX[i] && px < keyX[i] + keyW[i]) {
            return startNote_ + i;
        }
    }
    return -1;
}

void PianoKeyboard::draw(Framebuffer& fb, const bool activeNotes[128]) const {
    int keyX[MAX_KEYS], keyW[MAX_KEYS];
    bool isBlack[MAX_KEYS];
    layoutKeys(keyX, keyW, isBlack);
    int blackKeyH = (h_ * 3) / 5;

    for (int i = 0; i < numKeys_; i++) {
        if (isBlack[i]) continue;
        int note = startNote_ + i;
        bool active = (note >= 0 && note < 128) && activeNotes && activeNotes[note];
        fb.fillRect(keyX[i], y_, keyW[i], h_, active ? activeColor_ : whiteColor_);
        fb.drawRect(keyX[i], y_, keyW[i], h_, blackColor_);
    }
    for (int i = 0; i < numKeys_; i++) {
        if (!isBlack[i]) continue;
        int note = startNote_ + i;
        bool active = (note >= 0 && note < 128) && activeNotes && activeNotes[note];
        fb.fillRect(keyX[i], y_, keyW[i], blackKeyH, active ? activeColor_ : blackColor_);
    }
}

bool PianoKeyboard::onTouchBegan(const TouchEvent& event) {
    int note = hitTest(event.x, event.y);
    if (note < 0) return false;
    pressedNote_ = note;
    lastNote_ = note;
    wasTriggered_ = true;
    return true;
}

void PianoKeyboard::onTouchEnded(const TouchEvent&) {
    if (pressedNote_ >= 0) {
        lastReleasedNote_ = pressedNote_;
        wasReleased_ = true;
        pressedNote_ = -1;
    }
}

void PianoKeyboard::onTouchCancelled(const TouchEvent& event) {
    onTouchEnded(event);
}

} // namespace ui
