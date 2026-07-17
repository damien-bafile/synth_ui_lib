#include <gtest/gtest.h>
#include "ui/tween.h"

TEST(TweenTest, Lerp) {
    EXPECT_FLOAT_EQ(0.0f, ui::lerp(0.0f, 10.0f, 0.0f));
    EXPECT_FLOAT_EQ(10.0f, ui::lerp(0.0f, 10.0f, 1.0f));
    EXPECT_FLOAT_EQ(5.0f, ui::lerp(0.0f, 10.0f, 0.5f));
    EXPECT_FLOAT_EQ(-5.0f, ui::lerp(-10.0f, 0.0f, 0.5f));
    EXPECT_FLOAT_EQ(15.0f, ui::lerp(10.0f, 20.0f, 0.5f));
}

TEST(TweenTest, LerpColor) {
    uint16_t black = ui::RGB565(0, 0, 0);
    uint16_t white = ui::RGB565(255, 255, 255);
    EXPECT_EQ(black, ui::lerpColor(black, white, 0.0f));
    EXPECT_EQ(white, ui::lerpColor(black, white, 1.0f));

    uint16_t mid = ui::lerpColor(black, white, 0.5f);
    EXPECT_EQ(33808, mid);

    uint16_t red = ui::RGB565(255, 0, 0);
    uint16_t blue = ui::RGB565(0, 0, 255);
    EXPECT_EQ(32784, ui::lerpColor(red, blue, 0.5f));
}

TEST(TweenTest, Approach) {
    EXPECT_FLOAT_EQ(5.0f, ui::approach(0.0f, 10.0f, 1.0f, 500));
    EXPECT_FLOAT_EQ(10.0f, ui::approach(0.0f, 10.0f, 1.0f, 2000));
    EXPECT_FLOAT_EQ(5.0f, ui::approach(5.0f, 5.0f, 1.0f, 500));
    float v = ui::approach(0.0f, 10.0f, 0.5f, 1000);
    EXPECT_GT(v, 4.0f);
    EXPECT_LT(v, 6.0f);
}

TEST(TweenTest, StartValue) {
    ui::Tween t;
    t.start(0.0f, 100.0f, 1000, ui::Easing::LINEAR, 0);
    EXPECT_FLOAT_EQ(0.0f, t.value(0));
    EXPECT_FLOAT_EQ(50.0f, t.value(500));
    EXPECT_FLOAT_EQ(100.0f, t.value(1000));
    EXPECT_FLOAT_EQ(100.0f, t.value(2000));
}

TEST(TweenTest, Done) {
    ui::Tween t;
    t.start(0.0f, 1.0f, 100, ui::Easing::LINEAR, 0);
    EXPECT_FALSE(t.done(0));
    EXPECT_FALSE(t.done(50));
    EXPECT_TRUE(t.done(100));
    EXPECT_TRUE(t.done(200));
}

TEST(TweenTest, Retarget) {
    ui::Tween t;
    t.start(0.0f, 100.0f, 1000, ui::Easing::LINEAR, 0);
    t.retarget(200.0f, 500);
    float v = t.value(500);
    EXPECT_GE(v, 50.0f);
    EXPECT_FLOAT_EQ(200.0f, t.value(1500));
}

TEST(TweenTest, Loop) {
    ui::Tween t;
    t.start(0.0f, 10.0f, 100, ui::Easing::LINEAR, 0, 0, ui::LoopMode::LOOP);
    EXPECT_FLOAT_EQ(5.0f, t.value(50));
    EXPECT_FLOAT_EQ(0.0f, t.value(100));
    EXPECT_FLOAT_EQ(5.0f, t.value(150));
    EXPECT_FALSE(t.done(10000));
}

TEST(TweenTest, PingPong) {
    ui::Tween t;
    t.start(0.0f, 10.0f, 100, ui::Easing::LINEAR, 0, 0, ui::LoopMode::PINGPONG);
    EXPECT_FLOAT_EQ(5.0f, t.value(50));
    EXPECT_FLOAT_EQ(10.0f, t.value(100));
    EXPECT_FLOAT_EQ(5.0f, t.value(150));
    EXPECT_FLOAT_EQ(0.0f, t.value(200));
    EXPECT_FALSE(t.done(10000));
}
