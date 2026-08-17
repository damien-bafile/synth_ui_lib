#include <gtest/gtest.h>
#include "ui/infobar.h"
#include "ui/framebuffer.h"

static constexpr int W = 64;
static constexpr int H = 16;

static bool regionContainsColor(const uint8_t* buf, uint16_t color) {
    uint8_t lo = color & 0xFF;
    uint8_t hi = (color >> 8) & 0xFF;
    for (int i = 0; i < W * H; i++) {
        if (buf[i * 2 + 0] == lo && buf[i * 2 + 1] == hi) return true;
    }
    return false;
}

TEST(InfoBarTest, GettersReflectConstructorArgs) {
    ui::InfoBar bar(1, 2, 60, 12);
    EXPECT_EQ(1, bar.getX());
    EXPECT_EQ(2, bar.getY());
    EXPECT_EQ(60, bar.getWidth());
    EXPECT_EQ(12, bar.getHeight());
}

TEST(InfoBarTest, SetBoundsUpdatesGetters) {
    ui::InfoBar bar(0, 0, 60, 12);
    bar.setBounds(5, 6, 70, 20);
    EXPECT_EQ(5, bar.getX());
    EXPECT_EQ(6, bar.getY());
    EXPECT_EQ(70, bar.getWidth());
    EXPECT_EQ(20, bar.getHeight());
}

TEST(InfoBarTest, DrawLeftRightPaintsForeground) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::InfoBar bar(0, 0, W, H, ui::WHITE, ui::BLACK);
    bar.draw(fb, "L", "R");

    EXPECT_TRUE(regionContainsColor(buf, ui::WHITE));
}

TEST(InfoBarTest, DrawSegmentsPaintsEachColor) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::InfoBar bar(0, 0, W, H);
    ui::InfoBar::Segment segs[] = {
        {"A", ui::RED},
        {"B", ui::GREEN},
    };
    bar.draw(fb, segs, 2);

    EXPECT_TRUE(regionContainsColor(buf, ui::RED));
    EXPECT_TRUE(regionContainsColor(buf, ui::GREEN));
}
