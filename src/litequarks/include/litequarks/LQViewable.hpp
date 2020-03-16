#pragma once

#include "LQLength.hpp"
#include "LQSurface.hpp"

class LQViewable : public LQSurface {
public:
    LQViewable();
    LQViewable(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
    LQViewable(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLint color);

    LQLength<LQViewable> left();
    LQLength<LQViewable> right();
    LQLength<LQViewable> top();
    LQLength<LQViewable> bottom();

    LQLength<LQViewable> width();
    LQLength<LQViewable> height();
    // margin
    // padding-top

protected:
    void linkToLengths();

    LQLength<LQViewable> m_left;
    LQLength<LQViewable> m_top;
    LQLength<LQViewable> m_w;
    LQLength<LQViewable> m_h;
};
