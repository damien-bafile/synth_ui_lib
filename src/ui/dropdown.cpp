#include "dropdown.h"
#include "font.h"

namespace ui {

Dropdown::Dropdown(int x, int y, int w, int h, DropdownStyle style,
                   uint16_t fg, uint16_t bg, uint16_t accent)
    : style_(style), fg_(fg), bg_(bg), accent_(accent), itemCount_(0),
      lastSelectedIndex_(0) {
    setBounds(x, y, w, h);
    for (int i = 0; i < MAX_ITEMS; i++) {
        items_[i] = nullptr;
        icons_[i] = SynthIcon::NONE;
    }
}

void Dropdown::addItem(const char* item) {
    if (itemCount_ < MAX_ITEMS) {
        items_[itemCount_] = item;
        icons_[itemCount_] = SynthIcon::NONE;
        itemCount_++;
    }
}

void Dropdown::addItem(const char* item, SynthIcon icon) {
    if (itemCount_ < MAX_ITEMS) {
        items_[itemCount_] = item;
        icons_[itemCount_] = icon;
        itemCount_++;
    }
}

void Dropdown::clearItems() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        items_[i] = nullptr;
        icons_[i] = SynthIcon::NONE;
    }
    itemCount_ = 0;
}

int Dropdown::getItemHeight() const noexcept {
    switch (style_) {
        case DropdownStyle::COMPACT: return 14;
        default: return h_;
    }
}

void Dropdown::drawChevron(Framebuffer& fb, int cx, int cy, bool down, uint16_t color) {
    if (down) {
        fb.drawLine(cx - 4, cy - 1, cx, cy + 2, color);
        fb.drawLine(cx, cy + 2, cx + 4, cy - 1, color);
    } else {
        fb.drawLine(cx - 4, cy + 1, cx, cy - 2, color);
        fb.drawLine(cx, cy - 2, cx + 4, cy + 1, color);
    }
}

void Dropdown::drawClassic(Framebuffer& fb, int selectedIndex, bool expanded) {
    uint16_t border = expanded ? accent_ : fg_;
    fb.fillRect(x_, y_, w_, h_, bg_);
    fb.drawRect(x_, y_, w_, h_, border);

    if (selectedIndex >= 0 && selectedIndex < itemCount_ && items_[selectedIndex]) {
        int tx = x_ + 4;
        int ty = y_ + (h_ - FONT_H) / 2;
        if (icons_[selectedIndex] != SynthIcon::NONE) {
            fb.drawSynthIcon(x_ + 4, ty, icons_[selectedIndex], fg_, bg_);
            tx = x_ + 10;
        }
        fb.drawText(tx, ty, items_[selectedIndex], fg_, bg_);
    }

    drawChevron(fb, x_ + w_ - 10, y_ + h_ / 2, expanded, fg_);

    if (expanded) {
        int ih = getItemHeight();
        int listY = y_ + h_;
        int listH = ih * itemCount_;
        lastSelectedIndex_ = selectedIndex;
        fb.enqueueOverlay(x_, listY, w_, listH,
                          &Dropdown::paintExpandedListTrampoline, this);
    }
}

void Dropdown::drawOutlined(Framebuffer& fb, int selectedIndex, bool expanded) {
    fb.fillRect(x_, y_, w_, h_, BG_DARK);
    fb.drawRect(x_, y_, w_, h_, expanded ? accent_ : GRAY_MID);

    if (selectedIndex >= 0 && selectedIndex < itemCount_ && items_[selectedIndex]) {
        int tx = x_ + 4;
        int ty = y_ + (h_ - FONT_H) / 2;
        if (icons_[selectedIndex] != SynthIcon::NONE) {
            fb.drawSynthIcon(x_ + 4, ty, icons_[selectedIndex], fg_, BG_DARK);
            tx = x_ + 10;
        }
        fb.drawText(tx, ty, items_[selectedIndex], fg_, BG_DARK);
    }

    drawChevron(fb, x_ + w_ - 10, y_ + h_ / 2, expanded, TEXT_DIM);

    if (expanded) {
        int ih = getItemHeight();
        int listY = y_ + h_;
        int listH = ih * itemCount_;
        lastSelectedIndex_ = selectedIndex;
        fb.enqueueOverlay(x_, listY, w_, listH,
                          &Dropdown::paintExpandedListTrampoline, this);
    }
}

