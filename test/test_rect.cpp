#include <gtest/gtest.h>
#include "ui/rect.h"

TEST(RectTest, ContainsInside) {
    ui::Rect r = {10, 20, 100, 50};
    EXPECT_TRUE(r.contains(10, 20));
    EXPECT_TRUE(r.contains(50, 40));
    EXPECT_TRUE(r.contains(109, 69));
}

TEST(RectTest, ContainsEdge) {
    ui::Rect r = {0, 0, 100, 100};
    EXPECT_TRUE(r.contains(0, 0));
    EXPECT_FALSE(r.contains(100, 100));
    EXPECT_FALSE(r.contains(100, 50));
    EXPECT_FALSE(r.contains(50, 100));
}

TEST(RectTest, ContainsOutside) {
    ui::Rect r = {10, 10, 20, 20};
    EXPECT_FALSE(r.contains(5, 15));
    EXPECT_FALSE(r.contains(15, 5));
    EXPECT_FALSE(r.contains(30, 15));
    EXPECT_FALSE(r.contains(15, 30));
}
