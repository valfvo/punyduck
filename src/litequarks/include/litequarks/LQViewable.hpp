#pragma once

#include "LQMetric.hpp"
#include "LQSurface.hpp"

class LQViewable : public LQSurface {
public:
    LQViewable();
    LQViewable(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
    LQViewable(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLint color);

    LQMetricExpr<LQViewable> left();
    LQMetricExpr<LQViewable> right();
    LQMetricExpr<LQViewable> top();
    LQMetricExpr<LQViewable> bottom();

    LQMetricExpr<LQViewable> width();
    LQMetricExpr<LQViewable> height();
    // margin
    // padding-top

protected:
    void linkToLengths();

    LQMetric<LQViewable> m_left;
    LQMetric<LQViewable> m_top;
    LQMetric<LQViewable> m_w;
    LQMetric<LQViewable> m_h;
};
