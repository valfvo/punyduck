#include <litequarks/LQAppController.hpp>
#include <litequarks/LQButton.hpp>
#include <litequarks/LQText.hpp>

using namespace LQUnit;

LQButton::LQButton(LQNumber&& x, LQNumber&& y, LQImageData imageData,
                   const std::function<void(void)>& callback)
    // LQViewable* target, void (LQViewable::*callback)(LQClickEvent&))
: LQViewable(std::move(x), std::move(y)), m_callback(callback)
//   m_target(target), m_callback(callback)
{
    lqOn<LQClickEvent>(this, onclick);
}

LQButton::LQButton(LQNumber&& x, LQNumber&& y,
    LQNumber&& width, LQNumber&& height, GLint color,
    const std::string& iconPath, const std::function<void(void)>& callback)
    // LQViewable* target, void (LQViewable::*callback)(LQClickEvent&))
: LQViewable(std::move(x), std::move(y), std::move(width), std::move(height),
             color, iconPath),
  m_callback(callback)
//   m_target(target), m_callback(callback)
{
    lqOn<LQClickEvent>(this, onclick);
}

LQButton::LQButton(LQNumber&& _x, LQNumber&& _y,
    LQNumber&& _width, LQNumber&& _height, GLint color,
    const std::string& _text, GLint textColor,
    const std::function<void(void)>& callback)
    // LQViewable* target, void (LQViewable::*callback)(LQClickEvent&))
: LQViewable(std::move(_x), std::move(_y), std::move(_width), std::move(_height),
             color),
  m_callback(callback)
//   m_target(target), m_callback(callback)
{
    lqOn<LQClickEvent>(this, onclick);

    auto* text = new LQText(_text, 0.0f, height() / 2 + 0.4_em, 1_em, textColor);
    text->x() = width() / 2 - text->widthF() / 2;
    appendChild(text);
}

void LQButton::onclick(LQClickEvent& event) {
    // if (m_target && m_callback) {
    if (m_callback) {
        m_callback();
        // (m_target->*m_callback)(event);
    }
}

void LQButton::draw() {
    if (firstChild()) {
        clear();
    }
}
