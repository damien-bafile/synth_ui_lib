#include "framebuffer.h"
#include "font.h"
#include <cstring>

namespace ui {

Framebuffer::Framebuffer(uint8_t* data, int width, int height)
    : data_(data), width_(width), height_(height) {}

inline void Framebuffer::packRgb565(uint8_t* dst, uint16_t color) {
    dst[0] = color & 0xFF;
    dst[1] = (color >> 8) & 0xFF;
}

void Framebuffer::setPixel(int x, int y, uint16_t color) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) return;
    packRgb565(data_ + (y * width_ + x) * 2, color);
}

void Framebuffer::fillRect(int x, int y, int w, int h, uint16_t color) {
    int x0 = x < 0 ? 0 : x;
    int y0 = y < 0 ? 0 : y;
    int x1 = x + w > width_ ? width_ : x + w;
    int y1 = y + h > height_ ? height_ : y + h;

    uint8_t lo = color & 0xFF;
    uint8_t hi = (color >> 8) & 0xFF;

    for (int cy = y0; cy < y1; cy++) {
        uint8_t* line = data_ + (cy * width_ + x0) * 2;
        for (int col = x0; col < x1; col++) {
            *line++ = lo;
            *line++ = hi;
        }
    }
}

void Framebuffer::drawRect(int x, int y, int w, int h, uint16_t color) {
    fillRect(x, y, w, 1, color);
    fillRect(x, y + h - 1, w, 1, color);
    fillRect(x, y, 1, h, color);
    fillRect(x + w - 1, y, 1, h, color);
}

void Framebuffer::fillScreen(uint16_t color) {
    fillRect(0, 0, width_, height_, color);
}

void Framebuffer::drawCircle(int xc, int yc, int r, uint16_t color) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;
    while (y >= x) {
        setPixel(xc + x, yc + y, color);
        setPixel(xc - x, yc + y, color);
        setPixel(xc + x, yc - y, color);
        setPixel(xc - x, yc - y, color);
        setPixel(xc + y, yc + x, color);
        setPixel(xc - y, yc + x, color);
        setPixel(xc + y, yc - x, color);
        setPixel(xc - y, yc - x, color);
        x++;
        if (d > 0) { y--; d = d + 4 * (x - y) + 10; }
        else       {       d = d + 4 * x + 6; }
    }
}

void Framebuffer::fillCircle(int xc, int yc, int r, uint16_t color) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;
    while (y >= x) {
        for (int i = xc - x; i <= xc + x; i++) {
            setPixel(i, yc + y, color);
            setPixel(i, yc - y, color);
        }
        for (int i = xc - y; i <= xc + y; i++) {
            setPixel(i, yc + x, color);
            setPixel(i, yc - x, color);
        }
        x++;
        if (d > 0) { y--; d = d + 4 * (x - y) + 10; }
        else       {       d = d + 4 * x + 6; }
    }
}

void Framebuffer::drawChar(int x, int y, char c, uint16_t fg, uint16_t bg) {
    if (c < 32 || c > 127) c = '?';
    int glyph = c - 32;

    for (int col = 0; col < 5; col++) {
        unsigned char bits = font_5x7[glyph * 5 + col];
        for (int row = 0; row < 7; row++) {
            setPixel(x + col, y + row, (bits >> row) & 1 ? fg : bg);
        }
    }
}

void Framebuffer::drawText(int x, int y, const char* text, uint16_t fg, uint16_t bg) {
    for (int i = 0; text[i]; i++) {
        drawChar(x + i * 6, y, text[i], fg, bg);
    }
}

int Framebuffer::textWidth(const char* text) {
    int len = 0;
    while (text[len]) len++;
    return len * 6;
}

void Framebuffer::drawSynthIcon(int x, int y, SynthIcon icon, uint16_t fg, uint16_t bg) {
    int idx = static_cast<int>(icon);
    if (idx >= 19) return;
    for (int col = 0; col < 5; col++) {
        unsigned char bits = synth_glyphs[idx * 5 + col];
        for (int row = 0; row < 7; row++) {
            setPixel(x + col, y + row, (bits >> row) & 1 ? fg : bg);
        }
    }
}

void Framebuffer::drawFxIcon(int x, int y, FxIcon icon, uint16_t fg, uint16_t bg) {
    int idx = static_cast<int>(icon);
    if (idx >= 14) return;
    for (int col = 0; col < 5; col++) {
        unsigned char bits = fx_glyphs[idx * 5 + col];
        for (int row = 0; row < 7; row++) {
            setPixel(x + col, y + row, (bits >> row) & 1 ? fg : bg);
        }
    }
}

} // namespace ui
