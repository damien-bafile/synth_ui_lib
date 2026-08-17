#include <gtest/gtest.h>
#include "ui/radio.h"
#include "ui/touch_dispatcher.h"

static void tap(ui::TouchDispatcher& disp, int x, int y) {
    ui::TouchState ts[] = {{x, y, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);
}

TEST(RadioTest, TapInsideCircleSetsWasTapped) {
    ui::Radio r(20, 20);  // center (20,20), radius DEFAULT_RADIUS=7
    ui::TouchDispatcher disp;
    disp.add(&r);

    EXPECT_FALSE(r.wasTapped());
    tap(disp, 20, 20);  // dead center
    EXPECT_TRUE(r.wasTapped());
    EXPECT_FALSE(r.wasTapped());
}

TEST(RadioTest, TapOutsideCircleButInsideBoundingBoxIsIgnored) {
    ui::Radio r(20, 20);
    ui::TouchDispatcher disp;
    disp.add(&r);

    // Corner of the bounding square (x_,y_)..(x_+2r,y_+2r) but outside the
    // inscribed circle used by Radio::isInside.
    tap(disp, 20 - 7, 20 - 7);
    EXPECT_FALSE(r.wasTapped());
}

TEST(RadioTest, SetPositionRecentersBounds) {
    ui::Radio r(20, 20);
    EXPECT_EQ(20 - ui::Radio::DEFAULT_RADIUS, r.getX());
    EXPECT_EQ(20 - ui::Radio::DEFAULT_RADIUS, r.getY());

    r.setPosition(50, 60);
    EXPECT_EQ(50 - ui::Radio::DEFAULT_RADIUS, r.getX());
    EXPECT_EQ(60 - ui::Radio::DEFAULT_RADIUS, r.getY());
}
