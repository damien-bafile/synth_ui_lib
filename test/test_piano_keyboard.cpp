#include <gtest/gtest.h>
#include "ui/piano_keyboard.h"
#include "ui/framebuffer.h"

// Geometry reference: PianoKeyboard(0,0,140,50, startNote=60, numKeys=13)
// covers MIDI 60 (C4) .. 72 (C5). 8 white keys -> whiteKeyW=17, blackKeyW=10.
// Note 60 (C, white): x[0,17).  Note 61 (C#, black): x[12,22), h=[0,30).

static bool regionContainsColor(const uint8_t* buf, int bufW, int x0, int y0,
                                 int w, int h, uint16_t color) {
    uint8_t lo = color & 0xFF;
    uint8_t hi = (color >> 8) & 0xFF;
    for (int y = y0; y < y0 + h; y++) {
        for (int x = x0; x < x0 + w; x++) {
            int idx = (y * bufW + x) * 2;
            if (buf[idx + 0] == lo && buf[idx + 1] == hi) return true;
        }
    }
    return false;
}

TEST(PianoKeyboardTest, GettersReflectConstructorArgs) {
    ui::PianoKeyboard kb(0, 0, 140, 50, 60, 13);
    EXPECT_EQ(60, kb.getStartNote());
    EXPECT_EQ(13, kb.getNumKeys());
}

TEST(PianoKeyboardTest, NumKeysClampedToMax) {
    ui::PianoKeyboard kb(0, 0, 300, 50, 0, 1000);
    EXPECT_EQ(ui::PianoKeyboard::MAX_KEYS, kb.getNumKeys());
}

TEST(PianoKeyboardTest, TouchWhiteKeyTriggersCorrectNote) {
    ui::PianoKeyboard kb(0, 0, 140, 50, 60, 13);
    ui::TouchEvent began = {5, 40, ui::TouchPhase::Began, 0, 0};  // inside C (60)
    EXPECT_TRUE(kb.handleTouch(began));
    EXPECT_TRUE(kb.wasNoteTriggered());
    EXPECT_EQ(60, kb.getLastNote());
}

TEST(PianoKeyboardTest, TouchBlackKeyTakesPriorityOverWhiteBeneathIt) {
    ui::PianoKeyboard kb(0, 0, 140, 50, 60, 13);
    // x=15,y=10 lies inside both white key C's x-range [0,17) and black
    // key C#'s rect [12,22)x[0,30) -- the black key (drawn on top) must win.
    ui::TouchEvent began = {15, 10, ui::TouchPhase::Began, 0, 0};
    EXPECT_TRUE(kb.handleTouch(began));
    EXPECT_EQ(61, kb.getLastNote());
}

TEST(PianoKeyboardTest, TouchReleaseReportsSameNote) {
    ui::PianoKeyboard kb(0, 0, 140, 50, 60, 13);
    ui::TouchEvent began = {5, 40, ui::TouchPhase::Began, 0, 0};
    kb.handleTouch(began);

    EXPECT_FALSE(kb.wasNoteReleased());
    ui::TouchEvent ended = {5, 40, ui::TouchPhase::Ended, 0, 0};
    kb.handleTouch(ended);
    EXPECT_TRUE(kb.wasNoteReleased());
    EXPECT_EQ(60, kb.getLastReleasedNote());
}

TEST(PianoKeyboardTest, TouchBelowKeyboardIsNotCaptured) {
    ui::PianoKeyboard kb(0, 0, 140, 50, 60, 13);
    ui::TouchEvent ev = {5, 500, ui::TouchPhase::Began, 0, 0};
    EXPECT_FALSE(kb.handleTouch(ev));
}

TEST(PianoKeyboardTest, DrawPaintsActiveColorForActiveNote) {
    static constexpr int W = 140, H = 50;
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::GRAY_DARK);

    ui::PianoKeyboard kb(0, 0, W, H, 60, 13, ui::WHITE, ui::BLACK, ui::RED);
    bool active[128] = {};
    active[60] = true;
    kb.draw(fb, active);

    // Interior of the C (60) key, away from its 1px border.
    EXPECT_TRUE(regionContainsColor(buf, W, 3, 35, 5, 5, ui::RED));
}

TEST(PianoKeyboardTest, SetRangeUpdatesGetters) {
    ui::PianoKeyboard kb(0, 0, 140, 50, 60, 13);
    kb.setRange(48, 8);
    EXPECT_EQ(48, kb.getStartNote());
    EXPECT_EQ(8, kb.getNumKeys());
}
