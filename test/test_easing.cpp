#include <gtest/gtest.h>
#include "ui/easing.h"

TEST(EasingTest, EaseLinear) {
    EXPECT_FLOAT_EQ(0.0f, ui::easeLinear(0.0f));
    EXPECT_FLOAT_EQ(0.5f, ui::easeLinear(0.5f));
    EXPECT_FLOAT_EQ(1.0f, ui::easeLinear(1.0f));
}

TEST(EasingTest, EaseQuadIn) {
    EXPECT_NEAR(0.25f, ui::easeQuadIn(0.5f), 0.0001f);
    EXPECT_LT(ui::easeQuadIn(0.1f), ui::easeQuadIn(0.2f));
}

TEST(EasingTest, EaseQuadOut) {
    EXPECT_NEAR(0.75f, ui::easeQuadOut(0.5f), 0.0001f);
}

TEST(EasingTest, EaseQuadInOut) {
    EXPECT_NEAR(0.5f, ui::easeQuadInOut(0.5f), 0.0001f);
    EXPECT_LE(ui::easeQuadInOut(0.25f), ui::easeQuadInOut(0.5f));
}

TEST(EasingTest, EaseCubicIn) {
    EXPECT_NEAR(0.125f, ui::easeCubicIn(0.5f), 0.0001f);
}

TEST(EasingTest, EaseCubicOut) {
    EXPECT_NEAR(0.875f, ui::easeCubicOut(0.5f), 0.0001f);
}

TEST(EasingTest, EaseCubicInOut) {
    EXPECT_NEAR(0.5f, ui::easeCubicInOut(0.5f), 0.0001f);
    EXPECT_TRUE(ui::easeCubicInOut(0.25f) <= 0.5f);
}

TEST(EasingTest, EaseBackOut) {
    EXPECT_FLOAT_EQ(0.0f, ui::easeBackOut(0.0f));
    EXPECT_FLOAT_EQ(1.0f, ui::easeBackOut(1.0f));
    EXPECT_GT(ui::easeBackOut(0.5f), 0.5f);
}

TEST(EasingTest, ApplyEasingClamp) {
    EXPECT_FLOAT_EQ(0.0f, ui::applyEasing(ui::Easing::LINEAR, -0.5f));
    EXPECT_FLOAT_EQ(1.0f, ui::applyEasing(ui::Easing::LINEAR, 1.5f));
    EXPECT_FLOAT_EQ(0.0f, ui::applyEasing(ui::Easing::CUBIC_OUT, -1.0f));
    EXPECT_FLOAT_EQ(1.0f, ui::applyEasing(ui::Easing::CUBIC_OUT, 2.0f));
    EXPECT_NEAR(0.25f, ui::applyEasing(ui::Easing::QUAD_IN, 0.5f), 0.0001f);
    EXPECT_NEAR(0.5f, ui::applyEasing(ui::Easing::SINE_INOUT, 0.5f), 0.0001f);
    EXPECT_GT(ui::applyEasing(ui::Easing::ELASTIC_OUT, 0.3f), 0.0f);
    EXPECT_LT(ui::applyEasing(ui::Easing::BOUNCE_OUT, 0.9f), 1.0f);
}
