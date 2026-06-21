#pragma once
#include <cstdint>
#include "framebuffer.h"
#include "colors.h"

namespace ui {

class Panel {
public:
    static constexpr int TITLE_H = 18;

    Panel(int x, int y, int w, int h, const char* title,
          uint16_t titleColor = ACCENT_1);

    void draw(Framebuffer& fb);

    void setTitle(const char* title) noexcept { title_ = title; }
    void setTitleColor(uint16_t color) noexcept { titleColor_ = color; }

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getWidth() const noexcept { return w_; }
    int getHeight() const noexcept { return h_; }
    const char* getTitle() const noexcept { return title_; }

private:
    int x_, y_, w_, h_;
    const char* title_;
    uint16_t titleColor_;
};

} // namespace ui
