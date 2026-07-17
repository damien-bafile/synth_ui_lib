#include <gtest/gtest.h>
#include "ui/touch.h"

TEST(TouchCalibrationTest, Identity) {
    ui::TouchCalibration cal;
    auto s = cal.transform(100, 200, true);
    EXPECT_EQ(100, s.x);
    EXPECT_EQ(200, s.y);
    EXPECT_TRUE(s.pressed);

    s = cal.transform(0, 0, false);
    EXPECT_EQ(0, s.x);
    EXPECT_EQ(0, s.y);
    EXPECT_FALSE(s.pressed);

    s = cal.transform(-10, -20, true);
    EXPECT_EQ(-10, s.x);
    EXPECT_EQ(-20, s.y);
    EXPECT_TRUE(s.pressed);
}

TEST(TouchCalibrationTest, Scale) {
    ui::TouchCalibration cal;
    cal.setMatrix(2.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f);
    auto s = cal.transform(100, 200, true);
    EXPECT_EQ(200, s.x);
    EXPECT_EQ(100, s.y);
    EXPECT_TRUE(s.pressed);
}

TEST(TouchCalibrationTest, Offset) {
    ui::TouchCalibration cal;
    cal.setMatrix(1.0f, 0.0f, 10.0f, 0.0f, 1.0f, -20.0f);
    auto s = cal.transform(100, 200, true);
    EXPECT_EQ(110, s.x);
    EXPECT_EQ(180, s.y);
}

TEST(TouchCalibrationTest, Affine) {
    ui::TouchCalibration cal;
    cal.setMatrix(1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f);
    auto s = cal.transform(100, 10, true);
    EXPECT_EQ(105, s.x);
    EXPECT_EQ(10, s.y);
}
