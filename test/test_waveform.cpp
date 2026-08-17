#include <gtest/gtest.h>
#include "ui/waveform.h"
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

TEST(WaveformTest, DrawSinePaintsForeground) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::Waveform wf(0, 0, W, H, ui::WHITE, ui::BLACK);
    wf.draw(fb, ui::Waveform::WaveformType::SINE, 1.0f);

    EXPECT_TRUE(regionContainsColor(buf, ui::WHITE));
}

TEST(WaveformTest, DrawEachTypeDoesNotCrash) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    ui::Waveform wf(0, 0, W, H, ui::WHITE, ui::BLACK);

    wf.draw(fb, ui::Waveform::WaveformType::SINE, 1.0f);
    wf.draw(fb, ui::Waveform::WaveformType::SQUARE, 1.0f);
    wf.draw(fb, ui::Waveform::WaveformType::TRIANGLE, 1.0f);
    wf.draw(fb, ui::Waveform::WaveformType::SAWTOOTH, 1.0f);
    SUCCEED();
}

TEST(WaveformTest, DrawCustomPaintsForeground) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::Waveform wf(0, 0, W, H, ui::WHITE, ui::BLACK);
    float samples[8] = {0.0f, 0.5f, 1.0f, 0.5f, 0.0f, -0.5f, -1.0f, -0.5f};
    wf.drawCustom(fb, samples, 8);

    EXPECT_TRUE(regionContainsColor(buf, ui::WHITE));
}
