#include <gtest/gtest.h>
#include "ui/tabbar.h"
#include "ui/touch_dispatcher.h"

static void tap(ui::TouchDispatcher& disp, int x, int y) {
    ui::TouchState ts[] = {{x, y, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);
}

TEST(TabBarTest, TapSelectsTabByPosition) {
    ui::TabBar bar(0, 0, 90, 20);  // 3 equal 30px-wide tabs
    const char* labels[] = {"A", "B", "C"};
    bar.setTabs(labels, 3);

    ui::TouchDispatcher disp;
    disp.add(&bar);

    EXPECT_EQ(-1, bar.selectedIndex());
    tap(disp, 45, 10);  // second tab (30-60)
    EXPECT_TRUE(bar.wasSelected());
    EXPECT_FALSE(bar.wasSelected());  // clears after read
    EXPECT_EQ(1, bar.selectedIndex());
}

TEST(TabBarTest, TapFirstAndLastTab) {
    ui::TabBar bar(0, 0, 90, 20);
    const char* labels[] = {"A", "B", "C"};
    bar.setTabs(labels, 3);

    ui::TouchDispatcher disp;
    disp.add(&bar);

    tap(disp, 5, 10);
    EXPECT_EQ(0, bar.selectedIndex());

    tap(disp, 85, 10);
    EXPECT_EQ(2, bar.selectedIndex());
}

TEST(TabBarTest, SetTabsClampsToMaxTabs) {
    ui::TabBar bar(0, 0, 90, 20);
    const char* labels[] = {"1", "2", "3", "4", "5", "6", "7", "8"};
    bar.setTabs(labels, 8);

    ui::TouchDispatcher disp;
    disp.add(&bar);

    // With MAX_TABS=6, tapping near the right edge should resolve to tab
    // index 5, not 7.
    tap(disp, 89, 10);
    EXPECT_EQ(5, bar.selectedIndex());
}
