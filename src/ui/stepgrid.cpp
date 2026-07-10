#include "stepgrid.h"
#include "font.h"

namespace ui {

StepGrid::StepGrid(int x, int y, int w, int h,
                   int steps, int tracks, StepGridStyle style)
    : steps_(steps), tracks_(tracks), activeTrack_(-1), style_(style),
      accent_(ACCENT_2), bg_(BG_DARK) {
    setBounds(x, y, w, h);
    uint16_t defaults[MAX_TRACKS] = {
        TRACK_0, TRACK_1, TRACK_2, TRACK_3,
        TRACK_4, TRACK_5, TRACK_6, TRACK_7
    };
    for (int i = 0; i < MAX_TRACKS; i++) {
        trackColors_[i] = (i < tracks) ? defaults[i] : TEXT_DIM;
    }
    if (steps_ > MAX_STEPS) steps_ = MAX_STEPS;
    if (tracks_ > MAX_TRACKS) tracks_ = MAX_TRACKS;
    if (steps_ < 1) steps_ = 1;
    if (tracks_ < 1) tracks_ = 1;
}

void StepGrid::setColors(const uint16_t trackColors[MAX_TRACKS],
                         uint16_t accent, uint16_t bg) noexcept {
    for (int i = 0; i < MAX_TRACKS; i++) {
        trackColors_[i] = trackColors[i];
    }
    accent_ = accent;
    bg_ = bg;
}

void StepGrid::setActiveTrack(int track) noexcept {
    activeTrack_ = (track >= 0 && track < tracks_) ? track : -1;
}

int StepGrid::cellW() const noexcept {
    int beats = (steps_ + 3) / 4;
    if (beats < 1) beats = 1;
    int beatGap = 2;
    int beatW = (w_ - beatGap * (beats - 1)) / beats;
    return beatW / 4;
}

int StepGrid::cellH() const noexcept {
    return h_ / tracks_;
}

int StepGrid::cellX(int step) const noexcept {
    int beat = step / 4;
    int inBeat = step % 4;
    int beatGap = 2;
    int beatW = (w_ - beatGap * ((steps_ + 3) / 4 - 1)) / ((steps_ + 3) / 4);
    return x_ + beat * (beatW + beatGap) + inBeat * cellW();
}

int StepGrid::cellY(int track) const noexcept {
    return y_ + track * cellH();
}

bool StepGrid::mapTouch(int tx, int ty, int& outTrack, int& outStep) const {
    outTrack = -1;
    outStep = -1;

    if (tx < x_ || tx >= x_ + w_) return false;
    if (ty < y_ || ty >= y_ + h_) return false;

    int ch = cellH();
    if (ch <= 0) return false;
    int track = (ty - y_) / ch;
    if (track < 0 || track >= tracks_) return false;

    int beats = (steps_ + 3) / 4;
    if (beats < 1) beats = 1;
    int beatGap = 2;
    int beatW = (w_ - beatGap * (beats - 1)) / beats;
    int cw = beatW / 4;
    if (cw <= 0) return false;

    int relX = tx - x_;
    int beat = relX / (beatW + beatGap);
    if (beat < 0) beat = 0;
    if (beat >= beats) beat = beats - 1;
    int inBeatX = relX - beat * (beatW + beatGap);
    if (inBeatX > beatW) inBeatX = beatW;
    int inBeat = inBeatX / cw;
    if (inBeat < 0) inBeat = 0;
    if (inBeat > 3) inBeat = 3;

    int step = beat * 4 + inBeat;
    if (step >= steps_) step = steps_ - 1;
    if (step < 0) step = 0;

    outTrack = track;
    outStep = step;
    return true;
}

bool StepGrid::onTouchBegan(const TouchEvent& event) {
    return mapTouch(event.x, event.y, touchedTrack_, touchedStep_);
}

bool StepGrid::onTouchMoved(const TouchEvent& event) {
    mapTouch(event.x, event.y, touchedTrack_, touchedStep_);
    return true;
}

void StepGrid::onTouchEnded(const TouchEvent&) {
    touchedTrack_ = -1;
    touchedStep_ = -1;
}

void StepGrid::drawSquares(Framebuffer& fb,
                           const bool pattern[MAX_TRACKS][MAX_STEPS],
                           int playStep,
                           const char* const notes[MAX_TRACKS][MAX_STEPS]) {
    int cw = cellW();
    int ch = cellH();
    int pad = 2;

    for (int t = 0; t < tracks_; t++) {
        for (int s = 0; s < steps_; s++) {
            int cx = cellX(s);
            int cy = cellY(t);
            uint16_t trackColor = trackColors_[t];
            if (pattern[t][s]) {
                fb.fillRect(cx + pad, cy + pad, cw - pad * 2, ch - pad * 2, trackColor);
            } else {
                fb.drawRect(cx + pad, cy + pad, cw - pad * 2, ch - pad * 2, trackColor);
            }
            drawCellNote(fb, cx, cy, cw, ch, notes ? notes[t][s] : nullptr,
                         pattern[t][s] ? TEXT : TEXT_DIM,
                         pattern[t][s] ? trackColor : bg_);
        }
    }

    if (playStep >= 0 && playStep < steps_) {
        for (int t = 0; t < tracks_; t++) {
            int cx = cellX(playStep);
            int cy = cellY(t);
            fb.drawRect(cx, cy, cw, ch, accent_);
        }
    }
}

void StepGrid::drawBars(Framebuffer& fb,
                        const bool pattern[MAX_TRACKS][MAX_STEPS], int playStep,
                        const uint8_t velocities[MAX_TRACKS][MAX_STEPS],
                        const char* const notes[MAX_TRACKS][MAX_STEPS]) {
    int cw = cellW();
    int ch = cellH();
    int pad = 1;

    for (int t = 0; t < tracks_; t++) {
        for (int s = 0; s < steps_; s++) {
            int cx = cellX(s);
            int cy = cellY(t);
            uint16_t trackColor = trackColors_[t];

            if (pattern[t][s]) {
                uint8_t vel = (velocities) ? velocities[t][s] : 100;
                if (vel > 127) vel = 127;
                int barH = (ch - pad * 2) * vel / 127;
                if (barH < 2) barH = 2;
                int barY = cy + ch - pad - barH;
                fb.fillRect(cx + pad, barY, cw - pad * 2, barH, trackColor);
            } else {
                int dotY = cy + ch - pad - 2;
                int dotX = cx + cw / 2;
                fb.fillRect(dotX - 1, dotY, 2, 2, trackColor);
            }
            drawCellNote(fb, cx, cy, cw, ch, notes ? notes[t][s] : nullptr,
                         pattern[t][s] ? TEXT : TEXT_DIM,
                         pattern[t][s] ? trackColor : bg_);
        }
    }

    if (playStep >= 0 && playStep < steps_) {
        int cx = cellX(playStep);
        int lineY = y_ + h_ - 1;
        fb.drawLine(cx + 1, lineY, cx + cw - 1, lineY, accent_);
        fb.drawLine(cx + 1, lineY - 1, cx + cw - 1, lineY - 1, accent_);
    }
}

void StepGrid::drawDots(Framebuffer& fb,
                        const bool pattern[MAX_TRACKS][MAX_STEPS],
                        int playStep,
                        const char* const notes[MAX_TRACKS][MAX_STEPS]) {
    int cw = cellW();
    int ch = cellH();
    int r = (cw < ch ? cw : ch) / 3;
    if (r < 2) r = 2;
    int dotR = 1;

    for (int t = 0; t < tracks_; t++) {
        for (int s = 0; s < steps_; s++) {
            int cx = cellX(s) + cw / 2;
            int cy = cellY(t) + ch / 2;
            uint16_t trackColor = trackColors_[t];

            if (pattern[t][s]) {
                int thisR = (s == playStep) ? r + 1 : r;
                fb.fillCircle(cx, cy, thisR, trackColor);
            } else {
                fb.fillCircle(cx, cy, dotR, trackColor);
            }
            drawCellNote(fb, cellX(s), cellY(t), cw, ch,
                         notes ? notes[t][s] : nullptr,
                         pattern[t][s] ? TEXT : TEXT_DIM,
                         pattern[t][s] ? trackColor : bg_);
        }
    }
}

void StepGrid::drawStrips(Framebuffer& fb,
                          const bool pattern[MAX_TRACKS][MAX_STEPS],
                          int playStep,
                          const char* const notes[MAX_TRACKS][MAX_STEPS]) {
    int cw = cellW();
    int stripH = h_ / tracks_;

    int beats = (steps_ + 3) / 4;
    int beatGap = 2;
    int beatW = (w_ - beatGap * (beats - 1)) / beats;
    for (int b = 1; b < beats; b++) {
        int divX = x_ + b * (beatW + beatGap) - beatGap / 2;
        fb.drawLine(divX, y_, divX, y_ + h_, GRAY_DARK);
    }

    for (int t = 0; t < tracks_; t++) {
        int trackY = y_ + t * stripH;
        for (int s = 0; s < steps_; s++) {
            int cx = cellX(s);
            uint16_t trackColor = trackColors_[t];

            if (pattern[t][s]) {
                int cellPad = 1;
                fb.fillRect(cx + cellPad, trackY + cellPad,
                            cw - cellPad * 2, stripH - cellPad * 2, trackColor);
            } else {
                int dashW = (cw > 4) ? 3 : cw - 2;
                int dashH = 2;
                int dashX = cx + (cw - dashW) / 2;
                int dashY = trackY + (stripH - dashH) / 2;
                fb.fillRect(dashX, dashY, dashW, dashH, trackColor);
            }
            drawCellNote(fb, cx, trackY, cw, stripH,
                         notes ? notes[t][s] : nullptr,
                         pattern[t][s] ? TEXT : TEXT_DIM,
                         pattern[t][s] ? trackColor : bg_);
        }
    }

    if (playStep >= 0 && playStep < steps_) {
        int cx = cellX(playStep) + cw / 2;
        fb.drawLine(cx, y_, cx, y_ + h_, accent_);
        fb.drawLine(cx - 1, y_, cx - 1, y_ + h_, accent_);
    }
}

void StepGrid::drawGlow(Framebuffer& fb,
                        const bool pattern[MAX_TRACKS][MAX_STEPS],
                        int playStep,
                        const char* const notes[MAX_TRACKS][MAX_STEPS]) {
    int cw = cellW();
    int ch = cellH();

    for (int t = 0; t < tracks_; t++) {
        for (int s = 0; s < steps_; s++) {
            int cx = cellX(s);
            int cy = cellY(t);
            uint16_t trackColor = trackColors_[t];

            int subW = cw / 3;
            if (subW < 2) subW = 2;
            int subH = 2;
            int subX = cx + (cw - subW) / 2;
            int subY = cy + (ch - subH) / 2;
            fb.fillRect(subX, subY, subW, subH, GRAY_DARK);

            if (pattern[t][s]) {
                fb.fillRect(cx, cy, cw, ch, trackColor);
            }
            drawCellNote(fb, cx, cy, cw, ch, notes ? notes[t][s] : nullptr,
                         pattern[t][s] ? TEXT : TEXT_DIM,
                         pattern[t][s] ? trackColor : bg_);
        }
    }

    if (playStep >= 0 && playStep < steps_) {
        int cx = cellX(playStep) + cw / 2;
        fb.drawLine(cx - 1, y_, cx - 1, y_ + h_, GRAY_MID);
        fb.drawLine(cx, y_, cx, y_ + h_, accent_);
        fb.drawLine(cx + 1, y_, cx + 1, y_ + h_, GRAY_MID);
    }
}

void StepGrid::draw(Framebuffer& fb,
                    const bool pattern[MAX_TRACKS][MAX_STEPS],
                    int playStep,
                    const uint8_t velocities[MAX_TRACKS][MAX_STEPS],
                    const char* const notes[MAX_TRACKS][MAX_STEPS]) {
    fb.fillRect(x_, y_, w_, h_, bg_);

    // Smooth playhead step so it glides between columns instead of jumping.
    if (playStep >= 0 && playStep < steps_) {
        if (smoothPlayStep_ < 0.0f) smoothPlayStep_ = (float)playStep;
        else smoothPlayStep_ += ((float)playStep - smoothPlayStep_) * 0.2f;
        int animStep = (int)(smoothPlayStep_ + 0.5f);
        if (animStep < 0) animStep = 0;
        if (animStep >= steps_) animStep = steps_ - 1;
        playStep = animStep;
    } else {
        smoothPlayStep_ = -1.0f;
    }

    switch (style_) {
        case StepGridStyle::SQUARES:
            drawSquares(fb, pattern, playStep, notes);
            break;
        case StepGridStyle::BARS:
            drawBars(fb, pattern, playStep, velocities, notes);
            break;
        case StepGridStyle::DOTS:
            drawDots(fb, pattern, playStep, notes);
            break;
        case StepGridStyle::STRIPS:
            drawStrips(fb, pattern, playStep, notes);
            break;
        case StepGridStyle::GLOW:
            drawGlow(fb, pattern, playStep, notes);
            break;
    }

    if (activeTrack_ >= 0) {
        int ay = cellY(activeTrack_);
        int ah = cellH();
        fb.drawRect(x_, ay, w_, ah, ACCENT_3);
    }
}

void StepGrid::drawCellNote(Framebuffer& fb, int cx, int cy, int cw, int ch,
                            const char* text, uint16_t fg, uint16_t bg) const {
    if (!text || !text[0]) return;
    int minW = FONT_STEP * 2 + 2;
    if (cw < minW) return;
    int tw = fb.textWidth(text);
    if (tw > cw - 2) tw = cw - 2;
    int tx = cx + (cw - tw) / 2;
    int ty = cy + (ch - FONT_H) / 2;
    fb.drawText(tx, ty, text, fg, bg);
}

} // namespace ui
