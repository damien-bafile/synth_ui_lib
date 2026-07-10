#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "button.h"
#include "touch_dispatcher.h"
#include "colors.h"
#include "icons.h"

namespace ui {

// Bottom transport strip: play/stop/record icon buttons, a row of small
// color swatches (e.g. one per sequencer track), and left/right status
// text. Everything is vertically centered within the bar. The host app
// supplies state each frame and polls tappedButton() after dispatch.
class TransportBar {
public:
    enum class PlayState : uint8_t { STOPPED, PLAYING, PAUSED };

    static constexpr int MAX_SWATCHES = 16;
    static constexpr int BTN_PLAY = 0;
    static constexpr int BTN_STOP = 1;
    static constexpr int BTN_REC  = 2;
    static constexpr int BTN_MENU = 3;

    enum class BpmAnimMode : uint8_t {
        SWEEP_BAR,
        PULSE,
        WALKING_DOT,
        STEP_PROGRESS,
        COUNT
    };

    TransportBar(int x, int y, int w, int h);

    void setSwatches(const uint16_t* colors, int count);

    // Per-track note activity animation (0-255 intensity per track).
    // Pass null or count=0 to disable.
    void setNoteActivity(const uint8_t* intensities, int count);

    // BPM-synced timing indicator.
    void setBpmIndicator(BpmAnimMode mode, float stepProgress, uint8_t currentStep);

    // Optional hamburger button at the bar's right edge (off by default);
    // tappedButton() reports it as BTN_MENU.
    void setMenuButton(bool on) noexcept { menuButton_ = on; }

    // Optional 2x2 status dot after the left text (e.g. external-clock
    // health). Hidden while show is false.
    void setStatusDot(bool show, uint16_t color) noexcept {
        statusDot_ = show;
        statusDotColor_ = color;
    }

    void draw(Framebuffer& fb, PlayState state, bool recording,
              const char* leftText, const char* rightText);

    void addTo(TouchDispatcher& d);
    int tappedButton();  // BTN_PLAY/BTN_STOP/BTN_REC/BTN_MENU, -1 = none

    // Left/right text column origin (lets the app overlay a tap target).
    int leftTextX() const noexcept { return x_ + LEFT_TEXT_X; }
    int rightTextX(const char* text) const noexcept {
        int tw = Framebuffer::textWidth(text);
        int rx = x_ + w_ - tw - 4;
        if (menuButton_) rx -= BTN_W + BTN_GAP + 2;
        return rx;
    }
    int textY() const noexcept { return y_ + (h_ - 7) / 2; }  // FONT_H rows

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }

private:
    static constexpr int BTN_W       = 26;
    static constexpr int BTN_H       = 12;
    static constexpr int BTN_GAP     = 2;
    static constexpr int SWATCH_X    = 90;
    static constexpr int SWATCH_STEP = 9;
    static constexpr int SWATCH_SIZE = 4;
    static constexpr int LEFT_TEXT_X = 142;

    int btnX(int i) const noexcept {
        if (i == BTN_MENU) return x_ + w_ - BTN_W - 2;
        return x_ + 2 + i * (BTN_W + BTN_GAP);
    }
    int btnY() const noexcept { return y_ + (h_ - BTN_H) / 2; }

    int x_, y_, w_, h_;
    Button btns_[4];
    uint16_t swatches_[MAX_SWATCHES];
    int swatchCount_ = 0;
    bool menuButton_ = false;
    bool statusDot_ = false;
    uint16_t statusDotColor_ = 0;

    // Animation state
    uint8_t noteActivity_[MAX_SWATCHES] = {};
    int noteActivityCount_ = 0;
    BpmAnimMode bpmAnimMode_ = BpmAnimMode::SWEEP_BAR;
    float stepProgress_ = 0.0f;
    uint8_t currentStep_ = 0;
};

} // namespace ui
