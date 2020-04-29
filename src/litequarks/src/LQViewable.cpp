#include <litequarks/LQViewable.hpp>
#include <litequarks/LQAppController.hpp>

LQViewable::LQViewable()
: LQSurface(), m_flex(false), m_hidden(false)
{
    // m_width.linkQuark<LQViewable, resizeCallback>(*this);
    // m_height.linkQuark<LQViewable, resizeCallback>(*this);
}
//   m_left(0.0f, this, LQNumber::Kind::coords, moveToX), m_top(LQMetricKind_Coords),
//   m_w(LQMetricKind_Length), m_h(LQMetricKind_Length)
// {
//     linkToLengths();
// }

LQViewable::LQViewable(LQNumber&& x, LQNumber&& y,
                       LQNumber&& width, LQNumber&& height,
                       GLint color, const std::string& iconPath)
: LQSurface(std::move(x), std::move(y), std::move(width), std::move(height),
            color, iconPath),
  m_flex(false), m_hidden(false)
//   m_left(this, LQMetricKind_Coords), m_top(this, LQMetricKind_Coords),
//   m_w(LQMetricKind_Length), m_h(LQMetricKind_Length)
{
    // m_width.linkQuark<LQViewable, resizeCallback>(*this);
    // m_height.linkQuark<LQViewable, resizeCallback>(*this);
}

LQViewable::LQViewable(LQNumber&& x, LQNumber&& y, bool flex)
: LQSurface(std::move(x), std::move(y), 0.0f, 0.0f),
  m_flex(flex), m_hidden(false)
{
    // m_width.linkQuark<LQViewable, resizeCallback>(*this);
    // m_height.linkQuark<LQViewable, resizeCallback>(*this);
}

LQViewable::~LQViewable() {
    LQAppController::removeFocus(this);
}

bool LQViewable::hidden() {
    return m_hidden;
}

void LQViewable::hide() {
    m_hidden = true;
}

void LQViewable::unhide() {
    m_hidden = false;
}

bool LQViewable::flexible() {
    return m_flex;
}

void LQViewable::displayFlex() {
    m_flex = true;
}

void LQViewable::displayBlock() {
    m_flex = false;
}

#include <iostream>
LQViewable& LQViewable::appendChild(LQViewable* child) {
    LQSurface::appendChild(child);
    if (flexible()) {
        std::cout << "flex" << std::endl;
        float newWidth = child->m_x.f() + child->m_width.f();
        float newHeight = child->m_y.f() + child->m_height.f();

        if (m_width.f() < newWidth) {
            std::cout << "width !" << std::endl;
            width() = newWidth;
        }
        if (m_height.f() < newHeight) {
            std::cout << "height !" << std::endl;
            height() = newHeight;
        }
    }
    return *this;
}

void LQViewable::drawChildren() {
    draw();
    for (
        LQViewable* child = static_cast<LQViewable*>(m_firstChild);
        child != nullptr;
        child = static_cast<LQViewable*>(child->m_nextSibling)
    ) {
        if (!child->hidden()) {
            child->drawChildren();
            blit(*child);
        }
    }
}

// void LQViewable::resizeCallback() {
//     LQSurface::resizeCallback();
// }
