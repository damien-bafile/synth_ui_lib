#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"

namespace ui {

struct AdsrEnvelope {
    float attack_ms;
    float decay_ms;
    float sustain_level;
    float release_ms;
};

class Graph : public Widget {
public:
    Graph(int x, int y, int w, int h,
          uint16_t lineColor = ACCENT_1,
          uint16_t fillColor1 = ACCENT_1,
          uint16_t fillColor2 = ACCENT_2,
          uint16_t bg = BG_DARK);

    void draw(Framebuffer& fb, const AdsrEnvelope& env);

    void setEnvelope(const AdsrEnvelope& env) noexcept { env_ = env; }
    const AdsrEnvelope& getEnvelope() const noexcept { return env_; }

    void setLineColors(uint16_t color) noexcept { lineColor_ = color; }
    void setBackground(uint16_t bg) noexcept { bg_ = bg; }
    void setFillColors(uint16_t c1, uint16_t c2) noexcept { fillColor1_ = c1; fillColor2_ = c2; }
    void setPosition(int x, int y) noexcept { setBounds(x, y, w_, h_); }
    void setSize(int w, int h) noexcept { setBounds(x_, y_, w, h); }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }

    // Pixel positions of the four envelope handles (attack peak, decay end,
    // sustain-hold end, release end) as drawn by draw(). Lets overlays (e.g.
    // playheads) align with the on-screen envelope geometry.
    void envelopePoints(int pts[4][2]) const { envToPoints(env_, pts); }

    // Vertical playhead over the drawn envelope: phase 1..4 = A/D/S/R,
    // frac = 0..1 progress through that phase's segment.
    void drawPlayhead(Framebuffer& fb, uint8_t phase, float frac,
                      uint16_t color) const;

    // Split form of drawPlayhead for callers that animate the position
    // themselves: playheadX maps phase+frac to a pixel column (-1 when
    // idle/invalid), drawPlayheadLine draws the marker at a given column.
    int playheadX(uint8_t phase, float frac) const;
    void drawPlayheadLine(Framebuffer& fb, int x, uint16_t color) const;

private:
    uint16_t lineColor_, fillColor1_, fillColor2_, bg_;
    int selectedPoint_ = -1;
    AdsrEnvelope env_ = {10.0f, 100.0f, 0.7f, 200.0f};

    bool onTouchBegan(const TouchEvent& event) override;
    bool onTouchMoved(const TouchEvent& event) override;
    void onTouchEnded(const TouchEvent& event) override;

    void envToPoints(const AdsrEnvelope& env, int pts[4][2]) const;
};

} // namespace ui
