#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"
#include "font.h"

namespace ui {

// Floating text bubble, using the overlay headroom framebuffer.h already
// reserves for it. Not touch-interactive (no hover concept in a touch-only
// library) -- a screen calls show() from another widget's onLongPress(),
// then draw() every frame while visible. Follows PopupMenu's
// enqueueOverlay()/paintTrampoline() pattern: draw() only re-enqueues
// while visible, so a dismissed tooltip simply stops appearing next frame
// (the overlay queue is rebuilt every frame via clearOverlays()).
class Tooltip {
public:
    static constexpr int PADDING = 3;

    Tooltip(uint16_t fg = TEXT, uint16_t bg = BG_SURFACE,
            uint16_t border = ACCENT_2) noexcept
        : fg_(fg), bg_(bg), border_(border) {}

    void show(int x, int y, const char* text) noexcept {
        x_ = x; y_ = y; text_ = text; visible_ = true;
    }
    void dismiss() noexcept { visible_ = false; }
    bool isVisible() const noexcept { return visible_; }
    const char* getText() const noexcept { return text_; }
    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }

    int getWidth() const noexcept {
        return (text_ ? Framebuffer::textWidth(text_) : 0) + PADDING * 2;
    }
    int getHeight() const noexcept { return FONT_H + PADDING * 2; }

    void setColors(uint16_t fg, uint16_t bg, uint16_t border) noexcept {
        fg_ = fg; bg_ = bg; border_ = border;
    }

    // Enqueues the tooltip's paint for this frame; no-op while not visible.
    void draw(Framebuffer& fb);

private:
    static void paintTrampoline(Framebuffer& fb, void* user);
    void paint(Framebuffer& fb);

    bool visible_ = false;
    int x_ = 0, y_ = 0;
    const char* text_ = nullptr;
    uint16_t fg_, bg_, border_;
};

} // namespace ui