void Dropdown::drawUnderlined(Framebuffer& fb, int selectedIndex, bool expanded) {
    if (selectedIndex >= 0 && selectedIndex < itemCount_ && items_[selectedIndex]) {
        int tx = x_;
        int ty = y_ + (h_ - FONT_H) / 2;
        if (icons_[selectedIndex] != SynthIcon::NONE) {
            fb.drawSynthIcon(x_, ty, icons_[selectedIndex], fg_, BG_DARK);
            tx = x_ + 6;
        }
        fb.drawText(tx, ty, items_[selectedIndex], fg_, BG_DARK);
        int tw = Framebuffer::textWidth(items_[selectedIndex]);
        int iconW = (icons_[selectedIndex] != SynthIcon::NONE) ? 6 : 0;
        int lineY = y_ + h_ - 2;
        fb.drawLine(x_, lineY, x_ + tw + iconW + 12, lineY, expanded ? accent_ : TEXT_DIM);
    } else {
        fb.drawLine(x_, y_ + h_ - 2, x_ + w_, y_ + h_ - 2, TEXT_DIM);
    }

    drawChevron(fb, x_ + w_ - 4, y_ + h_ / 2, expanded, TEXT_DIM);

    if (expanded) {
        int ih = getItemHeight();
        int listY = y_ + h_ + 2;
        int listH = ih * itemCount_;
        lastSelectedIndex_ = selectedIndex;
        fb.enqueueOverlay(x_, listY, w_, listH,
                          &Dropdown::paintExpandedListTrampoline, this);
    }
}

void Dropdown::drawCompact(Framebuffer& fb, int selectedIndex, bool expanded) {
    int ch = h_ - 4;
    int cy = y_ + 2;
    uint16_t border = expanded ? accent_ : fg_;
    fb.fillRect(x_, cy, w_, ch, bg_);
    fb.drawRect(x_, cy, w_, ch, border);

    if (selectedIndex >= 0 && selectedIndex < itemCount_ && items_[selectedIndex]) {
        int tx = x_ + 3;
        int ty = cy + (ch - FONT_H) / 2;
        if (icons_[selectedIndex] != SynthIcon::NONE) {
            fb.drawSynthIcon(x_ + 3, ty, icons_[selectedIndex], fg_, bg_);
            tx = x_ + 9;
        }
        fb.drawText(tx, ty, items_[selectedIndex], fg_, bg_);
    }

    drawChevron(fb, x_ + w_ - 8, cy + ch / 2, expanded, fg_);

    if (expanded) {
        int ih = 14;
        int listY = cy + ch;
        int listH = ih * itemCount_;
        lastSelectedIndex_ = selectedIndex;
        fb.enqueueOverlay(x_, listY, w_, listH,
                          &Dropdown::paintExpandedListTrampoline, this);
    }
}

void Dropdown::paintExpandedListTrampoline(Framebuffer& fb, void* user) {
    Dropdown* self = static_cast<Dropdown*>(user);
    if (self) self->paintExpandedListImpl(fb, self->lastSelectedIndex_);
}

