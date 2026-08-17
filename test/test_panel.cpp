#include <gtest/gtest.h>
#include "ui/panel.h"
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

TEST(PanelTest, GettersReflectConstructorArgs) {
    ui::Panel p(2, 3, 40, 20, "Osc", ui::WHITE);
    EXPECT_EQ(2, p.getX());
    EXPECT_EQ(3, p.getY());
    EXPECT_EQ(40, p.getWidth());
    EXPECT_EQ(20, p.getHeight());
    EXPECT_STREQ("Osc", p.getTitle());
}

TEST(PanelTest, SetTitleAndColorUpdate) {
    ui::Panel p(0, 0, 40, 20, "Osc");
    p.setTitle("Filter");
    EXPECT_STREQ("Filter", p.getTitle());
    p.setTitleColor(ui::RED);  // just confirm it doesn't crash / compiles through
}

TEST(PanelTest, DrawPaintsTitleColor) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::Panel p(0, 0, W, H, "Osc", ui::WHITE);
    p.draw(fb);

    EXPECT_TRUE(regionContainsColor(buf, ui::WHITE));
}
