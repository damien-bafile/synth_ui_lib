#include <gtest/gtest.h>
#include "ui/midi_learn_badge.h"
#include "ui/framebuffer.h"

static constexpr int W = 32;
static constexpr int H = 32;
static constexpr int CX = 16;
static constexpr int CY = 16;

static bool regionContainsColor(const uint8_t* buf, uint16_t color) {
    uint8_t lo = color & 0xFF;
    uint8_t hi = (color >> 8) & 0xFF;
    for (int i = 0; i < W * H; i++) {
        if (buf[i * 2 + 0] == lo && buf[i * 2 + 1] == hi) return true;
    }
    return false;
}

static bool pixelIs(const uint8_t* buf, int x, int y, uint16_t color) {
    int idx = (y * W + x) * 2;
    return buf[idx + 0] == (color & 0xFF) && buf[idx + 1] == ((color >> 8) & 0xFF);
}

TEST(MidiLearnBadgeTest, IsPulseBrightAlternatesOnPeriod) {
    EXPECT_TRUE(ui::MidiLearnBadge::isPulseBright(0));
    EXPECT_TRUE(ui::MidiLearnBadge::isPulseBright(499));
    EXPECT_FALSE(ui::MidiLearnBadge::isPulseBright(500));
    EXPECT_FALSE(ui::MidiLearnBadge::isPulseBright(999));
    EXPECT_TRUE(ui::MidiLearnBadge::isPulseBright(1000));  // wraps
}

TEST(MidiLearnBadgeTest, IdleDrawsOutlineOnlyNoActiveFill) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::MidiLearnBadge badge(ui::RED, ui::GREEN);
    badge.draw(fb, CX, CY, /*learning=*/false, 0);

    EXPECT_FALSE(pixelIs(buf, CX, CY, ui::RED));
    EXPECT_TRUE(regionContainsColor(buf, ui::GREEN));
    EXPECT_FALSE(regionContainsColor(buf, ui::RED));
}

TEST(MidiLearnBadgeTest, LearningBrightPhaseFillsActiveColor) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::MidiLearnBadge badge(ui::RED, ui::GREEN);
    badge.draw(fb, CX, CY, /*learning=*/true, 0);  // ticks=0 -> bright phase

    // Near the circle's edge, away from the "L" glyph drawn at center.
    EXPECT_TRUE(pixelIs(buf, CX + 4, CY, ui::RED));
}

TEST(MidiLearnBadgeTest, LearningDimPhaseFillsInactiveColor) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::MidiLearnBadge badge(ui::RED, ui::GREEN);
    badge.draw(fb, CX, CY, /*learning=*/true, 500);  // dim phase

    EXPECT_TRUE(pixelIs(buf, CX + 4, CY, ui::GREEN));
    EXPECT_FALSE(pixelIs(buf, CX + 4, CY, ui::RED));
}

TEST(MidiLearnBadgeTest, SetColorsChangesDrawnColors) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::MidiLearnBadge badge(ui::RED, ui::GREEN);
    badge.setColors(ui::CYAN, ui::MAGENTA);
    badge.draw(fb, CX, CY, /*learning=*/true, 0);

    EXPECT_TRUE(pixelIs(buf, CX + 4, CY, ui::CYAN));
    EXPECT_FALSE(regionContainsColor(buf, ui::RED));
}
