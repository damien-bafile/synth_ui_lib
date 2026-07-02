#include "popup_menu.h"
#include "font.h"
#include <cstring>
#include <algorithm>

namespace ui {

PopupMenu::PopupMenu(int x, int y, const Item* items, int count,
                     uint16_t bg, uint16_t fg, uint16_t accent)
    : items_(items), itemCount_(count),
      bg_(bg), fg_(fg), accent_(accent) {
    int w = calcWidth(items, count);
    int h = calcHeight(count);
    setBounds(x, y, w, h);
    setVisible(false);
}

int PopupMenu::calcWidth(const Item* items, int count) {
    int maxW = 0;
    for (int i = 0; i < count; i++) {
        int tw = Framebuffer::textWidth(items[i].label);
        if (tw > maxW) maxW = tw;
    }
    return maxW + 16;
}

int PopupMenu::calcHeight(int count) {
    return count * 24 + 4;
}

void PopupMenu::show() {
    setVisible(true);
    selected_ = -1;
    pressedIdx_ = -1;
}

void PopupMenu::showAt(int x, int y) {
    setBounds(x, y, w_, h_);
    show();
}

void PopupMenu::dismiss() {
    setVisible(false);
    pressedIdx_ = -1;
}

void PopupMenu::setItems(const Item* items, int count) {
    items_ = items;
    itemCount_ = count;
    int w = calcWidth(items, count);
    int h = calcHeight(count);
    setBounds(x_, y_, w, h);
    selected_ = -1;
    pressedIdx_ = -1;
    wasSelected_ = false;
}

bool PopupMenu::wasSelected() const {
    bool v = wasSelected_;
    wasSelected_ = false;
    return v;
}

int PopupMenu::itemIndexAt(int py) const {
    int idx = (py - y_ - 2) / itemH_;
    if (idx < 0 || idx >= itemCount_) return -1;
    int iy = y_ + 2 + idx * itemH_;
    if (py < iy || py >= iy + itemH_) return -1;
    return idx;
}

bool PopupMenu::onTouchBegan(const TouchEvent& event) {
    if (!visible()) return false;
    if (!enabled()) return false;
    if (!contains(event.x, event.y)) {
        dismiss();
        return true;
    }
    pressedIdx_ = itemIndexAt(event.y);
    return true;
}

void PopupMenu::onTouchEnded(const TouchEvent& event) {
    if (!visible()) return;

    if (pressedIdx_ >= 0) {
        int idx = itemIndexAt(event.y);
        if (idx == pressedIdx_) {
            selected_ = idx;
            wasSelected_ = true;
            dismiss();
        }
    }
    pressedIdx_ = -1;
}

void PopupMenu::onTouchCancelled(const TouchEvent&) {
    pressedIdx_ = -1;
}

// ── Drawing ──

void PopupMenu::paintTrampoline(Framebuffer& fb, void* user) {
    static_cast<PopupMenu*>(user)->paint(fb);
}

void PopupMenu::paint(Framebuffer& fb) {
    if (!visible()) return;

    int pad = 2;
    int ix = x_ + pad;
    int iy = y_ + pad;

    fb.fillRect(x_, y_, w_, h_, bg_);
    fb.drawRect(x_, y_, w_, h_, accent_);

    for (int i = 0; i < itemCount_; i++) {
        int itemY = iy + i * itemH_;
        bool isPressed = (i == pressedIdx_);
        uint16_t itemBg = isPressed ? accent_ : bg_;
        uint16_t itemFg = isPressed ? BG_DARK : fg_;

        fb.fillRect(ix, itemY, w_ - pad * 2, itemH_, itemBg);
        fb.drawText(ix + 4, itemY + (itemH_ - FONT_H) / 2,
                    items_[i].label, itemFg, itemBg);
    }
}

void PopupMenu::draw(Framebuffer& fb, uint32_t) {
    if (!visible()) return;
    fb.enqueueOverlay(x_, y_, w_, h_,
                      &PopupMenu::paintTrampoline, this,
                      reinterpret_cast<uintptr_t>(this));
}

} // namespace ui
