#pragma once
#include <cstdint>
#include "rect.h"
#include "framebuffer.h"
#include "touch.h"

namespace ui {

using PageDrawFn  = void (*)(Framebuffer& fb);
using PageTouchFn = bool (*)(const TouchState& touch);

struct Page {
    const char* name;
    PageDrawFn   draw;
    PageTouchFn  handleTouch;
};

class PageLayout {
public:
    static constexpr int MAX_PAGES = 8;

    PageLayout() noexcept;

    void addPage(const char* name, PageDrawFn draw,
                 PageTouchFn handleTouch = nullptr) noexcept;

    void setPage(int index) noexcept;
    int  currentPage() const noexcept;
    int  pageCount() const noexcept;

    void draw(Framebuffer& fb) const;
    bool handleTouch(const TouchState& touch) const;

    Rect drawTabBar(Framebuffer& fb, int x, int y, int w, int h,
                    uint16_t fg, uint16_t bg, uint16_t activeBg) const;

private:
    Page pages_[MAX_PAGES];
    int  pageCount_ = 0;
    int  current_   = 0;
};

} // namespace ui
