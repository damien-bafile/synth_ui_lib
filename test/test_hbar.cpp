#include <gtest/gtest.h>
#include "ui/hbar.h"
#include "ui/framebuffer.h"
#include "ui/touch_dispatcher.h"

TEST(HBarTest, DefaultFractionIsZero) {
    ui::HorizontalBar bar(0, 0, 100, 10);
    EXPECT_FLOAT_EQ(0.0f, bar.getFraction());
}

TEST(HBarTest, DragUpdatesFraction) {
    ui::HorizontalBar bar(0, 0, 100, 10);
    ui::TouchDispatcher disp;
    disp.add(&bar);

    ui::TouchState ts[] = {{10, 5, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].x = 80;  // move far enough to exceed the drag threshold
    disp.dispatch(ts, 1, 16);

    EXPECT_NEAR(0.8f, bar.getFraction(), 0.02f);
}

TEST(HBarTest, TapAloneDoesNotChangeFraction) {
    // onTouchBegan only captures the touch (returns contains()); the
    // fraction is set from onDragMoved, not onTouchBegan.
    ui::HorizontalBar bar(0, 0, 100, 10);
    ui::TouchEvent ev = {50, 5, ui::TouchPhase::Began, 0, 0};
    EXPECT_TRUE(bar.handleTouch(ev));
    EXPECT_FLOAT_EQ(0.0f, bar.getFraction());
}

TEST(HBarTest, DrawCenteredCentsPaintsFill) {
    static constexpr int W = 64, H = 16;
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    static constexpr uint16_t FILL = 0xFFFF;
    static constexpr uint16_t BG = 0x0000;
    fb.fillScreen(BG);

    ui::HorizontalBar::drawCenteredCents(fb, 0, 0, W, H, 50.0f, 100.0f, FILL, BG);

    bool found = false;
    for (int i = 0; i < W * H; i++) {
        if (buf[i * 2 + 0] == 0xFF && buf[i * 2 + 1] == 0xFF) { found = true; break; }
    }
    EXPECT_TRUE(found);
}
