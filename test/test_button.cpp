#include <gtest/gtest.h>
#include "ui/button.h"
#include "ui/touch_dispatcher.h"

static void tap(ui::TouchDispatcher& disp, int x, int y) {
    ui::TouchState ts[] = {{x, y, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);
}

TEST(ButtonTest, TapSetsWasTapped) {
    ui::Button b(0, 0, 40, 20, "OK");
    ui::TouchDispatcher disp;
    disp.add(&b);

    EXPECT_FALSE(b.wasTapped());
    tap(disp, 20, 10);
    EXPECT_TRUE(b.wasTapped());
    // Clears after being read once.
    EXPECT_FALSE(b.wasTapped());
}

TEST(ButtonTest, TapOutsideBoundsDoesNothing) {
    ui::Button b(0, 0, 40, 20, "OK");
    ui::TouchDispatcher disp;
    disp.add(&b);

    tap(disp, 100, 100);
    EXPECT_FALSE(b.wasTapped());
}

TEST(ButtonTest, LabelAndPositionSetters) {
    ui::Button b(5, 5, 40, 20, "OK");
    EXPECT_STREQ("OK", b.getLabel());
    EXPECT_EQ(5, b.getX());
    EXPECT_EQ(5, b.getY());

    b.setLabel("GO");
    EXPECT_STREQ("GO", b.getLabel());

    b.setPosition(10, 15);
    EXPECT_EQ(10, b.getX());
    EXPECT_EQ(15, b.getY());
}
