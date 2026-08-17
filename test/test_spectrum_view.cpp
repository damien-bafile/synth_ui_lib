#include <gtest/gtest.h>
#include "ui/spectrum_view.h"
#include "ui/framebuffer.h"

static constexpr int W = 40;
static constexpr int H = 20;

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

TEST(SpectrumViewTest, GettersReflectConstructorArgs) {
    ui::SpectrumView sv(1, 2, W, H);
    EXPECT_EQ(1, sv.getX());
    EXPECT_EQ(2, sv.getY());
    EXPECT_EQ(W, sv.getWidth());
    EXPECT_EQ(H, sv.getHeight());
}

TEST(SpectrumViewTest, FullMagnitudeBarReachesTop) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::SpectrumView sv(0, 0, W, H, ui::WHITE, ui::BLACK);
    float mags[4] = {1.0f, 0.0f, 0.0f, 0.0f};
    sv.draw(fb, mags, 4);

    // Bin 0 spans x[0, binW); a full-height bar should paint the top row.
    EXPECT_TRUE(regionContainsColor(buf, 0, 0, 5, 1, ui::WHITE));
}

TEST(SpectrumViewTest, ZeroMagnitudeBinStaysBackground) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::SpectrumView sv(0, 0, W, H, ui::WHITE, ui::BLACK);
    float mags[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    sv.draw(fb, mags, 4);

    EXPECT_FALSE(regionContainsColor(buf, 0, 0, W, H, ui::WHITE));
}

TEST(SpectrumViewTest, PeakDecaysGraduallyAfterMagnitudeDrops) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);

    ui::SpectrumView sv(0, 0, W, H, ui::WHITE, ui::BLACK);
    float loud[4] = {1.0f, 0.0f, 0.0f, 0.0f};
    sv.draw(fb, loud, 4);  // peaks_[0] = 1.0

    float quiet[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    fb.fillScreen(ui::BLACK);
    sv.draw(fb, quiet, 4);  // peaks_[0] decays to 0.9, bar itself is gone

    // No bar (magnitude 0) but the peak-hold line near the top should
    // still be present -- 0.9*H = 18, well above bin 0's baseline.
    EXPECT_TRUE(regionContainsColor(buf, 0, 0, 5, 3, ui::WHITE));
}

TEST(SpectrumViewTest, ResetPeaksClearsHeldPeaks) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);

    ui::SpectrumView sv(0, 0, W, H, ui::WHITE, ui::BLACK);
    float loud[4] = {1.0f, 0.0f, 0.0f, 0.0f};
    sv.draw(fb, loud, 4);
    sv.resetPeaks();

    float quiet[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    fb.fillScreen(ui::BLACK);
    sv.draw(fb, quiet, 4);

    EXPECT_FALSE(regionContainsColor(buf, 0, 0, W, H, ui::WHITE));
}

TEST(SpectrumViewTest, CountAboveMaxBinsIsClamped) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    ui::SpectrumView sv(0, 0, W, H);
    float mags[64] = {};
    for (int i = 0; i < 64; i++) mags[i] = 1.0f;
    sv.draw(fb, mags, 64);  // must not read/write past MAX_BINS
    SUCCEED();
}
