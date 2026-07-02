#include "keyboard.h"
#include "font.h"
#include <cstring>

namespace ui {

// ── LOWER (lowercase letters) ────────────────────────────────────────────────

static const Keyboard::KeyDef L0_R0[] = {
    {"q", 'q', Keyboard::KeyAction::NONE, 1},
    {"w", 'w', Keyboard::KeyAction::NONE, 1},
    {"e", 'e', Keyboard::KeyAction::NONE, 1},
    {"r", 'r', Keyboard::KeyAction::NONE, 1},
    {"t", 't', Keyboard::KeyAction::NONE, 1},
    {"y", 'y', Keyboard::KeyAction::NONE, 1},
    {"u", 'u', Keyboard::KeyAction::NONE, 1},
    {"i", 'i', Keyboard::KeyAction::NONE, 1},
    {"o", 'o', Keyboard::KeyAction::NONE, 1},
    {"p", 'p', Keyboard::KeyAction::NONE, 1},
};

static const Keyboard::KeyDef L0_R1[] = {
    {"a", 'a', Keyboard::KeyAction::NONE, 1},
    {"s", 's', Keyboard::KeyAction::NONE, 1},
    {"d", 'd', Keyboard::KeyAction::NONE, 1},
    {"f", 'f', Keyboard::KeyAction::NONE, 1},
    {"g", 'g', Keyboard::KeyAction::NONE, 1},
    {"h", 'h', Keyboard::KeyAction::NONE, 1},
    {"j", 'j', Keyboard::KeyAction::NONE, 1},
    {"k", 'k', Keyboard::KeyAction::NONE, 1},
    {"l", 'l', Keyboard::KeyAction::NONE, 1},
};

static const Keyboard::KeyDef L0_R2[] = {
    {"z", 'z', Keyboard::KeyAction::NONE, 1},
    {"x", 'x', Keyboard::KeyAction::NONE, 1},
    {"c", 'c', Keyboard::KeyAction::NONE, 1},
    {"v", 'v', Keyboard::KeyAction::NONE, 1},
    {"b", 'b', Keyboard::KeyAction::NONE, 1},
    {"n", 'n', Keyboard::KeyAction::NONE, 1},
    {"m", 'm', Keyboard::KeyAction::NONE, 1},
    {"Del", 0,  Keyboard::KeyAction::BACKSPACE, 1},
};

static const Keyboard::KeyDef L0_R3[] = {
    {"Sft", 0, Keyboard::KeyAction::SHIFT, 1},
    {"123", 0, Keyboard::KeyAction::SWITCH_SYMBOLS, 1},
    {"",    ' ', Keyboard::KeyAction::SPACE, 4},
    {"OK",  0,  Keyboard::KeyAction::RETURN, 1},
};

const Keyboard::RowDef Keyboard::kLowerLayout_[ROWS] = {
    {L0_R0, 10}, {L0_R1, 9}, {L0_R2, 8}, {L0_R3, 4},
};

// ── UPPER (uppercase letters) ────────────────────────────────────────────────

static const Keyboard::KeyDef U0_R0[] = {
    {"Q", 'Q', Keyboard::KeyAction::NONE, 1},
    {"W", 'W', Keyboard::KeyAction::NONE, 1},
    {"E", 'E', Keyboard::KeyAction::NONE, 1},
    {"R", 'R', Keyboard::KeyAction::NONE, 1},
    {"T", 'T', Keyboard::KeyAction::NONE, 1},
    {"Y", 'Y', Keyboard::KeyAction::NONE, 1},
    {"U", 'U', Keyboard::KeyAction::NONE, 1},
    {"I", 'I', Keyboard::KeyAction::NONE, 1},
    {"O", 'O', Keyboard::KeyAction::NONE, 1},
    {"P", 'P', Keyboard::KeyAction::NONE, 1},
};

static const Keyboard::KeyDef U0_R1[] = {
    {"A", 'A', Keyboard::KeyAction::NONE, 1},
    {"S", 'S', Keyboard::KeyAction::NONE, 1},
    {"D", 'D', Keyboard::KeyAction::NONE, 1},
    {"F", 'F', Keyboard::KeyAction::NONE, 1},
    {"G", 'G', Keyboard::KeyAction::NONE, 1},
    {"H", 'H', Keyboard::KeyAction::NONE, 1},
    {"J", 'J', Keyboard::KeyAction::NONE, 1},
    {"K", 'K', Keyboard::KeyAction::NONE, 1},
    {"L", 'L', Keyboard::KeyAction::NONE, 1},
};

static const Keyboard::KeyDef U0_R2[] = {
    {"Z", 'Z', Keyboard::KeyAction::NONE, 1},
    {"X", 'X', Keyboard::KeyAction::NONE, 1},
    {"C", 'C', Keyboard::KeyAction::NONE, 1},
    {"V", 'V', Keyboard::KeyAction::NONE, 1},
    {"B", 'B', Keyboard::KeyAction::NONE, 1},
    {"N", 'N', Keyboard::KeyAction::NONE, 1},
    {"M", 'M', Keyboard::KeyAction::NONE, 1},
    {"Del", 0,  Keyboard::KeyAction::BACKSPACE, 1},
};

static const Keyboard::KeyDef U0_R3[] = {
    {"Sft", 0, Keyboard::KeyAction::SHIFT, 1},
    {"123", 0, Keyboard::KeyAction::SWITCH_SYMBOLS, 1},
    {"",    ' ', Keyboard::KeyAction::SPACE, 4},
    {"OK",  0,  Keyboard::KeyAction::RETURN, 1},
};

const Keyboard::RowDef Keyboard::kUpperLayout_[ROWS] = {
    {U0_R0, 10}, {U0_R1, 9}, {U0_R2, 8}, {U0_R3, 4},
};

// ── SYMBOLS_1 (numbers + common punctuation) ─────────────────────────────────

static const Keyboard::KeyDef S1_R0[] = {
    {"1", '1', Keyboard::KeyAction::NONE, 1},
    {"2", '2', Keyboard::KeyAction::NONE, 1},
    {"3", '3', Keyboard::KeyAction::NONE, 1},
    {"4", '4', Keyboard::KeyAction::NONE, 1},
    {"5", '5', Keyboard::KeyAction::NONE, 1},
    {"6", '6', Keyboard::KeyAction::NONE, 1},
    {"7", '7', Keyboard::KeyAction::NONE, 1},
    {"8", '8', Keyboard::KeyAction::NONE, 1},
    {"9", '9', Keyboard::KeyAction::NONE, 1},
    {"0", '0', Keyboard::KeyAction::NONE, 1},
};

static const Keyboard::KeyDef S1_R1[] = {
    {"-", '-', Keyboard::KeyAction::NONE, 1},
    {"/", '/', Keyboard::KeyAction::NONE, 1},
    {":", ':', Keyboard::KeyAction::NONE, 1},
    {";", ';', Keyboard::KeyAction::NONE, 1},
    {"(", '(', Keyboard::KeyAction::NONE, 1},
    {")", ')', Keyboard::KeyAction::NONE, 1},
    {"$", '$', Keyboard::KeyAction::NONE, 1},
    {"&", '&', Keyboard::KeyAction::NONE, 1},
    {"\"", '"', Keyboard::KeyAction::NONE, 1},
};

static const Keyboard::KeyDef S1_R2[] = {
    {".", '.', Keyboard::KeyAction::NONE, 1},
    {",", ',', Keyboard::KeyAction::NONE, 1},
    {"?", '?', Keyboard::KeyAction::NONE, 1},
    {"!", '!', Keyboard::KeyAction::NONE, 1},
    {"'", '\'', Keyboard::KeyAction::NONE, 1},
    {"@", '@', Keyboard::KeyAction::NONE, 1},
    {"Del", 0,  Keyboard::KeyAction::BACKSPACE, 1},
};

static const Keyboard::KeyDef S1_R3[] = {
    {"ABC", 0, Keyboard::KeyAction::SWITCH_LETTERS, 1},
    {"#+=", 0, Keyboard::KeyAction::SWITCH_SYMBOLS, 1},
    {"",    ' ', Keyboard::KeyAction::SPACE, 4},
    {"OK",  0,  Keyboard::KeyAction::RETURN, 1},
};

const Keyboard::RowDef Keyboard::kSymbols1Layout_[ROWS] = {
    {S1_R0, 10}, {S1_R1, 9}, {S1_R2, 7}, {S1_R3, 4},
};

// ── SYMBOLS_2 (brackets, math, extra punctuation) ───────────────────────────

static const Keyboard::KeyDef S2_R0[] = {
    {"[", '[', Keyboard::KeyAction::NONE, 1},
    {"]", ']', Keyboard::KeyAction::NONE, 1},
    {"{", '{', Keyboard::KeyAction::NONE, 1},
    {"}", '}', Keyboard::KeyAction::NONE, 1},
    {"#", '#', Keyboard::KeyAction::NONE, 1},
    {"%", '%', Keyboard::KeyAction::NONE, 1},
    {"^", '^', Keyboard::KeyAction::NONE, 1},
    {"*", '*', Keyboard::KeyAction::NONE, 1},
    {"+", '+', Keyboard::KeyAction::NONE, 1},
    {"=", '=', Keyboard::KeyAction::NONE, 1},
};

static const Keyboard::KeyDef S2_R1[] = {
    {"_", '_', Keyboard::KeyAction::NONE, 1},
    {"|", '|', Keyboard::KeyAction::NONE, 1},
    {"\\", '\\', Keyboard::KeyAction::NONE, 1},
    {"~", '~', Keyboard::KeyAction::NONE, 1},
    {"<", '<', Keyboard::KeyAction::NONE, 1},
    {">", '>', Keyboard::KeyAction::NONE, 1},
    {"`", '`', Keyboard::KeyAction::NONE, 1},
    {"'", '\'', Keyboard::KeyAction::NONE, 1},
    {"\t", 0, Keyboard::KeyAction::NONE, 1},
};

static const Keyboard::KeyDef S2_R2[] = {
    {".", '.', Keyboard::KeyAction::NONE, 1},
    {",", ',', Keyboard::KeyAction::NONE, 1},
    {"?", '?', Keyboard::KeyAction::NONE, 1},
    {"!", '!', Keyboard::KeyAction::NONE, 1},
    {"*", '*', Keyboard::KeyAction::NONE, 1},
    {"=", '=', Keyboard::KeyAction::NONE, 1},
    {"Del", 0,  Keyboard::KeyAction::BACKSPACE, 1},
};

static const Keyboard::KeyDef S2_R3[] = {
    {"ABC", 0, Keyboard::KeyAction::SWITCH_LETTERS, 1},
    {"123", 0, Keyboard::KeyAction::SWITCH_SYMBOLS, 1},
    {"",    ' ', Keyboard::KeyAction::SPACE, 4},
    {"OK",  0,  Keyboard::KeyAction::RETURN, 1},
};

const Keyboard::RowDef Keyboard::kSymbols2Layout_[ROWS] = {
    {S2_R0, 10}, {S2_R1, 9}, {S2_R2, 7}, {S2_R3, 4},
};

// ── Layout lookup ───────────────────────────────────────────────────────────

const Keyboard::RowDef* Keyboard::getLayout(Layer layer, int& outRows) {
    outRows = ROWS;
    switch (layer) {
        case Layer::LOWER:     return kLowerLayout_;
        case Layer::UPPER:     return kUpperLayout_;
        case Layer::SYMBOLS_1: return kSymbols1Layout_;
        case Layer::SYMBOLS_2: return kSymbols2Layout_;
    }
    return kLowerLayout_;
}

// ── Constructor ─────────────────────────────────────────────────────────────

Keyboard::Keyboard(int x, int y, int w, int h,
                   uint16_t keyFg,
                   uint16_t activeKeyBg,
                   uint16_t surfaceBg)
    : keyFg_(keyFg),
      activeKeyBg_(activeKeyBg),
      surfaceBg_(surfaceBg) {
    setBounds(x, y, w, h);
    clear();
}

void Keyboard::clear() {
    text_[0] = '\0';
    cursorPos_ = 0;
}

void Keyboard::setText(const char* text) {
    std::strncpy(text_, text, MAX_TEXT - 1);
    text_[MAX_TEXT - 1] = '\0';
    int len = static_cast<int>(std::strlen(text_));
    cursorPos_ = (len < MAX_TEXT) ? len : MAX_TEXT - 1;
}

void Keyboard::activate() {
    active_ = true;
    confirmed_ = false;
    currentLayer_ = Layer::LOWER;
    pressedRow_ = pressedKey_ = -1;
}

void Keyboard::dismiss() {
    active_ = false;
    confirmed_ = true;
    pressedRow_ = pressedKey_ = -1;
}

// ── Text buffer manipulation ────────────────────────────────────────────────

void Keyboard::insertChar(char c) {
    int len = static_cast<int>(std::strlen(text_));
    if (cursorPos_ < 0) cursorPos_ = 0;
    if (cursorPos_ > len) cursorPos_ = len;
    if (len >= MAX_TEXT - 1) return;

    for (int i = len; i > cursorPos_; i--) {
        text_[i] = text_[i - 1];
    }
    text_[cursorPos_] = c;
    text_[len + 1] = '\0';
    cursorPos_++;
}

void Keyboard::onChar(char c) {
    insertChar(c);
}

void Keyboard::onAction(KeyAction action) {
    switch (action) {
        case KeyAction::BACKSPACE: {
            int len = static_cast<int>(std::strlen(text_));
            if (cursorPos_ > 0 && len > 0) {
                for (int i = cursorPos_ - 1; i < len; i++) {
                    text_[i] = text_[i + 1];
                }
                cursorPos_--;
            }
            break;
        }
        case KeyAction::SPACE:
            insertChar(' ');
            break;
        case KeyAction::RETURN:
            dismiss();
            break;
        case KeyAction::SHIFT:
            currentLayer_ = (currentLayer_ == Layer::LOWER)
                              ? Layer::UPPER : Layer::LOWER;
            break;
        case KeyAction::SWITCH_LETTERS:
            currentLayer_ = Layer::LOWER;
            break;
        case KeyAction::SWITCH_SYMBOLS:
            switch (currentLayer_) {
                case Layer::LOWER:
                case Layer::UPPER:
                    currentLayer_ = Layer::SYMBOLS_1;
                    break;
                case Layer::SYMBOLS_1:
                    currentLayer_ = Layer::SYMBOLS_2;
                    break;
                case Layer::SYMBOLS_2:
                    currentLayer_ = Layer::SYMBOLS_1;
                    break;
            }
            break;
        default:
            break;
    }
}

// ── Key geometry ────────────────────────────────────────────────────────────

void Keyboard::keyRect(const RowDef& row, int rowIndex, int keyIndex,
                       int& rx, int& ry, int& rw, int& rh) const {
    int totalUnits = 0;
    for (int i = 0; i < row.count; i++)
        totalUnits += row.keys[i].widthFactor;

    int kaY = keyAreaY();
    int kaH = keyAreaH();
    int keyH = (kaH - (ROWS - 1) * GAP) / ROWS;
    int availW = w_ - (row.count - 1) * GAP;
    int unitW = availW / totalUnits;

    int kx = x_;
    for (int i = 0; i < keyIndex; i++)
        kx += row.keys[i].widthFactor * unitW + GAP;

    rx = kx;
    ry = kaY + rowIndex * (keyH + GAP);
    rw = row.keys[keyIndex].widthFactor * unitW;
    rh = keyH;
}

bool Keyboard::hitTestKey(int px, int py, int& row, int& key) const {
    if (py >= y_ && py < y_ + HEADER_H)
        return false;

    for (int r = 0; r < ROWS; r++) {
        int layoutRows = 0;
        const RowDef* layout = getLayout(currentLayer_, layoutRows);
        if (r >= layoutRows) break;
        const RowDef& rd = layout[r];

        for (int k = 0; k < rd.count; k++) {
            int rx, ry, rw, rh;
            keyRect(rd, r, k, rx, ry, rw, rh);
            if (px >= rx && px < rx + rw && py >= ry && py < ry + rh) {
                row = r;
                key = k;
                return true;
            }
        }
    }
    return false;
}

// ── Drawing ─────────────────────────────────────────────────────────────────

static uint32_t sDrawCount = 0;

void Keyboard::paintKeyboard(Framebuffer& fb) {
    sDrawCount++;

    fb.fillRect(x_, y_, w_, h_, surfaceBg_);
    fb.drawRect(x_, y_, w_, h_, GRAY_MID);

    // ── Header: text preview ─────────────────────────────────────────────
    int textAreaX = x_ + 4;
    int textAreaW = w_ - 8;
    int maxVisible = textAreaW / FONT_STEP;
    if (maxVisible < 1) maxVisible = 1;

    int textLen = static_cast<int>(std::strlen(text_));
    int scrollOffset = 0;
    if (cursorPos_ > maxVisible - 1)
        scrollOffset = cursorPos_ - maxVisible + 1;

    int drawLen = textLen - scrollOffset;
    if (drawLen > maxVisible) drawLen = maxVisible;

    int tx = textAreaX;
    int ty = y_ + (HEADER_H - FONT_H) / 2;

    for (int i = 0; i < drawLen; i++)
        fb.drawChar(tx + i * FONT_STEP, ty,
                    text_[scrollOffset + i], keyFg_, surfaceBg_);

    if ((sDrawCount / 30) % 2 == 0 && cursorPos_ >= scrollOffset &&
        cursorPos_ <= scrollOffset + drawLen && cursorPos_ <= textLen)
        fb.fillRect(tx + (cursorPos_ - scrollOffset) * FONT_STEP, ty, 1, FONT_H, activeKeyBg_);

    // ── Keys ─────────────────────────────────────────────────────────────
    int layoutRows = 0;
    const RowDef* layout = getLayout(currentLayer_, layoutRows);
    int kaY = keyAreaY();
    int kaH = keyAreaH();
    int keyH = (kaH - (ROWS - 1) * GAP) / ROWS;

    for (int r = 0; r < layoutRows; r++) {
        const RowDef& rd = layout[r];

        int totalUnits = 0;
        for (int i = 0; i < rd.count; i++)
            totalUnits += rd.keys[i].widthFactor;

        int availW = w_ - (rd.count - 1) * GAP;
        int unitW = availW / totalUnits;
        int kx = x_;
        int ky = kaY + r * (keyH + GAP);

        for (int k = 0; k < rd.count; k++) {
            const KeyDef& kd = rd.keys[k];
            int kw = kd.widthFactor * unitW;

            bool pressed = (pressedRow_ == r && pressedKey_ == k);
            bool isSpecial = (kd.action != KeyAction::NONE &&
                              kd.action != KeyAction::SPACE &&
                              kd.character == 0);

            uint16_t bg = surfaceBg_;
            uint16_t fg = keyFg_;

            if (pressed) {
                bg = activeKeyBg_;
                fg = surfaceBg_;
            }

            if (kd.action == KeyAction::SHIFT && currentLayer_ == Layer::UPPER) {
                bg = activeKeyBg_;
                fg = surfaceBg_;
            }

            uint16_t borderColor;
            if (kd.action == KeyAction::BACKSPACE)
                borderColor = ACCENT_1;
            else if (isSpecial)
                borderColor = ACCENT_3;
            else
                borderColor = LAVENDER;

            fb.fillRect(kx + 1, ky + 1, kw - 2, keyH - 2, bg);
            fb.drawRect(kx + 1, ky + 1, kw - 2, keyH - 2, borderColor);

            int labelW = Framebuffer::textWidth(kd.label);
            int lx = kx + (kw - labelW) / 2;
            int ly = ky + (keyH - FONT_H) / 2;
            fb.drawText(lx, ly, kd.label, fg, bg);

            kx += kw + GAP;
        }
    }
}

void Keyboard::paintTrampoline(Framebuffer& fb, void* user) {
    static_cast<Keyboard*>(user)->paintKeyboard(fb);
}

void Keyboard::draw(Framebuffer& fb, uint32_t) {
    if (!active_ || !visible_) return;
    fb.enqueueOverlay(x_, y_, w_, h_,
                      &Keyboard::paintTrampoline, this,
                      reinterpret_cast<uintptr_t>(this));
}

// ── Touch ───────────────────────────────────────────────────────────────────

bool Keyboard::onTouchBegan(const TouchEvent& event) {
    if (!active_ || !visible_) return false;
    int row, key;
    if (hitTestKey(event.x, event.y, row, key)) {
        pressedRow_ = row;
        pressedKey_ = key;
        return true;
    }
    return false;
}

void Keyboard::onTouchEnded(const TouchEvent& event) {
    if (!active_) return;

    int row, key;
    if (hitTestKey(event.x, event.y, row, key) &&
        row == pressedRow_ && key == pressedKey_) {
        int layoutRows = 0;
        const RowDef* layout = getLayout(currentLayer_, layoutRows);
        if (pressedRow_ >= 0 && pressedRow_ < layoutRows &&
            pressedKey_ >= 0 && pressedKey_ < layout[pressedRow_].count) {
            const KeyDef& kd = layout[pressedRow_].keys[pressedKey_];
            if (kd.character != 0)
                onChar(kd.character);
            else if (kd.action != KeyAction::NONE)
                onAction(kd.action);
        }
    }

    pressedRow_ = pressedKey_ = -1;
}

void Keyboard::onTouchCancelled(const TouchEvent&) {
    pressedRow_ = pressedKey_ = -1;
}

} // namespace ui
