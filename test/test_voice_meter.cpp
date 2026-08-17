#include <gtest/gtest.h>
#include "ui/voice_meter.h"
#include "ui/framebuffer.h"

static constexpr int W = 64;
static constexpr int H = 16;

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

TEST(VoiceMeterTest, ActiveVoicePaintsOnColor) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::VoiceMeter meter(0, 0, /*boxW=*/8, /*boxH=*/8, /*pitch=*/10,
                         ui::RED, ui::GREEN);
    bool active[2] = {true, false};
    meter.draw(fb, active, 2);

    // Voice 0 (active, RED) occupies x in [0,8); voice 1 (inactive, GREEN)
    // occupies x in [10,18) given pitch=10.
    EXPECT_TRUE(regionContainsColor(buf, 0, 0, 8, 8, ui::RED));
    EXPECT_FALSE(regionContainsColor(buf, 0, 0, 8, 8, ui::GREEN));
    EXPECT_TRUE(regionContainsColor(buf, 10, 0, 8, 8, ui::GREEN));
    EXPECT_FALSE(regionContainsColor(buf, 10, 0, 8, 8, ui::RED));
}

TEST(VoiceMeterTest, SetColorsChangesOnOffColors) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::VoiceMeter meter(0, 0, 8, 8, 10, ui::RED, ui::GREEN);
    meter.setColors(ui::BLUE, ui::WHITE, ui::BLACK);
    bool active[1] = {true};
    meter.draw(fb, active, 1);

    EXPECT_TRUE(regionContainsColor(buf, 0, 0, 8, 8, ui::BLUE));
    EXPECT_FALSE(regionContainsColor(buf, 0, 0, 8, 8, ui::RED));
}
