#include <gtest/gtest.h>
#include "ui/dropdown.h"
#include "ui/touch_dispatcher.h"

static void tap(ui::TouchDispatcher& disp, int x, int y) {
    ui::TouchState ts[] = {{x, y, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);
}

TEST(DropdownTest, TapHeaderExpands) {
    ui::Dropdown dd(0, 0);  // default 120x20
    dd.addItem("A");
    dd.addItem("B");

    ui::TouchDispatcher disp;
    disp.add(&dd);

    EXPECT_FALSE(dd.isExpanded());
    tap(disp, 10, 10);
    EXPECT_TRUE(dd.isExpanded());
}

TEST(DropdownTest, TapItemInExpandedListSelectsAndCollapses) {
    ui::Dropdown dd(0, 0);  // CLASSIC style opens directly below the header
    dd.addItem("A");
    dd.addItem("B");
    dd.addItem("C");
    dd.setExpanded(true);

    ui::TouchDispatcher disp;
    disp.add(&dd);

    // CLASSIC item height == h_ (20); list starts at y_+h_ == 20.
    // Item 1 ("B") occupies y in [40, 60).
    EXPECT_FALSE(dd.wasSelected());
    tap(disp, 10, 45);
    EXPECT_TRUE(dd.wasSelected());
    EXPECT_EQ(1, dd.getSelectedIndex());
    EXPECT_FALSE(dd.isExpanded());
}

TEST(DropdownTest, AddItemRespectsMaxItems) {
    ui::Dropdown dd(0, 0);
    for (int i = 0; i < ui::Dropdown::MAX_ITEMS + 5; i++) {
        dd.addItem("X");
    }
    EXPECT_EQ(ui::Dropdown::MAX_ITEMS, dd.getItemCount());
}

TEST(DropdownTest, ClearItemsResetsCount) {
    ui::Dropdown dd(0, 0);
    dd.addItem("A");
    dd.addItem("B");
    ASSERT_EQ(2, dd.getItemCount());
    dd.clearItems();
    EXPECT_EQ(0, dd.getItemCount());
}

TEST(DropdownTest, SetExpandedRaisesZOrder) {
    ui::Dropdown dd(0, 0);
    EXPECT_EQ(0, dd.zOrder());
    dd.setExpanded(true);
    EXPECT_EQ(1, dd.zOrder());
    EXPECT_TRUE(dd.isExpanded());
    dd.setExpanded(false);
    EXPECT_EQ(0, dd.zOrder());
}
