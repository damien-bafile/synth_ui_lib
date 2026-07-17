#include <gtest/gtest.h>
#include "ui/framebuffer.h"

static constexpr int W = 64;
static constexpr int H = 32;

TEST(FramebufferTest, SetPixel) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);

    fb.setPixel(10, 5, 0xFFFF);
    int idx = (5 * W + 10) * 2;
    EXPECT_EQ(0xFF, buf[idx + 0]);
    EXPECT_EQ(0xFF, buf[idx + 1]);
}

TEST(FramebufferTest, FillRect) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);

    fb.fillRect(5, 5, 10, 10, 0xF800);
    int idx = (5 * W + 5) * 2;
    EXPECT_EQ(0x00, buf[idx + 0]);
    EXPECT_EQ(0xF8, buf[idx + 1]);
}

TEST(FramebufferTest, DrawRect) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);

    fb.drawRect(2, 2, 8, 8, 0x07E0);
    int idx = (2 * W + 2) * 2;
    EXPECT_EQ(0xE0, buf[idx + 0]);
    EXPECT_EQ(0x07, buf[idx + 1]);
}

TEST(FramebufferTest, FillScreen) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);

    fb.fillScreen(0x001F);
    for (int i = 0; i < W * H; i++) {
        EXPECT_EQ(0x1F, buf[i * 2 + 0]) << "at pixel " << i;
        EXPECT_EQ(0x00, buf[i * 2 + 1]) << "at pixel " << i;
    }
}

static void fakePainter(ui::Framebuffer&, void*) {}

TEST(FramebufferTest, Overlay) {
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);

    EXPECT_EQ(0, fb.getOverlayCount());
    fb.enqueueOverlay(0, 0, 10, 10, fakePainter, nullptr);
    EXPECT_EQ(1, fb.getOverlayCount());
    fb.clearOverlays();
    EXPECT_EQ(0, fb.getOverlayCount());
}
