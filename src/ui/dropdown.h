#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"
#include "icons.h"

namespace ui {

enum class DropdownStyle : uint8_t {
    CLASSIC,
    OUTLINED,
    UNDERLINED,
    COMPACT,
};

class Dropdown : public Widget {
public:
    static constexpr int DEFAULT_W = 120;
    static constexpr int DEFAULT_H = 20;
    static constexpr int MAX_ITEMS = 16;

    Dropdown(int x, int y, int w = DEFAULT_W, int h = DEFAULT_H,
             DropdownStyle style = DropdownStyle::CLASSIC,
             uint16_t fg = TEXT, uint16_t bg = BG_SURFACE,
             uint16_t accent = ACCENT_2);

    void draw(Framebuffer& fb, int selectedIndex, bool expanded);

    void addItem(const char* item);
    void addItem(const char* item, SynthIcon icon);
    void clearItems();
    int getItemCount() const noexcept { return itemCount_; }

    bool isExpanded() const noexcept { return expanded_; }
    // Expanded lists float above sibling widgets (the list may open upward
    // over them), so raise the z-order while open.
    void setExpanded(bool e) noexcept { expanded_ = e; setZOrder(e ? 1 : 0); }

    int getSelectedIndex() const noexcept { return selectedIndex_; }
    bool wasSelected() noexcept { bool v = wasSelected_; wasSelected_ = false; return v; }

    void setPosition(int x, int y) noexcept { setBounds(x, y, w_, h_); }
    void setColors(uint16_t fg, uint16_t bg, uint16_t accent) noexcept {
        fg_ = fg; bg_ = bg; accent_ = accent;
    }
    void setStyle(DropdownStyle style) noexcept { style_ = style; }

    // Keep the expanded list inside [minY, maxY): when it would cross maxY
    // it opens upward instead, so it never paints over status bars or a
    // transport footer. Default (-1) keeps the legacy open-down behavior.
    void setListLimits(int minY, int maxY) noexcept {
        listMinY_ = minY; listMaxY_ = maxY;
    }

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
    int listOrigin() const noexcept;  // top Y of the expanded list
    void paintExpandedListImpl(Framebuffer& fb, int selectedIndex);
    static void paintExpandedListTrampoline(Framebuffer& fb, void* user);

    DropdownStyle style_;
    uint16_t fg_, bg_, accent_;
    const char* items_[MAX_ITEMS];
    SynthIcon icons_[MAX_ITEMS];
    int itemCount_;
    int lastSelectedIndex_;
    bool expanded_ = false;
    int selectedIndex_ = -1;
    bool wasSelected_ = false;
    int listMinY_ = -1;
    int listMaxY_ = -1;

    bool onTouchBegan(const TouchEvent& event) override;
    void onTap(const TouchEvent& event) override;
};

} // namespace ui
