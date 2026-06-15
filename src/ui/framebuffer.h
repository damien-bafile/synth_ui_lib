#pragma once
#include <cstdint>
#include "icons.h"

namespace ui {

class Framebuffer {
public:
    Framebuffer(uint8_t* data, int width, int height);

    void setPixel(int x, int y, uint16_t color);
    void fillRect(int x, int y, int w, int h, uint16_t color);
    void drawRect(int x, int y, int w, int h, uint16_t color);
    void fillScreen(uint16_t color);
    void fillCircle(int x, int y, int r, uint16_t color);
    void drawCircle(int x, int y, int r, uint16_t color);

    void drawChar(int x, int y, char c, uint16_t fg, uint16_t bg);
    void drawText(int x, int y, const char* text, uint16_t fg, uint16_t bg);
    static int textWidth(const char* text);

    void drawSynthIcon(int x, int y, SynthIcon icon, uint16_t fg, uint16_t bg);
    void drawFxIcon(int x, int y, FxIcon icon, uint16_t fg, uint16_t bg);

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

private:
    static inline void packRgb565(uint8_t* dst, uint16_t color);

    uint8_t* data_;
    int width_;
    int height_;
};

} // namespace ui
