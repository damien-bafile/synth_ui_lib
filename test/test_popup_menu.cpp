#include <gtest/gtest.h>
#include "ui/popup_menu.h"

TEST(PopupMenuTest, ShowMakesVisibleAndResetsSelection) {
    ui::PopupMenu::Item items[] = {{"One"}, {"Two"}, {"Three"}};
    ui::PopupMenu menu(0, 0, items, 3);

    EXPECT_FALSE(menu.isVisible());
    menu.show();
    EXPECT_TRUE(menu.isVisible());
    EXPECT_EQ(-1, menu.getSelectedIndex());
}

TEST(PopupMenuTest, TapItemSelectsOnTouchEnded) {
    ui::PopupMenu::Item items[] = {{"One"}, {"Two"}, {"Three"}};
    ui::PopupMenu menu(0, 0, items, 3);
    menu.show();

    // itemH_ defaults to 24, item origin offset by pad=2; item 1 ("Two")
    // occupies y in [26, 50).
    ui::TouchEvent began = {5, 30, ui::TouchPhase::Began, 0, 0};
    EXPECT_TRUE(menu.handleTouch(began));

    ui::TouchEvent ended = {5, 30, ui::TouchPhase::Ended, 0, 0};
    menu.handleTouch(ended);

    EXPECT_TRUE(menu.wasSelected());
    EXPECT_EQ(1, menu.getSelectedIndex());
    EXPECT_STREQ("Two", menu.getSelectedLabel());
    EXPECT_FALSE(menu.isVisible());  // selecting dismisses
}

TEST(PopupMenuTest, TouchOutsideDismisses) {
    ui::PopupMenu::Item items[] = {{"One"}, {"Two"}};
    ui::PopupMenu menu(0, 0, items, 2);
    menu.show();
    ASSERT_TRUE(menu.isVisible());

    ui::TouchEvent ev = {500, 500, ui::TouchPhase::Began, 0, 0};
    menu.handleTouch(ev);

    EXPECT_FALSE(menu.isVisible());
    EXPECT_TRUE(menu.wasDismissed());
}

TEST(PopupMenuTest, ShowAtMovesPosition) {
    ui::PopupMenu::Item items[] = {{"One"}};
    ui::PopupMenu menu(0, 0, items, 1);
    menu.showAt(50, 60);
    EXPECT_TRUE(menu.isVisible());

    // Touching where the menu now is (not where it was constructed) should
    // be treated as "inside" and select the sole item, not dismiss.
    ui::TouchEvent began = {55, 65, ui::TouchPhase::Began, 0, 0};
    EXPECT_TRUE(menu.handleTouch(began));
    ui::TouchEvent ended = {55, 65, ui::TouchPhase::Ended, 0, 0};
    menu.handleTouch(ended);
    EXPECT_TRUE(menu.wasSelected());
    EXPECT_EQ(0, menu.getSelectedIndex());
}

TEST(PopupMenuTest, SetItemsResetsState) {
    ui::PopupMenu::Item itemsA[] = {{"One"}};
    ui::PopupMenu menu(0, 0, itemsA, 1);
    menu.show();

    ui::PopupMenu::Item itemsB[] = {{"X"}, {"Y"}, {"Z"}};
    menu.setItems(itemsB, 3);
    EXPECT_EQ(3, menu.itemCount());
    EXPECT_EQ(-1, menu.getSelectedIndex());
}
