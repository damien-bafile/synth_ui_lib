#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"

namespace ui {

class Keyboard : public Widget {
public:
    static constexpr int MAX_TEXT = 64;
    static constexpr int HEADER_H = 28;
    static constexpr int ROWS = 4;
    static constexpr int GAP = 2;

    enum class Layer : uint8_t {
        LOWER, UPPER, SYMBOLS_1, SYMBOLS_2
    };

    enum class KeyAction : uint8_t {
        NONE,
        SHIFT,
        BACKSPACE,
        SPACE,
        RETURN,
        SWITCH_SYMBOLS,
        SWITCH_LETTERS
    };

    struct KeyDef {
        const char* label;
        char character;
        KeyAction action;
        uint8_t widthFactor;
    };

    struct RowDef {
        const KeyDef* keys;
        uint8_t count;
    };

    Keyboard(int x, int y, int w, int h,
             uint16_t keyFg,
             uint16_t activeKeyBg,
             uint16_t surfaceBg = BG_DARK);

    void draw(Framebuffer& fb, uint32_t ticks);

    void setText(const char* text);
    const char* getText() const { return text_; }
    int cursorPos() const { return cursorPos_; }
    void clear();

    bool isActive() const { return active_; }
    void activate();
    void dismiss();
    bool confirmed() const { return confirmed_; }

    bool onTouchBegan(const TouchEvent&) override;
    void onTouchEnded(const TouchEvent&) override;
    void onTouchCancelled(const TouchEvent&) override;

    Layer currentLayer() const { return currentLayer_; }
    void setCurrentLayer(Layer l) { currentLayer_ = l; }

    static const RowDef* getLayout(Layer layer, int& outRows);

private:
    static const KeyDef kLowerRow0_[];
    static const KeyDef kLowerRow1_[];
    static const KeyDef kLowerRow2_[];
    static const KeyDef kLowerRow3_[];
    static const RowDef kLowerLayout_[ROWS];

    static const KeyDef kUpperRow0_[];
    static const KeyDef kUpperRow1_[];
    static const KeyDef kUpperRow2_[];
    static const KeyDef kUpperRow3_[];
    static const RowDef kUpperLayout_[ROWS];

    static const KeyDef kSymbols1Row0_[];
    static const KeyDef kSymbols1Row1_[];
    static const KeyDef kSymbols1Row2_[];
    static const KeyDef kSymbols1Row3_[];
    static const RowDef kSymbols1Layout_[ROWS];

    static const KeyDef kSymbols2Row0_[];
    static const KeyDef kSymbols2Row1_[];
    static const KeyDef kSymbols2Row2_[];
    static const KeyDef kSymbols2Row3_[];
    static const RowDef kSymbols2Layout_[ROWS];

    static void paintTrampoline(Framebuffer& fb, void* user);
    void paintKeyboard(Framebuffer& fb);

    void keyRect(const RowDef& row, int rowIndex, int keyIndex,
                 int& rx, int& ry, int& rw, int& rh) const;
    int keyAreaY() const { return y_ + HEADER_H; }
    int keyAreaH() const { return h_ - HEADER_H; }
    bool hitTestKey(int px, int py, int& row, int& key) const;

    void onChar(char c);
    void onAction(KeyAction action);
    void insertChar(char c);

    char text_[MAX_TEXT];
    int cursorPos_ = 0;
    bool active_ = false;
    bool confirmed_ = false;
    Layer currentLayer_ = Layer::LOWER;

    int pressedRow_ = -1;
    int pressedKey_ = -1;

    uint16_t keyFg_, activeKeyBg_, surfaceBg_;
};

} // namespace ui
