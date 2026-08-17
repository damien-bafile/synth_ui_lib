#include <gtest/gtest.h>
#include "ui/keyboard.h"

// Geometry reference (LOWER layout, Keyboard(0,0,300,200)):
// HEADER_H=28, keyAreaY=28, keyAreaH=172, keyH=41, GAP=2.
// Row 0 ("qwertyuiop", 10 equal-width keys): unitW=28, key "q" at x[0,28) y[28,69).
// Row 2 ("zxcvbnm" + Del, 8 equal-width keys): unitW=35, each key advances
// by unitW+GAP=37; "Del" (index 7) at x[259,294) y[114,155).
// Row 3 ("Sft","123",space,"OK", widths 1/1/4/1): unitW=42, "Sft" (index 0)
//   at x[0,42) y[157,198).

static void pressKeyAt(ui::Keyboard& kb, int x, int y) {
    ui::TouchEvent began = {x, y, ui::TouchPhase::Began, 0, 0};
    kb.handleTouch(began);
    ui::TouchEvent ended = {x, y, ui::TouchPhase::Ended, 0, 0};
    kb.handleTouch(ended);
}

TEST(KeyboardTest, InactiveKeyboardIgnoresTouch) {
    ui::Keyboard kb(0, 0, 300, 200, ui::TEXT, ui::ACCENT_2);
    EXPECT_FALSE(kb.isActive());
    ui::TouchEvent ev = {10, 40, ui::TouchPhase::Began, 0, 0};
    EXPECT_FALSE(kb.handleTouch(ev));
}

TEST(KeyboardTest, TappingLetterKeyInsertsChar) {
    ui::Keyboard kb(0, 0, 300, 200, ui::TEXT, ui::ACCENT_2);
    kb.activate();
    ASSERT_TRUE(kb.isActive());

    pressKeyAt(kb, 10, 40);  // "q"

    EXPECT_STREQ("q", kb.getText());
    EXPECT_EQ(1, kb.cursorPos());
}

TEST(KeyboardTest, BackspaceRemovesLastChar) {
    ui::Keyboard kb(0, 0, 300, 200, ui::TEXT, ui::ACCENT_2);
    kb.activate();
    kb.setText("hi");

    pressKeyAt(kb, 270, 120);  // "Del"

    EXPECT_STREQ("h", kb.getText());
}

TEST(KeyboardTest, ShiftTogglesLayer) {
    ui::Keyboard kb(0, 0, 300, 200, ui::TEXT, ui::ACCENT_2);
    kb.activate();
    EXPECT_EQ(ui::Keyboard::Layer::LOWER, kb.currentLayer());

    pressKeyAt(kb, 10, 170);  // "Sft"
    EXPECT_EQ(ui::Keyboard::Layer::UPPER, kb.currentLayer());

    pressKeyAt(kb, 10, 170);  // "Sft" again
    EXPECT_EQ(ui::Keyboard::Layer::LOWER, kb.currentLayer());
}

TEST(KeyboardTest, SetTextGetTextClear) {
    ui::Keyboard kb(0, 0, 300, 200, ui::TEXT, ui::ACCENT_2);
    kb.setText("hello");
    EXPECT_STREQ("hello", kb.getText());
    EXPECT_EQ(5, kb.cursorPos());

    kb.clear();
    EXPECT_STREQ("", kb.getText());
    EXPECT_EQ(0, kb.cursorPos());
}

TEST(KeyboardTest, ActivateDismissAndConfirmed) {
    ui::Keyboard kb(0, 0, 300, 200, ui::TEXT, ui::ACCENT_2);
    EXPECT_FALSE(kb.isActive());
    kb.activate();
    EXPECT_TRUE(kb.isActive());
    EXPECT_FALSE(kb.confirmed());

    kb.dismiss();
    EXPECT_FALSE(kb.isActive());
    EXPECT_TRUE(kb.confirmed());
}

TEST(KeyboardTest, DoubleTapSelectsWord) {
    ui::Keyboard kb(0, 0, 300, 200, ui::TEXT, ui::ACCENT_2);
    kb.activate();
    kb.setText("hello world");

    EXPECT_FALSE(kb.wasDoubleTapped());
    // Header text area; px=16 maps to char index (16-4)/6 = 2, inside "hello".
    ui::TouchEvent ev = {16, 10, ui::TouchPhase::Began, 0, 0};
    kb.onDoubleTap(ev);

    EXPECT_TRUE(kb.wasDoubleTapped());
    EXPECT_TRUE(kb.hasSelection());
    EXPECT_EQ(0, kb.selBegin());
    EXPECT_EQ(5, kb.selEnd());
}
