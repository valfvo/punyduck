#include <litequarks/LQAppController.hpp>
#include <litequarks/LQButton.hpp>

LQButton::LQButton(LQNumber&& x, LQNumber&& y, LQImageData imageData,
         LQViewable* target, void (LQViewable::*callback)(LQClickEvent&))
: LQViewable(std::move(x), std::move(y)),
  m_target(target), m_callback(callback)
{
    lqOn<LQClickEvent>(this, onclick);
}

LQButton::LQButton(LQNumber&& x, LQNumber&& y,
         LQNumber&& width, LQNumber&& height, const std::string& iconPath,
         LQViewable* target, void (LQViewable::*callback)(LQClickEvent&))
: LQViewable(std::move(x), std::move(y), std::move(width), std::move(height),
             0xD9D9D9, iconPath),
  m_target(target), m_callback(callback)
{
    lqOn<LQClickEvent>(this, onclick);
}

void LQButton::onclick(LQClickEvent& event) {
    if (m_target && m_callback) {
        (m_target->*m_callback)(event);
    }
}

void LQButton::draw() { }
