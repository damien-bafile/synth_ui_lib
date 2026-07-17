#include <gtest/gtest.h>
#include "ui/page_layout.h"

static int g_drawCount = 0;
static void dummyDraw(ui::Framebuffer&) { g_drawCount++; }
static bool dummyTouch(const ui::TouchEvent&) { return true; }

TEST(PageLayoutTest, AddPage) {
    ui::PageLayout pl;
    g_drawCount = 0;
    pl.addPage("Page1", dummyDraw, dummyTouch);
    EXPECT_EQ(1, pl.pageCount());
    EXPECT_EQ(0, pl.currentPage());
}

TEST(PageLayoutTest, MaxPages) {
    ui::PageLayout pl;
    for (int i = 0; i < ui::PageLayout::MAX_PAGES + 3; i++) {
        pl.addPage("P", dummyDraw);
    }
    EXPECT_EQ(ui::PageLayout::MAX_PAGES, pl.pageCount());
}

TEST(PageLayoutTest, SetPageValid) {
    ui::PageLayout pl;
    pl.addPage("P1", dummyDraw);
    pl.addPage("P2", dummyDraw);
    pl.setPage(1);
    EXPECT_EQ(1, pl.currentPage());
}

TEST(PageLayoutTest, SetPageInvalid) {
    ui::PageLayout pl;
    pl.addPage("P1", dummyDraw);
    pl.setPage(-1);
    EXPECT_EQ(0, pl.currentPage());
    pl.setPage(10);
    EXPECT_EQ(0, pl.currentPage());
}
