#pragma once

#include <string>

#include "LQNumber.hpp"
#include "LQSurface.hpp"
#include "LQAppController.hpp"

class LQViewable : public LQSurface {
public:
    LQViewable();
    LQViewable(LQNumber&& x, LQNumber&& y,
               LQNumber&& width, LQNumber&& height,
               GLint color=0x000000, const std::string& iconPath="");
    LQViewable(LQNumber&& x, LQNumber&& y, bool flex=true);
    // LQViewable(LQNumber x, LQNumber y, LQImageData& image);
    LQViewable(LQViewable&& other) = default;

    bool hidden();
    void hide();
    void unhide();

    bool flexible();
    void displayFlex();
    void displayBlock();

    LQViewable& appendChild(LQViewable* child);

    void drawChildren() override;
    // margin
    // padding-top

protected:
    bool m_flex;
    bool m_hidden;
};
