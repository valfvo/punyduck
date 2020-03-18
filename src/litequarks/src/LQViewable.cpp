#include <litequarks/LQViewable.hpp>

LQViewable::LQViewable()
: LQSurface(),
  m_left(LQMetricKind_Coords), m_top(LQMetricKind_Coords),
  m_w(LQMetricKind_Length), m_h(LQMetricKind_Length)
{
    linkToLengths();
}

LQViewable::LQViewable(GLfloat x, GLfloat y, GLfloat width, GLfloat height)
: LQSurface(x, y, width, height),
  m_left(LQMetricKind_Coords), m_top(LQMetricKind_Coords),
  m_w(LQMetricKind_Length), m_h(LQMetricKind_Length)
{
    linkToLengths();
}

LQViewable::LQViewable(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLint color) 
: LQSurface(x, y, width, height, color),
  m_left(LQMetricKind_Coords), m_top(LQMetricKind_Coords),
  m_w(LQMetricKind_Length), m_h(LQMetricKind_Length)
{
    linkToLengths();
}

LQMetricExpr<LQViewable> LQViewable::left() {
    return m_left;
}

LQMetricExpr<LQViewable> LQViewable::right() {
    return (LQMetricExpr<LQViewable>)m_w + m_left;
}

LQMetricExpr<LQViewable> LQViewable::top() {
    return m_top;
}

LQMetricExpr<LQViewable> LQViewable::bottom() {
    return (LQMetricExpr<LQViewable>)m_top + m_h;
}

LQMetricExpr<LQViewable> LQViewable::width() {
    return m_w;
}

LQMetricExpr<LQViewable> LQViewable::height() {
    return m_h;
}

void LQViewable::linkToLengths() {
    m_left.linkToQuark(
        this,
        static_cast<LQGetterPtr<LQViewable, GLfloat>>(&LQViewable::getX),
        static_cast<LQSetterPtr<LQViewable, GLfloat>>(&LQViewable::moveToX));

    m_top.linkToQuark(
        this,
        static_cast<LQGetterPtr<LQViewable, GLfloat>>(&LQViewable::getY),
        static_cast<LQSetterPtr<LQViewable, GLfloat>>(&LQViewable::moveToY));

    m_w.linkToQuark(
        this,
        static_cast<LQGetterPtr<LQViewable, GLfloat>>(&LQViewable::getWidth),
        static_cast<LQSetterPtr<LQViewable, GLfloat>>(&LQViewable::resizeWidth));

    m_h.linkToQuark(
        this,
        static_cast<LQGetterPtr<LQViewable, GLfloat>>(&LQViewable::getHeight),
        static_cast<LQSetterPtr<LQViewable, GLfloat>>(&LQViewable::resizeHeight));
}
