#include <gtest/gtest.h>
#include "ui/stepgrid.h"
#include "ui/touch_dispatcher.h"

static void tap(ui::TouchDispatcher& disp, int x, int y) {
    ui::TouchState ts[] = {{x, y, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);
}

TEST(StepGridTest, ConstructorClampsCounts) {
    ui::StepGrid g(0, 0, 160, 80, 16, 4);
    EXPECT_EQ(16, g.getStepCount());
    EXPECT_EQ(4, g.getTrackCount());
}

TEST(StepGridTest, TouchBeganMapsToCellZero) {
    ui::StepGrid g(0, 0, 160, 80, 16, 4);
    EXPECT_FALSE(g.hasTouch());

    ui::TouchEvent ev = {2, 2, ui::TouchPhase::Began, 0, 0};
    EXPECT_TRUE(g.handleTouch(ev));

    EXPECT_TRUE(g.hasTouch());
    EXPECT_EQ(0, g.getTouchedTrack());
    EXPECT_EQ(0, g.getTouchedStep());
    EXPECT_EQ(0, g.getTouchStartTrack());
    EXPECT_EQ(0, g.getTouchStartStep());
}

TEST(StepGridTest, TouchEndedClearsTouch) {
    ui::StepGrid g(0, 0, 160, 80, 16, 4);
    ui::TouchEvent began = {2, 2, ui::TouchPhase::Began, 0, 0};
    g.handleTouch(began);
    ASSERT_TRUE(g.hasTouch());

    ui::TouchEvent ended = {2, 2, ui::TouchPhase::Ended, 0, 0};
    g.handleTouch(ended);
    EXPECT_FALSE(g.hasTouch());
    EXPECT_EQ(-1, g.getTouchedTrack());
    EXPECT_EQ(-1, g.getTouchedStep());
}

TEST(StepGridTest, TapSetsWasTapped) {
    ui::StepGrid g(0, 0, 160, 80, 16, 4);
    ui::TouchDispatcher disp;
    disp.add(&g);

    EXPECT_FALSE(g.wasTapped());
    tap(disp, 2, 2);
    EXPECT_TRUE(g.wasTapped());
    EXPECT_FALSE(g.wasTapped());  // clears after read
}

TEST(StepGridTest, DragAccumTracksVerticalMovement) {
    ui::StepGrid g(0, 0, 160, 80, 16, 4);
    ui::TouchEvent began = {2, 2, ui::TouchPhase::Began, 0, 0};
    g.handleTouch(began);

    ui::TouchEvent moved = {2, 22, ui::TouchPhase::Moved, 0, 16};
    g.handleTouch(moved);

    EXPECT_EQ(20, g.dragAccumY());

    int ticks = g.takeDragTicks(10);
    EXPECT_EQ(2, ticks);
    EXPECT_EQ(0, g.dragAccumY());
}

TEST(StepGridTest, SetStepAndTrackCountClampToRange) {
    ui::StepGrid g(0, 0, 160, 80, 16, 4);
    g.setStepCount(1000);
    EXPECT_EQ(ui::StepGrid::MAX_STEPS, g.getStepCount());
    g.setStepCount(0);
    EXPECT_EQ(1, g.getStepCount());

    g.setTrackCount(1000);
    EXPECT_EQ(ui::StepGrid::MAX_TRACKS, g.getTrackCount());
    g.setTrackCount(0);
    EXPECT_EQ(1, g.getTrackCount());
}

TEST(StepGridTest, SetActiveTrackRejectsOutOfRange) {
    ui::StepGrid g(0, 0, 160, 80, 16, 4);
    g.setActiveTrack(2);
    EXPECT_EQ(2, g.getActiveTrack());

    g.setActiveTrack(99);
    EXPECT_EQ(-1, g.getActiveTrack());

    g.setActiveTrack(-1);
    EXPECT_EQ(-1, g.getActiveTrack());
}
