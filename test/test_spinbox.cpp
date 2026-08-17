#include <gtest/gtest.h>
#include "ui/spinbox.h"
#include "ui/framebuffer.h"
#include "ui/touch_dispatcher.h"

// Geometry reference: Spinbox(0,0,60,20, min=0,max=10,step=1,value=5).
// BTN_W=12 -> minus button x[0,12), plus button x[48,60).

static void tap(ui::TouchDispatcher& disp, int x, int y) {
    ui::TouchState ts[] = {{x, y, true}};
    disp.dispatch(ts, 1, 0);
    ts[0].pressed = false;
    disp.dispatch(ts, 1, 100);
}

TEST(SpinboxTest, GettersReflectConstructorArgs) {
    ui::Spinbox sb(0, 0, 60, 20, 0.0f, 10.0f, 1.0f, 5.0f);
    EXPECT_FLOAT_EQ(5.0f, sb.getValue());
    EXPECT_FLOAT_EQ(0.0f, sb.getMin());
    EXPECT_FLOAT_EQ(10.0f, sb.getMax());
    EXPECT_FLOAT_EQ(1.0f, sb.getStep());
}

TEST(SpinboxTest, ConstructorClampsInitialValue) {
    ui::Spinbox sb(0, 0, 60, 20, 0.0f, 10.0f, 1.0f, 500.0f);
    EXPECT_FLOAT_EQ(10.0f, sb.getValue());
}

TEST(SpinboxTest, TapPlusIncrementsByStep) {
    uint8_t buf[60 * 20 * 2] = {};
    ui::Framebuffer fb(buf, 60, 20);
    ui::Spinbox sb(0, 0, 60, 20, 0.0f, 10.0f, 1.0f, 5.0f);
    sb.draw(fb, sb.getValue());  // populates the button hit-rects

    ui::TouchDispatcher disp;
    disp.add(&sb);

    EXPECT_FALSE(sb.wasChanged());
    tap(disp, 55, 10);  // inside plus button
    EXPECT_TRUE(sb.wasChanged());
    EXPECT_FALSE(sb.wasChanged());  // clears after read
    EXPECT_FLOAT_EQ(6.0f, sb.getValue());
}

TEST(SpinboxTest, TapMinusDecrementsByStep) {
    uint8_t buf[60 * 20 * 2] = {};
    ui::Framebuffer fb(buf, 60, 20);
    ui::Spinbox sb(0, 0, 60, 20, 0.0f, 10.0f, 1.0f, 5.0f);
    sb.draw(fb, sb.getValue());

    ui::TouchDispatcher disp;
    disp.add(&sb);

    tap(disp, 5, 10);  // inside minus button
    EXPECT_FLOAT_EQ(4.0f, sb.getValue());
}

TEST(SpinboxTest, TapMiddleDoesNotChangeValue) {
    uint8_t buf[60 * 20 * 2] = {};
    ui::Framebuffer fb(buf, 60, 20);
    ui::Spinbox sb(0, 0, 60, 20, 0.0f, 10.0f, 1.0f, 5.0f);
    sb.draw(fb, sb.getValue());

    ui::TouchDispatcher disp;
    disp.add(&sb);

    tap(disp, 30, 10);  // between the buttons
    EXPECT_FALSE(sb.wasChanged());
    EXPECT_FLOAT_EQ(5.0f, sb.getValue());
}

TEST(SpinboxTest, TapPlusAtMaxStaysClamped) {
    uint8_t buf[60 * 20 * 2] = {};
    ui::Framebuffer fb(buf, 60, 20);
    ui::Spinbox sb(0, 0, 60, 20, 0.0f, 10.0f, 1.0f, 10.0f);
    sb.draw(fb, sb.getValue());

    ui::TouchDispatcher disp;
    disp.add(&sb);

    tap(disp, 55, 10);
    EXPECT_FLOAT_EQ(10.0f, sb.getValue());
}

TEST(SpinboxTest, SetValueClampsToRange) {
    ui::Spinbox sb(0, 0, 60, 20, 0.0f, 10.0f, 1.0f, 5.0f);
    sb.setValue(500.0f);
    EXPECT_FLOAT_EQ(10.0f, sb.getValue());
    sb.setValue(-500.0f);
    EXPECT_FLOAT_EQ(0.0f, sb.getValue());
}

TEST(SpinboxTest, SetRangeAndStepUpdateGetters) {
    ui::Spinbox sb(0, 0, 60, 20, 0.0f, 10.0f, 1.0f, 5.0f);
    sb.setRange(-5.0f, 5.0f);
    EXPECT_FLOAT_EQ(-5.0f, sb.getMin());
    EXPECT_FLOAT_EQ(5.0f, sb.getMax());
    sb.setStep(0.5f);
    EXPECT_FLOAT_EQ(0.5f, sb.getStep());
}
