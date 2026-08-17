#include <gtest/gtest.h>
#include "ui/waveform_view.h"
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

TEST(WaveformViewTest, GettersReflectConstructorArgs) {
    ui::WaveformView view(2, 3, 40, 20);
    EXPECT_EQ(2, view.getX());
    EXPECT_EQ(3, view.getY());
    EXPECT_EQ(40, view.getWidth());
    EXPECT_EQ(20, view.getHeight());
}

TEST(WaveformViewTest, EmptyDataShowsPlaceholderText) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::WaveformView view(0, 0, W, H, ui::BLACK, ui::GRAY_DARK);
    view.draw(fb, nullptr, 0, ui::WHITE, "No Data");

    // Placeholder text is drawn in the frame color, not the wave color.
    EXPECT_TRUE(regionContainsColor(buf, ui::GRAY_DARK));
}

TEST(WaveformViewTest, DrawWithDataPaintsWaveColor) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    int16_t samples[64];
    for (int i = 0; i < 64; i++) {
        samples[i] = static_cast<int16_t>(30000 * ((i % 2 == 0) ? 1 : -1));
    }

    ui::WaveformView view(0, 0, W, H, ui::BLACK, ui::GRAY_DARK);
    view.draw(fb, samples, 64, ui::CYAN, "No Data");

    EXPECT_TRUE(regionContainsColor(buf, ui::CYAN));
}

TEST(WaveformViewTest, DrawMarkerPaintsColorAtNormalizedPosition) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::WaveformView view(0, 0, W, H);
    view.drawMarker(fb, 0.5f, ui::MAGENTA);

    EXPECT_TRUE(regionContainsColor(buf, ui::MAGENTA));
}

TEST(WaveformViewTest, SetColorsUpdatesPlaceholderColor) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::WaveformView view(0, 0, W, H, ui::BLACK, ui::GRAY_DARK);
    view.setColors(ui::BLACK, ui::ORANGE);
    view.draw(fb, nullptr, 0, ui::WHITE, "No Data");

    EXPECT_TRUE(regionContainsColor(buf, ui::ORANGE));
}
