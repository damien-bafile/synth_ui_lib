#include <gtest/gtest.h>
#include "ui/vu_slider.h"
#include "ui/touch_dispatcher.h"

TEST(VUSliderTest, TouchBeganJumpsToPosition) {
    ui::VUSlider s(0, 0, 10, 100, 0, 0, true);  // vertical
    EXPECT_FALSE(s.wasChanged());

    // Vertical: frac = 1 - (py - y) / height, so touching near the top
    // (py=10) should read close to 1.0.
    ui::TouchEvent ev = {5, 10, ui::TouchPhase::Began, 0, 0};
    EXPECT_TRUE(s.handleTouch(ev));

    EXPECT_NEAR(0.9f, s.getValue(), 0.02f);
    EXPECT_TRUE(s.wasChanged());
    EXPECT_FALSE(s.wasChanged());
}

TEST(VUSliderTest, DragUpdatesValue) {
    ui::VUSlider s(0, 0, 100, 10, 0, 0, false);  // horizontal
    ui::TouchDispatcher disp;
    disp.add(&s);

    ui::TouchState ts[] = {{5, 5, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].x = 95;
    disp.dispatch(ts, 1, 16);

    EXPECT_NEAR(0.95f, s.getValue(), 0.02f);
}

TEST(VUSliderTest, SetValueClampsToUnitRange) {
    ui::VUSlider s(0, 0, 100, 10);
    s.setValue(5.0f);
    EXPECT_FLOAT_EQ(1.0f, s.getValue());
    s.setValue(-5.0f);
    EXPECT_FLOAT_EQ(0.0f, s.getValue());
}
