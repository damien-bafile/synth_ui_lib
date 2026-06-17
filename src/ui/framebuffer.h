#pragma once
#include <cstdint>
#include "icons.h"

namespace ui {

class Framebuffer {
public:
    // Maximum number of deferred overlay rectangles. Sized for 2 expanded
    // dropdowns + headroom (tooltips, future modals). If you exceed this the
    // oldest entry is replaced (FIFO eviction).
    static constexpr int MAX_OVERLAYS = 8;

    // Painter callback used by the overlay queue. Receives the Framebuffer
    // and an opaque user pointer. The callback is expected to redraw into
    // the rect that was passed to enqueueOverlay().
    using OverlayPainter = void (*)(Framebuffer& fb, void* user);

    Framebuffer(uint8_t* data, int width, int height);

    void setPixel(int x, int y, uint16_t color);
    void fillRect(int x, int y, int w, int h, uint16_t color);
    void drawRect(int x, int y, int w, int h, uint16_t color);
    void drawLine(int x0, int y0, int x1, int y1, uint16_t color);
    void fillScreen(uint16_t color);
    void fillCircle(int x, int y, int r, uint16_t color);
    void drawCircle(int x, int y, int r, uint16_t color);

    void drawChar(int x, int y, char c, uint16_t fg, uint16_t bg);
    void drawText(int x, int y, const char* text, uint16_t fg, uint16_t bg);
    static int textWidth(const char* text);

    void drawSynthIcon(int x, int y, SynthIcon icon, uint16_t fg, uint16_t bg);
    void drawFxIcon(int x, int y, FxIcon icon, uint16_t fg, uint16_t bg);

    // Deferred-overlay API. Screens should call clearOverlays() at the start
    // of their render pass and paintOverlays() at the end so widgets that
    // need to sit on top of panels (dropdown lists, popups) are not clipped
    // by later draw calls.
    //
    // enqueueOverlay paints nothing immediately. The painter is invoked from
    // paintOverlays() in push order (oldest first). If a (tag, rect) is
    // enqueued multiple times before paintOverlays() runs, the latest entry
    // wins — so re-rendering a dropdown every frame doesn't grow the queue.
    //
    // tag is opaque; pass 0 for "no tag". closeOverlay(tag) marks a tagged
    // entry as not-live so paintOverlays() skips it (for self-dismissing
    // popups). closeOverlay() with tag=0 is a no-op.
    void enqueueOverlay(int x, int y, int w, int h,
                        OverlayPainter paint, void* user, uint32_t tag = 0);
    void closeOverlay(uint32_t tag);
    void clearOverlays();
    void paintOverlays();

    int getOverlayCount() const { return overlayCount_; }

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

private:
    static inline void packRgb565(uint8_t* dst, uint16_t color);

    uint8_t* data_;
    int width_;
    int height_;

    struct OverlayCmd {
        int x;
        int y;
        int w;
        int h;
        OverlayPainter paint;
        void* user;
        uint32_t tag;
    };

    // Declared after the POD data members so the constructor initializer
    // list can keep the original data_/width_/height_ order without
    // -Wreorder complaining.
    OverlayCmd overlays_[MAX_OVERLAYS];
    int overlayCount_;
};

} // namespace ui
