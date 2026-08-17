#include <gtest/gtest.h>
#include "ui/label.h"
#include "ui/framebuffer.h"

static constexpr int W = 64;
static constexpr int H = 32;
static constexpr uint16_t FG = 0xFFFF;
static constexpr uint16_t BG = 0x0000;

static bool regionContainsColor(const uint8_t* buf, int x0, int y0, int w, int h,
                                 uint16_t color) {
    uint8_t lo = color & 0xFF;
    uint8_t hi = (color >> 8) & 0xFF;
    for (int y = y0; y < y0 + h; y++) {
        for (int x = x0; x < x0 + w; x++) {
            int idx = (y * W + x) * 2;
            if (buf[idx + 0] == lo && buf[idx + 1] == hi) return true;
        }
    }
    return false;
}

TEST(LabelTest, DrawPaintsForegroundPixels) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(BG);

    ui::Label label(5, 5, 20, 8, FG, BG, ui::Label::Align::LEFT);
    label.draw(fb, "A");

    EXPECT_TRUE(regionContainsColor(buf, 0, 0, W, H, FG));
}

TEST(LabelTest, SetColorsChangesInkColor) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(BG);

    ui::Label label(5, 5, 20, 8, FG, BG, ui::Label::Align::LEFT);
    static constexpr uint16_t OTHER_FG = 0x07E0;
    label.setColors(OTHER_FG, BG);
    label.draw(fb, "A");

    EXPECT_FALSE(regionContainsColor(buf, 0, 0, W, H, FG));
    EXPECT_TRUE(regionContainsColor(buf, 0, 0, W, H, OTHER_FG));
}

TEST(LabelTest, SetBoundsMovesDrawnRegion) {
    uint8_t bufA[W * H * 2] = {};
    ui::Framebuffer fbA(bufA, W, H);
    fbA.fillScreen(BG);
    ui::Label labelA(0, 0, 20, 8, FG, BG, ui::Label::Align::LEFT);
    labelA.draw(fbA, "A");
    // Confined to the left half near the top.
    EXPECT_FALSE(regionContainsColor(bufA, 40, 20, 24, 12, FG));

    uint8_t bufB[W * H * 2] = {};
    ui::Framebuffer fbB(bufB, W, H);
    fbB.fillScreen(BG);
    ui::Label labelB(0, 0, 20, 8, FG, BG, ui::Label::Align::LEFT);
    labelB.setBounds(40, 20, 20, 8);
    labelB.draw(fbB, "A");
    EXPECT_TRUE(regionContainsColor(bufB, 40, 20, 24, 12, FG));
}
