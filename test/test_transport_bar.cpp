#include <gtest/gtest.h>
#include "ui/transport_bar.h"
#include "ui/touch_dispatcher.h"

static void tap(ui::TouchDispatcher& disp, int x, int y) {
    ui::TouchState ts[] = {{x, y, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);
}

TEST(TransportBarTest, TapPlayButtonReportsBtnPlay) {
    ui::TransportBar bar(0, 0, 300, 20);
    ui::TouchDispatcher disp;
    bar.addTo(disp);

    EXPECT_EQ(-1, bar.tappedButton());
    tap(disp, 10, 10);  // inside btn 0 (PLAY): x in [2,28)
    EXPECT_EQ(ui::TransportBar::BTN_PLAY, bar.tappedButton());
    EXPECT_EQ(-1, bar.tappedButton());  // clears after read
}

TEST(TransportBarTest, TapStopButtonReportsBtnStop) {
    ui::TransportBar bar(0, 0, 300, 20);
    ui::TouchDispatcher disp;
    bar.addTo(disp);

    tap(disp, 35, 10);  // inside btn 1 (STOP): x in [30,56)
    EXPECT_EQ(ui::TransportBar::BTN_STOP, bar.tappedButton());
}

TEST(TransportBarTest, TapEmptyAreaReportsNoButton) {
    ui::TransportBar bar(0, 0, 300, 20);
    ui::TouchDispatcher disp;
    bar.addTo(disp);

    tap(disp, 150, 10);  // between the transport buttons and the swatches
    EXPECT_EQ(-1, bar.tappedButton());
}

TEST(TransportBarTest, MenuButtonOnlyTappableWhenEnabledAndAdded) {
    ui::TransportBar bar(0, 0, 300, 20);
    ui::TouchDispatcher disp;
    bar.setMenuButton(true);
    bar.addTo(disp);  // menuButton_ is checked at addTo() time

    int menuX = 300 - 26 - 2 + 5;  // inside btn 3 (MENU)
    tap(disp, menuX, 10);
    EXPECT_EQ(ui::TransportBar::BTN_MENU, bar.tappedButton());
}

TEST(TransportBarTest, TextPositionHelpers) {
    ui::TransportBar bar(10, 0, 300, 20);
    EXPECT_EQ(10 + 142, bar.leftTextX());
    EXPECT_EQ(0 + (20 - 7) / 2, bar.textY());
}