void Dropdown::paintExpandedListImpl(Framebuffer& fb, int selectedIndex) {
    int listY;
    int ih;

    switch (style_) {
        case DropdownStyle::COMPACT: {
            int ch = h_ - 4;
            int cy = y_ + 2;
            ih = 14;
            listY = cy + ch;
            break;
        }
        case DropdownStyle::UNDERLINED: {
            ih = getItemHeight();
            listY = y_ + h_ + 2;
            break;
        }
        default: {
            ih = getItemHeight();
            listY = y_ + h_;
            break;
        }
    }

    for (int i = 0; i < itemCount_; i++) {
        if (!items_[i]) continue;
        int iy = listY + i * ih;

        switch (style_) {
            case DropdownStyle::CLASSIC: {
                uint16_t itemBg = (i == selectedIndex) ? accent_ : BG_DARK;
                uint16_t itemFg = (i == selectedIndex) ? BG_DARK : TEXT;
                fb.fillRect(x_, iy, w_, ih, itemBg);
                fb.drawRect(x_, iy, w_, ih, GRAY_MID);
                int ty = iy + (ih - FONT_H) / 2;
                if (icons_[i] != SynthIcon::NONE) {
                    fb.drawSynthIcon(x_ + 4, ty, icons_[i], itemFg, itemBg);
                }
                fb.drawText(x_ + 10, ty, items_[i], itemFg, itemBg);
                break;
            }
            case DropdownStyle::OUTLINED: {
                uint16_t itemBg = (i == selectedIndex) ? accent_ : BG_DARK;
                uint16_t itemFg = (i == selectedIndex) ? BG_DARK : fg_;
                fb.fillRect(x_, iy, w_, ih, itemBg);
                fb.drawRect(x_, iy, w_, ih, GRAY_MID);
                int ty = iy + (ih - FONT_H) / 2;
                if (icons_[i] != SynthIcon::NONE) {
                    fb.drawSynthIcon(x_ + 4, ty, icons_[i], itemFg, itemBg);
                }
                fb.drawText(x_ + 10, ty, items_[i], itemFg, itemBg);
                break;
            }
            case DropdownStyle::UNDERLINED: {
                uint16_t lineColor = (i == selectedIndex) ? accent_ : GRAY_MID;
                fb.drawLine(x_, iy + ih - 1, x_ + w_, iy + ih - 1, lineColor);
                int ty = iy + (ih - FONT_H) / 2;
                if (icons_[i] != SynthIcon::NONE) {
                    fb.drawSynthIcon(x_, ty, icons_[i],
                                     (i == selectedIndex) ? accent_ : fg_,
                                     BG_DARK);
                }
                uint16_t itemFg = (i == selectedIndex) ? accent_ : fg_;
                fb.drawText(x_ + 6, ty, items_[i], itemFg, BG_DARK);
                break;
            }
            case DropdownStyle::COMPACT: {
                uint16_t itemBg = (i == selectedIndex) ? accent_ : BG_DARK;
                uint16_t itemFg = (i == selectedIndex) ? BG_DARK : TEXT;
                fb.fillRect(x_, iy, w_, ih, itemBg);
                fb.drawRect(x_, iy, w_, ih, GRAY_MID);
                int ty = iy + (ih - FONT_H) / 2;
                if (icons_[i] != SynthIcon::NONE) {
                    fb.drawSynthIcon(x_ + 3, ty, icons_[i], itemFg, itemBg);
                }
                fb.drawText(x_ + 9, ty, items_[i], itemFg, itemBg);
                break;
            }
        }
    }
}

void Dropdown::draw(Framebuffer& fb, int selectedIndex, bool expanded) {
    switch (style_) {
        case DropdownStyle::CLASSIC:    drawClassic(fb, selectedIndex, expanded); break;
        case DropdownStyle::OUTLINED:   drawOutlined(fb, selectedIndex, expanded); break;
        case DropdownStyle::UNDERLINED: drawUnderlined(fb, selectedIndex, expanded); break;
        case DropdownStyle::COMPACT:    drawCompact(fb, selectedIndex, expanded); break;
    }
}

bool Dropdown::onTouchBegan(const TouchEvent& event) {
    int ih = getItemHeight();

    // Check header
    if (contains(event.x, event.y)) {
        // TODO: distinguish header from item — handled in onTap based on expanded_ state
        return true;
    }

    // Check expanded items
    if (expanded_) {
        int iy;
        switch (style_) {
            case DropdownStyle::COMPACT:
                iy = y_ + h_ - 2;
                break;
            case DropdownStyle::UNDERLINED:
                iy = y_ + h_ + 2;
                break;
            default:
                iy = y_ + h_;
                break;
        }
        for (int i = 0; i < itemCount_; i++) {
            if (event.y >= iy && event.y < iy + ih &&
                event.x >= x_ && event.x < x_ + w_) {
                return true;
            }
            iy += ih;
        }
    }

    return false;
}

void Dropdown::onTap(const TouchEvent& event) {
    int ih = getItemHeight();

    // Check header tap
    if (contains(event.x, event.y)) {
        expanded_ = !expanded_;
        return;
    }

    // Check item tap
    if (expanded_) {
        int iy;
        switch (style_) {
            case DropdownStyle::COMPACT:
                iy = y_ + h_ - 2;
                break;
            case DropdownStyle::UNDERLINED:
                iy = y_ + h_ + 2;
                break;
            default:
                iy = y_ + h_;
                break;
        }
        for (int i = 0; i < itemCount_; i++) {
            if (event.y >= iy && event.y < iy + ih &&
                event.x >= x_ && event.x < x_ + w_) {
                selectedIndex_ = i;
                wasSelected_ = true;
                expanded_ = false;
                return;
            }
            iy += ih;
        }
    }
}

} // namespace ui
