#include <gtest/gtest.h>
#include "ui/xy_pad.h"
#include "ui/touch_dispatcher.h"

TEST(XYPadTest, DefaultFracsAreCentered) {
    ui::XYPad pad(0, 0, 101);
    EXPECT_FLOAT_EQ(0.5f, pad.getX());
    EXPECT_FLOAT_EQ(0.5f, pad.getY());
}

TEST(XYPadTest, DragUpdatesFracs) {
    ui::XYPad pad(0, 0, 101);  // size-1 == 100, clean fractions
    ui::TouchDispatcher disp;
    disp.add(&pad);

    ui::TouchState ts[] = {{10, 10, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].x = 80;
    ts[0].y = 60;
    disp.dispatch(ts, 1, 16);

    EXPECT_NEAR(0.8f, pad.getX(), 0.02f);
    EXPECT_NEAR(0.6f, pad.getY(), 0.02f);
}

TEST(XYPadTest, FlipYInvertsYFraction) {
    ui::XYPad pad(0, 0, 101, ui::ACCENT_2, ui::BG_DARK, ui::BG_MID, /*flipY=*/true);
    ui::TouchDispatcher disp;
    disp.add(&pad);

    ui::TouchState ts[] = {{10, 10, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].x = 80;
    ts[0].y = 60;
    disp.dispatch(ts, 1, 16);

    EXPECT_NEAR(0.8f, pad.getX(), 0.02f);
    EXPECT_NEAR(0.4f, pad.getY(), 0.02f);  // 1 - 0.6
}

TEST(XYPadTest, FlipYSetterGetter) {
    ui::XYPad pad(0, 0, 101);
    EXPECT_FALSE(pad.flipY());
    pad.setFlipY(true);
    EXPECT_TRUE(pad.flipY());
}
