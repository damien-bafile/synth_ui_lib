#include "list_view.h"
#include "font.h"

namespace ui {

ListView::ListView(int x, int y, int w, int h,
                   const char* const* items, int count,
                   uint16_t fg, uint16_t bg, uint16_t accent) noexcept
    : items_(items), count_(0), fg_(fg), bg_(bg), accent_(accent) {
    setBounds(x, y, w, h);
    setItems(items, count);
}

void ListView::setItems(const char* const* items, int count) noexcept {
    items_ = items;
    count_ = (count > MAX_ITEMS) ? MAX_ITEMS : ((count < 0) ? 0 : count);
    for (int i = 0; i < count_; i++) order_[i] = i;
    selectedIndex_ = -1;
    wasSelected_ = false;
    scrollOffset_ = 0.0f;
    reordering_ = false;
    dragRow_ = -1;
}

float ListView::maxScrollOffset() const noexcept {
    float total = static_cast<float>(count_) * ROW_H;
    float over = total - static_cast<float>(h_);
    return over > 0.0f ? over : 0.0f;
}

void ListView::setScrollOffset(float px) noexcept {
    if (px < 0.0f) px = 0.0f;
    float maxOff = maxScrollOffset();
    if (px > maxOff) px = maxOff;
    scrollOffset_ = px;
}

int ListView::rowAt(int py) const noexcept {
    if (py < y_ || py >= y_ + h_) return -1;
    int localY = py - y_ + static_cast<int>(scrollOffset_);
    int row = localY / ROW_H;
    if (row < 0 || row >= count_) return -1;
    return row;
}

bool ListView::onTouchBegan(const TouchEvent& event) {
    return contains(event.x, event.y);
}

void ListView::onLongPress(const TouchEvent& event) {
    if (!reorderable_) return;
    int row = rowAt(event.y);
    if (row < 0) return;
    reordering_ = true;
    dragRow_ = row;
    reorderAccumY_ = 0;
}

void ListView::onDragMoved(const TouchEvent&, int /*dx*/, int dy) {
    if (reordering_) {
        reorderAccumY_ += dy;
        while (reorderAccumY_ >= ROW_H / 2 && dragRow_ < count_ - 1) {
            int tmp = order_[dragRow_];
            order_[dragRow_] = order_[dragRow_ + 1];
            order_[dragRow_ + 1] = tmp;
            dragRow_++;
            reorderAccumY_ -= ROW_H;
            wasReordered_ = true;
        }
        while (reorderAccumY_ <= -ROW_H / 2 && dragRow_ > 0) {
            int tmp = order_[dragRow_];
            order_[dragRow_] = order_[dragRow_ - 1];
            order_[dragRow_ - 1] = tmp;
            dragRow_--;
            reorderAccumY_ += ROW_H;
            wasReordered_ = true;
        }
    } else {
        setScrollOffset(scrollOffset_ - dy);
    }
}

void ListView::onDragEnded(const TouchEvent&) {
    reordering_ = false;
    dragRow_ = -1;
    reorderAccumY_ = 0;
}

void ListView::onTap(const TouchEvent& event) {
    if (reordering_) return;
    int row = rowAt(event.y);
    if (row < 0) return;
    selectedIndex_ = order_[row];
    wasSelected_ = true;
}

void ListView::draw(Framebuffer& fb) const {
    fb.fillRect(x_, y_, w_, h_, bg_);

    for (int r = 0; r < count_; r++) {
        int ry = y_ - static_cast<int>(scrollOffset_) + r * ROW_H;
        if (ry + ROW_H <= y_ || ry >= y_ + h_) continue;

        int origIdx = order_[r];
        bool selected = (origIdx == selectedIndex_);
        uint16_t rowBg = selected ? accent_ : bg_;
        uint16_t rowFg = selected ? BG_DARK : fg_;

        fb.fillRect(x_, ry, w_, ROW_H, rowBg);
        fb.drawRect(x_, ry, w_, ROW_H, GRAY_MID);

        int tx = x_ + 4;
        int ty = ry + (ROW_H - FONT_H) / 2;
        if (icons_ && icons_[origIdx] != SynthIcon::NONE) {
            fb.drawSynthIcon(x_ + 4, ty, icons_[origIdx], rowFg, rowBg);
            tx = x_ + 10;
        }
        if (items_[origIdx]) fb.drawText(tx, ty, items_[origIdx], rowFg, rowBg);
    }
}

} // namespace ui
