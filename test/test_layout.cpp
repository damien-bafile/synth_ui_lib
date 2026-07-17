#include <gtest/gtest.h>
#include "ui/layout.h"

TEST(LayoutBuilderTest, Construct) {
    ui::LayoutBuilder lb(10, 20, 200, 100);
    auto area = lb.contentArea();
    EXPECT_EQ(10, area.x);
    EXPECT_EQ(20, area.y);
    EXPECT_EQ(200, area.w);
    EXPECT_EQ(100, area.h);
}

TEST(LayoutBuilderTest, UniformPadding) {
    ui::LayoutBuilder lb(0, 0, 100, 50);
    lb.setPadding(5);
    auto area = lb.contentArea();
    EXPECT_EQ(5, area.x);
    EXPECT_EQ(5, area.y);
    EXPECT_EQ(90, area.w);
    EXPECT_EQ(40, area.h);
}

TEST(LayoutBuilderTest, HvPadding) {
    ui::LayoutBuilder lb(0, 0, 100, 50);
    lb.setPadding(10, 5);
    auto area = lb.contentArea();
    EXPECT_EQ(10, area.x);
    EXPECT_EQ(5, area.y);
    EXPECT_EQ(80, area.w);
    EXPECT_EQ(40, area.h);
}

TEST(LayoutBuilderTest, FourSidePadding) {
    ui::LayoutBuilder lb(0, 0, 100, 50);
    lb.setPadding(2, 4, 6, 8);
    auto area = lb.contentArea();
    EXPECT_EQ(8, area.x);
    EXPECT_EQ(2, area.y);
    EXPECT_EQ(88, area.w);
    EXPECT_EQ(42, area.h);
}

TEST(LayoutBuilderTest, NextWidthHeight) {
    ui::LayoutBuilder lb(0, 0, 100, 100);
    lb.setSpacing(5);

    auto r1 = lb.next(40, 20);
    EXPECT_EQ(0, r1.x);
    EXPECT_EQ(0, r1.y);
    EXPECT_EQ(40, r1.w);
    EXPECT_EQ(20, r1.h);

    auto r2 = lb.next(30, 20);
    EXPECT_EQ(45, r2.x);
    EXPECT_EQ(0, r2.y);
    EXPECT_EQ(30, r2.w);
    EXPECT_EQ(20, r2.h);
}

TEST(LayoutBuilderTest, NextFullWidth) {
    ui::LayoutBuilder lb(0, 0, 100, 100);
    lb.setSpacing(5);

    auto r1 = lb.next(20);
    EXPECT_EQ(0, r1.x);
    EXPECT_EQ(0, r1.y);
    EXPECT_EQ(100, r1.w);
    EXPECT_EQ(20, r1.h);

    auto r2 = lb.next(30);
    EXPECT_EQ(0, r2.x);
    EXPECT_EQ(25, r2.y);
    EXPECT_EQ(100, r2.w);
    EXPECT_EQ(30, r2.h);
}

TEST(LayoutBuilderTest, Remaining) {
    ui::LayoutBuilder lb(0, 0, 100, 100);
    lb.setSpacing(5);
    lb.next(30, 20);

    auto r = lb.remaining();
    EXPECT_EQ(35, r.x);
    EXPECT_EQ(0, r.y);
    EXPECT_EQ(65, r.w);
    EXPECT_EQ(20, r.h);
}

TEST(LayoutBuilderTest, AllRemaining) {
    ui::LayoutBuilder lb(0, 0, 100, 100);
    lb.next(30, 20);

    auto r = lb.allRemaining();
    EXPECT_EQ(30, r.x);
    EXPECT_EQ(0, r.y);
    EXPECT_EQ(70, r.w);
    EXPECT_EQ(100, r.h);
}

TEST(LayoutBuilderTest, Cell) {
    ui::LayoutBuilder lb(0, 0, 100, 100);
    auto c = lb.cell(1, 2, 4, 4, 1, 1);
    EXPECT_EQ(25, c.x);
    EXPECT_EQ(50, c.y);
    EXPECT_EQ(25, c.w);
    EXPECT_EQ(25, c.h);
}

TEST(LayoutBuilderTest, NextRow) {
    ui::LayoutBuilder lb(0, 0, 100, 100);
    lb.setSpacing(5);
    lb.next(40, 20);
    lb.next(40, 20);

    auto r = lb.next(40, 20);
    EXPECT_EQ(90, r.x);
    EXPECT_EQ(0, r.y);

    lb.nextRow();
    auto r2 = lb.next(80, 30);
    EXPECT_EQ(0, r2.x);
    EXPECT_EQ(25, r2.y);
}

TEST(LayoutBuilderTest, Reset) {
    ui::LayoutBuilder lb(0, 0, 100, 100);
    lb.setPadding(10);
    lb.next(40, 20);
    lb.reset();

    auto r = lb.next(80, 20);
    EXPECT_EQ(10, r.x);
    EXPECT_EQ(10, r.y);
    EXPECT_EQ(80, r.w);
}

TEST(LayoutBuilderTest, Sub) {
    ui::LayoutBuilder lb(0, 0, 100, 100);
    auto sub = lb.sub({10, 20, 80, 60});
    auto area = sub.contentArea();
    EXPECT_EQ(10, area.x);
    EXPECT_EQ(20, area.y);
    EXPECT_EQ(80, area.w);
    EXPECT_EQ(60, area.h);
}
