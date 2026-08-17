#include <gtest/gtest.h>
#include "ui/checkbox.h"
#include "ui/touch_dispatcher.h"

static void tap(ui::TouchDispatcher& disp, int x, int y) {
    ui::TouchState ts[] = {{x, y, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);
}

TEST(CheckboxTest, TapSetsWasTapped) {
    ui::Checkbox c(0, 0);
    ui::TouchDispatcher disp;
    disp.add(&c);

    EXPECT_FALSE(c.wasTapped());
    tap(disp, 5, 5);
    EXPECT_TRUE(c.wasTapped());
    EXPECT_FALSE(c.wasTapped());
}

TEST(CheckboxTest, DefaultSizeAndPosition) {
    ui::Checkbox c(3, 4);
    EXPECT_EQ(3, c.getX());
    EXPECT_EQ(4, c.getY());
    EXPECT_EQ(ui::Checkbox::DEFAULT_SIZE, c.getSize());

    c.setPosition(10, 20);
    EXPECT_EQ(10, c.getX());
    EXPECT_EQ(20, c.getY());
}
