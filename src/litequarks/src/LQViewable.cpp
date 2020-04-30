#include <litequarks/LQViewable.hpp>
#include <litequarks/LQAppController.hpp>

LQViewable::LQViewable()
: LQSurface(), m_flex(false), m_hidden(false)
{
    m_width.linkQuark<LQViewable, resizeWidthCallback>(*this);
    m_height.linkQuark<LQViewable, resizeHeightCallback>(*this);
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
    m_width.linkQuark<LQViewable, resizeWidthCallback>(*this);
    m_height.linkQuark<LQViewable, resizeHeightCallback>(*this);
}

LQViewable::LQViewable(LQNumber&& x, LQNumber&& y, bool flex)
: LQSurface(std::move(x), std::move(y), 0.0f, 0.0f),
  m_flex(flex), m_hidden(false)
{
    m_width.linkQuark<LQViewable, resizeWidthCallback>(*this);
    m_height.linkQuark<LQViewable, resizeHeightCallback>(*this);
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

LQViewable& LQViewable::appendChild(LQViewable* child) {
    LQSurface::appendChild(child);
    if (flexible()) {
        float newWidth = child->xF() + child->widthF();
        float newHeight = child->yF() + child->heightF();

        if (widthF() < newWidth) {
            width() = newWidth;
        }
        if (heightF() < newHeight) {
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

void LQViewable::resizeWidthCallback() {
    LQSurface::resizeCallback();
    auto* p_quark = parent();
    auto* p_parent = static_cast<LQViewable*>(p_quark);
    if (p_quark && p_parent->flexible()) {
        float newWidth = xF() + widthF();

        if (p_parent->widthF() < newWidth) {
            p_parent->width() = newWidth;
        }
        else if (xF() + LQNumber::old() >= p_parent->widthF()) {
            float newParentWidth = 0.0f, w = 0.0f;

            for (auto* child = static_cast<LQViewable*>(p_parent->firstChild());
                 child != nullptr;
                 child = static_cast<LQViewable*>(child->nextSibling()))
            {
                w = child->xF() + child->widthF();
                if (w > newParentWidth) {
                    newParentWidth = w;
                }
            }

            if (p_parent->widthF() > newParentWidth) {
                p_parent->width() = newParentWidth;
            }
        }
    }
}

void LQViewable::resizeHeightCallback() {
    LQSurface::resizeCallback();
    auto* p_quark = parent();
    auto* p_parent = static_cast<LQViewable*>(p_quark);
    if (p_quark && p_parent->flexible()) {
        float newHeight = yF() + heightF();

        if (p_parent->heightF() < newHeight) {
            p_parent->height() = newHeight;
        }
        else if (yF() + LQNumber::old() >= p_parent->heightF()) {
            float newParentHeight = 1.0f, h = 0.0f;

            for (auto* child = static_cast<LQViewable*>(p_parent->firstChild());
                 child != nullptr;
                 child = static_cast<LQViewable*>(child->nextSibling()))
            {
                h = child->yF() + child->heightF();
                if (h > newParentHeight) {
                    newParentHeight = h;
                }
            }
            if (p_parent->heightF() > newParentHeight) {
                p_parent->height() = newParentHeight;
            }
        }
    }
}
