#pragma once
#include "framebuffer.h"
#include "touch.h"

namespace ui {

class IWidget {
public:
    virtual ~IWidget() = default;
    virtual void draw(Framebuffer& fb) = 0;
    virtual bool handleTouch(const TouchState& /*touch*/) { return false; }
};

} // namespace ui
