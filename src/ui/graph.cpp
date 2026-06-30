#include "graph.h"
#include <cmath>
#include <algorithm>

namespace ui {

static constexpr float ADSR_ATTACK_MAX_MS  = 2000.0f;
static constexpr float ADSR_DECAY_MAX_MS   = 2000.0f;
static constexpr float ADSR_RELEASE_MAX_MS = 4000.0f;
static constexpr int   POINT_RADIUS = 3;
static constexpr int   TOUCH_RADIUS = 8;
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

void Graph::envToPoints(const AdsrEnvelope& env, int pts[4][2]) const {
    int gw = w_ - PADDING * 2;
    int gh = h_ - PADDING * 2 - LABEL_H;

    float totalTime = env.attack_ms + env.decay_ms + env.release_ms;
    if (totalTime < 1.0f) totalTime = 1.0f;

    float fixedTotal = ADSR_ATTACK_MAX_MS + ADSR_DECAY_MAX_MS + ADSR_RELEASE_MAX_MS;

    pts[0][0] = x_ + PADDING + (int)(gw * env.attack_ms / totalTime);
    pts[0][1] = y_ + PADDING;

    pts[1][0] = x_ + PADDING + (int)(gw * (env.attack_ms + env.decay_ms) / totalTime);
    pts[1][1] = y_ + PADDING + (int)(gh * (1.0f - env.sustain_level));

    pts[2][0] = x_ + PADDING + gw - (int)(gw * env.release_ms / fixedTotal);
    if (pts[2][0] <= pts[1][0] + 2) pts[2][0] = pts[1][0] + 3;
    if (pts[2][0] > x_ + PADDING + gw - 2) pts[2][0] = x_ + PADDING + gw - 2;
    pts[2][1] = pts[1][1];

    pts[3][0] = x_ + PADDING + gw;
    pts[3][1] = y_ + PADDING + gh;
}

void Graph::pointsToEnv(const int pts[4][2], AdsrEnvelope& env) const {
    int gw = w_ - PADDING * 2;
    int gh = h_ - PADDING * 2 - LABEL_H;
    if (gw < 1) gw = 1;
    if (gh < 1) gh = 1;

    float ax = (float)(pts[0][0] - x_ - PADDING) / gw;
    float dx = (float)(pts[1][0] - x_ - PADDING) / gw;
    float sx = (float)(pts[2][0] - x_ - PADDING) / gw;
    float rx = (float)(pts[3][0] - x_ - PADDING) / gw;

    if (ax < 0.0f) ax = 0.0f;
    if (dx <= ax) dx = ax + 0.01f;
    if (sx <= dx) sx = dx + 0.01f;
    if (rx <= sx) rx = sx + 0.01f;
    if (rx > 1.0f) rx = 1.0f;

    float susY = 1.0f - (float)(pts[1][1] - y_ - PADDING) / gh;
    if (susY < 0.0f) susY = 0.0f;
    if (susY > 1.0f) susY = 1.0f;

    float totalMs = ADSR_ATTACK_MAX_MS + ADSR_DECAY_MAX_MS + ADSR_RELEASE_MAX_MS;
    env.attack_ms     = ax * totalMs;
    env.decay_ms      = (dx - ax) * totalMs;
    env.sustain_level = susY;
    env.release_ms    = (rx - sx) * totalMs;

    if (env.attack_ms < 0.0f) env.attack_ms = 0.0f;
    if (env.attack_ms > ADSR_ATTACK_MAX_MS) env.attack_ms = ADSR_ATTACK_MAX_MS;
    if (env.decay_ms < 0.0f) env.decay_ms = 0.0f;
    if (env.decay_ms > ADSR_DECAY_MAX_MS) env.decay_ms = ADSR_DECAY_MAX_MS;
    if (env.release_ms < 0.0f) env.release_ms = 0.0f;
    if (env.release_ms > ADSR_RELEASE_MAX_MS) env.release_ms = ADSR_RELEASE_MAX_MS;
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
        fb.drawCircle(pts[i][0], pts[i][1], POINT_RADIUS, WHITE);
    }

    const char* labels[] = {"A", "D", "S", "R"};
    int labelY = y_ + h_ - LABEL_H;
    for (int i = 0; i < 4; i++) {
        int lx = pts[i][0] - 2;
        if (lx < x_) lx = x_;
        fb.drawText(lx, labelY, labels[i], TEXT_DIM, bg_);
    }
}

bool Graph::onTouchBegan(const TouchEvent& event) {
    int pts[4][2];
    envToPoints(env_, pts);

    for (int i = 0; i < 4; i++) {
        int dx = event.x - pts[i][0];
        int dy = event.y - pts[i][1];
        if (dx * dx + dy * dy <= TOUCH_RADIUS * TOUCH_RADIUS) {
            selectedPoint_ = i;
            return true;
        }
    }
    return false;
}

bool Graph::onTouchMoved(const TouchEvent& event) {
    if (selectedPoint_ < 0) return true;

    int pts[4][2];
    envToPoints(env_, pts);

    int minX = x_ + PADDING;
    int maxX = x_ + w_ - PADDING;
    int minY = y_ + PADDING;
    int maxY = y_ + PADDING + (h_ - PADDING * 2 - LABEL_H);

    int newX = event.x;
    int newY = event.y;
    if (newX < minX) newX = minX;
    if (newX > maxX) newX = maxX;
    if (newY < minY) newY = minY;
    if (newY > maxY) newY = maxY;

    if (selectedPoint_ == 0) {
        if (newX >= pts[1][0] - 2) newX = pts[1][0] - 3;
    } else if (selectedPoint_ == 1) {
        if (newX <= pts[0][0] + 2) newX = pts[0][0] + 3;
        if (newX >= pts[2][0] - 2) newX = pts[2][0] - 3;
    } else if (selectedPoint_ == 2) {
        if (newX <= pts[1][0] + 2) newX = pts[1][0] + 3;
        if (newX >= pts[3][0] - 2) newX = pts[3][0] - 3;
    } else if (selectedPoint_ == 3) {
        if (newX <= pts[2][0] + 2) newX = pts[2][0] + 3;
    }

    pts[selectedPoint_][0] = newX;
    pts[selectedPoint_][1] = newY;

    if (selectedPoint_ == 1) {
        pts[2][1] = pts[1][1];
    } else if (selectedPoint_ == 2) {
        pts[1][1] = pts[2][1];
    }

    pointsToEnv(pts, env_);
    return true;
}

void Graph::onTouchEnded(const TouchEvent&) {
    selectedPoint_ = -1;
}

} // namespace ui
