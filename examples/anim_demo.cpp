// Host-side smoke test for the animation system. Not part of the library
// build; compile ad hoc:
//
//   c++ -std=c++14 -Wall -Wextra -Wpedantic -Isrc
//       examples/anim_demo.cpp src/ui/anim.cpp src/ui/colors.cpp -o /tmp/anim_demo
//   (all three .cpp on one line) then run /tmp/anim_demo
//
// Exits non-zero on the first failed invariant.
#include "ui/anim.h"
#include "ui/anim_source.h"
#include "ui/easing.h"
#include "ui/tween.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace ui;

static int g_failures = 0;

static void check(bool cond, const char* what) {
    if (!cond) {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

static bool nearly(float a, float b, float eps = 1e-4f) {
    return std::fabs(a - b) <= eps;
}

int main() {
    // ---- easing endpoints ----
    check(nearly(applyEasing(Easing::LINEAR, 0.0f), 0.0f), "linear(0)==0");
    check(nearly(applyEasing(Easing::LINEAR, 1.0f), 1.0f), "linear(1)==1");
    check(nearly(applyEasing(Easing::LINEAR, 0.5f), 0.5f), "linear(0.5)==0.5");
    check(nearly(applyEasing(Easing::CUBIC_OUT, 0.0f), 0.0f), "cubicOut(0)==0");
    check(nearly(applyEasing(Easing::CUBIC_OUT, 1.0f), 1.0f), "cubicOut(1)==1");
    check(nearly(applyEasing(Easing::BOUNCE_OUT, 1.0f), 1.0f), "bounceOut(1)==1");
    check(nearly(applyEasing(Easing::ELASTIC_OUT, 1.0f), 1.0f), "elasticOut(1)==1");
    check(applyEasing(Easing::CUBIC_IN, 1.5f) <= 1.0f, "easing clamps t>1");

    // ---- Tween: endpoints, delay, monotonicity ----
    Tween tw;
    tw.start(0.0f, 10.0f, 1000, Easing::LINEAR, /*now=*/100, /*delay=*/50);
    check(nearly(tw.value(100), 0.0f), "tween holds from during delay");
    check(nearly(tw.value(140), 0.0f), "tween still delayed at 140");
    check(nearly(tw.value(150), 0.0f), "tween starts at from (t=0)");
    check(nearly(tw.value(650), 5.0f), "tween linear midpoint == 5");
    check(nearly(tw.value(1150), 10.0f), "tween reaches to at end");
    check(tw.done(1150), "tween done at end");
    check(!tw.done(650), "tween not done mid-flight");
    float prev = -1.0f;
    bool monotone = true;
    for (uint32_t t = 150; t <= 1150; t += 25) {
        const float v = tw.value(t);
        if (v + 1e-4f < prev) monotone = false;
        prev = v;
    }
    check(monotone, "linear tween is monotone non-decreasing");

    // ---- Tween pingpong returns toward start on odd cycles ----
    Tween pp;
    pp.start(0.0f, 1.0f, 100, Easing::LINEAR, 0, 0, LoopMode::PINGPONG);
    check(nearly(pp.value(50), 0.5f), "pingpong up-cycle midpoint");
    check(nearly(pp.value(150), 0.5f), "pingpong down-cycle midpoint");
    check(!pp.done(10000), "pingpong never done");

    // ---- lerpColor endpoints ----
    check(lerpColor(0x1234, 0xABCD, 0.0f) == 0x1234, "lerpColor(a,b,0)==a");
    check(lerpColor(0x1234, 0xABCD, 1.0f) == 0xABCD, "lerpColor(a,b,1)==b");

    // ---- approach converges toward target ----
    float cur = 0.0f;
    for (int i = 0; i < 200; ++i) cur = approach(cur, 1.0f, 8.0f, 16);
    check(nearly(cur, 1.0f, 1e-2f), "approach converges to target");

    // ---- Timeline: basic add/tick/value ----
    Timeline tl;
    auto h = tl.add(0.0f, 100.0f, 1000, Easing::LINEAR);
    tl.tick(0);
    check(nearly(tl.value(h), 0.0f), "timeline starts at from");
    check(tl.active(h), "timeline handle active");
    tl.tick(500);
    check(nearly(tl.value(h), 50.0f), "timeline linear midpoint");
    tl.tick(1000);
    check(nearly(tl.value(h), 100.0f), "timeline reaches to");
    check(!tl.active(h), "timeline handle inactive when done");

    // ---- Timeline: delay holds from ----
    Timeline tld;
    auto hd = tld.add(2.0f, 8.0f, 500, Easing::LINEAR, /*delay=*/200);
    tld.tick(0);
    tld.tick(100);
    check(nearly(tld.value(hd), 2.0f), "delayed timeline holds from");
    tld.tick(450); // 250ms into the 500ms run
    check(nearly(tld.value(hd), 5.0f), "delayed timeline midpoint after delay");

    // ---- Timeline: pool exhaustion + cancel frees a slot ----
    Timeline pool;
    int made = 0;
    Timeline::Handle first = Timeline::INVALID;
    for (int i = 0; i < Timeline::MAX_ANIMATIONS; ++i) {
        auto hh = pool.add(0.0f, 1.0f, 100, Easing::LINEAR);
        if (i == 0) first = hh;
        if (hh != Timeline::INVALID) ++made;
    }
    check(made == Timeline::MAX_ANIMATIONS, "pool fills to capacity");
    check(pool.add(0.0f, 1.0f, 100, Easing::LINEAR) == Timeline::INVALID,
          "add past capacity returns INVALID");
    pool.cancel(first);
    check(pool.add(0.0f, 1.0f, 100, Easing::LINEAR) != Timeline::INVALID,
          "cancel frees a slot");
    check(nearly(pool.value(first), 0.0f), "stale handle reads 0 after reuse");

    // ---- Timeline: then() chains from parent's final value ----
    Timeline seq;
    auto p1 = seq.add(0.0f, 10.0f, 100, Easing::LINEAR);
    auto p2 = seq.then(p1, 20.0f, 100, Easing::LINEAR);
    seq.tick(0);
    check(seq.active(p2), "chained anim is active (waiting)");
    check(nearly(seq.value(p2), 0.0f), "chained anim idle at 0 while waiting");
    seq.tick(100); // p1 completes -> p2 activates from p1.to (10)
    seq.tick(150); // p2 midpoint: 10 -> 20 at 50% == 15
    check(nearly(seq.value(p2), 15.0f), "chained anim starts from parent's end");
    seq.tick(200);
    check(nearly(seq.value(p2), 20.0f), "chained anim reaches its target");

    // ---- mapRange: endpoints, clamp, degenerate range, easing ----
    check(nearly(mapRange(0.0f, 0.0f, 10.0f, 100.0f, 200.0f), 100.0f),
          "mapRange low endpoint");
    check(nearly(mapRange(10.0f, 0.0f, 10.0f, 100.0f, 200.0f), 200.0f),
          "mapRange high endpoint");
    check(nearly(mapRange(5.0f, 0.0f, 10.0f, 0.0f, 1.0f), 0.5f),
          "mapRange midpoint (linear)");
    check(nearly(mapRange(-5.0f, 0.0f, 10.0f, 0.0f, 1.0f), 0.0f),
          "mapRange clamps below input range");
    check(nearly(mapRange(50.0f, 0.0f, 10.0f, 0.0f, 1.0f), 1.0f),
          "mapRange clamps above input range");
    check(nearly(mapRange(1.0f, 5.0f, 5.0f, 3.0f, 9.0f), 3.0f),
          "mapRange zero-width range returns outLo");
    check(nearly(mapRange(5.0f, 0.0f, 10.0f, 0.0f, 1.0f, Easing::QUAD_IN), 0.25f),
          "mapRange applies easing (quadIn at 0.5 == 0.25)");

    // ---- EnvelopeFollower: snap, attack faster than release, convergence ----
    EnvelopeFollower ef;
    ef.configure(/*attack=*/20.0f, /*release=*/2.0f);
    check(nearly(ef.push(0.8f, 0), 0.8f), "follower snaps to first sample");
    check(nearly(ef.push(0.8f, 0), 0.8f), "follower dt==0 is a no-op");
    // From a settled 0, measure frames to cross 0.5 rising vs. falling back.
    EnvelopeFollower up;
    up.configure(20.0f, 2.0f);
    up.push(0.0f, 0);
    int upFrames = 0;
    for (uint32_t t = 16; up.value() < 0.5f && t <= 100000; t += 16) {
        up.push(1.0f, t);
        ++upFrames;
    }
    EnvelopeFollower down;
    down.configure(20.0f, 2.0f);
    down.push(1.0f, 0);
    int downFrames = 0;
    for (uint32_t t = 16; down.value() > 0.5f && t <= 100000; t += 16) {
        down.push(0.0f, t);
        ++downFrames;
    }
    check(upFrames < downFrames, "follower attack rises faster than it releases");
    EnvelopeFollower conv;
    conv.configure(20.0f, 2.0f);
    conv.push(0.0f, 0);
    for (uint32_t t = 16; t <= 4000; t += 16) conv.push(0.3f, t);
    check(nearly(conv.value(), 0.3f, 1e-2f), "follower converges to held input");
    EnvelopeFollower snap;
    snap.configure(/*attack=*/100000.0f, 2.0f);
    snap.push(0.0f, 0);
    snap.push(1.0f, 16);
    check(nearly(snap.value(), 1.0f, 1e-3f), "huge attack rate tracks instantly");
    check(std::isfinite(snap.value()), "follower value stays finite");

    // ---- Trigger: fires on rising edge, hysteresis, refractory, manual fire ----
    Trigger tg;
    tg.configure(/*on=*/0.6f, /*off=*/0.4f, /*from=*/1.0f, /*to=*/0.0f,
                 /*dur=*/100, Easing::LINEAR, /*refractory=*/50);
    check(tg.process(0.7f, 1000), "trigger fires on rising crossing");
    check(!tg.process(0.7f, 1010), "trigger does not refire while held high");
    check(!tg.process(0.65f, 1200), "trigger stays disarmed above off-level");
    check(!tg.process(0.3f, 1300), "dropping below off-level re-arms (no fire yet)");
    check(tg.process(0.8f, 1400), "trigger fires again after re-arm");
    // refractory: re-arm then immediately cross again within the window
    Trigger rf;
    rf.configure(0.6f, 0.4f, 1.0f, 0.0f, 100, Easing::LINEAR, 500);
    check(rf.process(0.7f, 0), "refractory: first fire");
    rf.process(0.3f, 10); // re-arm
    check(!rf.process(0.7f, 20), "refractory blocks a fire inside the window");
    rf.process(0.3f, 600); // re-arm past the window
    check(rf.process(0.7f, 600), "refractory allows a fire past the window");
    Trigger mt;
    mt.configure(0.6f, 0.4f, 1.0f, 0.0f, 100, Easing::LINEAR);
    mt.fire(2000);
    check(nearly(mt.value(2000), 1.0f), "manual fire starts the pulse at from");
    check(nearly(mt.value(2100), 0.0f), "pulse reaches to");
    check(!mt.active(2100), "pulse inactive once complete");

    // ---- FollowerBank: independent channels, read(), clamped counts ----
    FollowerBank<4> bank;
    bank.configure(100000.0f, 2.0f); // instant attack for a deterministic check
    const float in[4] = {0.1f, 0.2f, 0.3f, 0.4f};
    bank.push(in, 4, 0);
    bank.push(in, 4, 16);
    float out[4] = {0, 0, 0, 0};
    bank.read(out, 4);
    check(nearly(out[0], 0.1f) && nearly(out[3], 0.4f, 1e-2f),
          "bank channels smooth independently");
    check(nearly(bank.value(2), 0.3f, 1e-2f), "bank value(i) matches channel");
    check(nearly(bank.value(99), 0.0f), "bank value() out of range returns 0");
    const float in2[2] = {0.9f, 0.9f};
    bank.push(in2, 2, 32); // count < N must not touch channels 2,3
    check(nearly(bank.value(3), 0.4f, 1e-2f), "bank push count<N leaves rest alone");
    float big[6] = {0, 0, 0, 0, 0, 0};
    bank.read(big, 6); // count > N must not overrun
    check(nearly(big[3], 0.4f, 1e-2f) && big[4] == 0.0f, "bank read count>N clamps");

    if (g_failures == 0) {
        std::printf("OK: all animation invariants passed\n");
        return 0;
    }
    std::printf("%d check(s) failed\n", g_failures);
    return EXIT_FAILURE;
}
