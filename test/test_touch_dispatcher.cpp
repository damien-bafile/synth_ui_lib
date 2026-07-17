#include <gtest/gtest.h>
#include "ui/touch_dispatcher.h"
#include "ui/widget.h"

struct SpyWidget : ui::Widget {
    int beganCount = 0;
    int movedCount = 0;
    int endedCount = 0;
    int cancelledCount = 0;
    int tapCount = 0;
    int doubleTapCount = 0;
    int longPressCount = 0;
    int dragBeganCount = 0;
    int dragMovedCount = 0;
    int dragEndedCount = 0;
    bool consumeTouch = true;

    bool onTouchBegan(const ui::TouchEvent&) override {
        beganCount++;
        return consumeTouch;
    }
    bool onTouchMoved(const ui::TouchEvent&) override {
        movedCount++;
        return true;
    }
    void onTouchEnded(const ui::TouchEvent&) override { endedCount++; }
    void onTouchCancelled(const ui::TouchEvent&) override { cancelledCount++; }
    void onTap(const ui::TouchEvent&) override { tapCount++; }
    void onDoubleTap(const ui::TouchEvent&) override { doubleTapCount++; }
    void onLongPress(const ui::TouchEvent&) override { longPressCount++; }
    void onDragBegan(const ui::TouchEvent&) override { dragBeganCount++; }
    void onDragMoved(const ui::TouchEvent&, int, int) override { dragMovedCount++; }
    void onDragEnded(const ui::TouchEvent&) override { dragEndedCount++; }
};

TEST(TouchDispatcherTest, AddRemove) {
    ui::TouchDispatcher disp;
    SpyWidget w;
    w.setBounds(0, 0, 100, 100);

    EXPECT_EQ(0, disp.count());
    disp.add(&w);
    EXPECT_EQ(1, disp.count());
    disp.remove(&w);
    EXPECT_EQ(0, disp.count());
}

TEST(TouchDispatcherTest, Clear) {
    ui::TouchDispatcher disp;
    SpyWidget w;
    disp.add(&w);
    disp.add(&w);
    disp.clear();
    EXPECT_EQ(0, disp.count());
}

TEST(TouchDispatcherTest, BeganCaptures) {
    ui::TouchDispatcher disp;
    SpyWidget w;
    w.setBounds(0, 0, 100, 100);
    disp.add(&w);

    ui::TouchState ts[] = {{50, 50, true}};
    disp.dispatch(ts, 1, 0);
    EXPECT_EQ(1, w.beganCount);
}

TEST(TouchDispatcherTest, MovedAfterBegan) {
    ui::TouchDispatcher disp;
    SpyWidget w;
    w.setBounds(0, 0, 100, 100);
    disp.add(&w);

    ui::TouchState ts[] = {{50, 50, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].x = 60; ts[0].y = 60;
    disp.dispatch(ts, 1, 16);
    EXPECT_EQ(1, w.movedCount);
}

TEST(TouchDispatcherTest, EndedReleases) {
    ui::TouchDispatcher disp;
    SpyWidget w;
    w.setBounds(0, 0, 100, 100);
    disp.add(&w);

    ui::TouchState ts[] = {{50, 50, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);
    EXPECT_EQ(1, w.endedCount);
}

TEST(TouchDispatcherTest, TapDetected) {
    ui::TouchDispatcher disp;
    SpyWidget w;
    w.setBounds(0, 0, 100, 100);
    disp.add(&w);

    ui::TouchState ts[] = {{50, 50, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 200);
    EXPECT_EQ(1, w.tapCount);
}

TEST(TouchDispatcherTest, DoubleTap) {
    ui::TouchDispatcher disp;
    SpyWidget w;
    w.setBounds(0, 0, 100, 100);
    disp.add(&w);

    ui::TouchState ts[] = {{50, 50, true}};

    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);

    ts[0].pressed = true;
    disp.dispatch(ts, 1, 200);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 300);

    EXPECT_EQ(2, w.tapCount);
    EXPECT_EQ(1, w.doubleTapCount);
}

TEST(TouchDispatcherTest, DragThreshold) {
    ui::TouchDispatcher disp;
    SpyWidget w;
    w.setBounds(0, 0, 100, 100);
    disp.add(&w);

    ui::TouchState ts[] = {{10, 10, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].x = 20; ts[0].y = 10;
    disp.dispatch(ts, 1, 16);

    EXPECT_EQ(1, w.dragBeganCount);
    EXPECT_EQ(1, w.dragMovedCount);
}

TEST(TouchDispatcherTest, LongPress) {
    ui::TouchDispatcher disp;
    SpyWidget w;
    w.setBounds(0, 0, 100, 100);
    disp.add(&w);

    ui::TouchState ts[] = {{50, 50, true}};
    disp.dispatch(ts, 1, 0);
    disp.dispatch(ts, 1, ui::TouchDispatcher::kLongPressMs + 50);

    EXPECT_EQ(1, w.longPressCount);
}

TEST(TouchDispatcherTest, ZOrderSort) {
    ui::TouchDispatcher disp;
    SpyWidget w1, w2;
    w1.setBounds(0, 0, 100, 100);
    w2.setBounds(0, 0, 100, 100);
    w1.setZOrder(1);
    w2.setZOrder(0);

    disp.add(&w2);
    disp.add(&w1);

    ui::TouchState ts[] = {{50, 50, true}};
    disp.dispatch(ts, 1, 0);

    EXPECT_EQ(1, w1.beganCount);
    EXPECT_EQ(0, w2.beganCount);
}

TEST(TouchDispatcherTest, RemoveCancels) {
    ui::TouchDispatcher disp;
    SpyWidget w;
    w.setBounds(0, 0, 100, 100);
    disp.add(&w);

    ui::TouchState ts[] = {{50, 50, true}};
    disp.dispatch(ts, 1, 0);
    disp.remove(&w);

    EXPECT_EQ(1, w.cancelledCount);
    EXPECT_EQ(0, disp.count());
}

TEST(TouchDispatcherTest, DispatchEvent) {
    ui::TouchDispatcher disp;
    SpyWidget w;
    w.setBounds(0, 0, 100, 100);
    disp.add(&w);

    ui::TouchEvent ev;
    ev.x = 50; ev.y = 50;
    ev.phase = ui::TouchPhase::Began;
    ev.fingerId = 0;
    ev.timestamp = 0;
    disp.dispatchEvent(ev);
    EXPECT_EQ(1, w.beganCount);

    ev.phase = ui::TouchPhase::Moved;
    ev.x = 60;
    disp.dispatchEvent(ev);
    EXPECT_EQ(1, w.movedCount);

    ev.phase = ui::TouchPhase::Ended;
    disp.dispatchEvent(ev);
    EXPECT_EQ(1, w.endedCount);
}
