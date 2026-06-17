#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "touch.h"
#include "colors.h"

namespace ui {

// ADSR envelope parameters. Layout must match the synth's ADSR struct.
struct AdsrEnvelope {
    float attack_ms;     // 0..2000
    float decay_ms;      // 0..2000
    float sustain_level; // 0..1
    float release_ms;    // 0..4000
};

class Graph {
public:
    Graph(int x, int y, int w, int h,
          uint16_t lineColor = ACCENT_1,
          uint16_t fillColor1 = ACCENT_1,
          uint16_t fillColor2 = ACCENT_2,
          uint16_t bg = BG_DARK);

    void draw(Framebuffer& fb, const AdsrEnvelope& env);
    bool handleTouch(const TouchState& touch, AdsrEnvelope& env);

    void setLineColors(uint16_t color) { lineColor_ = color; }
    void setFillColors(uint16_t c1, uint16_t c2) { fillColor1_ = c1; fillColor2_ = c2; }
    void setPosition(int x, int y) { x_ = x; y_ = y; }
    void setSize(int w, int h) { w_ = w; h_ = h; }

    int getX() const { return x_; }
    int getY() const { return y_; }
    int getWidth() const { return w_; }
    int getHeight() const { return h_; }

private:
    int x_, y_, w_, h_;
    uint16_t lineColor_, fillColor1_, fillColor2_, bg_;
    int selectedPoint_; // -1=none, 0=A, 1=D, 2=S, 3=R

    void envToPoints(const AdsrEnvelope& env, int pts[4][2]) const;
    void pointsToEnv(const int pts[4][2], AdsrEnvelope& env) const;
};

} // namespace ui
