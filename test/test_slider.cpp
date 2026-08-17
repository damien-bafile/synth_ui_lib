#include <gtest/gtest.h>
#include "ui/slider.h"
#include "ui/touch_dispatcher.h"

TEST(SliderTest, TouchBeganJumpsToPosition) {
    ui::Slider s(0, 0, 100, 10, 0.0f, 1.0f, "Vol", 0, 0, false);
    EXPECT_FALSE(s.wasChanged());

    ui::TouchEvent ev = {50, 5, ui::TouchPhase::Began, 0, 0};
    EXPECT_TRUE(s.handleTouch(ev));

    EXPECT_NEAR(0.5f, s.getValue(), 0.01f);
    EXPECT_TRUE(s.wasChanged());
    EXPECT_FALSE(s.wasChanged());  // clears after read
}

TEST(SliderTest, DragUpdatesValue) {
    ui::Slider s(0, 0, 100, 10, 0.0f, 1.0f, "Vol", 0, 0, false);
    ui::TouchDispatcher disp;
    disp.add(&s);

    ui::TouchState ts[] = {{10, 5, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].x = 90;  // move far enough to exceed the drag threshold
    disp.dispatch(ts, 1, 16);

    EXPECT_NEAR(0.9f, s.getValue(), 0.01f);
}

TEST(SliderTest, SetValueClampsToRange) {
    ui::Slider s(0, 0, 100, 10, 0.0f, 1.0f, "Vol");
    s.setValue(5.0f);
    EXPECT_FLOAT_EQ(1.0f, s.getValue());
    s.setValue(-5.0f);
    EXPECT_FLOAT_EQ(0.0f, s.getValue());
}
