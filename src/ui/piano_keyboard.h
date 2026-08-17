#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"

namespace ui {

// On-screen piano keyboard over a contiguous MIDI note range. White keys
// are full-height; black keys are drawn on top, inset and shorter. Touch
// hit-testing checks black keys first (they visually sit above the white
// keys), matching the draw order.
class PianoKeyboard : public Widget {
public:
    static constexpr int DEFAULT_START_NOTE = 60;  // middle C
    static constexpr int DEFAULT_NUM_KEYS   = 13;   // one octave + high C
    static constexpr int MAX_KEYS           = 32;

    PianoKeyboard(int x, int y, int w, int h,
                  int startNote = DEFAULT_START_NOTE,
                  int numKeys = DEFAULT_NUM_KEYS,
                  uint16_t whiteColor = WHITE,
                  uint16_t blackColor = BLACK,
                  uint16_t activeColor = ACCENT_1) noexcept;

    // activeNotes is indexed by absolute MIDI note number (0-127); only
    // the entries covering [startNote, startNote+numKeys) are consulted.
    void draw(Framebuffer& fb, const bool activeNotes[128]) const;

    void setRange(int startNote, int numKeys) noexcept;
    int getStartNote() const noexcept { return startNote_; }
    int getNumKeys() const noexcept { return numKeys_; }

    void setColors(uint16_t whiteColor, uint16_t blackColor, uint16_t activeColor) noexcept {
        whiteColor_ = whiteColor; blackColor_ = blackColor; activeColor_ = activeColor;
    }

    // Reports the most recent note-on/note-off from touch. Both clear on read.
    bool wasNoteTriggered() noexcept { bool v = wasTriggered_; wasTriggered_ = false; return v; }
    int getLastNote() const noexcept { return lastNote_; }
    bool wasNoteReleased() noexcept { bool v = wasReleased_; wasReleased_ = false; return v; }
    int getLastReleasedNote() const noexcept { return lastReleasedNote_; }

private:
    static bool isBlackKey(int note) noexcept;
    // Fills keyX/keyW for every note in range; returns white-key width.
    int layoutKeys(int keyX[MAX_KEYS], int keyW[MAX_KEYS], bool isBlack[MAX_KEYS]) const;
    int hitTest(int px, int py) const;  // returns MIDI note, or -1

    int startNote_;
    int numKeys_;
    uint16_t whiteColor_, blackColor_, activeColor_;

    int pressedNote_ = -1;
    bool wasTriggered_ = false;
    int lastNote_ = -1;
    bool wasReleased_ = false;
    int lastReleasedNote_ = -1;

    bool onTouchBegan(const TouchEvent& event) override;
    void onTouchEnded(const TouchEvent& event) override;
    void onTouchCancelled(const TouchEvent& event) override;
};

} // namespace ui
