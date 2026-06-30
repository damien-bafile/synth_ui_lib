#include "page_layout.h"

namespace ui {

PageLayout::PageLayout() noexcept {}

void PageLayout::addPage(const char* name, PageDrawFn draw,
                          PageTouchFn handleTouch) noexcept {
    if (pageCount_ < MAX_PAGES) {
        pages_[pageCount_].name        = name;
        pages_[pageCount_].draw        = draw;
        pages_[pageCount_].handleTouch = handleTouch;
        pageCount_++;
    }
}

void PageLayout::setPage(int index) noexcept {
    if (index >= 0 && index < pageCount_) {
        current_ = index;
    }
}

int PageLayout::currentPage() const noexcept { return current_; }
int PageLayout::pageCount() const noexcept { return pageCount_; }

void PageLayout::draw(Framebuffer& fb) const {
    if (pages_[current_].draw) {
        pages_[current_].draw(fb);
    }
}

bool PageLayout::handleTouch(const TouchEvent& touch) const {
    if (pages_[current_].handleTouch) {
        return pages_[current_].handleTouch(touch);
    }
    return false;
}

Rect PageLayout::drawTabBar(Framebuffer& fb, int x, int y, int w, int h,
                             uint16_t fg, uint16_t bg, uint16_t activeBg) const {
    int tabW = (pageCount_ > 0) ? w / pageCount_ : w;
    if (tabW < 1) tabW = 1;

    for (int i = 0; i < pageCount_; i++) {
        int tx = x + i * tabW;
        uint16_t tabBg = (i == current_) ? activeBg : bg;
        fb.fillRect(tx, y, tabW, h, tabBg);
        fb.drawRect(tx, y, tabW, h, fg);
        fb.drawText(tx + 2, y + 2, pages_[i].name, fg, tabBg);
    }

    return {x, y, w, h};
}

} // namespace ui
