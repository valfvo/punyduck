#pragma once

#include <string>

#include "LQEvent.hpp"
#include "LQViewable.hpp"

class LQButton : public LQViewable {
public:
    LQButton(LQNumber&& x, LQNumber&& y, LQImageData imageData,
             LQViewable* target=nullptr,
             void (LQViewable::*callback)(LQClickEvent&)=nullptr);

    LQButton(LQNumber&& x, LQNumber&& y, LQNumber&& width, LQNumber&& height,
             const std::string& iconPath="", LQViewable* target=nullptr,
             void (LQViewable::*callback)(LQClickEvent&)=nullptr);

    //LQButton(LQNumber x, LQNumber y, LQViewable* target, void (LQViewable::*m_callback)(LQClickEvent&), LQText text);

    void onclick(LQClickEvent& event);

    void draw() override;

private:
    LQViewable* m_target;
    void (LQViewable::*m_callback)(LQClickEvent&);
};
