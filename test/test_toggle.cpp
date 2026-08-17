#include <gtest/gtest.h>
#include "ui/toggle.h"
#include "ui/touch_dispatcher.h"

static void tap(ui::TouchDispatcher& disp, int x, int y) {
    ui::TouchState ts[] = {{x, y, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);
}

TEST(ToggleTest, TapSetsWasTapped) {
    ui::Toggle t(0, 0);
    ui::TouchDispatcher disp;
    disp.add(&t);

    EXPECT_FALSE(t.wasTapped());
    tap(disp, 5, 5);
    EXPECT_TRUE(t.wasTapped());
    EXPECT_FALSE(t.wasTapped());
}

TEST(ToggleTest, DefaultSizeAndPosition) {
    ui::Toggle t(3, 4);
    EXPECT_EQ(3, t.getX());
    EXPECT_EQ(4, t.getY());
    EXPECT_EQ(ui::Toggle::DEFAULT_W, t.getWidth());
    EXPECT_EQ(ui::Toggle::DEFAULT_H, t.getHeight());

    t.setPosition(10, 20);
    EXPECT_EQ(10, t.getX());
    EXPECT_EQ(20, t.getY());
}
