#include <gtest/gtest.h>
#include "ui/radial_dial.h"
#include "ui/touch_dispatcher.h"

// RadialDial's own onTouchBegan/isInside/draw all key off getX()/getY()
// as the pixel center (not the bounding-box top-left other widgets use),
// so tests drive touch positions from getX()/getY() rather than assuming
// a specific relationship to the constructor's (x, y) argument.

static void tap(ui::TouchDispatcher& disp, int x, int y) {
    ui::TouchState ts[] = {{x, y, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);
}

TEST(RadialDialTest, GettersReflectConstructorArgs) {
    ui::RadialDial dial(50, 50, 20, 0.0f, 100.0f, 25.0f, 1);
    EXPECT_EQ(20, dial.getRadius());
    EXPECT_FLOAT_EQ(0.0f, dial.getMin());
    EXPECT_FLOAT_EQ(100.0f, dial.getMax());
    EXPECT_FLOAT_EQ(25.0f, dial.getValue());
    EXPECT_EQ(1, dial.getSteps());
}

TEST(RadialDialTest, TouchOutsideCircleIsNotCaptured) {
    ui::RadialDial dial(50, 50, 20, 0.0f, 100.0f, 25.0f, 1);
    ui::TouchEvent ev = {dial.getX() + 1000, dial.getY(), ui::TouchPhase::Began, 0, 0};
    EXPECT_FALSE(dial.handleTouch(ev));
}

TEST(RadialDialTest, DragUpVerticallyIncreasesValue) {
    ui::RadialDial dial(50, 50, 20, 0.0f, 100.0f, 50.0f, 1);
    ui::TouchDispatcher disp;
    disp.add(&dial);

    int cx = dial.getX();
    int cy = dial.getY();

    ui::TouchState ts[] = {{cx, cy, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].y = cy - 20;  // drag up: sensitivity = range/(2*radius) = 100/40 = 2.5
    disp.dispatch(ts, 1, 16);

    // totalDy = -20, delta = -(-20)*2.5 = 50 -> value 50+50 = 100 (clamped)
    EXPECT_FLOAT_EQ(100.0f, dial.getValue());
}

TEST(RadialDialTest, DragDownDecreasesValue) {
    ui::RadialDial dial(50, 50, 20, 0.0f, 100.0f, 50.0f, 1);
    ui::TouchDispatcher disp;
    disp.add(&dial);

    int cx = dial.getX();
    int cy = dial.getY();

    ui::TouchState ts[] = {{cx, cy, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].y = cy + 20;
    disp.dispatch(ts, 1, 16);

    EXPECT_FLOAT_EQ(0.0f, dial.getValue());
}

TEST(RadialDialTest, TapSetsWasToggled) {
    ui::RadialDial dial(50, 50, 20, 0.0f, 100.0f, 50.0f, 1);
    ui::TouchDispatcher disp;
    disp.add(&dial);

    EXPECT_FALSE(dial.wasToggled());
    tap(disp, dial.getX(), dial.getY());
    EXPECT_TRUE(dial.wasToggled());
    EXPECT_FALSE(dial.wasToggled());
}

TEST(RadialDialTest, SetValueClampsToRange) {
    ui::RadialDial dial(50, 50, 20, 0.0f, 100.0f, 50.0f, 1);
    dial.setValue(500.0f);
    EXPECT_FLOAT_EQ(100.0f, dial.getValue());
    dial.setValue(-500.0f);
    EXPECT_FLOAT_EQ(0.0f, dial.getValue());
}

TEST(RadialDialTest, SetValueSnapsToNearestStep) {
    ui::RadialDial dial(50, 50, 20, 0.0f, 10.0f, 0.0f, 3);  // steps: 0, 5, 10
    dial.setValue(6.0f);
    EXPECT_FLOAT_EQ(5.0f, dial.getValue());
    dial.setValue(9.0f);
    EXPECT_FLOAT_EQ(10.0f, dial.getValue());
}

TEST(RadialDialTest, SetRangeAndStepsUpdateGetters) {
    ui::RadialDial dial(50, 50, 20, 0.0f, 100.0f, 50.0f, 1);
    dial.setRange(-10.0f, 10.0f);
    EXPECT_FLOAT_EQ(-10.0f, dial.getMin());
    EXPECT_FLOAT_EQ(10.0f, dial.getMax());

    dial.setSteps(5);
    EXPECT_EQ(5, dial.getSteps());
    dial.setSteps(0);  // clamped to at least 1
    EXPECT_EQ(1, dial.getSteps());
}
