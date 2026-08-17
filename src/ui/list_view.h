#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"
#include "icons.h"

namespace ui {

// Scrollable item list / patch browser: generalizes Dropdown's fixed
// 16-item cap (no popup/overlay -- renders inline in its own bounds, with
// a vertical-drag scroll offset). Reuses Dropdown's row-rendering look
// (fill + border + optional icon + label) rather than reinventing it.
//
// Optional reorder mode: long-press a row, then drag to swap it up/down
// past a half-row-height threshold (same "accumulate, then act every N
// pixels" shape as StepGrid::takeDragTicks). getOrder() exposes the
// resulting display order as indices into the original items array.
class ListView : public Widget {
public:
    static constexpr int ROW_H     = 20;
    static constexpr int MAX_ITEMS = 64;

    ListView(int x, int y, int w, int h,
             const char* const* items, int count,
             uint16_t fg = TEXT, uint16_t bg = BG_SURFACE,
             uint16_t accent = ACCENT_2) noexcept;

    void draw(Framebuffer& fb) const;

    void setItems(const char* const* items, int count) noexcept;
    void setIcons(const SynthIcon* icons) noexcept { icons_ = icons; }
    int getItemCount() const noexcept { return count_; }

    // Index into the *original* items array (not display row).
    int getSelectedIndex() const noexcept { return selectedIndex_; }
    bool wasSelected() noexcept { bool v = wasSelected_; wasSelected_ = false; return v; }

    float getScrollOffset() const noexcept { return scrollOffset_; }
    void setScrollOffset(float px) noexcept;

    void setReorderable(bool on) noexcept { reorderable_ = on; }
    bool isReorderable() const noexcept { return reorderable_; }
    bool wasReordered() noexcept { bool v = wasReordered_; wasReordered_ = false; return v; }
    // order[displayRow] = index into the original items array.
    const int* getOrder() const noexcept { return order_; }

    void setColors(uint16_t fg, uint16_t bg, uint16_t accent) noexcept {
        fg_ = fg; bg_ = bg; accent_ = accent;
    }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }

private:
    float maxScrollOffset() const noexcept;
    int rowAt(int py) const noexcept;  // display row, or -1

    const char* const* items_;
    int count_;
    const SynthIcon* icons_ = nullptr;
    int order_[MAX_ITEMS];

    uint16_t fg_, bg_, accent_;
    float scrollOffset_ = 0.0f;
    int selectedIndex_ = -1;
    bool wasSelected_ = false;

    bool reorderable_ = false;
    bool reordering_ = false;
    int dragRow_ = -1;
    int reorderAccumY_ = 0;
    bool wasReordered_ = false;

    bool onTouchBegan(const TouchEvent& event) override;
    void onLongPress(const TouchEvent& event) override;
    void onDragMoved(const TouchEvent& event, int dx, int dy) override;
    void onDragEnded(const TouchEvent& event) override;
    void onTap(const TouchEvent& event) override;
};

} // namespace ui
