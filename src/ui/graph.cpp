#include "graph.h"
#include <cmath>
#include <algorithm>

namespace ui {

static constexpr float ADSR_ATTACK_MAX_MS  = 2000.0f;
static constexpr float ADSR_DECAY_MAX_MS   = 2000.0f;
static constexpr float ADSR_RELEASE_MAX_MS = 4000.0f;

// Cube-root time mapping: musically typical values (5-200 ms) occupy a
// visible share of the band instead of collapsing against the left edge as
// they do linearly against the 2000-4000 ms maxes. Exactly invertible, so
// draw and touch round-trip losslessly and a dragged handle stays under the
// finger.
static float timeToFrac(float ms, float max_ms) {
    return cbrtf(ms / max_ms);
}
static float fracToTime(float t, float max_ms) {
    return t * t * t * max_ms;
}
static constexpr int   POINT_RADIUS = 3;
static constexpr int   LABEL_H      = 10;
static constexpr int   PADDING      = 4;

static uint16_t lerp565(uint16_t c1, uint16_t c2, float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    uint32_t r1 = (c1 >> 11) & 0x1F;
    uint32_t g1 = (c1 >> 5) & 0x3F;
    uint32_t b1 = c1 & 0x1F;
    uint32_t r2 = (c2 >> 11) & 0x1F;
    uint32_t g2 = (c2 >> 5) & 0x3F;
    uint32_t b2 = c2 & 0x1F;
    uint32_t r = r1 + (uint32_t)((int32_t)(r2 - r1) * t / 256);
    uint32_t g = g1 + (uint32_t)((int32_t)(g2 - g1) * t / 256);
    uint32_t b = b1 + (uint32_t)((int32_t)(b2 - b1) * t / 256);
    return (uint16_t)((r << 11) | (g << 5) | b);
}

Graph::Graph(int x, int y, int w, int h,
             uint16_t lineColor, uint16_t fillColor1,
             uint16_t fillColor2, uint16_t bg)
    : lineColor_(lineColor), fillColor1_(fillColor1),
      fillColor2_(fillColor2), bg_(bg) {
    setBounds(x, y, w, h);
}

// Fixed per-segment bands so the pixel<->ms mapping is identical in both
// directions: A, D, R each get 30% of the width (scaled by their max ms),
// with a fixed 10% sustain-hold segment between D and R. A dragged point
// therefore stays under the finger instead of re-normalizing away.
void Graph::envToPoints(const AdsrEnvelope& env, int pts[4][2]) const {
    int gw = w_ - PADDING * 2;
    int gh = h_ - PADDING * 2 - LABEL_H;
    int x0 = x_ + PADDING;
    int y0 = y_ + PADDING;

    int aBand = (int)(gw * 0.30f);
    int dBand = aBand;
    int rBand = aBand;
    int holdW = gw - aBand - dBand - rBand;  // sustain hold, ~10%

    pts[0][0] = x0 + (int)(aBand * timeToFrac(env.attack_ms, ADSR_ATTACK_MAX_MS));
    pts[0][1] = y0;

    pts[1][0] = pts[0][0] + (int)(dBand * timeToFrac(env.decay_ms, ADSR_DECAY_MAX_MS));
    pts[1][1] = y0 + (int)(gh * (1.0f - env.sustain_level));

    pts[2][0] = pts[1][0] + holdW;
    pts[2][1] = pts[1][1];

    pts[3][0] = pts[2][0] + (int)(rBand * timeToFrac(env.release_ms, ADSR_RELEASE_MAX_MS));
    pts[3][1] = y0 + gh;
}

void Graph::draw(Framebuffer& fb, const AdsrEnvelope& env) {
    int pts[4][2];
    envToPoints(env, pts);

    fb.fillRect(x_, y_, w_, h_, bg_);

    int gw = w_ - PADDING * 2;
    int gh = h_ - PADDING * 2 - LABEL_H;

    for (int i = 0; i <= 4; i++) {
        int gy = y_ + PADDING + (gh * i) / 4;
        fb.fillRect(x_ + PADDING, gy, gw, 1, GRAY_DARK);
    }

    if (pts[0][0] > x_ + PADDING)
        fb.fillRect(pts[0][0], y_ + PADDING, 1, gh, GRAY_DARK);
    if (pts[1][0] > x_ + PADDING && pts[1][0] < x_ + PADDING + gw)
        fb.fillRect(pts[1][0], y_ + PADDING, 1, gh, GRAY_DARK);
    if (pts[2][0] > pts[1][0] + 2 && pts[2][0] < x_ + PADDING + gw)
        fb.fillRect(pts[2][0], y_ + PADDING, 1, gh, GRAY_DARK);

    int baseY = y_ + PADDING + gh;
    int peakY = pts[0][1];
    int decayEndY = pts[1][1];
    int sustainEndX = pts[2][0];
    int releaseEndX = pts[3][0];

    for (int row = 0; row < gh; row++) {
        int fy = y_ + PADDING + row;
        if (fy < peakY || fy >= baseY) continue;

        float t = (float)row / gh;
        uint16_t color = lerp565(fillColor1_, fillColor2_, t);

        int curveLeft, curveRight;

        int attackLineH = baseY - peakY;
        if (attackLineH > 0) {
            float frac = (float)(fy - peakY) / attackLineH;
            curveLeft = pts[0][0] + (int)(frac * ((x_ + PADDING) - pts[0][0]));
        } else {
            curveLeft = x_ + PADDING;
        }

        if (fy <= decayEndY) {
            int decayLineH = decayEndY - peakY;
            if (decayLineH > 0) {
                float frac = (float)(fy - peakY) / decayLineH;
                curveRight = pts[0][0] + (int)(frac * (pts[1][0] - pts[0][0]));
            } else {
                curveRight = pts[1][0];
            }
        } else {
            int releaseLineH = baseY - decayEndY;
            if (releaseLineH > 0) {
                float frac = (float)(fy - decayEndY) / releaseLineH;
                curveRight = sustainEndX + (int)(frac * (releaseEndX - sustainEndX));
            } else {
                curveRight = releaseEndX;
            }
        }

        if (curveLeft < x_ + PADDING) curveLeft = x_ + PADDING;
        if (curveRight > x_ + PADDING + gw) curveRight = x_ + PADDING + gw;

        int fillW = curveRight - curveLeft;
        if (fillW > 0) {
            fb.fillRect(curveLeft, fy, fillW, 1, color);
        }
    }

    fb.drawLine(x_ + PADDING, baseY, pts[0][0], pts[0][1], lineColor_);
    fb.drawLine(pts[0][0], pts[0][1], pts[1][0], pts[1][1], lineColor_);
    fb.drawLine(pts[1][0], pts[1][1], pts[2][0], pts[2][1], lineColor_);
    fb.drawLine(pts[2][0], pts[2][1], pts[3][0], pts[3][1], lineColor_);

    for (int i = 0; i < 4; i++) {
        uint16_t c = (i == selectedPoint_) ? HIGHLIGHT : lineColor_;
        fb.fillCircle(pts[i][0], pts[i][1], POINT_RADIUS, c);
        fb.drawCircle(pts[i][0], pts[i][1], POINT_RADIUS, TEXT);
    }

    const char* labels[] = {"A", "D", "S", "R"};
    int labelY = y_ + h_ - LABEL_H;
    for (int i = 0; i < 4; i++) {
        int lx = pts[i][0] - 2;
        if (lx < x_) lx = x_;
        fb.drawText(lx, labelY, labels[i], TEXT_DIM, bg_);
    }
}

int Graph::playheadX(uint8_t phase, float frac) const {
    const int gx0 = x_ + PADDING;
    const int gw = w_ - PADDING * 2;
    if (gw <= 0) return -1;

    int pts[4][2];
    envToPoints(env_, pts);

    // Each phase's playhead sweeps the segment between the previous
    // handle and its own.
    int segX0, segX1;
    switch (phase) {
        case 1: segX0 = gx0;       segX1 = pts[0][0]; break;  // ATTACK
        case 2: segX0 = pts[0][0]; segX1 = pts[1][0]; break;  // DECAY
        case 3: segX0 = pts[1][0]; segX1 = pts[2][0]; break;  // SUSTAIN
        case 4: segX0 = pts[2][0]; segX1 = pts[3][0]; break;  // RELEASE
        default: return -1;                                   // IDLE
    }

    if (frac < 0.0f) frac = 0.0f;
    if (frac > 1.0f) frac = 1.0f;
    int x = segX0 + (int)((segX1 - segX0) * frac);
    if (x < gx0) x = gx0;
    if (x > gx0 + gw - 1) x = gx0 + gw - 1;
    return x;
}

void Graph::drawPlayheadLine(Framebuffer& fb, int x, uint16_t color) const {
    const int gyTop = y_ + PADDING;
    const int gyBot = y_ + h_ - LABEL_H - 2;
    if (x < 0 || gyBot <= gyTop) return;

    fb.fillRect(x, gyTop, 1, gyBot - gyTop, color);
    fb.fillCircle(x, gyTop + 1, 3, color);
}

void Graph::drawPlayhead(Framebuffer& fb, uint8_t phase, float frac,
                         uint16_t color) const {
    drawPlayheadLine(fb, playheadX(phase, frac), color);
}

bool Graph::onTouchBegan(const TouchEvent& event) {
    // Finger-friendly: any touch inside the graph grabs the nearest point.
    if (!contains(event.x, event.y)) return false;

    int pts[4][2];
    envToPoints(env_, pts);

    long bestD = -1;
    for (int i = 0; i < 4; i++) {
        long dx = event.x - pts[i][0];
        long dy = event.y - pts[i][1];
        long d = dx * dx + dy * dy;
        if (bestD < 0 || d < bestD) {
            bestD = d;
            selectedPoint_ = i;
        }
    }
    return true;
}

bool Graph::onTouchMoved(const TouchEvent& event) {
    if (selectedPoint_ < 0) return true;

    int gw = w_ - PADDING * 2;
    int gh = h_ - PADDING * 2 - LABEL_H;
    if (gh < 1) gh = 1;
    int x0 = x_ + PADDING;
    int y0 = y_ + PADDING;

    int aBand = (int)(gw * 0.30f);
    if (aBand < 1) aBand = 1;
    int holdW = gw - aBand * 3;

    int pts[4][2];
    envToPoints(env_, pts);

    // Each point maps its touch position back through the same fixed band
    // used to draw it, so the handle tracks the finger exactly.
    auto clamp01 = [](float v) {
        return v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v);
    };

    switch (selectedPoint_) {
    case 0:  // attack time (x)
        env_.attack_ms = fracToTime(clamp01((float)(event.x - x0) / aBand),
                                    ADSR_ATTACK_MAX_MS);
        break;
    case 1:  // decay time (x) + sustain level (y)
        env_.decay_ms = fracToTime(clamp01((float)(event.x - pts[0][0]) / aBand),
                                   ADSR_DECAY_MAX_MS);
        env_.sustain_level = clamp01(1.0f - (float)(event.y - y0) / gh);
        break;
    case 2:  // sustain level (y); x slides the plateau = adjusts decay time
        env_.sustain_level = clamp01(1.0f - (float)(event.y - y0) / gh);
        env_.decay_ms = fracToTime(
            clamp01((float)(event.x - holdW - pts[0][0]) / aBand),
            ADSR_DECAY_MAX_MS);
        break;
    case 3:  // release time (x)
        env_.release_ms = fracToTime(
            clamp01((float)(event.x - (pts[1][0] + holdW)) / aBand),
            ADSR_RELEASE_MAX_MS);
        break;
    }
    return true;
}

void Graph::onTouchEnded(const TouchEvent&) {
    selectedPoint_ = -1;
}

} // namespace ui
