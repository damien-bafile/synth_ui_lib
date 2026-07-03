#include "tabbar.h"

namespace ui {

TabBar::TabBar(int x, int y, int w, int h, uint16_t accent)
    : accent_(accent) {
    setBounds(x, y, w, h);
}

void TabBar::setTabs(const char* const* labels, int count) {
    if (count > MAX_TABS) count = MAX_TABS;
    for (int i = 0; i < count; i++) labels_[i] = labels[i];
    count_ = count;
}

void TabBar::draw(Framebuffer& fb, int activeIndex) {
    if (count_ <= 0) return;
    int tab_w = w_ / count_;

    fb.fillRect(x_, y_, w_, h_, BG_MID);

    for (int i = 0; i < count_; i++) {
        int tx = x_ + i * tab_w;
        bool is_active = (i == activeIndex);
        uint16_t bg = is_active ? accent_ : BG_SURFACE;
        uint16_t fg = is_active ? BG_DARK : TEXT_DIM;
        fb.fillRect(tx, y_, tab_w - 1, h_, bg);
        fb.drawText(tx + 2, y_ + 2, labels_[i], fg, bg);
    }
}

bool TabBar::onTouchBegan(const TouchEvent& event) {
    return contains(event.x, event.y);
}

void TabBar::onTap(const TouchEvent& event) {
    if (count_ <= 0) return;
    int tab = (event.x - x_) / (w_ / count_);
    if (tab < 0) tab = 0;
    if (tab >= count_) tab = count_ - 1;
    selectedIndex_ = tab;
    wasSelected_ = true;
}

} // namespace ui
