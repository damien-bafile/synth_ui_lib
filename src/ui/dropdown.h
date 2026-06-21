#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "touch.h"
#include "colors.h"
#include "icons.h"

namespace ui {

enum class DropdownStyle : uint8_t {
    CLASSIC,    // Filled bg, border, chevron
    OUTLINED,   // Border only, dark bg
    UNDERLINED, // Text + underline + chevron, no box
    COMPACT,    // Smaller height, tight padding
};

class Dropdown {
public:
    static constexpr int DEFAULT_W = 120;
    static constexpr int DEFAULT_H = 20;
    static constexpr int MAX_ITEMS = 16;

    Dropdown(int x, int y, int w = DEFAULT_W, int h = DEFAULT_H,
             DropdownStyle style = DropdownStyle::CLASSIC,
             uint16_t fg = TEXT, uint16_t bg = BG_SURFACE,
             uint16_t accent = ACCENT_2);

    void draw(Framebuffer& fb, int selectedIndex, bool expanded = false);
    bool handleTouch(const TouchState& touch, int& outIndex, bool expanded = false);

    void addItem(const char* item);
    void addItem(const char* item, SynthIcon icon);
    void clearItems();
    int getItemCount() const noexcept { return itemCount_; }

    void setPosition(int x, int y) noexcept { x_ = x; y_ = y; }
    void setColors(uint16_t fg, uint16_t bg, uint16_t accent) noexcept {
        fg_ = fg; bg_ = bg; accent_ = accent;
    }
    void setStyle(DropdownStyle style) noexcept { style_ = style; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }

private:
    void drawClassic(Framebuffer& fb, int selectedIndex, bool expanded);
    void drawOutlined(Framebuffer& fb, int selectedIndex, bool expanded);
    void drawUnderlined(Framebuffer& fb, int selectedIndex, bool expanded);
    void drawCompact(Framebuffer& fb, int selectedIndex, bool expanded);
    void drawChevron(Framebuffer& fb, int cx, int cy, bool down, uint16_t color);
    int getItemHeight() const noexcept;

    // Paints the expanded item list only (no closed-box header). Used by
    // draw() when expanded=true so the list is deferred to the overlay
    // pass and is not clipped by panels drawn later in the same frame.
    // Also exposed as a static OverlayPainter-compatible trampoline so the
    // Framebuffer overlay queue can invoke it without per-instance thunks.
    void paintExpandedListImpl(Framebuffer& fb, int selectedIndex);
    static void paintExpandedListTrampoline(Framebuffer& fb, void* user);

    int x_, y_, w_, h_;
    DropdownStyle style_;
    uint16_t fg_, bg_, accent_;
    const char* items_[MAX_ITEMS];
    SynthIcon icons_[MAX_ITEMS];
    int itemCount_;

    // Cached for the overlay pass; set by draw() each frame. Declared after
    // itemCount_ to match the constructor initializer order (silences
    // -Wreorder).
    int lastSelectedIndex_;
};

} // namespace ui
