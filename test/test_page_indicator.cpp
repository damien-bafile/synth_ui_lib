#include <gtest/gtest.h>
#include "ui/page_indicator.h"
#include "ui/framebuffer.h"
#include "ui/touch_dispatcher.h"

static constexpr int W = 128;
static constexpr int H = 32;

static void tap(ui::TouchDispatcher& disp, int x, int y) {
    ui::TouchState ts[] = {{x, y, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);
}

TEST(PageIndicatorTest, TapPrevBracketSetsNegativeDelta) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);

    ui::PageIndicator pi(0, 0, 100);
    pi.draw(fb, 2, 5);  // populates the "<"/">" bracket hit-rects

    ui::TouchDispatcher disp;
    disp.add(&pi);

    EXPECT_EQ(0, pi.pageDelta());
    // With w=100 and mid=" PG 3/5 " (8 chars * 6px + 6px per bracket), the
    // "<" bracket rect is x in [20,26). Tap inside it.
    tap(disp, 22, 3);
    EXPECT_EQ(-1, pi.pageDelta());
    EXPECT_EQ(0, pi.pageDelta());  // clears after read
}

TEST(PageIndicatorTest, TapNextBracketSetsPositiveDelta) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);

    ui::PageIndicator pi(0, 0, 100);
    pi.draw(fb, 2, 5);

    ui::TouchDispatcher disp;
    disp.add(&pi);

    // The ">" bracket rect is x in [74,80).
    tap(disp, 76, 3);
    EXPECT_EQ(1, pi.pageDelta());
}

TEST(PageIndicatorTest, TapMiddleTextDoesNotChangeDelta) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);

    ui::PageIndicator pi(0, 0, 100);
    pi.draw(fb, 2, 5);

    ui::TouchDispatcher disp;
    disp.add(&pi);

    tap(disp, 50, 3);  // center, over the "PG n/m" readout
    EXPECT_EQ(0, pi.pageDelta());
}
