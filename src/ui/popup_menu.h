#pragma once
#include "widget.h"
#include "framebuffer.h"
#include "colors.h"
#include <cstdint>

namespace ui {

class PopupMenu : public Widget {
public:
    struct Item {
        const char* label;
    };

    PopupMenu(int x, int y, const Item* items, int count,
              uint16_t bg = BG_SURFACE,
              uint16_t fg = TEXT,
              uint16_t accent = ACCENT_2);

    void show();
    void showAt(int x, int y);
    void setItems(const Item* items, int count);
    void dismiss();
    bool isVisible() const { return visible(); }

    int getSelectedIndex() const { return selected_; }
    const char* getSelectedLabel() const { return (selected_ >= 0) ? items_[selected_].label : nullptr; }
    bool wasSelected() const;
    int itemCount() const { return itemCount_; }

    void draw(Framebuffer& fb, uint32_t ticks);

    bool onTouchBegan(const TouchEvent&) override;
    void onTouchEnded(const TouchEvent&) override;
    void onTouchCancelled(const TouchEvent&) override;

    static int calcWidth(const Item* items, int count);
    static int calcHeight(int count);

private:
    int itemIndexAt(int py) const;

    static void paintTrampoline(Framebuffer& fb, void* user);
    void paint(Framebuffer& fb);

    const Item* items_;
    int itemCount_;
    int itemH_ = 24;
    int selected_ = -1;
    mutable bool wasSelected_ = false;
    int pressedIdx_ = -1;

    uint16_t bg_, fg_, accent_;
};

} // namespace ui
