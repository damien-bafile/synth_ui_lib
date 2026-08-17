#include <gtest/gtest.h>
#include "ui/list_view.h"
#include "ui/framebuffer.h"
#include "ui/touch_dispatcher.h"

// Geometry reference: ListView(0,0,100,60, 5 items). ROW_H=20 -> 3 rows
// visible at a time; maxScrollOffset = 5*20-60 = 40.
static const char* kItems[] = {"Init", "Bass", "Lead", "Pad", "Pluck"};

static void tap(ui::TouchDispatcher& disp, int x, int y) {
    ui::TouchState ts[] = {{x, y, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);
}

TEST(ListViewTest, GettersReflectConstructorArgs) {
    ui::ListView lv(0, 0, 100, 60, kItems, 5);
    EXPECT_EQ(5, lv.getItemCount());
    EXPECT_EQ(-1, lv.getSelectedIndex());
    EXPECT_FLOAT_EQ(0.0f, lv.getScrollOffset());
}

TEST(ListViewTest, TapSelectsRowByOriginalIndex) {
    ui::ListView lv(0, 0, 100, 60, kItems, 5);
    ui::TouchDispatcher disp;
    disp.add(&lv);

    EXPECT_FALSE(lv.wasSelected());
    tap(disp, 10, 25);  // row 1 ("Bass"): y in [20,40)
    EXPECT_TRUE(lv.wasSelected());
    EXPECT_EQ(1, lv.getSelectedIndex());
    EXPECT_FALSE(lv.wasSelected());  // clears after read
}

TEST(ListViewTest, DragScrollsClampedToMaxOffset) {
    ui::ListView lv(0, 0, 100, 60, kItems, 5);
    ui::TouchDispatcher disp;
    disp.add(&lv);

    ui::TouchState ts[] = {{10, 50, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].y = 10;  // drag up 40px -> scroll offset increases by 40
    disp.dispatch(ts, 1, 16);

    EXPECT_FLOAT_EQ(40.0f, lv.getScrollOffset());  // clamped to max

    ts[0].y = -100;  // drag further -- must not exceed the clamp
    disp.dispatch(ts, 1, 32);
    EXPECT_FLOAT_EQ(40.0f, lv.getScrollOffset());
}

TEST(ListViewTest, SetScrollOffsetClamps) {
    ui::ListView lv(0, 0, 100, 60, kItems, 5);
    lv.setScrollOffset(1000.0f);
    EXPECT_FLOAT_EQ(40.0f, lv.getScrollOffset());
    lv.setScrollOffset(-10.0f);
    EXPECT_FLOAT_EQ(0.0f, lv.getScrollOffset());
}

TEST(ListViewTest, IdentityOrderInitially) {
    ui::ListView lv(0, 0, 100, 60, kItems, 5);
    const int* order = lv.getOrder();
    for (int i = 0; i < 5; i++) EXPECT_EQ(i, order[i]);
}

TEST(ListViewTest, LongPressThenDragReordersRows) {
    ui::ListView lv(0, 0, 100, 60, kItems, 5);
    lv.setReorderable(true);
    ui::TouchDispatcher disp;
    disp.add(&lv);

    ui::TouchState ts[] = {{10, 25, true}};  // row 1
    disp.dispatch(ts, 1, 0);
    // Hold in place past the long-press threshold with no movement, so
    // onLongPress fires before any drag gesture would start.
    disp.dispatch(ts, 1, ui::TouchDispatcher::kLongPressMs + 50);

    // Now drag down by 25px (past the half-row threshold of 10px), which
    // should swap row 1 with row 2.
    ts[0].y = 50;
    disp.dispatch(ts, 1, ui::TouchDispatcher::kLongPressMs + 66);

    ts[0].pressed = false;
    disp.dispatch(ts, 1, ui::TouchDispatcher::kLongPressMs + 166);

    EXPECT_TRUE(lv.wasReordered());
    const int* order = lv.getOrder();
    EXPECT_EQ(2, order[1]);
    EXPECT_EQ(1, order[2]);
    // Selection must not have fired -- this was a drag, not a tap.
    EXPECT_FALSE(lv.wasSelected());
}

TEST(ListViewTest, ReorderDisabledByDefaultLeavesOrderIdentity) {
    ui::ListView lv(0, 0, 100, 60, kItems, 5);  // reorderable_ defaults false
    ui::TouchDispatcher disp;
    disp.add(&lv);

    ui::TouchState ts[] = {{10, 25, true}};
    disp.dispatch(ts, 1, 0);
    disp.dispatch(ts, 1, ui::TouchDispatcher::kLongPressMs + 50);
    ts[0].y = 50;
    disp.dispatch(ts, 1, ui::TouchDispatcher::kLongPressMs + 66);

    const int* order = lv.getOrder();
    EXPECT_EQ(1, order[1]);
    EXPECT_EQ(2, order[2]);
}

TEST(ListViewTest, SetItemsResetsSelectionScrollAndOrder) {
    ui::ListView lv(0, 0, 100, 60, kItems, 5);
    lv.setScrollOffset(30.0f);

    static const char* moreItems[] = {"A", "B"};
    lv.setItems(moreItems, 2);

    EXPECT_EQ(2, lv.getItemCount());
    EXPECT_EQ(-1, lv.getSelectedIndex());
    EXPECT_FLOAT_EQ(0.0f, lv.getScrollOffset());
}

TEST(ListViewTest, DrawDoesNotCrashAndPaintsSelectedRowAccent) {
    static constexpr int W = 100, H = 60;
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::ListView lv(0, 0, W, H, kItems, 5, ui::WHITE, ui::BLACK, ui::RED);
    ui::TouchDispatcher disp;
    disp.add(&lv);
    tap(disp, 10, 25);  // selects row 1

    lv.draw(fb);

    bool foundRed = false;
    for (int i = 0; i < W * H; i++) {
        if (buf[i * 2 + 0] == 0x00 && buf[i * 2 + 1] == 0xF8) { foundRed = true; break; }
    }
    EXPECT_TRUE(foundRed);
}
