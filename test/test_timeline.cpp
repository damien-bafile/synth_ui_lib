#include <gtest/gtest.h>
#include "ui/anim.h"

TEST(TimelineTest, AddValue) {
    ui::Timeline tl;
    auto h = tl.add(0.0f, 100.0f, 1000, ui::Easing::LINEAR);
    ASSERT_NE(ui::Timeline::INVALID, h);

    tl.tick(0);
    EXPECT_FLOAT_EQ(0.0f, tl.value(h));
    tl.tick(500);
    EXPECT_NEAR(50.0f, tl.value(h), 0.0001f);
    tl.tick(1000);
    EXPECT_FLOAT_EQ(100.0f, tl.value(h));
    tl.tick(2000);
    EXPECT_FLOAT_EQ(100.0f, tl.value(h));
}

TEST(TimelineTest, Then) {
    ui::Timeline tl;
    auto h1 = tl.add(0.0f, 10.0f, 100, ui::Easing::LINEAR);
    auto h2 = tl.then(h1, 20.0f, 100, ui::Easing::LINEAR);
    ASSERT_NE(ui::Timeline::INVALID, h2);

    tl.tick(0);
    tl.tick(50);
    EXPECT_FLOAT_EQ(5.0f, tl.value(h1));

    tl.tick(100);
    EXPECT_FLOAT_EQ(10.0f, tl.value(h1));

    tl.tick(150);
    EXPECT_NEAR(15.0f, tl.value(h2), 0.0001f);

    tl.tick(200);
    EXPECT_FLOAT_EQ(20.0f, tl.value(h2));
}

TEST(TimelineTest, Cancel) {
    ui::Timeline tl;
    auto h = tl.add(0.0f, 100.0f, 1000);
    tl.tick(0);
    EXPECT_TRUE(tl.active(h));
    tl.cancel(h);
    tl.tick(500);
    EXPECT_FALSE(tl.active(h));
    EXPECT_FLOAT_EQ(0.0f, tl.value(h));
}

TEST(TimelineTest, Clear) {
    ui::Timeline tl;
    tl.add(0.0f, 10.0f, 100);
    tl.add(0.0f, 20.0f, 200);
    tl.tick(0);
    EXPECT_EQ(2, tl.activeCount());
    tl.clear();
    EXPECT_EQ(0, tl.activeCount());
}

TEST(TimelineTest, StaleHandle) {
    ui::Timeline tl;
    auto h1 = tl.add(0.0f, 10.0f, 100);
    tl.tick(50);
    tl.cancel(h1);
    auto h2 = tl.add(0.0f, 20.0f, 100);
    tl.tick(100);
    EXPECT_FLOAT_EQ(0.0f, tl.value(h1));
    EXPECT_TRUE(tl.active(h2));
    EXPECT_FALSE(tl.active(h1));
}

TEST(TimelineTest, PoolFull) {
    ui::Timeline tl;
    for (int i = 0; i < ui::Timeline::MAX_ANIMATIONS + 5; i++) {
        auto h = tl.add(0.0f, 1.0f, 100);
        if (i < ui::Timeline::MAX_ANIMATIONS) {
            EXPECT_NE(ui::Timeline::INVALID, h);
        } else {
            EXPECT_EQ(ui::Timeline::INVALID, h);
        }
    }
}

TEST(TimelineTest, Loop) {
    ui::Timeline tl;
    auto h = tl.add(0.0f, 10.0f, 100, ui::Easing::LINEAR, 0, ui::LoopMode::LOOP);
    tl.tick(0);
    EXPECT_FLOAT_EQ(0.0f, tl.value(h));
    tl.tick(50);
    EXPECT_FLOAT_EQ(5.0f, tl.value(h));
    tl.tick(100);
    EXPECT_FLOAT_EQ(0.0f, tl.value(h));
    tl.tick(150);
    EXPECT_FLOAT_EQ(5.0f, tl.value(h));
    EXPECT_TRUE(tl.active(h));
}

TEST(TimelineTest, PingPong) {
    ui::Timeline tl;
    auto h = tl.add(0.0f, 10.0f, 100, ui::Easing::LINEAR, 0, ui::LoopMode::PINGPONG);
    tl.tick(0);
    EXPECT_FLOAT_EQ(0.0f, tl.value(h));
    tl.tick(50);
    EXPECT_FLOAT_EQ(5.0f, tl.value(h));
    tl.tick(100);
    EXPECT_FLOAT_EQ(10.0f, tl.value(h));
    tl.tick(150);
    EXPECT_FLOAT_EQ(5.0f, tl.value(h));
    tl.tick(200);
    EXPECT_FLOAT_EQ(0.0f, tl.value(h));
}

TEST(TimelineTest, ActiveCount) {
    ui::Timeline tl;
    EXPECT_EQ(0, tl.activeCount());
    auto h1 = tl.add(0.0f, 10.0f, 100);
    tl.tick(0);
    EXPECT_EQ(1, tl.activeCount());
    auto h2 = tl.add(0.0f, 20.0f, 200);
    tl.tick(0);
    EXPECT_EQ(2, tl.activeCount());
    tl.tick(50);
    EXPECT_EQ(2, tl.activeCount());
    tl.tick(100);
    EXPECT_EQ(1, tl.activeCount());
    tl.tick(200);
    EXPECT_EQ(0, tl.activeCount());
}
