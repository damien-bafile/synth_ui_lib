#include <gtest/gtest.h>
#include "ui/widget.h"

struct CallbackSpy : ui::Widget {
    int phase = 0;
    bool onTouchBegan(const ui::TouchEvent& ev) override {
        phase = 1;
        return Widget::onTouchBegan(ev);
    }
    bool onTouchMoved(const ui::TouchEvent&) override {
        phase = 2;
        return true;
    }
    void onTouchEnded(const ui::TouchEvent&) override { phase = 3; }
    void onTouchCancelled(const ui::TouchEvent&) override { phase = 4; }
};

TEST(WidgetTest, SetBounds) {
    ui::Widget w;
    w.setBounds(10, 20, 100, 50);
    EXPECT_EQ(10, w.x());
    EXPECT_EQ(20, w.y());
    EXPECT_EQ(100, w.width());
    EXPECT_EQ(50, w.height());

    auto b = w.bounds();
    EXPECT_EQ(10, b.x);
    EXPECT_EQ(20, b.y);
    EXPECT_EQ(100, b.w);
    EXPECT_EQ(50, b.h);
}

TEST(WidgetTest, Contains) {
    ui::Widget w;
    w.setBounds(10, 10, 50, 50);
    EXPECT_TRUE(w.contains(10, 10));
    EXPECT_TRUE(w.contains(59, 59));
    EXPECT_FALSE(w.contains(60, 60));
    EXPECT_FALSE(w.contains(9, 20));
    EXPECT_FALSE(w.contains(20, 9));
}

TEST(WidgetTest, HandleTouchBegan) {
    CallbackSpy w;
    w.setBounds(0, 0, 100, 100);
    ui::TouchEvent ev = {50, 50, ui::TouchPhase::Began, 0, 0};
    EXPECT_TRUE(w.handleTouch(ev));
    EXPECT_EQ(1, w.phase);
}

TEST(WidgetTest, HandleTouchMoved) {
    CallbackSpy w;
    ui::TouchEvent ev = {50, 50, ui::TouchPhase::Moved, 0, 0};
    EXPECT_TRUE(w.handleTouch(ev));
    EXPECT_EQ(2, w.phase);
}

TEST(WidgetTest, HandleTouchEnded) {
    CallbackSpy w;
    ui::TouchEvent ev = {50, 50, ui::TouchPhase::Ended, 0, 0};
    EXPECT_TRUE(w.handleTouch(ev));
    EXPECT_EQ(3, w.phase);
}
