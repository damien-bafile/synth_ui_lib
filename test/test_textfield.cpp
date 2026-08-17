#include <gtest/gtest.h>
#include "ui/textfield.h"
#include "ui/touch_dispatcher.h"

static void tap(ui::TouchDispatcher& disp, int x, int y) {
    ui::TouchState ts[] = {{x, y, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);
}

TEST(TextFieldTest, TapSetsWasTapped) {
    ui::TextField f(0, 0, 100, 20);
    ui::TouchDispatcher disp;
    disp.add(&f);

    EXPECT_FALSE(f.wasTapped());
    tap(disp, 50, 10);
    EXPECT_TRUE(f.wasTapped());
    EXPECT_FALSE(f.wasTapped());
}

TEST(TextFieldTest, PositionAndSizeSetters) {
    ui::TextField f(5, 5, 100, 20);
    EXPECT_EQ(5, f.getX());
    EXPECT_EQ(100, f.getWidth());

    f.setPosition(10, 15);
    EXPECT_EQ(10, f.getX());
    EXPECT_EQ(15, f.getY());

    f.setSize(50, 30);
    EXPECT_EQ(50, f.getWidth());
    EXPECT_EQ(30, f.getHeight());
}
