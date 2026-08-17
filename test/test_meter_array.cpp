#include <gtest/gtest.h>
#include "ui/meter_array.h"
#include "ui/framebuffer.h"

static constexpr int W = 64;
static constexpr int H = 64;

static bool regionContainsColor(const uint8_t* buf, uint16_t color) {
    uint8_t lo = color & 0xFF;
    uint8_t hi = (color >> 8) & 0xFF;
    for (int i = 0; i < W * H; i++) {
        if (buf[i * 2 + 0] == lo && buf[i * 2 + 1] == hi) return true;
    }
    return false;
}

TEST(MeterArrayTest, GetLevelColorThresholds) {
    EXPECT_EQ(ui::METER_LOW, ui::MeterArray::getLevelColor(0.0f));
    EXPECT_EQ(ui::METER_LOW, ui::MeterArray::getLevelColor(0.49f));
    EXPECT_EQ(ui::METER_MID, ui::MeterArray::getLevelColor(0.5f));
    EXPECT_EQ(ui::METER_MID, ui::MeterArray::getLevelColor(0.79f));
    EXPECT_EQ(ui::METER_HIGH, ui::MeterArray::getLevelColor(0.8f));
    EXPECT_EQ(ui::METER_HIGH, ui::MeterArray::getLevelColor(1.0f));
}

TEST(MeterArrayTest, DrawVerticalPaintsLevelColor) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::MeterArray meter(0, 0, W, H, 2, ui::MeterArray::MeterStyle::VERTICAL);
    float levels[2] = {0.9f, 0.9f};  // clearly in the METER_HIGH band
    meter.draw(fb, levels);

    EXPECT_TRUE(regionContainsColor(buf, ui::METER_HIGH));
}

TEST(MeterArrayTest, DrawHorizontalAndCircularDoNotCrash) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    float levels[2] = {0.3f, 0.6f};

    ui::MeterArray horiz(0, 0, W, H, 2, ui::MeterArray::MeterStyle::HORIZONTAL);
    horiz.draw(fb, levels);

    ui::MeterArray circ(0, 0, W, H, 2, ui::MeterArray::MeterStyle::CIRCULAR);
    circ.draw(fb, levels);
    // No crash is the assertion; both styles produce some drawn output.
    EXPECT_TRUE(regionContainsColor(buf, ui::METER_LOW) ||
                regionContainsColor(buf, ui::METER_MID));
}
