#include "page_layout.h"
#include "colors.h"
#include "font.h"
#include <cstring>

namespace ui {

PageLayout::PageLayout() noexcept {
    for (int i = 0; i < MAX_PAGES; i++) {
        pages_[i].name = nullptr;
        pages_[i].draw = nullptr;
        pages_[i].handleTouch = nullptr;
    }
}

void PageLayout::addPage(const char* name, PageDrawFn draw,
                         PageTouchFn handleTouch) noexcept {
    if (pageCount_ >= MAX_PAGES) return;
    pages_[pageCount_].name = name;
    pages_[pageCount_].draw = draw;
    pages_[pageCount_].handleTouch = handleTouch;
    pageCount_++;
}

void PageLayout::setPage(int index) noexcept {
    if (index >= 0 && index < pageCount_)
        current_ = index;
}

int PageLayout::currentPage() const noexcept { return current_; }
int PageLayout::pageCount() const noexcept { return pageCount_; }

void PageLayout::draw(Framebuffer& fb) const {
    if (current_ < pageCount_ && pages_[current_].draw)
        pages_[current_].draw(fb);
}

bool PageLayout::handleTouch(const TouchState& touch) const {
    if (current_ < pageCount_ && pages_[current_].handleTouch)
        return pages_[current_].handleTouch(touch);
    return false;
}

Rect PageLayout::drawTabBar(Framebuffer& fb, int x, int y, int w, int h,
                            uint16_t fg, uint16_t bg, uint16_t activeBg) const {
    int tabW = pageCount_ > 0 ? w / pageCount_ : w;

    fb.fillRect(x, y, w, h, bg);

    for (int i = 0; i < pageCount_; i++) {
        int tx = x + i * tabW;
        uint16_t tabBg = (i == current_) ? activeBg : bg;
        uint16_t tabFg = (i == current_) ? bg : fg;

        fb.fillRect(tx, y, tabW, h, tabBg);

        if (pages_[i].name) {
            int tw = Framebuffer::textWidth(pages_[i].name);
            int lx = tx + (tabW - tw) / 2;
            int ly = y + (h - FONT_H) / 2;
            fb.drawText(lx, ly, pages_[i].name, tabFg, tabBg);
        }
    }

    return {x, y, w, h};
}

} // namespace ui
