#include <gtest/gtest.h>
#include "ui/graph.h"

TEST(GraphTest, DefaultEnvelopeAndGetters) {
    ui::Graph g(0, 0, 200, 110);
    EXPECT_EQ(0, g.getX());
    EXPECT_EQ(0, g.getY());
    EXPECT_EQ(200, g.getWidth());
    EXPECT_EQ(110, g.getHeight());

    const ui::AdsrEnvelope& env = g.getEnvelope();
    EXPECT_FLOAT_EQ(10.0f, env.attack_ms);
    EXPECT_FLOAT_EQ(100.0f, env.decay_ms);
    EXPECT_FLOAT_EQ(0.7f, env.sustain_level);
    EXPECT_FLOAT_EQ(200.0f, env.release_ms);
}

TEST(GraphTest, SetEnvelopeRoundTrips) {
    ui::Graph g(0, 0, 200, 110);
    ui::AdsrEnvelope env{50.0f, 300.0f, 0.4f, 500.0f};
    g.setEnvelope(env);

    const ui::AdsrEnvelope& out = g.getEnvelope();
    EXPECT_FLOAT_EQ(50.0f, out.attack_ms);
    EXPECT_FLOAT_EQ(300.0f, out.decay_ms);
    EXPECT_FLOAT_EQ(0.4f, out.sustain_level);
    EXPECT_FLOAT_EQ(500.0f, out.release_ms);
}

TEST(GraphTest, DraggingAttackHandleChangesAttackTime) {
    ui::Graph g(0, 0, 200, 110);
    // With the default (short) attack/decay/sustain/release, the attack
    // handle sits near the graph's top-left padding corner, well away from
    // the other three handles -- touching there is unambiguously nearest
    // to point 0 (attack).
    ui::TouchEvent began = {13, 4, ui::TouchPhase::Began, 0, 0};
    EXPECT_TRUE(g.handleTouch(began));

    float before = g.getEnvelope().attack_ms;

    // Drag well to the right, inside the attack band, to increase the
    // attack fraction substantially.
    ui::TouchEvent moved = {32, 4, ui::TouchPhase::Moved, 0, 16};
    g.handleTouch(moved);

    float after = g.getEnvelope().attack_ms;
    EXPECT_GT(after, before);
}

TEST(GraphTest, TouchEndedClearsSelectionSoFurtherMovesAreNoOp) {
    ui::Graph g(0, 0, 200, 110);
    ui::TouchEvent began = {13, 4, ui::TouchPhase::Began, 0, 0};
    g.handleTouch(began);

    ui::TouchEvent ended = {13, 4, ui::TouchPhase::Ended, 0, 0};
    g.handleTouch(ended);

    float before = g.getEnvelope().attack_ms;
    ui::TouchEvent moved = {150, 4, ui::TouchPhase::Moved, 0, 16};
    g.handleTouch(moved);
    float after = g.getEnvelope().attack_ms;

    EXPECT_FLOAT_EQ(before, after);
}

TEST(GraphTest, TouchOutsideBoundsIsNotCaptured) {
    ui::Graph g(0, 0, 200, 110);
    ui::TouchEvent ev = {500, 500, ui::TouchPhase::Began, 0, 0};
    EXPECT_FALSE(g.handleTouch(ev));
}
