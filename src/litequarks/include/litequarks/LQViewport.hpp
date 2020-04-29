#pragma once

#include "LQViewable.hpp"
#include "LQEvent.hpp"

class LQViewport : public LQViewable {
public:
    LQViewport(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height,
               GLint color=0x000000, float minGripSize=20.0f);

    void onScroll(LQScrollEvent& event);
    void recalc();

protected:
    float m_minGripSize;
};
