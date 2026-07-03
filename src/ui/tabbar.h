#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "widget.h"
#include "colors.h"

namespace ui {

// Full-width bar of equal tabs; the active one fills with the accent
// color. Tapping a tab records it: poll wasSelected()/selectedIndex().
class TabBar : public Widget {
public:
    static constexpr int MAX_TABS = 6;

    TabBar(int x, int y, int w, int h, uint16_t accent = ACCENT_2);

    void setTabs(const char* const* labels, int count);
    void setAccent(uint16_t accent) noexcept { accent_ = accent; }

    void draw(Framebuffer& fb, int activeIndex);

    int selectedIndex() const noexcept { return selectedIndex_; }
    bool wasSelected() noexcept {
        bool v = wasSelected_;
        wasSelected_ = false;
        return v;
    }

private:
    const char* labels_[MAX_TABS] = {};
    int count_ = 0;
    uint16_t accent_;
    int selectedIndex_ = -1;
    bool wasSelected_ = false;

    bool onTouchBegan(const TouchEvent& event) override;
    void onTap(const TouchEvent& event) override;
};

} // namespace ui
