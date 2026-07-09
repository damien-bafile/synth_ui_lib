// Host-side smoke test for the animation system. Not part of the library
// build; compile ad hoc:
//
//   c++ -std=c++14 -Wall -Wextra -Wpedantic -Isrc
//       examples/anim_demo.cpp src/ui/anim.cpp src/ui/colors.cpp -o /tmp/anim_demo
//   (all three .cpp on one line) then run /tmp/anim_demo
//
// Exits non-zero on the first failed invariant.
#include "ui/anim.h"
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

    if (g_failures == 0) {
        std::printf("OK: all animation invariants passed\n");
        return 0;
    }
    std::printf("%d check(s) failed\n", g_failures);
    return EXIT_FAILURE;
}
