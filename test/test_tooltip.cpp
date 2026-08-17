#include <gtest/gtest.h>
#include "ui/tooltip.h"
#include "ui/framebuffer.h"

static constexpr int W = 64;
static constexpr int H = 32;

static bool regionContainsColor(const uint8_t* buf, uint16_t color) {
    uint8_t lo = color & 0xFF;
    uint8_t hi = (color >> 8) & 0xFF;
    for (int i = 0; i < W * H; i++) {
        if (buf[i * 2 + 0] == lo && buf[i * 2 + 1] == hi) return true;
    }
    return false;
}

TEST(TooltipTest, InitiallyNotVisible) {
    ui::Tooltip tip;
    EXPECT_FALSE(tip.isVisible());
}

TEST(TooltipTest, ShowSetsPositionTextAndVisibility) {
    ui::Tooltip tip;
    tip.show(10, 20, "Cutoff: 440Hz");
    EXPECT_TRUE(tip.isVisible());
    EXPECT_EQ(10, tip.getX());
    EXPECT_EQ(20, tip.getY());
    EXPECT_STREQ("Cutoff: 440Hz", tip.getText());
}

TEST(TooltipTest, DismissClearsVisibility) {
    ui::Tooltip tip;
    tip.show(0, 0, "hi");
    ASSERT_TRUE(tip.isVisible());
    tip.dismiss();
    EXPECT_FALSE(tip.isVisible());
}

TEST(TooltipTest, WidthGrowsWithTextLength) {
    ui::Tooltip tip;
    tip.show(0, 0, "a");
    int shortW = tip.getWidth();
    tip.show(0, 0, "a much longer tooltip string");
    int longW = tip.getWidth();
    EXPECT_GT(longW, shortW);
}

TEST(TooltipTest, DrawEnqueuesOverlayOnlyWhileVisible) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);

    ui::Tooltip tip;
    EXPECT_EQ(0, fb.getOverlayCount());
    tip.draw(fb);  // not visible yet -- no-op
    EXPECT_EQ(0, fb.getOverlayCount());

    tip.show(5, 5, "hi");
    tip.draw(fb);
    EXPECT_EQ(1, fb.getOverlayCount());
}

TEST(TooltipTest, PaintOverlaysRendersBorderAndText) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::Tooltip tip(ui::WHITE, ui::BLACK, ui::RED);
    tip.show(5, 5, "hi");
    tip.draw(fb);
    fb.paintOverlays();

    EXPECT_TRUE(regionContainsColor(buf, ui::RED));    // border
    EXPECT_TRUE(regionContainsColor(buf, ui::WHITE));  // text
}
