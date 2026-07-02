#include "framebuffer.h"
#include "font.h"
#include <cstring>

namespace ui {

Framebuffer::Framebuffer(uint8_t* data, int width, int height)
    : data_(data), width_(width), height_(height), overlayCount_(0) {
    for (int i = 0; i < MAX_OVERLAYS; i++) {
        overlays_[i].paint = nullptr;
        overlays_[i].user = nullptr;
        overlays_[i].tag = 0;
        overlays_[i].x = 0;
        overlays_[i].y = 0;
        overlays_[i].w = 0;
        overlays_[i].h = 0;
    }
}

void Framebuffer::clearOverlays() {
    overlayCount_ = 0;
}

void Framebuffer::enqueueOverlay(int x, int y, int w, int h,
                                 OverlayPainter paint, void* user, uint32_t tag) {
    if (!paint || w <= 0 || h <= 0) return;

    // Coalesce: if a live entry with the same tag is already queued, replace
    // it in place. This keeps the queue bounded when widgets re-paint every
    // frame. tag == 0 is treated as "untagged" and always appended.
    if (tag != 0) {
        for (int i = 0; i < overlayCount_; i++) {
            if (overlays_[i].tag == tag) {
                overlays_[i].x = x;
                overlays_[i].y = y;
                overlays_[i].w = w;
                overlays_[i].h = h;
                overlays_[i].paint = paint;
                overlays_[i].user = user;
                return;
            }
        }
    }

    int slot = overlayCount_;
    if (slot >= MAX_OVERLAYS) {
        // FIFO eviction: drop the oldest entry to make room. Drop-in
        // replacement (a re-pushed dropdown landing at the end) is fine
        // because every live dropdown re-enqueues every frame.
        for (int i = 1; i < MAX_OVERLAYS; i++) {
            overlays_[i - 1] = overlays_[i];
        }
        slot = MAX_OVERLAYS - 1;
    }
    overlays_[slot].x = x;
    overlays_[slot].y = y;
    overlays_[slot].w = w;
    overlays_[slot].h = h;
    overlays_[slot].paint = paint;
    overlays_[slot].user = user;
    overlays_[slot].tag = tag;
    overlayCount_ = slot + 1;
}

void Framebuffer::closeOverlay(uint32_t tag) {
    if (tag == 0) return;
    for (int i = 0; i < overlayCount_; i++) {
        if (overlays_[i].tag == tag) {
            // Shift remaining entries down to keep the queue compact.
            for (int j = i; j < overlayCount_ - 1; j++) {
                overlays_[j] = overlays_[j + 1];
            }
            overlayCount_--;
            return;
        }
    }
}

void Framebuffer::paintOverlays() {
    OverlayCmd snapshot[MAX_OVERLAYS];
    int n = overlayCount_;
    for (int i = 0; i < n; i++) snapshot[i] = overlays_[i];
    for (int i = 0; i < n; i++) {
        if (snapshot[i].paint)
            snapshot[i].paint(*this, snapshot[i].user);
    }
}

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
    if (w < 1 || h < 1) return;
    fillRect(x, y, w, 1, color);
    fillRect(x, y + h - 1, w, 1, color);
    fillRect(x, y, 1, h, color);
    fillRect(x + w - 1, y, 1, h, color);
}

void Framebuffer::drawLine(int x0, int y0, int x1, int y1, uint16_t color) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int sx = (dx >= 0) ? 1 : -1;
    int sy = (dy >= 0) ? 1 : -1;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;

    int err = dx - dy;

    for (;;) {
        setPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 <  dx) { err += dx; y0 += sy; }
    }
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
    unsigned char uc = static_cast<unsigned char>(c);
    if (uc < 32 || uc > 127) c = '?';
    int glyph = static_cast<unsigned char>(c) - 32;

    for (int col = 0; col < 5; col++) {
        unsigned char bits = font_5x7[glyph * 5 + col];
        for (int row = 0; row < 7; row++) {
            setPixel(x + col, y + row, (bits >> row) & 1 ? fg : bg);
        }
    }
}

void Framebuffer::drawText(int x, int y, const char* text, uint16_t fg, uint16_t bg) {
    for (int i = 0; text[i]; i++) {
        drawChar(x + i * FONT_STEP, y, text[i], fg, bg);
    }
}

int Framebuffer::textWidth(const char* text) {
    return std::strlen(text) * FONT_STEP;
}

void Framebuffer::drawSynthIcon(int x, int y, SynthIcon icon, uint16_t fg, uint16_t bg) {
    int idx = static_cast<int>(icon);
    if (idx >= 21) return;
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
