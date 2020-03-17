#include <litequarks/LQViewable.hpp>

LQViewable::LQViewable()
: LQSurface(), m_left(), m_top(), m_w(), m_h()
{
    linkToLengths();
}

LQViewable::LQViewable(GLfloat x, GLfloat y, GLfloat width, GLfloat height)
: LQSurface(x, y, width, height), m_left(), m_top(), m_w(), m_h()
{
    linkToLengths();
}

LQViewable::LQViewable(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLint color) 
: LQSurface(x, y, width, height, color), m_left(), m_top(), m_w(), m_h()
{
    linkToLengths();
}

LQLength<LQViewable> LQViewable::left() {
    return m_left;
}

LQLength<LQViewable> LQViewable::right() {
     return m_left + m_width;
}

LQLength<LQViewable> LQViewable::top() {
    return m_top;
}

LQLength<LQViewable> LQViewable::bottom() {
    return m_top + m_height;
}

LQLength<LQViewable> LQViewable::width() {
    return m_w.mock();
}

LQLength<LQViewable> LQViewable::height() {
    return m_h;
}

void LQViewable::linkToLengths() {
    m_left.linkToQuark(
        this,
        static_cast<LQGetterPointer<LQViewable, GLfloat>>(&LQViewable::getX),
        static_cast<LQSetterPointer<LQViewable, GLfloat>>(&LQViewable::moveToX));

    m_top.linkToQuark(
        this,
        static_cast<LQGetterPointer<LQViewable, GLfloat>>(&LQViewable::getY),
        static_cast<LQSetterPointer<LQViewable, GLfloat>>(&LQViewable::moveToY));

    m_w.linkToQuark(
        this,
        static_cast<LQGetterPointer<LQViewable, GLfloat>>(&LQViewable::getWidth),
        static_cast<LQSetterPointer<LQViewable, GLfloat>>(&LQViewable::resizeWidth));

    m_h.linkToQuark(
        this,
        static_cast<LQGetterPointer<LQViewable, GLfloat>>(&LQViewable::getHeight),
        static_cast<LQSetterPointer<LQViewable, GLfloat>>(&LQViewable::resizeHeight));
}
