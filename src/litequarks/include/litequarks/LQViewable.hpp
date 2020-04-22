#pragma once

// #include "LQNumber.hpp"
#include "LQSurface.hpp"
#include "LQAppController.hpp"

class LQViewable : public LQSurface {
public:
    LQViewable();
    LQViewable(LQNumber&& x, LQNumber&& y,
               LQNumber&& width, LQNumber&& height, GLint color=0x000000);
    // LQViewable(LQNumber x, LQNumber y, bool flex=true);
    // LQViewable(LQNumber x, LQNumber y, LQImageData& image);
    LQViewable(LQViewable&& other) = default;

    // margin
    // padding-top

protected:
    // bool m_flex;
    // bool m_hidden;
};
