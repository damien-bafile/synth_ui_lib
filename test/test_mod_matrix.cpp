#include <gtest/gtest.h>
#include "ui/mod_matrix.h"
#include "ui/framebuffer.h"
#include "ui/touch_dispatcher.h"

// Geometry reference: ModMatrix(0,0,150,90, 3 sources, 3 dests).
// LABEL_W=30, LABEL_H=12 -> grid 120x78 -> cellW=40, cellH=26.
// Cell (row0,col0): x[30,70), y[12,38); center ~= (50,25).

static const char* kSources[] = {"LFO1", "ENV1", "ENV2"};
static const char* kDests[]   = {"CUT", "PITCH", "AMP"};

static void tap(ui::TouchDispatcher& disp, int x, int y) {
    ui::TouchState ts[] = {{x, y, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);
}

TEST(ModMatrixTest, GettersReflectConstructorArgs) {
    ui::ModMatrix mm(0, 0, 150, 90, kSources, 3, kDests, 3);
    EXPECT_EQ(3, mm.getSourceCount());
    EXPECT_EQ(3, mm.getDestCount());
    EXPECT_EQ(0, mm.getX());
    EXPECT_EQ(150, mm.getWidth());
}

TEST(ModMatrixTest, AllCellsStartAtDepthZero) {
    ui::ModMatrix mm(0, 0, 150, 90, kSources, 3, kDests, 3);
    EXPECT_EQ(0, mm.getDepthLevel(0, 0));
    EXPECT_FLOAT_EQ(0.0f, mm.getDepth(0, 0));
}

TEST(ModMatrixTest, TapCyclesDepthLevelAndWraps) {
    ui::ModMatrix mm(0, 0, 150, 90, kSources, 3, kDests, 3);
    ui::TouchDispatcher disp;
    disp.add(&mm);

    tap(disp, 45, 20);  // cell (0,0)
    EXPECT_EQ(1, mm.getDepthLevel(0, 0));
    EXPECT_TRUE(mm.wasCellChanged());
    int row = -1, col = -1;
    mm.getChangedCell(row, col);
    EXPECT_EQ(0, row);
    EXPECT_EQ(0, col);

    tap(disp, 45, 20);
    EXPECT_EQ(2, mm.getDepthLevel(0, 0));
    tap(disp, 45, 20);
    EXPECT_EQ(3, mm.getDepthLevel(0, 0));
    tap(disp, 45, 20);
    EXPECT_EQ(0, mm.getDepthLevel(0, 0));  // wraps back to OFF
}

TEST(ModMatrixTest, TapDifferentCellDoesNotAffectOthers) {
    ui::ModMatrix mm(0, 0, 150, 90, kSources, 3, kDests, 3);
    ui::TouchDispatcher disp;
    disp.add(&mm);

    tap(disp, 45, 20);   // cell (0,0)
    tap(disp, 85, 45);   // cell (1,1): x[70,110) y[38,64)

    EXPECT_EQ(1, mm.getDepthLevel(0, 0));
    EXPECT_EQ(1, mm.getDepthLevel(1, 1));
    EXPECT_EQ(0, mm.getDepthLevel(0, 1));
}

TEST(ModMatrixTest, TapOutsideGridIsIgnored) {
    ui::ModMatrix mm(0, 0, 150, 90, kSources, 3, kDests, 3);
    ui::TouchDispatcher disp;
    disp.add(&mm);

    tap(disp, 10, 10);  // inside the label gutter, not a cell
    EXPECT_FALSE(mm.wasCellChanged());
}

TEST(ModMatrixTest, SetDepthLevelClampsAndGetDepthComputesFraction) {
    ui::ModMatrix mm(0, 0, 150, 90, kSources, 3, kDests, 3);
    mm.setDepthLevel(1, 2, 3);  // HIGH
    EXPECT_EQ(3, mm.getDepthLevel(1, 2));
    EXPECT_FLOAT_EQ(1.0f, mm.getDepth(1, 2));

    mm.setDepthLevel(1, 2, 99);  // clamped to DEPTH_LEVELS-1
    EXPECT_EQ(ui::ModMatrix::DEPTH_LEVELS - 1, mm.getDepthLevel(1, 2));

    mm.setDepthLevel(1, 2, -5);  // clamped to 0
    EXPECT_EQ(0, mm.getDepthLevel(1, 2));
}

TEST(ModMatrixTest, DrawPaintsCellFillOnlyWhenDepthNonZero) {
    static constexpr int W = 150, H = 90;
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);
    fb.fillScreen(ui::BLACK);

    ui::ModMatrix mm(0, 0, W, H, kSources, 3, kDests, 3, ui::WHITE, ui::BLACK, ui::GRAY_DARK);
    mm.draw(fb);

    int idx = (25 * W + 50) * 2;  // center of cell (0,0)
    EXPECT_NE(0xFF, buf[idx + 0]);  // not WHITE (0xFFFF) yet -- depth 0

    mm.setDepthLevel(0, 0, 1);
    fb.fillScreen(ui::BLACK);
    mm.draw(fb);
    EXPECT_EQ(0xFF, buf[idx + 0]);
    EXPECT_EQ(0xFF, buf[idx + 1]);
}

TEST(ModMatrixTest, HigherDepthCoversMoreOfTheCell) {
    static constexpr int W = 150, H = 90;
    uint8_t buf[W * H * 2] = {};
    ui::Framebuffer fb(buf, W, H);

    ui::ModMatrix mm(0, 0, W, H, kSources, 3, kDests, 3, ui::WHITE, ui::BLACK, ui::GRAY_DARK);
    // Point near the left edge of cell (0,0): covered by a HIGH fill but
    // not by a LOW one.
    int idx = (25 * W + 35) * 2;

    mm.setDepthLevel(0, 0, 1);  // LOW
    fb.fillScreen(ui::BLACK);
    mm.draw(fb);
    EXPECT_NE(0xFF, buf[idx + 0]);

    mm.setDepthLevel(0, 0, 3);  // HIGH
    fb.fillScreen(ui::BLACK);
    mm.draw(fb);
    EXPECT_EQ(0xFF, buf[idx + 0]);
    EXPECT_EQ(0xFF, buf[idx + 1]);
}
